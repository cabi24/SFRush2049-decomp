typedef signed int s32;
typedef struct {
    s32 ptr;
    s32 z;
} Pair;
extern s32 D_80155238;
extern s32 D_80155240;
extern s32 D_80155248[16];
Pair D_80155288;
extern s32 D_80152750[];
extern s32 D_8002E960;
extern s32 D_8002E928;
void *memcpy(void *, void *, unsigned);
void osJamMesg(void *, s32, s32);
void func_8010FBE0(void *src) {
    D_80155238 = 0;
    D_80155288.ptr = (s32)D_80152750;
    D_80155288.z = 0;
    D_80155240 = 2;
    memcpy(D_80155248, src, 64);
    osJamMesg(&D_8002E960, (s32)&D_80155238, 1);
    osJamMesg(&D_8002E928, 670, 1);
}
