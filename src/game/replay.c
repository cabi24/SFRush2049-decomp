/**
 * replay.c - Replay recording and playback for Rush 2049 N64
 *
 * Records car state each frame for ghost playback and instant replay.
 */

#include "game/replay.h"
#include "game/game.h"

/* Position scale for compression (16-bit fixed point) */
#define POS_SCALE           16.0f       /* Units per fixed point unit */
#define VEL_SCALE           128.0f      /* Velocity scale for 8-bit */
#define QUAT_SCALE          16384.0f    /* Quaternion scale for 16-bit */

/* Extended frame frequency */
#define EXT_FRAME_INTERVAL  30          /* Record full state every 30 frames */

/* Global state */
ReplayState gReplay;
GhostState gGhost;

/* Static frame buffers (allocated from heap in real implementation) */
static ReplayFrame sFrameBuffer[MAX_REPLAY_FRAMES / REPLAY_SAMPLE_RATE];
static ReplayFrameExt sExtFrameBuffer[MAX_REPLAY_FRAMES / EXT_FRAME_INTERVAL];

/*
 * replay_init - Initialize replay system
 */
void replay_init(void)
{
    s32 i;

    /* Clear replay state */
    gReplay.num_cars = 0;
    gReplay.current_frame = 0;
    gReplay.total_frames = 0;
    gReplay.playback_frame = 0;
    gReplay.playback_speed = SPEED_NORMAL;
    gReplay.playback_state = PLAYBACK_PLAY;
    gReplay.loop_enabled = 0;
    gReplay.mode = REPLAY_MODE_NONE;
    gReplay.type = REPLAY_TYPE_FULL;
    gReplay.flags = 0;
    gReplay.camera_car = 0;
    gReplay.camera_mode = 0;
    gReplay.track_id = 0;
    gReplay.start_time = 0;
    gReplay.end_time = 0;

    /* Clear car replays */
    for (i = 0; i < MAX_REPLAY_CARS; i++) {
        gReplay.cars[i].frames = NULL;
        gReplay.cars[i].ext_frames = NULL;
        gReplay.cars[i].num_frames = 0;
        gReplay.cars[i].max_frames = 0;
        gReplay.cars[i].car_index = 0;
        gReplay.cars[i].car_type = 0;
        gReplay.cars[i].car_color = 0;
        gReplay.cars[i].valid = 0;
    }

    /* Initialize ghost system */
    ghost_init();
}

/*
 * replay_reset - Reset replay for new recording
 */
void replay_reset(void)
{
    s32 i;

    gReplay.current_frame = 0;
    gReplay.total_frames = 0;
    gReplay.playback_frame = 0;
    gReplay.mode = REPLAY_MODE_NONE;
    gReplay.flags = 0;

    for (i = 0; i < MAX_REPLAY_CARS; i++) {
        gReplay.cars[i].num_frames = 0;
        gReplay.cars[i].valid = 0;
    }
}

/*
 * replay_allocate - Allocate replay buffers
 */
void replay_allocate(s32 num_cars, u32 max_frames)
{
    s32 i;
    u32 frames_per_car;

    if (num_cars > MAX_REPLAY_CARS) {
        num_cars = MAX_REPLAY_CARS;
    }

    frames_per_car = max_frames / REPLAY_SAMPLE_RATE;
    gReplay.num_cars = num_cars;

    /* For now, use static buffers divided among cars */
    for (i = 0; i < num_cars; i++) {
        gReplay.cars[i].frames = &sFrameBuffer[i * (frames_per_car / num_cars)];
        gReplay.cars[i].ext_frames = &sExtFrameBuffer[i * (frames_per_car / num_cars / EXT_FRAME_INTERVAL)];
        gReplay.cars[i].max_frames = frames_per_car / num_cars;
        gReplay.cars[i].num_frames = 0;
        gReplay.cars[i].car_index = i;
        gReplay.cars[i].valid = 0;
    }
}

/*
 * replay_free - Free replay buffers
 */
void replay_free(void)
{
    s32 i;

    for (i = 0; i < MAX_REPLAY_CARS; i++) {
        gReplay.cars[i].frames = NULL;
        gReplay.cars[i].ext_frames = NULL;
        gReplay.cars[i].max_frames = 0;
        gReplay.cars[i].num_frames = 0;
        gReplay.cars[i].valid = 0;
    }

    gReplay.num_cars = 0;
}

/*
 * replay_start_recording - Begin recording replay
 */
void replay_start_recording(void)
{
    replay_reset();
    gReplay.mode = REPLAY_MODE_RECORDING;
    gReplay.start_time = 0; /* Would get from system time */
}

