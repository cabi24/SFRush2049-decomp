/**
 * select.c - Track and car selection for Rush 2049 N64
 *
 * Handles track selection, car selection, and transmission choice.
 * Based on arcade sselect.c patterns from rushtherock/game/sselect.c
 *
 * Key arcade functions matched:
 *   InitSelect, InitTrkSel, TrackSel, ShowTrackSelect, TrackSelForce
 *   init_car_select, CarSel, ShowCarSelect, CarSelForce, ClutchSel
 *   create_cars, SelectCam, track_negotiation, race_negotiation
 *   waiting_for_others, SetCountdownTimer, GetCountdownTime, TimeOut
 */

#include "game/select.h"
#include "game/game.h"
#include "game/input.h"

/* ========================== ARCADE-COMPATIBLE GLOBALS ========================== */
/* These match the arcade sselect.c globals for reference/matching */

u32 start_time;                     /* IRQTIME when game started */
u32 play_time;                      /* Milliseconds of time to play */
u8 gFeedbackLevel;                  /* Force feedback level (0-2) */
s16 gThisCar;                       /* Currently selected car index */
s32 gCarLocked;                     /* Car selection locked flag (arcade: BOOL) */
s32 gClutchLocked;                  /* Transmission locked flag (arcade: BOOL) */
s32 gTrackLocked;                   /* Track selection locked flag (arcade: BOOL) */
s16 num_pads;                       /* Number of selectable car pads */
s8 negotiate_track;                 /* Track negotiation phase */
s8 negotiate_race;                  /* Race negotiation phase */
s16 trackno;                        /* Current track number */
s32 gMirror;                        /* Mirror mode flag */
s32 solo_flag;                      /* Solo play (no link) flag */
s32 autotrans;                      /* Automatic transmission flag */
u8 drones_flag;                     /* Drones enabled flag */
u8 catchup_flag;                    /* Catchup enabled flag */

/* Static arcade globals (from sselect.c) */
static s16 gLitPad;                 /* Currently highlighted pad */
static s16 gLastCar;                /* Last car index */
static s16 gPickPad;                /* Picked pad index */
static s16 gCarMove;                /* Car movement animation state */
static s16 gCarFirstSel;            /* First car in current bank */
static u32 gPickTime;               /* Time when car was picked */
static u32 gBeginTime;              /* Time when selection began */
static f32 gPadPitch;               /* Pad pitch angle */
static s32 gSoundOn;                /* Turntable sound playing */
static s32 gAltCars;                /* Alternate cars visible */
static s32 gCameraTransition;       /* Camera transitioning */
static s16 gCurCam;                 /* Current camera index */
static s16 gNextCam;                /* Next camera index */
static s16 gCamFrames;              /* Frames until next camera */
static s16 oldTrack;                /* Previous track selection */
static s16 old_num_on;              /* Previous number of linked players */
static s32 tlock_sound_delay;       /* Track lock sound delay timer */
static s32 twait_delay;             /* Waiting sound delay timer */
static s32 all_tracks_locked_time;  /* Time all tracks were locked */
static u32 countdown_timer;         /* Selection countdown timer */

/* Global state */
SelectState gSelect;

/* Track data */
const TrackInfo gTrackData[NUM_TRACKS] = {
    { "Marina",      "San Francisco", 0, 2, 3, TRACK_FLAG_UNLOCKED, 0, 8, 0 },
    { "Haight",      "San Francisco", 1, 3, 3, TRACK_FLAG_UNLOCKED, 0, 10, 0 },
    { "Metro",       "San Francisco", 2, 3, 3, TRACK_FLAG_UNLOCKED, 0, 9, 0 },
    { "Mission",     "San Francisco", 3, 4, 3, TRACK_FLAG_UNLOCKED, 0, 11, 0 },
    { "Presidio",    "San Francisco", 4, 4, 3, 0, 0, 10, 0 },  /* Locked initially */
    { "Downtown",    "San Francisco", 5, 5, 3, 0, 0, 12, 0 },
    { "Alcatraz",    "San Francisco", 6, 5, 4, TRACK_FLAG_HIDDEN, 0, 14, 0 },
    { "Stunt Arena", "San Francisco", 7, 1, 0, TRACK_FLAG_BONUS, 0, 0, 0 },
};

