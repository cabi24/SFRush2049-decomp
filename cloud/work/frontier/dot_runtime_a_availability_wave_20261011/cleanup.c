/* Image A: complete 168-byte cleanup. Native clobbers s0-s2; standalone is research. */
typedef signed char s8;
typedef short s16;
typedef int s32;
typedef struct { s32 fields[16]; } MenuRecord;
extern MenuRecord D_803B5F28[23];
extern s32 D_803BA7D0;
extern void *D_803B6530, *D_803B652C;
extern s8 D_803B6528;
extern void sound_call_minimal(s16);
extern void sound_handles_array_clear(void *);
extern void sound_stop(void *);
extern void ambient_sounds_clear(void);
void func_803932E4(void) {
    MenuRecord *record;
    for (record = D_803B5F28; record != D_803B5F28 + 23; record++) {
        if (record->fields[1] != -1) {
            sound_call_minimal((s16)record->fields[1]);
            record->fields[1] = -1;
        }
    }
    D_803BA7D0 = 0;
    if (D_803B6530 != 0) {
        sound_handles_array_clear(D_803B6530);
        D_803B6530 = 0;
    }
    if (D_803B652C != 0) {
        sound_stop(D_803B652C);
        D_803B652C = 0;
    }
    ambient_sounds_clear();
    D_803B6528 = 0;
}
