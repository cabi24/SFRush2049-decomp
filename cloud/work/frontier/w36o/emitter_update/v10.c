/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * emitter_update (0x800EEDB0, 1,240 B) = MAT3 animCountdownTimer@hud.c (GC 0x8001F048, 852 B). Direct typed port.
 */
#define gGameState D_801174B4
#define gShowRemaining D_8015B24C
#define gRace D_8014A110
#define gBattleMode D_80140B08
#define gStuntMode D_801407BC
#define gGameSoundsEnabled D_8010FFC0
#define sc D_8002E8E8
#define countdownTimerPos D_80115C68
#define mapPos D_801160A8
#define gJust D_80118E20
#define gFontSem D_801461D0

typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;
typedef struct tJust { s32 h, v; } tJust;
typedef struct tSchedCtl { u8 pad000[0x27C]; s32 frameCount; } tSchedCtl;

extern u32 gGameState;
extern s8 gShowRemaining;
extern s32 gRace;
extern s32 gBattleMode;
extern s32 gStuntMode;
extern s8 gGameSoundsEnabled;
extern volatile tSchedCtl sc;
extern s16 gNumWindows;
extern s32 countdownTimerPos[4][2];
extern s32 mapPos[4][2];
extern tJust gJust;
extern OSMesgQueue gFontSem;

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
f32 viGetTimeToDeadline(void);                 /* GetCountdownTimeRemaining */
s32 entity_flags_apply(s32 fxid, s32 subId, s32 replace, u8 priority); /* AudPlayEffect */
void render_helper(f32 depth);                 /* HAL_SetTextDepth */
s32 slot_state_setup(s32 fontID);              /* _HAL_SetFont */
u32 camera_shake_update(u16 ch);               /* HAL_GetCharWidth */
void dispatch_handler(s32 color);              /* TB_SetTextColor */
void state_utility(s16 x, s16 y, void *text);  /* TB_DrawText */

void _GetSemaphore_Countdown(void);
void _ReleaseSemaphore_Countdown(void);
s32 HAL_SetFont_Countdown(s32 fontID);
void TB_SetJust_Countdown(s32 hJust, s32 vJust);
s32 PlayGameSound_Countdown(s32 fxid, s32 subId, s32 replace, u8 priority);

s32 emitter_update(void *blt)
{
    s32 t;
    u8 digits[5];
    u8 str[2];
    s32 digitW, colonW, x, y, digitIdx;
    static s32 sec;
    static s32 nextBeep = 30;

    if (!(gGameState & 0x400000) || !gShowRemaining)
        goto skip;
    if (gRace == 6 && gBattleMode == 0)
        goto skip;
    if (gRace == 4 && gStuntMode == 1)
        goto skip;
    if (gRace == 0 || gRace == 1 || gRace == 3) {
skip:
        return 1;
    }
    t = viGetTimeToDeadline();
    if (t < 0)
        t = 0;
    sec = t;
    if (sec <= 30) {
        if (sec <= nextBeep) {
            if (nextBeep > 5)
                nextBeep = 5;
            else
                nextBeep--;
            PlayGameSound_Countdown(2, 0, 2, 0);
        }
        if (sec <= 5 && (sc.frameCount & 8))
            return 1;
    } else
        nextBeep = 30;
    x = countdownTimerPos[gNumWindows - 1][0];
    y = countdownTimerPos[gNumWindows - 1][1];
    render_helper(0.0f);
    TB_SetJust_Countdown(1, 3);
    if (gRace == 4 && gNumWindows > 1) {
        x = mapPos[gNumWindows - 1][0];
        y = mapPos[gNumWindows - 1][1];
        TB_SetJust_Countdown(1, 1);
    }
    digits[0] = t / 600 + '0';
    digits[1] = (t / 60) % 10 + '0';
    digits[2] = ':';
    digits[3] = (t / 10) % 6 + '0';
    digits[4] = t % 10 + '0';
    if (gNumWindows == 1)
        HAL_SetFont_Countdown(9);
    else if (gNumWindows <= 3)
        HAL_SetFont_Countdown(8);
    else
        HAL_SetFont_Countdown(5);
    digitW = camera_shake_update('8') + 1;
    colonW = camera_shake_update(':') + 1;
    x -= (3 * digitW + colonW) / 2;
    for (digitIdx = 0; digitIdx < 5; digitIdx++) {
        str[0] = digits[digitIdx];
        str[1] = 0;
        if (digitIdx > 0 || str[0] != '0') {
            dispatch_handler(0);
            state_utility(x + 1, y + 1, str);
            dispatch_handler(14);
            state_utility(x, y, str);
        }
        if (digitIdx == 1 || digitIdx == 2)
            x += (digitW + colonW) / 2;
        else
            x += digitW;
    }
    TB_SetJust_Countdown(0, 3);
    render_helper(-1.0f);
    return 1;
}

void _GetSemaphore_Countdown(void) { osRecvMesg(&gFontSem, (void **)0, 1); }
void _ReleaseSemaphore_Countdown(void) { osSendMesg(&gFontSem, (void *)0, 0); } s32 HAL_SetFont_Countdown(s32 fontID) { s32 old; _GetSemaphore_Countdown(); old = slot_state_setup(fontID); _ReleaseSemaphore_Countdown(); return old; }

void TB_SetJust_Countdown(s32 hJust, s32 vJust)
{
    gJust.h = hJust;
    gJust.v = vJust;
}

s32 PlayGameSound_Countdown(s32 fxid, s32 subId, s32 replace, u8 priority)
{
    if (!gGameSoundsEnabled) return -1;
    if (fxid == -1) return -1;
    return entity_flags_apply(fxid, subId, replace, priority);
}