/* Car data */
const CarInfo gCarData[NUM_CARS] = {
    { "Rocket",     0, CAR_FLAG_UNLOCKED, 8, 6, 7, 5, 0, 0 },
    { "Venom",      1, CAR_FLAG_UNLOCKED, 7, 7, 6, 6, 0, 0 },
    { "Crusher",    2, CAR_FLAG_UNLOCKED, 6, 5, 5, 9, 0, 0 },
    { "Prototype",  3, CAR_FLAG_UNLOCKED, 9, 5, 8, 4, 0, 0 },
    { "Velocity",   4, 0, 8, 8, 7, 5, 0, 0 },  /* Locked initially */
    { "Hot Rod",    5, 0, 7, 6, 9, 6, 0, 0 },
    { "Concept",    6, CAR_FLAG_HIDDEN, 10, 7, 9, 4, 0, 0 },
    { "Stunt Car",  7, CAR_FLAG_SPECIAL, 6, 9, 6, 5, 0, 0 },
};

/*
 * select_init - Initialize selection system
 */
void select_init(void)
{
    select_reset();
}

/*
 * select_reset - Reset selection state
 */
void select_reset(void)
{
    gSelect.phase = 0;
    gSelect.complete = 0;
    gSelect.cancelled = 0;
    gSelect.player = 0;

    /* Reset track selection */
    gSelect.track.state = SELECT_STATE_IDLE;
    gSelect.track.sub_state = TRKSEL_BROWSING;
    gSelect.track.current_track = 0;
    gSelect.track.selected_track = 0;
    gSelect.track.mirror_mode = 0;
    gSelect.track.timer = 0;
    gSelect.track.lock_delay = 0;
    gSelect.track.cursor.position = 0;
    gSelect.track.cursor.target = 0;
    gSelect.track.cursor.scroll_offset = 0.0f;
    gSelect.track.cursor.scroll_speed = 0.0f;
    gSelect.track.cursor.moving = 0;
    gSelect.track.preview_rotation = 0.0f;
    gSelect.track.show_preview = 1;

    /* Reset car selection */
    gSelect.car.state = SELECT_STATE_IDLE;
    gSelect.car.sub_state = CARSEL_BROWSING;
    gSelect.car.current_car = 0;
    gSelect.car.selected_car = 0;
    gSelect.car.current_color = 0;
    gSelect.car.selected_color = 0;
    gSelect.car.show_stats = 1;
    gSelect.car.timer = 0;
    gSelect.car.lock_delay = 0;
    gSelect.car.cursor.position = 0;
    gSelect.car.cursor.target = 0;
    gSelect.car.cursor.scroll_offset = 0.0f;
    gSelect.car.cursor.scroll_speed = 0.0f;
    gSelect.car.cursor.moving = 0;
    gSelect.car.car_rotation = 0.0f;
    gSelect.car.rotation_speed = 0.5f;
    gSelect.car.auto_rotate = 1;
    gSelect.car.transmission = TRANS_AUTO;
    gSelect.car.trans_locked = 0;

    /* Reset final selections */
    gSelect.final_track = 0;
    gSelect.final_car = 0;
    gSelect.final_color = 0;
    gSelect.final_trans = TRANS_AUTO;
    gSelect.final_mirror = 0;
    gSelect.final_laps = 3;
}

/*
 * select_start - Begin selection process
 */
void select_start(void)
{
    select_reset();
    gSelect.phase = 0;
    track_select_init();
}

/*
 * select_update - Update selection state
 */
