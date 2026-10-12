# w23g: ll.c/llcvt.c rename + game-side libultra rename (tooling, worktree)

Branch `w23g` (worktree `.claude/worktrees/w23g`, base wave23 = origin/master `68477154`). Not pushed to origin.
Status: done (ll.c rename + cpak_read natural, game side + legacy table emptied, 6 leads, packets rebound, DB applied).

**Renames: 18 ll.c/llcvt.c helpers + 6 verified leads (static and game side), and the game side of
w22g's 43. `game_static_legacy.us.txt` has no entries left.** Coverage unchanged (`make progress`
identical before/after: static 586/668, 124,448 B; game 982/1216, 322,664 B; overlay A 53/193; B 10/49).
Full ROM on the builder: `ROM matches!`, SHA-1 `3f99351d7bb61656614bdb2aa1a90cfe55d1922c`.

## 1. ll.c / llcvt.c (18 renames)

Table `ll_renames.json`, produced and checked by `ll_check.py`: each locked definition in
`src/rom/lib_e4f0.c` / `lib_a5f0.c` (score 0, w16g, `-g0 -O1 -mips3 -32`) equals the ultralib
`src/libc/ll.c` / `llcvt.c` definition token for token after the rename, and the ROM order of each
TU is the ultralib definition order. `__lshrdi3`->`__ull_rshift`, `__umoddi3`->`__ull_rem`,
`__udivdi3`->`__ull_div`, `__ashldi3`->`__ll_lshift`, `__umoddi3_alt`->`__ll_rem`, `__divdi3`->`__ll_div`,
`__muldi3`->`__ll_mul`, `__qdivrem`->`__ull_divremi`, `__moddi3`->`__ll_mod`, `__ashrdi3`->`__ll_rshift`;
`__fixdfdi`->`__d_to_ll`, `__fixsfdi`->`__f_to_ll`, `__fixunsdfdi`->`__d_to_ull`, `__fixunssfdi`->`__f_to_ull`,
`__floatdidf`->`__ll_to_d`, `__floatdisf`->`__ll_to_f`, `__floatundidf`->`__ull_to_d`, `__floatundisf`->`__ull_to_f`.

Applied with `static_rename symbols|source|lock|owned` (19 matched.lock.json entries incl.
`__osInitialize_common`, whose explicit calls are now `__ull_div(__ll_mul(...))`), the nonmatchings
renamed (`make extract` reproduces them exactly), boot_tail/blob symbols, `blob.ld`, the owned
.rodata slot (`__fixunsdfdi` -> `__d_to_ull`, co-owner `__f_to_ull`), `sdk_initialize` profile revision.
`disasm.SYMBOL_ALIASES["__ll_lshift"]` removed (symbol_addrs provides it now). Not a permutation, so
no legacy entries. **cpak_read** is now w22d's natural form `usec = (u64)D_80124F70 * 64 / 3000;`
(src/blob and cloud/matches), re-spliced: image matches; unit EQUAL. The gfx_modes group copy of
object_render calls `__ll_lshift` explicitly (pure rename of its `__ashldi3` calls; switching it to the
natural shift form is a separate change). `tools/recomp/identity.json` maps the 18 back to their libgcc
names for the recompiler only (N64Recomp lists the ultralib names as ignored/reimplemented: dropping
those entries changes recomp output, w23r's call).

## 2. Game side of w22g's 43 + 6 verified leads

- `static_rename source` with w22g's `renames.json` over `src/blob/**` (singles, groups, context copies)
  and the 86 `cloud/matches/*.c` that use the names. The permutation turns some generated prelude
  prototypes into conflicting redeclarations (e.g. `osPfsReAllocate(OSPfs*, s32)` becomes a second
  `osGetThreadId`); `drop_renamed_dups.py` removes the renamed statement when the name is declared
  again and not called (1,478 statements in 493 files). One file, `game_state_check_handler.c`, calls the
  renamed one (`osGetThreadId(0, arg1)`, an m2c artefact: 0x8000BE50 takes one argument), so there the
  ultralib `osGetThreadId(OSThread*)` prototype was removed instead.
- Leads verified against ultralib bodies (`suspect_renames.json`, evidence per row; renamed static and
  game side at once): `dll_remove`->`__osDequeueThread`, `dll_insert`->`__osInsertTimer`,
  `__osInsertTimer` (0x8000E390)->`osPiGetCmdQueue`, `osPiInit`->`__osPiCreateAccessQueue`,
  `__osCleanupThread`->`__osEnqueueAndYield`, `osWritebackDCache_full`->`osSyncPrintf` (medium:
  FINALROM empty varargs + segment position, as w22g's rmonPrintf). 18 matched.lock.json entries,
  rom_auto file git-mv'd, timer_services profile revision, boot_tail symbols (stale
  `__osEnqueueAndYield = 0x8000FBA0` key replaced by `osDestroyThread`), `corpus.CANONICAL_ALIASES`
  emptied (unit tests install their own alias), `static_rename.libultra_names()` = union of all three tables.
- **Blob lock**: `rename_resplice.py` proves for every changed lock entry that the file(s) at the base
  held the pin and the current file is exactly the base text renamed (redeclaration drops allowed),
  recompiles all changed singles/groups on the builder (own scratch, not /tmp/blobsplice), links every
  locked body and runs the image gate before re-pinning. Proofs: `blob_lock_migration_ll.json`
  (7 entries), `blob_lock_migration_game43.json` (563 entries: 397 singles, 47 groups),
  `blob_lock_migration_suspects.json` (465 entries). Every run: `image matches (647072 bytes) bf7da3fa...`.
- After the last step: `blob_unit --tag w23g check` -> `982 locked bodies, 982 equal ... 0 differ`;
  `blob_splice check` -> `982 entries, 0 problems`; `blob_group check` -> `0 problems`;
  `blob_rom produce` -> byte-identical to the ROM's stream; `asm/us/blob/*.s` unchanged (only
  `symbols.json`/`SHA256SUMS`/`blob.ld` regenerated). `asm/us/ovl_*` untouched (the runtime images
  resolve against their own frozen symbols.json, still old spellings).

