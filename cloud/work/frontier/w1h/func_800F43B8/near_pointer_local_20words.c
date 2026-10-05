/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
    Player76 *p;

    p = &D_8014A118[D_801543D4];
    if (p->ref == 0) {
        p->ref = &D_80146150[p->index];
    }
    idx = (*p->ref)->data->base->unk6FC;
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
