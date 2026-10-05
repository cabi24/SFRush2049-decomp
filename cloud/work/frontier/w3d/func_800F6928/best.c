typedef float f32;
typedef int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef struct { f32 x, y; } Point;
extern Point D_80114750[];
extern f32 D_80114744;
extern u32 D_80149B88;
extern void func_8008A644(u16);

void func_800F6928(f32 value) {
    s32 i;
    f32 a, b, d;

    i = 0;
    while (value > D_80114750[i].x && D_80114750[i].x < 32000.0f) {
        i++;
    }
    if (i > 0) {
        d = D_80114750[i].x - D_80114750[i - 1].x;
        b = D_80114750[i].y - D_80114750[i - 1].y;
        a = value - D_80114750[i - 1].x;
        value = a * b / d;
        value += D_80114750[i - 1].y;
    }
    D_80114744 = value;
    if (value > 0.0f) {
        D_80149B88 |= 0x10;
        func_8008A644(value);
    } else {
        D_80149B88 &= ~0x10;
    }
}
