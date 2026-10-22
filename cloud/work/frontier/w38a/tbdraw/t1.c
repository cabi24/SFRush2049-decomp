/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct { s32 h, v; } TBJust;
extern TBJust D_80118E20;                          /* gJust */
s16 sound_pitch_diff_halved(void *text, s16 centerX);   /* TB_CenterText */
s16 sound_pitch_diff_calc(void *text, s16 rightX);      /* TB_RightText */
s16 object_bytes23_sum(void);                       /* TB_GetFontHeight */
void func_800B70EC(s16 x, s16 y);                   /* HAL_SetTextPos */
void menu_input_process(void *text, s16 numChars);  /* HAL_DrawText */

void state_utility(s16 x, s16 y, void *text)
{
    if (D_80118E20.h == 1) x = sound_pitch_diff_halved(text, x);
    else if (D_80118E20.h == 2) x = sound_pitch_diff_calc(text, x);
    if (D_80118E20.v == 1) y -= object_bytes23_sum() / 2;
    else if (D_80118E20.v == 4) y -= object_bytes23_sum();
    func_800B70EC(x, y); menu_input_process(text, -1);
}
