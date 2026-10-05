/*
 * Resource-slot views and the sound-bank refresh (frontier wave 1, w1a). Real bodies only, no stand-ins.
 *
 * D_80156D38[]: 0x14-byte resource slots { u8 ?, s8 state, u8 active, ..., void *data @0xC, ... }.
 * func_80096288(index, 0, 0): empty debug/validation hook (retail: `beqz a2,+8; nop; jr ra; nop`), called
 *   by jal from 11 functions. Body = the locked src/blob/func_80096288.c text plus a dead `if (0) switch`
 *   so that umerge does not inline it (plan M4 "inline blocker"); it must be INTERNAL (not in keep): its
 *   callers then keep the index in a3 across the call. Both additions are compile-affecting quirks.
 * slot_value_get(index): returns slot->data. Two statements, so umerge inlines it into
 *   sound_update_channel (which is why that function has its own jal to func_80096288 and copies the
 *   index to a3).
 * display_list_alloc(index) / slot_deactivate(index): audio_loop_control(slot->data, 0 / 1) (lock-count
 *   down / up on the heap block), then slot->active = 1 / 0. The store is written `do { } while (0)`
 *   (macro-shaped): that keeps it out of the `jr ra` delay slot. Quirk, may not be the original spelling.
 * player_state_get(index): returns slot->state.
 * sound_update_channel(force): internal, `force` arrives in t0. Refreshes the cached bank
 *   D_801497F0 = *(Bank **)slot_value_get(D_80149780) when forced or changed: entry table D_80149800 =
 *   bank + 16, per-entry data pointers after the table, id -> entry index map D_80149878[256]; when forced
 *   also D_80149820[i] = D_80151AE8[D_801497A4].first + i * 36 and the two mode bytes. One `u8 *p` carries
 *   the bank pointer, the table and the running data pointer (retail reuses a1 the same way); the arrays
 *   are indexed, not walked by pointer (pointer walks force reloads of D_801497F0).
 * mode_byte2_set / object_type_byte2_get / object_type_byte3_get / mode_byte_set: already locked
 *   (src/blob/groups/codex_sound_channel), unchanged, real callers of sound_update_channel.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct ResSlot {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ s8 state;
    /* 0x02 */ u8 active;
    /* 0x03 */ u8 pad3[9];
    /* 0x0C */ void *data;
    /* 0x10 */ s32 unk10;
} ResSlot; /* 0x14 */

extern ResSlot D_80156D38[];
void audio_loop_control(void *ptr, s32 dec);

extern s32 D_80149B08;
void func_80096288(s32 arg0, s32 arg1, s32 arg2)
{
  s32 t;
  if (arg0) {}
  if (arg1) {}
  t = !arg2;
  if (arg2 != 0)
  {
    if (t) {}
    if (t) {}
  }
  if (t) {}
  if (0) { switch (arg0 + arg1) { case 1: D_80149B08 = 1; break; case 2: D_80149B08 = 2; break; case 3: D_80149B08 = 3; break; } }
}

void *slot_value_get(s32 index)
{
    func_80096288(index, 0, 0);
    return D_80156D38[index].data;
}

void display_list_alloc(s32 index)
{
    ResSlot *slot;

    func_80096288(index, 0, 0);
    slot = &D_80156D38[index];
    audio_loop_control(slot->data, 0);
    do { slot->active = 1; } while (0);
}

s8 player_state_get(s32 index)
{
    func_80096288(index, 0, 0);
    return D_80156D38[index].state;
}

void slot_deactivate(s32 index)
{
    ResSlot *slot;

    func_80096288(index, 0, 0);
    slot = &D_80156D38[index];
    audio_loop_control(slot->data, 1);
    do { slot->active = 0; } while (0);
}

typedef struct Bank {
    /* 0x0 */ u8 pad0;
    /* 0x1 */ u8 mode;
    /* 0x2 */ u8 f2;
    /* 0x3 */ u8 f3;
    /* 0x4 */ u8 f4;
    /* 0x5 */ u8 pad5;
    /* 0x6 */ u8 f6;
    /* 0x7 */ u8 f7;
    /* 0x8 */ u8 f8;
    /* 0x9 */ s8 f9;
    /* 0xA */ u8 f10;
    /* 0xB */ u8 f11;
    /* 0xC */ u8 f12;
} Bank;

typedef struct { u8 *ptr; u8 id; u8 pad[7]; } Entry;
typedef struct { s32 first; s32 second; } Pair;

extern Bank *D_801497F0;
extern s32 D_80149780;
extern s32 D_801497A4;
extern Entry *D_80149800;
extern s16 D_80149878[256];
extern s32 D_80149820[];
extern Pair D_80151AE8[];
extern u8 D_80149B60;
extern s8 D_80149B70;
extern s32 D_80149B28;

