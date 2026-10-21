/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Random(max): arcade LIB/fmath.c
 *     F32 Random(F32 max) { F32 rannum;
 *         rannum = ( ((F32)(rand() & 0x07FFF)*max) / 32767.0); return(rannum); }
 * with the N64 divisor 32768.0f (0x47000000). rand() is the kept libc
 * rand func_8008B2B4 (seed D_8011735C), inlined by umerge in the unit:
 * blob_unit score is EQUAL with `func_8008B2B4() & 0x07FFF`
 * (cloud/work/frontier/w8c/func_800B23E0/rand_arc_unit.c); standalone a
 * static copy reproduces it.
 * Quirk: the redundant `& 0x07FFF` (rand already masks) is what shifts the
 * temp ring to t0/t1 and leaves the seed address in v1.
 * No jal callers: inlined into its callers (e.g. func_800B23E0).
 */
typedef float f32;

extern int D_8011735C;

static int rand(void)
{
    D_8011735C = D_8011735C * 1103515245 + 12345;
    return (D_8011735C >> 16) & 0x7fff;
}

f32 func_8008B2E4(f32 max)
{
    f32 rannum;

    rannum = ((f32)(rand() & 0x07FFF) * max) / 32768.0f;
    return rannum;
}
