/* Genuine N64 Random(max), retaining its locked float arithmetic and mask.
 * The original global libc rand body is supplied in rand.c, rather than an
 * anonymous private copy. Both complete helpers are independently scored.
 */
typedef float f32;

extern int func_8008B2B4(void);

f32 func_8008B2E4(f32 max)
{
    f32 rannum;

    rannum = ((f32)(func_8008B2B4() & 0x07FFF) * max) / 32768.0f;
    return rannum;
}
