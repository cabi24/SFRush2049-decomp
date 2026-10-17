/* HAL_DrawText donor, native unscaled N64 fields and real queue helpers.
 * Original autos size, length, str[256]. Donor static data retain native symbols.
 */
typedef unsigned char u8; typedef signed char s8; typedef signed short s16;
typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct TextColor { union { unsigned char rgba[4]; unsigned int rgbaU32; } fore; } TextColor;
typedef struct NativeTextColor { union { unsigned char rgba[8]; unsigned int rgbaU32; } fore; } NativeTextColor;
extern TextColor D_80153F60; extern NativeTextColor D_80149B48;
extern f32 D_80153F80,D_80114744,D_80114748;
extern s16 D_80153FD0,D_80154180,D_80149D92,D_80149D9E;
extern s8 D_80154184,D_80154194,D_8015419C,D_8011473C,D_8011474C,D_80149DA0,D_80149B60,D_80149B70;
extern s32 D_80149DD0; extern u8 D_80149DD8[800];
u32 func_800B66B0(const u8 *,s16);
void audio_doppler_calc(const u8 *,u16);
void du_gfx_lock(void);void du_gfx_unlock(void);
void func_800B6748(u8 *,f32 *,s16 *,s16 *,s8 *,s8 *,s8 *);
char *func_800A473C(char *,const char *);char *func_80092DCC(char *,const char *,u32);
void *memcpy(void *,const void *,u32);
void menu_input_process(void *text, signed short numChars)
{
    int size;
    int length;
    char str[256];

    if (D_80114744 >= 0.0f) {
        audio_doppler_calc(text, func_800B66B0(text, numChars));
        return;
    }
    if (D_8011473C) {
        if (numChars < 0)
            func_800A473C(str, text);
        else
            func_80092DCC(str, text, numChars);
    }

    du_gfx_lock();
    if (!D_8011474C) {
        D_8011474C = 1;
        D_80149DD0 = 0;
        func_800B6748(D_80153F60.fore.rgba, &D_80153F80,
                     &D_80153FD0, &D_80154180,
                     &D_80154184, &D_80154194, &D_8015419C);
    }

    size = 0;
    if (D_80154184 != D_80149DA0) size = 2;
    if (D_80154194 != D_80149B60) size += 2;
    if (D_8015419C != D_80149B70) size += 2;
    if (D_80153FD0 != D_80149D92 || D_80154180 != D_80149D9E)
        size += 5;
    if (D_80153F80 != D_80114748) size += 5;
    if (D_80153F60.fore.rgbaU32 != D_80149B48.fore.rgbaU32)
        size += 5;

    length = func_800B66B0(text, numChars);
    size += length + 2;
    if (D_80149DD0 + size >= 800) {
        du_gfx_unlock();
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
        D_80149DD8[D_80149DD0++] = 3;
        D_80149DD8[D_80149DD0++] = (D_80149D92 >> 8) & 255;
        D_80149DD8[D_80149DD0++] = D_80149D92;
        D_80149DD8[D_80149DD0++] = (D_80149D9E >> 8) & 255;
        D_80149DD8[D_80149DD0++] = D_80149D9E;
        D_80153FD0 = D_80149D92;
        D_80154180 = D_80149D9E;
    }
    if (D_80153F80 != D_80114748) {
        D_80153F80 = D_80114748;
        D_80149DD8[D_80149DD0++] = 4;
        D_80149DD8[D_80149DD0++] = (*(unsigned int *)&D_80114748) >> 24;
        D_80149DD8[D_80149DD0++] = (*(unsigned int *)&D_80114748) >> 16;
        D_80149DD8[D_80149DD0++] = (*(unsigned int *)&D_80114748) >> 8;
        D_80149DD8[D_80149DD0++] = *(unsigned int *)&D_80114748;
    }
    if (D_80153F60.fore.rgbaU32 != D_80149B48.fore.rgbaU32) {
        D_80153F60.fore.rgbaU32 = D_80149B48.fore.rgbaU32;
        D_80149DD8[D_80149DD0++] = 5;
        memcpy(D_80149DD8 + D_80149DD0, &D_80149B48, 4);
        D_80149DD0 += 4;
    }

    D_80149DD8[D_80149DD0++] = 6;
    D_80149DD8[D_80149DD0++] = length;
    memcpy(D_80149DD8 + D_80149DD0, text, length);
    D_80149DD0 += length;
    du_gfx_unlock();
}
