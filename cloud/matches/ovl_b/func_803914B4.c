/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image B, stream 0xB6FEC4, 0x803914B4..0x80391640 (396 bytes).
 * Queue one 24-byte HUD event. Negative deltas update the real score table
 * before optional animation, and nonpositive player counters are cleared.
 * The consumer at B:803936A8 confirms the four records and every field.
 * N64-specific source reconstruction; no identified arcade equivalent.
 * Ordinary three-signed-byte O32 interface; standalone IDO 5.3 -O3.
 * Valid player/column/ring indices and backed coordinate-table rows required.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef int s32;
typedef float f32;

typedef struct Player {
    u8 unknown000[0x3A3];
    s8 remaining;
    u8 unknown3A4[0x14];
} Player;

typedef struct Event {
    s8 active;
    s8 age;
    s8 column;
    s8 row;
    f32 from_x;
    f32 from_y;
    f32 to_x;
    f32 to_y;
    s8 delta;
    u8 unknown15[3];
} Event;

extern void func_800F7E30(s8 row, s8 column, s8 delta);
extern s8 D_80146130;
extern Player D_80152818[];
extern s8 D_80395ED0;
extern Event D_80395E70[];
extern s16 D_80151AD0;
extern s32 D_80115F28[][4][2];
extern s32 D_803940D0[][4][2];

void func_803914B4(s8 row, s8 column, s8 delta) {
    Event *event;
    s32 index;
    if (delta < 0) {
        func_800F7E30(row, column, delta);
        if (D_80146130 == 0) {
            return;
        }
        if (D_80152818[row].remaining <= 0) {
            D_80152818[row].remaining = 0;
            return;
        }
    }
    index = D_80395ED0++;
    if (D_80395ED0 >= 4) {
        D_80395ED0 = 0;
    }
    event = &D_80395E70[index];
    event->active = 1;
    event->age = 0;
    event->row = row;
    event->column = column;
    event->delta = delta;
    event->from_x = D_80115F28[D_80151AD0 - 1][column][0];
    event->from_y = D_80115F28[D_80151AD0 - 1][column][1];
    event->to_x = D_803940D0[D_80151AD0 - 1][row][0];
    event->to_y = D_803940D0[D_80151AD0 - 1][row][1];
}
