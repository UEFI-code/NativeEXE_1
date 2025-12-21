## Research of Native Kernel APIs

### \Device\KeyboardClass0

**Object Information**

```
kd> !object \Device\KeyboardClass0
Object: ffff80061d5c7420  Type: (ffff80061c9bed20) Device
    ObjectHeader: ffff80061d5c73f0 (new version)
    HandleCount: 0  PointerCount: 3
    Directory Object: ffff910f1d62e490  Name: KeyboardClass0
```

```
kd> !devobj ffff80061d5c7420 2
Device object (ffff80061d5c7420) is for:
 KeyboardClass0 \Driver\kbdclass DriverObject ffff80061d5be9c0
Current Irp 00000000 RefCount 0 Type 0000000b Flags 00002044
SecurityDescriptor ffff910f1d772de0 DevExt ffff80061d5c7570 DevObjExt ffff80061d5c76f0 
ExtensionFlags (0x00000800)  DOE_DEFAULT_SD_PRESENT
Characteristics (0x00000100)  FILE_DEVICE_SECURE_OPEN
AttachedTo (Lower) ffff80061d5bebb0 \Driver\hyperkbd
Device queue is not busy.
```

```
kd> !drvobj ffff80061d5be9c0 2
Driver object (ffff80061d5be9c0) is for:
 \Driver\kbdclass

DriverEntry:   fffff80653d00010	kbdclass!GsDriverEntry
DriverStartIo: 00000000	
DriverUnload:  00000000	
AddDevice:     fffff80653cfd070	kbdclass!KeyboardAddDevice

Dispatch routines:
[00] IRP_MJ_CREATE                      fffff80653cf1fe0	kbdclass!KeyboardClassCreate
[01] IRP_MJ_CREATE_NAMED_PIPE           fffff806502e2eb0	nt!IopInvalidDeviceRequest
[02] IRP_MJ_CLOSE                       fffff80653cf1e70	kbdclass!KeyboardClassClose
[03] IRP_MJ_READ                        fffff80653cf1b00	kbdclass!KeyboardClassRead
[04] IRP_MJ_WRITE                       fffff806502e2eb0	nt!IopInvalidDeviceRequest
[05] IRP_MJ_QUERY_INFORMATION           fffff806502e2eb0	nt!IopInvalidDeviceRequest
[06] IRP_MJ_SET_INFORMATION             fffff806502e2eb0	nt!IopInvalidDeviceRequest
[07] IRP_MJ_QUERY_EA                    fffff806502e2eb0	nt!IopInvalidDeviceRequest
[08] IRP_MJ_SET_EA                      fffff806502e2eb0	nt!IopInvalidDeviceRequest
[09] IRP_MJ_FLUSH_BUFFERS               fffff80653cf55d0	kbdclass!KeyboardClassFlush
[0a] IRP_MJ_QUERY_VOLUME_INFORMATION    fffff806502e2eb0	nt!IopInvalidDeviceRequest
[0b] IRP_MJ_SET_VOLUME_INFORMATION      fffff806502e2eb0	nt!IopInvalidDeviceRequest
[0c] IRP_MJ_DIRECTORY_CONTROL           fffff806502e2eb0	nt!IopInvalidDeviceRequest
[0d] IRP_MJ_FILE_SYSTEM_CONTROL         fffff806502e2eb0	nt!IopInvalidDeviceRequest
[0e] IRP_MJ_DEVICE_CONTROL              fffff80653cfd650	kbdclass!KeyboardClassDeviceControl
[0f] IRP_MJ_INTERNAL_DEVICE_CONTROL     fffff80653cff720	kbdclass!KeyboardClassPassThrough
[10] IRP_MJ_SHUTDOWN                    fffff806502e2eb0	nt!IopInvalidDeviceRequest
[11] IRP_MJ_LOCK_CONTROL                fffff806502e2eb0	nt!IopInvalidDeviceRequest
[12] IRP_MJ_CLEANUP                     fffff80653cf3450	kbdclass!KeyboardClassCleanup
[13] IRP_MJ_CREATE_MAILSLOT             fffff806502e2eb0	nt!IopInvalidDeviceRequest
[14] IRP_MJ_QUERY_SECURITY              fffff806502e2eb0	nt!IopInvalidDeviceRequest
[15] IRP_MJ_SET_SECURITY                fffff806502e2eb0	nt!IopInvalidDeviceRequest
[16] IRP_MJ_POWER                       fffff80653cf25e0	kbdclass!KeyboardClassPower
[17] IRP_MJ_SYSTEM_CONTROL              fffff80653cfd3e0	kbdclass!KeyboardClassSystemControl
[18] IRP_MJ_DEVICE_CHANGE               fffff806502e2eb0	nt!IopInvalidDeviceRequest
[19] IRP_MJ_QUERY_QUOTA                 fffff806502e2eb0	nt!IopInvalidDeviceRequest
[1a] IRP_MJ_SET_QUOTA                   fffff806502e2eb0	nt!IopInvalidDeviceRequest
[1b] IRP_MJ_PNP                         fffff80653cf29c0	kbdclass!KeyboardPnP
```

