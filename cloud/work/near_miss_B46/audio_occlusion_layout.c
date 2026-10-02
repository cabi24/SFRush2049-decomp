/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
typedef struct OSMesgQueue OSMesgQueue;
extern OSMesgQueue D_801497A8;
extern int osRecvMesg(OSMesgQueue *,void **,int);
extern int osJamMesg(OSMesgQueue *,void *,int);
typedef struct ResetData {
    int first;
    unsigned char gap0[4];
    int second;
    unsigned char gap1[8];
    f32 vector[4][2];
    int third[4];
    unsigned char gap2[16];
    int fourth[4];
} ResetData;
extern ResetData D_80156944;
extern int D_80149784;
extern int D_80149B10[4],D_80143A00[4];
void audio_occlusion(void) {
    int i;
    osRecvMesg(&D_801497A8,0,1);
    D_80156944.first=0;
    D_80149784=0;
    D_80156944.second=0;
    for(i=0;i<4;i++) {
        D_80156944.vector[i][0]=D_80156944.vector[i][1]=0.0f;
        D_80149B10[i]=0;
        D_80143A00[i]=0;
        D_80156944.third[i]=0;
        D_80156944.fourth[i]=0;
    }
    osJamMesg(&D_801497A8,0,0);
}
