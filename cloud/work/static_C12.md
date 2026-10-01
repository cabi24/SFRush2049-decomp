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

## Additional PI manager packet (frozen separately)

`pi_manager/` contains osSpTaskLoad_full, actual SDK __osDevMgrMain, strict/raw0 at existing e9a0 O2 pin, 1,040 bytes. A miscopied SDK constant LEO_BM_CTL_CLR_MECHANIC_INTR was corrected to canonical0x01000000; no other structural repair. Body also strict/raw0 against current root header snapshot with local canonical disk-handle/typed-message overlays, preserving public OSPiHandle and OSIoMesg layouts/declarations. `context.h` is minimal private extra type/macros context; current_header_body.c uses identical body text and provides explicit field provenance via canonical offsets.

Its switch references local `.rodata`, which generic score.py honestly leaves unverified. Separate GNU MIPS linker proof binds actual candidate .text at target0x8000DF80 and .rodata at authoritative jtbl_8002D860_main address0x8002D860 (target assembly explicit original words and named %hi/%lo). Undefined symbols resolved individually using verified image symbol manifest, no ignored symbols. All1,040 text bytes and all28 actual seven-entry table bytes exactly retail hashes;32-byte aligned readonly section has four extra zero bytes. `linked_verification.json` + `retail_linker.ld` retain binding and artifact identities, no masks or skipped table comparison. Whole-TU/ROM gate must genuinely preserve local table placement; status `ready_pending_actual_rodata_placement_gate` explicitly records that integration requirement. This supplement does not change frozen five-function main packet or its verifier count.

## VI controls (blocked; frozen separately)

`controls/osSetEventMesg_stackfield.c` preserves canonical SDK stack endpoint using explicit existing thread-address displacement: gViMgrThreadArg0x800354F0+0x11B0=queue0x800366A0. Strict10/raw1 is solely differing relocation origin/addend; direct full96-word linked comparison is exact with no masks/unverified/errors. B17 independently audits authoritative typed SDK thread1B0+VIstack1000 versus current host header's incomplete threadsizeof; no fake sizeof assumption. No target normalization performed by this worker.

`controls/vi_manager_main_local.c` preserves original SDK function-local static u16 counter, yielding strict/raw0; globalscope static, extern, volatile, array, localextern controls differ. Actual candidate .bss0 lacks verified original physical0x80036700 binding: ten linked counter relocations unverified. This remains blocked, despite raw zero. Do not assign arbitrary .bss addresses or count it until source/image section placement is proven and preserved.

Coordinator acceptance: all five main candidates are ROM-promoted. EPi first attempt failed compilation because root placed its local macros after the promotion slot; rollback restored the exact ROM. Moving the same block before its slot, removing redundant extern, and re-gating baseline allowed the unchanged body to pass. Supplemental PI manager and VI storage models remain unaccepted pending genuine ownership integration.
