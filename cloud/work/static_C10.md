# Static packet C10 — frozen

Seven eligible strict stack-sensitive score-zero and raw-word-zero standalone TUs, 2,780 total static slot bytes. Selection used CURRENT shared root layout and matched.lock.json, excluding all previous packets including pending C9. Six ready bodies use O2; osSetEventMesgAlt uses O1. Exact common flags: `-g0 -mips2 -G 0 -non_shared`, with optimization shown below. No float body, assembly stub or broad formatting sweep.

| Current symbol | Segment | Slot bytes | Optimization | Actual canonical SDK body |
|---|---|---:|---|---|
| __osTimerInterrupt | 0xa810 | 264 | O2 | __osPfsCheckRamArea, io/pfsinitpak.c |
| osMotorStart | 0xab20 | 348 | O2 | osMotorInit, io/motor.c |
| osPfsAllocate | 0xaf50 | 464 | O2 | osPfsFindFile, io/pfssearchfile.c |
| osPfsDeleteFile | 0xb120 | 480 | O2 | osPfsFileState, io/pfsfilestate.c |
| osPfsRename | 0xb300 | 472 | O2 | osPfsDeleteFile, io/pfsdeletefile.c |
| __osContRamRead | 0xf4d0 | 560 | O2 | io/contramread.c |
| osSetEventMesgAlt | 0x7a10 | 192 | O1 | osSetEventMesg, os/seteventmesg.c |

Canonical VERSION_J/non-debug SDK bodies with actual named globals/helpers. __osContDataCrc in the controller-read body is current __osPfsDataChecksum; last-channel is current __osContLastChannel. Motor initializer invokes current __osMotorAccess packet builder and __osMotorPifBuf. Event setter uses current gEventTypeFlag for SDK __osPreNMI and osJamMesg for SDK append-to-queue semantics (names swapped, C7 proof).

The current shared __OSDir in include/PR/os_pfs.h has wrong field ordering and signed name bytes for these proven PFS bodies. Canonical layout preserves size32 but requires data_sum@0xA, u8 ext_name[4]@0xC, u8 game_name[16]@0x10 (versus current data_sum@0x1E, ext_name@0x1A, game_name@0x0A). The private standalone sources contain that exact SDK type and strict/linked proof. `rom_tu_declarations.txt` gives the required replacement and exact prototype/context audits; parent must preserve existing accepted caller/TU bytes and ROM before promoting.

osCreatePiManager is an honest blocked eighth zero: canonical PI manager body matches exactly under O2, but current shared 0x8e10 is pinned O1 by accepted osCreateViManager. The O1 control remains structurally different; see shared_flags_blocker.json. It is excluded from generic verifier and must not be promoted with an override. My initial selection guess of destroy-thread semantics was corrected by reading actual current assembly before generating the manager body. Its context uses real named manager/thread/queue/function symbols, no invented global stand-ins.

Exact literal flags, frozen source/target SHA-256 identities and integration statuses are in verification.json; current target metadata/layout snapshots are in targets.json. No target normalization was needed. Private target/candidate objects are frozen on Rocky in `~/agents/C/scratch/static-C10/`. Generic verifier checks all seven eligible strict/raw zeros and exact hash/flag identities; exit code0 on C. Independent invocation after copying packet and private targets into D:

```
python3 ~/agents/D/scratch/static-C10/verify.py --repo ~/agents/D/wt --toolkit ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 --target-dir ~/agents/D/scratch/static-C10 --output ~/agents/D/scratch/static-C10/independent.json
```

`retail_verification.json` proves every instruction prefix matches retail through verified image symbols, with no masks/unresolved/unverified/errors. Standalone ELF alignment has extra zero bytes only: __osTimerInterrupt8, osMotorStart4, osPfsRename8. Other ready bodies cover exact full slot lengths. Parent's padding/full-TU/ROM gates remain authority.

Public signatures needing replacement: osPfsAllocate takes six FindFile args; osPfsDeleteFile takes three FileState args; osMotorStart takes three MotorInit args. osPfsRename takes five DeleteFile args. Event entries are eight bytes (__OSEventState with queue/message); global declaration conflicts must be inspected before adding context. Preserve existing __osPfsDataChecksum locked return type if one exists, with independently proven explicit casts if needed, following C8 __osInsertTimer precedent.

Recommended acceptance is existing supported lock/prepcommit/promote flow with exact flags, shared pins, clean-tree checks and mandatory ROM/lock gates. Worker modified only packet artifacts; no source/layout/state/lock integration changes.
