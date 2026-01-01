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

/* ========================================================================
 * ARCADE-COMPATIBLE IMPLEMENTATIONS
 * Based on rushtherock/game/attract.c
 * ======================================================================== */

/* Arcade globals */
const f32 gVersion = 1.00f;

AttractMode attractFunc = ATR_MOVIE4;

u8   gStartDuringLoad = 0;
s32  gFreeGame = 0;
s32  gPlayingFree = 0;
s32  gContinuedGame = 0;
s32  explosion_state = 0;
s32  explosion_time = 0;
u8   skip = 4;
s32  gTourneyJoin = 0;

s16  gAttractTimes[NUM_ATTRACTS];

/* Countdown timer for attract mode */
static u32 countdown_timer = 0;
static u32 countdown_start = 0;

/* Demo game state */
static s32 demo_game = 0;
static s32 gDemoInited = 0;
static s16 gLastFunc = -1;

/* Tourney mode state */
static s32 tourney_setup = 0;
static Tourney gTourney;

/**
 * InitAttract - Initialize attract mode globals
 * Based on arcade: attract.c:InitAttract()
 *
 * Called once at game startup to initialize all attract mode state.
 */
void InitAttract(void) {
    attractFunc = ATR_MOVIE4;
    SetCountdownTimer(8 * ONE_SEC);
    gLastFunc = -1;
    demo_game = 0;
    gDemoInited = 0;
    gStartDuringLoad = 0;
    gFreeGame = 0;
    skip = 4;

    /* Set default times for each attract mode */
    gAttractTimes[ATR_HSENTRY] = 20;
    gAttractTimes[ATR_HISCORE1] = 20;
    gAttractTimes[ATR_HISCORE2] = 20;
    gAttractTimes[ATR_HISCORE3] = 20;
    gAttractTimes[ATR_HISCORE4] = 20;
    gAttractTimes[ATR_HISCORE5] = 20;
    gAttractTimes[ATR_HISCORE6] = 20;
    gAttractTimes[ATR_HISCORE7] = 20;
    gAttractTimes[ATR_ATARILOGO] = 10;
    gAttractTimes[ATR_ATARILOGO2] = 10;
    gAttractTimes[ATR_ATARILOGO3] = 10;
    gAttractTimes[ATR_MOVIE2] = 6;      /* Car 1 rotating */
    gAttractTimes[ATR_MOVIE3] = 6;      /* Car 2 rotating */
    gAttractTimes[ATR_MOVIE4] = 10;     /* Intro movie */
    gAttractTimes[ATR_MOVIE5] = 20;     /* Rush logo */
    gAttractTimes[ATR_MOVIE52] = 20;
    gAttractTimes[ATR_MOVIE53] = 20;
    gAttractTimes[ATR_OCREDITS] = 8;
    gAttractTimes[ATR_CREDITS] = 8;
    gAttractTimes[ATR_MIRROR] = 8;
    gAttractTimes[ATR_CARS] = 8;
    gAttractTimes[ATR_ADVISORY] = 8;
    gAttractTimes[ATR_ADVERT] = 8;
    gAttractTimes[ATR_3DFXLOGO] = 3;
    gAttractTimes[ATR_DEMO] = 35;
    gAttractTimes[ATR_DEMO2] = 35;
    gAttractTimes[ATR_TEAM] = 10;
    gAttractTimes[ATR_OTEAM] = 5;
    gAttractTimes[ATR_JOIN] = 15;
    gAttractTimes[ATR_TOURNEY] = 999;
    gAttractTimes[ATR_TRANSIT] = 999;
    gAttractTimes[ATR_GAMESTAT] = 999;

    /* Also initialize N64-specific attract state */
    attract_init();
}

/**
 * attract - Main attract mode handler
 * Based on arcade: attract.c:attract()
 *
 * Called every frame during attract mode.
 * Handles transitions between attract screens and checks for game start.
 */
