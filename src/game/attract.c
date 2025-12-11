/**
 * attract.c - Attract mode (demo/title screen) for Rush 2049 N64
 *
 * Handles attract sequence, demo gameplay, and title screens.
 * Based on arcade attract.c patterns.
 */

#include "game/attract.h"
#include "game/hiscore.h"
#include "game/menu.h"
#include "types.h"

/* Global state */
AttractState gAttract;

/* Default attract sequence */
AttractItem gDefaultSequence[] = {
    { ATTRACT_STATE_LOGO,       ATTRACT_LOGO_TIME,      0 },
    { ATTRACT_STATE_HISCORE,    ATTRACT_HISCORE_TIME,   0 },
    { ATTRACT_STATE_CREDITS,    ATTRACT_CREDITS_TIME,   0 },
    { ATTRACT_STATE_DEMO,       ATTRACT_DEMO_TIME,      0 },
    { ATTRACT_STATE_HISCORE,    ATTRACT_HISCORE_TIME,   0 },
    { ATTRACT_STATE_JOIN,       ATTRACT_JOIN_TIME,      0 },
};
const s32 gDefaultSequenceLength = sizeof(gDefaultSequence) / sizeof(gDefaultSequence[0]);

/* Game credits */
const CreditItem gGameCredits[] = {
    { "SAN FRANCISCO RUSH 2049",    NULL,               1 },    /* Header */
    { NULL,                         NULL,               2 },    /* Spacer */
    { "ATARI GAMES",               "DEVELOPED BY",      1 },
    { NULL,                         NULL,               2 },
    { "PROGRAMMING",                NULL,               1 },
    { "RUSH TEAM",                  NULL,               0 },
    { NULL,                         NULL,               2 },
    { "ART",                        NULL,               1 },
    { "RUSH TEAM",                  NULL,               0 },
    { NULL,                         NULL,               2 },
    { "SOUND",                      NULL,               1 },
    { "RUSH TEAM",                  NULL,               0 },
    { NULL,                         NULL,               2 },
    { "N64 PORT",                   NULL,               1 },
    { "MIDWAY",                     NULL,               0 },
};
const s32 gNumCredits = sizeof(gGameCredits) / sizeof(gGameCredits[0]);

