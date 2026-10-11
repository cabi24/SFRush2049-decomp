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
| game | func_800D816C (+ func_800D91A0 cluster) | Astra | 2026-10-11 | Astra lane 3, research draft `dot_game_caller_cluster_20261011`: D816C canonical 2/699; genuine D91A0 caller 239/965 +1 word; no new claims |
| ovl_a | 8038F648, 80394058, 8039D05C, 803A6F2C, 803A7180, 803A7620, 803A7918 | Astra | 2026-10-11 | PR #311 near-misses (frame-only residuals) |
| ovl_a / ovl_b | everything else in `asm/us/ovl_*` | Astra (default) | 2026-10-11 | coordinator lanes do not take overlay functions unless listed here |
| game | camera_play_script, entity_spawn_init, entity_render_setup, audio_frame_update, engine_sound_sync, func_800D6914, entity_process_main, func_8010FBE0 | coordinator (next wave) | 2026-10-11 | handoff §0 near-miss table; Astra welcome to take any of these: say so here first |
| static | boot tail `func_*` >= 400 B from `w17g/inv_table.md` | coordinator lane w19a | 2026-10-12 | |
| static | w18c not-matched list (`w18c/RESULTS.md`) | coordinator lane w19b | 2026-10-12 | |
| static | osAiSetNextBuffer, __fixunsdfdi/__fixunssfdi owned data, stale inventory rows | coordinator lane w19c | 2026-10-12 | |
| tooling | pytest-xdist + rocky as second CI-equivalent host | coordinator lane w19d | 2026-10-12 | |
| game | fresh `frontier next` singles (four, named in w19e/RESULTS.md) | coordinator lane w19e | 2026-10-12 | |
