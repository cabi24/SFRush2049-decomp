/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * credits_screen (0x800D6698, 296 B; historical label) = MAT3 uninitScreen@audioscr.c (GC 0x...; N64 form), w36w3
 * (wave 36). Internal (private IPA: s0-s3 unsaved); its only caller is updateAudioScreen = func_800D7634 (two
 * sites, both in this group). AudKillSong() (N64: under the slot-queue lock func_80091AF0/func_80091AF8, _GetCmd =
 * func_80091B00, command 1, then _ProcessCommand = osSendMesg to the audio queue D_801427A8), then if the screen is
 * inited (D_8011062C): uninitButtons() (12 buttons D_801102B4, 64 B each: ZOID_RemoveObject = entity_spawn_callback
 * (objIdx, 0, 0) for live objects, objIdx = -1; buttonsInited D_801105B4 = 0), outside the game state
 * (gGameState & 0x7C03FFFE == 0) RemovePanes(0) = sound_handles_clear(0) and gTitlePane D_80110648 = 0, the blit
 * D_80110630 removed (RemoveBlit = sound_stop), removeBG = ambient_sounds_clear(), panesCreated D_80110634 = 0,
 * inited = 0.
 * Helpers are non-static and not on keep (umerge inlines them; w17j stub rule), named per file like w36q's
 * di_post_type1 / di_ProcessCommand in init_w36q.c. uninitButtons is defined AFTER the caller, as in the GC file
 * (audioscr.c line order): as1 breaks ready-list ties by source line, and retail schedules the gGameState load
 * (caller line) before the inlined buttonsInited store (2 words otherwise).
 */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct Msg { s16 id; s8 type, used; u8 payload[20]; } Msg;
typedef struct {
    s32 type;            /* 0x00 */
    s32 handle;          /* 0x04 */
    u8 pad08[0x38];
} UButton; /* 0x40 */
extern s32 D_801427A8;
extern s8 D_8011062C;           /* inited */
extern s8 D_80110634;           /* panesCreated */
extern s32 D_80110630;          /* blit */
extern s32 D_80110648;          /* gTitlePane */
extern s32 D_801105B4;          /* buttonsInited */
extern UButton D_801102B4[12];  /* buttonInfo */
extern u32 state_word_a;        /* gGameState */
Msg *func_80091B00(void);
void func_80091AF0(void);
void func_80091AF8(void);
s32 osSendMesg(OSMesgQueue *, void *, s32);
void entity_spawn_callback(s16 idx, s32 freeChildren, s32 freeSiblings);
void sound_handles_clear(s32 arg0);
void sound_stop(s32 sound_id);
void ambient_sounds_clear(void);

void us_ProcessCommand(Msg *task)
{
    osSendMesg((OSMesgQueue *) &D_801427A8, task, 0);
}

void us_AudKillSong(void)
{
    Msg *task;
    func_80091AF0();
    task = func_80091B00(); task->type = 1;
    func_80091AF8();
    us_ProcessCommand(task);
}

void us_uninitButtons(void);

void credits_screen(void)
{
    us_AudKillSong();
    if (D_8011062C) {
        us_uninitButtons();
        if (!(state_word_a & 0x7C03FFFE)) {
            sound_handles_clear(0);
            D_80110648 = 0;
        }
        if (D_80110630) {
            sound_stop(D_80110630);
            D_80110630 = 0;
        }
        ambient_sounds_clear();
        D_80110634 = 0;
        D_8011062C = 0;
    }
}

void us_uninitButtons(void)
{
    s32 buttonIdx;

    for (buttonIdx = 0; buttonIdx < 12; buttonIdx++) {
        if (D_801102B4[buttonIdx].handle != -1) {
            entity_spawn_callback(D_801102B4[buttonIdx].handle, 0, 0);
            D_801102B4[buttonIdx].handle = -1;
        }
    }
    D_801105B4 = 0;
}
