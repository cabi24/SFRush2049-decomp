# Single-function near misses

Score = pipeline score (permuter units, 0 = match; stack offsets ignored). Each dir has base.c, the best known source for the WHOLE translation unit. Verify with:

    python3 tools/cloud/score.py fn cloud/work/near-miss/<name>/base.c <name> --flags "<flags>"

| function | score | source | flags |
|---|---|---|---|
| func_800DD45C | 5 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| audio_start | 5 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800C54F0 | 5 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| car_select_handler | 5 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800CCE5C | 10 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_8008C680 | 20 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800B4DA4 | 20 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800D4D84 | 30 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| transmission_shift | 30 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800966D8 | 35 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800B9338 | 35 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800A7D6C | 40 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| players_frame_update | 50 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_8008D0C0 | 55 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800972C4 | 60 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_80096BBC | 60 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800CFCA8 | 60 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800D18D8 | 70 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| input_aux_handler | 80 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800A7480 | 90 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800D0B14 | 90 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800D1248 | 90 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| task_complete_signal | 100 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| particle_position_set | 100 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800A8F38 | 105 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800FBE30 | 110 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_8008A644 | 110 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| track_collision | 115 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800A4770 | 120 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_8008A704 | 120 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800DC1AC | 145 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800A473C | 150 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800B90F8 | 150 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800B78A4 | 150 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_8008C5E0 | 150 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| mode_byte_set | 160 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| mode_byte2_set | 160 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_80096C28 | 180 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800C9480 | 195 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_8009002C | 200 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800E1540 | 200 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800B1F30 | 200 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800B73E4 | 210 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_8009E820 | 210 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800A61B0 | 210 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800AD650 | 210 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| resource_update_global | 215 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800DC57C | 220 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| camera_reset | 220 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800BB7F4 | 225 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800CF604 | 225 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| object_byte71_set_sync | 230 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800B9F60 | 235 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800EAFDC | 240 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800C1A00 | 245 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_8008AD04 | 255 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_80091BA8 | 255 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| game_timer_pause | 260 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800C4C9C | 270 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_80092FE0 | 270 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| camera_update_a | 275 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800D5828 | 280 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| entity_cull_check | 280 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_8008A38C | 285 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| init_wait_completion | 300 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800A5488 | 305 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| save_slot_valid | 305 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| input_new_data_wrapper | 310 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800F7448 | 310 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_8008ABE4 | 315 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| struct_init_and_call | 325 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_80096B00 | 325 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| state_update_global | 330 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_8008FFD0 | 340 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800D11BC | 340 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800DC120 | 340 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| UpdateActiveObjects | 345 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| tire_sound_update | 345 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800ACBC4 | 355 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800DCCE0 | 360 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800E2F00 | 360 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800A464C | 385 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| entity_state_check | 395 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800F84B0 | 395 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800AD5D0 | 400 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| results_time_display | 400 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800CDDE8 | 410 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800CDE38 | 410 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800ABB58 | 415 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| camera_smooth_lerp | 415 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| Input_SetAnalogBounds | 420 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| engine_rpm_calc | 430 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| random_seed_init | 430 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800C9590 | 435 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800D0A34 | 445 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800AB750 | 455 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_80091B00 | 455 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800C7200 | 460 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| audio_effect_remove | 465 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_8009D45C | 465 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| players_race_update | 475 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
| func_800A7E10 | 480 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800B0EA0 | 500 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| func_800D2C10 | 500 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
