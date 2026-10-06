/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef float f32;
typedef int s32;
typedef struct OSMesgQueue OSMesgQueue;
extern OSMesgQueue D_801497A8;
extern int osRecvMesg(OSMesgQueue *,void **,int);
extern int osJamMesg(OSMesgQueue *,void *,int);
extern s32 D_80156944,D_80149784,D_8015694C;
f32 D_80156958[4][2];
extern s32 D_80149B10[4],D_80143A00[4],D_80156978[4],D_80156998[4];
void audio_occlusion(void) {
    s32 i;
    osRecvMesg(&D_801497A8,0,1);
    D_8015694C = D_80156944 = D_80149784 = 0;
    for(i=0;i<4;i++) {
        D_80156958[i][0]=D_80156958[i][1]=0.0f;
        D_80149B10[i]=0;
        D_80143A00[i]=0;
        D_80156978[i]=0;
        D_80156998[i]=0;
    }
    osJamMesg(&D_801497A8,0,0);
}
