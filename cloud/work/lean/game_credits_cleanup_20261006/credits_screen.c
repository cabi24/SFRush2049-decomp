/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Whole native cleanup body; original internal caller context still required. */
typedef signed char s8;
typedef signed short s16;
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct Slot18 { s16 f0; s8 f2; s8 f3; u8 pad[0x14]; } Slot18;
typedef struct CleanupRow { u32 unknown00; int identifier; u8 unknown08[56]; } CleanupRow;
extern OSMesgQueue D_80142728, D_801427A8;
extern s8 D_8011062C, D_80110634;
extern int D_801105B4, D_80110648, D_80110630;
extern u32 state_word_a;
extern CleanupRow D_801102B4[12];
extern int osRecvMesg(OSMesgQueue *, void **, int);
extern int osJamMesg(OSMesgQueue *, void *, int);
extern Slot18 *func_80091B00(void);
extern void entity_spawn_callback(s16, int, int);
extern void sound_handles_clear(int);
extern void sound_stop(int);
extern void ambient_sounds_clear(void);
void credits_screen(void)
{
    Slot18 *task;
    CleanupRow *row;
    int identifier;
    osRecvMesg(&D_80142728, 0, 1);
    task = func_80091B00();
    task->f2 = 1;
    osJamMesg(&D_80142728, 0, 0);
    osJamMesg(&D_801427A8, task, 0);
    if (D_8011062C != 0) {
        for (row = D_801102B4; row != D_801102B4 + 12; ++row) {
            identifier = row->identifier;
            if (identifier != -1) {
                entity_spawn_callback((s16)identifier, 0, 0);
                row->identifier = -1;
            }
        }
        D_801105B4 = 0;
        if (!(state_word_a & 0x7C03FFFE)) {
            sound_handles_clear(0);
            D_80110648 = 0;
        }
        if (D_80110630 != 0) {
            sound_stop(D_80110630);
            D_80110630 = 0;
        }
        ambient_sounds_clear();
        D_80110634 = 0;
        D_8011062C = 0;
    }
}
