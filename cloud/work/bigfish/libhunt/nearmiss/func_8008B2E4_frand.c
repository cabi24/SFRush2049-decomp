extern int D_8011735C;
float func_8008B2E4(float scale) {
    int r;
    D_8011735C = D_8011735C * 1103515245 + 12345;
    r = (D_8011735C >> 16) & 0x7fff;
    return (float)r * scale / 32768.0f;
}
