/** @file
  CruxDumpDxe - 在 Mu（UEFI）阶段把 ramoops 保留区 0xB0000000 + 4MB 抓下来写进 ESP。

  目的：内核 pstore 依赖内存跨复位保活，实测复位后内存出现大量位翻转，
  panic 日志无法读出。此驱动在 OS 启动前直接读物理内存并落盘到
  \EFI\crux\RAMDUMP.BIN，用于判断"破坏者是谁"（XBL/SBL 还是 Mu 自己），
  同时作为原厂式 reset-time dump 的第一步。

  只读内存、只写 ESP 文件，不改任何固件状态。
**/

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Protocol/SimpleFileSystem.h>

#define RM_BASE     0x0B000000ULL
#define RM_SIZE     0x00400000U
#define PRZ_MAGIC   0x43474244U   /* "DBGC" */

STATIC
VOID
ScreenPrint (
  IN CONST CHAR16  *Str
  )
{
  if (gST != NULL && gST->ConOut != NULL && Str != NULL) {
    gST->ConOut->OutputString (gST->ConOut, (CHAR16 *)Str);
  }
}

/**
  在 ESP 上写文件（覆盖）。用 \EFI\crux\Image87 是否存在判定目标卷。
**/
STATIC
EFI_STATUS
WriteFileToEsp (
  IN CHAR16  *FileName,
  IN VOID    *Data,
  IN UINTN   Size
  )
{
  EFI_HANDLE                       *Handles;
  UINTN                            Count;
  UINTN                            i;
  EFI_STATUS                       Status;

  Handles = NULL;
  Count   = 0;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleFileSystemProtocolGuid,
                  NULL,
                  &Count,
                  &Handles
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (i = 0; i < Count; i++) {
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *Fs;
    EFI_FILE_PROTOCOL                *Root;
    EFI_FILE_PROTOCOL                *Probe;
    EFI_FILE_PROTOCOL                *File;
    UINTN                            Off;

    Fs    = NULL;
    Root  = NULL;
    Probe = NULL;
    File  = NULL;

    Status = gBS->HandleProtocol (Handles[i], &gEfiSimpleFileSystemProtocolGuid, (VOID **)&Fs);
    if (EFI_ERROR (Status) || (Fs == NULL)) {
      continue;
    }

    Status = Fs->OpenVolume (Fs, &Root);
    if (EFI_ERROR (Status) || (Root == NULL)) {
      continue;
    }

    /* 判定是不是 crux 的 ESP */
    Status = Root->Open (Root, &Probe, L"\\EFI\\crux\\Image87", EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR (Status)) {
      Root->Close (Root);
      continue;
    }
    Probe->Close (Probe);

    Status = Root->Open (
                     Root,
                     &File,
                     FileName,
                     EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE,
                     0
                     );
    if (!EFI_ERROR (Status)) {
      File->SetPosition (File, 0);
      Off = 0;
      while (Off < Size) {
        UINTN Chunk;

        Chunk  = Size - Off;
        if (Chunk > 0x100000) {
          Chunk = 0x100000;
        }
        Status = File->Write (File, &Chunk, (UINT8 *)Data + Off);
        if (EFI_ERROR (Status) || (Chunk == 0)) {
          break;
        }
        Off += Chunk;
      }
      File->Flush (File);
      File->Close (File);
    }

    Root->Close (Root);
    FreePool (Handles);
    return Status;
  }

  FreePool (Handles);
  return EFI_NOT_FOUND;
}

STATIC
VOID
AsciiAppend (
  IN OUT CHAR8   *Buf,
  IN     UINTN   BufSize,
  IN     CHAR8   *Fmt,
  ...
  )
{
  VA_LIST  Marker;
  UINTN    Len;

  Len = AsciiStrLen (Buf);
  if (Len + 1 >= BufSize) {
    return;
  }

  VA_START (Marker, Fmt);
  AsciiVSPrint (Buf + Len, BufSize - Len, Fmt, Marker);
  VA_END (Marker);
}

