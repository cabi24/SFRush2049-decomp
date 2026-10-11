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
| ovl_a | 8038F648, 803A7180, 803A7918 | Astra | 2026-10-11 | PR #311 near-misses (frame-only residuals); 80394058 (#315), 803A6F2C/803A7620 (#316) and 8039D05C/8039D494/8039ED20 (#318) landed in image A 2026-10-12 |
| ovl_a / ovl_b | everything else in `asm/us/ovl_*` | Astra (default) | 2026-10-11 | coordinator lanes do not take overlay functions unless listed here |
| game | world_effect_update, audio_frame_update, assign_drones, race_position_update | coordinator w20d | 2026-10-12 | wave 20 (running) |
| game | engine_sound_sync (14 rows, group with audio_effect_setup), func_8010FBE0, func_800D6914 (3 rows, provisional), entity_process_main | open | | w20e RESULTS 2026-10-12 |
| game | audio_fade_control unit (6/8 EQUAL), wheel_setup_initial, ghost_race_setup | open | | w20f RESULTS 2026-10-12; func_800D11BC landed |
| game | entity_spawn_init | open | | 7 words left (w20g RESULTS 2026-10-12: two spill slots) |
| game | camera_play_script (27 rows), entity_render_setup (20 rows; data-alignment decision, w20h RESULTS) | open | | 2026-10-12 |
| static | func_80018448, func_80010E80, func_800252AC, func_80011A64, func_80011D74, func_80018634, func_80022CD4 | open | | w20a near-misses (RESULTS has traced causes); func_8001824C and func_80017D38 landed 2026-10-12 |
| static | func_80016E68/80016EE0/80016F80 (code identical, need an owned .bss block for lib_16320: w20b `ownbss/`), func_8001EF8C, func_80022874, func_8001671C, func_80019F48, func_80016998, func_8001E0E0, func_8001A270 | open | | w20b leftovers; the eight others landed 2026-10-12 |
| static | other remaining boot tail `func_*` (func_800268D0, func_80012D18, func_8001A658, func_80023E9C, func_80012730, func_8001F13C, func_800178B0, func_8001C860 and the rest) | open | | Astra welcome; say so here |
| tooling | static recompilation Phase 1 (runtime, shims, overlay hook) | coordinator | 2026-10-12 | Phase 0 landed (`tools/recomp/`, plan §8) |
