/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w39b probe: TB_GetFontHeight with a private copy of the HAL_text.c chain (extern font header). Not a port. */
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

extern ResSlot gWaveInstance[];
void audio_loop_control(void *ptr, s32 dec);

extern s32 D_80149B08;
void func_80096288(s32 arg0, s32 arg1, s32 arg2);
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
extern Pair gTexDefPtrs[];
extern u8 D_80149B60;
extern u8 D_80149B70;
extern s32 D_80149B28;


static void *SVG_t(s32 index)
{
    func_80096288(index, 0, 0);
    return gWaveInstance[index].data;
}
void SFD_t(s32 force)
{
    u8 *p;
    s32 i;

    p = *(u8 **)SVG_t(D_80149780);
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
            D_80149820[i] = gTexDefPtrs[D_801497A4].first + i * 36;
        }
        D_80149B60 = D_801497F0->f4;
        D_80149B70 = D_801497F0->f8;
    }
    D_80149B08 = (D_801497F0->mode == 1) ? 4 : 3;
    D_80149B28 = (D_801497F0->mode == 1) ? 0 : 1;
}


u8 HGA_t(void) { SFD_t(0); return *((u8 *)D_801497F0 + 2); }
u8 HGD_t(void) { SFD_t(0); return *((u8 *)D_801497F0 + 3); }
s16 object_bytes23_sum(void) { return HGA_t() + HGD_t(); }
void tbs_extra1(void) { SFD_t(1); }
void tbs_extra2(void) { SFD_t(0); }