void select_update(void)
{
    switch (gSelect.phase) {
        case 0:
            track_select_update();
            if (track_select_is_locked()) {
                select_next_phase();
            }
            break;

        case 1:
            car_select_update();
            if (car_select_is_locked()) {
                select_next_phase();
            }
            break;

        case 2:
            trans_select_update();
            /* Transition handled by game state machine */
            break;
    }
}

/*
 * select_draw - Draw selection screen
 */
void select_draw(void)
{
    switch (gSelect.phase) {
        case 0:
            track_select_draw();
            break;
        case 1:
            car_select_draw();
            break;
        case 2:
            trans_select_draw();
            break;
    }
}

/*
 * select_next_phase - Advance to next selection phase
 */
void select_next_phase(void)
{
    switch (gSelect.phase) {
        case 0:
            /* Track -> Car */
            gSelect.final_track = gSelect.track.selected_track;
            gSelect.final_mirror = gSelect.track.mirror_mode;
            gSelect.final_laps = gTrackData[gSelect.final_track].laps;
            gSelect.phase = 1;
            car_select_init();
            break;

        case 1:
            /* Car -> Transmission */
            gSelect.final_car = gSelect.car.selected_car;
            gSelect.final_color = gSelect.car.selected_color;
            gSelect.phase = 2;
            trans_select_init();
            break;

        case 2:
            /* Complete */
            gSelect.final_trans = gSelect.car.transmission;
            gSelect.complete = 1;
            break;
    }
}

/*
 * select_prev_phase - Go back to previous phase
 */
void select_prev_phase(void)
{
    if (gSelect.phase > 0) {
        gSelect.phase--;

        switch (gSelect.phase) {
            case 0:
                track_select_init();
                gSelect.track.sub_state = TRKSEL_BROWSING;
                break;
            case 1:
                car_select_init();
                gSelect.car.sub_state = CARSEL_BROWSING;
                break;
        }
    }
}

/*
 * select_is_complete - Check if selection is done
 */
s32 select_is_complete(void)
{
    return gSelect.complete;
}

/*
 * Track Selection
 */

void track_select_init(void)
{
    gSelect.track.state = SELECT_STATE_ACTIVE;
    gSelect.track.sub_state = TRKSEL_BROWSING;
    gSelect.track.timer = TRK_SELECT_TIME;
    gSelect.track.lock_delay = TRK_DELAY_TIME;
    gSelect.track.preview_rotation = 0.0f;

    /* Find first available track */
    while (gSelect.track.current_track < NUM_TRACKS &&
           !track_is_available(gSelect.track.current_track)) {
        gSelect.track.current_track++;
    }

    gSelect.track.cursor.position = gSelect.track.current_track;
    gSelect.track.cursor.target = gSelect.track.current_track;
}

void track_select_update(void)
{
    /* Update timer */
    if (gSelect.track.timer > 0) {
        gSelect.track.timer--;
    }

    /* Update lock delay */
    if (gSelect.track.lock_delay > 0) {
        gSelect.track.lock_delay--;
    }

    /* Handle timeout */
    if (gSelect.track.timer == 0 &&
        gSelect.track.sub_state != TRKSEL_LOCKED) {
        /* Auto-select current track on timeout */
        track_select_confirm();
        return;
    }

    /* Handle input */
    track_select_input();

    /* Update preview rotation */
    gSelect.track.preview_rotation += 0.5f;
    if (gSelect.track.preview_rotation >= 360.0f) {
        gSelect.track.preview_rotation -= 360.0f;
    }

    /* Smooth cursor animation */
    if (gSelect.track.cursor.moving) {
        f32 diff = (f32)gSelect.track.cursor.target -
                   gSelect.track.cursor.scroll_offset;
        if (diff > 0.1f || diff < -0.1f) {
            gSelect.track.cursor.scroll_offset += diff * 0.2f;
        } else {
            gSelect.track.cursor.scroll_offset = (f32)gSelect.track.cursor.target;
            gSelect.track.cursor.moving = 0;
        }
    }
}

