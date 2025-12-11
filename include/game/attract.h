/**
 * attract.h - Attract mode (demo/title screen) for Rush 2049 N64
 *
 * Handles attract sequence, demo gameplay, and title screens.
 * Based on arcade attract.c patterns.
 */

#ifndef ATTRACT_H
#define ATTRACT_H

#include "types.h"

/* Attract mode states */
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

/* Attract timing (frames at 60fps) */
#define ATTRACT_LOGO_TIME           (60 * 5)    /* 5 seconds on logo */
#define ATTRACT_CREDITS_TIME        (60 * 4)    /* 4 seconds on credits */
#define ATTRACT_HISCORE_TIME        (60 * 5)    /* 5 seconds per track */
#define ATTRACT_DEMO_TIME           (60 * 30)   /* 30 seconds demo */
#define ATTRACT_JOIN_TIME           (60 * 10)   /* 10 seconds join prompt */
#define ATTRACT_TRANSITION_TIME     (60 / 2)    /* 0.5 second transition */

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

#endif /* ATTRACT_H */
