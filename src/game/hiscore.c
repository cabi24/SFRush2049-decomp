/**
 * hiscore.c - High Score system for Rush 2049 N64
 *
 * Handles high score tables, name entry, and persistence.
 * Based on arcade hiscore.c patterns.
 */

#include "game/hiscore.h"
#include "game/menu.h"
#include "types.h"

/* Global state */
HiScoreState gHiScore;

/* Default names for initial table (developer names) */
const char *gDefaultNames[] = {
    "AAA", "BBB", "CCC", "DDD", "EEE",
    "FFF", "GGG", "HHH", "III", "JJJ"
};

/* Default times (in frames, at 60fps) */
static const u32 sDefaultTimes[] = {
    60 * 60 * 2,        /* 2:00.00 */
    60 * 60 * 2 + 1800, /* 2:30.00 */
    60 * 60 * 3,        /* 3:00.00 */
    60 * 60 * 3 + 1800, /* 3:30.00 */
    60 * 60 * 4,        /* 4:00.00 */
    60 * 60 * 4 + 1800, /* 4:30.00 */
    60 * 60 * 5,        /* 5:00.00 */
    60 * 60 * 5 + 1800, /* 5:30.00 */
    60 * 60 * 6,        /* 6:00.00 */
    60 * 60 * 6 + 1800  /* 6:30.00 */
};

/* Character set for name entry */
static const char sCharSet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ _<";

/* ========== Initialization ========== */

void hiscore_init(void) {
    s32 i;

    /* Clear state */
    for (i = 0; i < (s32)sizeof(gHiScore); i++) {
        ((u8 *)&gHiScore)[i] = 0;
    }

    gHiScore.state = HISCORE_STATE_IDLE;
    gHiScore.num_tracks = MAX_TRACKS;

    /* Initialize default scores */
    hiscore_init_defaults();
}

void hiscore_reset(void) {
    gHiScore.state = HISCORE_STATE_IDLE;
    gHiScore.entering_name = 0;

    /* Reset entry state */
    gHiScore.entry.state = ENTRY_STATE_SELECT_CHAR;
    gHiScore.entry.char_index = 0;
    gHiScore.entry.letter_index = 0;
    gHiScore.entry.done = 0;

    /* Reset display state */
    gHiScore.display.current_track = 0;
    gHiScore.display.scroll_offset = 0;
    gHiScore.display.display_timer = 0;
}

void hiscore_init_defaults(void) {
    s32 track, rank;

    for (track = 0; track < MAX_TRACKS; track++) {
        gHiScore.tables[track].track_id = (u8)track;
        gHiScore.tables[track].num_valid = MAX_SCORES;

        for (rank = 0; rank < MAX_SCORES; rank++) {
            HiScoreEntry *entry = &gHiScore.tables[track].scores[rank];

            /* Copy default name */
            entry->name[0] = gDefaultNames[rank][0];
            entry->name[1] = gDefaultNames[rank][1];
            entry->name[2] = gDefaultNames[rank][2];
            entry->name[3] = '\0';

            /* Default time (increases with rank, varies by track) */
            entry->time = sDefaultTimes[rank] + (u32)(track * 60 * 30);
            entry->car_type = (u8)(rank % 8);
            entry->mirror = 0;
            entry->valid = 1;
        }
    }
}

/* ========== Update and Draw ========== */

void hiscore_update(void) {
    switch (gHiScore.state) {
        case HISCORE_STATE_IDLE:
            /* Nothing to do */
            break;

        case HISCORE_STATE_DISPLAY:
            /* Update display timer */
            gHiScore.display.display_timer++;

            /* Auto-scroll to next track after delay */
            if (gHiScore.display.auto_scroll) {
                if (gHiScore.display.display_timer > 60 * 5) {  /* 5 seconds */
                    hiscore_next_track();
                    gHiScore.display.display_timer = 0;
                }
            }
            break;

        case HISCORE_STATE_ENTRY:
            hiscore_update_entry();
            break;

        case HISCORE_STATE_SCROLL:
            /* Update scroll position */
            gHiScore.display.scroll_offset += (s32)gHiScore.display.scroll_speed;
            break;

        case HISCORE_STATE_TRANSITION:
            /* Transition animation */
            gHiScore.display.display_timer++;
            if (gHiScore.display.display_timer > 30) {
                gHiScore.state = HISCORE_STATE_DISPLAY;
                gHiScore.display.display_timer = 0;
            }
            break;
    }

    /* Update continue timer if active */
    if (gHiScore.continue_available && gHiScore.continue_timer > 0) {
        gHiScore.continue_timer--;
    }
}

