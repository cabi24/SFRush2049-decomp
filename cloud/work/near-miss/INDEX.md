# Single-function near misses

Pipeline score is the historical heuristic (stack offsets ignored), not matching evidence.
Strict words differing counts score.py's full-word differences plus nonzero words beyond
the target extent. Unverified relocations and unresolved symbols remain visible in the
result column: zero differences alone is not a verified match. Unscorable entries sort last.
Each directory contains base.c for the whole translation unit. Verify with:

    python3 tools/cloud/score.py fn cloud/work/near-miss/<name>/base.c <name> --flags "<flags>"

Regenerate (rescores every entry under -O2, -O2 + `-Wab,-r4300_mul`, and the -O1 variants and keeps the
best; run from the repo root): `python3 cloud/work/tools/gen_nearmiss_index.py`.
The `-O1` in older revisions of this file was wrong for the m2c-sweep rows: they need `-O2`.

Rescored 104 entries: 71 unmatched below, 2 strict matches not yet in `cloud/matches/`, 31 now in `cloud/matches/`.

| function | pipeline score | strict words differing | source | flags | score.py result |
|---|---|---|---|---|---|
| func_8008A704 | 120 | 2 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 2/28 words differ |
| func_800A8F38 | 105 | 2 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 2/11 words differ |
| players_frame_update | 50 | 2 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 2/36 words differ |
| players_race_update | 475 | 2 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 2/37 words differ |
| car_select_handler | 5 | 3 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 3/90 words differ |
| func_800CFCA8 | 60 | 3 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 3/81 words differ |
| func_8008C680 | 20 | 4 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 4/40 words differ |
| func_800D0B14 | 90 | 4 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 4/35 words differ |
| task_complete_signal | 100 | 6 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 5/36 words differ (1 extra words (nonzero beyond target length)) |
| func_800966D8 | 35 | 7 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 7/23 words differ |
| func_800D11BC | 340 | 7 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 7/35 words differ |
| func_800EAFDC | 240 | 8 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 8/19 words differ |
| audio_start | 5 | 9 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 9/27 words differ |
| func_8008D0C0 | 55 | 9 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 9/24 words differ |
| func_80091BA8 | 255 | 9 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 9/21 words differ |
| func_800972C4 | 60 | 9 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 9/22 words differ |
| func_800D5828 | 280 | 9 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 9/25 words differ |
| func_800D18D8 | 70 | 10 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 10/41 words differ |
| func_8008A38C | 285 | 11 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 11/22 words differ |
| func_80096BBC | 60 | 11 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 11/27 words differ |
| func_800AB750 | 455 | 12 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 12/32 words differ |
| camera_update_a | 275 | 14 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 14/32 words differ |
| func_800D1248 | 90 | 14 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 14/81 words differ |
| mode_byte_set | 160 | 14 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 14/20 words differ |
| func_800CCE5C | 10 | 15 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 15/40 words differ |
| func_800DC1AC | 145 | 15 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 15/39 words differ |
| camera_smooth_lerp | 415 | 16 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 16/49 words differ |
| engine_rpm_calc | 430 | 16 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 16/17 words differ |
| func_800BB7F4 | 225 | 16 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 15/16 words differ (1 extra words (nonzero beyond target length); unpaired R_MIPS_HI16 for D_8013F1DC at .text+0x38) |
| func_800D4D84 | 30 | 16 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 16/30 words differ |
| input_aux_handler | 80 | 16 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 16/121 words differ |
| tire_sound_update | 345 | 16 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 16/126 words differ |
| func_8008A644 | 110 | 18 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 18/24 words differ |
| func_800C9590 | 435 | 18 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 18/19 words differ |
| func_800DCCE0 | 360 | 18 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 17/28 words differ (1 extra words (nonzero beyond target length)) |
| object_byte71_set_sync | 230 | 18 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 18/20 words differ |
| camera_reset | 220 | 19 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 19/86 words differ |
| func_800B78A4 | 150 | 19 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 18/19 words differ (1 extra words (nonzero beyond target length)) |
| struct_init_and_call | 325 | 19 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 17/17 words differ (2 extra words (nonzero beyond target length)) |
| func_800CF604 | 225 | 20 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 20/38 words differ |
| func_800B73E4 | 210 | 21 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 21/21 words differ |
| game_timer_pause | 260 | 22 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 22/41 words differ |
| func_8008ABE4 | 315 | 23 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 23/36 words differ |
| func_80096B00 | 325 | 23 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 23/23 words differ |
| func_800DC120 | 340 | 23 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 23/35 words differ |
| func_80096C28 | 180 | 24 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 24/32 words differ |
| func_800A5488 | 305 | 24 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 24/29 words differ |
| func_800F84B0 | 395 | 24 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 22/51 words differ (2 extra words (nonzero beyond target length)) |
| entity_state_check | 395 | 25 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 25/30 words differ |
| results_time_display | 400 | 25 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 25/41 words differ |
| save_slot_valid | 305 | 25 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 25/44 words differ |
| state_update_global | 330 | 28 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 27/28 words differ (1 extra words (nonzero beyond target length)) |
| audio_effect_remove | 465 | 29 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 28/34 words differ (1 extra words (nonzero beyond target length)) |
| func_800A7480 | 90 | 30 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 29/34 words differ (1 extra words (nonzero beyond target length); unpaired R_MIPS_HI16 for D_80124FC8 at .text+0x70) |
| func_800AD650 | 210 | 30 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 30/57 words differ |
| func_800C9480 | 195 | 32 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 32/42 words differ |
| particle_position_set | 100 | 32 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 32/43 words differ |
| func_800B0EA0 | 500 | 34 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 34/48 words differ |
| func_800F7448 | 310 | 34 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 34/71 words differ |
| func_80092FE0 | 270 | 35 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 35/47 words differ |
| func_800C1A00 | 245 | 36 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 36/88 words differ |
| entity_cull_check | 280 | 39 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 38/64 words differ (1 extra words (nonzero beyond target length)) |
| func_800C7200 | 460 | 40 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 40/64 words differ |
| track_collision | 115 | 40 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 39/50 words differ (1 extra words (nonzero beyond target length)) |
| func_800D2C10 | 500 | 44 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 43/49 words differ (1 extra words (nonzero beyond target length)) |
| func_800E2F00 | 360 | 44 | m2c sweep | `-g0 -O2 -mips2 -G 0 -non_shared` | 44/107 words differ |
| random_seed_init | 430 | 62 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 61/61 words differ (1 extra words (nonzero beyond target length)) |
| func_800A7E10 | 480 | 66 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 66/176 words differ |
| func_8009D45C | 465 | 75 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 75/171 words differ |
| func_80091B00 | 455 | - | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | StopIteration |
| mode_byte2_set | 160 | - | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | -------------------------^ |

