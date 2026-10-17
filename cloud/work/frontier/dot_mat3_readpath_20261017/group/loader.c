/* MAT3 ReadPathData source/dataflow translated to native N64 endian/layout.
 * All six original donor locals participate in parsing or conversion loops.
 * Native instructions retain the conversion-loop counters but no swaps or
 * field accesses. These loops are evidenced by the native body, not inserted
 * allocator padding. The called timing helper remains unmatched. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef s16 Vertex[3];
typedef struct Alternate {u8 type; s8 firstPath;u16 firstIdx;s8 lastPath;u8 pad5;u16 lastIdx;s8 checkpoint;u8 pad9;u16 numPts;Vertex *pts;} Alternate;
typedef struct ResPath {u16 numPts;u16 pad2;Vertex *pts;u8 numAlt;u8 pad9[3];Alternate *alt;} ResPath;
typedef struct DronePoint {s16 pos[3];u8 mph,pad7;} DronePoint;
typedef struct DronePath {u16 numPts;u16 pad2;DronePoint *pts;} DronePath;
extern s8 D_80114650;
extern u8 *D_80153F20;
extern ResPath D_801407F0;
extern Alternate *D_80153E80;
extern u16 D_801527A4;
extern Vertex *D_801409E8;
extern DronePath D_8012E5E8[4];
extern s16 D_801527D0[4];
extern DronePoint *D_80153EF8[4];
void physics_response(void);
void audio_mixer_main(s16);
void physics_friction_apply(void);
void func_800B9740(void);
void *memcpy(void *,const void *,u32);
void audio_effect_apply(void)
{
    s16 i;
    s16 p;
    ResPath *tResPath;
    DronePath *tDronePath;
    char *ptr;
    s16 j;
    if (D_80114650) {
        physics_response(); audio_mixer_main(-1); physics_friction_apply();
        func_800B9740(); physics_response(); return;
    }
    ptr=(char *)D_80153F20+812;
    tResPath=(ResPath *)ptr;
    memcpy(&D_801407F0,tResPath,sizeof(D_801407F0));
    ptr+=sizeof(D_801407F0);
    D_80153E80=(Alternate *)ptr;
    ptr+=D_801407F0.numAlt*sizeof(*D_80153E80);
    for(i=0;i<D_801407F0.numAlt;++i) {
    }
    memcpy(&D_801527A4,ptr,sizeof(D_801527A4)); ptr+=sizeof(D_801527A4);
    D_801409E8=(Vertex *)ptr;
    ptr+=D_801407F0.numPts*sizeof(*D_801409E8);
    for(i=0;i<D_801527A4;++i)
        for(j=0;j<3;++j) { /* Native target retains this conversion loop, without field accesses. */ }
    D_801407F0.pts=D_801409E8;
    if(D_801407F0.numAlt > 0) D_801407F0.alt=D_80153E80;
    else D_801407F0.alt=D_80153E80=0;
    for(i=0;i<D_801407F0.numAlt;++i) {
        D_80153E80[i].pts=(Vertex *)ptr;
        ptr+=D_80153E80[i].numPts*sizeof(*D_801409E8);
    }
    for(p=0;p<4;++p) {
        tDronePath=(DronePath *)ptr;
        memcpy(&D_8012E5E8[p],tDronePath,sizeof(D_8012E5E8[p])); ptr+=sizeof(D_8012E5E8[p]);
        D_801527D0[p]=D_8012E5E8[p].numPts;
        D_80153EF8[p]=(DronePoint *)ptr;
        D_8012E5E8[p].pts=(DronePoint *)ptr;
        ptr+=D_8012E5E8[p].numPts*sizeof(*D_80153EF8[p]);
        for(i=0;i<D_8012E5E8[p].numPts;++i)
            for(j=0;j<3;++j) { /* Native target retains this conversion loop, without field accesses. */ }
    }
    audio_mixer_main(-1); physics_friction_apply(); func_800B9740(); physics_response();
}