void hiscore_draw(void) {
    switch (gHiScore.state) {
        case HISCORE_STATE_IDLE:
            /* Nothing to draw */
            break;

        case HISCORE_STATE_DISPLAY:
        case HISCORE_STATE_TRANSITION:
            hiscore_draw_table(gHiScore.display.current_track);

            /* Draw continue prompt if available */
            if (gHiScore.display.show_continue && gHiScore.continue_available) {
                /* Flash "CONTINUE?" text */
                if ((gHiScore.display.display_timer / 15) & 1) {
                    ui_draw_text(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 30,
                                "CONTINUE?", COLOR_YELLOW, ALIGN_CENTER);
                }
            }
            break;

        case HISCORE_STATE_ENTRY:
            hiscore_draw_entry();
            break;

        case HISCORE_STATE_SCROLL:
            hiscore_draw_scrolling();
            break;
    }
}

/* ========== Score Management ========== */

s32 hiscore_check_score(u8 track_id, u32 time) {
    s32 rank;
    TrackScores *table;

    if (track_id >= MAX_TRACKS) {
        return -1;
    }

    /* Check time bounds */
    if (time < MIN_SCORE_TIME || time > MAX_SCORE_TIME) {
        return -1;
    }

    table = &gHiScore.tables[track_id];

    /* Find rank (lower time = better) */
    for (rank = 0; rank < MAX_SCORES; rank++) {
        if (!table->scores[rank].valid || time < table->scores[rank].time) {
            return rank;
        }
    }

    return -1;  /* Didn't make the list */
}

void hiscore_add_score(u8 track_id, const char *name, u32 time, u8 car_type) {
    s32 rank = hiscore_check_score(track_id, time);

    if (rank >= 0) {
        hiscore_insert_score(track_id, rank, name, time, car_type);
    }
}

void hiscore_insert_score(u8 track_id, s32 rank, const char *name, u32 time, u8 car_type) {
    TrackScores *table;
    HiScoreEntry *entry;
    s32 i;

    if (track_id >= MAX_TRACKS || rank < 0 || rank >= MAX_SCORES) {
        return;
    }

    table = &gHiScore.tables[track_id];

    /* Shift scores down */
    for (i = MAX_SCORES - 1; i > rank; i--) {
        table->scores[i] = table->scores[i - 1];
    }

    /* Insert new score */
    entry = &table->scores[rank];
    entry->name[0] = name[0];
    entry->name[1] = name[1];
    entry->name[2] = name[2];
    entry->name[3] = '\0';
    entry->time = time;
    entry->car_type = car_type;
    entry->mirror = 0;
    entry->valid = 1;

    /* Update valid count */
    if (table->num_valid < MAX_SCORES) {
        table->num_valid++;
    }
}

void hiscore_clear_track(u8 track_id) {
    s32 rank;
    TrackScores *table;

    if (track_id >= MAX_TRACKS) {
        return;
    }

    table = &gHiScore.tables[track_id];

    for (rank = 0; rank < MAX_SCORES; rank++) {
        table->scores[rank].valid = 0;
    }
    table->num_valid = 0;

    /* Reinitialize with defaults */
    hiscore_init_defaults();
}

void hiscore_clear_all(void) {
    s32 track;

    for (track = 0; track < MAX_TRACKS; track++) {
        hiscore_clear_track((u8)track);
    }
}

/* ========== Name Entry ========== */

void hiscore_start_entry(u8 track_id, u32 time, u8 car_type, s32 rank) {
    NameEntry *entry = &gHiScore.entry;

    gHiScore.state = HISCORE_STATE_ENTRY;
    gHiScore.entering_name = 1;

    /* Initialize entry */
    entry->name[0] = 'A';
    entry->name[1] = 'A';
    entry->name[2] = 'A';
    entry->name[3] = '\0';
    entry->char_index = 0;
    entry->letter_index = 0;
    entry->rank = rank;
    entry->score_time = time;
    entry->track_id = track_id;
    entry->car_type = car_type;
    entry->state = ENTRY_STATE_SELECT_CHAR;
    entry->done = 0;
    entry->timeout = 60 * 30;   /* 30 seconds timeout */
    entry->blink_timer = 0;
}

void hiscore_update_entry(void) {
    NameEntry *entry = &gHiScore.entry;

    /* Update blink timer */
    entry->blink_timer++;
    if (entry->blink_timer >= 30) {
        entry->blink_timer = 0;
    }

    /* Check timeout */
    if (entry->timeout > 0) {
        entry->timeout--;
        if (entry->timeout == 0) {
            /* Auto-confirm on timeout */
            hiscore_entry_confirm();
        }
    }

    /* Entry is handled via external input calls */
}

