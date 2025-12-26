/**
 * minimap.c - Minimap and radar system for Rush 2049 N64
 *
 * Implements track overview display and position markers.
 */

#include "game/minimap.h"

/* External functions */
extern f32 sinf(f32 x);
extern f32 cosf(f32 x);
extern f32 atan2f(f32 y, f32 x);

/* External track functions */
extern s32 track_get_num_checkpoints(void);
extern void *track_get_checkpoint(s32 index);

/* External car data */
extern f32 gCarPositions[8][3];
extern f32 gCarRotations[8];

/* Global minimap state */
MinimapState gMinimap;

/* Default colors */
static const u8 sPlayerColors[8][3] = {
    { 0, 255, 0 },      /* Player 1 - Green */
    { 255, 0, 0 },      /* Player 2 - Red */
    { 0, 0, 255 },      /* Player 3 - Blue */
    { 255, 255, 0 },    /* Player 4 - Yellow */
    { 255, 128, 0 },    /* CPU 1 - Orange */
    { 128, 0, 255 },    /* CPU 2 - Purple */
    { 0, 255, 255 },    /* CPU 3 - Cyan */
    { 255, 0, 255 }     /* CPU 4 - Magenta */
};

/* Marker colors by type */
static const u8 sMarkerColors[NUM_MARKER_TYPES][4] = {
    { 0, 255, 0, 255 },     /* Player - Green */
    { 255, 0, 0, 255 },     /* Opponent - Red */
    { 255, 255, 0, 255 },   /* Checkpoint - Yellow */
    { 255, 128, 0, 255 },   /* Weapon - Orange */
    { 0, 255, 0, 255 },     /* Health - Green */
    { 0, 128, 255, 255 },   /* Boost - Blue */
    { 255, 0, 0, 255 },     /* Hazard - Red */
    { 128, 255, 128, 255 }, /* Shortcut - Light green */
    { 255, 255, 255, 255 }, /* Finish - White */
    { 255, 255, 0, 255 }    /* Arrow - Yellow */
};

/* Position offsets based on corner */
static const s16 sPositionOffsets[NUM_MINIMAP_POSITIONS][2] = {
    { 8, 8 },           /* Top-left */
    { -8, 8 },          /* Top-right (negative = from right edge) */
    { 8, -8 },          /* Bottom-left */
    { -8, -8 }          /* Bottom-right */
};

/* Screen dimensions */
#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   240

/* -------------------------------------------------------------------------- */
/* Initialization                                                              */
/* -------------------------------------------------------------------------- */

/**
 * Initialize minimap system
 */
void minimap_init(void) {
    s32 i;

    /* Clear state */
    for (i = 0; i < (s32)sizeof(MinimapState); i++) {
        ((u8*)&gMinimap)[i] = 0;
    }

    /* Default settings */
    gMinimap.mode = MINIMAP_MODE_CORNER;
    gMinimap.position = MINIMAP_POS_TOP_RIGHT;
    gMinimap.visible = 1;
    gMinimap.size = MINIMAP_SIZE_MEDIUM;
    gMinimap.zoom = 1.0f;
    gMinimap.opacity = 200;

    /* Display options */
    gMinimap.show_track = 1;
    gMinimap.show_checkpoints = 1;
    gMinimap.show_pickups = 1;
    gMinimap.show_opponents = 1;
    gMinimap.show_shortcuts = 0;
    gMinimap.border = 1;

    /* Default colors */
    gMinimap.bg_color[0] = 0;
    gMinimap.bg_color[1] = 0;
    gMinimap.bg_color[2] = 0;
    gMinimap.bg_color[3] = 128;

    gMinimap.track_color[0] = 255;
    gMinimap.track_color[1] = 255;
    gMinimap.track_color[2] = 255;
    gMinimap.track_color[3] = 255;

    gMinimap.border_color[0] = 255;
    gMinimap.border_color[1] = 255;
    gMinimap.border_color[2] = 255;
    gMinimap.border_color[3] = 255;

    /* Calculate screen position */
    minimap_set_position(gMinimap.position);
}

/**
 * Reset minimap
 */
void minimap_reset(void) {
    minimap_clear_markers();
    gMinimap.rotation = 0.0f;
    gMinimap.center_x = 0.0f;
    gMinimap.center_z = 0.0f;
}

/* -------------------------------------------------------------------------- */
/* Track Map Loading                                                           */
/* -------------------------------------------------------------------------- */

/**
 * Load track map data
 */
