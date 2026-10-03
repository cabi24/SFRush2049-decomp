/* Host-only observer: records helper order; models legal display-list advancement.
 * These stubs are never part of an IDO match candidate or closure. */
#include "candidate.c"
s32 D_8012E60C, D_8012E668, D_8012E610, D_8012E674;
RectangleGfx *D_80149438;
u8 D_8011EACF;
static RectangleGfx buffer[32];
static s32 calls[4], count;
static const u8 *expected_color;
static s32 valid;
static void observe(s32 id, s32 argument)
{
    RectangleGfx *command;
    if (count >= 4) { valid = 0; return; }
    calls[count++] = id;
    command = D_80149438++;
    command->w0 = 0xAB000000U | (u32)id;
    command->w1 = (u32)argument;
}
void func_8008A148(const u8 *rgba, s32 a, s32 b, s32 c)
{
    if (rgba != expected_color || a != -1 || b != -1 || c != 0 ||
        D_8011EACF != rgba[3]) valid = 0;
    observe(1, a);
}
void func_80086A50(s32 mode) { if (mode != 1) valid = 0; observe(2, mode); }
void func_800878E0(s32 flags) { if (flags != 0x4000) valid = 0; observe(3, flags); }
void func_8008705C(s32 flags) { if (flags != 0x4000) valid = 0; observe(4, flags); }
s32 rectangle_run(s32 l, s32 t, s32 r, s32 b, const u8 *color,
                  s32 cl, s32 ct, s32 cr, s32 cb, u32 *out)
{
    s32 i;
    D_8012E60C = cl; D_8012E668 = ct; D_8012E610 = cr; D_8012E674 = cb;
    D_80149438 = buffer; D_8011EACF = 0xA5;
    count = 0; valid = 1; expected_color = color;
    for (i = 0; i < 32; i++) buffer[i].w0 = buffer[i].w1 = 0xDEADBEEF;
    func_8008A46C(l, t, r, b, color);
    out[0] = (u32)(D_80149438 - buffer); out[1] = D_8011EACF;
    out[2] = count; out[3] = valid;
    for (i = 0; i < 32; i++) { out[4 + i*2] = buffer[i].w0; out[5 + i*2] = buffer[i].w1; }
    return valid;
}
