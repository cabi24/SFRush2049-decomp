void func_800C9150(void);
void func_800CA2F8(void);

static void initGameState(void)
{
    s32 i;

    if (state_word_a & 0x1) {
        func_800CA2F8();
        D_801461F8 = 0;
        D_80146205 = 0;
        D_80149414 = 0;
        D_80149774 = 0;
        D_80149794 = 0;
        D_801497C4 = 0;
        tire_compound_set();
        if (!D_80156994) {
            hud_setup(32, 16, 16, 5, 64, 1.0f, 200.0f, 0);
            func_800C885C();
        }
        display_enable(1);
        active_player_count = 1;
        gNumWindows = 1;
        init_state_begin();
        viScheduleTick(30.0f);
        game_mode_handler();
        game_mode_handler();
        game_mode_handler();
        func_800C9480();
        func_800B5570(0x2);
    } else if (state_word_a & 0x2) {
        func_800CA2F8();
        player_cleanup_slots();
        tire_compound_set();
        if (!D_80156994) {
            hud_setup(32, 16, 16, 5, 64, 1.0f, 200.0f, 0);
            func_800C885C();
        }
        speed_mode1_wrapper(D_80146108[13] / 10.0f, 0.1f);
        display_enable(1);
        resource_slots_clear_multiple();
        player_state_set(-1, 0);
        player_state_set(0, 1);
        player_mode_set(-1, 1);
    } else if (state_word_a & 0x40000000) {
    } else if (state_word_a & 0x4) {
        player_cleanup_slots();
        tire_compound_set();
        if (!D_80156994) {
            hud_setup(32, 16, 16, 5, 64, 1.0f, 200.0f, 0);
            func_800C885C();
        }
        speed_mode1_wrapper(D_80146108[13] / 10.0f, 0.1f);
        display_enable(1);
        resource_slots_clear_multiple();
        if (!(D_801174BC & 0x7cdfc0)) {
            for (i = 0; i < active_player_count; i++) {
                if (input_rec0[i].save) {
                    func_800A3424((*input_rec0[i].save)->fileh, 0);
                }
            }
        }
        player_state_set(-1, 0);
        player_state_set(0, 1);
        player_mode_set(-1, 1);
        if (D_80156994 && !(D_801174BC & 0x7c0000) && D_80146108[14] < 13) {
            speed_mode0_wrapper(D_80146108[12] / 10.0f, 0.05f);
            sync_entry_register(6, 1);
        }
        D_80151AD8 = 0;
    } else if (state_word_a & 0x8) {
        EnvObj **n;
        for (n = D_8012E6E0; n; n = (*n)->next) {
            object_render_cleanup(n);
        }
        scene_cleanup_slots();
        player_cleanup_slots();
        tire_compound_set();
        if (!D_80156994) {
            hud_setup(8, 8, 0, 0, 0, 1.0f, 200.0f, 0);
        }
        display_enable(0);
        if (D_80156994) {
            InitMaxPath();
        }
    } else if (state_word_a & 0x10) {
        player_state_set(-1, 0);
        player_state_set(0, 1);
        player_mode_set(-1, 1);
    } else if (state_word_a & 0x20) {
        s32 p;

        for (p = 0; p < active_player_count; p++) {
            player_state_set(p, 1);
            player_mode_set(p, 1);
        }
        for (; p < 4; p++) {
            player_state_set(p, 0);
            if (p > 1) {
            }
        }
    } else if (state_word_a & 0x40) {
        player_state_set(-1, 0);
        player_state_set(0, 1);
    } else if (state_word_a & 0x200) {
    } else if (state_word_a & 0x400) {
    } else if (state_word_a & 0x800) {
    } else if (state_word_a & 0x1000) {
    } else if (state_word_a & 0x2000) {
    } else if (state_word_a & 0x8000000) {
    } else if (state_word_a & 0x4000) {
        audio_frame_sync(57, 0, 0, 0, 0);
        func_800C9150();
    } else if (state_word_a & 0x100) {
        s32 p;

        audio_frame_sync(56, 0, 0, 0, 0);
        for (p = 0; p < active_player_count; p++) {
            player_state_set(p, 1);
            player_mode_set(p, 1);
        }
        for (; p < 4; p++) {
            player_state_set(p, 0);
            if (p > 1) {
            }
        }
    } else if (state_word_a & 0x80) {
        audio_frame_sync(60, 0, 0, 0, 0);
        player_state_set(-1, 0);
        player_state_set(0, 1);
        func_800C9150();
    } else if (state_word_a & 0x40000) {
        if (!D_80114650) {
            scene_cleanup_slots();
            player_cleanup_slots();
            tire_compound_set();
        }
        display_enable(0);
        if (!D_80114650) {
            if (!D_80156994 && gameplay_mode != 6 && gameplay_mode != 4 && gameplay_mode != 5) {
                hud_setup(8, 8, 0, 0, 0, 1.0f, 200.0f, 0);
            }
            if (!D_80156994 && gameplay_mode == 6) {
                hud_speed_display(5, 64, 1.0f, 200.0f, 0);
            }
            visual_objects_update(1);
        }
        if (gameplay_mode == 6 || gameplay_mode == 4 || gameplay_mode == 5 || D_80156994) {
            InitMaxPath();
        }
        if (!D_80114650) {
            object_create(13);
            object_create(10);
            object_create(11);
            object_create(12);
            object_create(4);
            object_create(1);
            object_create(2);
            object_create(3);
            object_create(9);
            object_create(8);
            object_create(5);
            object_create(6);
            object_create(7);
        }
        player_mode_set(-1, 0);
    } else if (state_word_a & 0x1000000) {
        player_mode_set(-1, 1);
    } else if (state_word_a & 0x800000) {
        SaveServer(1, 0);
        visual_objects_update(1);
    } else if (state_word_a & 0x20000000) {
        scene_cleanup_slots();
        tire_compound_set();
        audio_frame_sync(55, 0, 0, 0, 0);
    }
    if (state_word_a & 0x7c03fffe) {
        func_800A7480(1000, 999, playgame_settings[19][0], playgame_settings[19][1], playgame_settings[19][2], 0xff, 0);
        func_800A7480(1000, 999, playgame_settings[19][0], playgame_settings[19][1], playgame_settings[19][2], 0xff, 1);
    }
}

void playgame_state_change(void)        /* updateGameState */
{
    if (state_word_b != state_word_a) {
        state_change_preprocess();
        state_word_a = state_word_b;
        initGameState();
    }
}

void func_800C9150(void)
{
}

void func_800CA2F8(void)
{
}
