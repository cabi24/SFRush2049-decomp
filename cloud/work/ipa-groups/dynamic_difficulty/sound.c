typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Bank {
    /* 0x0 */ u8 pad0;
    /* 0x1 */ u8 mode;
    /* 0x2 */ u8 pad2[2];
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

typedef struct { u8 *ptr; u8 pad4; u8 id; u8 pad[6]; } Entry;
typedef struct { Bank *bank; } Hdr;
typedef struct { Hdr *hdr; s32 pad[4]; } Tbl;
typedef struct { s32 first; s32 second; } Pair;

extern Bank *D_801497F0;
extern s32 D_80149780;
extern s32 D_801497A4;
extern Tbl D_80156D44[];
extern Entry *D_80149800;
extern s16 D_80149878[256];
extern s32 D_80149820[];
extern Pair D_80151AE8[];
extern u8 D_80149B60;
extern u8 D_80149B70;
extern s32 D_80149B08;
extern s32 D_80149B28;

void func_80096288(s32, s32, s32);
extern s32 func_80097694(s32, s32);

void sound_update_channel(s32 force)
{
    Bank *b;
    u8 *p;
    s32 i;
    s16 *q;
    s32 *r;
    s32 v;

    func_80096288(D_80149780, 0, 0);
    b = D_80156D44[D_80149780].hdr->bank;
    if (force || b != D_801497F0) {
        D_801497F0 = b;
        D_80149800 = (Entry *)((u8 *)b + 16);
        p = (u8 *)b + 16 + D_801497F0->f12 * 12;
        if (D_801497F0->f10 != 0) {
            for (i = 0; i < D_801497F0->f12; i++) {
                D_80149800[i].ptr = p;
                p += D_801497F0->f12;
            }
        }
        for (q = D_80149878; q < &D_80149878[256]; q++) {
            *q = -1;
        }
        for (i = 0; i < D_801497F0->f12; i++) {
            D_80149878[D_80149800[i].id] = i;
        }
    }
    if (force) {
        v = D_80151AE8[D_801497A4].first;
        r = D_80149820;
        for (i = 0; i < D_801497F0->f11; i++) {
            *r++ = v;
            v += 36;
        }
        D_80149B60 = D_801497F0->f4;
        D_80149B70 = D_801497F0->f8;
    }
    D_80149B08 = (D_801497F0->mode == 1) ? 4 : 3;
    D_80149B28 = (D_801497F0->mode == 1) ? 0 : 1;
}

