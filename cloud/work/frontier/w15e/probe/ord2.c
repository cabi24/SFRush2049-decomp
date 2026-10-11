typedef float f32;
extern f32 G0[3], G1[3], G2[3];
extern f32 OUT[3];
extern void sink(f32 *);
typedef struct { f32 x, y, z; } V3;
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
    f32 *p = pos;
    p[0] = G0[0];
    p[0] += (s - 2.5f) * G1[0];
    OUT[0] = p[0];
}
void pc(f32 s)
{
    V3 pos;
    pos.x = G0[0];
    pos.x += (s - 2.5f) * G1[0];
    OUT[0] = pos.x;
}
void pd(f32 s)
{
    f32 pos[3];
    pos[0] = G0[0];
    pos[0] += (s - 2.5f) * G1[0];
    sink(pos);
}
void pe(f32 s, int i)
{
    f32 pos[3];
    pos[i] = G0[0];
    pos[0] += (s - 2.5f) * G1[0];
    OUT[0] = pos[0];
}
