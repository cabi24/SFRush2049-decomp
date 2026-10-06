typedef struct { int level[5]; float a, b; } Curve;
extern Curve D[4];
extern float out[4];
void f(int s, int i)
{
    int seg; int hi; Curve *c = &D[i];
    hi = c->level[4];
    s = (s > hi) ? hi : s;
    for (seg = 0; seg < 4; seg++) {
        if (c->level[seg + 1] >= s) break;
    }
    out[i] = (float)(s - c->level[seg]) / (float)(c->level[seg + 1] - c->level[seg]) + seg;
}
