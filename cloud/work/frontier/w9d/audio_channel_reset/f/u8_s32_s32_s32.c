/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u32 unk4;
    /* 0x08 */ u32 unk8;
    /* 0x0C */ u16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ u16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ s8 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ u8 pad24[0x10];
    /* 0x34 */ u16 index;
} Sprite;

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 slot;
    /* 0x02 */ u8 pad2[0x4A];
} InputRec; /* 0x4C */

typedef struct {
    /* 0x000 */ u8 pad0[6];
    /* 0x006 */ s8 unk6;
    /* 0x007 */ u8 pad7[0x2FD];
} PlayerSlot; /* 0x304 */

extern s16 D_8014A108;
extern InputRec D_8014A118[];
extern PlayerSlot D_80144030[];

void Input_ApplyPadConfig(Sprite *spr);

s32 audio_channel_reset(Sprite *spr)
{
    s32 i;
    s32 flag;
    u8 slot;
    s32 v;

    flag = 1;
    for (i = 0; i < D_8014A108; i++) {
        slot = D_8014A118[i].slot;
        v = D_80144030[slot].unk6;
        if (v == 0) {
            flag = 0;
        }
    }
    if (flag != spr->unk1A) {
        spr->unk1A = flag;
        Input_ApplyPadConfig(spr);
    }
    return 1;
}
