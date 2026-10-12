/**
  Qualcomm SM8150 ResetSystem via PS_HOLD hardware register.

  Reference: Linux msm-poweroff.c + SM8150 DTS
  - PS_HOLD base: 0x0C264000 (restart@c264000)
  - IMEM restart reason: 0x146BF65C (imem@146bf000 + 0x65c)

  ResetCold() deasserts PS_HOLD to trigger PMIC hard reset.
  It does NOT write IMEM reason - callers must write IMEM before reset
  if they need a specific boot mode (fastboot/recovery/EDL).

  Copyright (c) 2024, Operit. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/ResetSystemLib.h>

#define SM8150_PS_HOLD  0x0C264000ULL

VOID
EFIAPI
ResetCold (VOID)
{
  DEBUG ((EFI_D_INFO, "QcomReset: PS_HOLD cold reset\n"));
  MmioWrite32 (SM8150_PS_HOLD, 0);
  CpuDeadLoop ();
}

VOID
EFIAPI
ResetWarm (VOID)
{
  ResetCold ();
}

VOID
EFIAPI
ResetShutdown (VOID)
{
  DEBUG ((EFI_D_INFO, "QcomReset: PS_HOLD shutdown\n"));
  MmioWrite32 (SM8150_PS_HOLD, 0);
  CpuDeadLoop ();
}

VOID
EFIAPI
ResetPlatformSpecific (
  IN UINTN  DataSize,
  IN VOID  *ResetData
  )
{
  ResetCold ();
}

VOID
EFIAPI
ResetSystem (
  IN EFI_RESET_TYPE  ResetType,
  IN EFI_STATUS      ResetStatus,
  IN UINTN           DataSize,
  IN VOID           *ResetData
  )
{
  switch (ResetType) {
    case EfiResetWarm:   ResetWarm ();   break;
    case EfiResetCold:   ResetCold ();   break;
    case EfiResetShutdown: ResetShutdown (); return;
    case EfiResetPlatformSpecific: ResetPlatformSpecific (DataSize, ResetData); return;
    default: return;
  }
}

EFI_STATUS
EFIAPI
QcomResetSystemLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  DEBUG ((EFI_D_INFO, "QcomResetSystemLib: PS_HOLD @ 0x%llx\n", SM8150_PS_HOLD));
  return EFI_SUCCESS;
}