/*
 * replay_stop_recording - Stop recording replay
 */
void replay_stop_recording(void)
{
    s32 i;

    gReplay.mode = REPLAY_MODE_NONE;
    gReplay.end_time = 0; /* Would get from system time */
    gReplay.total_frames = gReplay.current_frame;
    gReplay.flags |= REPLAY_FLAG_VALID | REPLAY_FLAG_COMPLETE;

    /* Mark all car replays as valid */
    for (i = 0; i < gReplay.num_cars; i++) {
        if (gReplay.cars[i].num_frames > 0) {
            gReplay.cars[i].valid = 1;
        }
    }
}

/*
 * replay_record_frame - Record current frame for all cars
 */
void replay_record_frame(void)
{
    s32 i;

    if (gReplay.mode != REPLAY_MODE_RECORDING) {
        return;
    }

    /* Only record at sample rate */
    if ((gReplay.current_frame % REPLAY_SAMPLE_RATE) != 0) {
        gReplay.current_frame++;
        return;
    }

    /* Record each car */
    for (i = 0; i < gReplay.num_cars; i++) {
        replay_record_car_state(i);
    }

    gReplay.current_frame++;
}

/*
 * replay_record_car_state - Record single car's state
 */
void replay_record_car_state(s32 car_index)
{
    CarReplay *car;
    ReplayFrame *frame;
    u32 frame_idx;

    if (car_index < 0 || car_index >= gReplay.num_cars) {
        return;
    }

    car = &gReplay.cars[car_index];

    if (car->frames == NULL || car->num_frames >= car->max_frames) {
        return;
    }

    frame_idx = car->num_frames;
    frame = &car->frames[frame_idx];

    /* Get car state from physics system */
    /* TODO: Actually read from car physics state */
    /* For now, fill with placeholder data */
    frame->pos_x = 0;
    frame->pos_y = 0;
    frame->pos_z = 0;
    frame->quat_x = 0;
    frame->quat_y = 0;
    frame->quat_z = 0;
    frame->quat_w = (s16)QUAT_SCALE;  /* Identity quaternion w=1 */
    frame->vel_x = 0;
    frame->vel_y = 0;
    frame->vel_z = 0;
    frame->wheel_rot[0] = 0;
    frame->wheel_rot[1] = 0;
    frame->wheel_rot[2] = 0;
    frame->wheel_rot[3] = 0;
    frame->steer_angle = 0;
    frame->flags = 0;

    car->num_frames++;

    /* Record extended frame periodically */
    if ((frame_idx % EXT_FRAME_INTERVAL) == 0 && car->ext_frames != NULL) {
        ReplayFrameExt *ext = &car->ext_frames[frame_idx / EXT_FRAME_INTERVAL];
        /* TODO: Fill extended state */
        ext->pos[0] = 0.0f;
        ext->pos[1] = 0.0f;
        ext->pos[2] = 0.0f;
        ext->vel[0] = 0.0f;
        ext->vel[1] = 0.0f;
        ext->vel[2] = 0.0f;
        ext->engine_rpm = 0.0f;
        ext->speed = 0.0f;
        ext->gear = 1;
        ext->on_ground = 1;
        ext->surface_type = 0;
    }
}

/*
 * Playback functions
 */

void replay_start_playback(void)
{
    if (!(gReplay.flags & REPLAY_FLAG_VALID)) {
        return;
    }

    gReplay.mode = REPLAY_MODE_PLAYBACK;
    gReplay.playback_frame = 0;
    gReplay.playback_speed = SPEED_NORMAL;
    gReplay.playback_state = PLAYBACK_PLAY;
}

void replay_stop_playback(void)
{
    gReplay.mode = REPLAY_MODE_NONE;
}

void replay_update_playback(void)
{
    s32 speed_fixed;
    s32 frame_delta;

    if (gReplay.mode != REPLAY_MODE_PLAYBACK) {
        return;
    }

    if (gReplay.playback_state == PLAYBACK_PAUSE) {
        return;
    }

    /* Calculate frame advance based on speed */
    speed_fixed = gReplay.playback_speed;

    if (gReplay.playback_state == PLAYBACK_REWIND) {
        speed_fixed = -speed_fixed;
    }

    /* Fixed point 8.8 to frames */
    frame_delta = speed_fixed >> 8;

    gReplay.playback_frame += frame_delta;

    /* Handle bounds */
    if (gReplay.playback_frame < 0) {
        if (gReplay.loop_enabled) {
            gReplay.playback_frame = gReplay.total_frames - 1;
        } else {
            gReplay.playback_frame = 0;
            gReplay.playback_state = PLAYBACK_PAUSE;
        }
    }

    if (gReplay.playback_frame >= (s32)gReplay.total_frames) {
        if (gReplay.loop_enabled) {
            gReplay.playback_frame = 0;
        } else {
            gReplay.playback_frame = gReplay.total_frames - 1;
            gReplay.playback_state = PLAYBACK_PAUSE;
        }
    }
}

