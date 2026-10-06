/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Image A only: [0x80393004, 0x803930B8), 180 native bytes.
 * Initialize two menu availability rows using the selected player's count.
 * The producer at A:803930B8 fills four counts: -4..-1 sentinels or a
 * nonnegative list count (assuming a terminating, nonoverflowing traversal).
 * The selector must name a valid initialized slot.
 * An unsigned bounded induction variable with a signed value comparison
 * preserves IDO's native integer loop rather than pointer induction. This
 * matching shape does not establish the original source declaration.
 * See cloud/work/frontier/dot_runtime_a_flags_20261006/README.md.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef int s32;
extern u8 D_803BA7E0[];
extern u8 D_803BA7F0[];
extern s32 D_803BA830[];
extern s8 D_803B65A4;

void func_80393004(void)
{
    unsigned int i;
    s32 count;

    D_803BA7E0[0] = 1;
    D_803BA7F0[0] = 1;
    D_803BA7E0[1] = 1;
    D_803BA7F0[1] = 1;
    D_803BA7E0[2] = 0;
    count = D_803BA830[D_803B65A4];
    D_803BA7F0[2] = count < 1;
    for (i = 0; i < 8; i++) {
        D_803BA7E0[i + 3] = (s32)i < count;
        D_803BA7F0[i + 3] = (s32)i < count;
    }
}
