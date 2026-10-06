/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * NONMATCH research: runtime A 0x8038A634..0x8038A820 (492 bytes).
 * Snapshot a three-byte palette colour, snap or approach each output channel
 * by one tenth of the signed difference, derive alpha from the signed mode
 * byte, and update the existing BLIT's depth, alpha and image pointer.
 * Names describe accessed behavior; no exact arcade donor is established.
 * The Blit declaration is only the accessed prefix, not a full-size claim.
 *
 * Natural standalone O3: 41/123 differing words, improved from 113/123 in the
 * first array-loop draft. O2 gives the same residual. The first 38 words match;
 * later temporary-register and integer-division scheduling differ. Real
 * callers 0x8038DFEC and 0x8038E5DC may supply missing whole-program context.
 * No fake callers, unused locals, dummy reads, volatile shaping or assembly.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef struct Blit {
    const char *Name;
    void *Image;
    void *Info;
    u16 TexIndex;
    s16 X, Y;
    u16 Z;
    s16 Width, Height;
    u8 Alpha, Flip;
    s8 Hide, Init;
} Blit;
extern s8 D_801427A0;
extern s8 D_8014978C;
extern s8 D_8013F1D8;
typedef struct RGB { u8 r, g, b; } RGB;
extern RGB D_80114658[];
extern u8 D_803BA1E0[8];
extern u8 D_803BA1E8[];
extern u8 *D_803BA1FC;
extern Blit *D_803B828C;
extern void Input_ApplyPadConfig(Blit *);

void func_8038A634(s32 initialize)
{
    u8 color[3];
    if (D_801427A0 < 19)
        D_801427A0 = D_8014978C;
    color[0] = D_80114658[D_801427A0].r;
    color[1] = D_80114658[D_801427A0].g;
    color[2] = D_80114658[D_801427A0].b;
    if (initialize) {
        D_803BA1E0[0] = color[0];
        D_803BA1E0[1] = color[1];
        D_803BA1E0[2] = color[2];
    } else {
        D_803BA1E0[0] += (color[0] - D_803BA1E0[0]) / 10;
        D_803BA1E0[1] += (color[1] - D_803BA1E0[1]) / 10;
        D_803BA1E0[2] += (color[2] - D_803BA1E0[2]) / 10;
    }
    D_803BA1E0[3] = D_8013F1D8 * 192 / 3;
    if (D_803B828C) {
        D_803BA1FC = D_803BA1E0;
        D_803BA1E0[4] = 0;
        D_803BA1E0[5] = 0;
        D_803BA1E0[6] = 0;
        D_803BA1E0[7] = 0;
        D_803B828C->Z = 0x7E00;
        D_803B828C->Alpha = D_803BA1E0[3];
        D_803B828C->Image = D_803BA1E8;
        Input_ApplyPadConfig(D_803B828C);
    }
}