void track_select_input(void)
{
    if (gSelect.track.sub_state == TRKSEL_LOCKED) {
        return;
    }

    /* Left/Right to change track */
    /* TODO: Read actual controller input */
    /* For now, placeholder input handling */

    /* A button to confirm */
    /* B button for mirror mode toggle */
}

void track_select_confirm(void)
{
    if (gSelect.track.lock_delay > 0) {
        return;
    }

    if (!track_is_available(gSelect.track.current_track)) {
        return;
    }

    gSelect.track.selected_track = gSelect.track.current_track;
    gSelect.track.sub_state = TRKSEL_LOCKED;
    gSelect.track.state = SELECT_STATE_LOCKED;

    /* Play confirmation sound */
    /* sound_play(SFX_CONFIRM); */
}

void track_select_cancel(void)
{
    gSelect.cancelled = 1;
}

s32 track_select_is_locked(void)
{
    return (gSelect.track.sub_state == TRKSEL_LOCKED);
}

void track_select_draw(void)
{
    /* TODO: Draw track selection UI */
    /* - Track preview (3D or image) */
    /* - Track name and info */
    /* - Timer countdown */
    /* - Mirror mode indicator */
    select_draw_countdown();
}

/*
 * Car Selection
 */

void car_select_init(void)
{
    gSelect.car.state = SELECT_STATE_ACTIVE;
    gSelect.car.sub_state = CARSEL_BROWSING;
    gSelect.car.timer = CAR_SELECT_TIME;
    gSelect.car.lock_delay = CAR_DELAY_TIME;
    gSelect.car.car_rotation = 0.0f;

    /* Find first available car */
    while (gSelect.car.current_car < NUM_CARS &&
           !car_is_available(gSelect.car.current_car)) {
        gSelect.car.current_car++;
    }

    gSelect.car.cursor.position = gSelect.car.current_car;
    gSelect.car.cursor.target = gSelect.car.current_car;
}

void car_select_update(void)
{
    /* Update timer */
    if (gSelect.car.timer > 0) {
        gSelect.car.timer--;
    }

    /* Update lock delay */
    if (gSelect.car.lock_delay > 0) {
        gSelect.car.lock_delay--;
    }

    /* Handle timeout */
    if (gSelect.car.timer == 0 &&
        gSelect.car.sub_state != CARSEL_LOCKED) {
        car_select_confirm();
        return;
    }

    /* Handle input */
    car_select_input();

    /* Update car rotation */
    if (gSelect.car.auto_rotate) {
        gSelect.car.car_rotation += gSelect.car.rotation_speed;
        if (gSelect.car.car_rotation >= 360.0f) {
            gSelect.car.car_rotation -= 360.0f;
        }
    }

    /* Smooth cursor animation */
    if (gSelect.car.cursor.moving) {
        f32 diff = (f32)gSelect.car.cursor.target -
                   gSelect.car.cursor.scroll_offset;
        if (diff > 0.1f || diff < -0.1f) {
            gSelect.car.cursor.scroll_offset += diff * 0.2f;
        } else {
            gSelect.car.cursor.scroll_offset = (f32)gSelect.car.cursor.target;
            gSelect.car.cursor.moving = 0;
        }
    }
}

void car_select_input(void)
{
    if (gSelect.car.sub_state == CARSEL_LOCKED) {
        return;
    }

    /* Left/Right to change car */
    /* Up/Down to change color */
    /* A button to confirm */
    /* B button to go back */
    /* TODO: Read actual controller input */
}

void car_select_confirm(void)
{
    if (gSelect.car.lock_delay > 0) {
        return;
    }

    if (!car_is_available(gSelect.car.current_car)) {
        return;
    }

    gSelect.car.selected_car = gSelect.car.current_car;
    gSelect.car.selected_color = gSelect.car.current_color;
    gSelect.car.sub_state = CARSEL_LOCKED;
    gSelect.car.state = SELECT_STATE_LOCKED;

    /* Play confirmation sound */
    /* sound_play(SFX_CONFIRM); */
}

