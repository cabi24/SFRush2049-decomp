/**
 * save.c - Controller Pak save system for Rush 2049 N64
 *
 * Implements game save/load using the N64 Controller Pak (PFS).
 * Based on libultra osPfs* functions.
 */

#include "game/save.h"

/* External libultra PFS functions */
extern s32 osPfsInitPak(void *queue, void *pfs, s32 controller);
extern s32 osPfsFreeBlocks(void *pfs, s32 *bytes_free);
extern s32 osPfsFindFile(void *pfs, u16 company, u32 game, u8 *game_name, u8 *ext_name, s32 *file_no);
extern s32 osPfsAllocateFile(void *pfs, u16 company, u32 game, u8 *game_name, u8 *ext_name, s32 bytes, s32 *file_no);
extern s32 osPfsDeleteFile(void *pfs, u16 company, u32 game, u8 *game_name, u8 *ext_name);
extern s32 osPfsReadWriteFile(void *pfs, s32 file_no, u8 flag, s32 offset, s32 size, u8 *data);
extern s32 osPfsChecker(void *pfs);

/* External message queue for SI operations */
extern void *gSIEventMesgQueue;

/* PFS handle storage */
static u8 sPfsHandles[SAVE_NUM_CONTROLLERS][64];  /* OSPfs is ~60 bytes */

/* Read/write flags for osPfsReadWriteFile */
#define PFS_READ    0
#define PFS_WRITE   1

/* Global save state */
SaveState gSave;

/* Default names for initial high score table */
static const char *sDefaultNames[] = {
    "AAA", "BBB", "CCC", "DDD", "EEE",
    "FFF", "GGG", "HHH", "III", "JJJ"
};

/* Default times (in frames at 60fps) - roughly 2-5 minutes */
static const u32 sDefaultTimes[] = {
    2*60*60, 2*60*60 + 30*60, 3*60*60, 3*60*60 + 30*60, 4*60*60,
    4*60*60 + 30*60, 5*60*60, 5*60*60 + 30*60, 6*60*60, 7*60*60
};

/* -------------------------------------------------------------------------- */
/* Initialization                                                              */
/* -------------------------------------------------------------------------- */

/**
 * Initialize the save system
 */
void save_init(void) {
    s32 i;

    /* Clear state */
    for (i = 0; i < (s32)sizeof(SaveState); i++) {
        ((u8*)&gSave)[i] = 0;
    }

    /* Set defaults */
    save_set_defaults();

    /* Check all controller paks */
    save_check_all_paks();

    /* Try to load from first available pak */
    for (i = 0; i < SAVE_NUM_CONTROLLERS; i++) {
        if (gSave.pak[i].state == PAK_STATE_READY) {
            if (save_file_exists(i)) {
                if (save_read_from(i) == SAVE_OK) {
                    gSave.active_controller = i;
                    break;
                }
            }
        }
    }

    gSave.initialized = 1;
    gSave.modified = 0;
}

/**
 * Reset save data to defaults
 */
void save_reset(void) {
    save_set_defaults();
    gSave.modified = 1;
}

/**
 * Shutdown save system (auto-save if enabled)
 */
void save_shutdown(void) {
    if (gSave.auto_save && gSave.modified) {
        save_write();
    }
    gSave.initialized = 0;
}

/* -------------------------------------------------------------------------- */
/* Controller Pak Management                                                   */
/* -------------------------------------------------------------------------- */

/**
 * Detect and initialize a Controller Pak
 */
