# w24m — merge lane: w24t (tooling) into wave24 (worktree `.claude/worktrees/w24m`, branch `w24m` from wave24 `67d5f4df`)

Status: done (2026-10-12). Branch commits on top of wave24: merge `d6fe0a41` (w24t `afdcd543`), `792c9bdf`
(ipa_detect MIPS III decode), `d1ea9698` (orientation receipt re-pin), this file. Fast-forwardable onto wave24.

## Conflicts (tools/recomp/{replace.py,gen_config.py,README.md}) and resolution

Rule (w24t, coordinator-endorsed): w24r's build path + w24t's detector, one flag, one warning, one README section.
- `replace.build(..., images, ipa_table=None, ipa_detected=None, allow_ipa=False, warn=None)`. Detection =
  `ipa_hazards(elf, address_table, functions, images, ipa_detected)` (w24t: retail bodies of the jal callees via
  `ipa_detect.py`, works in a fresh clone) merged with w24r's `load_ipa_detection()` (build/ipa_members.json +
  build/frontier.json when present; gen_config passes it as `ipa_detected`). A callee the object defines is no hazard.
- `ipa_plan` (w24r) now refuses ANY detected callee unless allowed (`--allow-ipa` or entry `"allow_ipa": true`),
  table-described or not; allowed -> table callees get w24r's register shims / linked bodies, others o32 calls
  (`ipa note ... (called o32: --allow-ipa)`). Replaced function itself detected: unchanged w24r rule (inbound table
  entry or allow). Retail jr-ra stub bodies (func_80091AF0/AF8) are not hazards and are always linked.
- One refusal text (`calls IPA register-parameter callees (name: why; ...)` ... `Pass --allow-ipa (entry
  "allow_ipa": true)`) and one warning line (`replace: WARNING <id>: ...; built with --allow-ipa: register shims
  for X, linked bodies for Y, o32 calls for Z`). Manifest per replacement: `ipa_callees` (w24t) + `ipa_shims`,
  `ipa_bodies`, `ipa_inbound`, `ipa_notes` (w24r).
- gen_config: w24r's `--allow-ipa`, `--ipa-table PATH|none`, `--ipa-members`; help text describes the merged gate.
- README: w24t's separate "IPA callees are refused" section folded into the Phase 4 "Detection and refusal" bullet.
- replacements.json `draft_engine_sound_sync` keeps `"allow_ipa": true`; its note updated (shims by default,
  `--ipa-table none` reproduces frame 705).

## Follow-up fixes found while verifying
- `ipa_detect.decode` did not know MIPS III ops: `ld`/`dmtc1` were not writes, so libultra's long-long helpers were
  flagged (`__ll_mul t6,t7; __ull_div t6,t7; __ull_to_f t6,f4`) and the merged gate refused the matched
  `cpak_read`. Added ld/sd, daddi(u), ldl/ldr, dsllv/dsrlv/dsrav, dsll..dsra32, dmfc1/dmtc1 + a test with the retail
  words. Game-image calibration unchanged: 91 flagged, 91/94 ipa_members callees, 0 standalone locks.
- `cloud/work/frontier/dot_target_orientation_20261005/verification.json` pinned wave24's asm/us/blob/symbols.json;
  w24t's rename changed it (toolchain-only test, so Pi skipped it and w24t's rocky subset did not include it).
  Proved the new file == wave24's with exactly the 4 renames of w24t/rename/renames.json applied; re-pinned to
  `1edbefcc...`. Six other receipts still name the old hash `46c815ca...` but no test compares them.

## rocky checks (`~/rush2049/scratch/w24m`, clone of ciclone-coord + this branch; outputs `~/rush2049/scratch/w24m-out`)
Merged gen_config run from the clone (no build/*.json: retail detector only) with w24r's b.sh inputs (ROM,
N64Recomp src, IDO 5.3/7.1 toolkits). w24r's tree `~/rush2049/recomp/w24r` was only read.
- `--replace-set draft_engine_sound_sync`: exit 0, WARNING line, `ipa shim audio_effect_setup: 1`, `ipa shim
  entity_transform_calc: 1`, `ipa body func_80091AF0/AF8 linked`. repl.bin, all images, rush2049.recomp.toml,
  syms/datasyms tomls, headers and the synthetic ROM byte-identical to w24r's `build/repl-draft_engine_sound_sync`
  (the build w24r judged EQUIVALENT on 4 scripts, 3599 calls identical). manifest.json differs only by w24t's 4
  identity rows and the new `ipa_callees`.
- Refusal: entry flag removed (`--replacements` copy), no `--allow-ipa`: exit 2, `calls IPA register-parameter
  callees (audio_effect_setup: register parameters s0; entity_transform_calc: register parameters
  f22,f24,f26,f28) ...`; same with `--ipa-table none`. Same copy with `--allow-ipa`: repl.bin == w24r shimmed.
- `--ipa-table none` (entry flag): o32 calls, repl.bin == w24r `repl-ess_noipa` (the frame-705 control).
- matched10 (10), bug, statics, draft_func_800D7634, draft_engine_sound_sync_probe (5): all build; 0 warnings except
  the probe set's engine_sound_sync (1).
- w23r's all-matched set (688 of 689 sources present; src/blob/func_80098ADC.c is gone): before the decode fix,
  refused at x_cpak_read; after, one refusal: `x_func_800A5158 -> audio_frame_sync: register parameters v0`
  (the loop-exit `or a3,v0,zero` at 0x80097910 reachable without a v0 write: an uninitialised local, most likely,
  not a convention). With `--allow-ipa`: 688 replaced, 1 warning. Open: whether v0/v1 reads should count.

## Tests
- Pi `python3 -m pytest tests/recomp -q -p no:cacheprovider`: 140 passed, 2 skipped (with build/game_code.bin copied
  in for the calibration test; without it 139 passed, 3 skipped at the final head).
- rocky `REQUIRE_TOOLCHAIN=1 pytest tests/recomp`: 138 passed, 4 skipped; with private inputs (RUSH2049_ROM,
  RUSH2049_RECOMP_HOST/ROM from w24r's stock build, RUSH_N64RECOMP_INCLUDE): 141 passed, 1 skipped (game image).
- rocky `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16` (after adding an origin/master ref to the clone, as w22p):
  conveyor 2304 passed, 52 skipped; cloud 924 passed, 1 skipped; exit 0. (First run: 3 fails, 2 = missing
  origin/master in the fresh clone, 1 = the orientation receipt above.)
- Pi worktree `make test-fast JOBS=4` (at 792c9bdf): conveyor 1548 passed, 808 skipped; cloud 770 passed,
  155 skipped; exit 0. The receipt re-pin touches only a toolchain-gated test (skipped on the Pi).
- `lock check` (pre-commit hook): all 607 locked functions intact.

## Coordinator
- `git merge --ff-only w24m` onto wave24. w24t's notes still apply: first builder gate after merge cleans build/us
  once (owned-slot stamp); the Conveyor DB was already renamed live by w24t.
