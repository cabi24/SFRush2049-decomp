/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * explosion_effect (0x800EF8F4, 1460 B) = MAT3 animSpeedText@hud.c (GC 0x8001DB6C, 864 B): the digital
 * speedometer text. Direct typed port of the MAT3 body.
 */
#define gShowSpeed D_8015F72C
#define gJust D_80118E20
#define model D_8014A250
#define gGameCar D_80152818
#define gShowMetric D_801613AA
#define speedPos D_80115A68
#define gText D_8017A4E0
#define gFontHeader D_801497F0
#define gFontSem D_801461D0
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;

typedef struct tCarData { u8 pad000[0xEF]; s8 place_locked; u8 pad0F0[8]; s16 rwmph; u8 pad0FA[952 - 0xFA]; } tCarData;
typedef struct tModelData { u8 pad000[0x7C6]; s16 slot; u8 pad7C8[0x808 - 0x7C8]; } tModelData;
typedef struct tJust { s32 h, v; } tJust;
typedef struct tText { u16 numSingle; u8 **single; } tText;
typedef struct tFontHeader { u8 pad0[2]; u8 ascent; u8 descent; } tFontHeader;

extern s8 gShowSpeed;                /* D_8015F72C */
tJust gJust;   /* tentative definition (as credits_scroll_grp/gr3_d.c): one lui for the TB_SetJust tail stores */                  /* D_80118E20 */
extern s16 gNumWindows;              /* D_80151AD0 */
extern tModelData model[];           /* D_8014A250 */
extern tCarData gGameCar[];          /* D_80152818 */
extern s8 gShowMetric;               /* D_801613AA */
extern s32 speedPos[4][4][2];        /* D_80115A68 */
extern tText gText;                  /* D_8017A4E0 */
extern tFontHeader *gFontHeader;     /* D_801497F0 */
extern OSMesgQueue gFontSem;         /* D_801461D0 */

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
void render_helper(f32 depth);                 /* HAL_SetTextDepth */
s32 slot_state_setup(s32 fontID);              /* _HAL_SetFont */
void sound_update_channel(s32 force);          /* _SetupFontData */
u32 camera_shake_update(u16 ch);               /* HAL_GetCharWidth */
void dispatch_handler(s32 color);              /* TB_SetTextColor */
void state_utility(s16 x, s16 y, void *text);  /* TB_DrawText */

void _GetSemaphore_Speed(void);
void _ReleaseSemaphore_Speed(void);
s32 HAL_SetFont_Speed(s32 fontID);
void TB_SetJust_Speed(s32 hJust, s32 vJust);
u8 HAL_GetFontAscent_Speed(void);
u8 HAL_GetFontDescent_Speed(void);
s16 TB_GetFontHeight_Speed(void);

s32 explosion_effect(void *blt)
{
    f32 mph;
    u8 digits[3];
    u8 str[2];
    s32 digitW, x, y, digitIdx, slot;

    if (!gShowSpeed)
        return 1;
    render_helper(0.0f);
    TB_SetJust_Speed(1, 3);
    if (gNumWindows == 1)
        HAL_SetFont_Speed(4);
    else
        HAL_SetFont_Speed(1);
    digitW = camera_shake_update('8') + 1;
    for (slot = 0; slot < gNumWindows; slot++) {
        if (y) {}
        if (gGameCar[model[slot].slot].place_locked == 1)
            continue;
        mph = gGameCar[slot].rwmph >> 2;
        mph *= 1.2f;
        if (gShowMetric)
            mph *= 1.61f;
        if (mph < 0.0f)
            mph = -mph;
        x = speedPos[gNumWindows - 1][slot][0];
        x -= digitW;
        y = speedPos[gNumWindows - 1][slot][1];
        digits[0] = ((s32)(mph / 100.0f) % 10) + '0';
        digits[1] = ((s32)(mph / 10.0f) % 10) + '0';
        digits[2] = ((s32)mph % 10) + '0';
        if (gNumWindows == 1)
            HAL_SetFont_Speed(4);
        else
            HAL_SetFont_Speed(1);
        for (digitIdx = 0; digitIdx < 3; digitIdx++) {
            str[1] = 0;
            if (digits[digitIdx] != ':' && digits[digitIdx] != '.') {
                str[0] = '8';
                dispatch_handler(0);
                state_utility(x, y, str);
            }
            str[0] = digits[digitIdx];
            dispatch_handler(14);
            state_utility(x, y, str);
            x += digitW;
        }
        x = speedPos[gNumWindows - 1][slot][0];
        y += TB_GetFontHeight_Speed() - 4;
        HAL_SetFont_Speed(12);
        if (gShowMetric) {
            dispatch_handler(0);
            state_utility(x + 1, y + 1, gText.single[203]);
            dispatch_handler(1);
            state_utility(x, y, gText.single[203]);
        } else {
            dispatch_handler(0);
            state_utility(x + 1, y + 1, gText.single[202]);
            dispatch_handler(1);
            state_utility(x, y, gText.single[202]);
        }
        if (mph) {}
    }
    TB_SetJust_Speed(0, 3);
    render_helper(-1.0f);
    return 1;
}

void _GetSemaphore_Speed(void) { osRecvMesg(&gFontSem, (void **)0, 1); }
void _ReleaseSemaphore_Speed(void) { osSendMesg(&gFontSem, (void *)0, 0); } s32 HAL_SetFont_Speed(s32 fontID) { s32 old; _GetSemaphore_Speed(); old = slot_state_setup(fontID); _ReleaseSemaphore_Speed(); return old; }

void TB_SetJust_Speed(s32 hJust, s32 vJust)
{
    gJust.h = hJust;
    gJust.v = vJust;
}

u8 HAL_GetFontAscent_Speed(void)
{
    sound_update_channel(0);
    return gFontHeader->ascent;
}

u8 HAL_GetFontDescent_Speed(void)
{
    sound_update_channel(0);
    return gFontHeader->descent;
}

s16 TB_GetFontHeight_Speed(void)
{
    return HAL_GetFontAscent_Speed() + HAL_GetFontDescent_Speed();
}
