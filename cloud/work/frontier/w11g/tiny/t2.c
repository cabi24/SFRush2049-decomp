typedef struct { int level[5]; float a, b; } Curve;
extern Curve D[4];
extern float out[4];
void f(int s, int i)
{
    int seg; int hi; int *lo; Curve *c = &D[i];
    lo = c->level;
    hi = lo[4];
    if (hi < s) s = hi;
    for (seg = 0; seg < 4; seg++, lo++) {
        if (lo[1] >= s) break;
    }
    out[i] = (float)(s - lo[0]) / (float)(lo[1] - lo[0]) + seg;
}
