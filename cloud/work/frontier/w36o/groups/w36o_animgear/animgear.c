/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800EF288 (0x800EF288, 808 B; historical label) = MAT3 animGear@hud.c (GC 0x8001E144, 536 B;
 * reference/repos/mat3/decomp/src/GameSource/src/hud.c): per player, the "GEAR" label right-aligned at
 * gearPos - 4 (shadow + colour 1, gText.single[204]) and the gear character ('R' reverse, 'N' neutral, digit)
 * at gearPos + 4 (shadow + colour 14), with the font by window count (10/11/12) and text depth 0 / -1.
 * Direct typed port of the MAT3 body: MAT3 locals (u8 str[2]; s32 slot; s16 x, y; s8 gear), statement order,
 * TB_RightText and HAL_SetFont (with _GetSemaphore/_ReleaseSemaphore) inlined as in retail.
 * N64 differences: no early `!gShowShifter || !(gGameState & 0x400000)` return; gShowShifter is tested in
 * the per-slot skip condition after `slot >= gNumWindows`; text index 204 (GC 215).
 * Callee identities: render_helper = HAL_SetTextDepth, slot_state_setup = _HAL_SetFont (s2 parameter),
 * object_manager_update = HAL_GetTextWidth, dispatch_handler = TB_SetTextColor, state_utility = TB_DrawText.
 * Shaping (disclosed): helpers are non-static and not on keep (w17j), defined after the caller;
 * _ReleaseSemaphore_Gear and HAL_SetFont_Gear share one source line (as1 line tie in the group compile, see
 * w36o RESULTS.md, ShowHUD).
 */
#define gShowShifter D_80161394
#define model D_8014A250
#define gGameCar D_80152818
#define gearPos D_80115BE8
#define gText D_8017A4E0
#define gFontSem D_801461D0

typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;

typedef struct tCarData { u8 pad000[0xEF]; s8 place_locked; u8 pad0F0[952 - 0xF0]; } tCarData;
typedef struct tModelData {
    u8 pad000[0xA]; s8 autotrans; u8 pad00B[0x730 - 0xB]; s8 gear; u8 pad731[0x7C6 - 0x731]; s16 slot;
    u8 pad7C8[0x808 - 0x7C8];
} tModelData;
typedef struct tText { u16 numSingle; u8 **single; } tText;

extern s8 gShowShifter;
extern s16 gNumWindows;
extern tModelData model[];
extern tCarData gGameCar[];
extern s32 gearPos[4][4][2];
extern tText gText;
extern OSMesgQueue gFontSem;

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
void render_helper(f32 depth);                 /* HAL_SetTextDepth */
s32 slot_state_setup(s32 fontID);              /* _HAL_SetFont */
u32 object_manager_update(void *text, s16 numChars); /* HAL_GetTextWidth */
void dispatch_handler(s32 color);              /* TB_SetTextColor */
void state_utility(s16 x, s16 y, void *text);  /* TB_DrawText */

void _GetSemaphore_Gear(void);
void _ReleaseSemaphore_Gear(void);
s32 HAL_SetFont_Gear(s32 fontID);
s16 TB_RightText_Gear(void *text, s16 rightX);

s32 func_800EF288(void *blt)
{
    u8 str[2];
    s32 slot;
    s16 x, y;
    s8 gear;

    render_helper(0.0f);
    if (gNumWindows == 1)
        HAL_SetFont_Gear(10);
    else if (gNumWindows == 2)
        HAL_SetFont_Gear(11);
    else
        HAL_SetFont_Gear(12);
    for (slot = 0; slot < gNumWindows; slot++) {
        gear = model[slot].gear;
        if (slot >= gNumWindows || !gShowShifter || model[slot].autotrans ||
            gGameCar[model[slot].slot].place_locked == 1)
            continue;
        x = gearPos[gNumWindows - 1][slot][0] - 4;
        y = gearPos[gNumWindows - 1][slot][1];
        dispatch_handler(0);
        state_utility(TB_RightText_Gear(gText.single[204], x + 1), y + 1, gText.single[204]);
        dispatch_handler(1);
        state_utility(TB_RightText_Gear(gText.single[204], x), y, gText.single[204]);
        x = gearPos[gNumWindows - 1][slot][0] + 4;
        y = gearPos[gNumWindows - 1][slot][1];
        if (gear == -1)
            str[0] = 'R';
        else if (gear == 0)
            str[0] = 'N';
        else
            str[0] = gear + '0';
        str[1] = 0;
        dispatch_handler(0);
        state_utility(x + 1, y + 1, str);
        dispatch_handler(14);
        state_utility(x, y, str);
    }
    render_helper(-1.0f);
    return 1;
}

void _GetSemaphore_Gear(void) { osRecvMesg(&gFontSem, (void **)0, 1); }
void _ReleaseSemaphore_Gear(void) { osSendMesg(&gFontSem, (void *)0, 0); } s32 HAL_SetFont_Gear(s32 fontID) { s32 old; _GetSemaphore_Gear(); old = slot_state_setup(fontID); _ReleaseSemaphore_Gear(); return old; }

s16 TB_RightText_Gear(void *text, s16 rightX)
{
    return rightX - object_manager_update(text, -1);
}
