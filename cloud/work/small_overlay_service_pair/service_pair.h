/* Research views, not original typedefs. Native O32 scalar widths required. */
#ifndef SMALL_OVERLAY_SERVICE_PAIR_H
#define SMALL_OVERLAY_SERVICE_PAIR_H

typedef signed char SmallS8;
typedef signed short SmallS16;
typedef unsigned short SmallU16;
typedef signed int SmallS32;
typedef unsigned int SmallU32;

typedef struct SmallPlayer {
    unsigned char pad000[8];
    float position[3];                  /* 008 */
    unsigned char pad014[0x308 - 0x14];
    SmallS8 enabled;                    /* 308 */
    unsigned char pad309[0x359 - 0x309];
    SmallS8 excluded;                   /* 359 */
    unsigned char pad35a;
    SmallS8 car_index;                  /* 35b */
    unsigned char pad35c[0x384 - 0x35c];
    SmallS8 kind;                       /* 384 */
    unsigned char pad385;
    SmallS16 remaining;                 /* 386, gameplay label provisional */
    SmallS16 last_amount;               /* 388 */
    unsigned char pad38a[2];
    SmallU32 flags;                     /* 38c */
    unsigned char pad390[0x3b8 - 0x390];
} SmallPlayer;

typedef struct SmallModel {
    unsigned char pad000[0x640];
    SmallS8 excluded;                   /* 640 */
    unsigned char pad641[0x808 - 0x641];
} SmallModel;

extern SmallPlayer small_players[];      /* 80152818, stride 952 */
extern SmallModel small_models[];        /* 8014a250, stride 2056 */
extern SmallS8 small_teams[];             /* 8012e67c */
extern SmallS16 small_player_count;      /* 801543ca: see alias caveat */

/* Call boundaries verified by native consumption, not original declarations. */
extern void func_800A61B0(float *, float *, void *);
extern float func_8008C768(float, float);
extern void func_800C55E4(SmallS32, SmallS32, SmallS32);

void small_8038D3A4(SmallPlayer *, SmallPlayer *, SmallS32);
void small_8038D798(float *, float *, SmallS32, float, SmallS32);
#endif
