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
| game | world_effect_update, audio_frame_update, assign_drones, race_position_update | coordinator w20d | 2026-10-12 | wave 20 |
| game | engine_sound_sync, func_8010FBE0, func_800D6914, entity_process_main | coordinator w20e | 2026-10-12 | wave 20 |
| game | fresh singles/units from `frontier next` not listed elsewhere here | coordinator w20f | 2026-10-12 | wave 20; picks recorded in `cloud/work/frontier/w20f/RESULTS.md` |
| game | entity_spawn_init | coordinator w20g | 2026-10-12 | wave 20 |
| game | camera_play_script, entity_render_setup | coordinator w20h | 2026-10-12 | wave 20 |
| static | w19a near-miss list (func_8001824C, func_80018448, func_80010E80, func_800252AC, func_80011A64, func_80011D74, func_80017D38, func_80018634, func_80022CD4) | coordinator w20a | 2026-10-12 | wave 20, IDO 7.1 |
| static | w19b near-miss list + w19a untried D10 set (func_800177EC/80017824, 80016E68/80016F80, 800154A4/8001536C, 800230B0, 8001DC08, 80022160, 8001EF8C, 80016998, 80019F48, 8001E0E0, 80022874, 8001DA74, 8001A270, 8001671C) | coordinator w20b | 2026-10-12 | wave 20, IDO 7.1 |
| static | code-identical-not-lockable set (func_80021548, func_80023BDC owned jtbls; func_8001BE14, func_8001B9F8; func_800114C0, func_80011104; func_80013DEC) | coordinator w20c | 2026-10-12 | wave 20 tooling/data lane |
| static | other remaining boot tail `func_*` (func_800268D0, func_80012D18, func_8001A658, func_80023E9C, func_80012730, func_8001F13C, func_800178B0, func_8001C860 and the rest) | open | | Astra welcome; say so here |
| tooling | static recompilation Phase 0 (`tools/recomp/`) | coordinator w20r | 2026-10-12 | `docs/plans/2026-10-12-recomp-port-plan.md` |
