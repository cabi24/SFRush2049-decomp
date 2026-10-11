typedef float f32;
extern int D_8011735C;
extern f32 G[64];
int func_8008B2B4(void) {
    D_8011735C = D_8011735C * 1103515245 + 12345;
    return (D_8011735C >> 16) & 0x7fff;
}
void probe(void){ G[1]=(f32)func_8008B2B4(); G[2]=(f32)func_8008B2B4(); }
