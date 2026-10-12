/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800A5A40 (0x800A5A40, 252 B) -- full-screen viewport 0 setup.
 * Calls arb_rate_set(0, ...) with the screen size D_8002AFC0/D_8002AFC4 (w, h), their halves
 * and 0.0f, then re-binds entry 0 of D_8017A510 (stride 0x48) to &D_8011EA30 and the s16 rect
 * D_80149870 = {0, 0, (u16)w, (u16)h}. N64-specific (no arcade counterpart found).
 *
 * Shaping devices (disclosed):
 *  - arb_rate_set returns its entry pointer (retail arb_rate_set reloads &D_8017A510[idx] into v0
 *    before `jr ra`), so it is declared returning ArbEntry *; the result is kept in `e` and read by a
 *    compiled-out `if (e) {}` at the end. That keeps v0 busy (h/w colour a0/v1 as retail) and ends the
 *    block before the epilogue (retail `lw ra` after the stores, `jr ra; nop`).
 *  - `w = ...; h = ...;` on ONE source line: as1 breaks aftercycles ties by .loc line, and retail's
 *    lui/lhu interleave (lui v1, lui a0, lhu a0, lhu v1) needs both loads on the same line.
 * Unit: blob_unit score func_800A5A40 --neighbours -> EQUAL, 0 locked bodies differ.
 */
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;

typedef struct {
    void *f0;
    void *f4;
    u8 unk8[0x40]; /* fields 0x08-0x47 set by arb_rate_set */
} ArbEntry;

extern s32 D_8002AFC0;
extern s32 D_8002AFC4;
extern u8 D_80146204;
extern u8 D_8017A63C;
extern s32 D_8011EA30;
extern s16 D_80149870[4];
extern s32 D_80154188;
extern ArbEntry D_8017A510[];

ArbEntry *arb_rate_set(s32 idx, void *a, void *b, s32 c, f32 d, f32 e, f32 f, f32 g, f32 h);

void func_800A5A40(void) {
    ArbEntry *e;
    u16 h;
    u16 w;

    D_80146204 = 1;
    D_8017A63C = 0;
    e = arb_rate_set(0, &D_8011EA30, D_80149870, D_80154188, 0.0f, (f32)D_8002AFC0, (f32)D_8002AFC4,
                 (f32)(D_8002AFC0 / 2), (f32)(D_8002AFC4 / 2));
    w = D_8002AFC0; h = D_8002AFC4;
    D_80149870[0] = 0;
    D_80149870[1] = 0;
    D_8017A510[0].f0 = &D_8011EA30;
    D_8017A510[0].f4 = D_80149870;
    D_80149870[3] = h;
    D_80149870[2] = w;
    if (e) {}
}
