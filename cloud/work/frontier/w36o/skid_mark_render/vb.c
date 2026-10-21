/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * skid_mark_render (0x800F0100, 1,396 B; historical label) = MAT3 ShowHUD@hud.c (GC 0x8001C980, 904 B;
 * reference/repos/mat3/decomp/src/GameSource/src/hud.c). Creates the HUD blit sets on first call (clears the
 * per-player checkpoint/lap/elapsed timers and coin records, then NewMultiBlit = sound_control: overlay set for
 * attract/credits, 2/3-player divider sets, ShowHUDExtra (image B 0x803925D0) for battle/stunt or the race set,
 * then the HUD set), and in attract mode with credits on cycles the credit panes (SetCountdownTimer =
 * viScheduleTick, GetCountdownTimeRemaining = viGetTimeToDeadline, Random = func_8008B2E4 inlined, HAL_SetFont,
 * HAL_GetTextWidth = object_manager_update, RemovePanes = sound_handles_clear, InitPane = ambient_sound_set).
 * N64 constants differ from the GC (race/HUD blit counts 89/8, credit delay /13, bottom row 145; the 2/3-player
 * sets only when D_80156994 || D_8014978C >= 6).
 * Body: Astra's research packet PR #414 (cloud/work/frontier/dot_showhud_research_20261017/group/candidate.c,
 * function-local statics delay/nextSwitch, credit-name for-loop, x-before-y corner order), unchanged except
 * w36o's one lever: HAL_SetFont is inlined (as in retail: osRecvMesg + slot_state_setup(s2=11) + osSendMesg)
 * through a non-static copy HAL_SetFont_ShowHUD with _GetSemaphore/_ReleaseSemaphore defined after the caller
 * (the locked object_create is kept and stays a jal in the unit, 335/349 words off there).
 * gSeconds is `extern volatile float` as in MAT3 rush_protos.h (D_8002EB90).
 * The block-local loop index `i` is a reconstruction (absent from the DWARF local list of ShowHUD).
 * MATCH: whole-program unit EQUAL (blob_unit --tag w36o); group w36o_showhud (score.py group) MATCH.
 */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Blit Voice;
typedef struct Pane Pane;
typedef s32 (*PaneAnimation)(Voice *);
typedef struct MultiBlit {
    const char *texname; s16 dulx,duly,width,height,top,bot,left,right;
    u32 zdepth,alpha; s32 (*animfunc)(Voice *); u32 animid;
} MultiBlit;
typedef struct RecordSlot { s16 id; u8 pad02[50]; } RecordSlot;   /* 52 bytes */
typedef struct RecordRow { RecordSlot a, b; } RecordRow;              /* 104 bytes */
/* Complete native resource-table layout, shared with its actual loader. */
typedef struct ModelTables {
    u16 count0; u32 *ptrs0; u16 count1,count2; u16 *data1; u32 *ptrs2;
    u16 count3,count4,count5; u16 *data3,*data4; u32 *ptrs5;
} ModelTables;

extern Voice *D_80154198;
extern Voice *D_801541A0;
extern Voice *D_801541A4;
extern RecordRow D_801541A8[];
extern f32 D_80142740[4], D_80142770[4], D_80142AC8[4];
extern u32 D_801174B4;
extern s8 D_80156994;
extern s8 D_8014978C;
extern s16 D_80151AD0;
extern s32 gameplay_mode;
extern MultiBlit D_80115A44[], D_801159FC[], D_80115A20[], D_80114C58[], D_801158DC[];
extern s16 D_80142D70;
extern s32 D_80138660;
extern s32 D_80154394;
extern volatile f32 D_8002EB90;
extern s32 D_801543A0;
extern s32 D_801543A8;
extern s32 D_801543B0;
extern char *D_80114A0C[][11];
extern ModelTables D_8017A4E0;

