# High Score System Documentation

This document describes the high score system for the Rush 2049 N64 decompilation project, based on analysis of the arcade source code at `reference/repos/rushtherock/game/hiscore.c` and the N64 implementation.

## Overview

The high score system tracks lap times for each track, allows players to enter their name (initials) when achieving a qualifying score, and persists scores to storage. The system is fundamentally a **time-based leaderboard** where lower times are better.

## 1. Score Data Structure

### Arcade Structure (from `attract.h`)

```c
#define NLENGTH  9    /* Max length of a high score name + 1 (null terminator) */

typedef struct HiScore {
    U32     score;              /* Time in milliseconds (1/1000 second units) */
    char    name[NLENGTH];      /* Player name/initials (8 chars + null) */
    S8      deaths;             /* Number of crashes/deaths during race */
    U8      mirror;             /* TRUE if race was in mirror mode */
    S8      car;                /* Car type (low nibble) + node ID (high nibble) */
} HiScore;
```

**Size**: 16 bytes per entry (4 + 9 + 1 + 1 + 1)

### N64 Structure (from `include/game/hiscore.h`)

```c
#define MAX_NAME_LENGTH  3    /* 3-letter initials only */

typedef struct HiScoreEntry {
    char    name[MAX_NAME_LENGTH + 1];  /* Player initials + null (4 bytes) */
    u32     time;                       /* Time in frames at 60fps */
    u8      car_type;                   /* Car used */
    u8      mirror;                     /* Mirror mode flag */
    u8      valid;                      /* Entry is valid */
    u8      pad;                        /* Padding for alignment */
} HiScoreEntry;
```

**Size**: 12 bytes per entry

### Key Differences

| Field | Arcade | N64 |
|-------|--------|-----|
| Name length | 8 characters | 3 characters (initials only) |
| Time unit | Milliseconds | Frames (60fps) |
| Deaths tracked | Yes | No |
| Node ID in car | Yes (high nibble) | No (single player) |

## 2. High Score Table Layout

### Arcade Layout

```c
#define NSCORES    100    /* 100 scores per track */
#define NTRACKS    7      /* 7 tracks total */

static HiScore gScoreTable[NTRACKS][NSCORES];
```

**Total storage**: 7 tracks x 100 entries x 16 bytes = **11,200 bytes**

The arcade displays:
- **Top 10 (NNAMES=10)**: Full display with car icons on main screen
- **Scrolling list (SSCORES=9)**: Shows positions 11-100 in a scrolling sidebar

### N64 Layout

```c
#define MAX_SCORES   10    /* 10 scores per track */
#define MAX_TRACKS   8     /* 8 tracks total */

typedef struct TrackScores {
    HiScoreEntry    scores[MAX_SCORES];
    u8              track_id;
    u8              num_valid;
    u8              pad[2];
} TrackScores;

typedef struct HiScoreState {
    TrackScores     tables[MAX_TRACKS];
    /* ... other state ... */
} HiScoreState;
```

**Total storage**: 8 tracks x 10 entries x 12 bytes = **960 bytes** (plus overhead)

## 3. Time Representation

### Arcade (Milliseconds)

```c
#define ONE_SEC      1000           /* 1 second = 1000ms */
#define MIN_SCORE    (1L*60*ONE_SEC)   /* 1 minute minimum */
#define MAX_SCORE    (10L*60*ONE_SEC)  /* 10 minute maximum */
#define MAX_HSCORE   (100L*60*ONE_SEC) /* 99:59.99 display max */
```

The arcade stores times in milliseconds and displays in `M:SS.HH` format (hundredths):

```c
/* cvt_time_str format 'h': */
/* "M:SS.HH" - e.g., "2:34.56" */
```

### N64 (Frames at 60fps)

```c
#define MIN_SCORE_TIME   (1 * 60 * 60)    /* 1 minute = 3600 frames */
#define MAX_SCORE_TIME   (10 * 60 * 60)   /* 10 minutes = 36000 frames */
#define MAX_DISPLAY_TIME (100 * 60 * 60)  /* 99:59.99 = 360000 frames */
```

Time conversion from frames to display:
```c
total_hundredths = (frames * 100) / 60;
minutes = total_hundredths / 6000;
seconds = (total_hundredths / 100) % 60;
hundredths = total_hundredths % 100;
```

## 4. Name Entry Input Handling

### Arcade (Steering Wheel)

The arcade uses the steering wheel for character selection:

