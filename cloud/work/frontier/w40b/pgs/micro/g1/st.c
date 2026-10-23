typedef signed char s8; typedef int s32; typedef unsigned int u32;
void unloadB(void);
static void f(s32 load)
{
    static s32 loaded = 0;
    if (load) {
        if (!loaded) {
            unloadB();
            loaded = 1;
        }
    } else if (loaded) {
        loaded = 0;
    }
}
void g1(void) { f(1); }
void g2(void) { f(0); }
void g3(void) { f(1); f(0); f(1); }
