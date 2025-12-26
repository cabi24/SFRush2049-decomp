/**
 * save.h - Controller Pak save system for Rush 2049 N64
 *
 * Handles game save/load using the N64 Controller Pak (PFS).
 * Stores high scores, unlocked content, options, and statistics.
 *
 * Save data structure:
 * - Game options (sound, controls)
 * - High score tables (per track)
 * - Unlocked cars and tracks
 * - Player statistics
 * - Ghost replay data (separate file)
 */

#ifndef SAVE_H
#define SAVE_H

#include "types.h"

/* Controller Pak constants */
#define SAVE_COMPANY_CODE       0x3031      /* "01" */
#define SAVE_GAME_CODE          0x4E524855  /* "NRHU" (Rush 2049 US) */
#define SAVE_NUM_CONTROLLERS    4           /* Max controllers to check */

/* File sizes (in Controller Pak pages, 256 bytes each) */
#define SAVE_FILE_PAGES         4           /* Main save: 1024 bytes */
#define GHOST_FILE_PAGES        8           /* Ghost replay: 2048 bytes */

/* Save file names (Controller Pak format, 16 chars max) */
#define SAVE_GAME_NAME          "RUSH2049"
#define SAVE_EXT_NAME           "SAVE"
#define GHOST_EXT_NAME          "GHOST"

/* Save data version for compatibility */
#define SAVE_VERSION            1
#define SAVE_MAGIC              0x52534856  /* "RSHV" */

/* Error codes */
#define SAVE_OK                 0
#define SAVE_ERR_NO_PAK         1           /* No Controller Pak inserted */
#define SAVE_ERR_BAD_PAK        2           /* Controller Pak error */
#define SAVE_ERR_CORRUPT        3           /* Data corrupted */
#define SAVE_ERR_NO_SPACE       4           /* Not enough space */
#define SAVE_ERR_NO_FILE        5           /* File not found */
#define SAVE_ERR_WRITE_FAIL     6           /* Write failed */
#define SAVE_ERR_READ_FAIL      7           /* Read failed */
#define SAVE_ERR_INIT_FAIL      8           /* PFS init failed */

/* Controller Pak states */
#define PAK_STATE_UNKNOWN       0
#define PAK_STATE_NONE          1           /* No pak inserted */
#define PAK_STATE_BAD           2           /* Bad or incompatible pak */
#define PAK_STATE_READY         3           /* Pak ready for use */

/* Maximum values */
#define SAVE_MAX_TRACKS         8           /* Tracks with save data */
#define SAVE_MAX_CARS           8           /* Cars that can be unlocked */
#define SAVE_MAX_SCORES         10          /* Scores per track */
#define SAVE_MAX_NAME           4           /* 3 chars + null */
#define SAVE_MAX_GHOSTS         4           /* Ghost replays per save */

/* Sound mode options */
#define SOUND_MONO              0
#define SOUND_STEREO            1
#define SOUND_SURROUND          2
#define NUM_SOUND_MODES         3

/* Unlock flags */
#define UNLOCK_CAR_1            (1 << 0)
#define UNLOCK_CAR_2            (1 << 1)
#define UNLOCK_CAR_3            (1 << 2)
#define UNLOCK_CAR_4            (1 << 3)
#define UNLOCK_CAR_5            (1 << 4)
#define UNLOCK_CAR_6            (1 << 5)
#define UNLOCK_CAR_7            (1 << 6)
#define UNLOCK_CAR_8            (1 << 7)
#define UNLOCK_ALL_CARS         0xFF

#define UNLOCK_TRACK_1          (1 << 0)
#define UNLOCK_TRACK_2          (1 << 1)
#define UNLOCK_TRACK_3          (1 << 2)
#define UNLOCK_TRACK_4          (1 << 3)
#define UNLOCK_TRACK_5          (1 << 4)
#define UNLOCK_TRACK_6          (1 << 5)
#define UNLOCK_TRACK_7          (1 << 6)
#define UNLOCK_TRACK_8          (1 << 7)
#define UNLOCK_ALL_TRACKS       0xFF

#define UNLOCK_MIRROR_MODE      (1 << 0)
#define UNLOCK_STUNT_ARENAS     (1 << 1)
#define UNLOCK_BATTLE_ARENAS    (1 << 2)
#define UNLOCK_ALL_MUSIC        (1 << 3)

/* High score entry */
typedef struct SaveScore {
    char        name[SAVE_MAX_NAME];    /* Player initials */
    u32         time;                   /* Time in frames */
    u8          car_id;                 /* Car used */
    u8          mirror;                 /* Mirror mode flag */
    u8          valid;                  /* Entry valid */
    u8          pad;
} SaveScore;

/* Per-track scores */
typedef struct SaveTrackData {
    SaveScore   scores[SAVE_MAX_SCORES];
    u32         best_lap;               /* Best single lap time */
    u8          track_id;
    u8          num_valid;
    u8          pad[2];
} SaveTrackData;

/* Game options */
typedef struct SaveOptions {
    u8          sound_mode;             /* SOUND_* */
    u8          music_volume;           /* 0-100 */
    u8          sfx_volume;             /* 0-100 */
    u8          controller_sens;        /* Controller sensitivity */
    u8          vibration;              /* Rumble Pak enabled */
    u8          auto_trans;             /* Auto transmission */
    u8          camera_mode;            /* Default camera */
    u8          hud_mode;               /* HUD display mode */
} SaveOptions;