void sound_update_channel(s32 force)
{
    u8 *p;
    s32 i;

    p = *(u8 **)slot_value_get(D_80149780);
    if (force || p != (u8 *)D_801497F0) {
        D_801497F0 = (Bank *)p;
        p += 16;
        D_80149800 = (Entry *)p;
        p += D_801497F0->f12 * 12;
        if (D_801497F0->f10 != 0) {
            for (i = 0; i < D_801497F0->f12; i++) {
                D_80149800[i].ptr = p;
                p += D_801497F0->f12;
            }
        }
        for (i = 0; i < 256; i++) {
            D_80149878[i] = -1;
        }
        for (i = 0; i < D_801497F0->f12; i++) {
            D_80149878[D_80149800[i].id] = i;
        }
    }
    if (force) {
        for (i = 0; i < D_801497F0->f11; i++) {
            D_80149820[i] = D_80151AE8[D_801497A4].first + i * 36;
        }
        D_80149B60 = D_801497F0->f4;
        D_80149B70 = D_801497F0->f8;
    }
    D_80149B08 = (D_801497F0->mode == 1) ? 4 : 3;
    D_80149B28 = (D_801497F0->mode == 1) ? 0 : 1;
}

void mode_byte2_set(s16 a) {
 if (a < 0) { sound_update_channel(0); D_80149B60 = D_801497F0->f4; }
 else D_80149B60 = a;
}
u8 object_type_byte2_get(void) { sound_update_channel(0); return *((u8 *)D_801497F0 + 2); }
u8 object_type_byte3_get(void) { sound_update_channel(0); return *((u8 *)D_801497F0 + 3); }

void mode_byte_set(s16 a) { if (a < 0) { sound_update_channel(0); D_80149B70=D_801497F0->f8; } else D_80149B70=a; }

/* flags: -g0 -O3 -mips2 -G 0 -non_shared */

typedef struct FontHdr {
    /* 0x0 */ u8 pad0;
    /* 0x1 */ u8 mode;
    /* 0x2 */ u8 f2;
    /* 0x3 */ u8 f3;
    /* 0x4 */ u8 f4;
    /* 0x5 */ u8 pad5;
    /* 0x6 */ u8 fixedWidth;
    /* 0x7 */ u8 spaceWidth;
    /* 0x8 */ u8 f8;
    /* 0x9 */ u8 monospace;
    /* 0xA */ u8 kerning;
    /* 0xB */ u8 f11;
    /* 0xC */ u8 count;
} FontHdr;

typedef struct Glyph {
    /* 0x0 */ u8 *kern;
    /* 0x4 */ u8 id;
    /* 0x5 */ u8 pad5;
    /* 0x6 */ u8 left;
    /* 0x7 */ u8 pad7;
    /* 0x8 */ u8 right;
    /* 0x9 */ u8 pad9[3];
} Glyph;

#define D_801497F0 (*(FontHdr **)&D_801497F0)
#define D_80149800 (*(Glyph **)&D_80149800)


s32 object_manager_update(u8 *str, s16 maxlen)
{
    s32 width;
    s32 pos;
    s32 wide;
    s32 ch;
    s32 step;
    s32 prev;
    u8 *s;

    sound_update_channel(0);
    pos = 0;
    width = 0;
    if (str[0] == 255) {
        s = str + 1;
        wide = 1;
        step = 2;
    } else {
        s = str;
        wide = 0;
        step = 1;
    }
    str = 0;
    prev = -1;
    while ((maxlen < 0 || (s32) str < maxlen) && (s[pos] != 0 || (wide && s[pos + 1] != 0))) {
        if (D_801497F0->monospace) {
            width += D_801497F0->fixedWidth + D_80149B70;
            prev = ch;
        } else {
            if (wide) {
                ch = (s[pos] << 8) | s[pos + 1];
            } else {
                if (width < D_80149B70) {}
                ch = s[pos];
            }
            if (ch == 32 || ch >= 256) {
                width += D_801497F0->spaceWidth;
                ch = -1;
                prev = -1;
            } else {
                if (ch == 10) {
                    break;
                }
                ch = D_80149878[ch];
                if (ch < 0) {
                    width += D_801497F0->spaceWidth;
                    prev = ch;
                } else {
                    width += D_80149800[ch].right - D_80149800[ch].left + D_80149B70 + 1;
                    if (prev > 0 && D_801497F0->kerning) {
                        width -= D_80149800[ch].kern[prev];
                    }
                    prev = ch;
                }
            }
        }
        pos += step;
        str++;
    }
    return width - D_80149B70;
}