void car_select_cancel(void)
{
    select_prev_phase();
}

s32 car_select_is_locked(void)
{
    return (gSelect.car.sub_state == CARSEL_LOCKED);
}

void car_select_draw(void)
{
    /* TODO: Draw car selection UI */
    /* - 3D car model rotating */
    /* - Car name and stats */
    /* - Color selector */
    /* - Timer countdown */
    select_draw_countdown();
}

/*
 * Transmission Selection
 */

void trans_select_init(void)
{
    gSelect.car.trans_locked = 0;
}

void trans_select_update(void)
{
    /* Handle input */
    /* A to confirm auto, B to confirm manual */
    /* Or left/right to toggle, A to confirm */
    /* TODO: Read actual controller input */
}

void trans_select_toggle(void)
{
    if (!gSelect.car.trans_locked) {
        gSelect.car.transmission = (gSelect.car.transmission == TRANS_AUTO) ?
                                    TRANS_MANUAL : TRANS_AUTO;
    }
}

void trans_select_draw(void)
{
    /* TODO: Draw transmission selector */
    /* - AUTO / MANUAL options */
    /* - Current selection highlighted */
}

/*
 * Query functions
 */

u8 select_get_track(void)
{
    return gSelect.final_track;
}

u8 select_get_car(void)
{
    return gSelect.final_car;
}

u8 select_get_color(void)
{
    return gSelect.final_color;
}

u8 select_get_transmission(void)
{
    return gSelect.final_trans;
}

u8 select_get_mirror(void)
{
    return gSelect.final_mirror;
}

u8 select_get_laps(void)
{
    return gSelect.final_laps;
}

/*
 * Track/car availability
 */

s32 track_is_available(u8 track_id)
{
    if (track_id >= NUM_TRACKS) {
        return 0;
    }

    /* Check if unlocked */
    if (!(gTrackData[track_id].flags & TRACK_FLAG_UNLOCKED)) {
        return 0;
    }

    /* Hidden tracks require special unlock */
    if (gTrackData[track_id].flags & TRACK_FLAG_HIDDEN) {
        /* TODO: Check if hidden tracks unlocked */
        return 0;
    }

    return 1;
}

s32 car_is_available(u8 car_id)
{
    if (car_id >= NUM_CARS) {
        return 0;
    }

    /* Check if unlocked */
    if (!(gCarData[car_id].flags & CAR_FLAG_UNLOCKED)) {
        return 0;
    }

    /* Hidden cars require special unlock */
    if (gCarData[car_id].flags & CAR_FLAG_HIDDEN) {
        /* TODO: Check if hidden cars unlocked */
        return 0;
    }

    return 1;
}

void track_unlock(u8 track_id)
{
    /* TODO: Unlock track in save data */
    /* For now, this would modify gTrackData but it's const */
}

void car_unlock(u8 car_id)
{
    /* TODO: Unlock car in save data */
}

/*
 * Preview rendering (stubs)
 */

void track_preview_render(u8 track_id, f32 rotation)
{
    /* TODO: Render track preview */
}

void car_preview_render(u8 car_id, u8 color, f32 rotation)
{
    /* TODO: Render car preview model */
}

/*
 * Timer display
 */

void select_draw_timer(s32 x, s32 y, u32 frames)
{
    u32 seconds = frames / 60;
    /* TODO: Draw timer text at x,y */
}

void select_draw_countdown(void)
{
    u32 timer;

    switch (gSelect.phase) {
        case 0:
            timer = gSelect.track.timer;
            break;
        case 1:
            timer = gSelect.car.timer;
            break;
        default:
            return;
    }

    /* Draw timer in top corner */
    select_draw_timer(280, 20, timer);

    /* Blink when running low */
    if (timer < HURRY_BLINK_TIME * 5) {
        if ((timer / HURRY_BLINK_TIME) & 1) {
            /* TODO: Flash "HURRY UP" text */
        }
    }
}

