APRIORI DXE {
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/crux/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/crux/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/crux/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/PdcDxe/PdcDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/crux/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/crux/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf

  INF Binaries/crux/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  INF Binaries/crux/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf

  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf

  INF Binaries/crux/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/crux/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf

  INF Binaries/crux/QcomPkg/Drivers/PILDxe/PILDxe.inf
!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/crux/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/crux/QcomPkg/Drivers/FontDxe/FontDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/crux/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/crux/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/crux/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/CipherDxe/CipherDxe.inf
  INF Binaries/crux/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/crux/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
}
