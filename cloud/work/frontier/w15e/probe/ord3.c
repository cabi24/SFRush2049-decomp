typedef float f32;
extern f32 G0[3], G1[3];
extern f32 OUT[3];
void pa(f32 s)
{
    f32 pos[3];
    pos[0] = G0[0];
    pos[0] += (s - 2.5f) * G1[0];
    OUT[0] = pos[0];
}
void pb(f32 s)
{
    f32 pos[3];
    pos[0] = G0[0];
    pos[0] = pos[0] + (s - 2.5f) * G1[0];
    OUT[0] = pos[0];
}
void pc(f32 s)
{
    f32 pos[3];
    pos[0] = G0[0];
    pos[0] = (s - 2.5f) * G1[0] + pos[0];
    OUT[0] = pos[0];
}