void replay_set_frame(u32 frame)
{
    if (frame >= gReplay.total_frames) {
        frame = gReplay.total_frames - 1;
    }
    gReplay.playback_frame = frame;
}

void replay_set_speed(s16 speed)
{
    gReplay.playback_speed = speed;
}

void replay_toggle_pause(void)
{
    if (gReplay.playback_state == PLAYBACK_PAUSE) {
        gReplay.playback_state = PLAYBACK_PLAY;
    } else {
        gReplay.playback_state = PLAYBACK_PAUSE;
    }
}

void replay_play(void)
{
    gReplay.playback_state = PLAYBACK_PLAY;
    gReplay.playback_speed = SPEED_NORMAL;
}

void replay_pause(void)
{
    gReplay.playback_state = PLAYBACK_PAUSE;
}

void replay_rewind(void)
{
    gReplay.playback_state = PLAYBACK_REWIND;
    gReplay.playback_speed = SPEED_FAST;
}

void replay_fast_forward(void)
{
    gReplay.playback_state = PLAYBACK_FAST_FORWARD;
    gReplay.playback_speed = SPEED_FAST;
}

void replay_slow_motion(void)
{
    gReplay.playback_state = PLAYBACK_SLOW_MO;
    gReplay.playback_speed = SPEED_SLOW;
}

void replay_skip_forward(u32 frames)
{
    gReplay.playback_frame += frames;
    if (gReplay.playback_frame >= gReplay.total_frames) {
        gReplay.playback_frame = gReplay.total_frames - 1;
    }
}

void replay_skip_backward(u32 frames)
{
    if (gReplay.playback_frame >= frames) {
        gReplay.playback_frame -= frames;
    } else {
        gReplay.playback_frame = 0;
    }
}

/*
 * State retrieval for rendering
 */

void replay_get_car_state(s32 car_index, u32 frame, f32 *pos, f32 *matrix)
{
    CarReplay *car;
    ReplayFrame *rf;
    u32 sample_frame;

    if (car_index < 0 || car_index >= gReplay.num_cars) {
        return;
    }

    car = &gReplay.cars[car_index];
    if (!car->valid || car->frames == NULL) {
        return;
    }

    /* Convert to sample index */
    sample_frame = frame / REPLAY_SAMPLE_RATE;
    if (sample_frame >= car->num_frames) {
        sample_frame = car->num_frames - 1;
    }

    rf = &car->frames[sample_frame];

    /* Decompress position */
    if (pos != NULL) {
        pos[0] = (f32)rf->pos_x * POS_SCALE;
        pos[1] = (f32)rf->pos_y * POS_SCALE;
        pos[2] = (f32)rf->pos_z * POS_SCALE;
    }

    /* Convert quaternion to matrix */
    if (matrix != NULL) {
        f32 qx = (f32)rf->quat_x / QUAT_SCALE;
        f32 qy = (f32)rf->quat_y / QUAT_SCALE;
        f32 qz = (f32)rf->quat_z / QUAT_SCALE;
        f32 qw = (f32)rf->quat_w / QUAT_SCALE;

        /* Quaternion to 3x3 rotation matrix */
        f32 xx = qx * qx;
        f32 xy = qx * qy;
        f32 xz = qx * qz;
        f32 xw = qx * qw;
        f32 yy = qy * qy;
        f32 yz = qy * qz;
        f32 yw = qy * qw;
        f32 zz = qz * qz;
        f32 zw = qz * qw;

        matrix[0] = 1.0f - 2.0f * (yy + zz);
        matrix[1] = 2.0f * (xy - zw);
        matrix[2] = 2.0f * (xz + yw);
        matrix[3] = 2.0f * (xy + zw);
        matrix[4] = 1.0f - 2.0f * (xx + zz);
        matrix[5] = 2.0f * (yz - xw);
        matrix[6] = 2.0f * (xz - yw);
        matrix[7] = 2.0f * (yz + xw);
        matrix[8] = 1.0f - 2.0f * (xx + yy);
    }
}

void replay_interpolate_state(s32 car_index, f32 frame_f, f32 *pos, f32 *matrix)
{
    /* For now, just use nearest frame */
    replay_get_car_state(car_index, (u32)frame_f, pos, matrix);
}

/*
 * Ghost system
 */

