typedef signed char s8; typedef int s32; typedef float f32;
typedef struct S {s32 h; f32 a[5]; f32 m[9]; f32 p[3];} S;
extern s8 g;
extern S t[20];
extern f32 mp[3];
extern char *names[];
extern s32 f(s32);
extern s32 h1(char *, s32);
extern s32 h2(s32, f32 *, s32, s32);
extern void su(f32 *, f32 *, f32);
extern void wp(f32, f32 *);
extern f32 unit[9];
void inner(void)
{
    s32 save;
    save = g;
    for (g = 14; g <= 17; g++) {
        if (f(g) && t[g].h == -1) {
            t[g].h = h2(h1(names[g], 0), t[g].m, -1, 0);
            su(unit, t[g].m, 1.0f);
            wp(-1.5707963705062866f, t[g].m);
            t[g].p[0] = mp[0];
            t[g].p[1] = mp[1];
            t[g].p[2] = mp[2];
        }
    }
    g = save;
}
void root(void) { inner(); }