/* ========================== ARCADE-COMPATIBLE FUNCTIONS ========================== */
/* These are stub implementations matching arcade sselect.c signatures */
/* Marked with #ifdef NON_MATCHING for decompilation workflow */

#ifdef NON_MATCHING

/*
 * InitSelect - Called each time select mode is initially entered
 * Arcade: sselect.c line ~586
 * Loads models and prepares for the select process
 */
void InitSelect(void)
{
    s32 i;

    /* Give audio cue when game starts (arcade: SOUND(S_RUSHWHISPER)) */
    /* sound_play(SFX_RUSH_WHISPER); */

    num_pads = 0;

    /* Load the data from disk (arcade: Load(3)) */
    gTrackLocked = 0;
    /* gstate = TRKSEL; - handled by game state machine */

    /* Initialize visuals (arcade: InitVisuals(false)) */

    gPickTime = 0;
    gPadPitch = 0.0f;
    gCameraTransition = 0;
    gCarMove = CM_NOT;
    gLitPad = -1;
    gSoundOn = 0;
    oldTrack = -1;
    all_tracks_locked_time = 0;

    /* Set up camera */
    gCurCam = 0;
    gNextCam = 0;
    gCamFrames = 0;
    SelectCam();

    /* Prepare to select */
    SetCountdownTimer(TRK_SELECT_TIME);

    /* Create car select room objects (arcade uses MBOX_NewObject) */
    num_pads = NPADS;
    create_cars();
}

/*
 * InitTrkSel - Initialize global variables for track selection
 * Arcade: sselect.c line ~647
 * Called once at init time
 */
void InitTrkSel(void)
{
    gTrackLocked = 0;
    gCarLocked = 0;
    gClutchLocked = 0;
    drones_flag = 1;            /* Drones enabled by default */
    gFeedbackLevel = DEFAULT_FEEDBACK_LEVEL;
    catchup_flag = 1;           /* Catchup enabled by default */
    solo_flag = 0;
    gMirror = 0;
}

/*
 * TrackSel - Track selection state handler
 * Arcade: sselect.c line ~715
 * Called every frame during TRKSEL state
 */
void TrackSel(void)
{
    /* Handle force feedback (arcade steering wheel) */
    TrackSelForce();
    SelectCam();

    /* Check for initialization */
    if (oldTrack == -1) {
        /* First frame initialization */
        gCarLocked = 0;
        gClutchLocked = 0;
        autotrans = 1;              /* Default to automatic */
        gThisCar = -1;
        gCarFirstSel = 0;
        gFeedbackLevel = DEFAULT_FEEDBACK_LEVEL;
        old_num_on = 0;
        tlock_sound_delay = 0;

        SetCountdownTimer(TRK_SELECT_TIME);

        /* Get initial track from controller input */
        trackno = WheelSection(NTRACKS, 1);
        oldTrack = trackno;

        /* Set initial car selection */
        gPickPad = RawWheelSection(NPADS, 1);
        gThisCar = gPickPad + gCarFirstSel;

        gBeginTime = 0;  /* Would be IRQTIME in arcade */

        num_pads = NPADS;
        create_cars();

        negotiate_track = 0;
        negotiate_race = 0;
    }

    /* If track already locked, handle waiting/transition */
    if (gTrackLocked) {
        ShowTrackSelect(1);

        if (solo_flag) {
            /* Immediately go to car select in solo mode */
            init_car_select();
            return;
        }

        /* Handle negotiation phases */
        if (negotiate_track > 1) {
            track_negotiation();
            return;
        }
        negotiate_track = 1;

        /* Check timeout */
        if (TimeOut()) {
            negotiate_track = 2;
        }
        return;
    }

    /* Check for lock trigger (gas pressed or timeout) */
    if (GasPressed(1) || TimeOut()) {
        gTrackLocked = 1;
        /* Play lock sound */
        return;
    }

    /* Still selecting - show UI and handle input */
    ShowTrackSelect(1);

    /* Update track selection from input */
    trackno = WheelSection(NTRACKS, 1);

    /* Update car preview selection */
    gPickPad = RawWheelSection(NPADS, 1);
    gThisCar = gPickPad + gCarFirstSel;

    /* Check for track change */
    if (trackno != oldTrack) {
        oldTrack = trackno;
        /* Play track change sound */
    }
}

