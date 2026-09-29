# IPA call groups (IDO -O3 whole-program)

Each dir: group.json (members, keep list, flags) + group.c (all members after one shared prelude) + STATUS.md. Verify with:

    python3 tools/cloud/score.py group cloud/work/ipa-groups/<id>

| group | insns | status |
|---|---|---|
| menu_highlight_set | 18 | built; words differing per member: {"menu_highlight_set": 18} |
| save_context_stub | 27 | built; words differing per member: {"save_context_stub": 26} |
| session_leave | 29 | built; words differing per member: {"session_leave": 28} |
| menu_back | 33 | BUILD ERROR: group build failed: cfe: Error: group.c, line 3713: Dereferenced a non-pointer. temp_a2 = *(*( s32 *)((s8 * |
| dynamic_difficulty | 47 | no seed: m2c produced no seed for dynamic_difficulty |
| highscore_entry_anim | 47 | BUILD ERROR: group build failed: cfe: Error: group.c, line 3707: 'saved_reg_fp' undefined; reoccurrences will not be rep |
| catchup_logic | 57 | no seed: m2c produced no seed for catchup_logic |
| reconnect_attempt | 106 | no seed: m2c produced no seed for reconnect_attempt |
| spark_effect | 132 | built; words differing per member: {"spark_effect": 132} |
| car_collision_init | 137 | BUILD ERROR: group build failed: cfe: Error: group.c, line 3720: Dereferenced a non-pointer. *((*( s32 *)((s8 *)(arg0) + |
| difficulty_scaling | 178 | BUILD ERROR: group build failed: cfe: Error: group.c, line 3714: Unacceptable operand of a multiplicative operator. (*(  |
| func_8008B640 | 201 | BUILD ERROR: group build failed: cfe: Error: group.c, line 3763: Syntax Error } --------^ |
| func_8010A7A4 | 242 | no seed: m2c produced no seed for ping_measurement |
| name_entry_screen | 247 | no seed: m2c produced no seed for name_entry_screen |
| championship_standings | 279 | BUILD ERROR: group build failed: cfe: Warning 709: group.c, line 3725: Incompatible pointer type assignment temp_t6 = (s |
| session_host | 303 | built; words differing per member: {"session_host": 302} |
| func_800D2FA8 | 350 | BUILD ERROR: group build failed: cfe: Warning 712: group.c, line 3775: illegal combination of pointer and integer temp_v |
| audio_frame_update | 361 | BUILD ERROR: group build failed: cfe: Warning 712: group.c, line 3728: illegal combination of pointer and integer temp_v |
| car_cg_height_set | 481 | BUILD ERROR: group build failed: cfe: Warning 712: group.c, line 3782: illegal combination of pointer and integer var_v1 |
| func_800B9B64 | 487 | BUILD ERROR: group build failed: cfe: Warning 712: group.c, line 3717: illegal combination of pointer and integer temp_v |
| cpak_init | 513 | BUILD ERROR: group build failed: cfe: Warning 712: group.c, line 3807: illegal combination of pointer and integer var_t4 |
| func_800E4300 | 532 | BUILD ERROR: group build failed: cfe: Error: group.c, line 3727: 'arg3' undefined; reoccurrences will not be reported. a |
| func_800E681C | 539 | BUILD ERROR: group build failed: cfe: Error: group.c, line 3875: Dereferenced a non-pointer. if (((s8) var_a0->pad0EC[0x |
| car_collision_update | 597 | BUILD ERROR: group build failed: cfe: Warning 709: group.c, line 3741: Incompatible pointer type assignment var_v1 = (u8 |
| controller_poll | 602 | BUILD ERROR: group build failed: cfe: Warning 712: group.c, line 3788: illegal combination of pointer and integer var_t5 |
| audio_frame_sync | 652 | BUILD ERROR: group build failed: cfe: Warning 712: group.c, line 3742: illegal combination of pointer and integer var_v0 |
| MP_TargetSteerPos | 660 | BUILD ERROR: group build failed: cfe: Warning 712: group.c, line 3815: illegal combination of pointer and integer temp_v |
| billboard_render | 665 | no seed: m2c produced no seed for billboard_render |
| func_800E56F8 | 693 | BUILD ERROR: group build failed: cfe: Error: group.c, line 3779: Dereferenced a non-pointer. if ((state_word_a & 0x40000 |
| audio_pitch_adjust | 752 | no seed: m2c produced no seed for entity_ai_pathfind |
| func_800AD4C8 | 791 | BUILD ERROR: group build failed: cfe: Warning 712: group.c, line 3783: illegal combination of pointer and integer *ipa_s |
| func_800E92C8 | 805 | BUILD ERROR: unresolved symbol fabsf |
| func_800F0F44 | 815 | no seed: m2c produced no seed for func_800F1210 |
| audio_doppler_calc | 849 | no seed: m2c produced no seed for world_velocity_integrate |
| func_8008705C | 869 | no seed: m2c produced no seed for func_80086A50 |
| draw_number | 973 | no seed: m2c produced no seed for menu_confirm_render |
| best_times_display | 1068 | no seed: m2c produced no seed for mode_select_handler |
| camera_scene_manager | 1106 | BUILD ERROR: group build failed: cfe: Error: group.c, line 3906: Unacceptable operand of &. if ((temp_v1_2 > 0) // (var_ |
| camera_aspect_ratio | 1137 | BUILD ERROR: group build failed: cfe: Warning 712: group.c, line 3777: illegal combination of pointer and integer temp_t |
| car_crash_response | 1286 | BUILD ERROR: group build failed: cfe: Error: group.c, line 3757: The number of arguments doesn't agree with the number i |
