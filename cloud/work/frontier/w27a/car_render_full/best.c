/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * car_render_full (0x800A6404, 2016 B): historical label. N64 split-screen viewport layout for n players
 * (no arcade counterpart: the cabinet has one screen). Stores n in D_80146204; for n = 1, 2 and 3/4 it
 * calls arb_rate_set(k, &vp[k], rect, fov, 0, width, height, centre x, centre y) for each pane (fov
 * scaled by 127/90 for the 2-player halves, by 0.95 for the 3/4-player quarters), then fills the pane's
 * s16 rect {x, y, w, h} from the screen size D_8002AFC0/D_8002AFC4 and binds entry k of the 72-byte view
 * table D_8017A510 to (vp, rect). D_8017A63C = layout id (0 full, 1 halves, 2 quarters), returned.
 * Case 1 is the same code as the matched sibling func_800A5A40.
 *
 * Source-form notes:
 * - INTEGRATION: the helper is non-static and must be internal in the shadow unit (unit_overrides
 *   force_internal); as a `static` it leaves a 2-word deleted-procedure stub after the body that retail
 *   does not have (blob_unit: "stub tail: 2 words ... NOT as in the image").
 * - view_bind is an inlined helper (umerge inlines it at all 7 sites). Its unused return value
 *   &D_8017A510[k] is the dead `move v0,t0` / `lui v0` before the next arb_rate_set call (w15d lever:
 *   an inlined static's return value is not optimised away), and it stops uopt from holding
 *   &D_8017A510 in a callee-saved register. Disclosed as a helper-identity hypothesis.
 * - rect values go through block-scoped u16 locals (u16 narrows the screen-size reloads to lhu 2(x)
 *   and gives the a0/a1/a3/v0 copies); each pane stores h, w, then x/y (y first when nonzero), then binds.
 * - fov is passed as f32 in a3 (lw a3 for the plain case).
 * No unused locals. w27a.
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

ArbEntry *view_bind(s32 k, void *vp, s16 *r) {
    D_8017A510[k].f0 = vp;
    D_8017A510[k].f4 = r;
    return &D_8017A510[k];
}

ArbEntry *arb_rate_set(s32 idx, void *a, void *b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h);

s32 car_render_full(s32 n) {
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
                view_bind(0, &D_8011EA30, D_80149870);
            }
            D_8017A63C = 0;
        } else if (n == 2) {
            arb_rate_set(0, &D_8011EA40, D_80149B00, D_80154188 * 127.0f / 90.0f, 0.0f,
                         (f32)D_8002AFC0, (f32)(D_8002AFC4 / 2), (f32)(D_8002AFC0 / 2), (f32)(D_8002AFC4 / 4));
            {
                u16 w, h;
                w = D_8002AFC0; h = D_8002AFC4 / 2 - 1;
                D_80149B00[3] = h;
                D_80149B00[2] = w;
                D_80149B00[0] = 0;
                D_80149B00[1] = 0;
                view_bind(0, &D_8011EA40, D_80149B00);
            }
            arb_rate_set(1, &D_8011EA50, D_80149B20, D_80154188 * 127.0f / 90.0f, 0.0f,
                         (f32)D_8002AFC0, (f32)(D_8002AFC4 / 2), (f32)(D_8002AFC0 / 2), (f32)(D_8002AFC4 * 3 / 4));
            {
                u16 y, w, h;
                y = D_8002AFC4 / 2 + 1; w = D_8002AFC0; h = D_8002AFC4;
                D_80149B20[3] = h;
                D_80149B20[2] = w;
                D_80149B20[1] = y;
                D_80149B20[0] = 0;
                view_bind(1, &D_8011EA50, D_80149B20);
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
                view_bind(0, &D_8011EA60, D_80149B40);
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
                view_bind(1, &D_8011EA70, D_80149B58);
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
                view_bind(2, &D_8011EA80, D_80149B68);
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
                view_bind(3, &D_8011EA90, D_80149B78);
            }
            D_8017A63C = 2;
        }
    }
    return D_8017A63C;
}
