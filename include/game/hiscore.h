/**
 * hiscore.h - High Score system for Rush 2049 N64
 *
 * Handles high score tables, name entry, and persistence.
 * Based on arcade hiscore.c patterns.
 */

#ifndef HISCORE_H
#define HISCORE_H

#include "types.h"

/* High score limits */
#define MAX_TRACKS          8       /* Maximum tracks with scores */
#define MAX_SCORES          10      /* Scores per track */
#define MAX_NAME_LENGTH     3       /* 3-letter initials */
#define MAX_ENTRY_SLOTS     13      /* Selectable positions in name entry */

/* Time limits (in frames at 60fps) */
#define MIN_SCORE_TIME      (1 * 60 * 60)       /* 1 minute minimum */
#define MAX_SCORE_TIME      (10 * 60 * 60)      /* 10 minute maximum */
#define MAX_DISPLAY_TIME    (100 * 60 * 60)     /* 99:59.99 display max */

/* High score state */
#define HISCORE_STATE_IDLE          0
#define HISCORE_STATE_DISPLAY       1   /* Showing high scores */
#define HISCORE_STATE_ENTRY         2   /* Player entering name */
#define HISCORE_STATE_SCROLL        3   /* Scrolling scores */
#define HISCORE_STATE_TRANSITION    4   /* Transitioning between tracks */

/* Entry state */
#define ENTRY_STATE_SELECT_CHAR     0   /* Selecting character */
#define ENTRY_STATE_CONFIRM         1   /* Confirming entry */
#define ENTRY_STATE_DONE            2   /* Entry complete */

/* Valid characters for name entry */
#define HISCORE_CHAR_A          0
#define HISCORE_CHAR_Z          25
#define HISCORE_CHAR_SPACE      26
#define HISCORE_CHAR_DEL        27      /* Delete/backspace */
#define HISCORE_CHAR_END        28      /* End entry */
#define HISCORE_CHAR_COUNT      29      /* Total selectable */

/* Display positions */
#define HISCORE_X_LEFT          45      /* Left column X */
#define HISCORE_X_RIGHT         200     /* Right column X */
#define HISCORE_Y_TOP           40      /* Top Y position */
#define HISCORE_Y_SPACING       18      /* Vertical spacing */

/* Letter width in entry screen */
#define HISCORE_LETTER_WIDTH    24

/* Single high score entry */
typedef struct HiScoreEntry {
    char        name[MAX_NAME_LENGTH + 1];  /* Player initials + null */
    u32         time;                       /* Time in frames */
    u8          car_type;                   /* Car used */
    u8          mirror;                     /* Mirror mode flag */
    u8          valid;                      /* Entry is valid */
    u8          pad;
} HiScoreEntry;

/* Per-track high score table */
typedef struct TrackScores {
    HiScoreEntry    scores[MAX_SCORES];
    u8              track_id;
    u8              num_valid;              /* Number of valid scores */
    u8              pad[2];
} TrackScores;

/* Name entry state */
typedef struct NameEntry {
    char        name[MAX_NAME_LENGTH + 1];
    s32         char_index;                 /* Current character position (0-2) */
    s32         letter_index;               /* Current letter selection */
    s32         rank;                       /* Achieved rank (0-9) */
    u32         score_time;                 /* Player's time */
    u8          track_id;                   /* Track scored on */
    u8          car_type;                   /* Car used */
    u8          state;                      /* Entry state */
    u8          done;                       /* Entry complete */
    u32         timeout;                    /* Input timeout */
    u32         blink_timer;                /* Cursor blink timer */
} NameEntry;

/* High score display state */
typedef struct HiScoreDisplay {
    s32         current_track;              /* Track being displayed */
    s32         scroll_offset;              /* Scroll position */
    f32         scroll_speed;               /* Scroll speed */
    u32         display_timer;              /* Time on current track */
    u8          auto_scroll;                /* Auto-scrolling enabled */
    u8          show_continue;              /* Show continue prompt */
    u8          pad[2];
} HiScoreDisplay;

/* High score system state */
typedef struct HiScoreState {
    TrackScores     tables[MAX_TRACKS];     /* All track scores */
    NameEntry       entry;                  /* Current name entry */
    HiScoreDisplay  display;                /* Display state */

    u8              state;                  /* HISCORE_STATE_* */
    u8              num_tracks;             /* Tracks with scores */
    u8              entering_name;          /* Currently entering name */
    u8              pad;

    /* Continue mode */
    u8              continue_available;
    u8              continue_used;
    u8              pad2[2];
    u32             continue_timer;

} HiScoreState;

/* Global state */
extern HiScoreState gHiScore;

/* Default names for initial table */
extern const char *gDefaultNames[];

/* Initialization */
void hiscore_init(void);
void hiscore_reset(void);
void hiscore_init_defaults(void);

/* Update and draw */
void hiscore_update(void);
void hiscore_draw(void);

/* Score management */
s32 hiscore_check_score(u8 track_id, u32 time);
void hiscore_add_score(u8 track_id, const char *name, u32 time, u8 car_type);
void hiscore_insert_score(u8 track_id, s32 rank, const char *name, u32 time, u8 car_type);
void hiscore_clear_track(u8 track_id);
void hiscore_clear_all(void);

/* Name entry */
void hiscore_start_entry(u8 track_id, u32 time, u8 car_type, s32 rank);
void hiscore_update_entry(void);
void hiscore_draw_entry(void);
void hiscore_entry_next_char(void);
void hiscore_entry_prev_char(void);
void hiscore_entry_select_char(void);
void hiscore_entry_delete(void);
void hiscore_entry_confirm(void);
s32 hiscore_is_entering(void);

/* Display */
void hiscore_start_display(s32 track);
void hiscore_stop_display(void);
void hiscore_next_track(void);
void hiscore_prev_track(void);
void hiscore_draw_table(s32 track_id);
void hiscore_draw_scrolling(void);

/* Continue mode */
void hiscore_enable_continue(void);
void hiscore_use_continue(void);
s32 hiscore_can_continue(void);

/* Queries */
u32 hiscore_get_best_time(u8 track_id);
const char* hiscore_get_best_name(u8 track_id);
s32 hiscore_get_rank(u8 track_id, u32 time);
s32 hiscore_get_valid_count(u8 track_id);

/* Persistence (Controller Pak) */
s32 hiscore_save(void);
s32 hiscore_load(void);
s32 hiscore_save_track(u8 track_id);
s32 hiscore_load_track(u8 track_id);

/* Utility */
void hiscore_time_to_string(u32 frames, char *buffer);
char hiscore_index_to_char(s32 index);
s32 hiscore_char_to_index(char c);
s32 hiscore_validate_name(const char *name);
void hiscore_filter_name(char *name);

#endif /* HISCORE_H */
