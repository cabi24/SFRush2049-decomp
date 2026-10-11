typedef float f32;
extern int D_8011735C;
extern f32 G[64];
int func_8008B2B4(void) {
    D_8011735C = D_8011735C * 1103515245 + 12345;
    return (D_8011735C >> 16) & 0x7fff;
}
f32 h2(f32 a, f32 b){ f32 r; f32 s; r = a*G[9]; s = r+b; return s; }
void probe(void){ G[1]=h2(G[3],G[5]); G[2]=h2(G[4],G[6]); }
