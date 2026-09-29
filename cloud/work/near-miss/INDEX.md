# Single-function near misses

Pipeline score is the historical heuristic (stack offsets ignored), not matching evidence.
Strict words differing counts score.py's full-word differences plus nonzero words beyond
the target extent. Unverified relocations and unresolved symbols remain visible in the
result column: zero differences alone is not a verified match. Unscorable entries sort last.
Each directory contains base.c for the whole translation unit. Verify with:

    python3 tools/cloud/score.py fn cloud/work/near-miss/<name>/base.c <name> --flags "<flags>"

Re-rank on x86 Linux:

    python3 tools/cloud/rank_near_miss.py

Rescored 104 entries: 103 remain below. Strict matches found: 1.
Strict matches are reported separately for maintainer review and removed from the ranked worklist.

| function | pipeline score | strict words differing | source | flags | score.py result |
|---|---|---|---|---|---|
| audio_start | 5 | 1 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 1/27 words differ |
| func_800B1F30 | 200 | 1 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 1/55 words differ |
| func_800DD45C | 5 | 1 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 1/20 words differ |
| func_800FBE30 | 110 | 1 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 1/12 words differ |
| func_8008C5E0 | 150 | 2 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 2/40 words differ |
| func_800A7D6C | 40 | 2 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 2/33 words differ |
| car_select_handler | 5 | 3 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 3/90 words differ |
| func_800CFCA8 | 60 | 3 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 3/81 words differ |
| func_800B4DA4 | 20 | 4 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 4/49 words differ |
| resource_update_global | 215 | 4 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 4/9 words differ |
| func_8009002C | 200 | 5 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 4/23 words differ (1 extra words (nonzero beyond target length)) |
| func_800B9338 | 35 | 5 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 5/28 words differ |
| func_800D0B14 | 90 | 6 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 6/35 words differ |
| func_800D4D84 | 30 | 6 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 6/30 words differ |
| transmission_shift | 30 | 6 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 6/62 words differ |
| UpdateActiveObjects | 345 | 7 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 7/48 words differ |
| func_800966D8 | 35 | 7 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 7/23 words differ |
| players_frame_update | 50 | 7 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 7/36 words differ |
| func_80096B00 | 325 | 8 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 7/23 words differ (1 extra words (nonzero beyond target length)) |
| func_8008A38C | 285 | 9 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 9/22 words differ |
| func_8008D0C0 | 55 | 9 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 9/24 words differ |
| func_800972C4 | 60 | 9 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 9/22 words differ |
| func_800A8F38 | 105 | 9 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 9/11 words differ |
| func_800B90F8 | 150 | 9 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 9/13 words differ |
| func_800DC57C | 220 | 10 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 10/43 words differ |
| func_80096BBC | 60 | 11 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 11/27 words differ |
| func_800DC120 | 340 | 11 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 11/35 words differ |
| func_800DC1AC | 145 | 11 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 11/39 words differ |
| func_8008A644 | 110 | 12 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 12/24 words differ |
| func_800ACBC4 | 355 | 12 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 12/21 words differ |
| func_8008FFD0 | 340 | 14 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 14/23 words differ |
| func_800A7480 | 90 | 14 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 14/34 words differ |
| func_800D1248 | 90 | 14 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 14/81 words differ |
| mode_byte2_set | 160 | 14 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 14/20 words differ |
| mode_byte_set | 160 | 14 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 14/20 words differ |
| func_800BB7F4 | 225 | 15 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 14/16 words differ (1 extra words (nonzero beyond target length); unpaired R_MIPS_HI16 for D_8013F1DC at .text+0x38) |
| func_800EAFDC | 240 | 15 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 15/19 words differ |
| func_800A464C | 385 | 16 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 16/32 words differ |
| input_aux_handler | 80 | 16 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 16/121 words differ |
| tire_sound_update | 345 | 16 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 16/126 words differ |
| func_800DCCE0 | 360 | 17 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 17/28 words differ |
| init_wait_completion | 300 | 17 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 17/21 words differ |
| engine_rpm_calc | 430 | 18 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 16/17 words differ (2 extra words (nonzero beyond target length)) |
| func_8008AD04 | 255 | 18 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 16/17 words differ (2 extra words (nonzero beyond target length)) |
| func_800C9590 | 435 | 18 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 18/19 words differ |
| camera_reset | 220 | 19 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 19/86 words differ |
| func_800B73E4 | 210 | 19 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 19/21 words differ |
| func_800B78A4 | 150 | 19 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 18/19 words differ (1 extra words (nonzero beyond target length)) |
| input_new_data_wrapper | 310 | 19 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 15/15 words differ (4 extra words (nonzero beyond target length)) |
| func_800CF604 | 225 | 20 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 20/38 words differ |
| func_80091BA8 | 255 | 23 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 18/21 words differ (5 extra words (nonzero beyond target length); unpaired R_MIPS_HI16 for D_80110244 at .text+0x50) |
| func_8009E820 | 210 | 24 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 24/37 words differ |
| func_800A5488 | 305 | 24 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 24/29 words differ |
| func_800A61B0 | 210 | 24 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 24/37 words differ |
| func_800B9F60 | 235 | 24 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 24/43 words differ |
| func_800F84B0 | 395 | 24 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 22/51 words differ (2 extra words (nonzero beyond target length)) |
| camera_smooth_lerp | 415 | 25 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 25/49 words differ |
| func_800CDDE8 | 410 | 25 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 19/20 words differ (6 extra words (nonzero beyond target length)) |
| func_800CDE38 | 410 | 25 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 19/20 words differ (6 extra words (nonzero beyond target length)) |
| save_slot_valid | 305 | 25 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 25/44 words differ |
| object_byte71_set_sync | 230 | 26 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 20/20 words differ (6 extra words (nonzero beyond target length)) |
| func_8008A704 | 120 | 28 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 28/28 words differ |
| func_800A473C | 150 | 28 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 13/13 words differ (15 extra words (nonzero beyond target length)) |
| audio_effect_remove | 465 | 29 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 28/34 words differ (1 extra words (nonzero beyond target length)) |
| func_800D5828 | 280 | 30 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 24/25 words differ (6 extra words (nonzero beyond target length)) |
| func_800E1540 | 200 | 30 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 24/24 words differ (6 extra words (nonzero beyond target length)) |
| struct_init_and_call | 325 | 30 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 16/17 words differ (14 extra words (nonzero beyond target length); unpaired R_MIPS_HI16 for D_80153F10 at .text+0x40) |
| func_800C4C9C | 270 | 31 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 23/23 words differ (8 extra words (nonzero beyond target length)) |
| func_800C9480 | 195 | 32 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 32/42 words differ |
| particle_position_set | 100 | 32 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 32/43 words differ |
| camera_update_a | 275 | 33 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 32/32 words differ (1 extra words (nonzero beyond target length)) |
| func_80091B00 | 455 | 33 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 33/42 words differ |
| func_800D0A34 | 445 | 33 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 27/27 words differ (6 extra words (nonzero beyond target length)) |
| Input_SetAnalogBounds | 420 | 34 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 32/34 words differ (2 extra words (nonzero beyond target length)) |
| func_800AD5D0 | 400 | 34 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 32/32 words differ (2 extra words (nonzero beyond target length)) |
| func_800B0EA0 | 500 | 34 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 34/48 words differ |
| func_800F7448 | 310 | 34 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 34/71 words differ |
| task_complete_signal | 100 | 34 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 34/36 words differ |
| func_80092FE0 | 270 | 35 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 35/47 words differ |
| func_800C1A00 | 245 | 36 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 36/88 words differ |
| entity_state_check | 395 | 37 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 30/30 words differ (7 extra words (nonzero beyond target length)) |
| players_race_update | 475 | 37 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 37/37 words differ |
| func_800D11BC | 340 | 38 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 34/35 words differ (4 extra words (nonzero beyond target length)) |
| results_time_display | 400 | 38 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 34/41 words differ (4 extra words (nonzero beyond target length)) |
| state_update_global | 330 | 38 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 28/28 words differ (10 extra words (nonzero beyond target length); unpaired R_MIPS_HI16 for D_80117358 at .text+0x6c) |
| entity_cull_check | 280 | 39 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 38/64 words differ (1 extra words (nonzero beyond target length)) |
| func_800ABB58 | 415 | 40 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 30/30 words differ (10 extra words (nonzero beyond target length)) |
| func_800C7200 | 460 | 40 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 40/64 words differ |
| func_800D2C10 | 500 | 40 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 40/49 words differ |
| track_collision | 115 | 40 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 39/50 words differ (1 extra words (nonzero beyond target length)) |
| func_800A4770 | 120 | 41 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 20/20 words differ (21 extra words (nonzero beyond target length)) |
| game_timer_pause | 260 | 41 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 38/41 words differ (3 extra words (nonzero beyond target length)) |
| func_8008C680 | 20 | 42 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 40/40 words differ (2 extra words (nonzero beyond target length)) |
| func_8008ABE4 | 315 | 43 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 36/36 words differ (7 extra words (nonzero beyond target length)) |
| func_800AB750 | 455 | 43 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 32/32 words differ (11 extra words (nonzero beyond target length)) |
| func_80096C28 | 180 | 45 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 30/32 words differ (15 extra words (nonzero beyond target length)) |
| func_800CCE5C | 10 | 46 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 40/40 words differ (6 extra words (nonzero beyond target length)) |
| func_800D18D8 | 70 | 49 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 41/41 words differ (8 extra words (nonzero beyond target length); unpaired R_MIPS_HI16 for D_80110258 at .text+0xa0) |
| func_800AD650 | 210 | 58 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 51/57 words differ (7 extra words (nonzero beyond target length)) |
| random_seed_init | 430 | 62 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 61/61 words differ (1 extra words (nonzero beyond target length)) |
| func_800A7E10 | 480 | 66 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 66/176 words differ |
| func_8009D45C | 465 | 99 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | 99/171 words differ |
| func_800E2F00 | 360 | 100 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` | 100/107 words differ |

## Strict matches removed from the worklist

These are matches for the source and flags shown, not new ROM coverage.
Maintainers must check whether they are already locked and run the splice/image/ROM
gates before promotion. Source directories are retained.

| function | pipeline score | strict words differing | source | flags | score.py result |
|---|---|---|---|---|---|
| func_800C54F0 | 5 | 0 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` | MATCH |
