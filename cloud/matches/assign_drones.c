/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * assign_drones (0x800F4FEC, 1236 B): historical label. This is the N64 end-of-race records update
 * (w19e reconstruction). For each player (count D_8014A108, Player records D_8014A118[] stride 0x4C):
 * resolve the cached profile pointer (else &D_80146150[slot]); stop if the profile has no save block;
 * then for the profile's own record (*(*prof)->save)->rec[track] and the global D_80150F88[track]:
 * insert the car's finish time into the ascending 5-entry times[] (for the global set also shift the
 * owner bytes D_80151AC0[n+9 -> n+10] and owner pointers D_80151690[track].who[]), bump races/u54,
 * add laps, the lap-time sum D_80149A78[i][0..laps) and car distance / 528.0f. Finally
 * func_800CD8EC(prof, (u8)track2). With the mirror flag D_80152570, track += 6 and track2 += 19.
 *
 * Source-form notes (no shaping devices, no unused locals):
 * - track/track2 are each assigned from D_8014978C (retail keeps the loaded byte as its own value in v0,
 *   copies it to track and track2, and forms +6 from it); track2 += 19 reads the variable (ugen temp ring).
 * - car is assigned the record address, the finished test indexes D_80152818[] directly and the time read
 *   goes through car: retail computes the address in v0 and copies it to car (move s5,v0).
 */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Record {        /* 0x60 per track */
    u8 pad00[0x2C];
    f32 times[5];              /* 0x2C best times, ascending */
    f32 total;                 /* 0x40 */
    u16 laps;                  /* 0x44 */
    u16 races;                 /* 0x46 */
    u8 pad48[0x4E - 0x48];
    u16 u4E;                   /* 0x4E */
    u8 pad50[0x54 - 0x50];
    u16 u54;                   /* 0x54 */
    u8 pad56[0x58 - 0x56];
    u32 dist;                  /* 0x58 */
    u8 pad5C[0x60 - 0x5C];
} Record;

typedef struct SaveBlock {
    u8 pad00[0x8C];
    Record rec[1];             /* 0x8C */
} SaveBlock;

typedef struct Profile {
    u8 pad00[8];
    s32 valid;                 /* 0x08 */
    u8 pad0C[0x2C - 0x0C];
    SaveBlock **save;          /* 0x2C */
} Profile;

typedef struct Owner {         /* 0x3C per track */
    u8 pad00[0x28];
    Profile **who[5];          /* 0x28 */
} Owner;

typedef struct Player {        /* 0x4C */
    u8 car;                    /* 0x00 */
    u8 slot;                   /* 0x01 */
    u8 pad02[0x40 - 0x02];
    u16 laps;                  /* 0x40 */
    u8 pad42[0x48 - 0x42];
    Profile **prof;            /* 0x48 */
} Player;

typedef struct CarData {       /* 0x3B8 */
    u8 pad00[0xEF];
    s8 finished;               /* 0xEF */
    f32 time;                  /* 0xF0 */
    u8 padF4[0x108 - 0xF4];
    f32 dist;                  /* 0x108 */
    u8 pad10C[0x3B8 - 0x10C];
} CarData;

extern s8 D_8014978C;          /* track */
extern s8 D_80152570;          /* mirror / reverse flag */
extern s16 D_8014A108;         /* player count */
extern Player D_8014A118[];
extern Profile *D_80146150[];
extern u8 D_80144018[];        /* laps driven per player */
extern f32 D_80149A78[][8];    /* lap times per player */
extern CarData D_80152818[];
extern Record D_80150F88[];
extern Owner D_80151690[];
extern s8 D_80151AC0[];
void func_800CD8EC(Profile **prof, u8 track);

void assign_drones(void)
{
    s32 track, track2;
    s32 i, j, k, n;
    Record *r;
    CarData *car;
    f32 time;

    track = D_8014978C;
    track2 = D_8014978C;
    if (D_80152570) {
        track = D_8014978C + 6;
        track2 += 19;
    }
    for (i = 0; i < D_8014A108; i++) {
        if (D_8014A118[i].prof == 0)
            D_8014A118[i].prof = &D_80146150[D_8014A118[i].slot];
        if ((*D_8014A118[i].prof)->save == 0)
            return;
        for (j = 0; j < 2; j++) {
            if (j == 0)
                r = &(*(*D_8014A118[i].prof)->save)->rec[track];
            else
                r = &D_80150F88[track];
            car = &D_80152818[D_8014A118[i].car];
            if (D_80152818[D_8014A118[i].car].finished) {
                time = car->time;
                for (k = 0; k < 5; k++) {
                    if (r->times[k] == 0.0f || time < r->times[k]) {
                        for (n = 4; n > k; n--) {
                            r->times[n] = r->times[n - 1];
                            if (j == 1) {
                                D_80151AC0[n + 10] = D_80151AC0[n + 9];
                                D_80151690[track].who[n] = D_80151690[track].who[n - 1];
                            }
                        }
                        r->times[k] = time;
                        if (j == 1) {
                            D_80151AC0[n + 10] = i;
                            if ((*D_8014A118[i].prof)->valid)
                                D_80151690[track].who[k] = D_8014A118[i].prof;
                        }
                        break;
                    }
                }
            }
            r->races++;
            r->u54++;
            r->u4E += D_8014A118[i].laps;
            r->laps += D_80144018[i];
            for (n = 0; n < D_80144018[i]; n++)
                r->total += D_80149A78[i][n];
            r->dist += car->dist / 528.0f;
        }
        func_800CD8EC(D_8014A118[i].prof, track2);
    }
}
