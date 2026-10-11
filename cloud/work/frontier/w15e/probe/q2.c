typedef float f32;
extern int D_8011735C;
extern f32 G[64];
int func_8008B2B4(void) {
    D_8011735C = D_8011735C * 1103515245 + 12345;
    return (D_8011735C >> 16) & 0x7fff;
}
f32 h1(f32 a){ return a*G[9]; }
void probe(void){ G[1]=h1(G[3]); G[2]=h1(G[4]); }
