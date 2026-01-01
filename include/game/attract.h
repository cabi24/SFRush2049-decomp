/**
 * attract.h - Attract mode (demo/title screen) for Rush 2049 N64
 *
 * Handles attract sequence, demo gameplay, and title screens.
 * Based on arcade attract.c/attract.h patterns.
 */

#ifndef ATTRACT_H
#define ATTRACT_H

#include "types.h"

/* ========================================================================
 * ARCADE-COMPATIBLE DEFINES (from rushtherock/game/attract.h)
 * ======================================================================== */

/* Attract mode enumeration - matches arcade AttractMode enum */
typedef enum AttractMode {
    ATR_HSSETUP = -1,       /* High score setup (internal) */
    ATR_HSENTRY = 0,        /* High score entry */
    ATR_HISCORE1,           /* High score track 1 */
    ATR_ATARILOGO,          /* Atari logo movie 1 */
    ATR_HISCORE2,           /* High score track 2 */
    ATR_MOVIE5,             /* Rush logo movie */
    ATR_HISCORE3,           /* High score track 3 */
    ATR_TEAM,               /* Team credits */
    ATR_OTEAM,              /* Old team credits */
    ATR_ATARILOGO2,         /* Atari logo movie 2 */
    ATR_ADVERT,             /* Advertisement screen */
    ATR_MOVIE2,             /* Car 1 rotating movie */
    ATR_HISCORE4,           /* High score track 4 */
    ATR_MIRROR,             /* Mirror mode screen */
    ATR_HISCORE5,           /* High score track 5 */
    ATR_MOVIE52,            /* Rush logo movie 2 */
    ATR_HISCORE6,           /* High score track 6 */
    ATR_OCREDITS,           /* Old credits */
    ATR_3DFXLOGO,           /* 3dfx logo (N64: Nintendo logo) */
    ATR_ATARILOGO3,         /* Atari logo movie 3 */
    ATR_HISCORE7,           /* High score track 7 */
    ATR_MOVIE4,             /* Girl movie */
    ATR_DEMO,               /* Demo game 1 */
    ATR_CREDITS,            /* Credits screen */
    ATR_MOVIE53,            /* Rush logo movie 3 */
    ATR_ADVISORY,           /* Advisory screen */
    ATR_MOVIE3,             /* Car 2 rotating movie */
    ATR_DEMO2,              /* Demo game 2 */
    ATR_CARS,               /* Cars screen */
    /* Special modes after this point */
    ATR_TOURNEY,            /* Tourney setup */
    ATR_TRANSIT,            /* Transition screen */
    ATR_JOIN,               /* Join-in screen */
    ATR_GAMESTAT,           /* Game statistics */
    NUM_ATTRACTS            /* Total count */
} AttractMode;

/* Model types for loading - matches arcade ModelType enum */
typedef enum ModelTypes {
    MT_NONE = 0,
    MT_ATTRACT,
    MT_HISCORE,
    MT_HUD,
    MT_STATIC
} ModelType;

/* Arcade timing constants (ONE_SEC = 1000 ms on arcade, 60 frames on N64) */
#define ONE_SEC_ARCADE      1000    /* Arcade uses milliseconds */
#define ONE_SEC             60      /* N64 uses frames at 60fps */

/* High score defines from arcade attract.h */
#define NSCORES             100     /* Number of scores saved */
#define MAX_GAME_CNT        2000    /* Max games before auto HSCORE reset */
#define MAX_HI_CNT          200     /* Max non-HS games before reset */
#define NLENGTH             9       /* Max length of high score name + 1 */

/* ========================================================================
 * N64-SPECIFIC DEFINES
 * ======================================================================== */

/* Attract mode states (simplified for N64) */
#define ATTRACT_STATE_INIT          0
#define ATTRACT_STATE_LOGO          1   /* Title logo display */
#define ATTRACT_STATE_CREDITS       2   /* Game credits display */
#define ATTRACT_STATE_HISCORE       3   /* High score table */
#define ATTRACT_STATE_DEMO          4   /* Demo gameplay */
#define ATTRACT_STATE_JOIN          5   /* "Press Start" / Join prompt */
#define ATTRACT_STATE_TRANSITION    6   /* Transitioning between states */
#define ATTRACT_STATE_COUNT         7

/* Demo modes */
#define DEMO_MODE_NONE              0
#define DEMO_MODE_REPLAY            1   /* Replay recorded demo */
#define DEMO_MODE_AI                2   /* AI-controlled demo */
#define DEMO_MODE_ATTRACT           3   /* Attract-specific demo */

