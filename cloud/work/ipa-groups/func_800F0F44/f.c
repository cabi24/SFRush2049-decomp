typedef signed int s32;
typedef signed char s8;
typedef unsigned char u8;

typedef struct Q { u8 pad[16]; u8 id; u8 pad17; s8 name[14]; } Q;
typedef struct Obj { struct Obj *next; Q *q; s8 b8; u8 b9; s8 pos[14]; s32 w24; s32 w28; } Obj;
typedef struct Ref { s32 w0; s32 w4; Q **qq; s32 w12; s32 w16; s8 pos[4]; } Ref;
typedef struct Slot { u8 pad0; s8 b1; u8 pad2[3]; s8 b5; s8 b6; u8 rest[765]; } Slot;   /* 772 bytes */

extern Ref *D_8014A160;
extern s32 D_80146198[];        /* [idx] = matched list node */
extern s32 D_801461C0[];        /* [idx] = result code */
extern s8 D_80156CF0[][16];     /* [idx*16] active flag */
extern Slot D_80144030[];
extern s32 D_8014A110;
extern Obj **D_80152028;
extern s8 D_8014978C;
extern s8 D_80152570;
extern s32 func_800950AC(void *, void *, s32);
extern s32 func_8008AD04(void *, void *);

void func_800F0F44(s32 idx)
{
    Ref *r = D_8014A160;
    Slot *sl;
    Obj **n, *o;

    D_80146198[idx] = 0;
    if (D_80156CF0[idx][0] == 0) { D_801461C0[idx] = -4; return; }
    sl = &D_80144030[idx];
    if (sl->b1 == 0) { D_801461C0[idx] = -1; return; }
    if (sl->b5 != 0) { D_801461C0[idx] = -2; return; }
    if (sl->b6 != 0) { D_801461C0[idx] = -3; return; }
    D_801461C0[idx] = 0;
    if (D_8014A110 == 2) {
        for (n = D_80152028; n != 0; n = (Obj **)o->next) {
            o = (Obj *)*n;
            if (o->q == 0) continue;
            if (o->q->id != idx) continue;
            if (D_8014978C != o->b8) continue;
            if (D_80152570 != o->b9) continue;
            if (func_800950AC(o->q->name, (*r->qq)->name, 14) != 0) continue;
            D_80146198[idx] = (s32)n;
            if (func_8008AD04(o->pos, r->pos) == 0 && o->w24 == r->w12 && o->w28 == r->w16)
                D_801461C0[idx] = 1;
            else
                D_801461C0[idx] = 2;
        }
    }
}

extern s8 D_80148000[];
void caller_a(void) { s32 i = 0; func_800F0F44(i); if (D_80148000[0] == 4) func_800F0F44(i + 1); }
void caller_b(s8 *p) { func_800F0F44(*p); }