s32 save_detect_pak(s32 controller) {
    s32 result;
    s32 free_bytes;
    PakState *pak;

    if (controller < 0 || controller >= SAVE_NUM_CONTROLLERS) {
        return SAVE_ERR_INIT_FAIL;
    }

    pak = &gSave.pak[controller];
    pak->controller = (u8)controller;
    pak->state = PAK_STATE_UNKNOWN;

    /* Initialize the pak */
    result = osPfsInitPak(gSIEventMesgQueue, sPfsHandles[controller], controller);

    if (result != 0) {
        if (result == 1) {  /* PFS_ERR_NOPACK */
            pak->state = PAK_STATE_NONE;
            pak->last_error = SAVE_ERR_NO_PAK;
        } else {
            pak->state = PAK_STATE_BAD;
            pak->last_error = SAVE_ERR_BAD_PAK;
        }
        return pak->last_error;
    }

    /* Check the pak */
    result = osPfsChecker(sPfsHandles[controller]);
    if (result != 0) {
        pak->state = PAK_STATE_BAD;
        pak->last_error = SAVE_ERR_BAD_PAK;
        return SAVE_ERR_BAD_PAK;
    }

    /* Get free space */
    result = osPfsFreeBlocks(sPfsHandles[controller], &free_bytes);
    if (result != 0) {
        pak->state = PAK_STATE_BAD;
        pak->last_error = SAVE_ERR_BAD_PAK;
        return SAVE_ERR_BAD_PAK;
    }

    pak->free_pages = free_bytes / 256;  /* Convert bytes to pages */
    pak->state = PAK_STATE_READY;
    pak->last_error = SAVE_OK;

    /* Check if files exist */
    pak->file_exists = (save_file_exists(controller) == 1) ? 1 : 0;

    return SAVE_OK;
}

/**
 * Check all controller paks
 */
s32 save_check_all_paks(void) {
    s32 i;
    s32 found = 0;

    for (i = 0; i < SAVE_NUM_CONTROLLERS; i++) {
        if (save_detect_pak(i) == SAVE_OK) {
            found++;
        }
    }

    return found;
}

/**
 * Get the active pak controller
 */
s32 save_get_active_pak(void) {
    return gSave.active_controller;
}

/**
 * Set the active pak controller
 */
s32 save_set_active_pak(s32 controller) {
    if (controller < 0 || controller >= SAVE_NUM_CONTROLLERS) {
        return SAVE_ERR_INIT_FAIL;
    }

    if (gSave.pak[controller].state != PAK_STATE_READY) {
        return SAVE_ERR_NO_PAK;
    }

    gSave.active_controller = controller;
    return SAVE_OK;
}

/**
 * Get pak state for a controller
 */
s32 save_get_pak_state(s32 controller) {
    if (controller < 0 || controller >= SAVE_NUM_CONTROLLERS) {
        return PAK_STATE_UNKNOWN;
    }
    return gSave.pak[controller].state;
}

/**
 * Get free pages on a pak
 */
s32 save_get_free_pages(s32 controller) {
    if (controller < 0 || controller >= SAVE_NUM_CONTROLLERS) {
        return 0;
    }
    return gSave.pak[controller].free_pages;
}

/* -------------------------------------------------------------------------- */
/* File Operations                                                             */
/* -------------------------------------------------------------------------- */

/**
 * Check if save file exists on a pak
 */
s32 save_file_exists(s32 controller) {
    s32 file_no;
    s32 result;

    if (controller < 0 || controller >= SAVE_NUM_CONTROLLERS) {
        return 0;
    }

    if (gSave.pak[controller].state != PAK_STATE_READY) {
        return 0;
    }

    result = osPfsFindFile(sPfsHandles[controller],
                           SAVE_COMPANY_CODE, SAVE_GAME_CODE,
                           (u8*)SAVE_GAME_NAME, (u8*)SAVE_EXT_NAME,
                           &file_no);

    return (result == 0) ? 1 : 0;
}

/**
 * Create a new save file
 */
