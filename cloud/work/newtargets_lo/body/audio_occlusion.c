typedef struct { f32 a; f32 b; } FP;
extern s32 D_80156944;
extern s32 D_80149784;
extern s32 D_8015694C;
extern FP D_80156958[4];
extern s32 D_80149B10[4];
extern s32 D_80143A00[4];
extern s32 D_80156978[4];
extern s32 D_80156998[4];
extern u8 D_801497A8[];
s32 osRecvMesg(void *, void *, s32);
s32 osJamMesg(void *, void *, s32);
void audio_occlusion(void) {
    s32 i;
    s32 *p = (s32 *)(u32)&D_80156944;
    s32 *q = (s32 *)(u32)&D_80149784;
    osRecvMesg(D_801497A8, 0, 1);
    *p = 0;
    *q = 0;
    D_8015694C = 0;
    for (i = 0; i < 4; i++) {
        D_80156958[i].b = 0.0f;
        D_80156958[i].a = 0.0f;
        D_80149B10[i] = 0;
        D_80143A00[i] = 0;
        D_80156978[i] = 0;
        D_80156998[i] = 0;
    }
    osJamMesg(D_801497A8, 0, 0);
}