Voice *sound_control(s16, s16, const MultiBlit *, s16);
Pane *ambient_sound_set(s32,s32,s32,s32,s32,PaneAnimation,u32,s32);
void sound_handles_clear(s32 arg0);
void viScheduleTick(f32);
f32 viGetTimeToDeadline(void);
void func_803925D0(void);
typedef struct { u32 opaque[6]; } OSMesgQueue;
extern OSMesgQueue D_801461D0;
s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 fontID);
void _GetSemaphore_ShowHUD(void);
void _ReleaseSemaphore_ShowHUD(void);
s32 HAL_SetFont_ShowHUD(s32 fontID);
extern u32 object_manager_update(void *, s16);

extern f32 func_8008B2E4(f32);

void skid_mark_render(void) {
    s32 created = 0;
    s32 x;
    s32 y;
    static f32 delay;
    static f32 nextSwitch;
    if (D_80154198 == 0) {
        s32 i; /* Reconstruction-only loop index; absent from original named-local list. */
        for (i = 0; i < 4; i++) {
            D_80142740[i] = 0.0f;
            D_80142770[i] = 0.0f;
            D_80142AC8[i] = 0.0f;
            D_801541A8[i].a.id = -1;
            D_801541A8[i].b.id = -1;
        }
        if (D_801174B4 & 0x02000000) {
            D_80154198 = sound_control(0, 0, D_80115A44, 1);
        } else if (D_801174B4 & 8) {
            D_80154198 = sound_control(0, 0, D_80115A44, 1);
        } else {
            if (D_80156994 || D_8014978C >= 6) {
                if (D_80151AD0 == 2) {
                    D_801541A4 = sound_control(0, 0, D_801159FC, 1);
                } else if (D_80151AD0 == 3) {
                    D_801541A4 = sound_control(0, 0, D_80115A20, 1);
                }
            }
            if (gameplay_mode == 6 || gameplay_mode == 4) {
                func_803925D0();
            } else {
                D_801541A0 = sound_control(0, 0, D_80114C58, 89);
            }
            D_80154198 = sound_control(0, 0, D_801158DC, 8);
        }
        D_80142D70 = 0;
        created = 1;
    }
    if ((D_801174B4 & 8) && D_80138660) {
        if (created) {
            D_80154394 = -1;
            viScheduleTick(90.0f);
            delay = viGetTimeToDeadline() / 13.0f;
            nextSwitch = D_8002EB90;
        }
        if (nextSwitch <= D_8002EB90) {
            nextSwitch += delay;
            D_80154394++;
            D_801543A0 = func_8008B2E4(4.0f);
            HAL_SetFont_ShowHUD(11);
            D_801543A8 = object_manager_update((void *)D_8017A4E0.ptrs2[D_8017A4E0.data1[0] + D_80154394], -1);
            for (D_801543B0 = 0; D_80114A0C[D_80154394][D_801543B0]; D_801543B0++) {
                if (D_801543A8 < object_manager_update(D_80114A0C[D_80154394][D_801543B0], -1)) {
                    D_801543A8 = object_manager_update(D_80114A0C[D_80154394][D_801543B0], -1);
                }
            }
            switch (D_801543A0) {
            case 0:
                x = 20;
                y = 20;
                break;
            case 1:
                x = 280 - D_801543A8;
                y = 20;
                break;
            case 2:
                x = 20;
                y = 145 - D_801543B0 * 10;
                break;
            default:
                x = 280 - D_801543A8;
                y = 145 - D_801543B0 * 10;
                break;
            }
            sound_handles_clear(0);
            ambient_sound_set(x, y, x + D_801543A8 + 20, D_801543B0 * 10 + y + 32, 192, 0, 0, 0);
        }
    }
}

s32 HAL_SetFont_ShowHUD(s32 fontID)
{
    s32 old;
    _GetSemaphore_ShowHUD();
    old = slot_state_setup(fontID);
    _ReleaseSemaphore_ShowHUD();
    return old;
}

void _GetSemaphore_ShowHUD(void)
{
    osRecvMesg(&D_801461D0, (void **)0, 1);
}

void _ReleaseSemaphore_ShowHUD(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}
