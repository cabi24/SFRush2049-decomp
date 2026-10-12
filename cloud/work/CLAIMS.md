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
| game | func_800D816C (+ func_800D91A0 cluster) | Astra | 2026-10-11 | #322: D816C 699/699 in its group, D91A0 166/965; packet `cloud/work/frontier/dot_d816c_stock_delay_20261011` imported by w24p (research). D816C is internal to D91A0 (private ABI): in the whole-program unit it fails kept (691/699) and internal with the #322 caller (570/699, DA0B4 inlined); lands with D91A0. Coordinator's w17e notes in `cloud/work/frontier/w17e/RESULTS.md` |
| ovl_a | 803A7180 (frame research, paused), 8039A4D4 (623/716), 803A6B50 (3/241 words, frame 160 vs 144; #330 packet imported, no coverage) | Astra | 2026-10-12 | PR #311 near-misses; 80394058 (#315), 803A6F2C/803A7620 (#316), 8039D05C/8039D494/8039ED20 (#318), 803A7918 (#321) landed in image A; image B func_803936A8 (#319) landed 2026-10-12; w22p landed 8038F648 (#324), 8038C93C (#329), 8039C140 (#326) and 8039B560 (#328; merged into the superseding `dot_runtime_a_text_helpers_20261006`, which absorbed the config_caller/config_parent/scene_init/cleanup groups) and image B 803908D0 (#327) 2026-10-12; w24p landed 803990D0 (#331, superseding `dot_runtime_a_menu_state_20261006`, 4 members), 803A4340 (#332), 8039BB80 (#333, `dot_runtime_a_text_helpers_20261006` now 12 members) and image B 8038D3A4 (#334, group `dot_runtime_b_damage_20261012`) 2026-10-12 |
| ovl_b | func_803925D0 (HUD initializer, 13/177), func_80392FE4 (health bar, 14/333) | Astra | 2026-10-12 | paused per #334; no coverage |
| ovl_a / ovl_b | everything else in `asm/us/ovl_*` | Astra (default) | 2026-10-11 | coordinator lanes do not take overlay functions unless listed here |
| game | world_effect_update (12 words, needs a block boundary), audio_frame_update (22 words), race_position_update (50 words, s4 register variable) | open | | w20d RESULTS 2026-10-12; assign_drones landed |
| game | camera_play_script (27 rows; w18d hypothesis stands) | open | | engine_sound_sync + audio_effect_setup + func_80098ADC landed 2026-10-12 (w23f) |
| game | func_8010FBE0, entity_process_main | open | | w20e RESULTS 2026-10-12 |
| game | func_800EC190 (2 words), func_8008AD6C (29), func_800A1BB4 (35), wheel_setup_initial, ghost_race_setup, audio_fade_control unit (6/8) | open | | w21g/w20f RESULTS; func_800D63EC, func_800B3704, func_800D6530 landed 2026-10-12 |
| game | controller_poll (11 words; t2/t3 tie + as1 order), entity_render_setup (8 rows; live range needs block 17 AND a split), func_800EC190 (2) | open | | w23e RESULTS 2026-10-12 |
| game | func_80090308 (210/282; frame 160 vs 272) + entity_physics_update (EQUAL only with an empty-goto device, not accepted) + save_write_data group | open | | w23d RESULTS; buffer_swap landed 2026-10-12 |
| tooling | remaining libultra leads: dll_init/dll_update/dll_reschedule (probably __osTimerServicesInit/__osTimerInterrupt/__osSetTimerIntr) and the rest of docs/SYMBOL_MISATTRIBUTION.md; gfx_modes object_render explicit __ll_lshift calls -> natural shifts | open | | w23g landed 24 more renames + game side 2026-10-12; game_static_legacy.us.txt is empty |
| static | func_80018448 (held temps or emission order; no FIFO ring produces retail), func_80012D18 (two named u32 position variables must survive copy propagation) | open | | w23a RESULTS; func_80018634 landed 2026-10-12 |
| static | func_8001A658 (synthHandle, 2770: span store-PRE, midi split pieces, fader end pointer), func_8001F13C (voiceAllocate, 2045: voice*416 PRE hoist; needs a 7.1 PRE oracle) | open | | w23b RESULTS 2026-10-12 |
| static | func_8001558C (180, loop-invariant u16 copy placement), func_80010DD8, func_80011910, func_80012234, func_80010450; 0x26F60 segment (4 fns, 5,232 B, needs conversion); pre-tail boot code (36 slots, 13,152 B, untriaged) | open | | w23c "remaining boot tail" table 2026-10-12 |
| static | other remaining boot tail `func_*` (func_800268D0, func_80012D18, func_8001A658, func_80023E9C, func_80012730, func_8001F13C, func_800178B0, func_8001C860 and the rest) | open | | Astra welcome; say so here |
| tooling | static recompilation Phase 4 (IPA-aware replacement build; RT64 with lockstep as test mode; more scripts) | coordinator | 2026-10-12 | Phase 3 landed (per-call capture, 4 scripts, static binding; plan §11) |
| tooling | recomp o32 wrapper generator with guest-pointer (`rush_addr`) mode: fold into `replace.py` native mode rather than a second generator; RDRAM accessors (`tools/recomp/rdram.h`, `game_rdram.h`) landed from #325 | Astra (#323 reservation) | 2026-10-12 | w22p; `tests/recomp/test_rdram.py`; the accessors own README section `RDRAM accessors` |

| ovl_b | func_8038CB20 | Astra lane 2 | 2026-10-12 | Trail callback, 333/333 strict MATCH + 28 owned-data bytes in dot_runtime_b_trail_20261012; +1,332 fresh candidate bytes, zero accepted/ROM gain; two exact-residual unused s32 locals disclosed |