void hiscore_draw_entry(void) {
    NameEntry *entry = &gHiScore.entry;
    char time_str[16];
    char rank_str[16];
    s32 x, y;
    s32 i;
    u32 color;

    /* Draw title */
    ui_draw_text(SCREEN_WIDTH / 2, 30, "ENTER YOUR NAME", COLOR_WHITE, ALIGN_CENTER);

    /* Draw rank achieved */
    rank_str[0] = '#';
    rank_str[1] = (char)('1' + entry->rank);
    rank_str[2] = '\0';
    ui_draw_text(SCREEN_WIDTH / 2, 50, rank_str, COLOR_YELLOW, ALIGN_CENTER);

    /* Draw time achieved */
    hiscore_time_to_string(entry->score_time, time_str);
    ui_draw_text(SCREEN_WIDTH / 2, 70, time_str, COLOR_GREEN, ALIGN_CENTER);

    /* Draw current name */
    x = SCREEN_WIDTH / 2 - (MAX_NAME_LENGTH * HISCORE_LETTER_WIDTH) / 2;
    y = SCREEN_HEIGHT / 2;

    for (i = 0; i < MAX_NAME_LENGTH; i++) {
        /* Highlight current character */
        if (i == entry->char_index) {
            /* Blink the current character */
            if ((entry->blink_timer / 8) & 1) {
                color = COLOR_YELLOW;
            } else {
                color = COLOR_WHITE;
            }
        } else {
            color = COLOR_WHITE;
        }

        /* Draw the character */
        {
            char char_str[2];
            char_str[0] = entry->name[i];
            char_str[1] = '\0';
            ui_draw_text_scaled((s16)(x + i * HISCORE_LETTER_WIDTH), (s16)y,
                               char_str, color, ALIGN_CENTER, 2.0f);
        }
    }

    /* Draw character selection alphabet */
    y = SCREEN_HEIGHT / 2 + 50;
    x = SCREEN_WIDTH / 2 - (13 * 20) / 2;  /* 26 chars in 2 rows */

    for (i = 0; i < HISCORE_CHAR_COUNT; i++) {
        s32 row = i / 13;
        s32 col = i % 13;
        s32 px = x + col * 20;
        s32 py = y + row * 25;

        if (i == entry->letter_index) {
            color = COLOR_YELLOW;
        } else {
            color = COLOR_GRAY;
        }

        {
            char char_str[2];
            char_str[0] = sCharSet[i];
            char_str[1] = '\0';
            ui_draw_text((s16)px, (s16)py, char_str, color, ALIGN_CENTER);
        }
    }

    /* Draw instructions */
    ui_draw_text(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 30,
                "A=SELECT  B=DELETE  START=DONE", COLOR_GRAY, ALIGN_CENTER);
}

void hiscore_entry_next_char(void) {
    NameEntry *entry = &gHiScore.entry;

    entry->letter_index++;
    if (entry->letter_index >= HISCORE_CHAR_COUNT) {
        entry->letter_index = 0;
    }

    /* Reset timeout on input */
    entry->timeout = 60 * 30;
}

void hiscore_entry_prev_char(void) {
    NameEntry *entry = &gHiScore.entry;

    entry->letter_index--;
    if (entry->letter_index < 0) {
        entry->letter_index = HISCORE_CHAR_COUNT - 1;
    }

    entry->timeout = 60 * 30;
}

void hiscore_entry_select_char(void) {
    NameEntry *entry = &gHiScore.entry;
    char selected;

    /* Handle special characters */
    if (entry->letter_index == HISCORE_CHAR_DEL) {
        hiscore_entry_delete();
        return;
    }

    if (entry->letter_index == HISCORE_CHAR_END) {
        hiscore_entry_confirm();
        return;
    }

    /* Get selected character */
    selected = hiscore_index_to_char(entry->letter_index);

    /* Set character in name */
    entry->name[entry->char_index] = selected;

    /* Move to next character position */
    entry->char_index++;
    if (entry->char_index >= MAX_NAME_LENGTH) {
        /* Auto-confirm when all characters entered */
        hiscore_entry_confirm();
    }

    entry->timeout = 60 * 30;
}

void hiscore_entry_delete(void) {
    NameEntry *entry = &gHiScore.entry;

    if (entry->char_index > 0) {
        entry->char_index--;
        entry->name[entry->char_index] = 'A';
    }

    entry->timeout = 60 * 30;
}

void hiscore_entry_confirm(void) {
    NameEntry *entry = &gHiScore.entry;

    /* Validate name */
    hiscore_filter_name(entry->name);

    /* Insert the score */
    hiscore_insert_score(entry->track_id, entry->rank,
                        entry->name, entry->score_time, entry->car_type);

    /* Mark entry complete */
    entry->state = ENTRY_STATE_DONE;
    entry->done = 1;
    gHiScore.entering_name = 0;

    /* Transition to display */
    gHiScore.state = HISCORE_STATE_DISPLAY;
    gHiScore.display.current_track = entry->track_id;
}

s32 hiscore_is_entering(void) {
    return gHiScore.entering_name;
}

/* ========== Display ========== */

void hiscore_start_display(s32 track) {
    if (track < 0 || track >= MAX_TRACKS) {
        track = 0;
    }

    gHiScore.state = HISCORE_STATE_DISPLAY;
    gHiScore.display.current_track = track;
    gHiScore.display.scroll_offset = 0;
    gHiScore.display.display_timer = 0;
    gHiScore.display.auto_scroll = 1;
}

