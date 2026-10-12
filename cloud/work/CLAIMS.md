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
| ovl_a | 8038F648, 803A7180 | Astra | 2026-10-11 | PR #311 near-misses; 80394058 (#315), 803A6F2C/803A7620 (#316), 8039D05C/8039D494/8039ED20 (#318), 803A7918 (#321) landed in image A; image B func_803936A8 (#319) landed 2026-10-12 |
| ovl_a / ovl_b | everything else in `asm/us/ovl_*` | Astra (default) | 2026-10-11 | coordinator lanes do not take overlay functions unless listed here |
| game | world_effect_update (12 words, needs a block boundary), audio_frame_update (22 words), race_position_update (50 words, s4 register variable) | open | | w20d RESULTS 2026-10-12; assign_drones landed |
| game | engine_sound_sync + audio_effect_setup (8 rows; float temp offset), func_800D7634 + func_800D6914 (15 words; D_80110640 colour) | open | | w21h RESULTS 2026-10-12 |
| game | func_8010FBE0, entity_process_main | open | | w20e RESULTS 2026-10-12 |
| game | func_800EC190 (2 words), func_8008AD6C (29), func_800A1BB4 (35), wheel_setup_initial, ghost_race_setup, audio_fade_control unit (6/8) | open | | w21g/w20f RESULTS; func_800D63EC, func_800B3704, func_800D6530 landed 2026-10-12 |
| game | entity_render_setup (12 rows with the data_align pad; causes 2 and 3 in w20h RESULTS), camera_play_script (27 rows) | open | | 2026-10-12 |
| game | func_80090308, buffer_swap, cpak_read (unblocked by entity_spawn_init) | open | | ready since 2026-10-12 |
| static | func_80018448 (325), func_80010E80 (460), func_80018634 (780) | open | | w21a RESULTS 2026-10-12: traced causes, need a 7.1 optimiser trace |
| static | func_80022874 (code identical; its static must sit at 0x80056208 between func_80023E9C's statics), func_8001A270 (225), func_8001E0E0 (2215), func_80019F48 (2235) | open | | w21b RESULTS 2026-10-12 |
| static | func_800178B0 (seqStartPlay, 205), func_8001F13C (voiceAllocate), func_80012D18 (per-voice command builder), func_80023E9C (macro interpreter; owned jump-table slot 0x8002D964/0x1C8), func_8001A658 | open | | w21c RESULTS 2026-10-12 has identities and best sources |
| static | other remaining boot tail `func_*` (func_800268D0, func_80012D18, func_8001A658, func_80023E9C, func_80012730, func_8001F13C, func_800178B0, func_8001C860 and the rest) | open | | Astra welcome; say so here |
| tooling | static recompilation Phase 2 (function replacement, rdram view, frame oracle) | coordinator | 2026-10-12 | Phase 1 landed: boots to the front end and loads image B under scripted input (plan §9) |
