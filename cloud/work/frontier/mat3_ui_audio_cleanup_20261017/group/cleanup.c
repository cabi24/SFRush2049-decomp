/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* MAT3 audioscr teardown and native N64 queue/renderer boundaries.
 * This research closure retains its real native caller and every known body. */
typedef signed char s8;
typedef signed short s16;
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct Blit Blit;
typedef struct Pane Pane;
typedef struct Slot18 { s16 f0; s8 f2; s8 f3; u8 pad[0x14]; } Slot18;
typedef struct { int type; int handle; float ang; float mat[3][3]; float pos[3]; u8 pad3C[4]; } Button;
extern OSMesgQueue D_80142728, D_801427A8;
extern s8 D_8011062C, D_80110634;
extern int D_801105B4;
extern Pane *D_80110648;
extern Blit *D_80110630;
extern u32 state_word_a;
extern Button D_801102B4[12];
extern int osRecvMesg(OSMesgQueue *, void **, int);
extern int osSendMesg(OSMesgQueue *, void *, int);
extern Slot18 *func_80091B00(void);
extern void entity_spawn_callback(s16, int, int);
extern void sound_handles_clear(int);
extern void sound_stop(Blit *);
extern void ambient_sounds_clear(void);
void sound_call_simple(s16, int);
void sound_call_minimal(s16);
void ui_uninitButtons(void);
void func_80091AF0(void);
void func_80091AF8(void);
void object_type1_create(void);
void credits_screen(void)
{
    object_type1_create();
    if (D_8011062C != 0) {
        ui_uninitButtons();
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
void ui_uninitButtons(void)
{
    int buttonIdx;
    for (buttonIdx = 0; buttonIdx < 12; buttonIdx++) {
        if (D_801102B4[buttonIdx].handle != -1) {
            sound_call_minimal(D_801102B4[buttonIdx].handle);
            D_801102B4[buttonIdx].handle = -1;
        }
    }
    D_801105B4 = 0;
}
void ui_ProcessCommand(Slot18 *task)
{
    osSendMesg(&D_801427A8, task, 0);
}
void object_type1_create(void)
{
    Slot18 *task;
    func_80091AF0();
    task = func_80091B00();
    task->f2 = 1;
    func_80091AF8();
    ui_ProcessCommand(task);
}
void sound_call_simple(s16 objnum, int removeChildren)
{
    entity_spawn_callback(objnum, removeChildren, 0);
}
void sound_call_minimal(s16 objnum)
{
    sound_call_simple(objnum, 0);
}
