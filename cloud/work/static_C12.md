# Static C12 frozen five-function packet

Current root locks/layout selected eight actual passthroughs. Five are ready, 4,376 logical bytes. No shared sources, layout, lock/state, or generated files modified.

| Target | Segment | SDK body | Flags | Strict/raw | Linked retail prefix |
|---|---|---|---|---|---|
| osPfsReadWriteFile | b570 | osPfsAllocateFile | O2 | 0/0 | 0 |
| osPfsGetFileSize | b9f0 | osPfsReadWriteFile | O2 | 0/0 | 0 |
| osPfsChecker | bfe0 | osPfsChecker | O2 | 0/0 | 0 |
| __osRepairId | f700 | __osRepairPackId | O2 | 0/0 | 0 |
| osPiSetDeviceTiming | e9a0 | __osEPiRawStartDma | O2 | 0/0 | 0 |

Exact flags for all five: `-g0 -O2 -mips2 -G 0 -non_shared`. Main `verification.json` records immutable source and current target object hashes. `targets.json` includes old/current metadata after coordinator's supported refreshes. Repair and timing changed from raw/no_asm_region to reloc_aware/gate null. No scorer changes or masks used for these five.

All five independently compiled against a snapshot of CURRENT root include tree and rom_tu.h, with the minimal private proposal in `minimal_declarations.txt`; `shared_header_verification.json` records strict/raw zeros. Only existing declaration changed privately is osPfsReadWriteFile: actual target is allocator, seven arguments with u16 company/u32 game. `accepted_dependency_audit.json` finds no accepted current bodies depending on this declaration. New declarations and local canonical PI macros captured in shared_header_sources/context.h. B16 separately auditing.

`retail_verification.json`: all five linked words agree, no unresolved/unverified relocations, no errors or masks. ELF section alignment adds 3/1/2 zero words to allocator/readwrite/checker respectively; their logical slot lengths are205/223/334 words. Repair212 and timing120 exactly span slots. Whole-TU/ROM proof remains final authority for alignment; no padding source tricks.

Private target objects: Rocky `~/agents/C/scratch/static-C12/<target>.target.o`. Full TUs: cloud/work/static_C12/<target>.c. Independent invocation on Rocky or D scratch:

```
python3 verify.py --repo ~/agents/C/wt --toolkit ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 --target-dir ~/agents/C/scratch/static-C12
```

Recommended root lock: `python3 -m tools.conveyor.cli lock --help` / established pool lock workflow using each exact TU and named target; then established clean-tree promote and forced full-ROM gate. Worker does not guess alternate CLI arguments or mutate integration state.

Remaining three are explicitly unmatched. VI creator osSetEventMesg O2 strict495/raw39; VI worker O2 strict2429/raw91 under tested volatile counter control; O1 worse. PI device-manager osSpTaskLoad_full O2 strict1315/raw175 after refresh; current local switch table references unresolved provenance in linked standalone proof, so no linked claim. Continue bounded directed controls separately; frozen five sources will remain unchanged.

Corpus changes are actual symbol-name mapping, SDK J/_FINALROM condition selection, canonical __OSDir layout, and SDK macro/type definitions; no formatting sweeps. Four PFS bodies matched baseline once canonical declarations/macros matched. Timing was canonical before supported refresh (old raw score15/three relocation words).

## Alternate flags audit (frozen separately)

`shared_flags/` verifies unchanged accepted neighbor bodies with recorded normalized body hashes, current header snapshot and current target object hashes. Accepted osViSetMode O2 baseline strict/raw0 against supported-refreshed reloc_aware/gate-null target; alternate O1 strict646/raw16. Accepted osCreateViManager O1 baseline strict/raw0, but alternate O2 strict3149/raw55. Neither existing segment can legitimately change pin by recompiling these accepted bodies unchanged. C7 osSendMesg and C10 osCreatePiManager remain blocked by incompatible segment flags. No pin or source mutations.
