/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef void *OSMesg;
typedef struct OSMesgQueue { s32 w[6]; } OSMesgQueue;
extern s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
extern s32 osSendMesg(OSMesgQueue *, OSMesg, s32);
extern void *memcpy(void *, const void *, u32);
extern void func_800A473C(void *, void *);          /* strcpy */
extern void func_80092DCC(void *, void *, s32);     /* strncpy */
extern void audio_doppler_calc(void *text, u16 length);  /* _DrawText */
extern void func_800B6748(u32 *, f32 *, s16 *, s16 *, s8 *, s8 *, s8 *);  /* _SetDefaults */

extern f32 D_80114744;      /* gDepth */
extern s8 D_8011473C;       /* gFinished */
extern f32 D_80114748;      /* gTrans */
extern s8 D_8011474C;       /* gBufferInited */
extern OSMesgQueue D_801461D0;
extern u32 D_80149B48;      /* gFontColor */
extern s8 D_80149B60;       /* gLeading */
extern s8 D_80149B70;       /* gSpacing */
extern s16 D_80149D92;      /* gFontXpos */
extern s16 D_80149D9E;      /* gFontYpos */
extern s8 D_80149DA0;       /* gFontIndex */
extern s32 D_80149DD0;      /* gBufLen */
extern u8 D_80149DD8[800];  /* gBuffer */

void dt_GetSemaphore(void) { osRecvMesg(&D_801461D0, 0, 1); }
void dt_ReleaseSemaphore(void) { osSendMesg(&D_801461D0, 0, 0); }

u32 func_800B66B0(const u8 *text, s16 numChars)      /* _TextLen (MAT3 HAL_text.c) */
{
    u32 len;
    s32 nc;
    nc = 0;
    if (text[0] == 255) {
        len = 1;
        do {
            len += 2;
            nc++;
        } while ((text[len - 1] || text[len - 2]) && (numChars < 0 || nc <= numChars));
    } else {
        len = 0;
        do {
            len++;
            nc++;
        } while (*text++ && (numChars < 0 || nc <= numChars));
    }
    return len;
}

void menu_input_process(void *text, s16 numChars)
{
    s32 size;
    s32 length;
    u8 str[256];
    static u32 D_80153F60;      /* lastFontColor */
    static f32 D_80153F80;      /* lastTrans */
    static s16 D_80153FD0;      /* lastFontXpos */
    static s16 D_80154180;      /* lastFontYpos */
    static s8 D_80154184;       /* lastFontIndex */
    static s8 D_80154194;       /* lastLeading */
    static s8 D_8015419C;       /* lastSpacing */

    if (D_80114744 >= 0.0f) {
        audio_doppler_calc(text, func_800B66B0(text, numChars));
        return;
    }
    if (D_8011473C) {
        if (numChars < 0) func_800A473C(str, text);
        else func_80092DCC(str, text, numChars);
    }
    dt_GetSemaphore();
    if (!D_8011474C) {
        D_8011474C = 1;
        D_80149DD0 = 0;
        func_800B6748(&D_80153F60, &D_80153F80, &D_80153FD0, &D_80154180, &D_80154184, &D_80154194, &D_8015419C);
    }
    size = 0;
    if (D_80154184 != D_80149DA0) size = 2; else ;
    if (D_80154194 != D_80149B60) size += 2;
    if (D_8015419C != D_80149B70) size += 2;
    if (D_80153FD0 != D_80149D92 || D_80154180 != D_80149D9E) size += 5;
    if (D_80153F80 != D_80114748) size += 5;
    if (D_80153F60 != D_80149B48) size += 5;
    length = func_800B66B0(text, numChars);
    size += length + 2;
    if (D_80149DD0 + size >= 800) {
        dt_ReleaseSemaphore();
        return;
    }
    if (D_80154184 != D_80149DA0) {
        D_80154184 = D_80149DA0;
        D_80149DD8[D_80149DD0++] = 0;
        D_80149DD8[D_80149DD0++] = D_80149DA0;
    }
    if (D_80154194 != D_80149B60) {
        D_80154194 = D_80149B60;
        D_80149DD8[D_80149DD0++] = 1;
        D_80149DD8[D_80149DD0++] = D_80149B60;
    }
    if (D_8015419C != D_80149B70) {
        D_8015419C = D_80149B70;
        D_80149DD8[D_80149DD0++] = 2;
        D_80149DD8[D_80149DD0++] = D_80149B70;
    }
    if (D_80153FD0 != D_80149D92 || D_80154180 != D_80149D9E) {
        D_80153FD0 = D_80149D92;
        D_80154180 = D_80149D9E;
        D_80149DD8[D_80149DD0++] = 3;
        D_80149DD8[D_80149DD0++] = (D_80153FD0 >> 8) & 255;
        D_80149DD8[D_80149DD0++] = D_80153FD0 & 255;
        D_80149DD8[D_80149DD0++] = (D_80154180 >> 8);
        D_80149DD8[D_80149DD0++] = D_80154180;
        D_80153FD0 = D_80149D92;
        D_80154180 = D_80149D9E;
    }
    if (D_80153F80 != D_80114748) {
        D_80153F80 = D_80114748;
        D_80149DD8[D_80149DD0++] = 4;
        D_80149DD8[D_80149DD0++] = (*(u32 *)&D_80114748 >> 24) & 255;
        D_80149DD8[D_80149DD0++] = (*(u32 *)&D_80114748 >> 16) & 255;
        D_80149DD8[D_80149DD0++] = (*(u32 *)&D_80114748 >> 8) & 255;
        D_80149DD8[D_80149DD0++] = (*(u32 *)&D_80114748) & 255;
    }
    if (D_80153F60 != D_80149B48) {
        D_80153F60 = D_80149B48;
        D_80149DD8[D_80149DD0++] = 5;
        memcpy(D_80149DD8 + D_80149DD0, &D_80149B48, 4);
        D_80149DD0 += 4;
    }
    D_80149DD8[D_80149DD0++] = 6;
    D_80149DD8[D_80149DD0++] = length;
    memcpy(D_80149DD8 + D_80149DD0, text, length);
    D_80149DD0 += length;
    dt_ReleaseSemaphore();
}