void hiscore_stop_display(void) {
    gHiScore.state = HISCORE_STATE_IDLE;
}

void hiscore_next_track(void) {
    gHiScore.display.current_track++;
    if (gHiScore.display.current_track >= (s32)gHiScore.num_tracks) {
        gHiScore.display.current_track = 0;
    }
    gHiScore.display.display_timer = 0;
}

void hiscore_prev_track(void) {
    gHiScore.display.current_track--;
    if (gHiScore.display.current_track < 0) {
        gHiScore.display.current_track = gHiScore.num_tracks - 1;
    }
    gHiScore.display.display_timer = 0;
}

void hiscore_draw_table(s32 track_id) {
    TrackScores *table;
    char time_str[16];
    char track_name[32];
    s32 x, y, rank;

    if (track_id < 0 || track_id >= MAX_TRACKS) {
        return;
    }

    table = &gHiScore.tables[track_id];

    /* Draw track name header */
    track_name[0] = 'T';
    track_name[1] = 'R';
    track_name[2] = 'A';
    track_name[3] = 'C';
    track_name[4] = 'K';
    track_name[5] = ' ';
    track_name[6] = (char)('1' + track_id);
    track_name[7] = '\0';

    ui_draw_text(SCREEN_WIDTH / 2, 20, track_name, COLOR_YELLOW, ALIGN_CENTER);
    ui_draw_text(SCREEN_WIDTH / 2, 35, "HIGH SCORES", COLOR_WHITE, ALIGN_CENTER);

    /* Draw column headers */
    y = HISCORE_Y_TOP;
    ui_draw_text(HISCORE_X_LEFT, (s16)y, "RANK", COLOR_GRAY, ALIGN_LEFT);
    ui_draw_text(HISCORE_X_LEFT + 50, (s16)y, "NAME", COLOR_GRAY, ALIGN_LEFT);
    ui_draw_text(HISCORE_X_RIGHT, (s16)y, "TIME", COLOR_GRAY, ALIGN_LEFT);

    y += HISCORE_Y_SPACING;

    /* Draw scores */
    for (rank = 0; rank < MAX_SCORES; rank++) {
        HiScoreEntry *entry = &table->scores[rank];
        char rank_str[4];
        u32 color;

        if (!entry->valid) {
            continue;
        }

        /* Highlight recent entries */
        if (gHiScore.entering_name && rank == gHiScore.entry.rank &&
            track_id == gHiScore.entry.track_id) {
            color = COLOR_YELLOW;
        } else {
            color = COLOR_WHITE;
        }

        /* Rank number */
        rank_str[0] = (char)('0' + (rank + 1) / 10);
        rank_str[1] = (char)('0' + (rank + 1) % 10);
        rank_str[2] = '.';
        rank_str[3] = '\0';
        if (rank_str[0] == '0') {
            rank_str[0] = ' ';
        }
        ui_draw_text(HISCORE_X_LEFT, (s16)y, rank_str, color, ALIGN_LEFT);

        /* Name */
        ui_draw_text(HISCORE_X_LEFT + 50, (s16)y, entry->name, color, ALIGN_LEFT);

        /* Time */
        hiscore_time_to_string(entry->time, time_str);
        ui_draw_text(HISCORE_X_RIGHT, (s16)y, time_str, color, ALIGN_LEFT);

        y += HISCORE_Y_SPACING;
    }
}

void hiscore_draw_scrolling(void) {
    /* Draw scrolling scores across all tracks */
    /* This is used in attract mode */
    hiscore_draw_table(gHiScore.display.current_track);
}

/* ========== Continue Mode ========== */

void hiscore_enable_continue(void) {
    gHiScore.continue_available = 1;
    gHiScore.continue_timer = 60 * 10;  /* 10 seconds */
    gHiScore.display.show_continue = 1;
}

void hiscore_use_continue(void) {
    if (gHiScore.continue_available) {
        gHiScore.continue_available = 0;
        gHiScore.continue_used = 1;
        gHiScore.display.show_continue = 0;
    }
}

s32 hiscore_can_continue(void) {
    return gHiScore.continue_available && gHiScore.continue_timer > 0;
}

/* ========== Queries ========== */

u32 hiscore_get_best_time(u8 track_id) {
    if (track_id >= MAX_TRACKS) {
        return MAX_DISPLAY_TIME;
    }

    if (gHiScore.tables[track_id].scores[0].valid) {
        return gHiScore.tables[track_id].scores[0].time;
    }

    return MAX_DISPLAY_TIME;
}

const char* hiscore_get_best_name(u8 track_id) {
    if (track_id >= MAX_TRACKS) {
        return "---";
    }

    if (gHiScore.tables[track_id].scores[0].valid) {
        return gHiScore.tables[track_id].scores[0].name;
    }

    return "---";
}

