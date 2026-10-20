/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800D6530 (w21g): post one four-vector packet per active player and set the vsync blend.
 * Unless D_801174B4 bit 3 is set: for each player record D_8014A118[0..D_8014A108) (0x4C bytes)
 *   message (+0x44) = func_800D63EC(&D_801141B0, &D_801141B0, &D_801141A4, &D_80114198), except
 *   in mode D_8014A110 == 2 for records whose byte +0 is set, which get -1;
 * then speed_set((f32)D_80146115 / 10.0f, 0.1f, 0, 1) (the same blend vsync_wait computes).
 * speed_set is internal (IPA: blend/amount in f20/f22, the flags in s1/s2), so this function only
 * installs in the codex_vsync_a145 group. Retail's pointer loop with a recomputed end is IDO's
 * test replacement of the index loop below (as in audio_channel_reset/assign_drones).
 * No shaping devices. Own .rodata: 0.1f at 0x801241AC (0x3DCCCCCD).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;

typedef struct Vec3 { f32 x, y, z; } Vec3;
typedef struct Packet Packet;
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 pad1[0x43];
    /* 0x44 */ s32 message;
    /* 0x48 */ u8 pad48[4];
} PlayerRec; /* 0x4C */

extern PlayerRec D_8014A118[];
extern s32 D_801174B4;
extern s32 D_8014A110;
extern s16 D_8014A108;
extern Vec3 D_801141B0, D_801141A4, D_80114198;
extern s8 D_80146115;

Packet *func_800D63EC(Vec3 *a, Vec3 *b, Vec3 *c, Vec3 *d);
void speed_set(f32 blend, f32 amount, s32 first, s32 second);

void func_800D6530(void)
{
    s32 i;

    if (!(D_801174B4 & 8)) {
        for (i = 0; i < D_8014A108; i++) {
            if (D_8014A110 != 2 || D_8014A118[i].unk0 == 0)
                D_8014A118[i].message = (s32)func_800D63EC(&D_801141B0, &D_801141B0, &D_801141A4, &D_80114198);
            else D_8014A118[i].message = -1;
        }
        speed_set((f32)D_80146115 / 10.0f, 0.1f, 0, 1);
    }
}