s32 save_create_file(s32 controller) {
    s32 file_no;
    s32 result;
    s32 file_size;

    if (controller < 0 || controller >= SAVE_NUM_CONTROLLERS) {
        return SAVE_ERR_INIT_FAIL;
    }

    if (gSave.pak[controller].state != PAK_STATE_READY) {
        return SAVE_ERR_NO_PAK;
    }

    /* Check if file already exists */
    if (save_file_exists(controller)) {
        return SAVE_OK;  /* Already exists */
    }

    /* Check free space */
    file_size = SAVE_FILE_PAGES * 256;
    if (gSave.pak[controller].free_pages < SAVE_FILE_PAGES) {
        gSave.pak[controller].last_error = SAVE_ERR_NO_SPACE;
        return SAVE_ERR_NO_SPACE;
    }

    /* Allocate the file */
    result = osPfsAllocateFile(sPfsHandles[controller],
                               SAVE_COMPANY_CODE, SAVE_GAME_CODE,
                               (u8*)SAVE_GAME_NAME, (u8*)SAVE_EXT_NAME,
                               file_size, &file_no);

    if (result != 0) {
        gSave.pak[controller].last_error = SAVE_ERR_WRITE_FAIL;
        return SAVE_ERR_WRITE_FAIL;
    }

    gSave.pak[controller].file_exists = 1;
    gSave.pak[controller].free_pages -= SAVE_FILE_PAGES;

    return SAVE_OK;
}

/**
 * Delete save file from a pak
 */
s32 save_delete_file(s32 controller) {
    s32 result;

    if (controller < 0 || controller >= SAVE_NUM_CONTROLLERS) {
        return SAVE_ERR_INIT_FAIL;
    }

    if (gSave.pak[controller].state != PAK_STATE_READY) {
        return SAVE_ERR_NO_PAK;
    }

    result = osPfsDeleteFile(sPfsHandles[controller],
                             SAVE_COMPANY_CODE, SAVE_GAME_CODE,
                             (u8*)SAVE_GAME_NAME, (u8*)SAVE_EXT_NAME);

    if (result != 0) {
        gSave.pak[controller].last_error = SAVE_ERR_WRITE_FAIL;
        return SAVE_ERR_WRITE_FAIL;
    }

    gSave.pak[controller].file_exists = 0;
    gSave.pak[controller].free_pages += SAVE_FILE_PAGES;

    return SAVE_OK;
}

/**
 * Write save data to active pak
 */
s32 save_write(void) {
    return save_write_to(gSave.active_controller);
}

/**
 * Read save data from active pak
 */
s32 save_read(void) {
    return save_read_from(gSave.active_controller);
}

/**
 * Write save data to specific controller pak
 */
s32 save_write_to(s32 controller) {
    s32 file_no;
    s32 result;

    if (controller < 0 || controller >= SAVE_NUM_CONTROLLERS) {
        return SAVE_ERR_INIT_FAIL;
    }

    if (gSave.pak[controller].state != PAK_STATE_READY) {
        return SAVE_ERR_NO_PAK;
    }

    /* Create file if it doesn't exist */
    if (!save_file_exists(controller)) {
        result = save_create_file(controller);
        if (result != SAVE_OK) {
            return result;
        }
    }

    /* Find the file */
    result = osPfsFindFile(sPfsHandles[controller],
                           SAVE_COMPANY_CODE, SAVE_GAME_CODE,
                           (u8*)SAVE_GAME_NAME, (u8*)SAVE_EXT_NAME,
                           &file_no);

    if (result != 0) {
        gSave.pak[controller].last_error = SAVE_ERR_NO_FILE;
        return SAVE_ERR_NO_FILE;
    }

    /* Update header and checksum */
    gSave.data.magic = SAVE_MAGIC;
    gSave.data.version = SAVE_VERSION;
    gSave.data.checksum = save_calc_checksum(&gSave.data.options,
                                              sizeof(SaveData) - 8);  /* Skip magic/version/checksum */

    /* Write the data */
    result = osPfsReadWriteFile(sPfsHandles[controller], file_no, PFS_WRITE,
                                0, sizeof(SaveData), (u8*)&gSave.data);

    if (result != 0) {
        gSave.pak[controller].last_error = SAVE_ERR_WRITE_FAIL;
        return SAVE_ERR_WRITE_FAIL;
    }

    gSave.modified = 0;
    return SAVE_OK;
}

/**
 * Read save data from specific controller pak
 */
