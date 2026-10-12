# w24t — tooling lane (worktree `.claude/worktrees/w24t`, branch `w24t` from wave24 `3bec9ce3`)

Status: done (jobs 1-6). Branch rebased onto wave24 `1dde4ccb`.

## 1. Builder gate: stale owned-slot state (done)
`promote.owned_stamp_guard()` / `owned_slot_inputs()` / `report_gate_clean()`; used by `promote run
--via-builder` (gate and rollback rebuild), `cloud/work/boot_tail_promotion/promote_batch.py` gate() and
`owned_storage` builder gate. On the builder, before `make`, the gate hashes the owned-slot inputs
(rom_owned_data.json, rush2049.us.ld, rom_owned_storage.ld, src/rom/{storage,opt}_overrides.mk,
rom_compilers.json, rom_symbol_aliases.ld, undefined_syms_auto.txt, splat.us.yaml, Makefile, every
passthrough companion, owned_text inputs; not src/rom/*.c, not baserom) and compares with
`build/us/.owned_slot_inputs.sha256` (written after a successful make). Different (or unstamped) ->
`rm -rf build/us` first and the log line `builder gate: GATE-CLEAN: owned-slot inputs changed since the last
build (stamp OLD -> NEW); rm -rf build/us for a full rebuild`. The first gate after merge cleans once.
Test: tests/conveyor/unit/test_gate_owned_stamp.py (runs the real shell fragments). Docs: BUILDING.md.
Live on watchman2 (`~/rush2049/scratch/frontier/w24t/repo`, the guard as the gates chain it, -j2): run 1 (unstamped
build dir) `GATE-CLEAN ... (stamp none -> d95431dc...)`, `ROM matches!`; run 2 no clean, `ROM matches!`; run 3
after appending a newline to rom_owned_data.json `GATE-CLEAN (stamp d95431dc... -> a583e27a...)`, `ROM matches!`;
final run at the rebased HEAD (wave24 added lib_26f60 + splat change) cleaned automatically, 106 TUs, SHA-1
3f99351d7bb61656614bdb2aa1a90cfe55d1922c.

## 2. Stale build/blob/obj after a rename (done)
`blob_splice.stale_objects()` + `rebuild_stale()` (w23m's script folded in: drift check first, singles per
flagset, groups one by one, re-check; provenance check in spliced_bodies unchanged), CLI
`blob_splice rebuild-stale [--dry-run] [--include-unbound] [--builder-dir D] [--group-builder-dir D]`;
`blob_rom rom` runs it before composing (`--no-rebuild-stale` restores the refusal), `blob_rom produce
--rebuild-stale` opts in; produce's refusal names the remedy. Test: tests/conveyor/test_blob_rebuild_stale.py.
Live (this worktree had no objects, i.e. everything stale): `blob_splice rebuild-stale --builder-dir
~/rush2049/scratch/frontier/w24t/splice --group-builder-dir ~/rush2049/scratch/frontier/w24t/group` rebuilt 688
singles (456+46+176+10 per flagset) and 103 groups; then `blob objects: 0 stale singles, 0 stale groups`;
`blob_rom produce`: `image 647072 B (985 spliced)`, `byte-identical to the ROM's stream: True`. (produce also
rewrote asm/us/blob/symbols.json and src/blob/blob.ld from this worktree's environment, -2,240 lines, as w22p saw;
reverted, not committed.)

## 3. replace.py IPA callees (done)
New `tools/recomp/ipa_detect.py` (stdlib): register parameters read from the RETAIL body of each jal callee
(must-write dataflow, delay slots/likely modelled, caller-save regs written after a call). `replace.build(...,
ipa_detected=None, allow_ipa=False, warn=None)` refuses a hit unless `gen_config --allow-ipa` or entry
`"allow_ipa": true` (then warns, records `ipa_callees`). Calibration on the game image: 91 flagged,
entity_transform_calc f22,f24,f26,f28; audio_effect_setup s0; 0/688 standalone locks; 91/94 of
build/ipa_members.json callees. `draft_engine_sound_sync` got `"allow_ipa": true` (reproduces the frame-706
harness verdict). Test: tests/recomp/test_ipa_detect.py.
Coordination with w24r: SendMessage could not reach w24r. Its uncommitted worktree diff uses the same names
(`build(..., ipa_detected=, allow_ipa=)`, gen_config `--allow-ipa`) but detects only from build/ipa_members.json
+ build/frontier.json (absent on a fresh rocky clone -> no refusal). Merge: keep w24r's shim/body build, call
`ipa_hazards()` (this lane) for detection and pass w24r's `load_ipa_detection()` as `ipa_detected`.

## 4. dll_init / dll_update / dll_reschedule (PROVEN)
`rename/prove.sh` (builder, IDO 5.3 toolkit 796ae99a, ultralib VERSION_J -D_FINALROM, flags of the locked TUs)
+ `rename/compare.py` (whole .text vs ROM words, relocations by value), log `rename/prove.log`:
- os/timerintr.c at -g0 -O1: `RESULT: IDENTICAL (256 words, 50 relocated fields compared by value, 0 differing
  words)` at 0x8000C090: __osTimerServicesInit = dll_init, __osTimerInterrupt = dll_update, __osSetTimerIntr =
  dll_reschedule, __osInsertTimer = __osInsertTimer.
- io/pfsinitpak.c at -g0 -O2: `RESULT: IDENTICAL (196 words, 18 relocated fields ...)` at 0x80009C10:
  the static `__osPfsCheckRamArea` is 0x80009E18, currently named `__osTimerInterrupt` (must move to free
  the name).
Rename table `rename/renames.json` (4 rows, a permutation).

Applied (static side only; no game source calls these names; game blob unchanged):
- `static_rename symbols|source|lock|owned` with `rename/renames.json`: symbol_addrs (4 rows), src/rom/{lib_cc50,
  lib_7630,lib_efd0,lib_a810}.c + rom_tu.h, PROMOTED headers retitled by hand, matched.lock.json 8 entries
  (4 renamed keys, 4 callers re-pinned with the pure-rename proof), rom_owned_data.json (timer_services members,
  prior locks; rebound copies under `rename/rebind/`), `owned_existing_storage.PROFILES` w24t revision (now with
  per-revision `new_members`), storage_overrides.mk regenerated by make.
- asm: the four nonmatchings files git-mv'd to the new names and identifiers permuted in them and in
  osPfsInitPak.s/osSetTimer.s (as w23g); asm/us/boot_tail/symbols.json keys + SHA256SUMS; asm/us/blob/symbols.json
  + SHA256SUMS and src/blob/blob.ld PROVIDE lines (permuted in place; the generator output differs here only at
  those 4 names). asm/us/ovl_* untouched (as w23g). asm/us/7630.s (unlinked whole-segment asm) untouched.
- tools/tests: targets.py `_typed_data_field_aliases` key, test_dll_init_alias, test_owned_text,
  test_known_pairings. `tools/recomp/identity.json`: 4 rows mapping back to the pre-w24t names (all four new
  names are on N64Recomp's ignored_funcs; 0x80009E18 was already ignored under its old name `__osTimerInterrupt`,
  a pre-existing recomp quirk kept unchanged for the recomp lane to decide).
- `make extract` in the worktree: no tracked change. `lock check`: all 607 intact (after rebase). `blob_rom
  coverage` identical before/after (static 600/668 before rebase; 604/668 after).
- Builder: full ROM in scratch (all TUs rebuilt) `ROM matches!`, SHA-1 3f99351d... (twice: before rebase, and at
  the rebased HEAD from a clean build/us).
- Conveyor DB: backups `scratchpad/conveyor.db.w24t.bak` and `.bak2` (sqlite backup API; .bak2 immediately before
  the live apply, 07:23 CDT). Copy run: 4 renamed, 2 matrix_entry rows purged, audit clean. Live:
  `python3 -m tools.conveyor.pipeline.targets_rename rename cloud/work/frontier/w24t/rename/renames.json` ->
  `renamed 4 rows in ~/.conveyor/conveyor.db (...), 2 matrix_entry rows purged`; `targets_rename audit` clean.
  No open work units on the 4 rows. Until w24t is merged, master's layout and the DB disagree on these 4 names.
- Further leads from the same compile (not renamed): `__osCurrentTime`=`gViTimeAccumHi` 0x80037C50,
  `__osBaseCounter`=`gViLastCount`, `__osViIntrCount`=`gViRetraceCount`, `__osCheckPackId`=`__osCheckId`
  0x8000EFC0, `__osRepairPackId`=`__osRepairId` 0x8000EC70.

## 5. layout convert leaves rush2049.us.ld naming build/us/asm/us/<SEG>.o (coordinator addition, done)
`layout.linker_object_drift()/sync_linker_objects()`, CLI `layout ldsync [PATH] [--check]`: for every `c`
subsegment, `build/us/asm/us/<OFF>.o(` becomes `build/us/src/<tu>.o(` (the exact splat difference); an asm
subsegment whose object is missing, or a c TU missing entirely, refuses with "run `make extract`".
`_do_convert` runs it after writing the TU; `_do_revert` checks. Makefile: `$(LD_SCRIPT)` depends on
splat.us.yaml (+ layout.py) and runs `layout ldsync $@` before `owned_data linker $@`. Replay on 4b4be251's
stale script: `rewrote build/us/asm/us/26F60.o -> build/us/src/rom/lib_26f60.o`, output == ee1a515f's
make-extract script byte for byte. Test: tests/conveyor/unit/test_layout_ld_opt_sync.py.

## 6. opt_overrides.mk vs write_opt_overrides (coordinator addition, done)
Decision: the generator is right. Its `lib_cc50.o: CFLAGS := $(subst -O2,-O1,$(CFLAGS))` is the segment's verified
flagset (-g0 -O1); storage_overrides.mk's explicit `-O1` recipe builds lib_cc50.o either way, so 7a90b14e's
hand removal changed nothing but made the file drift. On wave24 (after 4b4be251 regenerated it) the checked-in
file already equals the generator; `render_opt_overrides()` split out and a test asserts equality (skips
without the ROM, since derive reads it).

## Tests (at f80fb7de, rebased on wave24 1dde4ccb)
- Pi worktree `make test-fast JOBS=4`: conveyor `1561 passed, 795 skipped`; cloud `770 passed, 155 skipped`; exit 0.
  (Before f80fb7de: 2 conveyor fails in test_boot_tail_ido71_pins, which also fail on wave24 itself because w24a's
  0x26f60 7.1 pin was not in the test's list -- fixed here; 1 cloud fail, the nearest_point_donor receipt pinning
  asm/us/blob/symbols.json -- re-pinned after proving the new file is the pinned one with the 4 keys renamed.)
- rocky `~/rush2049/scratch/w24t` (copy of ciclone-coord + this branch), REQUIRE_TOOLCHAIN=1 on the affected files
  (gate stamp, promote, blob_rebuild_stale, blob_splice, blob_rom, layout_ld_opt_sync, layout, dll_init_alias,
  owned_text, existing_storage, initialize_storage, owned_storage, owned_data, rom_compiler_override,
  debug_build_flags, mips3_32_tu, boot_tail_ido71_pins, cloud nearest_point_donor, tests/recomp):
  `351 passed, 7 skipped`.
- `static_rename.RENAME_TABLES` now includes `w24t/rename/renames.json` (renamed_source/pinned_bytes know it).