s32 minimap_load_track(s32 track_id) {
    /* Would load track-specific minimap data from ROM */
    minimap_generate_from_checkpoints();
    return 1;
}

/**
 * Unload track map
 */
void minimap_unload_track(void) {
    gMinimap.track_map = NULL;
}

/**
 * Generate map from checkpoint positions
 */
void minimap_generate_from_checkpoints(void) {
    /* Would build map outline from checkpoint data */
    /* For now, use placeholder bounds */

    if (gMinimap.track_map == NULL) {
        static TrackMap sDefaultMap;
        gMinimap.track_map = &sDefaultMap;
    }

    gMinimap.track_map->min_x = -1000.0f;
    gMinimap.track_map->max_x = 1000.0f;
    gMinimap.track_map->min_z = -1000.0f;
    gMinimap.track_map->max_z = 1000.0f;
}

/* -------------------------------------------------------------------------- */
/* Display Control                                                             */
/* -------------------------------------------------------------------------- */

void minimap_show(void) {
    gMinimap.visible = 1;
}

void minimap_hide(void) {
    gMinimap.visible = 0;
}

void minimap_toggle(void) {
    gMinimap.visible = !gMinimap.visible;
}

s32 minimap_is_visible(void) {
    return gMinimap.visible;
}

void minimap_set_mode(s32 mode) {
    if (mode >= 0 && mode < NUM_MINIMAP_MODES) {
        gMinimap.mode = (u8)mode;

        /* Adjust size based on mode */
        switch (mode) {
            case MINIMAP_MODE_OFF:
                gMinimap.visible = 0;
                break;
            case MINIMAP_MODE_CORNER:
                gMinimap.size = MINIMAP_SIZE_MEDIUM;
                gMinimap.visible = 1;
                break;
            case MINIMAP_MODE_FULL:
                gMinimap.size = MINIMAP_SIZE_FULL;
                gMinimap.visible = 1;
                break;
            case MINIMAP_MODE_ROTATING:
                gMinimap.size = MINIMAP_SIZE_MEDIUM;
                gMinimap.visible = 1;
                break;
        }

        minimap_set_position(gMinimap.position);
    }
}

s32 minimap_get_mode(void) {
    return gMinimap.mode;
}

void minimap_cycle_mode(void) {
    s32 new_mode = gMinimap.mode + 1;
    if (new_mode >= NUM_MINIMAP_MODES) {
        new_mode = MINIMAP_MODE_OFF;
    }
    minimap_set_mode(new_mode);
}

/* -------------------------------------------------------------------------- */
/* Position and Size                                                           */
/* -------------------------------------------------------------------------- */

void minimap_set_position(s32 position) {
    s16 offset_x, offset_y;

    if (position < 0 || position >= NUM_MINIMAP_POSITIONS) {
        return;
    }

    gMinimap.position = (u8)position;
    gMinimap.width = gMinimap.size;
    gMinimap.height = gMinimap.size;

    offset_x = sPositionOffsets[position][0];
    offset_y = sPositionOffsets[position][1];

    /* Calculate screen position */
    if (offset_x >= 0) {
        gMinimap.screen_x = offset_x;
    } else {
        gMinimap.screen_x = SCREEN_WIDTH + offset_x - gMinimap.width;
    }

    if (offset_y >= 0) {
        gMinimap.screen_y = offset_y;
    } else {
        gMinimap.screen_y = SCREEN_HEIGHT + offset_y - gMinimap.height;
    }
}

void minimap_set_screen_pos(s16 x, s16 y) {
    gMinimap.screen_x = x;
    gMinimap.screen_y = y;
}

void minimap_set_size(s32 size) {
    if (size >= MINIMAP_SIZE_SMALL && size <= MINIMAP_SIZE_FULL) {
        gMinimap.size = (u8)size;
        gMinimap.width = size;
        gMinimap.height = size;
        minimap_set_position(gMinimap.position);
    }
}

void minimap_set_zoom(f32 zoom) {
    if (zoom >= 0.25f && zoom <= 4.0f) {
        gMinimap.zoom = zoom;
    }
}

/* -------------------------------------------------------------------------- */
/* View Control                                                                */
/* -------------------------------------------------------------------------- */

void minimap_set_center(f32 x, f32 z) {
    gMinimap.center_x = x;
    gMinimap.center_z = z;
}

