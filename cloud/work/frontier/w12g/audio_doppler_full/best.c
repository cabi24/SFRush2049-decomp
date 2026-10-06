/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w12g NEAR-MISS (not a match): 39/238 words in the whole-program unit (w11d best 119), length 238,
 * frame 256, every s-register as retail. Needs the direct-return func_800B61A8
 * (cloud/work/frontier/w12g/func_800B61A8.c) as the unit's definition; with the locked new_var form
 * the same file scores worse (w11d natural 125 vs 119).
 * Changes against w11d best.c:
 *  - the texture word at 0x80154398 is spelled through three bases: &D_80154398 for the two
 *    address arguments, D_80154368+0x30 for the create-call load, D_80154394[2] for the two draw-call
 *    loads. Every texture address web then has tot <= 20, below quad and colour (tot 30 each), so
 *    quad (first appearance) takes s7 and &D_801140F4 takes s8, as retail. One or two spellings cannot
 *    do it: & plus any load in one web is >= 30 and wins the tie by first appearance.
 *  - two scalar locals declared between quad and name: name lands at sp+176 (retail), frame 256.
 * Residual (all 39 words): ugen temp ring +3 from the create call on. Retail allocates three more
 * GP temps between `li t5,1` and `sllv` (t9/t0/t1 for ours t6/t7/t8) and the offset holds to the end.
 * Ring is pure next-free (traced). 20+ spellings of that statement tried without movement. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    u8 pad0[0x24];
    f32 pos[3];
    f32 matrix[9];
    u8 pad54[152 - 0x54];
} View;
typedef struct {
    u16 pad0;
    u16 flags;
} Poly;

extern s16 D_80151AD0;
extern View D_80150B70[];
extern Poly *D_80154368[];
extern f32 D_801140F8[4][3];
extern u8 D_801140F4[];
extern u16 D_80154398;
extern u16 D_80154394[];
extern u16 D_801543A4;
extern s16 D_80152032;
extern volatile u8 D_80140BDC;
extern s8 D_8010FFC0;

f32 viGetTimeToDeadline();
void func_8008C544(f32 *in, f32 *out, f32 *m);
s32 func_800B24EC();
Poly *func_800A78BC(s32 count, f32 *vertices, u16 texture, u8 *color, u16 flags, s32 indexed);
void func_8008C074(Poly *poly, s32 count, f32 *vertices, u16 texture, u8 *color, u16 flags, s32 indexed);
void func_8008D0C0(Poly *poly);
u32 entity_flags_apply(u32 index, u32 other, u32 value, u8 mode);
int sprintf(char *, const char *, ...);

s32 func_800B61A8(s32 a0, s32 a1, s32 a2, u8 a3);

void audio_doppler_full(s32 show) {
    f32 quad[4][3];
    s32 player;
    s32 i;
    char name[24];
    f32 size;
    Poly *p;


    for (player = 0; player < D_80151AD0; player++) {
        if (show) {
            size = viGetTimeToDeadline() - 0.001f;
            size = (size - (s32)size) * 12.0f;
            if (size < 5.0f)
                size = 5.0f;
            for (i = 0; i < 4; i++) {
                D_801140F8[i][2] = size;
                func_8008C544(D_801140F8[i], quad[i], D_80150B70[player].matrix);
                quad[i][0] += D_80150B70[player].pos[0];
                quad[i][1] += D_80150B70[player].pos[1];
                quad[i][2] += D_80150B70[player].pos[2];
            }
            if (D_80154368[player] == 0) {
                func_800B24EC("CNTDWN3", &D_80154398, 0, (s8)(D_80140BDC - 1), 1);
                D_80154368[player] = func_800A78BC(4, quad[0], *(u16 *)((char *)D_80154368 + 0x30), D_801140F4, (1 << player) | 0x82C0, 1);
                D_801543A4 = 4;
            } else if (D_80152032 < D_801543A4) {
                if (player + 1 == D_80151AD0) {
                    sprintf(name, "CNTDWN%d", D_80152032);
                    func_800B24EC(name, &D_80154398, 0, (s8)(D_80140BDC - 1), 1);
                    D_801543A4 = D_80152032;
                    if (D_80152032 == 3) {
                        func_800B61A8(75, 0, 1, 0);
                    } else if (D_80152032 == 2) {
                        func_800B61A8(76, 0, 1, 0);
                    } else {
                        func_800B61A8(77, 0, 1, 0);
                    }
                }
                func_8008C074(D_80154368[player], 4, quad[0], D_80154394[2], D_801140F4, 0, 1);
                p = D_80154368[player];
                p->flags &= 0x7FFF;
            } else {
                func_8008C074(D_80154368[player], 4, quad[0], D_80154394[2], D_801140F4, 0, 1);
            }
        } else if (D_80154368[player] != 0) {
            func_8008D0C0(D_80154368[player]);
            D_80154368[player] = 0;
            D_80152032 = -1;
        }
    }
}
