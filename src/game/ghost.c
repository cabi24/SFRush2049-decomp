/**
 * ghost.c - Ghost car replay system for Rush 2049 N64
 *
 * Ghost car system for time trial mode. Records player position/orientation
 * for playback as a translucent "ghost" to race against.
 *
 * Based on arcade Rush The Rock dead reckoning concepts:
 *   - reckon.c: dead_reckon_posuv, dead_reckon_all
 *   - modeldat.h: RECKON structure
 *   - maxpath.h: MPATH structure
 */

#include "game/ghost.h"
#include "types.h"

#ifdef NON_MATCHING

/* Math helpers - declarations only, not definitions */
extern f32 sqrtf(f32 x);
extern f32 sinf(f32 x);
extern f32 cosf(f32 x);
extern f32 fabsf(f32 x);

/* Arcade-style vector math (from vecmath.c) */
static void veccopy(f32 *src, f32 *dst) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

static void scalmul(f32 *v, f32 s, f32 *out) {
    out[0] = v[0] * s;
    out[1] = v[1] * s;
    out[2] = v[2] * s;
}

static void vecadd(f32 *a, f32 *b, f32 *out) {
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
    out[2] = a[2] + b[2];
}

static void vecsub(f32 *a, f32 *b, f32 *out) {
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
    out[2] = a[2] - b[2];
}