s32 save_read_from(s32 controller) {
    s32 file_no;
    s32 result;
    u16 calc_checksum;

    if (controller < 0 || controller >= SAVE_NUM_CONTROLLERS) {
        return SAVE_ERR_INIT_FAIL;
    }

    if (gSave.pak[controller].state != PAK_STATE_READY) {
        return SAVE_ERR_NO_PAK;
    }

    /* Find the file */
    result = osPfsFindFile(sPfsHandles[controller],
                           SAVE_COMPANY_CODE, SAVE_GAME_CODE,
                           (u8*)SAVE_GAME_NAME, (u8*)SAVE_EXT_NAME,
                           &file_no);

    if (result != 0) {
        gSave.pak[controller].last_error = SAVE_ERR_NO_FILE;
        return SAVE_ERR_NO_FILE;
    }

    /* Read the data */
    result = osPfsReadWriteFile(sPfsHandles[controller], file_no, PFS_READ,
                                0, sizeof(SaveData), (u8*)&gSave.data);

    if (result != 0) {
        gSave.pak[controller].last_error = SAVE_ERR_READ_FAIL;
        return SAVE_ERR_READ_FAIL;
    }

    /* Validate magic */
    if (gSave.data.magic != SAVE_MAGIC) {
        save_set_defaults();
        gSave.pak[controller].last_error = SAVE_ERR_CORRUPT;
        return SAVE_ERR_CORRUPT;
    }

    /* Validate checksum */
    calc_checksum = save_calc_checksum(&gSave.data.options,
                                       sizeof(SaveData) - 8);

    if (gSave.data.checksum != calc_checksum) {
        save_set_defaults();
        gSave.pak[controller].last_error = SAVE_ERR_CORRUPT;
        return SAVE_ERR_CORRUPT;
    }

    gSave.modified = 0;
    return SAVE_OK;
}

/* -------------------------------------------------------------------------- */
/* Ghost Operations                                                            */
/* -------------------------------------------------------------------------- */

/**
 * Check if ghost file exists
 */
s32 save_ghost_exists(s32 controller, s32 slot) {
    if (controller < 0 || controller >= SAVE_NUM_CONTROLLERS) {
        return 0;
    }
    if (slot < 0 || slot >= SAVE_MAX_GHOSTS) {
        return 0;
    }

    return gSave.data.ghosts[slot].valid;
}

/**
 * Write ghost replay data
 */
s32 save_write_ghost(s32 slot, u8 *data, s32 size) {
    /* Ghost data would be written to a separate file */
    /* For now, just update the header */
    if (slot < 0 || slot >= SAVE_MAX_GHOSTS) {
        return SAVE_ERR_INIT_FAIL;
    }

    gSave.data.ghosts[slot].valid = 1;
    gSave.data.ghosts[slot].data_size = (u16)size;
    gSave.data.ghosts[slot].checksum = save_calc_checksum(data, size);

    gSave.modified = 1;
    return SAVE_OK;
}

/**
 * Read ghost replay data
 */
s32 save_read_ghost(s32 slot, u8 *data, s32 max_size) {
    /* Ghost data would be read from a separate file */
    if (slot < 0 || slot >= SAVE_MAX_GHOSTS) {
        return SAVE_ERR_INIT_FAIL;
    }

    if (!gSave.data.ghosts[slot].valid) {
        return SAVE_ERR_NO_FILE;
    }

    /* Return the stored size */
    return gSave.data.ghosts[slot].data_size;
}

/**
 * Delete ghost replay
 */
s32 save_delete_ghost(s32 slot) {
    if (slot < 0 || slot >= SAVE_MAX_GHOSTS) {
        return SAVE_ERR_INIT_FAIL;
    }

    gSave.data.ghosts[slot].valid = 0;
    gSave.modified = 1;
    return SAVE_OK;
}

/* -------------------------------------------------------------------------- */
/* Data Access - Options                                                       */
/* -------------------------------------------------------------------------- */

void save_set_sound_mode(u8 mode) {
    if (mode < NUM_SOUND_MODES) {
        gSave.data.options.sound_mode = mode;
        gSave.modified = 1;
    }
}

u8 save_get_sound_mode(void) {
    return gSave.data.options.sound_mode;
}

void save_set_music_volume(u8 volume) {
    if (volume > 100) volume = 100;
    gSave.data.options.music_volume = volume;
    gSave.modified = 1;
}