void minimap_set_rotation(f32 rotation) {
    gMinimap.rotation = rotation;
    while (gMinimap.rotation >= 360.0f) gMinimap.rotation -= 360.0f;
    while (gMinimap.rotation < 0.0f) gMinimap.rotation += 360.0f;
}

void minimap_follow_player(s32 player) {
    if (player >= 0 && player < 8) {
        gMinimap.center_x = gCarPositions[player][0];
        gMinimap.center_z = gCarPositions[player][2];

        if (gMinimap.mode == MINIMAP_MODE_ROTATING) {
            gMinimap.rotation = gCarRotations[player];
        }
    }
}

/* -------------------------------------------------------------------------- */
/* Appearance                                                                  */
/* -------------------------------------------------------------------------- */

void minimap_set_opacity(u8 opacity) {
    gMinimap.opacity = opacity;
}

void minimap_set_bg_color(u8 r, u8 g, u8 b, u8 a) {
    gMinimap.bg_color[0] = r;
    gMinimap.bg_color[1] = g;
    gMinimap.bg_color[2] = b;
    gMinimap.bg_color[3] = a;
}

void minimap_set_track_color(u8 r, u8 g, u8 b, u8 a) {
    gMinimap.track_color[0] = r;
    gMinimap.track_color[1] = g;
    gMinimap.track_color[2] = b;
    gMinimap.track_color[3] = a;
}

void minimap_show_element(s32 element, s32 visible) {
    switch (element) {
        case 0: gMinimap.show_track = visible ? 1 : 0; break;
        case 1: gMinimap.show_checkpoints = visible ? 1 : 0; break;
        case 2: gMinimap.show_pickups = visible ? 1 : 0; break;
        case 3: gMinimap.show_opponents = visible ? 1 : 0; break;
        case 4: gMinimap.show_shortcuts = visible ? 1 : 0; break;
    }
}

/* -------------------------------------------------------------------------- */
/* Markers                                                                     */
/* -------------------------------------------------------------------------- */

void minimap_clear_markers(void) {
    s32 i;

    for (i = 0; i < MAX_MARKERS; i++) {
        gMinimap.markers[i].active = 0;
    }
    gMinimap.marker_count = 0;
}

s32 minimap_add_marker(s32 type, f32 x, f32 z, f32 rotation) {
    s32 i;
    MapMarker *marker;

    for (i = 0; i < MAX_MARKERS; i++) {
        if (!gMinimap.markers[i].active) {
            marker = &gMinimap.markers[i];
            marker->active = 1;
            marker->type = (u8)type;
            marker->world_x = x;
            marker->world_z = z;
            marker->rotation = rotation;
            marker->scale = 1.0f;
            marker->blinking = 0;

            /* Set default color for type */
            marker->color[0] = sMarkerColors[type][0];
            marker->color[1] = sMarkerColors[type][1];
            marker->color[2] = sMarkerColors[type][2];
            marker->color[3] = sMarkerColors[type][3];

            gMinimap.marker_count++;
            return i;
        }
    }

    return -1;  /* No free slots */
}

void minimap_remove_marker(s32 index) {
    if (index >= 0 && index < MAX_MARKERS) {
        if (gMinimap.markers[index].active) {
            gMinimap.markers[index].active = 0;
            gMinimap.marker_count--;
        }
    }
}

void minimap_update_marker(s32 index, f32 x, f32 z, f32 rotation) {
    if (index >= 0 && index < MAX_MARKERS) {
        gMinimap.markers[index].world_x = x;
        gMinimap.markers[index].world_z = z;
        gMinimap.markers[index].rotation = rotation;
    }
}

void minimap_set_marker_color(s32 index, u8 r, u8 g, u8 b, u8 a) {
    if (index >= 0 && index < MAX_MARKERS) {
        gMinimap.markers[index].color[0] = r;
        gMinimap.markers[index].color[1] = g;
        gMinimap.markers[index].color[2] = b;
        gMinimap.markers[index].color[3] = a;
    }
}

void minimap_set_marker_blinking(s32 index, u8 blinking) {
    if (index >= 0 && index < MAX_MARKERS) {
        gMinimap.markers[index].blinking = blinking;
    }
}

/* -------------------------------------------------------------------------- */
/* Player Markers                                                              */
/* -------------------------------------------------------------------------- */

void minimap_update_player(s32 player, f32 x, f32 z, f32 rotation) {
    /* Player markers are stored in first 8 marker slots */
    if (player >= 0 && player < 8) {
        MapMarker *marker = &gMinimap.markers[player];
        marker->active = 1;
        marker->type = (player == 0) ? MARKER_PLAYER : MARKER_OPPONENT;
        marker->world_x = x;
        marker->world_z = z;
        marker->rotation = rotation;
        marker->player = (u8)player;
    }
}

