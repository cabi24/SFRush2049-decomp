typedef float f32;
extern int D_8011735C;
extern f32 G[64];
int func_8008B2B4(void) {
    D_8011735C = D_8011735C * 1103515245 + 12345;
    return (D_8011735C >> 16) & 0x7fff;
}
f32 func_8008B2E4(f32 max)
{
    f32 rannum;
    rannum = ((f32)(func_8008B2B4() & 0x07FFF) * max) / 32768.0f;
    return rannum;
}
void probe(void)
{
    G[1] = func_8008B2E4((f32)1);
}
