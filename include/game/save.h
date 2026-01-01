/**
 * save.h - Controller Pak save system for Rush 2049 N64
 *
 * Handles game save/load using the N64 Controller Pak (PFS).
 * Stores high scores, unlocked content, options, and statistics.
 *
 * Based on arcade NVRAM save system from rushtherock/game/stats.c
 * and rushtherock/game/hiscore.c.
 *
 * Save data structure:
 * - Game options (sound, controls)
 * - High score tables (per track)
 * - Unlocked cars and tracks
 * - Player statistics (arcade-compatible)
 * - Ghost replay data (separate file)
 */

#ifndef SAVE_H
#define SAVE_H

#include "types.h"

/* ============================================================================
 * Arcade-compatible constants (from rushtherock/game/stats.h)
 * ============================================================================ */

/* Statistics dimensions (matching arcade) */
#define STAT_NUM_HOURS          24          /* Hours in a day */
#define STAT_NUM_DAYS           7           /* Days in a week */
#define STAT_NUM_TRACKS         8           /* Track count */
#define STAT_NUM_CARS           4           /* Car difficulty levels */
#define STAT_NUM_TIMES          32          /* Time histogram bins */
#define STAT_BIN_SECS           15          /* Seconds per time bin */

/* Drone options (arcade) */
#define STAT_NUM_DRONE_OPTIONS  3
#define STAT_DRONE_REG          0
#define STAT_DRONE_NONE         1
#define STAT_DRONE_RACERX       2

/* Force feedback options (arcade) - N64 uses rumble instead */
#define STAT_NUM_FORCE_OPTIONS  3
#define STAT_FORCE_REG          0
#define STAT_FORCE_MORE         1
#define STAT_FORCE_FULL         2

/* Catch-up/rubber-banding options */
#define STAT_NUM_KETCHUP_OPTIONS 2
#define STAT_KETCHUP_ON         0
#define STAT_KETCHUP_OFF        1

/* View and music options */
#define STAT_NUM_TUNES          7
#define STAT_NUM_VIEWS          3

/* High score constants (from arcade hiscore.c) */
#define NLENGTH                 8           /* Name length (arcade: 7 chars + null) */
#define NSCORES                 100         /* Scores per track (arcade has 100) */
#define NNAMES                  10          /* Names shown on screen at once */
#define MIN_SCORE               (1L*60*1000)    /* 1 minute minimum */
#define MAX_SCORE               (10L*60*1000)   /* 10 minute maximum */
#define MAX_HSCORE              (100L*60*1000)  /* 100 minute absolute max */
#define ONE_SEC                 1000            /* Milliseconds per second */

/* High score table NVRAM constants (arcade) */
#define TABLE_SIZE              ((NLENGTH + 8) * NSCORES)

/* Game counters for auto-clear (arcade) */
#define MAX_GAME_CNT            64
#define MAX_HI_CNT              32

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

/* ============================================================================
 * Arcade-compatible game statistics (from rushtherock/game/stats.h)
 * ============================================================================ */

/**
 * Per-track statistics (arcade TRKSTAT equivalent)
 * Tracks gameplay patterns for operator statistics display.
 */
typedef struct ArcadeTrackStat {
    u32     games;                          /* Total games on this track */
    u32     trksel_secs;                    /* Time spent in track select */
    u32     carsel_secs;                    /* Time spent in car select */
    u32     transsel_secs;                  /* Time in transmission select */
    u32     race_secs;                      /* Total racing time */
    u32     hiscore_secs;                   /* Time in high score entry */
    u32     finish;                         /* Race finishes */
    u32     hiscore;                        /* High score entries */
    u32     linked;                         /* Linked/multiplayer games */
    u32     autotrans;                      /* Auto transmission uses */
    u32     reverse;                        /* Reverse direction uses */
    u32     abort_count;                    /* Aborted games */
    u32     crash;                          /* Total crashes */
    u32     car[STAT_NUM_CARS];             /* Car selection histogram */
    u32     music_secs[STAT_NUM_TUNES];     /* Music track play time */
    u32     view_secs[STAT_NUM_VIEWS];      /* Camera view time */
    u32     drone_opt[STAT_NUM_DRONE_OPTIONS];   /* Drone option selections */
    u32     force_opt[STAT_NUM_FORCE_OPTIONS];   /* Force feedback options (rumble) */
    u32     ketchup_opt[STAT_NUM_KETCHUP_OPTIONS]; /* Catch-up option selections */
    u32     tc_hist[STAT_NUM_TIMES][STAT_NUM_CARS]; /* Time x car histogram */
} ArcadeTrackStat;

