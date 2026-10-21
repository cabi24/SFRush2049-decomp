typedef float f32;
extern void func_8008B32C(f32 *src, f32 *dst, f32 s);

void func_8038A408(f32 *p, f32 *q)
{
    f32 a = p[0] * q[0];
    f32 b = p[1] * q[1];
    f32 c = p[2] * q[2];
    q[3] = a + b + c;
    q[4] = (a - b) * c;
    func_8008B32C(p, q, a);
}
