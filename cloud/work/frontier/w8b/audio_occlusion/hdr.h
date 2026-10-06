/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef float f32;
typedef int s32;
typedef struct OSMesgQueue OSMesgQueue;
extern OSMesgQueue D_801497A8;
extern int osRecvMesg(OSMesgQueue *,void **,int);
extern int osJamMesg(OSMesgQueue *,void *,int);
extern s32 D_80156944,D_80149784,D_8015694C;
extern f32 D_80156958[4][2];
extern s32 D_80149B10[4],D_80143A00[4],D_80156978[4],D_80156998[4];