/*
 * ShowTrackSelect - Show or hide track select screen
 * Arcade: sselect.c line ~661
 */
void ShowTrackSelect(s32 active)
{
    if (active) {
        /* Create track select UI if not already visible */
        /* Arcade: NewMultiBlit(TrackMultiBlit) */
        if (gSelect.track.state == SELECT_STATE_IDLE) {
            gSelect.track.state = SELECT_STATE_ACTIVE;
        }
    } else {
        /* Remove track select UI */
        gSelect.track.state = SELECT_STATE_IDLE;
    }
}

/*
 * TrackSelForce - Handle force feedback during track selection
 * Arcade: sselect.c line ~699
 * N64 has no force feedback - stub implementation
 */
void TrackSelForce(void)
{
    /* No force feedback on N64 - stub */
}

/*
 * init_car_select - Initialize car selection state
 * Arcade: sselect.c line ~1190
 */
void init_car_select(void)
{
    s32 i;

    /* gstate = CARSEL; - handled by game state machine */
    SetCountdownTimer(CAR_SELECT_TIME);
    ShowTrackSelect(0);
    gCameraTransition = 1;
    ShowCarSelect(1);

    negotiate_race = 0;
    negotiate_track = 3;  /* Holdoff phase */

    for (i = 0; i < MAX_LINKS; i++) {
        /* Reset negotiation data */
    }
}

/*
 * CarSel - Car selection state handler
 * Arcade: sselect.c line ~2252
 * Called every frame during CARSEL state
 */
void CarSel(void)
{
    CarSelForce();
    SelectCam();

    /* If car already locked, handle transmission select */
    if (gCarLocked) {
        ClutchSel();
        return;
    }

    /* Check for lock trigger */
    if (GasPressed(1) || TimeOut()) {
        gCarLocked = 1;
        gPickTime = 1;  /* Mark pick time */

        SetCountdownTimer(CLUTCH_SEL_TIME + GetCountdownTime());

        /* Play car select sound */
        gSoundOn = 0;

        /* Set flags based on options */
        return;
    }

    /* Still selecting - update from input */
    gPickPad = WheelSection(NPADS, 1);
    gThisCar = gPickPad + gCarFirstSel;
}

/*
 * ShowCarSelect - Show or hide car select screen
 * Arcade: sselect.c line ~2188
 */
void ShowCarSelect(s32 active)
{
    if (active) {
        if (gSelect.car.state == SELECT_STATE_IDLE) {
            gSelect.car.state = SELECT_STATE_ACTIVE;
            SetCountdownTimer(CAR_SELECT_TIME);
            gBeginTime = 0;
            /* Play garage ambient sound */
        }
    } else {
        gSelect.car.state = SELECT_STATE_IDLE;
    }
}

/*
 * CarSelForce - Handle force feedback during car selection
 * Arcade: sselect.c line ~2225
 * N64 has no force feedback - stub implementation
 */
void CarSelForce(void)
{
    /* No force feedback on N64 - stub */
}

/*
 * ClutchSel - Transmission selection
 * Arcade: sselect.c line ~2426
 * Handles auto/manual transmission choice
 */
