typedef float f32;
extern f32 G0[3], G1[3], G2[3], G3[3];
extern f32 OUT[3];
extern f32 TAB[];
extern unsigned char IDX;
void probe(f32 s, f32 *off)
{
    f32 pos[3];
    f32 sc;
    pos[0] = G0[0];
    pos[1] = G0[1];
    pos[2] = G0[2];
    pos[0] += (s - 2.5f) * G1[0];
    pos[1] += off[1] * G1[1];
    sc = TAB[IDX];
    pos[0] += sc * G2[0];
    pos[1] += sc * G2[1];
    pos[2] += G3[2] * 0.5f;
    pos[0] += G3[0] * 0.5f;
    OUT[0] = pos[0]; OUT[1] = pos[1]; OUT[2] = pos[2];
}