/* Attract timing (frames at 60fps) - based on arcade gAttractTimes[] */
#define ATTRACT_LOGO_TIME           (ONE_SEC * 10)  /* Logo: 10 seconds */
#define ATTRACT_CREDITS_TIME        (ONE_SEC * 8)   /* Credits: 8 seconds */
#define ATTRACT_HISCORE_TIME        (ONE_SEC * 20)  /* High score: 20 seconds */
#define ATTRACT_DEMO_TIME           (ONE_SEC * 35)  /* Demo: 35 seconds */
#define ATTRACT_JOIN_TIME           (ONE_SEC * 15)  /* Join: 15 seconds */
#define ATTRACT_TRANSITION_TIME     (ONE_SEC / 2)   /* Transition: 0.5 second */
#define ATTRACT_ADVISORY_TIME       (ONE_SEC * 8)   /* Advisory: 8 seconds */
#define ATTRACT_3DFX_TIME           (ONE_SEC * 3)   /* 3dfx/Nintendo: 3 seconds */

/* Attract flags */
#define ATTRACT_FLAG_ACTIVE         0x01
#define ATTRACT_FLAG_SKIP_ALLOWED   0x02
#define ATTRACT_FLAG_DEMO_ACTIVE    0x04
#define ATTRACT_FLAG_SHOW_CREDITS   0x08
#define ATTRACT_FLAG_FREEPLAY       0x10

/* Logo animation */
#define LOGO_ANIM_NONE              0
#define LOGO_ANIM_FADE_IN           1
#define LOGO_ANIM_HOLD              2
#define LOGO_ANIM_FADE_OUT          3

/* Attract sequence item */
typedef struct AttractItem {
    u8          state;              /* ATTRACT_STATE_* */
    u16         duration;           /* Duration in frames */
    u8          flags;              /* Item flags */
} AttractItem;

/* Demo state */
typedef struct DemoState {
    u8          mode;               /* DEMO_MODE_* */
    u8          track_id;           /* Track for demo */
    u8          car_type;           /* Car for demo */
    u8          active;             /* Demo currently running */
    u32         frame;              /* Current frame */
    u32         max_frames;         /* Maximum frames */
    u8          ai_car_index;       /* AI car being shown */
    u8          pad[3];
} DemoState;

/* Logo display state */
typedef struct LogoState {
    u8          animation;          /* LOGO_ANIM_* */
    f32         alpha;              /* Current alpha (0-1) */
    f32         scale;              /* Current scale */
    f32         rotation;           /* Current rotation */
    u32         timer;              /* Animation timer */
} LogoState;

/* Credit display item */
typedef struct CreditItem {
    const char  *name;              /* Name or text */
    const char  *role;              /* Role/title (optional) */
    u8          type;               /* 0=name, 1=header, 2=spacer */
    u8          pad[3];
} CreditItem;

/* Attract mode state */
typedef struct AttractState {
    /* Current state */
    u8          state;              /* ATTRACT_STATE_* */
    u8          prev_state;         /* Previous state */
    u8          flags;              /* ATTRACT_FLAG_* */
    u8          sequence_index;     /* Current sequence position */

    /* Timing */
    u32         state_timer;        /* Time in current state */
    u32         total_timer;        /* Total attract time */
    u32         state_duration;     /* Duration for current state */

    /* Sequence */
    AttractItem *sequence;          /* Attract sequence */
    s32         sequence_length;    /* Number of items */

    /* Sub-states */
    DemoState   demo;               /* Demo state */
    LogoState   logo;               /* Logo display state */

    /* High score display */
    s32         hiscore_track;      /* Current track showing */
    u32         hiscore_timer;      /* Track display timer */

    /* Credits scroll */
    f32         credits_scroll;     /* Scroll position */
    f32         credits_speed;      /* Scroll speed */
    s32         credits_index;      /* Current credit item */

    /* Join prompt */
    u32         join_blink_timer;   /* Blink timer for "Press Start" */
    u8          join_blink_on;      /* Blink state */
    u8          pad[3];

    /* Transition */
    f32         fade_alpha;         /* Fade alpha */
    u8          fade_in;            /* Fading in vs out */
    u8          pad2[3];

} AttractState;

/* Global state */
extern AttractState gAttract;

/* Credits data */
extern const CreditItem gGameCredits[];
extern const s32 gNumCredits;

/* Default attract sequence */
extern AttractItem gDefaultSequence[];
extern const s32 gDefaultSequenceLength;

/* Initialization */
void attract_init(void);
void attract_reset(void);
void attract_start(void);
void attract_stop(void);

/* Update and draw */
void attract_update(void);
void attract_draw(void);

/* State management */
void attract_set_state(u8 state);
void attract_next_state(void);
void attract_prev_state(void);
s32 attract_is_active(void);

/* Input handling */
void attract_handle_input(void);
s32 attract_check_start(void);
void attract_skip(void);

/* Logo display */
void attract_show_logo(void);
void attract_hide_logo(void);
void attract_update_logo(void);
void attract_draw_logo(void);

/* Credits display */
void attract_show_credits(void);
void attract_update_credits(void);
void attract_draw_credits(void);

/* High score display (uses hiscore.c) */
void attract_show_hiscore(void);
void attract_update_hiscore(void);
void attract_draw_hiscore(void);

/* Demo mode */
void attract_start_demo(void);
void attract_stop_demo(void);
void attract_update_demo(void);
void attract_draw_demo(void);
s32 attract_is_demo_active(void);

