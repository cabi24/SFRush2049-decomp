STATUS (2026-10-12): draft_check READY. On the Pi from the repo root:
`python3 .claude/worktrees/w27r/tools/recomp/draft_check.py --remote rocky NAME path/to/draft.c`
(add `--params PARAM=REG,...` for a register-parameter function; `--scripts race,battle` to choose scripts).
Prints EQUIVALENT (behaviour only, NOT a match) / FIRST DIVERGENCE (script, frame, call #, retail vs draft) /
NEVER CALLED. Docs: tools/recomp/README.md section "draft_check".
regmap_verify READY: `python3 .claude/worktrees/w27r/tools/recomp/regmap_verify.py --remote rocky NAME 'a0=x:ptr,t3=m:int'`
(README "regmap_verify"). Census: in progress.

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
