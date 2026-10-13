STATUS (2026-10-12): DONE. Recomp Phase 8 (decomp feedback tooling), branch w28r. Not a matching lane: no src/,
asm/, lock or coverage change. Docs: tools/recomp/README.md "Phase 8" + "script_probe"; plan §17.

For lanes (once w28r is merged; until then call the files from .claude/worktrees/w28r/tools/recomp/ by path; the
rocky service tree ~/rush2049/recomp/dc/repo is already at this branch):
- `draft_check NAME draft.c` no longer needs `--callee` for slot_state_setup, sound_update_channel, func_800B6748,
  func_80096CA8, func_80097164, func_800A1DD4 (ipa_callees.json), nor `--params` when drafting one of them.
- `draft_check x --probe-callees F[,G] [--static-only]`: F's register convention in --callee syntax (order and
  return type are a guess: match your prototype).
- `draft_check --unit A,B,C cluster.c [--params B:p=r,...]`: the whole cluster TU replaces all members; the first
  divergence names the member.
- `script_probe.py --remote rocky $PWD/x.press --text`: what a script reaches and the on-screen strings.
- `draft_check x --gc [--dry-run]`: dc cleanup (also automatic).

# w28r: what landed

## 1. IPA callee table + --probe-callees

Entries (each with evidence): slot_state_setup `s32 (s32 selection)` = s2 / clobbers s0,s1,s3 (w27a/c/d);
sound_update_channel `void (s32 force)` = t0 (w27a); func_800B6748 7 pointers = a0-a3,t0-t2 (retail `sb v1,0(t0)`
etc.; w27b's world_trigger_check/world_velocity_integrate crashes); func_80096CA8 `(slot, first)` = s8,t0 /
s0-s7 (w27e); func_80097164 `(slot)` = s8 / s1,s3,s5,s6,s7; func_800A1DD4 `(handle, address, bytes, mark)` =
a0,a1,a2,t3 (w27r/w27e).

Finding: sound_update_channel's t0 is read AFTER its first call (the callee 0x80096288 does not touch t0); the
w24t intra-procedural flow assumes every call clobbers t0, so regmap_verify REFUTED the lanes' correct claim.
ipa_detect.register_params got an optional `call_effects` (interprocedural: callee reads/writes, transitive over
static jal; replace.py's hazard detection unchanged), regmap_verify uses it: slot_state_setup s2,
sound_update_channel t0, func_800A1DD4 a0,a1,a2,t3, entity_transform_calc a0,f22-f28, audio_effect_setup s0 all
`VERDICT: CONSISTENT`. `regmap_verify F --derive` / `draft_check x --probe-callees F` print e.g. (rocky, race,
dynamic) `--callee 's32 func_800B6748(void *p0, ..., void *p6)=a0,a1,a2,a3,t0,t1,t2'` (2024 calls) and
`--callee 's32 sound_update_channel(s32 p0)=t0'` (207,370 calls). Parameter order is by register number: the
call sites' setter order is not prototype order (menu_input_process sets func_800B6748's t2..a0 backwards).

## 2. Scripts + census

New tool `tools/recomp/script_probe.py` + host `--text-log` (runtime oracle.cpp `rush_text_log`, an extra hook
in menu_input_process 0x800B6994, the game's string queue): per script the probed functions reached and the
strings drawn, by frame (1,200 frames < 1 s). Seven scripts (fork of this lane): options_menus, pause_menus,
controls_frontend, race_finish, two_player_finish, player_finish, stunt_short (README "Phase 8 scripts").

Census (rocky, 26 scripts, 206 unmatched game functions at e3b1f926+w28r): **183 reached (197,184 B), 23 never
reached (22,768 B)**; before: 167 of 218 / 51 (52,660 B). Still unreached: func_8010C974, control_settings,
func_80103D28, func_80104B14, draw_speedometer, func_80102980, func_80101904, func_800DD0C0, func_800CC040,
drone_set_catchup, attract_demo_handler, AdjustSpeed, func_800CC50C, attract_video_handler, func_800C7EC8,
func_800F0F44, world_effect_update, func_800CBF2C, attract_mode_handler, draw_ui_element, menu_back,
fp_call_wrapper, object_bytes23_sum. Reasons (fork's analysis, README "Census refresh"): ghost-save branch
needs a finished ghost race (func_80103D28, func_80104B14, func_800CC50C, func_800CC040); battle results / mine
need scoring (func_80102980, func_80101904, func_8010C974); control_settings and its callees (the three
"attract_*" labels are wrong) are reached only from image A; HUD elements gated in func_800C813C's per-object
loop; world_effect_update, fp_call_wrapper, object_bytes23_sum have no static reference in any image.
Newly reached from the wave-28 priority list: game_results_update/input/exit, func_80100564, highscore_entry_anim,
func_8010F218, func_8010EA08, func_8010E8B4, func_8010BC84, func_8010A8D0/A7A4/AEAC, func_800D5A04 and more
(census/README.md). Countdown functions (func_800E543C/56F8/6AF8) were already reached by every race script.

## 3. draft_check --unit

replace.py entries with `"unit": [...]` compile/link once (one chunk, `-u` per member); each member gets its own
hook record `draft__<member>`, inbound shim, statics binding, self-call retarget; calls between members stay
inside the chunk. gen_config's unit hooks record only the outermost member call per thread on both sides
(runtime `rush_unit_enter/exit`, `rush_cap_arm`). Checked on rocky (w27e pak cluster.c, saves):
- `--unit func_800A1BB4,func_800A1DD4`: `VERDICT: EQUIVALENT over 12 calls / 1 scripts (saves 12)` (3 + 9).
- `--unit slot_state_lookup,func_800A1DD4,func_800A1C6C,func_800A1BB4 --params func_800A1C6C:handle=a0,
  address=a1,bytes=t1,offset=t2`: DIVERGES at frame 720, func_800A1C6C call #1, memory (the stack words its
  pointer parameters write land one slot off). Reading the written words: `handle=a0,address=t2,bytes=a1,
  offset=t1` -> `VERDICT: EQUIVALENT over 684 calls` (slot_state_lookup 670, func_800A1DD4 4, func_800A1C6C 7,
  func_800A1BB4 3). **For the pak-cluster lane: func_800A1C6C's mapping is address=t2, bytes=a1, offset=t1.**

## 4. dc cleanup

`draft_check x --gc [--dry-run] [--keep-trees 12] [--gc-days 3]`, automatic after a run past 16 trees: keep the
12 most recently used build trees (never a busy one), the newest retail cache per kept function, 5 runs per
function or 3 days; verify/probe/census/inbox older than 3 days. Dry run on rocky: 45 entries, 9.7 GB of 20.4 GB
(not executed by hand; the next draft_check run collects).

## Checks

See the final section (filled at the end of the lane).

## Permission denials

None.