s32 hiscore_get_rank(u8 track_id, u32 time) {
    return hiscore_check_score(track_id, time);
}

s32 hiscore_get_valid_count(u8 track_id) {
    if (track_id >= MAX_TRACKS) {
        return 0;
    }
    return gHiScore.tables[track_id].num_valid;
}

/* ========== Persistence ========== */

s32 hiscore_save(void) {
    /* TODO: Implement Controller Pak save */
    /* For now, return success */
    return 1;
}

s32 hiscore_load(void) {
    /* TODO: Implement Controller Pak load */
    /* For now, use defaults */
    hiscore_init_defaults();
    return 1;
}

s32 hiscore_save_track(u8 track_id) {
    /* TODO: Implement single track save */
    (void)track_id;
    return 1;
}

s32 hiscore_load_track(u8 track_id) {
    /* TODO: Implement single track load */
    (void)track_id;
    return 1;
}

/* ========== Utility ========== */

void hiscore_time_to_string(u32 frames, char *buffer) {
    u32 total_hundredths;
    u32 minutes, seconds, hundredths;

    /* Convert frames to time */
    total_hundredths = (frames * 100) / 60;
    minutes = total_hundredths / 6000;
    seconds = (total_hundredths / 100) % 60;
    hundredths = total_hundredths % 100;

    /* Clamp to display max */
    if (minutes > 99) {
        minutes = 99;
        seconds = 59;
        hundredths = 99;
    }

    /* Format: M:SS.HH or MM:SS.HH */
    if (minutes >= 10) {
        buffer[0] = (char)('0' + minutes / 10);
        buffer[1] = (char)('0' + minutes % 10);
        buffer[2] = ':';
        buffer[3] = (char)('0' + seconds / 10);
        buffer[4] = (char)('0' + seconds % 10);
        buffer[5] = '.';
        buffer[6] = (char)('0' + hundredths / 10);
        buffer[7] = (char)('0' + hundredths % 10);
        buffer[8] = '\0';
    } else {
        buffer[0] = (char)('0' + minutes);
        buffer[1] = ':';
        buffer[2] = (char)('0' + seconds / 10);
        buffer[3] = (char)('0' + seconds % 10);
        buffer[4] = '.';
        buffer[5] = (char)('0' + hundredths / 10);
        buffer[6] = (char)('0' + hundredths % 10);
        buffer[7] = '\0';
    }
}

char hiscore_index_to_char(s32 index) {
    if (index >= 0 && index < (s32)sizeof(sCharSet) - 1) {
        return sCharSet[index];
    }
    return ' ';
}

s32 hiscore_char_to_index(char c) {
    s32 i;

    for (i = 0; i < (s32)sizeof(sCharSet) - 1; i++) {
        if (sCharSet[i] == c) {
            return i;
        }
    }

    return HISCORE_CHAR_SPACE;  /* Default to space */
}

s32 hiscore_validate_name(const char *name) {
    s32 i;

    if (name == NULL) {
        return 0;
    }

    for (i = 0; i < MAX_NAME_LENGTH; i++) {
        char c = name[i];

        /* Check valid characters */
        if (c >= 'A' && c <= 'Z') {
            continue;
        }
        if (c == ' ') {
            continue;
        }
        if (c == '\0' && i > 0) {
            break;  /* Null terminator OK after at least one char */
        }

        return 0;  /* Invalid character */
    }

    return 1;
}

void hiscore_filter_name(char *name) {
    s32 i;

    if (name == NULL) {
        return;
    }

    for (i = 0; i < MAX_NAME_LENGTH; i++) {
        char c = name[i];

        /* Convert lowercase to uppercase */
        if (c >= 'a' && c <= 'z') {
            name[i] = c - 'a' + 'A';
            continue;
        }

        /* Allow uppercase letters and space */
        if (c >= 'A' && c <= 'Z') {
            continue;
        }
        if (c == ' ') {
            continue;
        }

        /* Replace invalid with space */
        name[i] = ' ';
    }

    /* Ensure null terminator */
    name[MAX_NAME_LENGTH] = '\0';
}

/* ========================================================================
 * ARCADE-COMPATIBLE FUNCTIONS (hiscore.c)
 * These match the arcade function signatures exactly
 * ======================================================================== */

/* String functions */
extern s32 strlen(const char *s);
extern char *strcpy(char *dst, const char *src);
extern char *strncpy(char *dst, const char *src, s32 n);
extern s32 strncmp(const char *s1, const char *s2, s32 n);
extern void *memmove(void *dst, const void *src, s32 n);

/* Arcade global state */
s32 gEnteringName;                      /* True if entering name in High Score */
s32 continue_flag;                      /* Continue mode active */
s32 continue_carry;                     /* Continue carried over */

/* Score tables */
HiScore gScoreTable[NTRACKS][NSCORES];
s32 InThisGame[NTRACKS][NSCORES];

