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
extern void arb_rate_set(s32, Viewport *, ClipRect *, f32, f32, f32, f32, f32, f32);

s32 wheel_render_full(s32 players)
{
    s32 width, height;

    if (players > 0) {
        D_80146204 = players;
        switch (players) {
        case 1:
            width = D_8002AFC0;
            height = D_8002AFC4;
            arb_rate_set(0, &D_8011EA30, &D_80149870, D_80154188,
                         0.0f, width, height, width / 2, height / 2);
            D_80149870.left = 0;
            D_80149870.top = 0;
            D_80149870.right = D_8002AFC0;
            D_80149870.bottom = D_8002AFC4;
            D_8017A510[0].viewport = &D_8011EA30;
            D_8017A510[0].clip = &D_80149870;
            D_8017A63C = 0;
            break;
        case 2:
            width = D_8002AFC0 * 3;
            height = D_8002AFC4;
            arb_rate_set(0, &D_8011EA40, &D_80149B00, D_80154188,
                         0.0f, width / 4, height / 2, width / 8 + 2, height / 4);
            width = D_8002AFC0 * 3;
            height = D_8002AFC4;
            D_80149B00.left = 2;
            D_80149B00.top = 0;
            D_80149B00.bottom = height / 2 - 1;
            D_80149B00.right = width / 4 + 2;
            D_8017A510[0].viewport = &D_8011EA40;
            D_8017A510[0].clip = &D_80149B00;
            arb_rate_set(1, &D_8011EA50, &D_80149B20, D_80154188,
                         0.0f, width / 4, height / 2, width / 8 + 2, height * 3 / 4);
            width = D_8002AFC0 * 3;
            height = D_8002AFC4;
            D_80149B20.left = 2;
            D_80149B20.right = width / 4 + 2;
            D_80149B20.top = height / 2 + 1;
            D_80149B20.bottom = height;
            D_8017A510[1].viewport = &D_8011EA50;
            D_8017A510[1].clip = &D_80149B20;
            D_8017A63C = 1;
            break;
        case 3:
        case 4:
            width = D_8002AFC0;
            height = D_8002AFC4;
            arb_rate_set(0, &D_8011EA60, &D_80149B40, D_80154188 * D_80123BEC,
                         0.0f, width / 2, height / 2, width / 4 + 1, height / 4 + 1);
            width = D_8002AFC0;
            height = D_8002AFC4;
            D_80149B40.bottom = height / 2 - 1;
            D_80149B40.right = width / 2 - 1;
            D_80149B40.left = 0;
            D_80149B40.top = 0;
            D_8017A510[0].viewport = &D_8011EA60;
            D_8017A510[0].clip = &D_80149B40;
            arb_rate_set(1, &D_8011EA70, &D_80149B58, D_80154188 * D_80123BF0,
                         0.0f, width / 2, height / 2, width * 3 / 4 - 2, height / 4 + 1);
            width = D_8002AFC0;
            height = D_8002AFC4;
            D_80149B58.bottom = height / 2 - 1;
            D_80149B58.right = width;
            D_80149B58.left = width / 2 + 1;
            D_80149B58.top = 0;
            D_8017A510[1].viewport = &D_8011EA70;
            D_8017A510[1].clip = &D_80149B58;
            arb_rate_set(2, &D_8011EA80, &D_80149B68, D_80154188 * D_80123BF4,
                         0.0f, width / 2, height / 2 + 2, width / 4 + 1, height * 3 / 4 + 2);
            width = D_8002AFC0;
            height = D_8002AFC4;
            D_80149B68.bottom = height;
            D_80149B68.right = width / 2 - 1;
            D_80149B68.top = height / 2 + 1;
            D_80149B68.left = 0;
            D_8017A510[2].viewport = &D_8011EA80;
            D_8017A510[2].clip = &D_80149B68;
            arb_rate_set(3, &D_8011EA90, &D_80149B78, D_80154188 * D_80123BF8,
                         0.0f, width / 2, height / 2 + 2, width * 3 / 4 - 2, height * 3 / 4 + 2);
            width = D_8002AFC0;
            height = D_8002AFC4;
            D_80149B78.top = height / 2 + 1;
            D_80149B78.left = width / 2 + 1;
            D_80149B78.right = width;
            D_80149B78.bottom = height;
            D_8017A510[3].viewport = &D_8011EA90;
            D_8017A510[3].clip = &D_80149B78;
            D_8017A63C = 2;
            break;
        }
    }
    return D_8017A63C;
}