void attract(void) {
    s32 start_game;

    /* Handle steering wheel forces */
    AttractForce();

    /* Check for state timeout */
    if (TimeOut() || attractFunc == ATR_HSENTRY) {
        ResetJoinIn();
        gTourneyJoin = 0;

        /* Advance to next attract mode */
        do {
            attractFunc++;
            if (attractFunc >= ATR_TOURNEY) {
                skip++;
                attractFunc = ATR_HSENTRY + 1;
            }
        } while ((skip % 5) && (attractFunc == ATR_CREDITS ||
                 attractFunc == ATR_3DFXLOGO || attractFunc == ATR_OTEAM ||
                 attractFunc == ATR_OCREDITS || attractFunc == ATR_TEAM));

        SetCountdownTimer(gAttractTimes[attractFunc] * ONE_SEC);
        gTourneyJoin = 0;
    }

    /* If changing modes, update display */
    if (gLastFunc != attractFunc) {
        ShowAttract(gLastFunc, 0);
        ShowAttract(attractFunc, 1);
    }

    /* Check for game start */
    start_game = chk_start();
    if (gStartDuringLoad || start_game) {
        explosion_state = 0;

        /* Remove current screen and show transition */
        ShowAttract(gLastFunc, 0);
        ShowAttract(ATR_TRANSIT, 1);
        attractFunc = ATR_TRANSIT;

        demo_game = 0;
        gDemoInited = 0;
        gLastFunc = -1;
    } else {
        /* Update current attract mode */
        switch (attractFunc) {
            case ATR_DEMO:
            case ATR_DEMO2:
            case ATR_HISCORE1:
            case ATR_HISCORE2:
            case ATR_HISCORE3:
            case ATR_HISCORE4:
            case ATR_HISCORE5:
            case ATR_HISCORE6:
            case ATR_HISCORE7:
            case ATR_ADVERT:
            case ATR_ADVISORY:
                play_demogame(1, 0, -1, 0);
                break;
            default:
                break;
        }
    }
}

/**
 * AttractForce - Handle steering wheel forces in attract mode
 * Based on arcade: attract.c:AttractForce()
 *
 * Applies gentle centering force to steering wheel during attract.
 */
void AttractForce(void) {
    /* N64 doesn't have force feedback steering, so this is a stub */
    /* On arcade: force_wheel_to_center(0, 5, 11, 20); */
}

/**
 * ShowAttract - Show or hide an attract screen
 * Based on arcade: attract.c:ShowAttract()
 *
 * @param func AttractMode enum value for the screen
 * @param show 1 to show, 0 to hide
 */
void ShowAttract(s16 func, s32 show) {
    gDemoInited = 0;

    switch (func) {
        case ATR_HSENTRY:
            ShowScoreEntry(show);
            break;
        case ATR_HISCORE1:
            ShowHiScore(show, 0);
            break;
        case ATR_HISCORE2:
            ShowHiScore(show, 1);
            break;
        case ATR_HISCORE3:
            ShowHiScore(show, 2);
            break;
        case ATR_HISCORE4:
            ShowHiScore(show, 3);
            break;
        case ATR_HISCORE5:
            ShowHiScore(show, 4);
            break;
        case ATR_HISCORE6:
            ShowHiScore(show, 5);
            break;
        case ATR_HISCORE7:
            ShowHiScore(show, 6);
            break;
        case ATR_CREDITS:
        case ATR_OCREDITS:
            ShowCredits(show);
            break;
        case ATR_TEAM:
        case ATR_OTEAM:
            ShowTeam(show);
            break;
        case ATR_3DFXLOGO:
        case ATR_ATARILOGO:
        case ATR_ATARILOGO2:
        case ATR_ATARILOGO3:
            ShowMovie(show, 1);
            break;
        case ATR_MOVIE2:
        case ATR_MOVIE3:
        case ATR_MOVIE4:
        case ATR_MOVIE5:
        case ATR_MOVIE52:
        case ATR_MOVIE53:
            ShowMovie(show, func - ATR_MOVIE2 + 2);
            break;
        case ATR_ADVISORY:
            ShowAdvisory(show);
            break;
        case ATR_ADVERT:
            ShowAdvert(show);
            break;
        case ATR_DEMO:
        case ATR_DEMO2:
            play_demogame(show, 1, -1, 1);
            break;
        case ATR_TRANSIT:
            ShowTransit(show, 1);
            break;
        case ATR_TOURNEY:
            ShowTourneySetup(show);
            break;
        case ATR_JOIN:
            ShowJoin(show);
            break;
        case ATR_GAMESTAT:
            ShowGameStats(show);
            break;
        default:
            break;
    }

    if (show) {
        gLastFunc = func;
    } else {
        gLastFunc = -1;
    }
}

/**
 * ShowTransit - Show or hide the transition screen
 * Based on arcade: attract.c:ShowTransit()
 */
void ShowTransit(s32 show, s16 num) {
    if (show) {
        /* Set up transition fade */
        attract_fade_in();
    } else {
        attract_fade_out();
    }
}

/**
 * ShowJoin - Show or hide the join-in screen
 * Based on arcade: attract.c:ShowJoin()
 */
void ShowJoin(s32 show) {
    if (show) {
        attract_set_state(ATTRACT_STATE_JOIN);
    }
}

/**
 * ShowCredits - Show or hide the credits screen
 * Based on arcade: attract.c:ShowCredits()
 */
void ShowCredits(s32 show) {
    if (show) {
        attract_set_state(ATTRACT_STATE_CREDITS);
    }
}

/**
 * ShowHiScore - Show or hide high score for a track
 * Based on arcade: attract.c:ShowHiScore()
 */
void ShowHiScore(s32 show, s32 track) {
    if (show) {
        gAttract.hiscore_track = track;
        attract_set_state(ATTRACT_STATE_HISCORE);
    }
}

