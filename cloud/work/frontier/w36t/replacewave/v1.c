/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* suspension_setup = ReplaceWave (MAT3 GameSource/src/waves.c ReplaceWave, GC 0x8005B6EC) with
 * _ReplaceWaveInstance (GC 0x8005BAC4) inlined. */
typedef signed char s8;
typedef signed short s16;
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
extern s16 D_8002EB70;                      /* gRenderInProgress */
#define gWaveInstance D_80156D38
#define gRenderInProgress D_8002EB70

int func_80097694(int waveNum, signed char handle);           /* _FindWaveInstance */
void func_80096288(int idx, int loaded, int ready);           /* _ValidateWaveInstance */
void func_800972C4(int idx);                                  /* _SetWaveInstanceModel */
void func_80097164(int idx);                                  /* _ReadWave */

static void _ReplaceWaveInstance(int idx, int newWave)
{
    anon_loaded_ready_14 *winst;
    winst = &gWaveInstance[idx];
    func_80096288(idx, 1, 1);
    winst->loaded = 0; winst->ready = 0; winst->wave = newWave; winst->mirrored = 0;
    func_800972C4(idx);
}

void suspension_setup(int curWave, int newWave)
{
    int idx;
    if (func_80097694(newWave, -1) < 0) {
        idx = func_80097694(curWave, -1);
        if (idx >= 0) {
            while (*(volatile s16 *)&gRenderInProgress) {}
            _ReplaceWaveInstance(idx, newWave); func_80097164(idx);
        }
    }
}