u8 save_get_music_volume(void) {
    return gSave.data.options.music_volume;
}

void save_set_sfx_volume(u8 volume) {
    if (volume > 100) volume = 100;
    gSave.data.options.sfx_volume = volume;
    gSave.modified = 1;
}

u8 save_get_sfx_volume(void) {
    return gSave.data.options.sfx_volume;
}

void save_set_vibration(u8 enabled) {
    gSave.data.options.vibration = enabled ? 1 : 0;
    gSave.modified = 1;
}

u8 save_get_vibration(void) {
    return gSave.data.options.vibration;
}

void save_set_auto_trans(u8 enabled) {
    gSave.data.options.auto_trans = enabled ? 1 : 0;
    gSave.modified = 1;
}

u8 save_get_auto_trans(void) {
    return gSave.data.options.auto_trans;
}

/* -------------------------------------------------------------------------- */
/* Data Access - Unlocks                                                       */
/* -------------------------------------------------------------------------- */

void save_unlock_car(s32 car_id) {
    if (car_id >= 0 && car_id < 8) {
        gSave.data.unlocks.cars |= (1 << car_id);
        gSave.modified = 1;
    }
}

s32 save_is_car_unlocked(s32 car_id) {
    if (car_id >= 0 && car_id < 8) {
        return (gSave.data.unlocks.cars & (1 << car_id)) ? 1 : 0;
    }
    return 0;
}

void save_unlock_track(s32 track_id) {
    if (track_id >= 0 && track_id < 8) {
        gSave.data.unlocks.tracks |= (1 << track_id);
        gSave.modified = 1;
    }
}

s32 save_is_track_unlocked(s32 track_id) {
    if (track_id >= 0 && track_id < 8) {
        return (gSave.data.unlocks.tracks & (1 << track_id)) ? 1 : 0;
    }
    return 0;
}

void save_unlock_extra(u8 flag) {
    gSave.data.unlocks.extras |= flag;
    gSave.modified = 1;
}

s32 save_is_extra_unlocked(u8 flag) {
    return (gSave.data.unlocks.extras & flag) ? 1 : 0;
}

void save_unlock_all(void) {
    gSave.data.unlocks.cars = UNLOCK_ALL_CARS;
    gSave.data.unlocks.tracks = UNLOCK_ALL_TRACKS;
    gSave.data.unlocks.extras = 0xFF;
    gSave.modified = 1;
}

/* -------------------------------------------------------------------------- */
/* Data Access - High Scores                                                   */
/* -------------------------------------------------------------------------- */

/**
 * Check if a time qualifies for the high score table
 * Returns rank (0-9) or -1 if doesn't qualify
 */
s32 save_check_score(s32 track_id, u32 time) {
    s32 i;
    SaveTrackData *track;

    if (track_id < 0 || track_id >= SAVE_MAX_TRACKS) {
        return -1;
    }

    track = &gSave.data.tracks[track_id];

    /* Find where this time would rank */
    for (i = 0; i < SAVE_MAX_SCORES; i++) {
        if (!track->scores[i].valid || time < track->scores[i].time) {
            return i;
        }
    }

    return -1;  /* Doesn't qualify */
}

/**
 * Add a score to the high score table
 */
void save_add_score(s32 track_id, const char *name, u32 time, u8 car_id, u8 mirror) {
    s32 rank;
    s32 i;
    SaveTrackData *track;
    SaveScore *score;

    rank = save_check_score(track_id, time);
    if (rank < 0) {
        return;  /* Doesn't qualify */
    }

    track = &gSave.data.tracks[track_id];

    /* Shift lower scores down */
    for (i = SAVE_MAX_SCORES - 1; i > rank; i--) {
        track->scores[i] = track->scores[i - 1];
    }

    /* Insert new score */
    score = &track->scores[rank];
    for (i = 0; i < SAVE_MAX_NAME - 1 && name[i]; i++) {
        score->name[i] = name[i];
    }
    score->name[i] = '\0';
    score->time = time;
    score->car_id = car_id;
    score->mirror = mirror;
    score->valid = 1;

    /* Update valid count */
    if (track->num_valid < SAVE_MAX_SCORES) {
        track->num_valid++;
    }

    gSave.modified = 1;
}

