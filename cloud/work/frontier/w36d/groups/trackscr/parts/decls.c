/* ---- context (w36e region): OptionValid, TrackValid, SetFogIntensity, unloadTrackScreen, clearTrackMap ---- */
extern s32 D_803B77D4[][21];   /* validOption */
extern s8 D_803B8314[19];      /* TrackValid static trackReady */
extern s32 D_803AF980;
/* SetFogIntensity: research body cloud/work/frontier/dot_runtime_a_color_smoothing_20261006/candidate.c */
struct Blit {const char *Name;void *Image;void *Info;u16 TexIndex;s16 X,Y;u16 Z;s16 Width,Height;u8 Alpha,Flip;s8 Hide,Init;
    u8 other28[24];u16 resource;};
typedef struct RGB { u8 r, g, b; } RGB;
extern s8 D_801427A0;
extern s8 D_8013F1D8;
extern RGB D_80114658[];
extern u8 D_803BA1E0[8];
extern u8 D_803BA1E8[];
extern u8 *D_803BA1FC;
extern Blit *D_803B8288,*D_803B828C;
extern void Input_ApplyPadConfig(Blit *);
/* BlowFog (inlined into _InitScreen); MAT3 static fogTileSize (anon_uls_ult_8) */
typedef struct TileSize {u16 uls,ult,lrs,lrt;} TileSize;
extern volatile s16 D_8002EB70;   /* gRenderInProgress */
extern s32 D_803B8284;            /* trackMapObj */
extern void sound_stop(Blit *);   /* RemoveBlit (ZOID_RemovePoly on N64: blits) */
extern void sound_call_minimal(s16);  /* ZOID_RemoveObject */
extern void ambient_sounds_clear(void);  /* removeBG */
extern void func_800A5B3C(void);  /* ZOID_Reset */
/* ---- context (w36e region): _UninitScreen, buildTrackMap, animTrackMap: option_parent research bodies ---- */
typedef struct MapLod {u16 texidx,flags;f32 lodRad;u32 *displayList;void *verts;} MapLod;   /* N64 16-byte model LOD */
typedef struct RomObjDef {char name[16];f32 radius;s16 unused,lodCount;MapLod lod[4];} RomObjDef;   /* 88 B */
typedef struct ModelBank {RomObjDef *romObjDefs;s32 count;} ModelBank;
typedef struct Slot72 {s32 handle;f32 offset_x,offset_y,angle,scale_from,scale_to;f32 matrix[3][3],position[3];} Slot72;   /* trackMap: anon_objIdx_cityX_48 */
extern Slot72 D_803B7CE4[20];
extern s8 D_803BAAE0;            /* trackMapsBuilt */
extern MapLod D_803BAB58[7];   /* original lod[0] of the seven map objects */
extern MapLod D_803BAAE8[7];   /* mapObj */
extern void *D_803BABC8[7],*D_803BABE8[7];
extern ModelBank D_801161F4[];   /* gRomObjDefPtrs */
extern char *D_803B7738[];       /* trackMapObjNames */
extern void func_800960D4(void *);  /* DeallocateMemory */
typedef struct ImageMeta {u8 other0[21];u8 flags;u8 other22[10];} ImageMeta;
typedef struct DisplayCommand {u32 w0,w1;} DisplayCommand;
extern ImageMeta D_80140BF0[];
extern char *D_803B7788[];
extern char D_803B9260[];
extern Color D_803B8328;
extern s32 D_803B7CD8;          /* kNumMapSects */
extern Gfx *D_803BAB50;            /* mapObj[6].displayList */
extern f32 D_803B8254[12],D_803BAC30[12],D_803B832C[3];
extern Blit *func_800B3704(char *,s16,s16,s16);
extern void func_80094EC8(Blit *);
#define DISPLAY_WORDS(p,a,b) do {DisplayCommand *command=(p)++; command->w0=(a);command->w1=(b);} while(0)
#define DISPLAY_TEXTURE(p,on) DISPLAY_WORDS(p,0xD7000000u|((on)<<1),0xFFFFFFFFu)
#define DISPLAY_VERTEX(p,v,n,first) DISPLAY_WORDS(p,0x01000000u|((n)<<12)|(((first)+(n))<<1),(u32)(v))
#define DISPLAY_TRIANGLES(p,a,b,c,d,e,f) DISPLAY_WORDS(p,0x06000000u|((a)*2<<16)|((b)*2<<8)|((c)*2),((d)*2<<16)|((e)*2<<8)|((f)*2))
extern f32 D_803B8350,D_803B8338[3],D_803B8344[3];
extern void func_800FD754(f32 (*)[3],f32 (*)[3],f32,f32,f32);   /* ScaleUVXYZ */
extern void gfx_setup_fc(f32,f32 (*)[3]);   /* WYawUV */
/* ---- context (w36e region): showTrackSelect stand-in (MAT3 port), animTrackMaps stand-in ---- */
extern s8 D_80152570;      /* gBackwards */
extern s8 D_80140A04;      /* gMirrorWaves */
extern f32 D_803B8290;     /* showTrackSelect static angle */
extern f32 D_803B9A90[3];  /* showTrackSelect static fogPos */
extern void math_utility(f32 *,f32 (*)[3]);          /* CopyUV */
extern void func_80090E9C(f32,f32 (*)[3]);           /* YawUV */
extern f32 D_803B8388[3];   /* animTrackMaps static mapPos */
/* ---- buildTrackMaps (MAT3 trackscr.c, GC 0x8008ad10) and its inlined statics ---- */
typedef struct Vtx_t {s16 ob[3];u16 flag;s16 tc[2];u8 cn[4];} Vtx_t;
typedef struct ResPath {u16 numPts;s16 (*pts)[3];} ResPath;
typedef struct Checkpoints {s16 startTime,loopPt,finishPt,startPt,numPoints;} Checkpoints;
extern s16 D_801407B4[3];          /* gPathMax */
extern s16 D_801407D4[3];          /* gPathMin */
extern ResPath D_801407F0;         /* gResPath */
extern Checkpoints D_80151CE8;     /* gCheckpoints */
extern void *D_80153F20;           /* gPathWave */
extern s32 D_803B7CDC;             /* pathWaveIdx */
extern Color D_803B8354;           /* buildRaceTrackMaps static map vertex colour {63,63,63,255} */
extern f32 D_803B8358[3],D_803B8364[3];  /* buildRaceTrackMaps static mapPos (x2) */
extern f32 D_803B8370[3];          /* buildStuntTrackMaps static mapPos */
extern f32 D_803B837C[3];          /* buildBattleTrackMaps static mapPos */
extern s32 audio_frame_sync(s32,s32,s32,s32,s32);  /* LoadWave */
extern void *slot_value_get(s32);  /* GetWaveInstanceAddr */
extern void audio_dsp_process(void);   /* ReadCheckpointData */
extern void audio_effect_apply(void);  /* ReadPathData */
extern void func_80096130(s32);    /* UnloadWaveInstance */
extern void *audio_dma_sync(s32,s32);  /* AllocateMemoryHeap */
extern s32 func_800BA61C(s16);     /* FindResPathIntersect */
extern f32 sqrtf(f32);
#pragma intrinsic (sqrtf)
#define GFX(p,a,b) do { Gfx *_g = (p)++; _g->words.w0 = (a); _g->words.w1 = (b); } while (0)
