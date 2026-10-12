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
| game | engine_sound_sync + audio_effect_setup (8 rows; FP spill decisions in the cross-product block) | open | | w22e RESULTS; func_800D7634 + func_800D6914 landed 2026-10-12 |
| game | func_8010FBE0, entity_process_main | open | | w20e RESULTS 2026-10-12 |
| game | func_800EC190 (2 words), func_8008AD6C (29), func_800A1BB4 (35), wheel_setup_initial, ghost_race_setup, audio_fade_control unit (6/8) | open | | w21g/w20f RESULTS; func_800D63EC, func_800B3704, func_800D6530 landed 2026-10-12 |
| game | entity_render_setup (8 rows; palette address live range needs block 17), controller_poll (92), func_800EC190 (2), camera_play_script (27 rows) | open | | w22f RESULTS 2026-10-12 |
| game | buffer_swap (43 words; model/car spill homes), func_80090308 + entity_physics_update (7 rows) + save_write_data group | open | | w22d RESULTS; cpak_read landed 2026-10-12 |
| static | rename the ll.c/llcvt.c statics to their ultralib names (__muldi3 -> __ll_mul, __udivdi3 -> __ull_div, __floatundisf -> __ull_to_f, and the rest of the family): retail game code calls them by those names (w22d cpak_read) | open | | follow-up to w22g |
| static | func_80018448 (ugen ring order), func_80018634 (colouring only, jump value must live into default), func_80012D18 (12735) | open | | w22c RESULTS; trace71 toolkit in cloud/work/frontier/tools/trace71; func_80010E80 and func_8001A270 landed 2026-10-12 |
| static | func_8001A658 (synthHandle, 7725; midi vs 0x7F0000 priority: trace71) | open | | w22b RESULTS; func_80023E9C and func_80022874 landed 2026-10-12 |
| static | func_8001E0E0 (2215), func_80019F48 (2235) | open | | w21b RESULTS 2026-10-12 |
| static | func_8001F13C (voiceAllocate, 2420; PRE hoists voice*416) | open | | w22a RESULTS; func_800178B0 landed 2026-10-12 |
| static | other remaining boot tail `func_*` (func_800268D0, func_80012D18, func_8001A658, func_80023E9C, func_80012730, func_8001F13C, func_800178B0, func_8001C860 and the rest) | open | | Astra welcome; say so here |
| tooling | static recompilation Phase 3 (per-call capture at replaced functions, more input scripts, static binding) | coordinator | 2026-10-12 | Phase 2 landed (replace.py, oracle.py, lockstep; plan §10); RT64 by the owner's agent elsewhere |
| tooling | recomp o32 wrapper generator with guest-pointer (`rush_addr`) mode: fold into `replace.py` native mode rather than a second generator; RDRAM accessors (`tools/recomp/rdram.h`, `game_rdram.h`) landed from #325 | Astra (#323 reservation) | 2026-10-12 | w22p; `tests/recomp/test_rdram.py`; the accessors own README section `RDRAM accessors` |
| tooling | game-side libultra rename: callers in src/blob still use the pre-w22g names via game_static_legacy.us.txt (docs/SYMBOL_MISATTRIBUTION.md); five more suspected misnames listed there | open | | w22g landed the 43 static renames 2026-10-12 |
| ovl_b | func_803925D0 (HUD initializer), func_80392FE4 (health-bar callback) | Astra / dot image-B lane | 2026-10-12 | Reserved for trace-first matching in `dot/ovl-b-next-cluster-20261012`; no credit for already integrated 803908D0 or 803936A8; separate compact deliverables if both close |