/**
  把一段内存里的字节转成可打印 ascii 追加到 Buf。
**/
STATIC
VOID
AsciiAppendDump (
  IN OUT CHAR8   *Buf,
  IN     UINTN   BufSize,
  IN     CONST UINT8  *Mem,
  IN     UINTN   Len
  )
{
  UINTN  i;
  CHAR8  Chunk[3];

  Chunk[2] = 0;
  for (i = 0; i < Len; i++) {
    UINT8 c = Mem[i];
    if ((c >= 0x20) && (c < 0x7F)) {
      Chunk[0] = (CHAR8)c;
    } else if (c == '\n') {
      Chunk[0] = '|';
    } else {
      Chunk[0] = '.';
    }
    Chunk[1] = 0;
    AsciiAppend (Buf, BufSize, "%a", Chunk);
  }
  AsciiAppend (Buf, BufSize, "\r\n");
}

EFI_STATUS
EFIAPI
CruxDumpEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  UINT8           *Base;
  UINTN           i;
  UINTN           Hits;
  UINTN           Offs[8];
  UINT32          Magic;
  CHAR8           *Txt;
  EFI_STATUS      Status;
  EFI_TIME        Now;

  Base = (UINT8 *)(UINTN)RM_BASE;
  Hits = 0;
  ZeroMem (Offs, sizeof (Offs));

  ScreenPrint (L"[crux] ramdump probe: scanning 0xB0000000 (4MB)...\r\n");

  for (i = 0; i + 4 < RM_SIZE; i += 4) {
    CopyMem (&Magic, Base + i, 4);
    if (Magic != PRZ_MAGIC) {
      continue;
    }
    if (Hits < 8) {
      Offs[Hits] = i;
    }
    Hits++;
  }

  Txt = AllocateZeroPool (8192);
  if (Txt == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem (&Now, sizeof (Now));
  gRT->GetTime (&Now, NULL);

  AsciiAppend (Txt, 8192, "crux ramdump probe (Mu phase)\r\n");
  AsciiAppend (Txt, 8192, "time: %04d-%02d-%02d %02d:%02d:%02d\r\n",
               Now.Year, Now.Month, Now.Day, Now.Hour, Now.Minute, Now.Second);
  AsciiAppend (Txt, 8192, "base=0xB0000000 size=0x400000 dbgc_hits=%lu\r\n", Hits);

  for (i = 0; i < Hits && i < 8; i++) {
    UINT32  Start;
    UINT32  Size;
    UINT32  TimeLow;
    UINT32  TimeHigh;

    CopyMem (&Start,    Base + Offs[i] + 4,  4);
    CopyMem (&Size,     Base + Offs[i] + 12, 4);
    CopyMem (&TimeLow,  Base + Offs[i] + 20, 4);
    CopyMem (&TimeHigh, Base + Offs[i] + 24, 4);
    AsciiAppend (Txt, 8192, "hit[%lu] off=0x%lx start=%lu size=%lu time=%lu%lu\r\n",
                 i, Offs[i], Start, Size, TimeHigh, TimeLow);
  }

  /* 各区域抽样内容，直接看有没有 kernel 文本残留 */
  AsciiAppend (Txt, 8192, "--- dump region 0x000000 ---\r\n");
  AsciiAppendDump (Txt, 8192, Base, 128);
  AsciiAppend (Txt, 8192, "--- console region 0x100000 ---\r\n");
  AsciiAppendDump (Txt, 8192, Base + 0x100000, 192);
  AsciiAppend (Txt, 8192, "--- console region 0x115000 (上次关机日志处) ---\r\n");
  AsciiAppendDump (Txt, 8192, Base + 0x115000, 320);
  AsciiAppend (Txt, 8192, "--- pmsg region 0x300000 ---\r\n");
  AsciiAppendDump (Txt, 8192, Base + 0x300000, 128);

  Status = WriteFileToEsp (L"\\EFI\\crux\\RAMDUMP.TXT", Txt, AsciiStrLen (Txt));
  if (EFI_ERROR (Status)) {
    ScreenPrint (L"[crux] RAMDUMP.TXT FAILED\r\n");
  } else {
    ScreenPrint (L"[crux] RAMDUMP.TXT written\r\n");
  }

  if (Hits > 0) {
    Status = WriteFileToEsp (L"\\EFI\\crux\\RAMDUMP.BIN", Base, RM_SIZE);
    if (EFI_ERROR (Status)) {
      ScreenPrint (L"[crux] RAMDUMP.BIN FAILED\r\n");
    } else {
      ScreenPrint (L"[crux] RAMDUMP.BIN written (4MB)\r\n");
    }
  } else {
    ScreenPrint (L"[crux] no DBGC magic, skip BIN\r\n");
  }

  FreePool (Txt);
  return EFI_SUCCESS;
}