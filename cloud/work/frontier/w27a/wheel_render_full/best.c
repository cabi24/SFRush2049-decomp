/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * wheel_render_full (0x800A6BE4, 2072 B): historical label. Twin of car_render_full (0x800A6404): the
 * same N64 split-screen viewport layout, except the 2-player layout uses two 3/4-width panes starting at
 * x = 2 (width 3W/4 + 2, centre 3W/8 + 2) with the unscaled fov. Cases 1 and 3/4 are identical to
 * car_render_full (0.95f quarter fov). No arcade counterpart.
 *
 * Source-form notes: as car_render_full (helper here view_bind_w, non-static, force_internal in the unit) (inlined static view_bind whose unused return value is the dead
 * v0 copy before each next call; block-scoped u16 rect locals; per-pane store order). The constant 3 is
 * one coloured web here (n == 3 compare, pane index 3, the W * 3 / H * 3 multiplies: multu by s4).
 * The second 2-player pane stores h, x, w, y (as1 order). No unused locals. w27a.
 */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;

typedef struct {
    void *f0;
    void *f4;
    u8 unk8[0x40];
} ArbEntry;

extern u8 D_80146204;
extern s8 D_8017A63C;
extern s32 D_8002AFC0, D_8002AFC4;
extern f32 D_80154188;
extern s32 D_8011EA30, D_8011EA40, D_8011EA50, D_8011EA60, D_8011EA70, D_8011EA80, D_8011EA90;
extern s16 D_80149870[4], D_80149B00[4], D_80149B20[4], D_80149B40[4], D_80149B58[4], D_80149B68[4], D_80149B78[4];
extern ArbEntry D_8017A510[];

ArbEntry *view_bind_w(s32 k, void *vp, s16 *r) {
    D_8017A510[k].f0 = vp;
    D_8017A510[k].f4 = r;
    return &D_8017A510[k];
}

ArbEntry *arb_rate_set(s32 idx, void *a, void *b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h);

s32 wheel_render_full(s32 n) {
    if (n > 0) {
        D_80146204 = n;
        if (n == 1) {
            arb_rate_set(0, &D_8011EA30, D_80149870, D_80154188, 0.0f, (f32)D_8002AFC0, (f32)D_8002AFC4,
                         (f32)(D_8002AFC0 / 2), (f32)(D_8002AFC4 / 2));
            {
                u16 w, h;
                w = D_8002AFC0; h = D_8002AFC4;
                D_80149870[3] = h;
                D_80149870[2] = w;
                D_80149870[0] = 0;
                D_80149870[1] = 0;
                view_bind_w(0, &D_8011EA30, D_80149870);
            }
            D_8017A63C = 0;
        } else if (n == 2) {
            arb_rate_set(0, &D_8011EA40, D_80149B00, D_80154188, 0.0f,
                         (f32)(D_8002AFC0 * 3 / 4), (f32)(D_8002AFC4 / 2), (f32)(D_8002AFC0 * 3 / 8 + 2), (f32)(D_8002AFC4 / 4));
            {
                u16 w, h;
                w = D_8002AFC0 * 3 / 4 + 2; h = D_8002AFC4 / 2 - 1;
                D_80149B00[3] = h;
                D_80149B00[2] = w;
                D_80149B00[0] = 2;
                D_80149B00[1] = 0;
                view_bind_w(0, &D_8011EA40, D_80149B00);
            }
            arb_rate_set(1, &D_8011EA50, D_80149B20, D_80154188, 0.0f,
                         (f32)(D_8002AFC0 * 3 / 4), (f32)(D_8002AFC4 / 2), (f32)(D_8002AFC0 * 3 / 8 + 2), (f32)(D_8002AFC4 * 3 / 4));
            {
                u16 y, w, h;
                y = D_8002AFC4 / 2 + 1; w = D_8002AFC0 * 3 / 4 + 2; h = D_8002AFC4;
                D_80149B20[3] = h;
                D_80149B20[0] = 2;
                D_80149B20[2] = w;
                D_80149B20[1] = y;
                view_bind_w(1, &D_8011EA50, D_80149B20);
            }
            D_8017A63C = 1;
        } else if (n == 3 || n == 4) {
            arb_rate_set(0, &D_8011EA60, D_80149B40, D_80154188 * 0.95f, 0.0f,
                         (f32)(D_8002AFC0 / 2), (f32)(D_8002AFC4 / 2), (f32)(D_8002AFC0 / 4 + 1), (f32)(D_8002AFC4 / 4 + 1));
            {
                u16 w, h;
                w = D_8002AFC0 / 2 - 1; h = D_8002AFC4 / 2 - 1;
                D_80149B40[3] = h;
                D_80149B40[2] = w;
                D_80149B40[0] = 0;
                D_80149B40[1] = 0;
                view_bind_w(0, &D_8011EA60, D_80149B40);
            }
            arb_rate_set(1, &D_8011EA70, D_80149B58, D_80154188 * 0.95f, 0.0f,
                         (f32)(D_8002AFC0 / 2), (f32)(D_8002AFC4 / 2), (f32)(D_8002AFC0 * 3 / 4 - 2), (f32)(D_8002AFC4 / 4 + 1));
            {
                u16 x, w, h;
                x = D_8002AFC0 / 2 + 1; w = D_8002AFC0; h = D_8002AFC4 / 2 - 1;
                D_80149B58[3] = h;
                D_80149B58[2] = w;
                D_80149B58[0] = x;
                D_80149B58[1] = 0;
                view_bind_w(1, &D_8011EA70, D_80149B58);
            }
            arb_rate_set(2, &D_8011EA80, D_80149B68, D_80154188 * 0.95f, 0.0f,
                         (f32)(D_8002AFC0 / 2), (f32)(D_8002AFC4 / 2 + 2), (f32)(D_8002AFC0 / 4 + 1), (f32)(D_8002AFC4 * 3 / 4 + 2));
            {
                u16 y, w, h;
                y = D_8002AFC4 / 2 + 1; w = D_8002AFC0 / 2 - 1; h = D_8002AFC4;
                D_80149B68[3] = h;
                D_80149B68[2] = w;
                D_80149B68[1] = y;
                D_80149B68[0] = 0;
                view_bind_w(2, &D_8011EA80, D_80149B68);
            }
            arb_rate_set(3, &D_8011EA90, D_80149B78, D_80154188 * 0.95f, 0.0f,
                         (f32)(D_8002AFC0 / 2), (f32)(D_8002AFC4 / 2 + 2), (f32)(D_8002AFC0 * 3 / 4 - 2), (f32)(D_8002AFC4 * 3 / 4 + 2));
            {
                u16 x, y, w, h;
                x = D_8002AFC0 / 2 + 1; y = D_8002AFC4 / 2 + 1; w = D_8002AFC0; h = D_8002AFC4;
                D_80149B78[3] = h;
                D_80149B78[2] = w;
                D_80149B78[1] = y;
                D_80149B78[0] = x;
                view_bind_w(3, &D_8011EA90, D_80149B78);
            }
            D_8017A63C = 2;
        }
    }
    return D_8017A63C;
}
