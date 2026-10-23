typedef signed char s8; typedef int s32; typedef unsigned int u32;
extern s32 gQ[];
void lock(void *q, s32 a, s32 b);
void unlockq(void *q, s32 a, s32 b);
void freeit(u32 a, s32 b);
void loadit(s8 *loaded);
static void ovlB(s32 load)
{
    static s8 sFlagB = 0;
    if (load) {
        loadit(&sFlagB);
    } else if (sFlagB) {
        lock(gQ, 0, 1);
        freeit(0x8038A400, 0);
        unlockq(gQ, 0, 0);
        sFlagB = 0;
    }
}
void InitB(void) { ovlB(1); }
void unloadB(void) { ovlB(0); }