void minimap_set_player_color(s32 player, u8 r, u8 g, u8 b) {
    if (player >= 0 && player < 8) {
        gMinimap.markers[player].color[0] = r;
        gMinimap.markers[player].color[1] = g;
        gMinimap.markers[player].color[2] = b;
        gMinimap.markers[player].color[3] = 255;
    }
}

/* -------------------------------------------------------------------------- */
/* Coordinate Conversion                                                       */
/* -------------------------------------------------------------------------- */

/**
 * Convert world coordinates to minimap screen coordinates
 */
void minimap_world_to_screen(f32 world_x, f32 world_z, s16 *screen_x, s16 *screen_y) {
    f32 map_x, map_z;
    f32 scale_x, scale_z;
    f32 norm_x, norm_z;
    f32 rot_x, rot_z;
    f32 sin_r, cos_r;
    TrackMap *map;

    map = gMinimap.track_map;
    if (map == NULL) {
        *screen_x = gMinimap.screen_x + gMinimap.width / 2;
        *screen_y = gMinimap.screen_y + gMinimap.height / 2;
        return;
    }

    /* Normalize to 0-1 range */
    scale_x = map->max_x - map->min_x;
    scale_z = map->max_z - map->min_z;

    if (scale_x <= 0.0f) scale_x = 1.0f;
    if (scale_z <= 0.0f) scale_z = 1.0f;

    norm_x = (world_x - map->min_x) / scale_x;
    norm_z = (world_z - map->min_z) / scale_z;

    /* Apply zoom */
    if (gMinimap.mode == MINIMAP_MODE_ROTATING) {
        /* Center on player position */
        f32 center_norm_x = (gMinimap.center_x - map->min_x) / scale_x;
        f32 center_norm_z = (gMinimap.center_z - map->min_z) / scale_z;

        norm_x = (norm_x - center_norm_x) * gMinimap.zoom + 0.5f;
        norm_z = (norm_z - center_norm_z) * gMinimap.zoom + 0.5f;

        /* Apply rotation */
        sin_r = sinf(gMinimap.rotation * 3.14159f / 180.0f);
        cos_r = cosf(gMinimap.rotation * 3.14159f / 180.0f);

        rot_x = (norm_x - 0.5f) * cos_r - (norm_z - 0.5f) * sin_r + 0.5f;
        rot_z = (norm_x - 0.5f) * sin_r + (norm_z - 0.5f) * cos_r + 0.5f;

        norm_x = rot_x;
        norm_z = rot_z;
    }

    /* Convert to screen coordinates */
    map_x = norm_x * gMinimap.width;
    map_z = norm_z * gMinimap.height;

    *screen_x = gMinimap.screen_x + (s16)map_x;
    *screen_y = gMinimap.screen_y + (s16)map_z;
}

/**
 * Convert screen coordinates to world coordinates
 */
void minimap_screen_to_world(s16 screen_x, s16 screen_y, f32 *world_x, f32 *world_z) {
    f32 norm_x, norm_z;
    f32 scale_x, scale_z;
    TrackMap *map;

    map = gMinimap.track_map;
    if (map == NULL) {
        *world_x = 0.0f;
        *world_z = 0.0f;
        return;
    }

    /* Normalize screen position */
    norm_x = (f32)(screen_x - gMinimap.screen_x) / (f32)gMinimap.width;
    norm_z = (f32)(screen_y - gMinimap.screen_y) / (f32)gMinimap.height;

    /* Convert to world coordinates */
    scale_x = map->max_x - map->min_x;
    scale_z = map->max_z - map->min_z;

    *world_x = map->min_x + norm_x * scale_x;
    *world_z = map->min_z + norm_z * scale_z;
}

/* -------------------------------------------------------------------------- */
/* Update and Render                                                           */
/* -------------------------------------------------------------------------- */

/**
 * Update minimap state
 */
void minimap_update(void) {
    s32 i;

    if (!gMinimap.visible || gMinimap.mode == MINIMAP_MODE_OFF) {
        return;
    }

    /* Update blink timer */
    gMinimap.blink_timer += 1.0f / 60.0f;
    if (gMinimap.blink_timer >= 0.5f) {
        gMinimap.blink_timer -= 0.5f;
        gMinimap.blink_state = !gMinimap.blink_state;
    }

    /* Update player positions */
    for (i = 0; i < 8; i++) {
        minimap_update_player(i,
            gCarPositions[i][0],
            gCarPositions[i][2],
            gCarRotations[i]);
    }

    /* Follow player 0 in rotating mode */
    if (gMinimap.mode == MINIMAP_MODE_ROTATING) {
        minimap_follow_player(0);
    }
}

