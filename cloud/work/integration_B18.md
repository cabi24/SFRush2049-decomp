B18 freezes a private, ROM-proven proposal for the complete VI manager module. This is pending coordinator integration and adds no accepted coverage yet. Both real functions and their authentic complete BSS must move together; a standalone counter binding is unsupported.

The frozen module is `integration_B18/lib_7630.c`, SHA-256 `2c583d59ef40599c644e9bbc456bd2d7bbf0cce5a14d0673354069bd7e53cce5`, with literal `-g0 -O2 -mips2 -G 0 -non_shared`. It preserves the accepted `s32 osGetActiveQueue(void)` declaration/body and casts its returned value. The corrected SDK thread context uses eight-byte FP pair slots, giving OSThread size 0x1B0. Private before/after compilation of all 63 then-current accepted ROM TUs produced identical text; no accepted body changed. That snapshot predates the latest coordinator acceptances, so repeat the current-tree audit at integration.

Canonical function-scoped, stack-sensitive scoring of the complete shared-header module gives creator `osSetEventMesg` strict 0/raw 9 (96 words) and worker `vi_manager_main` strict 0/raw 5 (100 words). These raw residuals are genuine unrelocated section-relative references, not a claim of raw-zero standalone objects. Coordinator D independently reproduced both scores against the same authoritative target hashes. Resolving the complete original-address module produces all 196 retail words / 784 bytes exactly, SHA-256 `ea7fa7d6effd8003aaf38622f1396826cd7914d9402d95e230bb2aa0b89b4cb5`. No scorer settings were weakened, no dummy context was introduced, and no standalone counter address was assigned.

The original SDK storage ordering is retained:

| Member | Address | Size |
|---|---|---|
| Thread | 0x800354F0 | 0x1B0 |
| Real u64 stack | 0x800356A0 | 0x1000 |
| Queue | 0x800366A0 | 0x18 |
| Five-message buffer | 0x800366B8 | 0x14 |
| DP event | 0x800366D0 | 0x18 |
| SP event | 0x800366E8 | 0x18 |
| Function-local u16 retrace counter | 0x80036700 | 2 |

The complete aligned block is `[0x800354F0,0x80036710)`, size 0x1220. Root independently verified retail entry clears `[0x8002E8E0,0x80086A50)` using the fixed 0x58170-byte doubleword loop, covering the whole proposed NOLOAD block. Thus the original startup zeroing includes stack, queue, events and local counter. `coordinator_D_startup.json` pins the entry slice hash and interval evidence.

The isolated builder `watchman2:~/agents/B/B18-private-rom` passed the full ROM gate with custom ownership, sustainable separate linker script, after local SPLAT regeneration, and with the automatic registry generator recipe. The last `make COMPILER=ido -j2 && make test` exited 0 and printed `ROM matches!` twice. All gates retain retail SHA-1 `3f99351d7bb61656614bdb2aa1a90cfe55d1922c`. Extraction used local `/home/cburnes/.splat-venv/bin/python`; default watchman Python lacks SPLAT. The private game snapshot is older than root's current game blob; fresh root gates remain mandatory.

The reviewable lifecycle patch is `integration_B18/lifecycle.patch`. It adds only the distinct `owned_storage.py`, focused tests, and small hooks against the current C13 root. `hooks.patch` is the hook-only alternative for manual integration. Neither patch replaces C13's owned_data module, ROM splitter, asm_processor, or registry ROM records. `patch_baseline.json` identifies each hook file baseline. `storage_block.json` is one inactive row; `rom_owned_data.proposed.json` is historical reference only and must not replace the current registry.

The ownership generator consumes the whole real `.bss`, renames it to `.bss.vi_manager`, and places it as NOLOAD after `.data` at the proven address. It asserts complete size and authoritative aliases. Renaming prevents regenerated SPLAT's ordinary `.bss` collection from stealing the block. The original whole-module assembly remains available for rollback and is filtered out of active O_FILES. The shared registry API preserves both `rom_slots` and `storage_blocks`; package_paths does not recurse into C13 promotion_paths.

Promotion and revert operate on the complete tuple: source, SDK header, registry, generated linker, Make overrides, SPLAT entry/generated linker, both body locks, and original assembly. Source/context hashes pin complete storage declarations. A failed gate restores original file contents/modes/missing files and rebuilds the restored builder. Both ordinary body locks migrate only after the complete ROM gate. Normal one-function layout/promotion actions refuse registered storage owners. Active-module states come from the pinned whole module, and fast lock checks also reject full context drift. The focused four-test suite passes for failed-gate rollback, successful whole-pair activation/revert, storage declaration drift, and overlapping owners. Tests mock external gate/DB/commit actions; worker never performed a commit or shared promotion.

Coordinator integration sequence:

1. Review/apply lifecycle.patch after C13 baseline preparation; if hook baselines changed, use the small hook diff manually. Retain C13's existing word/float/double asm sizing branches.
2. Register only the inactive row with `python3 -m tools.conveyor.pipeline.owned_storage register --from cloud/work/integration_B18/storage_block.json`. This preserves current ROM metadata and installs a tracked inactive TU placeholder plus generated empty storage overrides. Commit the reviewed preparation so promotion can require a clean complete package.
3. Independently establish both normal strict body locks against the exact candidate whole module, flags and current targets. Retain the linked-text and startup evidence alongside the honest nonzero unrelocated raw counts. Run focused ownership tests and required full suite.
4. With farm stopped and current game/blob state verified, run `python3 -m tools.conveyor.pipeline.owned_storage promote vi_manager --via-builder`. This installs the full module/header and atomically gates both functions with whole storage. Repeat lock/ROM gates against current master before accepting coverage.
5. Revert the whole owner through `python3 -m tools.conveyor.pipeline.owned_storage revert vi_manager --via-builder` when needed. It refuses drifted context and gates the baseline restoration. Do not use single-function splices or map the local counter alone.

No root source, header, layout, lock, farm state, or metadata was changed by worker B18. Objects, ROMs and archives remain ignored/private; committed deliverables contain source, patches and hash/count evidence only.

Coordinator preparation: current shared-builder inactive baseline and source-built pipeline ROM both pass SHA-1 EXACT; full pytest exit0 and all game/group/body locks pass. Production storage helper includes the reviewed explicit local make-test gate; five lifecycle tests include rejecting a misleading successful build followed by failed SHA test. Test fixtures restore original metadata even after live activation. Worker prototype patch/helper files are historical proposals; production tools/conveyor/pipeline/owned_storage.py is authoritative. Both candidate locks were recorded from the reviewed B19 proof digest after independent D strict, linked-text and startup verification.