**Learn from win32k**

`bp kbdclass!KeyboardClassCreate`

```
kd> k
 # Child-SP          RetAddr               Call Site
00 fffffe8c`2af24718 fffff806`50361ef5     kbdclass!KeyboardClassCreate
01 fffffe8c`2af24720 fffff806`5078f401     nt!IofCallDriver+0x55
02 fffffe8c`2af24760 fffff806`50793b85     nt!IopParseDevice+0x891
03 fffffe8c`2af24920 fffff806`50793021     nt!ObpLookupObjectName+0x625
04 fffffe8c`2af24ac0 fffff806`507cb27f     nt!ObOpenObjectByNameEx+0x1f1
05 fffffe8c`2af24bf0 fffff806`507cadc8     nt!IopCreateFile+0x40f
06 fffffe8c`2af24c90 fffff806`50435c35     nt!NtOpenFile+0x58
07 fffffe8c`2af24d20 fffff806`50428000     nt!KiSystemServiceCopyEnd+0x25
08 fffffe8c`2af24f28 ffffa1c4`d9483bc1     nt!KiServiceLinkage
09 fffffe8c`2af24f30 ffffa1c4`d94c3db2     win32kbase!RIMGetDeviceObjectPointer+0x9d
0a fffffe8c`2af25010 ffffa1c4`d94c3c8e     win32kbase!RIMRegOpenDeviceInstanceKey+0x42
0b fffffe8c`2af25070 ffffa1c4`d9452f16     win32kbase!RIMPopulateExtendedKeyboardDeviceProperties+0x26
0c fffffe8c`2af250b0 ffffa1c4`d94521a3     win32kbase!RIMCreateDev+0x2fa
0d fffffe8c`2af25120 fffff806`507c03df     win32kbase!RIMDeviceClassNotify+0x1d3
0e fffffe8c`2af25240 fffff806`5065098d     nt!PnpNotifyDriverCallback+0x97
0f fffffe8c`2af252f0 ffffa1c4`d94c23ad     nt!IoRegisterPlugPlayNotification+0x30d
10 fffffe8c`2af253b0 ffffa1c4`d945246e     win32kbase!RIMRegisterForDeviceClassNotifications+0x4d
11 fffffe8c`2af25400 ffffa1c4`d9453ee8     win32kbase!RIMDiscoverDevicesOfInputType+0xfa
12 fffffe8c`2af25450 ffffa1c4`d945703d     win32kbase!RIMRegisterForInputWithCallbacks+0x508
13 fffffe8c`2af25590 ffffa1c4`d945713f     win32kbase!RIMRegisterForInput+0x4d
14 fffffe8c`2af25600 ffffa1c4`d9457572     win32kbase!CRIMBase::CreateHandles+0xf3
15 fffffe8c`2af25650 ffffa1c4`da0389b3     win32kbase!CBaseInput::InitializeSensor+0x92
16 fffffe8c`2af256f0 ffffa1c4`da104357     win32kfull!xxxRegisterForDeviceClassNotifications+0x107
17 fffffe8c`2af25720 ffffa1c4`d94720b0     win32kfull!RawInputThread+0xad7
18 fffffe8c`2af25890 ffffa1c4`d9471e4a     win32kbase!xxxCreateSystemThreads+0x200
19 fffffe8c`2af259b0 ffffa1c4`d98aa77a     win32kbase!NtUserCreateSystemThreads+0x11a
1a fffffe8c`2af25ab0 fffff806`50435c35     win32k!NtUserCreateSystemThreads+0x16
1b fffffe8c`2af25ae0 00007ffe`a91e89a4     nt!KiSystemServiceCopyEnd+0x25
1c 00000079`436ffb78 00007ffe`a8d738ec     0x00007ffe`a91e89a4
1d 00000079`436ffb80 00000000`00000000     0x00007ffe`a8d738ec
```

```
kd> r
rax=fffff80653cf1fe0 rbx=ffff80061d3bedb0 rcx=ffff80061d5c7420
rdx=ffff80061d8e9cb0 rsi=ffff80061d8e9cb0 rdi=ffff80061e268b60
rip=fffff80653cf1fe0 rsp=fffffe8c2af24718 rbp=fffffe8c2af24a20
 r8=0000000000000000  r9=ffff80061d5c7420 r10=fffff80653cf1fe0
r11=0000000000000000 r12=ffff80061e268bf8 r13=ffff80061d8e9ea0
r14=ffff80061ce7ca20 r15=ffff80061d5c7420
```

Well, the parameter of `KeyboardClassCreate` seems too complex.

Try to break on `NtOpenFile` instead.

```
kd> r rdx
rdx=0000000000000080

```

```
kd> dt nt!_OBJECT_ATTRIBUTES %r8
   +0x000 Length           : 0x30
   +0x008 RootDirectory    : (null) 
   +0x010 ObjectName       : 0xffff9684`9b8f5590 _UNICODE_STRING "\Device\HarddiskVolume3"
   +0x018 Attributes       : 0x240
   +0x020 SecurityDescriptor : (null) 
   +0x028 SecurityQualityOfService : (null) 
```