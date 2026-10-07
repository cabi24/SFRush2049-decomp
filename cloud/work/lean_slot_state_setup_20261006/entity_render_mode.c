/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * entity_render_mode(index): after func_80096288(index, 1, 1) (slot validation hook), if resource slot
 * D_80156D38[index] is loaded (s8 +3) and its cached mode byte (+4) differs from (D_80140A04 != 0), walk the
 * slot's model list D_801161F4[index] = { Model *models; s32 count; } (88-byte models, four 16-byte parts at
 * +0x18 with a display list at +8) and run display_list_traverse(dl, 0, 0, 2, func_8008AD6C) on every
 * non-null part, then store the new mode byte (D_80140A04 is re-read: the calls kill it). N64-only; no arcade
 * ancestor found.
 *
 * Must be in the unit with func_80096288 internal (slot_sound group): IPA keeps `index` in a3 across the call.
 * Shaping: the slot is indexed directly (D_80156D38[index].x) -- a `ResSlot *slot` local adds a named
 * 4-byte slot and moves the spill of the CSE'd address from 68(sp) to 64(sp).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

typedef struct ResSlot {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ s8 state;
    /* 0x02 */ u8 active;
    /* 0x03 */ s8 loaded;
    /* 0x04 */ u8 mode;
    /* 0x05 */ u8 pad5[7];
    /* 0x0C */ void *data;
    /* 0x10 */ s32 unk10;
} ResSlot;

typedef struct { u8 pad0[8]; u32 *dl; u8 padC[4]; } Part;
typedef struct { u8 pad0[0x18]; Part part[4]; } Model;
typedef struct { Model *models; s32 count; } ModelList;

extern ResSlot D_80156D38[];
extern ModelList D_801161F4[];
extern s8 D_80140A04;

void func_80096288(s32 index, s32 a, s32 b);
void display_list_traverse(u32 *dl, u32 match, u32 replace, s32 flags, s32 (*cb)(u32 *));
s32 func_8008AD6C(u32 *);

void entity_render_mode(s32 index)
{
    Model *m;
    s32 i;
    s32 j;
    s32 count;

    func_80096288(index, 1, 1);
    if (D_80156D38[index].loaded) {
        if (D_80156D38[index].mode != (D_80140A04 != 0)) {
            m = D_801161F4[index].models;
            count = D_801161F4[index].count;
            for (i = 0; i < count; i++) {
                for (j = 0; j < 4; j++) {
                    if (m->part[j].dl) {
                        display_list_traverse(m->part[j].dl, 0, 0, 2, func_8008AD6C);
                    }
                }
                m++;
            }
            D_80156D38[index].mode = (D_80140A04 != 0);
        }
    }
}
