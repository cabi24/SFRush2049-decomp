typedef float f32;
extern int D_8011735C;
extern f32 G[64];
int func_8008B2B4(void) {
    D_8011735C = D_8011735C * 1103515245 + 12345;
    return (D_8011735C >> 16) & 0x7fff;
}
void h3(void){ G[7]=G[8]; }
void probe(void){ h3(); h3(); }
