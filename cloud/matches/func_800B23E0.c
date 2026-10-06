/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Pick a random bit number 0..31 allowed by the mask D_80123418[index]
 * (rejection sampling; a zero mask never terminates). N64 code; the
 * random source is the arcade LIB/fmath.c Random(F32 max):
 *     rannum = ((F32)(rand() & 0x07FFF) * max) / 32767.0;   (N64: 32768.0f)
 * over libc rand() (seed D_8011735C, kept func_8008B2B4), i.e. the kept
 * func_8008B2E4 = Random, both inlined here by umerge in the unit.
 * blob_unit score is EQUAL with the plain call `bit = func_8008B2E4(32.0f);`
 * (cloud/work/frontier/w8c/func_800B23E0/v/a.c with rand_arc_unit.c).
 * Standalone, static copies of rand/Random reproduce it.
 *
 * Quirks: the arcade's redundant `& 0x07FFF` on rand()'s already-masked
 * result costs a temp (t0/t1 instead of t9/v0) and the named `rannum`
 * local; the mask is read in the loop condition (a named mask local is
 * 14 words off); the result is a u8.
 */
typedef float f32;
typedef unsigned char u8;
typedef unsigned int u32;

extern int D_8011735C;
extern u32 D_80123418[];

static int rand(void)
{
    D_8011735C = D_8011735C * 1103515245 + 12345;
    return (D_8011735C >> 16) & 0x7fff;
}

static f32 Random(f32 max)
{
    f32 rannum;

    rannum = ((f32)(rand() & 0x07FFF) * max) / 32768.0f;
    return rannum;
}

u8 func_800B23E0(u8 index)
{
    u8 bit;

    do {
        bit = Random(32.0f);
    } while (!(D_80123418[index] & (1 << bit)));
    return bit;
}