/* Player statistics */
typedef struct SaveStats {
    u32         total_play_time;        /* Total time in game (seconds) */
    u32         total_races;            /* Races completed */
    u32         total_wins;             /* First place finishes */
    u32         total_crashes;          /* Crash count */
    u32         total_distance;         /* Distance traveled */
    u32         total_stunts;           /* Stunts performed */
    u32         best_stunt_score;       /* Best stunt session score */
    u32         battle_kills;           /* Battle mode kills */
    u32         battle_deaths;          /* Battle mode deaths */
    u32         battle_wins;            /* Battle mode wins */
} SaveStats;

/* Unlock state */
typedef struct SaveUnlocks {
    u8          cars;                   /* UNLOCK_CAR_* flags */
    u8          tracks;                 /* UNLOCK_TRACK_* flags */
    u8          extras;                 /* UNLOCK_* extra flags */
    u8          difficulty;             /* Highest difficulty beaten */
} SaveUnlocks;

/* Ghost header (stored separately) */
typedef struct SaveGhostHeader {
    u8          valid;
    u8          track_id;
    u8          car_id;
    u8          mirror;
    u32         time;                   /* Total time */
    u16         data_size;              /* Compressed size */
    u16         checksum;
} SaveGhostHeader;

/* Main save data structure */
typedef struct SaveData {
    /* Header */
    u32             magic;              /* SAVE_MAGIC */
    u16             version;            /* SAVE_VERSION */
    u16             checksum;           /* Data checksum */

    /* Game settings */
    SaveOptions     options;

    /* Unlock state */
    SaveUnlocks     unlocks;

    /* Statistics */
    SaveStats       stats;

    /* High scores per track */
    SaveTrackData   tracks[SAVE_MAX_TRACKS];

    /* Ghost info (data stored separately) */
    SaveGhostHeader ghosts[SAVE_MAX_GHOSTS];

} SaveData;

/* Controller Pak file state */
typedef struct PakState {
    u8          state;                  /* PAK_STATE_* */
    u8          controller;             /* Controller index */
    u8          file_exists;            /* Save file found */
    u8          ghost_exists;           /* Ghost file found */
    s32         free_pages;             /* Free pages available */
    s32         last_error;             /* Last error code */
} PakState;

/* Global save state */
typedef struct SaveState {
    SaveData    data;                   /* Current save data */
    PakState    pak[SAVE_NUM_CONTROLLERS];  /* Per-controller pak state */
    s32         active_controller;      /* Controller with active pak */
    u8          initialized;            /* System initialized */
    u8          modified;               /* Data has unsaved changes */
    u8          auto_save;              /* Auto-save enabled */
    u8          pad;
} SaveState;

/* Global save state */
extern SaveState gSave;

/* Initialization */
void save_init(void);
void save_reset(void);
void save_shutdown(void);

/* Controller Pak management */
s32 save_detect_pak(s32 controller);
s32 save_check_all_paks(void);
s32 save_get_active_pak(void);
s32 save_set_active_pak(s32 controller);
s32 save_get_pak_state(s32 controller);
s32 save_get_free_pages(s32 controller);

/* File operations */
s32 save_file_exists(s32 controller);
s32 save_create_file(s32 controller);
s32 save_delete_file(s32 controller);
s32 save_write(void);
s32 save_read(void);
s32 save_write_to(s32 controller);
s32 save_read_from(s32 controller);

/* Ghost operations */
s32 save_ghost_exists(s32 controller, s32 slot);
s32 save_write_ghost(s32 slot, u8 *data, s32 size);
s32 save_read_ghost(s32 slot, u8 *data, s32 max_size);
s32 save_delete_ghost(s32 slot);

/* Data access - Options */
void save_set_sound_mode(u8 mode);
u8 save_get_sound_mode(void);
void save_set_music_volume(u8 volume);
u8 save_get_music_volume(void);
void save_set_sfx_volume(u8 volume);
u8 save_get_sfx_volume(void);
void save_set_vibration(u8 enabled);
u8 save_get_vibration(void);
void save_set_auto_trans(u8 enabled);
u8 save_get_auto_trans(void);

/* Data access - Unlocks */
void save_unlock_car(s32 car_id);
s32 save_is_car_unlocked(s32 car_id);
void save_unlock_track(s32 track_id);
s32 save_is_track_unlocked(s32 track_id);
void save_unlock_extra(u8 flag);
s32 save_is_extra_unlocked(u8 flag);
void save_unlock_all(void);

/* Data access - High scores */
s32 save_check_score(s32 track_id, u32 time);
void save_add_score(s32 track_id, const char *name, u32 time, u8 car_id, u8 mirror);
SaveScore* save_get_score(s32 track_id, s32 rank);
u32 save_get_best_time(s32 track_id);
void save_get_best_name(s32 track_id, char *name);
void save_set_best_lap(s32 track_id, u32 time);
u32 save_get_best_lap(s32 track_id);
void save_clear_track_scores(s32 track_id);
void save_clear_all_scores(void);

/* Data access - Statistics */
void save_add_play_time(u32 seconds);
void save_add_race(s32 won);
void save_add_crash(void);
void save_add_distance(u32 amount);
void save_add_stunt(void);
void save_set_best_stunt_score(u32 score);
void save_add_battle_result(s32 kills, s32 deaths, s32 won);
SaveStats* save_get_stats(void);

/* Utility */
u16 save_calc_checksum(void *data, s32 size);
s32 save_validate(void);
void save_set_defaults(void);
void save_mark_modified(void);
s32 save_is_modified(void);
void save_set_auto_save(u8 enabled);

/* Error handling */
const char* save_get_error_string(s32 error);
s32 save_get_last_error(s32 controller);

#endif /* SAVE_H */
