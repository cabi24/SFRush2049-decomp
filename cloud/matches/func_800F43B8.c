/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* -O3 required: at -O2 the slot-pointer web takes s0 and the frame grows, 136/144 */
/*
 * End-of-race statistics update for the current player (D_801543D4), N64-only.
 * Two 28-byte records are updated with the same code: first the one inside
 * the player's save data (base + 0x684 + sel * 0x1C, sel = s8 at base+0x6FC),
 * then the session copy D_80151618[sel].  Per record: total++, one of three
 * counters by D_80154450.unk1 (0/1/2), two more counters when D_80142760 is
 * set (the second only for unk1 < 3), sum += D_80154450.unk2, best =
 * max(best, unk2), and bestTime = base->unk704 when bestTime is 0 or greater
 * than it.  Finally menu_dialog_close(ref, sel) re-checksums the save record
 * (it stores format_string_parse(rec+4, 24) at rec+0 and marks the slot).
 *
 * `ref` (Player76 +0x48) points into the pointer table D_80146150 and is
 * filled in lazily from the byte at +1, as in func_800F7A98.
 *
 * What mattered (no quirks): every access is spelled
 * D_8014A118[D_801543D4].ref, never through a local `Player76 *p`.  The
 * value of that expression is then one CSE web for the whole function (t1 at
 * the top and again in front of the final call), which pushes the constants
 * to t1/t2 as in retail.  With a pointer local the top and the end are two
 * webs (v1 and a0) and 20 words differ.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    /* 0x00 */ s32 checksum;
    /* 0x04 */ u16 total;
    /* 0x06 */ u16 count[3];
    /* 0x0C */ u16 unkC;
    /* 0x0E */ u16 unkE;
    /* 0x10 */ u16 best;
    /* 0x14 */ f32 bestTime;
    /* 0x18 */ s32 sum;
} Rec; /* 0x1C */

typedef struct {
    /* 0x000 */ u8 pad0[0x684];
    /* 0x684 */ Rec recs[4];
    /* 0x6F4 */ u8 pad6F4[8];
    /* 0x6FC */ s8 unk6FC;
    /* 0x700 */ f32 unk700;
    /* 0x704 */ f32 unk704;
} Base;

typedef struct {
    Base *base;
} Data;

typedef struct {
    u8 pad0[0x2C];
    Data *data;
} Car;

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 index;
    /* 0x02 */ u8 pad2[0x46];
    /* 0x48 */ Car **ref;
} Player76; /* 0x4C */

typedef struct {
    u8 unk0;
    u8 unk1;
    u16 unk2;
} Result;

extern u8 D_801543D4;
extern Player76 D_8014A118[];
extern Car *D_80146150[];
extern Rec D_80151618[];
extern s8 D_80142760;
extern Result D_80154450;

void menu_dialog_close(Car **ref, u8 idx);

void func_800F43B8(void) {
    s32 i;
    s32 idx;
    Rec *rec;

    if (D_8014A118[D_801543D4].ref == 0) {
        D_8014A118[D_801543D4].ref = &D_80146150[D_8014A118[D_801543D4].index];
    }
    idx = (*D_8014A118[D_801543D4].ref)->data->base->unk6FC;
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            rec = &(*D_8014A118[D_801543D4].ref)->data->base->recs[idx];
        } else {
            rec = &D_80151618[idx];
        }
        rec->total++;
        if (D_80154450.unk1 == 0) {
            rec->count[0]++;
        } else if (D_80154450.unk1 == 1) {
            rec->count[1]++;
        } else if (D_80154450.unk1 == 2) {
            rec->count[2]++;
        }
        if (D_80142760) {
            rec->unkC++;
            if (D_80154450.unk1 < 3) {
                rec->unkE++;
            }
        }
        rec->sum += D_80154450.unk2;
        if (rec->best < D_80154450.unk2) {
            rec->best = D_80154450.unk2;
        }
        if (rec->bestTime == 0.0f || (*D_8014A118[D_801543D4].ref)->data->base->unk704 < rec->bestTime) {
            rec->bestTime = (*D_8014A118[D_801543D4].ref)->data->base->unk704;
        }
    }
    menu_dialog_close(D_8014A118[D_801543D4].ref, (*D_8014A118[D_801543D4].ref)->data->base->unk6FC);
}