## 3. Research packets

~30 receipt packets (cloud/work/**/verify.py + tests) pinned pre-rename game sources or names.
`rebind_packets.py` rebinds them by the same rename: sha256 pins of files whose current form is a proven
rename of the pinned bytes move to the current hashes (ambiguity-aware), receipts' exact name strings and
verifiers' C-name tokens follow the permutation, and packet/external candidate, host and superseded group
sources (`cloud/work/frontier/superseded/{audio_heap,frontier_list_alloc_sound,func_800AD4C8}`, tiny_A40,
tiny_A50, tiny_A116, game_C41, game_C104, heads_B10, near_miss_B8, ipa-groups codex_mode_outer_a169 and
dot_steady_camera) are renamed like src/blob. Log: `rebind_log.json`. One receipt
(`dot_constructor_prepend_20261005/verification.json`) had object/ELF metadata hashes re-recorded on rocky
(strtab one byte longer: `osSendMesg` vs `osJamMesg`; all words identical, host fields kept).
`static_rename.pinned_bytes()/renamed_game_text()` let a packet accept the base bytes of a pin when the
live file is their pure rename (`RENAME_BASE = 68477154df`, an ancestor of master).

## Conveyor DB

Backup (sqlite backup API): `scratchpad/conveyor.db.w23g.bak`. Dry run + full apply on a copy
(`scratchpad/w23g/data`): 24 renames, 7,339 matrix_entry rows purged, `targets_rename audit` clean.
Live apply (last step, 2026-10-11 23:50 CDT, after a second consistent backup
`scratchpad/conveyor.db.w23g.bak2` taken immediately before; no open work units on the 24 rows):
`python3 -m tools.conveyor.pipeline.targets_rename rename cloud/work/frontier/w23g/db_renames.json`
-> `renamed 24 rows in ~/.conveyor/conveyor.db (n64_target, function_status, matrix_entry, work_unit,
promotion_record), 7339 matrix_entry rows purged`. Then `targets_rename audit` clean (against this
branch's layout), 24/24 new names present, no old names or temporary ids left. The 43 w22g rows were
already renamed by w22g. Until w23g is merged, master's layout and this DB disagree on these 24 names.

## Builder

`watchman2:~/rush2049/scratch/frontier/w23g/repo` (copy of w22g's scratch repo, then rsynced from this
worktree incl. the produced game blob and ovl streams): `nice make COMPILER=ido -j2` (every ROM TU
rebuilt) then `make test`: `MAKE=0 TEST=0`, `ROM matches!`, SHA-1 3f99351d...
Unit/splice compiles: `~/rush2049/scratch/frontier/unit/w23g`, `~/rush2049/scratch/frontier/w23g/{splice,group}`.

## Tests (at 560ada35)

- Pi worktree `make test-fast JOBS=4`: conveyor `1524 passed, 796 skipped`; cloud `770 passed,
  150 skipped`; exit 0.
- rocky `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16` (clone `~/rush2049/scratch/w23g`, origin/master =
  68477154): conveyor `2269 passed, 51 skipped`; cloud `919 passed, 1 skipped, 27 subtests passed`; exit 0.
  `check_submissions.py --base origin/master --head HEAD`: `Checked 92 changed cloud submissions`, exit 0
  (87 cloud/matches + 5 ipa-groups, all claims MATCH). `make check-matched`: all 589 locked functions
  intact; ovl lock 63 intact.
- Builder full ROM at ffc96fc4 (later commits touch only cloud/work, tests and static_rename helpers):
  `ROM matches!`.

## For the coordinator / other lanes

- **In-flight drafts written against the old game names change meaning**: `osSendMesg`/`osJamMesg`,
  `osSetThreadPri`, `osPiStartDma`, ... now name the true functions. Pass any pre-w23g game draft through
  `static_rename.renamed_source()` (plus `drop_renamed_dups.py`-style prototype cleanup when a generated
  prelude is included) before scoring it against the new `asm/us/blob/symbols.json`.
- `__ll_mul`/`__ull_div`/`__ull_to_f`/... resolve in game C now: write u64 arithmetic naturally.
- Leads still open: `dll_init`/`dll_update`/`dll_reschedule` (probably `__osTimerServicesInit`,
  `__osTimerInterrupt`, `__osSetTimerIntr`) and the rest of the doc's list.
- Recomp (w23r): identity.json pins the 24 old names for the recompiler; decide whether N64Recomp should
  see the ultralib names.