/**
 * Global statistics (arcade STAT equivalent)
 * Saved to Controller Pak / NVRAM.
 */
typedef struct ArcadeStat {
    u32             attract_secs;           /* Total attract mode time */
    u32             tod_hist[STAT_NUM_HOURS][STAT_NUM_DAYS]; /* Time-of-day histogram */
    ArcadeTrackStat trk[STAT_NUM_TRACKS];   /* Per-track statistics */
} ArcadeStat;

/**
 * Running game statistics (arcade GAMESTAT equivalent)
 * Tracks current game session, saved on game over.
 */
typedef struct ArcadeGameStat {
    u8      game_in_progress;               /* Currently playing */
    u8      pad1;
    s16     next_gstate;                    /* Expected next game state */
    u32     attract_mstime;                 /* Attract mode start time */
    u32     trksel_mstime;                  /* Track select start time */
    u32     carsel_mstime;                  /* Car select start time */
    u32     transel_mstime;                 /* Trans select start time */
    u32     race_mstime;                    /* Race start time */
    u32     hiscore_mstime;                 /* High score entry start */
    u32     trk_num;                        /* Current track number */
    u32     car_num;                        /* Current car number */
    u32     autotrans;                      /* Using auto transmission */
    u32     linked;                         /* In linked/multiplayer mode */
    u32     reverse_cnt;                    /* Reverse uses this game */
    u32     abort_cnt;                      /* Abort count this game */
    u32     death_cnt;                      /* Deaths this game */
    u32     finish;                         /* Finished the race */
    u32     hiscore;                        /* Made high score table */
    u32     music_mstime[STAT_NUM_TUNES];   /* Music track start times */
    u32     view_mstime[STAT_NUM_VIEWS];    /* View mode start times */
} ArcadeGameStat;

/**
 * Arcade-style high score entry (matching arcade HiScore struct)
 */
typedef struct ArcadeHiScore {
    char    name[NLENGTH];                  /* Player name (7 chars + null) */
    u32     score;                          /* Time in milliseconds */
    u8      deaths;                         /* Death count during race */
    u8      mirror;                         /* Mirror mode flag */
    u8      car;                            /* Car ID in low nibble, node in high */
    u8      pad;
} ArcadeHiScore;

/* ============================================================================
 * N64-specific save structures
 * ============================================================================ */

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

/* ============================================================================
 * Arcade-compatible game statistics functions
 * Based on rushtherock/game/stats.c
 * ============================================================================ */

/* Global arcade statistics state */
extern ArcadeGameStat gCurStat;         /* Running stats for current game */
extern ArcadeStat     gArcadeStat;      /* Saved statistics (NVRAM equivalent) */
extern s32            gGameStats;       /* Statistics display active flag */

/* Arcade stats initialization (stats.c:init_game_stats) */
void init_game_stats(void);

/* Per-state statistics tracking (stats.c:game_stats) */
void game_stats(void);
void stat_attract(void);
void stat_trksel(void);
void stat_carsel(void);
void stat_preplay(void);
void stat_playgame(void);
void stat_gameover(void);

/* Statistics update and persistence */
void stat_update(void);
void reset_gamestats(void);
void save_gamestats(void);
void load_gamestats(void);

/* Statistics display (operator menu) */
s32  ChkGameStats(void);
void ShowGameStats(s32 show);
void stat_erase(void);
void stat_disp(void);

/* High score functions (arcade hiscore.c compatible) */
s32  HiScoreRank(u32 score, s16 track);
void LoadHighScores(void);
void ClearHighScores(void);
s32  SaveHighScore(char *name, u32 score, u32 track, u32 deaths,
                   u32 mirror, u32 car, u32 flags);
void EnterHighScore(s16 track, u32 score, char *name,
                    u32 deaths, u32 mirror, u32 car);
void InitGameScores(void);
u8   cvt_time_str(s32 t, u8 *dest, char format);

/* High score name entry (arcade) */
void ShowHiScore(s32 show, s16 track);
void ShowScoreEntry(s32 show);
void GetHighScoreName(void);
void HiScoreForce(void);

#endif /* SAVE_H */