void ghost_init(void)
{
    gGhost.best_lap.data.frames = NULL;
    gGhost.best_lap.data.num_frames = 0;
    gGhost.best_lap.data.valid = 0;
    gGhost.best_lap.lap_time = 0;
    gGhost.best_lap.is_best = 0;

    gGhost.current_lap.data.frames = NULL;
    gGhost.current_lap.data.num_frames = 0;
    gGhost.current_lap.data.valid = 0;
    gGhost.current_lap.lap_time = 0;

    gGhost.comparison_lap.data.frames = NULL;
    gGhost.comparison_lap.data.num_frames = 0;
    gGhost.comparison_lap.data.valid = 0;

    gGhost.ghost_visible = 1;
    gGhost.recording = 0;
    gGhost.comparing = 0;
    gGhost.time_delta = 0.0f;
}

void ghost_reset(void)
{
    gGhost.current_lap.data.num_frames = 0;
    gGhost.current_lap.data.valid = 0;
    gGhost.recording = 0;
    gGhost.time_delta = 0.0f;
}

void ghost_start_lap(void)
{
    gGhost.current_lap.data.num_frames = 0;
    gGhost.current_lap.lap_time = 0;
    gGhost.recording = 1;
}

void ghost_end_lap(u32 lap_time)
{
    gGhost.recording = 0;
    gGhost.current_lap.lap_time = lap_time;
    gGhost.current_lap.data.valid = 1;

    /* Check if this is a new best */
    if (!gGhost.best_lap.data.valid ||
        lap_time < gGhost.best_lap.lap_time) {
        /* Copy current to best */
        /* TODO: Actual copy implementation */
        gGhost.best_lap.lap_time = lap_time;
        gGhost.best_lap.is_best = 1;
        gGhost.best_lap.data.valid = 1;
    }
}

void ghost_record_frame(void)
{
    if (!gGhost.recording) {
        return;
    }

    /* TODO: Record player car state to current_lap */
    gGhost.current_lap.lap_time++;
}

void ghost_update(void)
{
    if (!gGhost.comparing || !gGhost.best_lap.data.valid) {
        return;
    }

    /* Calculate time delta vs ghost */
    /* TODO: Compare player position to ghost position */
}

void ghost_draw(void)
{
    if (!gGhost.ghost_visible || !gGhost.best_lap.data.valid) {
        return;
    }

    /* TODO: Render ghost car at interpolated position */
}

void ghost_load_best(u32 track_id)
{
    /* TODO: Load from save file */
}

void ghost_save_best(u32 track_id)
{
    /* TODO: Save to Controller Pak */
}

void ghost_set_comparison(GhostLap *lap)
{
    if (lap != NULL && lap->data.valid) {
        gGhost.comparing = 1;
        /* TODO: Copy lap data */
    } else {
        gGhost.comparing = 0;
    }
}

f32 ghost_get_time_delta(void)
{
    return gGhost.time_delta;
}

/*
 * Queries
 */

s32 replay_is_recording(void)
{
    return (gReplay.mode == REPLAY_MODE_RECORDING);
}

s32 replay_is_playing(void)
{
    return (gReplay.mode == REPLAY_MODE_PLAYBACK);
}

u32 replay_get_length(void)
{
    return gReplay.total_frames;
}

u32 replay_get_current_frame(void)
{
    if (gReplay.mode == REPLAY_MODE_RECORDING) {
        return gReplay.current_frame;
    }
    return gReplay.playback_frame;
}

f32 replay_get_progress(void)
{
    if (gReplay.total_frames == 0) {
        return 0.0f;
    }
    return (f32)gReplay.playback_frame / (f32)gReplay.total_frames;
}

/*
 * Camera
 */

void replay_set_camera_car(s32 car_index)
{
    if (car_index < -1 || car_index >= gReplay.num_cars) {
        return;
    }
    gReplay.camera_car = car_index;
}

void replay_set_camera_mode(s32 mode)
{
    gReplay.camera_mode = mode;
}

void replay_cycle_camera(void)
{
    gReplay.camera_car++;
    if (gReplay.camera_car >= gReplay.num_cars) {
        gReplay.camera_car = -1;  /* Free camera */
    }
}

/*
 * Save/Load stubs
 */

s32 replay_save(const char *filename)
{
    /* TODO: Implement file save */
    return 0;
}

s32 replay_load(const char *filename)
{
    /* TODO: Implement file load */
    return 0;
}

s32 ghost_save_to_pak(u32 track_id)
{
    /* TODO: Save to Controller Pak */
    return 0;
}

s32 ghost_load_from_pak(u32 track_id)
{
    /* TODO: Load from Controller Pak */
    return 0;
}