SaveScore* save_get_score(s32 track_id, s32 rank) {
    if (track_id < 0 || track_id >= SAVE_MAX_TRACKS) {
        return NULL;
    }
    if (rank < 0 || rank >= SAVE_MAX_SCORES) {
        return NULL;
    }
    return &gSave.data.tracks[track_id].scores[rank];
}

u32 save_get_best_time(s32 track_id) {
    if (track_id < 0 || track_id >= SAVE_MAX_TRACKS) {
        return 0xFFFFFFFF;
    }
    if (!gSave.data.tracks[track_id].scores[0].valid) {
        return 0xFFFFFFFF;
    }
    return gSave.data.tracks[track_id].scores[0].time;
}

void save_get_best_name(s32 track_id, char *name) {
    s32 i;
    SaveScore *score;

    name[0] = '\0';
    if (track_id < 0 || track_id >= SAVE_MAX_TRACKS) {
        return;
    }

    score = &gSave.data.tracks[track_id].scores[0];
    if (!score->valid) {
        return;
    }

    for (i = 0; i < SAVE_MAX_NAME; i++) {
        name[i] = score->name[i];
    }
}

void save_set_best_lap(s32 track_id, u32 time) {
    if (track_id < 0 || track_id >= SAVE_MAX_TRACKS) {
        return;
    }

    if (time < gSave.data.tracks[track_id].best_lap ||
        gSave.data.tracks[track_id].best_lap == 0) {
        gSave.data.tracks[track_id].best_lap = time;
        gSave.modified = 1;
    }
}

u32 save_get_best_lap(s32 track_id) {
    if (track_id < 0 || track_id >= SAVE_MAX_TRACKS) {
        return 0xFFFFFFFF;
    }
    return gSave.data.tracks[track_id].best_lap;
}

void save_clear_track_scores(s32 track_id) {
    s32 i;
    SaveTrackData *track;

    if (track_id < 0 || track_id >= SAVE_MAX_TRACKS) {
        return;
    }

    track = &gSave.data.tracks[track_id];
    for (i = 0; i < SAVE_MAX_SCORES; i++) {
        track->scores[i].valid = 0;
    }
    track->num_valid = 0;
    track->best_lap = 0;
    gSave.modified = 1;
}

void save_clear_all_scores(void) {
    s32 i;
    for (i = 0; i < SAVE_MAX_TRACKS; i++) {
        save_clear_track_scores(i);
    }
}

/* -------------------------------------------------------------------------- */
/* Data Access - Statistics                                                    */
/* -------------------------------------------------------------------------- */

void save_add_play_time(u32 seconds) {
    gSave.data.stats.total_play_time += seconds;
    gSave.modified = 1;
}

void save_add_race(s32 won) {
    gSave.data.stats.total_races++;
    if (won) {
        gSave.data.stats.total_wins++;
    }
    gSave.modified = 1;
}

void save_add_crash(void) {
    gSave.data.stats.total_crashes++;
    gSave.modified = 1;
}

void save_add_distance(u32 amount) {
    gSave.data.stats.total_distance += amount;
    gSave.modified = 1;
}

void save_add_stunt(void) {
    gSave.data.stats.total_stunts++;
    gSave.modified = 1;
}

void save_set_best_stunt_score(u32 score) {
    if (score > gSave.data.stats.best_stunt_score) {
        gSave.data.stats.best_stunt_score = score;
        gSave.modified = 1;
    }
}

void save_add_battle_result(s32 kills, s32 deaths, s32 won) {
    gSave.data.stats.battle_kills += kills;
    gSave.data.stats.battle_deaths += deaths;
    if (won) {
        gSave.data.stats.battle_wins++;
    }
    gSave.modified = 1;
}

SaveStats* save_get_stats(void) {
    return &gSave.data.stats;
}