/**
 * Draw minimap
 */
void minimap_draw(void) {
    if (!gMinimap.visible || gMinimap.mode == MINIMAP_MODE_OFF) {
        return;
    }

    /* Draw background */
    /* gfx_draw_rect_rgba(...) */

    /* Draw border */
    if (gMinimap.border) {
        minimap_draw_border();
    }

    /* Draw track outline */
    if (gMinimap.show_track) {
        minimap_draw_track();
    }

    /* Draw markers */
    minimap_draw_markers();
}

void minimap_draw_track(void) {
    /* Would draw track outline using line primitives */
}

void minimap_draw_markers(void) {
    s32 i;
    MapMarker *marker;
    s16 sx, sy;

    for (i = 0; i < MAX_MARKERS; i++) {
        marker = &gMinimap.markers[i];
        if (!marker->active) {
            continue;
        }

        /* Skip blinking markers when blink state is off */
        if (marker->blinking && !gMinimap.blink_state) {
            continue;
        }

        /* Filter by type settings */
        if (marker->type == MARKER_OPPONENT && !gMinimap.show_opponents) {
            continue;
        }
        if ((marker->type >= MARKER_PICKUP_WEAPON &&
             marker->type <= MARKER_PICKUP_BOOST) && !gMinimap.show_pickups) {
            continue;
        }
        if (marker->type == MARKER_SHORTCUT && !gMinimap.show_shortcuts) {
            continue;
        }

        /* Convert to screen position */
        minimap_world_to_screen(marker->world_x, marker->world_z, &sx, &sy);

        /* Check if in bounds */
        if (sx >= gMinimap.screen_x && sx < gMinimap.screen_x + gMinimap.width &&
            sy >= gMinimap.screen_y && sy < gMinimap.screen_y + gMinimap.height) {
            /* Would draw marker sprite/shape at sx, sy */
        }
    }
}

void minimap_draw_player_marker(s32 player) {
    MapMarker *marker;
    s16 sx, sy;

    if (player < 0 || player >= 8) {
        return;
    }

    marker = &gMinimap.markers[player];
    if (!marker->active) {
        return;
    }

    minimap_world_to_screen(marker->world_x, marker->world_z, &sx, &sy);
    /* Would draw player arrow at sx, sy with rotation */
}

void minimap_draw_border(void) {
    /* Would draw rectangle border around minimap */
}

/* -------------------------------------------------------------------------- */
/* Split-screen Support                                                        */
/* -------------------------------------------------------------------------- */

void minimap_set_viewport(s32 player, s16 x, s16 y, s16 w, s16 h) {
    /* Adjust minimap position for split-screen viewport */
    gMinimap.screen_x = x + 8;
    gMinimap.screen_y = y + 8;

    /* Scale minimap for smaller viewports */
    if (w < SCREEN_WIDTH || h < SCREEN_HEIGHT) {
        gMinimap.size = MINIMAP_SIZE_SMALL;
        gMinimap.width = MINIMAP_SIZE_SMALL;
        gMinimap.height = MINIMAP_SIZE_SMALL;
    }
}

void minimap_draw_for_player(s32 player) {
    /* Set up minimap for specific player's viewport */
    minimap_follow_player(player);
    minimap_draw();
}

/* -------------------------------------------------------------------------- */
/* Direction Arrow                                                             */
/* -------------------------------------------------------------------------- */

void minimap_draw_direction_arrow(f32 target_x, f32 target_z) {
    f32 player_x, player_z;
    f32 dx, dz;
    f32 angle;

    /* Get player position */
    player_x = gCarPositions[0][0];
    player_z = gCarPositions[0][2];

    /* Calculate direction to target */
    dx = target_x - player_x;
    dz = target_z - player_z;

    angle = atan2f(dx, dz) * 180.0f / 3.14159f;

    /* Would draw arrow pointing in direction */
}

void minimap_point_to_checkpoint(s32 checkpoint) {
    /* Would get checkpoint position and draw arrow */
}

void minimap_point_to_position(f32 x, f32 z) {
    minimap_draw_direction_arrow(x, z);
}