void ClutchSel(void)
{
    /* If already locked, wait for race start */
    if (gClutchLocked) {
        if (negotiate_race == 3) {
            /* Ready to start race */
            /* gstate = PREPLAY; */
            ShowCarSelect(0);
        }

        if (negotiate_race > 1) {
            race_negotiation();
            return;
        }

        negotiate_race = 1;

        if (TimeOut()) {
            if (solo_flag) {
                negotiate_race = 3;
            } else {
                negotiate_race = 2;
            }
        }
        return;
    }

    /* Check for lock trigger */
    if (GasPressed(1) || TimeOut()) {
        /* Get transmission choice from input */
        autotrans = !WheelSection(2, 3);  /* 0=manual, 1=auto */
        gClutchLocked = 1;
    }
}

/*
 * create_cars - Create all cars for car select
 * Arcade: sselect.c line ~1218
 */
void create_cars(void)
{
    s32 i;

    gLastCar = -1;

    /* Create cars on each pad */
    for (i = 0; i < NPADS; i++) {
        /* CreateCar, SetupCar, StartCar for each pad */
        /* Also create alternate car banks (arcade has 3 banks of 4) */
    }
}

/*
 * SelectCam - Update selection screen camera
 * Arcade: called from TrackSel/CarSel
 */
void SelectCam(void)
{
    /* Handle camera transitions */
    if (gCameraTransition) {
        gCamFrames++;
        if (gCamFrames >= 30) {
            gCameraTransition = 0;
            gCurCam = gNextCam;
        }
    }
}

/*
 * track_negotiation - Negotiate track selection with linked players
 * Arcade: sselect.c line ~1008
 * N64 single-player - stub implementation
 */
void track_negotiation(void)
{
    /* No network on N64 - immediately proceed to car select */
    negotiate_track = 3;
    init_car_select();
}

/*
 * race_negotiation - Negotiate race start with linked players
 * Arcade: sselect.c (similar to track_negotiation)
 * N64 single-player - stub implementation
 */
void race_negotiation(void)
{
    /* No network on N64 - immediately proceed */
    negotiate_race = 3;
}

/*
 * waiting_for_others - Play "waiting for others" sound
 * Arcade: sselect.c line ~1293
 * N64 single-player - stub implementation
 */
void waiting_for_others(s16 type)
{
    /* No multiplayer on N64 - stub */
    (void)type;
}

/*
 * SetCountdownTimer - Set the countdown timer
 * Arcade: used throughout sselect.c
 */
void SetCountdownTimer(u32 time)
{
    countdown_timer = time;
}

/*
 * GetCountdownTime - Get remaining countdown time
 * Arcade: used throughout sselect.c
 */
u32 GetCountdownTime(void)
{
    return countdown_timer;
}

/*
 * TimeOut - Check if timer has expired
 * Arcade: used throughout sselect.c
 */
s32 TimeOut(void)
{
    if (countdown_timer > 0) {
        countdown_timer--;
        return 0;
    }
    return 1;
}

/*
 * ShowCountdown - Show/hide countdown display
 * Arcade: used for race start countdown
 */
void ShowCountdown(s32 active)
{
    /* Update countdown display visibility */
    (void)active;
}

/*
 * start_countdown - Start the race countdown
 * Arcade: called when all players ready
 */
void start_countdown(void)
{
    /* Begin 3-2-1 countdown sequence */
    ShowCountdown(1);
}

/*
 * WheelSection - Get wheel position section
 * Arcade: divides steering wheel range into sections
 * N64: Maps controller stick to sections
 */
s32 WheelSection(s32 sections, s32 snap)
{
    /* TODO: Read controller stick and map to section */
    /* For now, return center section */
    (void)snap;
    return sections / 2;
}

/*
 * RawWheelSection - Get raw wheel section without smoothing
 * Arcade: similar to WheelSection but without filtering
 */
s32 RawWheelSection(s32 sections, s32 snap)
{
    /* TODO: Read controller stick directly */
    (void)snap;
    return sections / 2;
}

/*
 * GasPressed - Check if gas/confirm button pressed
 * Arcade: checks gas pedal position
 * N64: checks A button
 */
s32 GasPressed(s32 check)
{
    /* TODO: Read controller A button */
    (void)check;
    return 0;
}

#endif /* NON_MATCHING */
