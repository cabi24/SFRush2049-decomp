/* Complete source reconstruction of 0x8008A46C; research, not a match.
 * Coordinates are signed inclusive pixel bounds. Color is four RGBA bytes.
 * The fifth argument is a genuine o32 stack argument at incoming sp+16.
 */
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;
typedef struct { u32 w0, w1; } RectangleGfx;
extern s32 D_8012E60C, D_8012E668, D_8012E610, D_8012E674;
extern RectangleGfx *D_80149438;
extern u8 D_8011EACF;
extern void func_8008A148(const u8 *, s32, s32, s32);
extern void func_80086A50(s32);
extern void func_800878E0(s32);
extern void func_8008705C(s32);

void func_8008A46C(s32 left, s32 top, s32 right, s32 bottom,
                   const u8 *rgba)
{
    RectangleGfx *command;
    if (left < D_8012E60C) left = D_8012E60C;
    if (top < D_8012E668) top = D_8012E668;
    if (right > D_8012E610) right = D_8012E610;
    if (bottom > D_8012E674) bottom = D_8012E674;
    if (right < left || bottom < top) return;

    command = D_80149438++;
    command->w0 = 0xE7000000;
    command->w1 = 0;
    command = D_80149438++;
    command->w0 = 0xFA000000;
    command->w1 = rgba[3] | ((u32)rgba[0] << 24) |
                  ((u32)rgba[1] << 16) | ((u32)rgba[2] << 8);
    D_8011EACF = rgba[3];
    func_8008A148(rgba, -1, -1, 0);
    func_80086A50(1);
    func_800878E0(0x4000);

    command = D_80149438++;
    command->w0 = 0xF6000000 | (((u32)(right + 1) & 0x3FF) << 14) |
                  (((u32)(bottom + 1) & 0x3FF) << 2);
    command->w1 = (((u32)left & 0x3FF) << 14) |
                  (((u32)top & 0x3FF) << 2);
    command = D_80149438++;
    command->w0 = 0xE7000000;
    command->w1 = 0;
    func_8008705C(0x4000);
}