/* Demo track rotation */
static const u8 sDemoTracks[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
static s32 sDemoTrackIndex = 0;

/* ========== Initialization ========== */

void attract_init(void) {
    s32 i;

    /* Clear state */
    for (i = 0; i < (s32)sizeof(gAttract); i++) {
        ((u8 *)&gAttract)[i] = 0;
    }

    gAttract.state = ATTRACT_STATE_INIT;
    gAttract.sequence = gDefaultSequence;
    gAttract.sequence_length = gDefaultSequenceLength;

    /* Initialize logo state */
    gAttract.logo.alpha = 0.0f;
    gAttract.logo.scale = 1.0f;
    gAttract.logo.rotation = 0.0f;

    /* Initialize credits scroll */
    gAttract.credits_scroll = 0.0f;
    gAttract.credits_speed = 1.0f;
}

void attract_reset(void) {
    gAttract.state = ATTRACT_STATE_INIT;
    gAttract.sequence_index = 0;
    gAttract.state_timer = 0;
    gAttract.total_timer = 0;
    gAttract.flags = 0;

    /* Reset demo state */
    gAttract.demo.active = 0;
    gAttract.demo.frame = 0;

    /* Reset logo */
    gAttract.logo.alpha = 0.0f;
    gAttract.logo.animation = LOGO_ANIM_NONE;

    /* Reset credits */
    gAttract.credits_scroll = 0.0f;
    gAttract.credits_index = 0;

    /* Reset high score display */
    gAttract.hiscore_track = 0;
    gAttract.hiscore_timer = 0;
}

void attract_start(void) {
    attract_reset();
    gAttract.flags |= ATTRACT_FLAG_ACTIVE;
    gAttract.sequence_index = 0;

    /* Start with first state in sequence */
    if (gAttract.sequence_length > 0) {
        attract_set_state(gAttract.sequence[0].state);
    } else {
        attract_set_state(ATTRACT_STATE_LOGO);
    }
}

void attract_stop(void) {
    gAttract.flags &= ~ATTRACT_FLAG_ACTIVE;
    gAttract.state = ATTRACT_STATE_INIT;

    /* Stop demo if running */
    if (gAttract.demo.active) {
        attract_stop_demo();
    }
}

/* ========== Update and Draw ========== */

void attract_update(void) {
    if (!(gAttract.flags & ATTRACT_FLAG_ACTIVE)) {
        return;
    }

    /* Update timers */
    gAttract.state_timer++;
    gAttract.total_timer++;

    /* Check for state timeout */
    if (gAttract.state_timer >= gAttract.state_duration) {
        attract_next_state();
        return;
    }

    /* Update based on current state */
    switch (gAttract.state) {
        case ATTRACT_STATE_INIT:
            attract_next_state();
            break;

        case ATTRACT_STATE_LOGO:
            attract_update_logo();
            break;

        case ATTRACT_STATE_CREDITS:
            attract_update_credits();
            break;

        case ATTRACT_STATE_HISCORE:
            attract_update_hiscore();
            break;

        case ATTRACT_STATE_DEMO:
            attract_update_demo();
            break;

        case ATTRACT_STATE_JOIN:
            attract_update_join();
            break;

        case ATTRACT_STATE_TRANSITION:
            attract_update_fade();
            if (gAttract.fade_alpha <= 0.0f || gAttract.fade_alpha >= 1.0f) {
                attract_next_state();
            }
            break;
    }

    /* Handle input (start button exits attract) */
    attract_handle_input();
}

void attract_draw(void) {
    if (!(gAttract.flags & ATTRACT_FLAG_ACTIVE)) {
        return;
    }

    switch (gAttract.state) {
        case ATTRACT_STATE_LOGO:
            attract_draw_logo();
            break;

        case ATTRACT_STATE_CREDITS:
            attract_draw_credits();
            break;

        case ATTRACT_STATE_HISCORE:
            attract_draw_hiscore();
            break;

        case ATTRACT_STATE_DEMO:
            attract_draw_demo();
            break;

        case ATTRACT_STATE_JOIN:
            attract_draw_join();
            break;

        case ATTRACT_STATE_TRANSITION:
            /* Draw fade overlay */
            {
                u32 alpha = (u32)(gAttract.fade_alpha * 255.0f);
                u32 color = (alpha << 24) | 0x000000;
                ui_draw_box_filled(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color, color);
            }
            break;

        default:
            break;
    }

    /* Always show "Press Start" in attract mode (except during demo) */
    if (gAttract.state != ATTRACT_STATE_DEMO) {
        /* Blink the text */
        if ((gAttract.total_timer / 30) & 1) {
            ui_draw_text(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 20,
                        "PRESS START", COLOR_YELLOW, ALIGN_CENTER);
        }
    }
}

/* ========== State Management ========== */

void attract_set_state(u8 state) {
    gAttract.prev_state = gAttract.state;
    gAttract.state = state;
    gAttract.state_timer = 0;

    /* Set duration from sequence or default */
    if (gAttract.sequence_index < gAttract.sequence_length) {
        gAttract.state_duration = gAttract.sequence[gAttract.sequence_index].duration;
    } else {
        /* Default durations */
        switch (state) {
            case ATTRACT_STATE_LOGO:
                gAttract.state_duration = ATTRACT_LOGO_TIME;
                break;
            case ATTRACT_STATE_CREDITS:
                gAttract.state_duration = ATTRACT_CREDITS_TIME;
                break;
            case ATTRACT_STATE_HISCORE:
                gAttract.state_duration = ATTRACT_HISCORE_TIME;
                break;
            case ATTRACT_STATE_DEMO:
                gAttract.state_duration = ATTRACT_DEMO_TIME;
                break;
            case ATTRACT_STATE_JOIN:
                gAttract.state_duration = ATTRACT_JOIN_TIME;
                break;
            default:
                gAttract.state_duration = 60 * 5;
                break;
        }
    }

    /* State-specific initialization */
    switch (state) {
        case ATTRACT_STATE_LOGO:
            attract_show_logo();
            break;

        case ATTRACT_STATE_CREDITS:
            attract_show_credits();
            break;

        case ATTRACT_STATE_HISCORE:
            attract_show_hiscore();
            break;

        case ATTRACT_STATE_DEMO:
            attract_start_demo();
            break;

        case ATTRACT_STATE_JOIN:
            attract_show_join();
            break;

        case ATTRACT_STATE_TRANSITION:
            gAttract.fade_alpha = 0.0f;
            gAttract.fade_in = 0;
            break;
    }
}

void attract_next_state(void) {
    gAttract.sequence_index++;

    /* Loop back to beginning of sequence */
    if (gAttract.sequence_index >= gAttract.sequence_length) {
        gAttract.sequence_index = 0;
    }

    /* Set next state */
    if (gAttract.sequence_length > 0) {
        attract_set_state(gAttract.sequence[gAttract.sequence_index].state);
    } else {
        attract_set_state(ATTRACT_STATE_LOGO);
    }
}

void attract_prev_state(void) {
    gAttract.sequence_index--;

    if (gAttract.sequence_index < 0) {
        gAttract.sequence_index = gAttract.sequence_length - 1;
    }

    if (gAttract.sequence_length > 0) {
        attract_set_state(gAttract.sequence[gAttract.sequence_index].state);
    }
}

s32 attract_is_active(void) {
    return (gAttract.flags & ATTRACT_FLAG_ACTIVE) != 0;
}

/* ========== Input Handling ========== */

void attract_handle_input(void) {
    /* Check if start button pressed (handled externally in game.c) */
    if (attract_check_start()) {
        attract_stop();
        /* Game state machine will transition to TRKSEL or CARSEL */
    }
}

s32 attract_check_start(void) {
    /* This would check controller input */
    /* For now, return 0 - actual input is handled in game loop */
    return 0;
}

void attract_skip(void) {
    if (gAttract.flags & ATTRACT_FLAG_SKIP_ALLOWED) {
        attract_next_state();
    }
}

/* ========== Logo Display ========== */

void attract_show_logo(void) {
    gAttract.logo.animation = LOGO_ANIM_FADE_IN;
    gAttract.logo.alpha = 0.0f;
    gAttract.logo.scale = 1.0f;
    gAttract.logo.timer = 0;
}

void attract_hide_logo(void) {
    gAttract.logo.animation = LOGO_ANIM_FADE_OUT;
}

void attract_update_logo(void) {
    f32 fade_speed = 1.0f / 30.0f;  /* 0.5 second fade */

    gAttract.logo.timer++;

    switch (gAttract.logo.animation) {
        case LOGO_ANIM_FADE_IN:
            gAttract.logo.alpha += fade_speed;
            if (gAttract.logo.alpha >= 1.0f) {
                gAttract.logo.alpha = 1.0f;
                gAttract.logo.animation = LOGO_ANIM_HOLD;
            }
            break;

        case LOGO_ANIM_HOLD:
            /* Check if time to fade out */
            if (gAttract.state_timer >= gAttract.state_duration - 30) {
                gAttract.logo.animation = LOGO_ANIM_FADE_OUT;
            }
            break;

        case LOGO_ANIM_FADE_OUT:
            gAttract.logo.alpha -= fade_speed;
            if (gAttract.logo.alpha <= 0.0f) {
                gAttract.logo.alpha = 0.0f;
                gAttract.logo.animation = LOGO_ANIM_NONE;
            }
            break;

        default:
            break;
    }
}

void attract_draw_logo(void) {
    /* Draw background */
    ui_draw_box_filled(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_BLACK, COLOR_BLACK);

    /* Draw logo text (would be a texture in real implementation) */
    if (gAttract.logo.alpha > 0.0f) {
        u32 alpha = (u32)(gAttract.logo.alpha * 255.0f);
        u32 color = 0xFFFFFF00 | alpha;

        ui_draw_text_scaled(SCREEN_WIDTH / 2, 60, "SAN FRANCISCO",
                           color, ALIGN_CENTER, 1.5f);
        ui_draw_text_scaled(SCREEN_WIDTH / 2, 100, "RUSH 2049",
                           color, ALIGN_CENTER, 2.5f);

        /* Draw subtitle */
        ui_draw_text(SCREEN_WIDTH / 2, 150, "NINTENDO 64", color, ALIGN_CENTER);
    }
}

/* ========== Credits Display ========== */

void attract_show_credits(void) {
    gAttract.credits_scroll = 0.0f;
    gAttract.credits_index = 0;
    gAttract.credits_speed = 1.0f;
}

void attract_update_credits(void) {
    /* Scroll credits */
    gAttract.credits_scroll += gAttract.credits_speed;
}

void attract_draw_credits(void) {
    s32 i;
    s32 y_offset;
    s32 start_y = SCREEN_HEIGHT;

    /* Draw background */
    ui_draw_box_filled(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_BLACK, COLOR_BLACK);

    /* Draw scrolling credits */
    y_offset = start_y - (s32)gAttract.credits_scroll;

    for (i = 0; i < gNumCredits; i++) {
        const CreditItem *item = &gGameCredits[i];
        s32 y = y_offset + i * 25;

        /* Skip if off screen */
        if (y < -30 || y > SCREEN_HEIGHT + 30) {
            continue;
        }

        if (item->type == 1) {
            /* Header */
            ui_draw_text_scaled(SCREEN_WIDTH / 2, (s16)y,
                               item->name, COLOR_YELLOW, ALIGN_CENTER, 1.2f);
            if (item->role != NULL) {
                ui_draw_text(SCREEN_WIDTH / 2, (s16)(y + 15),
                            item->role, COLOR_GRAY, ALIGN_CENTER);
            }
        } else if (item->type == 0 && item->name != NULL) {
            /* Name entry */
            ui_draw_text(SCREEN_WIDTH / 2, (s16)y,
                        item->name, COLOR_WHITE, ALIGN_CENTER);
        }
        /* Type 2 is spacer, draw nothing */
    }
}

/* ========== High Score Display ========== */

void attract_show_hiscore(void) {
    gAttract.hiscore_track = 0;
    gAttract.hiscore_timer = 0;
    hiscore_start_display(0);
}

void attract_update_hiscore(void) {
    gAttract.hiscore_timer++;

    /* Rotate through tracks */
    if (gAttract.hiscore_timer >= ATTRACT_HISCORE_TIME / 8) {
        gAttract.hiscore_timer = 0;
        gAttract.hiscore_track++;
        if (gAttract.hiscore_track >= 8) {
            gAttract.hiscore_track = 0;
        }
        hiscore_start_display(gAttract.hiscore_track);
    }

    hiscore_update();
}

void attract_draw_hiscore(void) {
    hiscore_draw();
}

/* ========== Demo Mode ========== */

void attract_start_demo(void) {
    /* Select next demo track */
    gAttract.demo.track_id = sDemoTracks[sDemoTrackIndex];
    sDemoTrackIndex++;
    if (sDemoTrackIndex >= (s32)(sizeof(sDemoTracks) / sizeof(sDemoTracks[0]))) {
        sDemoTrackIndex = 0;
    }

    /* Select random car for demo */
    gAttract.demo.car_type = (u8)(gAttract.total_timer % 8);

    gAttract.demo.mode = DEMO_MODE_AI;
    gAttract.demo.active = 1;
    gAttract.demo.frame = 0;
    gAttract.demo.max_frames = ATTRACT_DEMO_TIME;

    gAttract.flags |= ATTRACT_FLAG_DEMO_ACTIVE;

    /* TODO: Initialize demo race state */
}

void attract_stop_demo(void) {
    gAttract.demo.active = 0;
    gAttract.flags &= ~ATTRACT_FLAG_DEMO_ACTIVE;

    /* TODO: Clean up demo race state */
}

void attract_update_demo(void) {
    if (!gAttract.demo.active) {
        return;
    }

    gAttract.demo.frame++;

    /* Check if demo finished */
    if (gAttract.demo.frame >= gAttract.demo.max_frames) {
        attract_stop_demo();
    }

    /* TODO: Update demo race simulation */
}

void attract_draw_demo(void) {
    /* TODO: Draw demo race */
    /* For now, just show placeholder */
    ui_draw_text(SCREEN_WIDTH / 2, 30, "DEMO", COLOR_RED, ALIGN_CENTER);

    /* Show demo info */
    {
        char track_str[32];
        track_str[0] = 'T';
        track_str[1] = 'R';
        track_str[2] = 'A';
        track_str[3] = 'C';
        track_str[4] = 'K';
        track_str[5] = ' ';
        track_str[6] = (char)('1' + gAttract.demo.track_id);
        track_str[7] = '\0';
        ui_draw_text(20, 30, track_str, COLOR_WHITE, ALIGN_LEFT);
    }
}

s32 attract_is_demo_active(void) {
    return gAttract.demo.active;
}

/* ========== Join Prompt ========== */

void attract_show_join(void) {
    gAttract.join_blink_timer = 0;
    gAttract.join_blink_on = 1;
}

void attract_update_join(void) {
    /* Update blink timer */
    gAttract.join_blink_timer++;
    if (gAttract.join_blink_timer >= 20) {
        gAttract.join_blink_timer = 0;
        gAttract.join_blink_on = !gAttract.join_blink_on;
    }
}

void attract_draw_join(void) {
    /* Draw background */
    ui_draw_box_filled(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_BLACK, COLOR_BLACK);

    /* Draw "INSERT COIN" or "PRESS START" */
    if (gAttract.flags & ATTRACT_FLAG_FREEPLAY) {
        ui_draw_text_scaled(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 20,
                           "FREE PLAY", COLOR_GREEN, ALIGN_CENTER, 1.5f);
    } else {
        /* Show credits required message */
        ui_draw_text(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 40,
                    "READY TO RACE?", COLOR_WHITE, ALIGN_CENTER);
    }

    /* Blinking "PRESS START" */
    if (gAttract.join_blink_on) {
        ui_draw_text_scaled(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 20,
                           "PRESS START", COLOR_YELLOW, ALIGN_CENTER, 1.5f);
    }

    /* Player count display */
    ui_draw_text(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 40,
                "1-4 PLAYERS", COLOR_GRAY, ALIGN_CENTER);
}

/* ========== Transitions ========== */

void attract_fade_in(void) {
    gAttract.fade_in = 1;
    gAttract.fade_alpha = 1.0f;
}

void attract_fade_out(void) {
    gAttract.fade_in = 0;
    gAttract.fade_alpha = 0.0f;
}

void attract_update_fade(void) {
    f32 fade_speed = 1.0f / 15.0f;  /* ~0.25 second fade */

    if (gAttract.fade_in) {
        gAttract.fade_alpha -= fade_speed;
        if (gAttract.fade_alpha <= 0.0f) {
            gAttract.fade_alpha = 0.0f;
        }
    } else {
        gAttract.fade_alpha += fade_speed;
        if (gAttract.fade_alpha >= 1.0f) {
            gAttract.fade_alpha = 1.0f;
        }
    }
}

/* ========== Utility ========== */

u32 attract_get_demo_track(void) {
    return gAttract.demo.track_id;
}

u32 attract_get_demo_car(void) {
    return gAttract.demo.car_type;
}
