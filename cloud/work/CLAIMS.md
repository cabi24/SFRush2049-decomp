# Claims: who is working on what

One line per function, group or tool being worked, so the coordinator's lanes and the cloud contributor
(Astra, GitHub `cabi24`) do not collide. Add a line when you start, remove it when the work lands or is
written up. Populations: `game` (`asm/us/blob`), `static` (`asm/us/boot_tail`), `ovl_a` / `ovl_b`
(`asm/us/ovl_{a,b}`). Dates are when the claim was taken.

| Population | Target | Who | Since | Note |
|---|---|---|---|---|
| game | func_80087110 (texture rectangle) | Astra | 2026-10 | long-running; coordinator lanes keep off |
| game | stat_race_update, func_800FE5B0 | Astra | 2026-10 | |
| game | camera_transform | Astra | 2026-10-07 | PRs #307/#308 |
| game | func_800D816C (+ func_800D91A0 cluster) | Astra | 2026-10-11 | Astra's draft is the base; coordinator's w17e notes in `cloud/work/frontier/w17e/RESULTS.md` |
| ovl_a | 803A7180 (frame research, paused), 803A6B50 (3/241 words, frame 160 vs 144; #330 packet imported, no coverage) | Astra | 2026-10-12 | PR #311 near-misses; 80394058 (#315), 803A6F2C/803A7620 (#316), 8039D05C/8039D494/8039ED20 (#318), 803A7918 (#321) landed in image A; image B func_803936A8 (#319) landed 2026-10-12; w22p landed 8038F648 (#324), 8038C93C (#329), 8039C140 (#326) and 8039B560 (#328; merged into the superseding `dot_runtime_a_text_helpers_20261006`, which absorbed the config_caller/config_parent/scene_init/cleanup groups) and image B 803908D0 (#327) 2026-10-12 |
| ovl_a / ovl_b | everything else in `asm/us/ovl_*` | Astra (default) | 2026-10-11 | coordinator lanes do not take overlay functions unless listed here |
| game | world_effect_update (12 words, needs a block boundary), audio_frame_update (22 words), race_position_update (50 words, s4 register variable) | open | | w20d RESULTS 2026-10-12; assign_drones landed |
| game | engine_sound_sync + audio_effect_setup (oracle: DIFFERENT at frame 706), camera_play_script | coordinator w23f | 2026-10-12 | wave 23 |
| game | func_8010FBE0, entity_process_main | open | | w20e RESULTS 2026-10-12 |
| game | func_800EC190 (2 words), func_8008AD6C (29), func_800A1BB4 (35), wheel_setup_initial, ghost_race_setup, audio_fade_control unit (6/8) | open | | w21g/w20f RESULTS; func_800D63EC, func_800B3704, func_800D6530 landed 2026-10-12 |
| game | entity_render_setup, controller_poll, fresh singles by fewest attempts | coordinator w23e | 2026-10-12 | wave 23 |
| game | buffer_swap, func_80090308 + entity_physics_update + save_write_data group | coordinator w23d | 2026-10-12 | wave 23 |
| tooling | ll.c/llcvt.c family rename + cpak_read natural form; game-side libultra rename (drop game_static_legacy.us.txt) | coordinator w23g | 2026-10-12 | wave 23 |
| static | func_80018634, func_80018448, func_80012D18 | coordinator w23a | 2026-10-12 | wave 23 (trace71) |
| static | func_8001A658 (synthHandle), func_8001F13C (voiceAllocate) | coordinator w23b | 2026-10-12 | wave 23 (trace71) |
| static | func_8001E0E0, func_80019F48 + triage of the remaining boot tail | coordinator w23c | 2026-10-12 | wave 23 |
| static | other remaining boot tail `func_*` (func_800268D0, func_80012D18, func_8001A658, func_80023E9C, func_80012730, func_8001F13C, func_800178B0, func_8001C860 and the rest) | open | | Astra welcome; say so here |
| tooling | static recompilation Phase 3 (per-call capture, input scripts, static binding, OSThread word mask) | coordinator w23r | 2026-10-12 | wave 23, on rocky; RT64 by the owner's agent elsewhere |
| tooling | recomp o32 wrapper generator with guest-pointer (`rush_addr`) mode: fold into `replace.py` native mode rather than a second generator; RDRAM accessors (`tools/recomp/rdram.h`, `game_rdram.h`) landed from #325 | Astra (#323 reservation) | 2026-10-12 | w22p; `tests/recomp/test_rdram.py`; the accessors own README section `RDRAM accessors` |
| ovl_a | func_8039BB80 (712-byte strict match); func_8039A4D4 (bounded probes, unchanged 623/716) | Astra literal lane1 | 2026-10-12 | same-name text_helpers group preserves all 11 installed members; dot/overlay-a-literals-20261012 |
