#ifndef GSTATE_H
#define GSTATE_H

/* Game states (bitmask flags used in N64 version) */
#define GSTATE_INIT     0x00040000
#define GSTATE_SETUP    0x00080000
#define GSTATE_MENU     0x00100000
#define GSTATE_CAMERA   0x00200000
#define GSTATE_READY    0x00400000
#define GSTATE_PLAY     0x00800000
#define GSTATE_CARS     0x01000000
#define GSTATE_FINISH   0x02000000

/* Arcade-style enum (for reference) */
typedef enum {
    ATTRACT,
    TRKSEL,     /* Track select */
    CARSEL,     /* Car select */
    PREPLAY,
    COUNTDOWN,
    PREPLAY2,
    PLAYGAME,
    ENDGAME,
    GAMEOVER,
    HISCORE
} GState;

/* Key globals */
extern u32 gstate;           /* 0x801146EC */
extern void* car_array;      /* 0x80152818 */
extern void* game_struct;    /* 0x801461D0 */
extern u32 frame_counter;    /* 0x80142AFC */

#endif