/* -------------------------------------------------------------------------- */
/* Utility Functions                                                           */
/* -------------------------------------------------------------------------- */

/**
 * Calculate checksum using modified Adler-16
 */
u16 save_calc_checksum(void *data, s32 size) {
    u8 *bytes = (u8*)data;
    u32 a = 1;
    u32 b = 0;
    s32 i;

    for (i = 0; i < size; i++) {
        a = (a + bytes[i]) % 251;
        b = (b + a) % 251;
    }

    return (u16)((b << 8) | a);
}

/**
 * Validate current save data
 */
s32 save_validate(void) {
    u16 calc_checksum;

    if (gSave.data.magic != SAVE_MAGIC) {
        return 0;
    }

    calc_checksum = save_calc_checksum(&gSave.data.options,
                                       sizeof(SaveData) - 8);

    return (gSave.data.checksum == calc_checksum) ? 1 : 0;
}

/**
 * Set save data to defaults
 */
void save_set_defaults(void) {
    s32 i, j;
    SaveTrackData *track;

    /* Clear all data */
    for (i = 0; i < (s32)sizeof(SaveData); i++) {
        ((u8*)&gSave.data)[i] = 0;
    }

    /* Set magic and version */
    gSave.data.magic = SAVE_MAGIC;
    gSave.data.version = SAVE_VERSION;

    /* Default options */
    gSave.data.options.sound_mode = SOUND_STEREO;
    gSave.data.options.music_volume = 80;
    gSave.data.options.sfx_volume = 80;
    gSave.data.options.controller_sens = 50;
    gSave.data.options.vibration = 1;
    gSave.data.options.auto_trans = 1;
    gSave.data.options.camera_mode = 0;
    gSave.data.options.hud_mode = 0;

    /* Default unlocks - first 4 cars and tracks unlocked */
    gSave.data.unlocks.cars = 0x0F;
    gSave.data.unlocks.tracks = 0x0F;
    gSave.data.unlocks.extras = 0;
    gSave.data.unlocks.difficulty = 0;

    /* Initialize high score tables with defaults */
    for (i = 0; i < SAVE_MAX_TRACKS; i++) {
        track = &gSave.data.tracks[i];
        track->track_id = (u8)i;
        track->num_valid = SAVE_MAX_SCORES;
        track->best_lap = 0;

        for (j = 0; j < SAVE_MAX_SCORES; j++) {
            track->scores[j].name[0] = sDefaultNames[j][0];
            track->scores[j].name[1] = sDefaultNames[j][1];
            track->scores[j].name[2] = sDefaultNames[j][2];
            track->scores[j].name[3] = '\0';
            track->scores[j].time = sDefaultTimes[j] + (i * 30 * 60);  /* Scale by track */
            track->scores[j].car_id = (u8)(j % 4);
            track->scores[j].mirror = 0;
            track->scores[j].valid = 1;
        }
    }

    /* Calculate checksum */
    gSave.data.checksum = save_calc_checksum(&gSave.data.options,
                                              sizeof(SaveData) - 8);
}

void save_mark_modified(void) {
    gSave.modified = 1;
}

s32 save_is_modified(void) {
    return gSave.modified;
}

void save_set_auto_save(u8 enabled) {
    gSave.auto_save = enabled;
}

/* -------------------------------------------------------------------------- */
/* Error Handling                                                              */
/* -------------------------------------------------------------------------- */

static const char *sErrorStrings[] = {
    "OK",
    "No Controller Pak",
    "Controller Pak Error",
    "Data Corrupted",
    "Not Enough Space",
    "File Not Found",
    "Write Failed",
    "Read Failed",
    "Init Failed"
};

const char* save_get_error_string(s32 error) {
    if (error < 0 || error > SAVE_ERR_INIT_FAIL) {
        return "Unknown Error";
    }
    return sErrorStrings[error];
}

s32 save_get_last_error(s32 controller) {
    if (controller < 0 || controller >= SAVE_NUM_CONTROLLERS) {
        return SAVE_ERR_INIT_FAIL;
    }
    return gSave.pak[controller].last_error;
}
