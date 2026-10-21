typedef signed char s8; typedef int s32; typedef float f32;
typedef struct S {s32 h; f32 a[5]; f32 m[9]; f32 p[3];} S;
extern s8 g;
extern S t[20];
extern f32 mp[3];
extern s32 f(s32);
extern s32 h(s32);
extern s32 out;
void inner(void)
{
    s32 i;
    for (i = 0; i < 4; i++) {
        h(i);
        t[g].p[0] = mp[0];
        t[g].p[1] = mp[1];
        out = g;
    }
}
void root(void) { inner(); }
