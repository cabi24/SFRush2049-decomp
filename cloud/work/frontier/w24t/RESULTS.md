# w24t — tooling lane (worktree `.claude/worktrees/w24t`, branch `w24t` from wave24 `3bec9ce3`)

Status: jobs 1-3 committed; job 4 proven, rename in progress.

## 1. Builder gate: stale owned-slot state (done, c8fc08aa)
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

## 2. Stale build/blob/obj after a rename (done, d2879f84)
`blob_splice.stale_objects()` + `rebuild_stale()` (w23m's script folded in: drift check first, singles per
flagset, groups one by one, re-check; provenance check in spliced_bodies unchanged), CLI
`blob_splice rebuild-stale [--dry-run] [--include-unbound] [--builder-dir D] [--group-builder-dir D]`;
`blob_rom rom` runs it before composing (`--no-rebuild-stale` restores the refusal), `blob_rom produce
--rebuild-stale` opts in; produce's refusal names the remedy. Test: tests/conveyor/test_blob_rebuild_stale.py.

## 3. replace.py IPA callees (done, ed62e0ae)
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