static f32 vecmag(f32 *v) {
    return sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

/* Global ghost system state */
GhostSystem gGhostSystem;

/**
 * ghost_system_init - Initialize the ghost car system
 *
 * Initializes all ghost car structures and buffers.
 */
void ghost_system_init(void) {
    s32 i;

    /* Clear recording ghost */
    gGhostSystem.recording.state = GHOST_STATE_IDLE;
    gGhostSystem.recording.frame_count = 0;
    gGhostSystem.recording.max_frames = 0;
    gGhostSystem.recording.track_id = 0;
    gGhostSystem.recording.lap_time = 0;
    gGhostSystem.recording.car_model = 0;
    gGhostSystem.recording.car_color = 0;
    gGhostSystem.recording.valid = 0;
    gGhostSystem.recording.is_best = 0;
    gGhostSystem.recording.frame_data = gGhostSystem.record_buffer;

    /* Clear best lap ghost */
    gGhostSystem.best_lap.state = GHOST_STATE_IDLE;
    gGhostSystem.best_lap.frame_count = 0;
    gGhostSystem.best_lap.max_frames = 0;
    gGhostSystem.best_lap.valid = 0;
    gGhostSystem.best_lap.is_best = 1;
    gGhostSystem.best_lap.frame_data = gGhostSystem.best_buffer;

    /* Clear loaded ghost */
    gGhostSystem.loaded.state = GHOST_STATE_IDLE;
    gGhostSystem.loaded.frame_count = 0;
    gGhostSystem.loaded.max_frames = 0;
    gGhostSystem.loaded.valid = 0;
    gGhostSystem.loaded.frame_data = gGhostSystem.loaded_buffer;

    /* System state */
    gGhostSystem.ghost_visible = GHOST_VISIBLE;
    gGhostSystem.recording_active = 0;
    gGhostSystem.playback_active = 0;
    gGhostSystem.comparison_mode = 0;

    /* Time comparison */
    gGhostSystem.time_delta = 0.0f;
    gGhostSystem.distance_delta = 0.0f;

    /* Render settings */
    gGhostSystem.render_color = GHOST_COLOR_BLUE;
    gGhostSystem.alpha = GHOST_ALPHA_NORMAL;
    gGhostSystem.interp_mode = GHOST_INTERP_LINEAR;

    /* Clear position data */
    for (i = 0; i < 3; i++) {
        gGhostSystem.recording.pos[i] = 0.0f;
        gGhostSystem.recording.rot[i] = 0.0f;
        gGhostSystem.recording.vel[i] = 0.0f;
        gGhostSystem.best_lap.pos[i] = 0.0f;
        gGhostSystem.best_lap.rot[i] = 0.0f;
        gGhostSystem.best_lap.vel[i] = 0.0f;
        gGhostSystem.loaded.pos[i] = 0.0f;
        gGhostSystem.loaded.rot[i] = 0.0f;
        gGhostSystem.loaded.vel[i] = 0.0f;
    }
}

/**
 * ghost_system_reset - Reset ghost system for new race
 */
void ghost_system_reset(void) {
    gGhostSystem.recording.state = GHOST_STATE_IDLE;
    gGhostSystem.recording.frame_count = 0;
    gGhostSystem.recording.valid = 0;

    gGhostSystem.recording_active = 0;
    gGhostSystem.playback_active = 0;
    gGhostSystem.time_delta = 0.0f;
    gGhostSystem.distance_delta = 0.0f;
}

/*
 * Recording functions
 */

/**
 * ghost_start_recording - Begin recording ghost data
 * @track_id: Current track identifier
 * @car_model: Player's car model
 * @car_color: Player's car color
 */
void ghost_start_recording(s32 track_id, s32 car_model, s32 car_color) {
    gGhostSystem.recording.state = GHOST_STATE_RECORDING;
    gGhostSystem.recording.frame_count = 0;
    gGhostSystem.recording.max_frames = GHOST_MAX_FRAMES / GHOST_SAMPLE_RATE;
    gGhostSystem.recording.track_id = track_id;
    gGhostSystem.recording.lap_time = 0;
    gGhostSystem.recording.car_model = car_model;
    gGhostSystem.recording.car_color = car_color;
    gGhostSystem.recording.valid = 0;

    gGhostSystem.recording_active = 1;
}

/**
 * ghost_stop_recording - Stop recording and finalize ghost data
 * @lap_time: Final lap time in centiseconds
 */
void ghost_stop_recording(s32 lap_time) {
    gGhostSystem.recording.state = GHOST_STATE_IDLE;
    gGhostSystem.recording.lap_time = lap_time;
    gGhostSystem.recording.valid = 1;
    gGhostSystem.recording.max_frames = gGhostSystem.recording.frame_count;

    gGhostSystem.recording_active = 0;

    /* Check if this is a new best lap */
    if (ghost_is_best_lap(lap_time)) {
        ghost_promote_to_best();
    }
}

/**
 * ghost_cancel_recording - Cancel current recording
 */
void ghost_cancel_recording(void) {
    gGhostSystem.recording.state = GHOST_STATE_IDLE;
    gGhostSystem.recording.frame_count = 0;
    gGhostSystem.recording.valid = 0;
    gGhostSystem.recording_active = 0;
}

/**
 * ghost_record_car_frame - Record current car state
 * @pos: Car position [x, y, z]
 * @rot: Car rotation [pitch, yaw, roll]
 * @vel: Car velocity [vx, vy, vz]
 *
 * Records car state every GHOST_SAMPLE_RATE frames.
 */
void ghost_record_car_frame(f32 *pos, f32 *rot, f32 *vel) {
    f32 *frame_data;
    s32 offset;

    if (!gGhostSystem.recording_active) {
        return;
    }

    if (gGhostSystem.recording.frame_count >= gGhostSystem.recording.max_frames) {
        return;
    }

    if (gGhostSystem.recording.frame_data == NULL) {
        return;
    }

    /* Calculate buffer offset */
    offset = gGhostSystem.recording.frame_count * GHOST_DATA_PER_FRAME;
    frame_data = gGhostSystem.recording.frame_data;

    /* Store position */
    frame_data[offset + 0] = pos[0];
    frame_data[offset + 1] = pos[1];
    frame_data[offset + 2] = pos[2];

    /* Store rotation */
    frame_data[offset + 3] = rot[0];
    frame_data[offset + 4] = rot[1];
    frame_data[offset + 5] = rot[2];

    /* Store velocity */
    frame_data[offset + 6] = vel[0];
    frame_data[offset + 7] = vel[1];
    frame_data[offset + 8] = vel[2];

    gGhostSystem.recording.frame_count++;
}

/*
 * Playback functions
 */

/**
 * ghost_start_playback - Begin ghost playback
 */
void ghost_start_playback(void) {
    GhostCar *ghost;

    /* Use best lap if valid, otherwise loaded ghost */
    if (gGhostSystem.best_lap.valid) {
        ghost = &gGhostSystem.best_lap;
    } else if (gGhostSystem.loaded.valid) {
        ghost = &gGhostSystem.loaded;
    } else {
        return;
    }

    ghost->state = GHOST_STATE_PLAYING;
    ghost->frame_count = 0;
    gGhostSystem.playback_active = 1;
    gGhostSystem.comparison_mode = 1;
}

/**
 * ghost_stop_playback - Stop ghost playback
 */
void ghost_stop_playback(void) {
    gGhostSystem.best_lap.state = GHOST_STATE_IDLE;
    gGhostSystem.loaded.state = GHOST_STATE_IDLE;
    gGhostSystem.playback_active = 0;
    gGhostSystem.comparison_mode = 0;
}

/**
 * ghost_update_playback - Update ghost position for current frame
 *
 * Updates ghost position using interpolation between recorded frames.
 * Based on arcade dead_reckon_posuv function.
 */
void ghost_update_playback(void) {
    GhostCar *ghost;
    f32 frame_f;

    if (!gGhostSystem.playback_active) {
        return;
    }

    /* Get active ghost */
    if (gGhostSystem.best_lap.state == GHOST_STATE_PLAYING) {
        ghost = &gGhostSystem.best_lap;
    } else if (gGhostSystem.loaded.state == GHOST_STATE_PLAYING) {
        ghost = &gGhostSystem.loaded;
    } else {
        return;
    }

    /* Advance frame */
    ghost->frame_count++;

    /* Loop or stop at end */
    if (ghost->frame_count >= ghost->max_frames) {
        ghost->frame_count = ghost->max_frames - 1;
    }

    /* Interpolate frame */
    frame_f = (f32)ghost->frame_count;
    ghost_interpolate_frame(ghost, frame_f);
}

/**
 * ghost_set_playback_frame - Set playback to specific frame
 * @frame: Frame index
 */
void ghost_set_playback_frame(s32 frame) {
    GhostCar *ghost;

    if (gGhostSystem.best_lap.state == GHOST_STATE_PLAYING) {
        ghost = &gGhostSystem.best_lap;
    } else if (gGhostSystem.loaded.state == GHOST_STATE_PLAYING) {
        ghost = &gGhostSystem.loaded;
    } else {
        return;
    }

    if (frame < 0) {
        frame = 0;
    }
    if (frame >= ghost->max_frames) {
        frame = ghost->max_frames - 1;
    }

    ghost->frame_count = frame;
    ghost_interpolate_frame(ghost, (f32)frame);
}

/*
 * Interpolation (based on arcade dead_reckon_posuv)
 */

/**
 * ghost_interpolate_frame - Interpolate ghost position between frames
 * @ghost: Ghost car to interpolate
 * @frame_f: Floating-point frame index for sub-frame interpolation
 *
 * Linear interpolation between recorded frames.
 * Based on arcade dead_reckon_posuv dead reckoning.
 */
void ghost_interpolate_frame(GhostCar *ghost, f32 frame_f) {
    s32 frame0, frame1;
    s32 offset0, offset1;
    f32 t;
    f32 *data;
    s32 i;

    if (ghost == NULL || ghost->frame_data == NULL) {
        return;
    }

    if (ghost->max_frames <= 1) {
        return;
    }

    /* Get frame indices */
    frame0 = (s32)frame_f;
    frame1 = frame0 + 1;

    if (frame0 < 0) frame0 = 0;
    if (frame1 >= ghost->max_frames) frame1 = ghost->max_frames - 1;
    if (frame0 >= ghost->max_frames - 1) frame0 = ghost->max_frames - 2;

    /* Interpolation factor */
    t = frame_f - (f32)frame0;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    data = ghost->frame_data;
    offset0 = frame0 * GHOST_DATA_PER_FRAME;
    offset1 = frame1 * GHOST_DATA_PER_FRAME;

    /* Interpolate position (arcade: RWR = base_RWR + base_RWV * dt) */
    ghost->pos[0] = data[offset0 + 0] * (1.0f - t) + data[offset1 + 0] * t;
    ghost->pos[1] = data[offset0 + 1] * (1.0f - t) + data[offset1 + 1] * t;
    ghost->pos[2] = data[offset0 + 2] * (1.0f - t) + data[offset1 + 2] * t;

    /* Interpolate rotation */
    ghost->rot[0] = data[offset0 + 3] * (1.0f - t) + data[offset1 + 3] * t;
    ghost->rot[1] = data[offset0 + 4] * (1.0f - t) + data[offset1 + 4] * t;
    ghost->rot[2] = data[offset0 + 5] * (1.0f - t) + data[offset1 + 5] * t;

    /* Interpolate velocity */
    ghost->vel[0] = data[offset0 + 6] * (1.0f - t) + data[offset1 + 6] * t;
    ghost->vel[1] = data[offset0 + 7] * (1.0f - t) + data[offset1 + 7] * t;
    ghost->vel[2] = data[offset0 + 8] * (1.0f - t) + data[offset1 + 8] * t;
}

/**
 * ghost_dead_reckon - Dead reckon ghost position forward in time
 * @reckon: Dead reckoning state structure
 * @dt: Delta time in seconds
 *
 * Based on arcade dead_reckon_posuv from reckon.c
 */
void ghost_dead_reckon(GhostReckon *reckon, f32 dt) {
    f32 temp[3];
    s32 i;

    if (reckon == NULL) {
        return;
    }

    /* Copy base velocity */
    veccopy(reckon->base_vel, reckon->vel);

    /* Extrapolate position: pos = base_pos + base_vel * dt */
    scalmul(reckon->base_vel, dt, temp);
    vecadd(reckon->base_pos, temp, reckon->pos);

    /* Interpolate quaternion if steps remaining */
    if (reckon->num_quat_steps > 0) {
        reckon->num_quat_steps--;
        for (i = 0; i < 4; i++) {
            reckon->base_quat[i] += reckon->delta_quat[i];
        }
        ghost_make_matrix_from_quat(reckon->base_quat, reckon->matrix);
    }
}

/**
 * ghost_make_matrix_from_quat - Convert quaternion to rotation matrix
 * @quat: Input quaternion [x, y, z, w]
 * @matrix: Output 3x3 rotation matrix
 *
 * Based on arcade make_uvs_from_quat function
 */
void ghost_make_matrix_from_quat(f32 *quat, f32 matrix[3][3]) {
    f32 x, y, z, w;
    f32 xx, xy, xz, xw;
    f32 yy, yz, yw;
    f32 zz, zw;

    x = quat[0];
    y = quat[1];
    z = quat[2];
    w = quat[3];

    xx = x * x;
    xy = x * y;
    xz = x * z;
    xw = x * w;
    yy = y * y;
    yz = y * z;
    yw = y * w;
    zz = z * z;
    zw = z * w;

    /* Build rotation matrix from quaternion */
    matrix[0][0] = 1.0f - 2.0f * (yy + zz);
    matrix[0][1] = 2.0f * (xy - zw);
    matrix[0][2] = 2.0f * (xz + yw);

    matrix[1][0] = 2.0f * (xy + zw);
    matrix[1][1] = 1.0f - 2.0f * (xx + zz);
    matrix[1][2] = 2.0f * (yz - xw);

    matrix[2][0] = 2.0f * (xz - yw);
    matrix[2][1] = 2.0f * (yz + xw);
    matrix[2][2] = 1.0f - 2.0f * (xx + yy);
}

/**
 * ghost_make_quat_from_euler - Convert Euler angles to quaternion
 * @rot: Input rotation [pitch, yaw, roll] in radians
 * @quat: Output quaternion [x, y, z, w]
 */
void ghost_make_quat_from_euler(f32 *rot, f32 *quat) {
    f32 cy, sy, cp, sp, cr, sr;
    f32 pitch, yaw, roll;

    pitch = rot[0] * 0.5f;
    yaw = rot[1] * 0.5f;
    roll = rot[2] * 0.5f;

    cy = cosf(yaw);
    sy = sinf(yaw);
    cp = cosf(pitch);
    sp = sinf(pitch);
    cr = cosf(roll);
    sr = sinf(roll);

    quat[3] = cr * cp * cy + sr * sp * sy;  /* w */
    quat[0] = sr * cp * cy - cr * sp * sy;  /* x */
    quat[1] = cr * sp * cy + sr * cp * sy;  /* y */
    quat[2] = cr * cp * sy - sr * sp * cy;  /* z */
}

/*
 * Rendering
 */

/**
 * ghost_render - Render active ghost car
 */
void ghost_render(void) {
    GhostCar *ghost;

    if (!gGhostSystem.ghost_visible) {
        return;
    }

    if (!gGhostSystem.playback_active) {
        return;
    }

    /* Get active ghost */
    if (gGhostSystem.best_lap.state == GHOST_STATE_PLAYING &&
        gGhostSystem.best_lap.valid) {
        ghost = &gGhostSystem.best_lap;
    } else if (gGhostSystem.loaded.state == GHOST_STATE_PLAYING &&
               gGhostSystem.loaded.valid) {
        ghost = &gGhostSystem.loaded;
    } else {
        return;
    }

    ghost_render_car(ghost, gGhostSystem.render_color, gGhostSystem.alpha);
}

/**
 * ghost_render_car - Render a specific ghost car
 * @ghost: Ghost car to render
 * @color: RGBA color for ghost tint
 * @alpha: Transparency (0-255)
 *
 * Stub - actual rendering done by display system
 */
void ghost_render_car(GhostCar *ghost, s32 color, s32 alpha) {
    /* Rendering is handled by the display system using ghost position/rotation */
    /* This function is a placeholder for ghost-specific rendering setup */

    /* Set ghost transparency in display state */
    /* Push transformation matrix */
    /* Render car model with ghost shader/tint */
}

/**
 * ghost_set_visibility - Set ghost visibility
 * @visible: GHOST_VISIBLE or GHOST_HIDDEN
 */
void ghost_set_visibility(s32 visible) {
    gGhostSystem.ghost_visible = (u8)visible;
}

/**
 * ghost_set_color - Set ghost render color
 * @color: RGBA color constant
 */
void ghost_set_color(s32 color) {
    gGhostSystem.render_color = color;
}

/**
 * ghost_set_alpha - Set ghost transparency
 * @alpha: 0 (invisible) to 255 (opaque)
 */
void ghost_set_alpha(s32 alpha) {
    if (alpha < 0) alpha = 0;
    if (alpha > 255) alpha = 255;
    gGhostSystem.alpha = alpha;
}

/*
 * Time comparison
 */

/**
 * ghost_get_time_delta - Get time difference from ghost
 * @return: Time delta in seconds (positive = ahead, negative = behind)
 */
f32 ghost_get_time_delta(void) {
    return gGhostSystem.time_delta;
}

/**
 * ghost_get_distance_delta - Get position difference from ghost
 * @return: Distance in feet
 */
f32 ghost_get_distance_delta(void) {
    return gGhostSystem.distance_delta;
}

/**
 * ghost_update_comparison - Update time/distance comparison with player
 * @player_pos: Player's current position
 */
void ghost_update_comparison(f32 *player_pos) {
    GhostCar *ghost;
    f32 diff[3];

    if (!gGhostSystem.comparison_mode) {
        return;
    }

    /* Get active ghost */
    if (gGhostSystem.best_lap.state == GHOST_STATE_PLAYING) {
        ghost = &gGhostSystem.best_lap;
    } else if (gGhostSystem.loaded.state == GHOST_STATE_PLAYING) {
        ghost = &gGhostSystem.loaded;
    } else {
        return;
    }

    /* Calculate distance delta */
    vecsub(player_pos, ghost->pos, diff);
    gGhostSystem.distance_delta = vecmag(diff);

    /* Estimate time delta based on velocity difference */
    /* Positive = player ahead, negative = player behind */
    if (ghost->vel[1] > 0.1f) {
        /* Assume forward is Y axis */
        gGhostSystem.time_delta = diff[1] / ghost->vel[1];
    }

    /* Update ghost color based on position */
    if (gGhostSystem.time_delta > 0.5f) {
        gGhostSystem.render_color = GHOST_COLOR_GREEN;  /* Player ahead */
    } else if (gGhostSystem.time_delta < -0.5f) {
        gGhostSystem.render_color = GHOST_COLOR_RED;    /* Player behind */
    } else {
        gGhostSystem.render_color = GHOST_COLOR_BLUE;   /* Close race */
    }
}

/*
 * Best lap management
 */

/**
 * ghost_is_best_lap - Check if lap time is a new best
 * @lap_time: Lap time in centiseconds
 * @return: 1 if new best, 0 otherwise
 */
s32 ghost_is_best_lap(s32 lap_time) {
    if (!gGhostSystem.best_lap.valid) {
        return 1;  /* No previous best */
    }

    return (lap_time < gGhostSystem.best_lap.lap_time);
}

/**
 * ghost_promote_to_best - Promote current recording to best lap
 */
void ghost_promote_to_best(void) {
    ghost_copy_recording_to_best();
}

/**
 * ghost_copy_recording_to_best - Copy recording data to best lap
 */
void ghost_copy_recording_to_best(void) {
    s32 i;
    s32 data_size;

    if (!gGhostSystem.recording.valid) {
        return;
    }

    /* Copy metadata */
    gGhostSystem.best_lap.track_id = gGhostSystem.recording.track_id;
    gGhostSystem.best_lap.lap_time = gGhostSystem.recording.lap_time;
    gGhostSystem.best_lap.car_model = gGhostSystem.recording.car_model;
    gGhostSystem.best_lap.car_color = gGhostSystem.recording.car_color;
    gGhostSystem.best_lap.max_frames = gGhostSystem.recording.frame_count;
    gGhostSystem.best_lap.valid = 1;
    gGhostSystem.best_lap.is_best = 1;

    /* Copy frame data */
    data_size = gGhostSystem.recording.frame_count * GHOST_DATA_PER_FRAME;
    for (i = 0; i < data_size; i++) {
        gGhostSystem.best_buffer[i] = gGhostSystem.record_buffer[i];
    }
}

/*
 * Controller Pak I/O
 */

/**
 * ghost_save_to_pak - Save ghost data to Controller Pak
 * @slot: Controller Pak slot (0-3)
 * @return: 0 on success, -1 on error
 */
s32 ghost_save_to_pak(s32 slot) {
    GhostFileHeader header;
    GhostCar *ghost;
    s32 data_size;
    s32 i;

    if (slot < 0 || slot >= GHOST_MAX_SLOTS) {
        return -1;
    }

    /* Get ghost to save (best lap or recording) */
    if (gGhostSystem.best_lap.valid) {
        ghost = &gGhostSystem.best_lap;
    } else if (gGhostSystem.recording.valid) {
        ghost = &gGhostSystem.recording;
    } else {
        return -1;  /* No valid data */
    }

    /* Build header */
    header.magic = GHOST_FILE_MAGIC;
    header.version = GHOST_FILE_VERSION;
    header.frame_count = (u16)ghost->max_frames;
    header.track_id = (u8)ghost->track_id;
    header.car_model = (u8)ghost->car_model;
    header.car_color = (u8)ghost->car_color;
    header.flags = ghost->is_best ? 0x01 : 0x00;
    header.lap_time = (u32)ghost->lap_time;

    data_size = ghost->max_frames * GHOST_DATA_PER_FRAME * sizeof(f32);
    header.checksum = ghost_calc_checksum(ghost->frame_data, data_size);

    /* Write to Controller Pak */
    /* TODO: Actual Controller Pak write via osPfs functions */

    return 0;
}

/**
 * ghost_load_from_pak - Load ghost data from Controller Pak
 * @slot: Controller Pak slot (0-3)
 * @return: 0 on success, -1 on error
 */
s32 ghost_load_from_pak(s32 slot) {
    GhostFileHeader header;
    s32 data_size;
    u32 checksum;

    if (slot < 0 || slot >= GHOST_MAX_SLOTS) {
        return -1;
    }

    /* Read header from Controller Pak */
    /* TODO: Actual Controller Pak read via osPfs functions */

    /* For now, return error (no pak access) */
    return -1;
}

/**
 * ghost_delete_from_pak - Delete ghost from Controller Pak
 * @slot: Controller Pak slot (0-3)
 * @return: 0 on success, -1 on error
 */
s32 ghost_delete_from_pak(s32 slot) {
    if (slot < 0 || slot >= GHOST_MAX_SLOTS) {
        return -1;
    }

    /* TODO: Delete file via osPfs functions */

    return 0;
}

/**
 * ghost_get_pak_slot_info - Get info about saved ghost
 * @slot: Controller Pak slot
 * @track_id: Output track ID (or NULL)
 * @lap_time: Output lap time (or NULL)
 * @return: 1 if slot has data, 0 if empty
 */
s32 ghost_get_pak_slot_info(s32 slot, s32 *track_id, s32 *lap_time) {
    if (slot < 0 || slot >= GHOST_MAX_SLOTS) {
        return 0;
    }

    /* TODO: Read from Controller Pak */

    return 0;
}

/**
 * ghost_count_saved_ghosts - Count ghosts saved on Controller Pak
 * @return: Number of saved ghosts
 */
s32 ghost_count_saved_ghosts(void) {
    s32 count;
    s32 i;

    count = 0;
    for (i = 0; i < GHOST_MAX_SLOTS; i++) {
        if (ghost_get_pak_slot_info(i, NULL, NULL)) {
            count++;
        }
    }

    return count;
}

/*
 * Data compression (for Controller Pak storage)
 */

/**
 * ghost_compress_frame - Compress frame data for storage
 * @in: Full precision frame data
 * @out: Compressed frame data
 *
 * Based on arcade dead reckoning compression scales
 */
void ghost_compress_frame(GhostFrame *in, GhostFrameCompressed *out) {
    /* Compress position: divide by GHOST_POS_SCALE */
    out->pos_x = (s16)(in->pos[0] / GHOST_POS_SCALE);
    out->pos_y = (s16)(in->pos[1] / GHOST_POS_SCALE);
    out->pos_z = (s16)(in->pos[2] / GHOST_POS_SCALE);

    /* Compress rotation: multiply by GHOST_ROT_SCALE (2^15 / pi) */
    out->rot_x = (s16)(in->rot[0] * GHOST_ROT_SCALE);
    out->rot_y = (s16)(in->rot[1] * GHOST_ROT_SCALE);
    out->rot_z = (s16)(in->rot[2] * GHOST_ROT_SCALE);

    /* Compress velocity: divide by GHOST_VEL_SCALE */
    out->vel_x = (s8)(in->vel[0] / GHOST_VEL_SCALE);
    out->vel_y = (s8)(in->vel[1] / GHOST_VEL_SCALE);
    out->vel_z = (s8)(in->vel[2] / GHOST_VEL_SCALE);

    out->flags = 0;
}

/**
 * ghost_decompress_frame - Decompress frame data from storage
 * @in: Compressed frame data
 * @out: Full precision frame data
 */
void ghost_decompress_frame(GhostFrameCompressed *in, GhostFrame *out) {
    /* Decompress position: multiply by GHOST_POS_SCALE */
    out->pos[0] = (f32)in->pos_x * GHOST_POS_SCALE;
    out->pos[1] = (f32)in->pos_y * GHOST_POS_SCALE;
    out->pos[2] = (f32)in->pos_z * GHOST_POS_SCALE;

    /* Decompress rotation: divide by GHOST_ROT_SCALE */
    out->rot[0] = (f32)in->rot_x / GHOST_ROT_SCALE;
    out->rot[1] = (f32)in->rot_y / GHOST_ROT_SCALE;
    out->rot[2] = (f32)in->rot_z / GHOST_ROT_SCALE;

    /* Decompress velocity: multiply by GHOST_VEL_SCALE */
    out->vel[0] = (f32)in->vel_x * GHOST_VEL_SCALE;
    out->vel[1] = (f32)in->vel_y * GHOST_VEL_SCALE;
    out->vel[2] = (f32)in->vel_z * GHOST_VEL_SCALE;
}

/**
 * ghost_calc_checksum - Calculate checksum for data integrity
 * @data: Data buffer
 * @size: Buffer size in bytes
 * @return: 32-bit checksum
 */
u32 ghost_calc_checksum(void *data, s32 size) {
    u8 *bytes;
    u32 checksum;
    s32 i;

    bytes = (u8 *)data;
    checksum = 0;

    for (i = 0; i < size; i++) {
        checksum = (checksum >> 1) + ((checksum & 1) << 31);
        checksum += bytes[i];
    }

    return checksum;
}

/*
 * Queries
 */

/**
 * ghost_is_recording - Check if ghost is being recorded
 * @return: 1 if recording, 0 otherwise
 */
s32 ghost_is_recording(void) {
    return gGhostSystem.recording_active;
}

/**
 * ghost_is_playing - Check if ghost is playing
 * @return: 1 if playing, 0 otherwise
 */
s32 ghost_is_playing(void) {
    return gGhostSystem.playback_active;
}

/**
 * ghost_is_visible - Check if ghost is visible
 * @return: 1 if visible, 0 if hidden
 */
s32 ghost_is_visible(void) {
    return gGhostSystem.ghost_visible;
}

/**
 * ghost_has_valid_data - Check if ghost has valid recorded data
 * @return: 1 if valid data exists, 0 otherwise
 */
s32 ghost_has_valid_data(void) {
    return gGhostSystem.best_lap.valid || gGhostSystem.loaded.valid;
}

/**
 * ghost_get_frame_count - Get number of recorded frames
 * @return: Frame count of active ghost
 */
s32 ghost_get_frame_count(void) {
    if (gGhostSystem.best_lap.valid) {
        return gGhostSystem.best_lap.max_frames;
    }
    if (gGhostSystem.loaded.valid) {
        return gGhostSystem.loaded.max_frames;
    }
    return 0;
}

/**
 * ghost_get_lap_time - Get lap time of active ghost
 * @return: Lap time in centiseconds
 */
s32 ghost_get_lap_time(void) {
    if (gGhostSystem.best_lap.valid) {
        return gGhostSystem.best_lap.lap_time;
    }
    if (gGhostSystem.loaded.valid) {
        return gGhostSystem.loaded.lap_time;
    }
    return 0;
}

#endif /* NON_MATCHING */
