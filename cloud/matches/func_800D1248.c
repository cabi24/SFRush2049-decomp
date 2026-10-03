/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef float f32;
extern s16 D_8014A96C;
void math_utility(void *arg0, void *arg1);
void func_800D11BC(void *arg0);
/* N64 port of the arcade check_if_finished_resurrecting (game/resurrect.c),
 * applied to one model. */
void func_800D1248(void *arg0)
{
    s16 i;
    s16 idx;
    if (*(s16 *)((s8 *)arg0 + 0x6C4) != -2) {
        return;
    }
    /* A three-level debug guard whose body is compiled out of the retail
     * build. Its empty blocks are what make ugen address arg0 through a2,
     * as the retail code does; the tests themselves are not recoverable from
     * the binary, so these operands are placeholders (owner-accepted,
     * 2026-10-03; cloud/work/near_miss_800D1248_round12/STATUS.md). */
    if (D_8014A96C) {
        if (D_8014A96C) {
            if (D_8014A96C) {
            }
        }
    }
    *(s16 *)((s8 *)arg0 + 0x71C) = 0;
    *(s16 *)((s8 *)arg0 + 0x6C4) = -1;
    *(f32 *)((s8 *)arg0 + 0x6D4) = *(f32 *)((s8 *)arg0 + 0x660);
    *(f32 *)((s8 *)arg0 + 0x6D8) = *(f32 *)((s8 *)arg0 + 0x664);
    *(f32 *)((s8 *)arg0 + 0x6DC) = *(f32 *)((s8 *)arg0 + 0x668);
    math_utility((u8 *)arg0 + 0x678, (u8 *)arg0 + 0x6E0);
    func_800D11BC(arg0);
    if ((f32)*(s32 *)((s8 *)arg0 + 0x6BC) >= 44.0f) {
        *(s8 *)((s8 *)arg0 + 0x730) = 2;
        *(s16 *)((s8 *)arg0 + 0x3F6) = 2;
        *(s16 *)((s8 *)arg0 + 0x3F4) = 2;
    } else {
        *(s8 *)((s8 *)arg0 + 0x730) = 1;
        *(s16 *)((s8 *)arg0 + 0x3F6) = 1;
        *(s16 *)((s8 *)arg0 + 0x3F4) = 1;
    }
    *(f32 *)((s8 *)arg0 + 0x48) = *(s32 *)((s8 *)arg0 + 0x6BC);
    i = 0;
    do {
        *(f32 *)((s8 *)arg0 + i * 0xC + 0x9C) = *(s32 *)((s8 *)arg0 + 0x6BC);
        *(f32 *)((s8 *)arg0 + i * 0x5C + 0x478) =
            *(f32 *)((s8 *)arg0 + i * 0xC + 0x9C) / *(f32 *)((s8 *)arg0 + i * 0x5C + 0x430);
        i++;
    } while (i < 4);
    idx = *(s16 *)((s8 *)arg0 + 0x7C6);
    *(s16 *)((u8 *)&D_8014A96C + idx * 0x808) = 1;
}
