STATUS (2026-10-12): draft_check READY. On the Pi from the repo root:
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
EQUIVALENT. First check of a function ~50 s (two builds + retail runs), repeat ~5-16 s.

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
