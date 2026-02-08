# Launch GUI/CUI EXE before Win32k Load?!

I just tried that. Kernel will BSOD with c0000145

```
kd> !process 0 1
PROCESS ffffc1084b187080
    SessionId: none  Cid: 0170    Peb: fc8d9d8000  ParentCid: 0168
    DirBase: 1c665002  ObjectTable: ffffb001fad818c0  HandleCount:  18.
    Image: cmd.exe
    VadRoot ffffc1084b182f60 Vads 17 Clone 0 Private 79. Modified 0. Locked 0.
    DeviceMap ffffb001fa636190
    Token                             ffffb001fae020a0
    ElapsedTime                       00:00:00.670
    UserTime                          00:00:00.000
    KernelTime                        00:00:00.000
    QuotaPoolUsage[PagedPool]         9728
    QuotaPoolUsage[NonPagedPool]      2512
    Working Set Sizes (now,min,max)  (315, 50, 345) (1260KB, 200KB, 1380KB)
    PeakWorkingSetSize                332
    VirtualSize                       2101287 Mb
    PeakVirtualSize                   2101290 Mb
    PageFaultCount                    392
    MemoryPriority                    BACKGROUND
    BasePriority                      8
    CommitCharge                      621
```

```
kd> !process ffffc1084b187080 3
PROCESS ffffc1084b187080
    SessionId: none  Cid: 0170    Peb: fc8d9d8000  ParentCid: 0168
    DirBase: 1c665002  ObjectTable: ffffb001fad818c0  HandleCount:  18.
    Image: cmd.exe
    VadRoot ffffc1084b182f60 Vads 17 Clone 0 Private 79. Modified 0. Locked 0.
    DeviceMap ffffb001fa636190
    Token                             ffffb001fae020a0
    ElapsedTime                       00:00:00.670
    UserTime                          00:00:00.000
    KernelTime                        00:00:00.000
    QuotaPoolUsage[PagedPool]         9728
    QuotaPoolUsage[NonPagedPool]      2512
    Working Set Sizes (now,min,max)  (315, 50, 345) (1260KB, 200KB, 1380KB)
    PeakWorkingSetSize                332
    VirtualSize                       2101287 Mb
    PeakVirtualSize                   2101290 Mb
    PageFaultCount                    392
    MemoryPriority                    BACKGROUND
    BasePriority                      8
    CommitCharge                      621

        THREAD ffffc10849fd8080  Cid 0170.0174  Teb: 000000fc8d9d9000 Win32Thread: 0000000000000000 WAIT: (Suspended) KernelMode Alertable
            ffffc1084b0f1830  NotificationEvent

        THREAD ffffc10849fed040  Cid 0170.01b4  Teb: 000000fc8d9db000 Win32Thread: 0000000000000000 WAIT: (UserRequest) UserMode Non-Alertable
            ffffc1084b159ee0  NotificationEvent
```

```
kd> .thread /r /p ffffc10849fd8080
Implicit thread is now ffffc108`49fd8080
Implicit process is now ffffc108`4b187080
.cache forcedecodeuser done
Loading User Symbols
..
kd> kv
  *** Stack trace for last set context - .thread/.cxr resets it
 # Child-SP          RetAddr               : Args to Child                                                           : Call Site
00 ffff9888`50a05cd0 fffff806`4556ad95     : 00000000`fffffffe 00000000`00000008 00000000`00000000 ffff9888`50a06900 : nt!KiSwapContext+0x76
01 ffff9888`50a05e10 fffff806`4556bbfa     : ffffc108`00000000 00000000`00000000 ffffc108`49fd8100 00000000`00000000 : nt!KiSwapThread+0x545
02 ffff9888`50a05eb0 fffff806`4556c856     : 00000000`00000037 ffffc108`00000000 00000000`00000000 00000000`00000000 : nt!KiCommitThreadWait+0x15a
03 ffff9888`50a05f50 fffff806`459e9d5d     : ffffc108`4b0f1830 ffffb001`00000005 00000000`00000000 00000000`00000001 : nt!KeWaitForSingleObject+0x236
04 ffff9888`50a06040 fffff806`45635c35     : ffffc108`49fd8080 ffff9888`50a063d8 00000000`00000000 00000000`00000000 : nt!NtInitiatePowerAction+0x18d
05 ffff9888`50a060d0 fffff806`45628000     : fffff806`45b71a59 00000000`00000061 ffff9888`50a06328 ffffb001`fa7c0680 : nt!KiSystemServiceCopyEnd+0x25 (TrapFrame @ ffff9888`50a060d0)
06 ffff9888`50a06268 fffff806`45b71a59     : 00000000`00000061 ffff9888`50a06328 ffffb001`fa7c0680 00000000`000000c0 : nt!KiServiceLinkage
07 ffff9888`50a06270 fffff806`45c4dfd9     : ffffc108`4b159c90 ffffc108`4aff11d0 ffffc108`4aff1100 00000000`00000100 : nt!PoShutdownBugCheck+0xd9
08 ffff9888`50a06310 fffff806`4562dcc8     : 00000000`00000000 00000000`00000000 00000000`00000000 00000000`00000000 : nt!ExpSystemErrorHandler2+0x699
09 ffff9888`50a06570 fffff806`45aac069     : 00000000`c0000145 00000000`00000001 00000000`00000000 ffff9888`50a069a8 : nt!ExpSystemErrorHandler+0xd8
0a ffff9888`50a065b0 fffff806`4595c9fe     : 00000000`00000000 fffff806`4557a9fc 000000fc`8dafef50 ffff9888`50a06b60 : nt!ExpRaiseHardError+0x14f62d
0b ffff9888`50a06930 fffff806`45635c35     : f2c29d83`9242ca87 00000000`00000000 00000000`00000000 ffff9888`50a06a00 : nt!NtRaiseHardError+0x1de
0c ffff9888`50a06a70 00007ff9`1bef1c84     : 00007ff9`1be5108c 00007ff9`1bf7a910 00007fff`00000a21 ffffffff`c0000142 : nt!KiSystemServiceCopyEnd+0x25 (TrapFrame @ ffff9888`50a06ae0)
0d 000000fc`8daff038 00007ff9`1be5108c     : 00007ff9`1bf7a910 00007fff`00000a21 ffffffff`c0000142 00000000`00000000 : ntdll!NtRaiseHardError+0x14
0e 000000fc`8daff040 00007ff9`1bec820c     : 00007ff9`1bf7a910 00000000`000008f4 ffffffff`c0000142 00000000`00000001 : ntdll!LdrpInitializationFailure+0x84
0f 000000fc`8daff080 00007ff9`1beed90d     : 00000000`00000000 00007ff9`1be50000 000000fc`8d9d9000 000000fc`8d9d8050 : ntdll!LdrpInitialize+0x320
10 000000fc`8daff100 00007ff9`1bec7e5e     : 000000fc`8daff3d0 00000000`00000000 000000fc`8daff3d0 00000000`00000000 : ntdll!LdrpInitializeInternal+0xed
11 000000fc`8daff380 00000000`00000000     : 00000000`00000000 00000000`00000000 00000000`00000000 00000000`00000000 : ntdll!LdrInitializeThunk+0xe
```