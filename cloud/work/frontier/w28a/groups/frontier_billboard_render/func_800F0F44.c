/* Genuine slot-status walk, still NONMATCH. Both double-pointer loads are
 * witnessed in retail. The 2U spelling on the mode comparison pools the native
 * constant-2 web and gives the s8 index input required by both actual callers;
 * its literal value and equality result are unchanged for every s32 value.
 * No declaration of the shared global was changed. Original spelling unknown.
 * The explicit value local represents the three actual status-byte reads.
 * Native 72-byte frame is not reproduced; no padding was added to fit it.
 */
typedef signed int s32;
typedef signed char s8;
typedef unsigned char u8;

typedef struct Q { u8 pad[16]; u8 id; u8 pad17; s8 name[14]; } Q;
typedef struct Obj { struct Obj **next; Q **q; s8 b8; u8 b9; s8 pos[14]; s32 w24; s32 w28; } Obj;
typedef struct Ref { s32 w0; s32 w4; Q **qq; s32 w12; s32 w16; s8 pos[4]; } Ref;
typedef struct Slot { u8 pad0; s8 b1; u8 pad2[3]; s8 b5; s8 b6; u8 rest[765]; } Slot;   /* 772 bytes */

extern void *D_8014A160;
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
    Ref *r = *(Ref **)D_8014A160;
    Slot *sl;
    s32 value;
    Obj **n, *o;

    D_80146198[idx] = 0;
    if (D_80156CF0[idx][0] == 0) { D_801461C0[idx] = -4; return; }
    sl = &D_80144030[idx];
    value = sl->b1;
    if (value == 0) { D_801461C0[idx] = -1; return; }
    value = sl->b5;
    if (value != 0) { D_801461C0[idx] = -2; return; }
    value = sl->b6;
    if (value != 0) { D_801461C0[idx] = -3; return; }
    D_801461C0[idx] = 0;
    if (D_8014A110 == 2U) {
        for (n = D_80152028; n != 0; n = o->next) {
            o = (Obj *)*n;
            if (o->q == 0) continue;
            if ((*o->q)->id != idx) continue;
            if (D_8014978C != o->b8) continue;
            if (D_80152570 != o->b9) continue;
            if (func_800950AC((*o->q)->name, (*r->qq)->name, 14) != 0) continue;
            D_80146198[idx] = (s32)n;
            if (func_8008AD04(o->pos, r->pos) == 0 && o->w24 == r->w12 && o->w28 == r->w16)
                D_801461C0[idx] = 1;
            else
                D_801461C0[idx] = 2;
        }
    }
}
