/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* tire_compound_set = InitWaves (MAT3 GameSource/src/waves.c InitWaves, GC 0x8005B4B4) with the N64
 * _BGLoadWaveCheck (its deleted stub is func_800A5BB0) and zoid.c ZOID_Reset (func_800A5B3C) inlined. */
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
typedef void *OSMesg;
typedef struct OSMesgQueue { s32 pad[6]; } OSMesgQueue;
typedef struct Heap Heap;

typedef struct anon_loaded_ready_14 {
    signed char loaded;
    signed char ready;
    signed char locked;
    signed char model;
    unsigned char mirrored;
    signed char handle;
    unsigned char wave;
    unsigned int size;
    unsigned char *ptr;
    unsigned char *lastPtr;
} anon_loaded_ready_14;

extern anon_loaded_ready_14 D_80156D38[64];  /* gWaveInstance */
extern unsigned char D_80140BDC;            /* gNumModels */
extern s16 D_8002EB70;                      /* gRenderInProgress */
extern OSMesgQueue D_80035440;              /* background loader idle token */
extern s32 D_80156990, D_80156CE0, D_801613AC, D_80156BAC, D_801613B4;
extern s16 D_8015B254;                      /* ObjHead */
extern f32 D_801613B8;                      /* gGlobalCameraFacingScale */
extern u16 D_80140618;
extern s32 D_801406B8;
extern s32 D_8011EA18;
#define gWaveInstance D_80156D38
#define gNumModels D_80140BDC
#define gRenderInProgress D_8002EB70

s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
s32 osSendMesg(OSMesgQueue *, OSMesg, s32);
void func_800A5A40(void);                   /* ZOID_ResetWindows */
void func_800A5488(void);                   /* InitBlits */
void func_80096130(s32 idx);                /* UnloadWaveInstance */
void car_damage_visual(Heap *heap);
void func_800A5158(void);                   /* InitTextWave */

static int _BGLoadWaveCheck(int wait)
{
    OSMesg msg;
    if (osRecvMesg(&D_80035440, &msg, 0) == -1) {
        if (!wait) return 0;
        osRecvMesg(&D_80035440, &msg, 1);
    }
    osSendMesg(&D_80035440, 0, 1);
    return 1;
}

static void ZOID_Reset(void)
{
    D_80156990 = 0;
    D_80156CE0 = 0;
    D_801613AC = 0;
    D_80156BAC = 0;
    D_801613B4 = 0;
    D_8015B254 = -1;
    func_800A5A40();
    D_801613B8 = 1.0f;
    D_80140618 = 0;
    D_801406B8 = (s32) &D_8011EA18;
}

static void ClearMemory(void)
{
    car_damage_visual(0);
}

void tire_compound_set(void)
{
    int i;
    _BGLoadWaveCheck(1);
    while (*(volatile s16 *)&gRenderInProgress) {}
    ZOID_Reset(); func_800A5488();
    for (i = 0; i < 64; i++) {
        if (gWaveInstance[i].ptr && !gWaveInstance[i].locked) func_80096130(i);
    }
    gNumModels = 0;
    for (i = 0; i < 64; i++) if (gWaveInstance[i].ptr) gNumModels = i + 1;
    ClearMemory();
    func_800A5158();
}