/* Join prompt */
void attract_show_join(void);
void attract_update_join(void);
void attract_draw_join(void);

/* Transitions */
void attract_fade_in(void);
void attract_fade_out(void);
void attract_update_fade(void);

/* Utility */
u32 attract_get_demo_track(void);
u32 attract_get_demo_car(void);

/* ========================================================================
 * ARCADE-COMPATIBLE STRUCTURES (from rushtherock/game/attract.h)
 * ======================================================================== */

/* High score structure - matches arcade HiScore typedef */
typedef struct HiScore {
    u32     score;                  /* Score value */
    char    name[NLENGTH];          /* Player name */
    s8      deaths;                 /* Number of deaths */
    u8      mirror;                 /* TRUE if mirror mode race */
    s8      car;                    /* High nibble = node, low nibble = car */
} HiScore;

/* Tourney mode structure - matches arcade Tourney typedef */
typedef struct Tourney {
    s32     free;                   /* 0=pay each time, 1=free play */
    s8      ext_start;              /* 0=player select, 1=external switch, 2=quick join */
    u32     cabinets;               /* Bitmask: 1<<node if node is in tourney */
    s16     laps;                   /* >0=lap count, 0=no limit, <0=normal */
    s8      track;                  /* >=0=track, <0=external switch */
    s8      car;                    /* >=0=car, <0=any car */
    s8      tranny;                 /* >=0=tranny, <0=any tranny */
    s8      drones;                 /* 1=drones, 0=none, <0=normal */
    s8      catchup;                /* 1=catchup, 0=none, <0=normal */
    u32     sanity;                 /* If != 0xDEADBEEF, invalid */
} Tourney;

/* ========================================================================
 * ARCADE-COMPATIBLE GLOBALS (from rushtherock/game/attract.c)
 * ======================================================================== */

/* Current attract function/mode */
extern AttractMode attractFunc;

/* Global attract state flags */
extern u8   gStartDuringLoad;       /* Start button pressed during load */
extern s32  gFreeGame;              /* Player won a free game */
extern s32  gPlayingFree;           /* Playing a free game */
extern s32  gContinuedGame;         /* Playing a continued game */
extern s32  explosion_state;        /* Explosion effects active */
extern s32  explosion_time;         /* Explosion timer */
extern u8   skip;                   /* Skip counter for attract screens */
extern s32  gTourneyJoin;           /* Tourney join flag */

/* Attract timing array - indexed by AttractMode */
extern s16  gAttractTimes[NUM_ATTRACTS];

/* Version number */
extern const f32 gVersion;

/* ========================================================================
 * ARCADE-COMPATIBLE FUNCTIONS (from rushtherock/game/attract.c)
 * ======================================================================== */

/* Main attract functions - match arcade prototypes */
void InitAttract(void);             /* Initialize attract mode globals */
void attract(void);                 /* Main attract mode handler */
void AttractForce(void);            /* Handle steering wheel forces in attract */

/* Show/hide functions - match arcade static functions (made public for N64) */
void ShowAttract(s16 func, s32 show);   /* Show/hide attract screen by func */
void ShowTransit(s32 show, s16 num);    /* Show/hide transition screen */
void ShowJoin(s32 show);                /* Show/hide join-in screen */
void ShowCredits(s32 show);             /* Show/hide credits screen */
void ShowHiScore(s32 show, s32 track);  /* Show/hide high score for track */
void ShowScoreEntry(s32 show);          /* Show/hide score entry screen */
void ShowTeam(s32 show);                /* Show/hide team credits */
void ShowAdvisory(s32 show);            /* Show/hide advisory screen */
void ShowAdvert(s32 show);              /* Show/hide advertisement screen */
void ShowMovie(s32 show, s32 num);      /* Show/hide movie screen */

/* Demo game functions */
void play_demogame(s32 run, s32 init, s32 track, s32 attract_mode);

/* Utility functions */
s32 TimeOut(void);                  /* Check if countdown timer expired */
void SetCountdownTimer(u32 time);   /* Set countdown timer */
s32 chk_start(void);                /* Check if start button pressed */
s32 EnoughCredit(void);             /* Check if enough credits to play */
void ResetJoinIn(void);             /* Reset join-in state */
s32 TourneyOn(void);                /* Check if tourney mode is on */
s32 TourneyNode(s32 node);          /* Check if node is in tourney */
s32 ChkGameStats(void);             /* Check for game stats mode */
void ShowGameStats(s32 show);       /* Show/hide game stats */
void HandleTourneySetup(void);      /* Handle tourney setup screen */
void ShowTourneySetup(s32 show);    /* Show/hide tourney setup */

/* Animation functions - match arcade BlitFunc prototypes */
s32 AnimateTimer(void *blt);        /* Animate countdown timer */
s32 AnimateTransit(void *blt);      /* Animate transition screen */
s32 AnimateCreds(void *blt);        /* Animate credits display */

#endif /* ATTRACT_H */
