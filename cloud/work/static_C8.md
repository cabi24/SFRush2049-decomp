# Static packet C8 — frozen

Eight strict stack-sensitive score-zero and raw-word-zero standalone TUs, 1,544 total static slot bytes. Canonical SDK extraction and explicit symbol/type audits produced all eight with eight baseline compiles, five compile-context corrections and one O1 baseline. No formatting sweeps or source/layout/state integration changes.

| Current symbol | Segment | Slot bytes | Flags optimization | Actual SDK body |
|---|---|---:|---|---|
| osPiReadWord | 0xe7c0 | 80 | O2 | __osPiRawReadIo, io/pirawread.c |
| osContStartReadData | 0xf160 | 208 | O2 | __osPfsGetStatus, io/pfsgetstatus.c, VERSION_J |
| __osPfsSelectBank | 0xf450 | 128 | O2 | io/pfsselectbank.c, VERSION_J |
| __osContBuildPacket | 0xa050 | 348 | O2 | osContInit, io/controller.c |
| __osPiRawStartDma | 0x9230 | 192 | O2 | osPiStartDma, io/pidma.c |
| osPfsGetFileStat | 0xb9f0 | 212 | O2 | __osPfsGetNextPage, io/pfsreadwritefile.c |
| osPfsFindFile | 0xb300 | 152 | O2 | __osPfsReleasePages, io/pfsdeletefile.c, VERSION_J |
| osCreateViManager | 0x8e10 | 224 | O1 | osSetThreadPri, os/setthreadpri.c |

Exact flags, source and target SHA-256 identities appear in `static_C8/verification.json`; common flags `-g0 -mips2 -G 0 -non_shared`, optimization above. No floating-point body is in C8. Current supported target exports and authoritative layout snapshot are in targets.json. Private target and candidate objects: Rocky `~/agents/C/scratch/static-C8/NAME.target.o` and NAME.o. Sources are frozen.

The generic standalone verifier enforces eight strict/raw zeros and exact hashes/first flags lines; exit code 0 on C. Independent invocation after copying the packet and private targets into D scratch:

```
python3 ~/agents/D/scratch/static-C8/verify.py --repo ~/agents/D/wt --toolkit ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 --target-dir ~/agents/D/scratch/static-C8 --output ~/agents/D/scratch/static-C8/independent.json
```

`retail_verification.json` additionally uses verified image symbols and direct relocated retail words: every linked prefix equals its authoritative slot with no masks, unresolved names, unverified addresses or errors. Standalone assembler alignment creates one extra zero word for __osContBuildPacket and three for osPfsGetFileStat; osPfsFindFile lacks the slot's two trailing zero words. These are explicitly recorded, not masked or called whole-slot equality. Parent's slot splice/padding and full-TU/full-ROM gates remain acceptance authority.

`rom_tu_declarations.txt` supplies minimal context and required prototype corrections. osContStartReadData is TWO-argument s32(OSMesgQueue*,int), osPfsFindFile is FIVE-argument s32(OSPfs*,__OSInode*,u8,u8,__OSInodeUnit*), and osPiReadWord is s32(u32,u32*). Their existing declarations conflict. __osPiRawStartDma's typed OSIoMesg* first argument also conflicts with the m2c void* declaration; reconcile through the established parent full-caller/full-ROM proof before promotion. osCreateViManager already has the proven two-argument thread/priority prototype; its historical public name is misleading.

Real target semantic mappings retained: __osPiMgrState.flag is SDK __osPiDevMgr.active; __osInsertTimer is SDK osPiGetCmdQueue; osSendMesg/osJamMesg are swapped relative to SDK names, as independently proved C7. In osContInit, SDK initialization flag is current __osContPifBuf, EEPROM timer queue/message are current __osContQueryMsgQ/__osContQueryMesg, and controller count/last command use current __osPfsRequestType2/Type. PI MMIO uses its real KSEG1 literal addresses. Thread scheduling's private historical prelude declares __osActiveQueue as pointer-to-pointer, so its delivered body explicitly casts the loaded value to OSThread* and its address to OSThread**. The current shared declaration was correctly changed in C4 to OSThread*; the same explicit casts remain consistent with that declaration and reproduce both authoritative target usages.

Recommended acceptance: lock each exact literal flags line using supported APIs, reconcile necessary prototypes without changing prior locked code, verify existing shared pins (O2 for e7c0/f160/a050/b300; currently unconverted 8e10 can pin O1), then standard clean-tree prepcommit, full-TU byte proof and ROM/lock gates. No mixed-flag override is requested.


Coordinator compatibility correction: retain the already-promoted `s32 __osInsertTimer(void)` body and declaration in lib_e9a0. The new PI DMA caller explicitly casts its pointer-valued result to OSMesgQueue* at both message calls. Current source hash is recorded in verification.json and coordinator_compatibility.json, superseding the original standalone hash for that one function. Independent D reran all eight objects (strict/stack-sensitive and raw0), then pool relocked the changed body at score0. B11 independently rebuilt all eight bodies against the actual shared header snapshot and obtained strict/raw0; full-TU/ROM gates still decide acceptance. Earlier retail prefix evidence describes the original object; identical relocated instruction/relocation proof and subsequent full ROM establish the compatible caller. Redundant shared-header macros were omitted, and the old accepted integer-valued PiRawReadWord caller remains unchanged pending its full-ROM proof.
