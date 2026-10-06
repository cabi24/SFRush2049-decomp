typedef float f32;

extern int D_8011735C;

static unsigned short rand15(void)
{
    D_8011735C = D_8011735C * 1103515245 + 12345;
    return (D_8011735C >> 16) & 0x7fff;
}

f32 func_8008B2E4(f32 range)
{
    return rand15() * range / 32768.0f;
}