## Matched (now in cloud/matches/)

Strict matches for the source and flags shown (the flags are the header comment of the file in
`cloud/matches/`). Not new ROM coverage: maintainers must check whether each is already locked
and run the splice/image/ROM gates before promotion. `func_800E1540` needs `-Wab,-r4300_mul`;
`func_800ABB58` and `func_800B9F60` use `volatile` reads found by trial and need review.

| function | pipeline score | source | flags | score.py result (rescored) |
|---|---|---|---|---|
| Input_SetAnalogBounds | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| UpdateActiveObjects | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_8008AD04 | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_8008FFD0 | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_8009002C | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_8009E820 | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800A464C | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800A473C | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800A4770 | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800A61B0 | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800A7D6C | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800ABB58 | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800ACBC4 | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800AD5D0 | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800B1F30 | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800B4DA4 | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800B90F8 | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800B9338 | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800B9F60 | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800C4C9C | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800CDDE8 | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800CDE38 | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800D0A34 | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800DC57C | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800DD45C | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800E1540 | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul` | MATCH |
| func_800FBE30 | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| init_wait_completion | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| input_new_data_wrapper | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| resource_update_global | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| transmission_shift | ? | ? | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |

## Strict matches not yet in cloud/matches/

Score 0 under the flags shown but no file in `cloud/matches/` yet; maintainers should review them
(check the function is not already locked) before promotion.

| function | pipeline score | source | flags | score.py result |
|---|---|---|---|---|
| func_8008C5E0 | 150 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
| func_800C54F0 | 5 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
