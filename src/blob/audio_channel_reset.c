/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * audio_channel_reset (0x80094FE8; historical label): for every active input record D_8014A118[0..
 * D_8014A108) (0x4C bytes, controller port at +1) check that the port's controller is present
 * (D_80144030[port].present, the 0x304-byte pak/controller records, see src/blob/check_mpath_save.c);
 * if the all-present flag differs from spr->unk1A, store it there and re-apply the pad configuration
 * (Input_ApplyPadConfig). Always returns 1.
 *
 * Shaping (w9d): the presence read is an inlined static helper returning the s8 flag. Written inline
 * (with or without named locals) the loop's webs tie and uopt colours the record pointer first
 * (15/41 words, register permutation only); the helper's return value raises the flag web's priority,
 * giving retail's v0/v1/a0 assignment. No stub for the helper is known.
 */
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
    /* 0x01 */ u8 port;
    /* 0x02 */ u8 pad2[0x4A];
} InputRec; /* 0x4C */

typedef struct {
    /* 0x000 */ u8 pad0[6];
    /* 0x006 */ s8 present;
    /* 0x007 */ u8 pad7[0x2FD];
} Controller; /* 0x304 */

extern s16 D_8014A108;
extern InputRec D_8014A118[];
extern Controller D_80144030[];

void Input_ApplyPadConfig(Sprite *spr);

static s8 controller_present(u8 port)
{
    return D_80144030[port].present;
}

s32 audio_channel_reset(Sprite *spr)
{
    s32 i;
    s32 flag;

    flag = 1;
    for (i = 0; i < D_8014A108; i++) {
        if (controller_present(D_8014A118[i].port) == 0) {
            flag = 0;
        }
    }
    if (flag != spr->unk1A) {
        spr->unk1A = flag;
        Input_ApplyPadConfig(spr);
    }
    return 1;
}
