# Static packet C9 — frozen

Eight strict stack-sensitive score-zero and raw-word-zero standalone TUs, 2,684 total static slot bytes. All use exact flags `-g0 -O2 -mips2 -G 0 -non_shared`. No floating-point function or giant formatting sweep is in this packet. Canonical SDK bodies plus current-symbol/type/macro corrections suffice; only one compile-context repair was needed after baseline.

| Current symbol | Segment | Slot bytes | Actual SDK body |
|---|---|---:|---|
| osPiStartDma | 0xe8d0 | 208 | __osPiRawStartDma, io/pirawdma.c |
| __osCheckId | 0xf700 | 356 | __osCheckPackId, io/contpfs.c |
| __osGetId | 0xf700 | 428 | io/contpfs.c |
| osPfsReadWriteFile_pages | 0xf700 | 212 | __osCheckId, io/contpfs.c |
| __osPfsDeclearPage | 0xb570 | 332 | io/pfsallocatefile.c |
| osPfsInitPak | 0xa810 | 520 | io/pfsinitpak.c |
| osMotorInit | 0xab20 | 360 | __osMotorAccess, io/motor.c |
| __osMotorAccess | 0xab20 | 268 | __osMakeMotorData, io/motor.c |

Selected VERSION_J, non-debug branches from local canonical SDK. The two-argument target __osCheckId is identified by its [1,3,4,6] alternate ID-block array and checksum-copy loop; it is SDK __osCheckPackId. The earlier tentative selection guess of repair was discarded before generating sources. SDK repair calls use existing __osRepairId. Motor packet buffer uses its authoritative existing __osMotorPifBuf symbol; no fresh private/global stand-ins.

Three converted f700 target objects initially remained raw_word/no_asm_region, with only four/eight/five absolute JAL mismatches. Parent performed supported scoped target refresh, preserving canonical source files. Current exported metadata is reloc-aware/no fallback; all three now strict/raw zero. `targets.json` and `verification.json` contain exact source/current-target SHA-256 identities and flags. Private targets/candidate objects are frozen on Rocky in `~/agents/C/scratch/static-C9/`.

Independent verifier requires all eight strict/raw zeros, exact hashes and first-line flags; C invocation exit code 0. Copy packet and private target objects to D and run:

```
python3 ~/agents/D/scratch/static-C9/verify.py --repo ~/agents/D/wt --toolkit ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 --target-dir ~/agents/D/scratch/static-C9 --output ~/agents/D/scratch/static-C9/independent.json
```

`retail_verification.json` proves each candidate's actual instruction prefix matches retail after relocation via verified image symbols, with no masks/unresolved/unverified/errors. Standalone ELF alignment is explicit: extra zero bytes CheckId12/GetId4/pages12/InitPak8/MotorInit8/MotorAccess4; DeclearPage lacks twelve trailing retail-zero bytes. osPiStartDma covers its full slot. These are honestly recorded size differences, leaving parent slot padding/full-TU/full-ROM proof as acceptance authority.

`rom_tu_declarations.txt` supplies only required context and prototype audits. Public osPiStartDma currently has the seven-argument SDK manager signature, but this target takes four arguments (direction/devAddr/dramAddr/size). Public osMotorInit currently has the three-argument SDK initialization signature, but this target takes two (OSPfs*/flag). __osMotorAccess is void(int,OSPifRam*), and __osCheckId is s32(OSPfs*,__OSPackId*). Parent should inspect generated rom_auto.h prototypes as well and preserve existing locked caller bytes through the full-TU/ROM gate. C8's corrected two-argument osContStartReadData remains needed by osPfsInitPak; SDK check-RAM helper maps to current __osTimerInterrupt(OSPfs*).

Recommended acceptance: supported lock/prepcommit/promote sequence using exact literal O2 flags, existing f700 O2 pin, clean-tree checks and mandatory ROM/lock gates. No mixed flag override. Worker changed only its packet artifacts and C8 report correction; no source/layout/state/lock changes.
