# B16: read-only C12 integration audit

All five ready C12 bodies independently compile against the frozen current root shared headers plus minimal corrected context, with strict stack-sensitive score zero and raw word difference zero. Ten accepted dependencies independently retain strict/raw zero before and after. All 63 current ROM translation units containing promoted C compile successfully before and after with unchanged text words. The added prototype does not require any accepted caller-body casts or adaptations.

No shared source, header, assembly, layout, lock, build state or match delivery was changed. Only source/proof artifacts in `cloud/work/integration_B16/` and this report are deliverables; private compiler objects and archives remain ignored. Root still owns grouped integration, final relocation resolution, lock checks and full-ROM acceptance.

## Required shared context

Replace the current six-argument SDK declaration of `osPfsReadWriteFile` in `include/PR/os_pfs.h` with `s32 osPfsReadWriteFile(OSPfs *, u16, u32, u8 *, u8 *, int, s32 *)`. The linked target at 0x8000A970 is historically mislabeled: its actual body allocates a file. The C12 body uses exactly this seven-argument signature. No accepted ROM or blob body calls this label; repeated six-argument declarations in blob standalone preludes are independent of the shared header and their files do not include the shared context. Do not rewrite those frozen preludes. Legacy `src/game/save.c` and `src/game/game.c` contain six-argument calls but are excluded from the actual linked object list.

Add `s32 __osPfsDeclearPage(OSPfs *, __OSInode *, int, int *, u8, int *, int *)`, matching the accepted definition at `src/rom/lib_b570.c:14`. That declaration is currently absent from shared headers. Add `extern OSPiHandle *__osPiDevList[2]`, matching the two-domain pointer table used by the new PI implementation. Existing `__osRepairId`, `osPfsChecker`, PFS cache/page structures, and other helper declarations already agree.

`minimal.h` freezes only missing macros and declarations. It omits existing PFS_CHECK_STATUS, PFS_MAX_BANKS, PFS_ERR_EXIST/BAD_DATA/CORRUPTED, PFS_PAGE_NOT_USED, CHECK_IPAGE, OS_READ/WRITE, IO_READ/WRITE, PI_DOMAIN1 and K1_TO_PHYS rather than duplicating them. The new constants are PFS_CHECK_ID, ROUND_UP_DIVIDE, PFS_DATA_FULL, PFS_DIR_FULL, DIR_STATUS_OCCUPIED/EMPTY; the PI additions are status/register constants and the canonical WAIT_ON_IOBUSY, UPDATE_REG and EPI_SYNC macros. `__osPiDevList` is declared once. No existing macro semantics change.

## Independent strict checks

Every literal flagset is `-g0 -O2 -mips2 -G 0 -non_shared`. These are object words including alignment, not newly claimed slot extents.

| Body | Trusted object words | Strict / raw |
|---|---:|---|
| __osCheckId | 92 | 0 / 0 |
| __osGetId | 108 | 0 / 0 |
| __osIdCheckSum | 64 | 0 / 0 |
| __osPfsDeclearPage | 80 | 0 / 0 |
| __osRepairId | 212 | 0 / 0 |
| osMotorInit | 92 | 0 / 0 |
| osPfsChecker | 336 | 0 / 0 |
| osPfsFindFile | 36 | 0 / 0 |
| osPfsFreeBlocks | 104 | 0 / 0 |
| osPfsGetFileSize | 224 | 0 / 0 |
| osPfsGetFileStat | 56 | 0 / 0 |
| osPfsInitPak | 132 | 0 / 0 |
| osPfsReadWriteFile | 208 | 0 / 0 |
| osPfsReadWriteFile_pages | 56 | 0 / 0 |
| osPiSetDeviceTiming | 120 | 0 / 0 |

The five new C12 bodies are `osPfsReadWriteFile`, `osPfsGetFileSize`, `osPfsChecker`, `__osRepairId`, and `osPiSetDeviceTiming`; the other ten rows are currently accepted dependencies. Targets were copied independently from CURRENT authoritative DB/blob hashes, including refreshed reloc-aware PI and repair targets. `targets.json` preserves those identities rather than relying on C12's older target inventory snapshot. `proof.json` records source/target hashes, actual compiler exit codes and stderr, strict/raw verdicts, accepted-body before/after checks, and all 63 whole-TU comparisons. Every body compile is warning-free. Some unrelated existing whole-TU compiler warnings may persist unchanged; none is introduced by this minimal delta.

Whole-TU comparison compiles each current promoted-C ROM file with its GLOBAL_ASM pragmas omitted privately, using the same O2 flagset on both sides. All 63 comparisons have raw text difference zero and successful exits. Individual authoritative strict checks above verify the affected accepted dependencies, beyond simple text equality. This avoids claiming an arbitrary multi-function object is one standalone target.

## Actual caller audit

Current accepted `osPfsInitPak` at `src/rom/lib_a810.c` calls `__osRepairId` and `osPfsChecker`; accepted `__osGetId` at `src/rom/lib_f700.c` calls `__osRepairId` twice. Their existing signatures agree and they remain strict/raw zero. `caller_audit.json` gives current linked C paths/lines. Original assembly also confirms these calls at 0x80009D20 / 0x80009DF0 and 0x8000F1CC / 0x8000F214. New `osPfsReadWriteFile` calls accepted `__osPfsDeclearPage` at 0x8000AB18, with all seven genuine arguments; the new declaration agrees. No accepted C calls to the historical `osPfsGetFileSize` or `osPiSetDeviceTiming` labels were found.

`os_pfs_proposed.h` and `rom_tu_proposed.h` are private proposed snapshots for review, not shared-file mutations. `context_hashes.json` freezes original header/source inputs, and the verifier source is included. Private reproduction scratch is Rocky `~/agents/B/scratch/codex_B/B16context`; local ignored input/archive is `build/codex-B16/`.

## Actual root preparation follow-up

After root prepared its real shared headers and local PI macros, B independently snapshotted that actual state and reran the checks against the original before-preparation baseline. Root adds public `osPfsGetFileSize(OSPfs *,s32,u8,int,int,u8 *)` and `osPiSetDeviceTiming(OSPiHandle *,s32,u32,void *,u32)` declarations as well as the corrected allocator and declare-page signatures. The PI implementation macros remain local to `src/rom/lib_e9a0.c` after the accepted `__osInsertTimer` body. This is the actual root preparation audited here; the earlier proposed snapshots are reference evidence only.

All fifteen individual bodies still compile successfully and score strict/raw zero under these actual headers and, for PI, the exact local macro block. All 63 promoted-C ROM TUs again compile successfully and have text-word difference zero against the original pre-preparation baseline. The accepted `__osInsertTimer` stays unchanged. No new source adaptations are needed. `actual_proof.json` preserves the additional compiler/verdict results; `actual_context_hashes.json` freezes the actual inputs. `verify_actual.py` preserves the verifier. Root's preparation still has passthrough pragmas for the five new bodies at snapshot time; these checks establish the shared-context safety needed before promotion, not completed ROM acceptance.
