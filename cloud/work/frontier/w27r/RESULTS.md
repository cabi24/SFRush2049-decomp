STATUS (2026-10-12): DONE. draft_check READY. On the Pi from the repo root:
`python3 .claude/worktrees/w27r/tools/recomp/draft_check.py --remote rocky NAME path/to/draft.c`
(add `--params PARAM=REG,...` for a register-parameter function; `--scripts race,battle` to choose scripts).
Prints EQUIVALENT (behaviour only, NOT a match) / FIRST DIVERGENCE (script, frame, call #, retail vs draft) /
NEVER CALLED. Docs: tools/recomp/README.md section "draft_check".
regmap_verify READY: `python3 .claude/worktrees/w27r/tools/recomp/regmap_verify.py --remote rocky NAME 'a0=x:ptr,t3=m:int'`
(README "regmap_verify"). Census READY: tools/recomp/census/README.md (167 of 218 unmatched reached; 51 never,
listed there); draft_check now defaults to the 6 census scripts with the most calls of NAME.

# w27r: recomp Phase 7, decomp feedback tooling (branch w27r, worktree; host rocky)

Not a matching lane: no src/, asm/, lock or coverage change.

## 1. draft_check (done)

See tools/recomp/README.md "draft_check". Checked on rocky: matched render_helper EQUIVALENT (60,802 calls over
race/two_player/attract); injected bug DIVERGES at race frame 2 call #2; w21h engine_sound_sync EQUIVALENT;
func_800D7634 NEVER CALLED (race) / EQUIVALENT (audio_options, 435 calls); audio_effect_setup (inbound s0)
EQUIVALENT; w26d cluster.c func_800A1DD4 with `--params handle=a0,address=a1,bytes=a2,mark=t3`: EQUIVALENT
(saves, 9 calls; without --params it is refused). First check of a function ~50 s (two builds + retail runs), repeat ~5-16 s.

## 2. regmap_verify (done)

README "regmap_verify". func_800A1DD4 (w27e) claim `a0,t3`: REFUTED, a1 (buffer pointer) and a2 (size) are
live-in too; t3 = 0 (track_data_decompress) / 1 (slot_state_lookup); a0 = 0x80144D5C. entity_transform_calc
f22-f28 + forwarded a0: CONSISTENT, 51,092 calls at 4 of 10 static sites (race, battle). audio_effect_setup
claimed `a0`: REFUTED (s0 live-in, a0 not read).

## 3. Call census (done)

`tools/recomp/census/` (README "Call census"). 167 of 218 unmatched game functions reached by the 19 scripts
(178,776 of 231,436 B); 51 never (52,660 B): the table in census/README.md. New script stunt_end.press (stunt
until its timer ends) adds func_80101D84 and func_80102448; a ghost race held to the end added nothing (dropped).
Per lane (unreached = draft_check can only say NEVER CALLED):
- w27a: all reached except particle_render.
- w27b: unreached attract_mode_handler, attract_demo_handler, attract_video_handler, stat_race_start; the rest
  reached (world_object_destroy only in attract/attract_pak).
- w27c: reached func_80100D5C, game_results_render (audio_options), func_80100B8C, func_80100E58, func_8010221C,
  func_80101D84, func_80102448 (stunt_end), func_80102F30, func_80105480 (saves), func_80105EA8; unreached
  game_results_input/update/exit, func_80100564, func_80101904, func_80102980, func_80103D28,
  highscore_entry_anim, func_80104B14.
- w27d: unreached save_settings, fp_call_wrapper, func_800C7EC8, func_800D5A04, func_800DD0C0, func_8010BC84,
  func_8010E8B4, func_8010F218; drone_collision_avoid once (audio_options), track_texture_load once (saves).
- w27e: func_800A1DD4 / func_800A1BB4 only in saves (9 / 3 calls); func_80096CA8 and func_80097164 everywhere.

## 4. Package v4 (done, no laptop wait)

- `rocky:~/rush2049/recomp/dist/rush2049-recomp-test-v4.tar.gz` = Pi `build/recomp/dist/` in this worktree
  (git-ignored): 36,485,508 bytes, sha256 `a080ca8e26690793cfaa624a8893ea8b7df9c3930e438e5deec2183ecb0d2333`,
  repo commit edf96ad6, 0 `.z64` members, `--verify` OK. Private (rocky only) `-with-rom`: 47,912,357 bytes,
  sha256 `9e2a7e27780f9abb822ab51bc66869082fcbc2698bcd6c3d35a032719658c269`.
- Sanity (`~/rush2049/recomp/w27r/sanity/run.sh`, fresh extract, INSTALL B): MANIFEST OK; prebuilt + built hosts
  headless (pak created, ovl_b loaded, non-silent audio); saves/saves_reload pak lines as v3; lockstep a vs b
  `EQUIVALENT over 1800 frames`; RT64 container `RT64 setup: ok, graphics API Vulkan`, a vs c `DIFFERENT at frame 2:
  71 rdram page(s)`, `--mask-renderer` `EQUIVALENT except renderer-written pages over 1800 frames`.
- After merging wave27 (w27s identity.json renames): stock `rush2049.recomp.toml` and `rush2049.syms.toml`
  byte-identical to the v4 build (only manifest.json names differ), so v4's recompiled code is current.

## Checks (merged tree d4b48407 + this commit)

- Pi `python3 -m pytest tests/recomp -p no:cacheprovider`: 158 passed, 5 skipped (private inputs).
- rocky (civenv; RUSH2049_ROM / RECOMP_HOST / RECOMP_ROM / N64RECOMP_INCLUDE from build/stock): 161 passed,
  2 skipped (game_code.bin, libubsan); includes the draft_check end-to-end good/broken pair.
- Pi `make test-fast JOBS=4` run in the MAIN checkout (branch wave27 as it stood, the lanes' environment with
  the mips_to_c submodule): tests/conveyor 1618 passed, 764 skipped; tests/cloud 770 passed, 187 skipped; exit 0.
  This lane touches only tools/recomp, tests/recomp, the plan and this dir; no conveyor/cloud test imports
  tools/recomp.

## Open items for Phase 8

Scripts that finish a race (results, high-score entry, replay save: needs scripted steering or a lever), options
screens (controller/save settings), attract demo/video handlers; draft_check for a whole IPA unit (several
functions with their conventions); overlays; `--probe-callees` in draft_check to localise a divergence; cleanup
policy for `~/rush2049/recomp/dc` (~250 MB per function tree); refresh `dc/repo` when wave27 moves (README rsync
line; it is at this commit now).

## Permission denials

None.
