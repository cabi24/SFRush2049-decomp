/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;

typedef struct Viewport {
    s16 scale[4];
    s16 trans[4];
} Viewport;

typedef struct ClipRect {
    s16 left, top, right, bottom;
} ClipRect;

typedef struct ViewConfig {
    Viewport *viewport;
    ClipRect *clip;
    char rest[64];
} ViewConfig;

extern s32 D_8002AFC0, D_8002AFC4;
extern s8 D_80146204, D_8017A63C;
extern f32 D_80154188;
extern f32 D_80123BEC, D_80123BF0, D_80123BF4, D_80123BF8;
extern Viewport D_8011EA30, D_8011EA40, D_8011EA50, D_8011EA60;
extern Viewport D_8011EA70, D_8011EA80, D_8011EA90;
extern ClipRect D_80149870, D_80149B00, D_80149B20, D_80149B40;
extern ClipRect D_80149B58, D_80149B68, D_80149B78;
extern ViewConfig D_8017A510[];
extern void arb_rate_set(s32, s16 *, s16 *, f32, f32, f32, f32, f32, f32);

s32 wheel_render_full(s32 arg0) {
    s32 temp_lo;
    s32 temp_t1;
    s32 temp_v1;

    if (arg0 > 0) {
        D_80146204 = arg0;
        if (arg0 == 1) {
            arb_rate_set(0, (s16 *)&D_8011EA30, (s16 *)&D_80149870, D_80154188, 0.0f, (f32) D_8002AFC0, (f32) D_8002AFC4, (f32) ((s32) D_8002AFC0 / 2), (f32) ((s32) D_8002AFC4 / 2));
            D_80149870.left = 0;
            D_80149870.top = 0;
            D_8017A510[0].viewport = &D_8011EA30;
            D_8017A510[0].clip = &D_80149870;
            D_80149870.right = ((unsigned short *)&D_8002AFC0)[1];
            D_80149870.bottom = ((unsigned short *)&D_8002AFC4)[1];
            D_8017A63C = 0;
        } else if (arg0 == 2) {
            temp_lo = D_8002AFC0 * 3;
            arb_rate_set(0, (s16 *)&D_8011EA40, (s16 *)&D_80149B00, D_80154188, 0.0f, (f32) (temp_lo / 4), (f32) ((s32) D_8002AFC4 / 2), (f32) ((temp_lo / 8) + 2), (f32) ((s32) D_8002AFC4 / 4));
            temp_lo = D_8002AFC0 * 3;
            D_80149B00.left = 2;
            D_80149B00.top = 0;
            D_8017A510[0].clip = &D_80149B00;
            temp_t1 = temp_lo / 4;
            temp_v1 = (s32) D_8002AFC4 / 2;
            D_80149B00.bottom = temp_v1 - 1;
            D_80149B00.right = temp_t1 + 2;
            D_8017A510[0].viewport = &D_8011EA40;
            arb_rate_set(1, (s16 *)&D_8011EA50, (s16 *)&D_80149B20, D_80154188, 0.0f, (f32) temp_t1, (f32) temp_v1, (f32) ((temp_lo / 8) + 2), (f32) ((s32) (D_8002AFC4 * 3) / 4));
            D_80149B20.left = 2;
            D_80149B20.right = ((s32) (D_8002AFC0 * 3) / 4) + 2;
            D_80149B20.top = ((s32) D_8002AFC4 / 2) + 1;
            D_8017A510[1].viewport = &D_8011EA50;
            D_8017A510[1].clip = &D_80149B20;
            D_80149B20.bottom = (s16) D_8002AFC4;
            D_8017A63C = 1;
        } else if ((arg0 == 3) || (arg0 == 4)) {
            arb_rate_set(0, (s16 *)&D_8011EA60, (s16 *)&D_80149B40, D_80154188 * D_80123BEC, 0.0f, (f32) ((s32) D_8002AFC0 / 2), (f32) ((s32) D_8002AFC4 / 2), (f32) (((s32) D_8002AFC0 / 4) + 1), (f32) (((s32) D_8002AFC4 / 4) + 1));
            temp_t1 = (s32) D_8002AFC0 / 2;
            temp_v1 = (s32) D_8002AFC4 / 2;
            D_80149B40.bottom = temp_v1 - 1;
            D_80149B40.right = temp_t1 - 1;
            D_80149B40.left = 0;
            D_80149B40.top = 0;
            D_8017A510[0].viewport = &D_8011EA60;
            D_8017A510[0].clip = &D_80149B40;
            arb_rate_set(1, (s16 *)&D_8011EA70, (s16 *)&D_80149B58, D_80154188 * D_80123BF0, 0.0f, (f32) temp_t1, (f32) temp_v1, (f32) (((s32) (D_8002AFC0 * 3) / 4) - 2), (f32) (((s32) D_8002AFC4 / 4) + 1));
            temp_t1 = (s32) D_8002AFC0 / 2;
            temp_v1 = (s32) D_8002AFC4 / 2;
            D_80149B58.bottom = temp_v1 - 1;
            D_80149B58.right = (s16) D_8002AFC0;
            D_80149B58.left = temp_t1 + 1;
            D_80149B58.top = 0;
            D_8017A510[1].viewport = &D_8011EA70;
            D_8017A510[1].clip = &D_80149B58;
            arb_rate_set(2, (s16 *)&D_8011EA80, (s16 *)&D_80149B68, D_80154188 * D_80123BF4, 0.0f, (f32) temp_t1, (f32) (temp_v1 + 2), (f32) (((s32) D_8002AFC0 / 4) + 1), (f32) (((s32) (D_8002AFC4 * 3) / 4) + 2));
            temp_v1 = (s32) D_8002AFC4 / 2;
            temp_t1 = (s32) D_8002AFC0 / 2;
            D_80149B68.bottom = (s16) D_8002AFC4;
            D_80149B68.right = temp_t1 - 1;
            D_80149B68.top = temp_v1 + 1;
            D_80149B68.left = 0;
            D_8017A510[2].viewport = &D_8011EA80;
            D_8017A510[2].clip = &D_80149B68;
            arb_rate_set(3, (s16 *)&D_8011EA90, (s16 *)&D_80149B78, D_80154188 * D_80123BF8, 0.0f, (f32) temp_t1, (f32) (temp_v1 + 2), (f32) (((s32) (D_8002AFC0 * 3) / 4) - 2), (f32) (((s32) (D_8002AFC4 * 3) / 4) + 2));
            D_80149B78.top = ((s32) D_8002AFC4 / 2) + 1;
            D_80149B78.left = ((s32) D_8002AFC0 / 2) + 1;
            D_8017A510[3].viewport = &D_8011EA90;
            D_8017A510[3].clip = &D_80149B78;
            D_80149B78.right = (s16) D_8002AFC0;
            D_80149B78.bottom = (s16) D_8002AFC4;
            D_8017A63C = 2;
        }
    }
    return D_8017A63C;
}