/**
 * ShowScoreEntry - Show or hide the score entry screen
 * Based on arcade: attract.c:ShowScoreEntry()
 */
void ShowScoreEntry(s32 show) {
    /* Uses hiscore.c functions for actual implementation */
    if (show) {
        attract_set_state(ATTRACT_STATE_HISCORE);
    }
}

/**
 * ShowTeam - Show or hide team credits
 * Based on arcade: attract.c:ShowTeam()
 */
void ShowTeam(s32 show) {
    if (show) {
        attract_set_state(ATTRACT_STATE_CREDITS);
    }
}

/**
 * ShowAdvisory - Show or hide advisory screen
 * Based on arcade: attract.c:ShowAdvisory()
 */
void ShowAdvisory(s32 show) {
    if (show) {
        /* Show ESRB rating or similar advisory */
        attract_set_state(ATTRACT_STATE_LOGO);
    }
}

/**
 * ShowAdvert - Show or hide advertisement screen
 * Based on arcade: attract.c:ShowAdvert()
 */
void ShowAdvert(s32 show) {
    if (show) {
        /* N64 doesn't have advertisements, show logo instead */
        attract_set_state(ATTRACT_STATE_LOGO);
    }
}

/**
 * ShowMovie - Show or hide a movie/video screen
 * Based on arcade: attract.c:ShowMovie()
 */
void ShowMovie(s32 show, s32 num) {
    if (show) {
        /* N64 uses static logos instead of FMV */
        attract_set_state(ATTRACT_STATE_LOGO);
    }
}

/**
 * play_demogame - Run the demo game
 * Based on arcade: attract.c (calls to play_demogame)
 */
void play_demogame(s32 run, s32 init, s32 track, s32 attract_mode_flag) {
    if (init) {
        gDemoInited = 1;
        attract_start_demo();
    }

    if (run && gDemoInited) {
        attract_update_demo();
    }
}

/**
 * TimeOut - Check if countdown timer has expired
 * Based on arcade timeout logic
 */
s32 TimeOut(void) {
    if (countdown_timer == 0) {
        return 0;
    }
    return (gAttract.total_timer >= countdown_start + countdown_timer);
}

/**
 * SetCountdownTimer - Set the countdown timer
 * Based on arcade: SetCountdownTimer()
 */
void SetCountdownTimer(u32 time) {
    countdown_start = gAttract.total_timer;
    countdown_timer = time;
}

/**
 * chk_start - Check if start button was pressed
 * Based on arcade: chk_start()
 */
s32 chk_start(void) {
    return attract_check_start();
}

/**
 * EnoughCredit - Check if player has enough credits
 * Based on arcade: EnoughCredit()
 */
s32 EnoughCredit(void) {
    /* N64 doesn't use credits, always return true */
    return 1;
}

/**
 * ResetJoinIn - Reset join-in state
 * Based on arcade: ResetJoinIn()
 */
void ResetJoinIn(void) {
    /* Reset any join-in flags */
}

/**
 * TourneyOn - Check if tournament mode is active
 * Based on arcade: TourneyOn()
 */
s32 TourneyOn(void) {
    /* N64 doesn't have tournament mode */
    return 0;
}

/**
 * TourneyNode - Check if a node is in the tournament
 * Based on arcade: TourneyNode()
 */
s32 TourneyNode(s32 node) {
    return 0;
}

/**
 * ChkGameStats - Check for game stats mode
 * Based on arcade: ChkGameStats()
 */
s32 ChkGameStats(void) {
    return 0;
}

/**
 * ShowGameStats - Show or hide game statistics
 * Based on arcade: ShowGameStats()
 */
void ShowGameStats(s32 show) {
    /* N64 doesn't have arcade game stats */
}

/**
 * HandleTourneySetup - Handle tournament setup screen
 * Based on arcade: HandleTourneySetup()
 */
void HandleTourneySetup(void) {
    /* N64 doesn't have tournament mode */
}

/**
 * ShowTourneySetup - Show or hide tournament setup
 * Based on arcade: ShowTourneySetup()
 */
void ShowTourneySetup(s32 show) {
    /* N64 doesn't have tournament mode */
}

/**
 * AnimateTimer - Animate countdown timer display
 * Based on arcade: AnimateTimer()
 */
s32 AnimateTimer(void *blt) {
    /* Update timer display */
    return 1;
}

/**
 * AnimateTransit - Animate transition screen
 * Based on arcade: AnimateTransit()
 */
s32 AnimateTransit(void *blt) {
    attract_update_fade();
    return 1;
}

/**
 * AnimateCreds - Animate credits display
 * Based on arcade: AnimateCreds()
 */
s32 AnimateCreds(void *blt) {
    /* Update credits counter display */
    return 1;
}
