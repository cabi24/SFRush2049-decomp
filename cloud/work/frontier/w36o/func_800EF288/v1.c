/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800EF288 (0x800EF288, 808 B) = MAT3 animGear@hud.c (GC 0x8001E144, 536 B): the gear indicator text.
 * Direct typed port. N64 differences: no early gShowShifter/gGameState return (gShowShifter is tested per slot).
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

static void _GetSemaphore(void)
{
    osRecvMesg(&gFontSem, (void **)0, 1);
}

static void _ReleaseSemaphore(void)
{
    osSendMesg(&gFontSem, (void *)0, 0);
}

static s32 HAL_SetFont(s32 fontID)
{
    s32 old;
    _GetSemaphore();
    old = slot_state_setup(fontID);
    _ReleaseSemaphore();
    return old;
}

static s16 TB_RightText(void *text, s16 rightX)
{
    return rightX - object_manager_update(text, -1);
}

s32 func_800EF288(void *blt)
{
    u8 str[2];
    s32 slot;
    s16 x, y;
    s8 gear;

    render_helper(0.0f);
    if (gNumWindows == 1)
        HAL_SetFont(10);
    else if (gNumWindows == 2)
        HAL_SetFont(11);
    else
        HAL_SetFont(12);
    for (slot = 0; slot < gNumWindows; slot++) {
        gear = model[slot].gear;
        if (slot >= gNumWindows || !gShowShifter || model[slot].autotrans ||
            gGameCar[model[slot].slot].place_locked == 1)
            continue;
        x = gearPos[gNumWindows - 1][slot][0] - 4;
        y = gearPos[gNumWindows - 1][slot][1];
        dispatch_handler(0);
        state_utility(TB_RightText(gText.single[204], x + 1), y + 1, gText.single[204]);
        dispatch_handler(1);
        state_utility(TB_RightText(gText.single[204], x), y, gText.single[204]);
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
