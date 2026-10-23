typedef signed char s8; typedef int s32; typedef unsigned int u32;
extern s8 gOvlB;
#define gFlagA (*(s8 *)&gOvlB)
extern s32 gQ[];
void lock(void *q, s32 a, s32 b);
void unlockq(void *q, s32 a, s32 b);
void freeit(u32 a, s32 b);
void loadit(s8 *p, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
static s32 sFlag = 0;
static void f(s32 load)
{
    static s32 loaded = 0;
    if (load) {
        if (!loaded) {
            if (gFlagA) {
                lock(gQ, 0, 1);
                freeit(0x8038A400, 0);
                unlockq(gQ, 0, 0);
                gFlagA = 0;
            }
            loaded = 1;
        }
    } else if (loaded) {
        loaded = 0;
    }
}
void g1(void) { f(1); }
void g2(void) { f(0); }
void g3(void) { f(1); f(0); f(1); }
