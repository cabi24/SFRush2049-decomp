/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* audio_frame_sync = LoadWave (MAT3 GameSource/src/waves.c LoadWave, GC 0x8005B648) with
 * _NewWaveInstance (GC 0x8005B9BC) inlined. N64: background load hand-off to the loader
 * thread (two message queues) instead of the GC/PS2 _BGLoadWaveCheck path. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
typedef void *OSMesg;
typedef struct OSMesgQueue { s32 pad[6]; } OSMesgQueue;

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
extern unsigned int D_8002E580[];           /* gWaveSizes */
extern OSMesgQueue D_80035440;              /* loader: ready queue */
extern OSMesgQueue D_80156BB0;              /* loader: request queue */
extern int D_801569AC;                      /* loader: wave instance to read */
#define gWaveInstance D_80156D38
#define gWaveSizes D_8002E580

s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
s32 osSendMesg(OSMesgQueue *, OSMesg, s32);
int func_80097694(int waveNum, signed char handle);          /* _FindWaveInstance */
void entity_render_mode(int idx);                            /* MirrorWaveInstance */
unsigned char *audio_task_complete(void *heap, unsigned int size); /* AllocateHandleHeap */
unsigned char *audio_dma_sync(void *heap, unsigned int size);      /* AllocateMemoryHeap */
unsigned int audio_buffer_sync(void *ptr);                   /* GetPtrSize */
void func_800972C4(int idx);                                 /* _SetWaveInstanceModel */
void func_80097164(int idx);                                 /* _ReadWave */

static int _NewWaveInstance(int waveNum, int handle, void *heap)
{
    anon_loaded_ready_14 *winst;
    int idx;
    for (idx = 0; idx < 64; idx++) {
        if (!gWaveInstance[idx].ptr) {
            winst = &gWaveInstance[idx];
            winst->loaded = 0; winst->ready = 0; winst->locked = 0; winst->mirrored = 0;
            winst->wave = waveNum; winst->handle = handle;
            if (handle) winst->ptr = audio_task_complete(heap, (gWaveSizes[waveNum] + 31) & ~31);
            else winst->ptr = audio_dma_sync(heap, (gWaveSizes[waveNum] + 31) & ~31);
            winst->size = audio_buffer_sync(winst->ptr);
            func_800972C4(idx);
            return idx;
        }
    }
}

static void _BGLoadWave(int idx)
{
    OSMesg msg;
    osRecvMesg(&D_80035440, &msg, 1);
    D_801569AC = idx;
    osSendMesg(&D_80156BB0, 0, 1);
}

static void _ValidateWaveInstance(int idx, int loaded, int ready)
{
}

int audio_frame_sync(int waveNum, int loadAgain, int loadBackground, int handle, void *heap)
{
    int idx;
    if (!loadAgain) {
        idx = func_80097694(waveNum, handle);
        if (idx >= 0) {
            if (gWaveInstance[idx].model) entity_render_mode(idx);
            _ValidateWaveInstance(idx, 1, 1);
            return idx;
        }
    }
    idx = _NewWaveInstance(waveNum, handle, heap);
    if (loadBackground) _BGLoadWave(idx);
    else func_80097164(idx);
    return idx;
}