```c
#define NUM_HENTRY_POS  13    /* 13 selectable positions */
#define HSCORE_LWIDTH   32    /* Letter width in pixels */

static const char font_list[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!?.{}";
/* Note: { = DELETE, } = END */
```

**Input flow:**
1. Wheel position maps to `WheelSection(NUM_HENTRY_POS, 1)` to select letters
2. Gas pedal confirms selection
3. Special characters `{` (delete) and `}` (end) for control
4. Arrow indicators show if more letters available left/right
5. Force feedback keeps wheel centered at each position

**Timeout**: 15 seconds per letter, 6 letters maximum:
```c
gLetterTries = 6;  /* Initial letter tries */
if (gLetterTries > 0)
    SetCountdownTimer(15 * ONE_SEC);
```

### N64 (Controller)

The N64 uses the analog stick/D-pad for navigation:

```c
static const char sCharSet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ _<";
/* Note: _ = SPACE, < = DELETE */

#define HISCORE_CHAR_A       0
#define HISCORE_CHAR_Z       25
#define HISCORE_CHAR_SPACE   26
#define HISCORE_CHAR_DEL     27
#define HISCORE_CHAR_END     28
```

**Input mapping:**
- D-pad/Stick left/right: Navigate character selection
- A button: Select character
- B button: Delete previous character
- Start: Confirm and end entry

**Timeout**: 30 seconds total:
```c
entry->timeout = 60 * 30;  /* 30 seconds at 60fps */
```

## 5. Score Persistence

### Arcade (NVRAM/BRAM)

The arcade stores scores in battery-backed RAM:

```c
#define BRAM_BASE    /* Hardware-specific base address */
#define TABLE_BASE   ((U32 *)(BRAM_BASE + 8*1024L))
#define TABLE_SIZE   (sizeof(HiScore) * NSCORES)  /* 1600 bytes per track */

static void WriteTable(S16 t) {
    U8  *src = (U8 *)gScoreTable[t];
    U32 *tbl = TABLE_BASE + TABLE_SIZE * t;

    for (i = 0; i < TABLE_SIZE; ++i)
        eer_write_byte((U8 *)tbl++, (U8 *)BRAM_UNLK, 0xff & *src++);
}
```

Also writes to GUTS high score table:
```c
eer_hstw(i, &hent, HST_TRACK1_TOP + t);
```

**Auto-clear feature:**
```c
#define MAX_GAME_CNT  2000   /* Games before auto-reset */
#define MAX_HI_CNT    200    /* Non-HS games before reset */

if ((game_opt & GO_AUTOCLEARHS) &&
    eer_gets(EER_GAME_CNT) >= MAX_GAME_CNT &&
    eer_gets(EER_HI_CNT) >= MAX_HI_CNT) {
    ClearHighScores();
}
```

### N64 (Controller Pak)

The N64 uses the Controller Pak (Memory Pak) via libultra `osPfs*` functions:

```c
/* From src/game/save.c */
extern s32 osPfsInitPak(void *queue, void *pfs, s32 controller);
extern s32 osPfsFreeBlocks(void *pfs, s32 *bytes_free);
extern s32 osPfsFindFile(void *pfs, u16 company, u32 game, ...);
extern s32 osPfsAllocateFile(void *pfs, ...);
extern s32 osPfsReadWriteFile(void *pfs, s32 file_no, u8 flag, ...);
```

**Controller Pak specs:**
- Total capacity: 32KB (256 pages x 128 bytes)
- Page size: 128 bytes
- 16 file slots
- Company code + game code identification

**Save data includes:**
- High scores for all tracks
- Game options/settings
- Unlocked content

## 6. Track-Specific vs Global Scores

### Track-Specific (Both Platforms)

Both arcade and N64 maintain **separate leaderboards per track**:

```c
/* Arcade */
static HiScore gScoreTable[NTRACKS][NSCORES];

/* N64 */
TrackScores tables[MAX_TRACKS];
```

**Score qualification is per-track:**
```c
S16 HiScoreRank(U32 score, S16 track) {
    for (i = 0; i < NSCORES; ++i)
        if (gScoreTable[track][i].score >= score)
            return i;
    return -1;  /* Did not qualify */
}
```

### Global Elements

**Current game session tracking (arcade):**
```c
static BOOL InThisGame[NTRACKS][NSCORES];  /* Marks scores from current session */
```

**Display state:**
```c
/* Tracks currently displayed track for scrolling between tables */
gHiScore.display.current_track
```

## 7. N64 vs Arcade Differences Summary