/* Private state */
static s16 gThisRank;                   /* Current score to highlight */
static s16 gThisTrack;
static char gCurName[NLENGTH + 5];      /* Current name being entered */
static s16 gNameIndex;                  /* Place to put next char */
static s16 gCurLetter;
static u32 gThisScore;
static s16 gLetterTries;
static s16 nextBlit;
static s16 char_on_left, char_on_right, last_section;
static s16 delta_x, old_delta_x;
static s32 left_side_active, right_side_active, holdoff_force;

/* Valid characters for name entry (arcade) */
static const char font_list[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!?.{}";

/**
 * revcpy - Reverse copy a string (for mirror mode)
 * Based on arcade: hiscore.c:revcpy()
 */
void revcpy(char *dest, char *src) {
    s32 i, len;

    len = strlen(src);
    for (i = 0; i < len; i++) {
        dest[len - 1 - i] = src[i];
    }
    dest[len] = '\0';
}

/**
 * cvt_time_str - Convert time to string format
 * Based on arcade: hiscore.c:cvt_time_str()
 *
 * @param t     Time in milliseconds (0.001 second units)
 * @param dest  Output buffer (at least 9 bytes)
 * @param format Format character:
 *               's' - MM:SS (seconds only)
 *               'h' - M:SS.hh (default, with hundredths)
 *               'c' - SSS (countdown timer)
 *               'f' - MM:SS.hh (full format)
 * @return Number of digits set
 */
u8 cvt_time_str(s32 t, u8 *dest, char format) {
    s32 sec, min;
    u8 ret;

    ret = 0;

    if (t < 0) {
        t = 0;
    }

    switch (format) {
        case 'c':
            sec = t;
            if (sec > 999) {
                sec = 999;
            }
            dest[0] = (u8)((sec / 100) % 10) + '0';
            dest[1] = (u8)((sec / 10) % 10) + '0';
            dest[2] = (u8)(sec % 10) + '0';
            ret = 3;
            break;

        case 'f':
            if (t >= (s32)MAX_HSCORE) {
                strcpy((char *)dest, "99:59.99");
                return 8;
            }
            dest[5] = '.';
            dest[6] = (u8)((t / 100) % 10) + '0';
            dest[7] = (u8)((t / 10) % 10) + '0';
            dest[8] = 0;
            ret = 8;
            sec = t / 1000;
            min = sec / 60;
            sec %= 60;
            dest[0] = (u8)(min / 10) % 10 + '0';
            dest[1] = (u8)(min % 10) + '0';
            dest[2] = ':';
            dest[3] = (u8)(sec / 10) + '0';
            dest[4] = (u8)(sec % 10) + '0';
            break;

        case 'h':
            if (t >= (s32)MAX_SCORE) {
                strcpy((char *)dest, "9:99.99");
                return 7;
            }
            dest[4] = '.';
            dest[5] = (u8)((t / 100) % 10) + '0';
            dest[6] = (u8)((t / 10) % 10) + '0';
            dest[7] = 0;
            ret = 7;
            /* Fall through to do seconds */

        case 's':
            sec = t / 1000;
            min = (sec / 60) % 60;
            sec %= 60;
            dest[0] = (u8)(min % 10) + '0';
            dest[1] = ':';
            dest[2] = (u8)(sec / 10) + '0';
            dest[3] = (u8)(sec % 10) + '0';
            if (!ret) {
                dest[4] = 0;
                ret = 4;
            }
            break;

        default:
            break;
    }

    return ret;
}

/**
 * InitGameScores - Clear old players from high score table
 * Based on arcade: hiscore.c:InitGameScores()
 */
void InitGameScores(void) {
    s32 i, j;

    for (j = 0; j < NTRACKS; j++) {
        for (i = 0; i < NSCORES; i++) {
            InThisGame[j][i] = 0;
        }
    }
    gThisRank = -1;
    gThisTrack = -1;
}

/**
 * HiScoreRank - Get rank for a given score on a given track
 * Based on arcade: hiscore.c:HiScoreRank()
 *
 * @param score Player's score (time in ms)
 * @param track Track number
 * @return Rank (0 = best), or -1 if not on table
 */
s16 HiScoreRank(u32 score, s16 track) {
    s16 i;

    if (score == 0) {
        return -1;
    }

    for (i = 0; i < NSCORES; i++) {
        if (gScoreTable[track][i].score >= score) {
            return i;
        }
    }
    return -1;
}

/**
 * ClearAScore - Clear a high score entry to default values
 * Based on arcade: hiscore.c:ClearAScore() (static in arcade)
 */
static void ClearAScore(HiScore *s, const char *name) {
    s->score = MAX_SCORE;
    strcpy(s->name, name);
    s->deaths = 0;
    s->mirror = 0;
    s->car = 0;  /* gThisNode << 4 in arcade */
}

/**
 * ClearHighScores - Clear high score table to default values
 * Based on arcade: hiscore.c:ClearHighScores()
 */
void ClearHighScores(void) {
    s16 i, trk;
    static const char * const defNames[32] = {
        "CAPIZZI", "GIZMO", "SPOO", "ROBMAN", "GUMMER", "RAY",
        "ALAN", "JOHN", "AARON", "SKRID", "SPENCE", "TOM", "ROB", "PETE",
        "FORREST", "DAVE", "PAUL", "JRG", "JAM", "PIXEL", "BINKLEY",
        "FRENZY", "KAYA", "KENNA", "DECAL", "NITE", "ALIEN",
        "SCRODER", "MAGGIE", "MARGE", "HOMER", "ZIGGY"
    };
    u32 rand_idx;

    /* Initialize with random default names */
    for (trk = 0; trk < NTRACKS; trk++) {
        for (i = 0; i < NSCORES; i++) {
            /* Simple pseudo-random selection */
            rand_idx = (u32)((trk * NSCORES + i) * 7 + 3) % 32;
            ClearAScore(&gScoreTable[trk][i], defNames[rand_idx]);
        }
    }

    InitGameScores();
}

/**
 * LoadHighScores - Load high score table from NVRAM
 * Based on arcade: hiscore.c:LoadHighScores()
 *
 * Note: N64 uses Controller Pak, arcade uses BRAM
 */
void LoadHighScores(void) {
    s32 trk, i;
    HiScore *hs;

    /* For N64, use default scores initially */
    /* TODO: Implement Controller Pak load */

    ClearHighScores();

    /* Validate loaded scores */
    for (trk = 0; trk < NTRACKS; trk++) {
        for (i = 0; i < NSCORES; i++) {
            hs = &gScoreTable[trk][i];

            /* Check for invalid scores */
            if (hs->score < MIN_SCORE || hs->score > MAX_SCORE) {
                ClearAScore(hs, "DEFAULT");
            }
            if (i < NSCORES - 1 && hs->score > gScoreTable[trk][i + 1].score) {
                /* Scores out of order - clear rest */
                ClearAScore(hs, "DEFAULT");
            }
        }
    }

    InitGameScores();
}

/**
 * SaveHighScore - Save a high score table entry
 * Based on arcade: hiscore.c:SaveHighScore()
 *
 * @param name Player name
 * @param score Time in milliseconds
 * @param track Track number
 * @param deaths Death count
 * @param mirror Mirror mode flag
 * @param car Car type and player node
 * @param flags 2 = don't mark InThisGame
 * @return Rank, or -1 if not saved
 */
s32 SaveHighScore(char *name, u32 score, u32 track, u32 deaths, u32 mirror, u32 car, u32 flags) {
    s32 rank;
    HiScore *hs;
    s32 i;

    rank = HiScoreRank(score, (s16)track);

    if (rank < 0) {
        return -1;
    }

    /* Check if score already exists in table (reject duplicates) */
    hs = &gScoreTable[track][rank];
    for (i = rank; i < NSCORES; i++, hs++) {
        if (hs->score != score) {
            break;
        }
        if (hs->mirror == (u8)mirror && hs->deaths == (u16)deaths &&
            hs->car == (u8)car && strncmp(hs->name, name, NLENGTH) == 0) {
            return -1;  /* Duplicate entry */
        }
    }

    /* Move other scores down */
    hs = &gScoreTable[track][rank];
    if (rank < NSCORES - 1) {
        memmove(&gScoreTable[track][rank + 1], hs,
                sizeof(HiScore) * (NSCORES - 1 - rank));
        memmove(&InThisGame[track][rank + 1], &InThisGame[track][rank],
                sizeof(s32) * (NSCORES - 1 - rank));

        if ((s32)track == gThisTrack && rank < gThisRank) {
            gThisRank++;
        }
    }

    /* Insert new entry */
    strncpy(hs->name, name, NLENGTH - 1);
    hs->name[NLENGTH - 1] = 0;
    hs->score = score;
    hs->deaths = (u16)deaths;
    hs->mirror = (u8)mirror;
    hs->car = (u8)car;

    if (!(flags & 2)) {
        InThisGame[track][rank] = 1;
    }

    /* TODO: Write to Controller Pak */

    return rank;
}

/**
 * EnterHighScore - Take score and enter into high score table
 * Based on arcade: hiscore.c:EnterHighScore()
 */
void EnterHighScore(s16 track, u32 score, char *name, u32 deaths, u32 mirror, u32 car) {
    s32 rank;

    /* Assume not entering name */
    gEnteringName = 0;
    continue_flag = 1;

    /* Check if score qualifies */
    if (score == 0 || score >= MAX_SCORE) {
        return;
    }

    /* Get rank for this score */
    rank = HiScoreRank(score, track);

    gThisRank = (s16)rank;
    gThisTrack = track;
    gThisScore = score;

    if (rank < 0) {
        gThisRank = 100;  /* Didn't make table */
        return;
    }

    /* Top ten name? */
    if (rank < NNAMES) {
        /* Setup for entry of name */
        holdoff_force = 1;
        gEnteringName = 1;
        gCurName[0] = 0;
        gNameIndex = 0;
        gLetterTries = 6;
        gCurLetter = 'M' - 'A';
        char_on_left = gCurLetter - (NUM_HENTRY_POS - 4 - 1) / 2;
        char_on_right = gCurLetter + (NUM_HENTRY_POS - 4 - 1) / 2;
        last_section = (NUM_HENTRY_POS - 4 - 1) / 2 + 1;
        old_delta_x = 0;
    } else {
        /* Not top 10 - save with default name */
        SaveHighScore("GIZMO", score, track, deaths, mirror, car, 0);
    }
}

/**
 * ShowHiScore - Display or remove the High Score screen
 * Based on arcade: hiscore.c:ShowHiScore()
 *
 * Note: N64 version is simplified - no blit system
 */
void ShowHiScore(s32 show, s16 track) {
    if (show) {
        /* Start display */
        gHiScore.state = HISCORE_STATE_DISPLAY;
        gHiScore.display.current_track = track;
        gHiScore.display.display_timer = 0;
    } else {
        /* Stop display */
        gHiScore.state = HISCORE_STATE_IDLE;
        continue_carry = continue_flag;
        continue_flag = 0;
    }
}

/**
 * ShowScoreEntry - Display or remove the high score entry screen
 * Based on arcade: hiscore.c:ShowScoreEntry()
 */
void ShowScoreEntry(s32 show) {
    if (show) {
        gHiScore.state = HISCORE_STATE_ENTRY;
        gCurName[0] = 0;
        gNameIndex = 0;
        gLetterTries = 6;
        gCurLetter = 'M' - 'A';
        char_on_left = gCurLetter - (NUM_HENTRY_POS - 4 - 1) / 2;
        char_on_right = gCurLetter + (NUM_HENTRY_POS - 4 - 1) / 2;
        last_section = (NUM_HENTRY_POS - 4 - 1) / 2 + 1;
        old_delta_x = 0;
    } else {
        gHiScore.state = HISCORE_STATE_IDLE;
        gEnteringName = 0;
    }
}

/**
 * GetHighScoreName - Process name entry each frame
 * Based on arcade: hiscore.c:GetHighScoreName()
 *
 * Note: N64 version uses different input system
 */
void GetHighScoreName(void) {
    s16 sections;

    sections = (s16)(sizeof(font_list) - 1);

    /* Clamp letter selection */
    if (gCurLetter < 0) {
        gCurLetter = 0;
    }
    if (gCurLetter > sections - 1) {
        gCurLetter = sections - 1;
    }

    /* Check if entered all letters - force to END */
    if (strlen(gCurName) == NLENGTH - 1) {
        gCurLetter = sections - 1;  /* END position */
        left_side_active = 0;
        right_side_active = 0;
    }

    /* Adjust bounds for scrolling letter display */
    if (gCurLetter < char_on_left) {
        char_on_left--;
        char_on_right--;
    } else if (gCurLetter > char_on_right) {
        char_on_left++;
        char_on_right++;
    }

    /* Deactivate arrows if at bounds */
    if (char_on_left == 0) {
        left_side_active = 0;
    } else if (char_on_right == sections - 1) {
        right_side_active = 0;
    }

    holdoff_force = 0;
}

/**
 * HiScoreForce - Handle steering wheel force during name entry
 * Based on arcade: hiscore.c:HiScoreForce()
 *
 * Note: N64 version is a stub - no force feedback
 */
void HiScoreForce(void) {
    /* N64 doesn't have force feedback */
}

/**
 * InitMsgs - Initialize message system
 * Based on arcade: hiscore.c:InitMsgs()
 *
 * Note: N64 version is a stub - different display system
 */
void InitMsgs(s16 fontNum, s32 reset) {
    if (reset) {
        nextBlit = 0;
    }
    /* N64: font selection would go here */
    (void)fontNum;
}

/**
 * AddMsg - Add a message to display
 * Based on arcade: hiscore.c:AddMsg()
 *
 * Note: N64 version is a stub - different display system
 */
void AddMsg(s32 x, s32 y, const char *txt, s32 AnimID, void *AnimFunc, s32 data) {
    /* N64: Would add text to display list */
    (void)x;
    (void)y;
    (void)txt;
    (void)AnimID;
    (void)AnimFunc;
    (void)data;
}

/**
 * RemoveMsgs - Remove all messages
 * Based on arcade: hiscore.c:RemoveMsgs()
 */
void RemoveMsgs(void) {
    nextBlit = 0;
}

/**
 * check_ctrs - Check if tables should be auto-cleared
 * Based on arcade: hiscore.c:check_ctrs()
 */
s32 check_ctrs(void) {
    /* N64: Simplified - no EER counters */
    /* Return 0 = don't auto-clear */
    return 0;
}
