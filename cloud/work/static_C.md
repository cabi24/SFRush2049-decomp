# Static packet C — 2026-10-01

Scope is static code only. Baseline is 23/230 functions, 1,448/61,440 bytes. No locks, source ROM TUs, generated files, or coordinator state were modified by this worker.

Historical DB statuses are insufficient: the first six apparent corpus-zero targets (osPhysicalToVirtual, memcpy, strchr, guMtxIdentF, guMtxL2F, __osIdCheckSum) were already promoted according to `layout.derive()`. This packet instead selects genuine passthrough slots.

All six delivered full TUs in `cloud/work/static_C/` compile on Rocky using IDO 5.3, exactly `-g0 -O2 -mips2 -G 0 -non_shared`. Verification uses `tools/conveyor/jobs/scoring.score(target, candidate, stack_differences=True)` plus an exact raw ELF `.text` word comparison (including aligned object padding). Both are zero for all six. Exported target SHA-256 values and authoritative layout slots are recorded in `targets.json`; exact source hashes and results are in `verification.json`. Target object bytes are deliberately outside the repo, at `/tmp/<name>.target.o` on the Pi and `~/agents/C/scratch/static-20261001/<name>.target.o` on Rocky. Matching sources are historical `work/auto/<name>/matched.c`, with the one noted correction below.

| Function | Segment | Slot bytes | Strict score | Raw word difference |
|---|---|---:|---:|---:|
| osGetActiveQueue | 0xd260 | 16 | 0 | 0 |
| inflate_flush_window | 0x5610 | 28 | 0 | 0 |
| viDeadlinePassed | 0x1050 | 32 | 0 | 0 |
| osPiReleaseAccess | 0xe7c0 | 44 | 0 | 0 |
| osPiGetAccess | 0xe7c0 | 68 | 0 | 0 |
| osViGetFramebuffer | 0x83d0 | 64 | 0 | 0 |

`osViGetFramebuffer` is a historical false zero under stack-masked scoring: the original seed saves/loads its return value at `sp+0x1c`, but the target uses `sp+0x18`. Its initial strict score is 8, with two raw differing words. Reversing the local declaration order to declare `temp_a0` before `sp18` fixes both instructions without changing function behavior. This exact correction is in the delivered TU.

These are object matches, not a cartridge coverage claim. Coordinator acceptance must independently rescore, lock with the proven flags, use static promotion, and require full-ROM SHA-1. The potential newly promoted total is six functions / 252 slot bytes.

Recommended lock command per function:

```sh
python3 -m tools.conveyor.pipeline.lock add cloud/work/static_C/NAME.c:NAME --flags "-g0 -O2 -mips2 -G 0 -non_shared"
```

Recommended promotion command per corresponding table row (after any required segment conversion and header-context check):

```sh
python3 -m tools.conveyor.pipeline.promote run SEGMENT:NAME --from cloud/work/static_C/NAME.c --via-builder
```

The promotion tool extracts only the named function definition into the ROM TU, so its declarations/types must be provided by the shared `src/rom/rom_tu.h` context. Keep the delivered full TU for independent object verification; supply any missing additive declarations in that hand-authored shared header. Both lock and promote may mutate state; the worker did not invoke them. Promotion automatically commits on success.

Second bounded packet audits six additional unpromoted historical matches/seeds. Five strict/raw object matches add 756 slot bytes, bringing the ready packet to 11 functions / 1,008 slot bytes (potential coverage only). All retain the same flags. Evidence is in `targets_B.json` and `verification_B.json` (second packet).

| Function | Segment | Slot bytes | Strict score | Raw word difference |
|---|---|---:|---:|---:|
| osPiInit | 0xe7c0 | 80 | 0 | 0 |
| osSiInit | 0xf160 | 80 | 0 | 0 |
| osSetIntMask | 0x79a0 | 112 | 0 | 0 |
| osContStartReadData2 | 0xa330 | 132 | 0 | 0 |
| osViSetSpecialFeatures | 0x8920 | 352 | 0 | 0 |
| osPiRawReadWord — blocked | 0x8dd0 | 64 | 15 | 3 |

`osContStartReadData2` is another historical stack-masked false zero: seed saves/loads the return value at `sp+0x18`; target requires `sp+0x1c`. Swapping the `temp_v0` and `sp1C` declaration order moves it to the correct home. Initial strict score 8 / two raw differences becomes true zero.

`osPiRawReadWord` has both the same two-word stack-home false zero and a target-normalization blocker. The exported inventory object has `tier=raw_word`, with three absolute JAL instructions and no relocation records, while the correct candidate has named external relocations. Swapping the same declaration pair fixes the frame, reducing strict score 23→15 and raw differences 5→3. Resolving its three JAL relocations against existing `symbol_addrs.us.txt` addresses yields **exactly identical words**, with no masks, unresolved symbols, unverified references, or errors; evidence is `relocated_osPiRawReadWord.json`. This is a strict linked-word proof but cannot pass the ordinary lock object's current normalized comparison. Do not treat it as ready for lock/promotion without repairing the exported target relocation attribution through supported tools or establishing an approved integration path with full ROM SHA-1. Its source is supplied for investigation, not as a zero-object match.

The existing historical symbol names are misattributed in places: for example `osSetIntMask`'s body operates on VI context state. Preserve current target identity for this matching batch; no semantic renaming was performed.

Segment conversion requirements from the authoritative layout before coordinator changes: `0xd260`, `0xe7c0`, `0x83d0`, `0xf160`, `0x79a0`, `0xa330`, and `0x8920` are unconverted; `0x5610` and `0x1050` are already converted. Lock evidence supplies missing segment flag pins. Additive shared-header declarations needed for the first packet include `__osViModeInfo`, `gDisplayListEnd`, `gViAccumTime`, `gViTickCounter`, `__osPiInitialized`, and `__osPiMesgQueue`; `__osViContext` is already declared via `m2c_types.h`. Audit the corresponding SI globals for the second packet before compiling promoted bodies.


Independent x86 acceptance command after copying this directory and the target objects to a separate scratch directory:

```sh
python3 STATIC_DIR/verify.py --repo ~/agents/D/wt --toolkit ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 --target-dir TARGET_DIR --output D_VERIFIED.json
```

The generic verifier checks all recorded exact source/target SHA-256 values and the flags line before compiling, runs strict stack-sensitive scoring plus raw-word identity, excludes the blocked raw-word lead, and exits nonzero unless exactly 11 matches pass. Its own end-to-end run in C scratch exited 0. The normal `compile_score` pool job calls the scorer without `stack_differences=True`, so a normal lock job alone still masks stack-home false zeros; this separate evidence and the full-ROM gate are essential.

Required integration declarations are supplied in `static_C/rom_tu_declarations.txt`, without copying the full TU preludes into the report. No new types are necessary. Segment conversion and function promotion remain coordinator work. Worker used one serial compile process and left no running searches.