| Feature | Arcade | N64 |
|---------|--------|-----|
| **Scores per track** | 100 | 10 |
| **Tracks** | 7 | 8 |
| **Name length** | 8 characters | 3 initials |
| **Time storage** | Milliseconds | Frames (60fps) |
| **Deaths tracked** | Yes | No |
| **Input device** | Steering wheel + gas pedal | Controller |
| **Storage** | NVRAM (11KB) | Controller Pak (~1KB) |
| **Character set** | A-Z, 0-9, !?.{} | A-Z, space, delete |
| **Entry timeout** | 15 sec/letter | 30 sec total |
| **Display format** | Top 10 + scrolling 11-100 | Top 10 only |
| **Mirror mode tracking** | Yes | Yes |
| **Continue system** | Arcade credits | N/A |
| **Network scores** | Node ID embedded | Single player only |
| **Auto-clear** | After 2000 games | No |
| **Censorship** | Bad word filter | No |

## 8. Function Mapping: Arcade to N64

Based on `symbol_addrs.us.txt`:

| Arcade Function | N64 Address | N64 Name | Description |
|-----------------|-------------|----------|-------------|
| `EnterHighScore()` | 0x800FBF88 | `hiscore_entry_handler` | Main entry point after race |
| `HiScoreRank()` | 0x800DBFC0 | `hiscore_check_handler` | Check if score qualifies |
| `ShowHiScore()` | 0x800DC000 | `hiscore_display_handler` | Display high score table |
| `SaveHighScore()` | 0x800DC100 | `hiscore_insert_handler` | Insert new score entry |
| `ShowScoreEntry()` | - | (part of entry handler) | Name entry screen |
| `GetHighScoreName()` | - | (part of entry handler) | Process name input |
| `ClearHighScores()` | - | `hiscore_clear_all` | Reset all tables |
| `LoadHighScores()` | - | `hiscore_load` | Load from storage |
| `WriteTable()` | - | `hiscore_save_track` | Save single track |

**Note**: The N64 stunt mode has its own high scores at `0x800D5C90` (`high_scores_display`).

## 9. Related Files

### Arcade Source
- `reference/repos/rushtherock/game/hiscore.c` - Main implementation (1,725 lines)
- `reference/repos/rushtherock/game/attract.h` - HiScore structure, constants
- `reference/repos/rushtherock/game/globals.h` - NTRACKS, MAX_TRACKS

### N64 Source
- `src/game/hiscore.c` - N64 implementation (793 lines)
- `include/game/hiscore.h` - N64 structures and API (182 lines)
- `src/game/save.c` - Controller Pak save system
- `src/libultra/os_pfs*.c` - Low-level Controller Pak functions

## 10. Default Names

### Arcade (32 names, random selection)
```c
const char *defNames[] = {
    "CAPIZZI", "GIZMO", "SPOO", "ROBMAN", "GUMMER", "RAY",
    "ALAN", "JOHN", "AARON", "SKRID", "SPENCE", "TOM", "ROB", "PETE",
    "FORREST", "DAVE", "PAUL", "JRG", "JAM", "MR.PIXEL", "BINKLEY",
    "FRENZY", "KAYA", "KENNA", "DECAL", "LATENITE", "ALIEN",
    "SCHRODER", "MAGGIE", "MARGE", "HOMER", "ZIGGY"
};
```

### N64 (10 names, sequential)
```c
const char *gDefaultNames[] = {
    "AAA", "BBB", "CCC", "DDD", "EEE",
    "FFF", "GGG", "HHH", "III", "JJJ"
};
```

## 11. Content Filtering (Arcade Only)

The arcade includes a profanity filter:

```c
const char *obscenities[] = {
    "ASSHOLE", "ASSWHOLE", "FUCK", "PHUK", "PHUCK", "FUK", "SHIT", "CUM",
    "PISS", "CUNT", "PUSSY", "TWAT", "COCK", "NIGGER", "NIGGA", "FAGGOT",
    "FAG", "CHINK", "GOOK", "BEANER", "WETBACK", ""
};

/* Filter replaces matched letters with '!' */
static BOOL checkword(char *wp);
static BOOL fixword(char *op, char *wp);
static void FilterBadChars(char *str);
```

The N64 version does not include content filtering due to memory/complexity constraints.

## 12. Sound Effects

### Arcade
- `S_WHATSYERNAME` - Played when entering name entry
- `S_PLAYITAGAIN` - Played when score doesn't qualify
- `S_WHATSTAG` - Played after name entry completion
- `S_STOP_ALL_SNDS` - Stops all sounds before new music

### N64
Sound effects are handled through the N64 audio system but follow similar patterns.
