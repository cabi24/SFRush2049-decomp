typedef float f32;

extern int D_8011735C;

f32 func_8008B2E4(f32 range)
{
    D_8011735C = D_8011735C * 1103515245 + 12345;
    return (f32)((D_8011735C >> 16) & 0x7fff) * range / 32768.0f;
}
