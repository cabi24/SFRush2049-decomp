/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* N64 Random: arcade LIB/fmath.c Random supplies the real outer rand mask.
 * N64 uses the observed 32768.0f denominator instead of arcade 32767.0.
 * Compile beside the unchanged accepted ANSI rand body in rand.c. */
extern int func_8008B2B4(void);
float func_8008B2E4(float max) {
    float rannum;

    rannum = (((float)(func_8008B2B4() & 0x07FFF) * max) / 32768.0f);

    return(rannum);
}
