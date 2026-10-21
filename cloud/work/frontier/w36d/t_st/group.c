typedef signed char s8; typedef int s32; typedef float f32;
typedef struct S {s32 h; f32 a[5]; f32 m[9]; f32 p[3];} S;
extern s8 g;
static S t[20];
static f32 mp[3] = {1.0f,2.0f,3.0f};
extern s32 f(s32);
extern s32 h(s32);
void inner(void)
{
    s32 save;
    save = g;
    for (g = 14; g <= 17; g++) {
        if (f(g) && t[g].h == -1) {
            h(g);
            t[g].p[0] = mp[0];
            t[g].p[1] = mp[1];
            t[g].p[2] = mp[2];
        }
    }
    g = save;
}
void root(void) { inner(); }
