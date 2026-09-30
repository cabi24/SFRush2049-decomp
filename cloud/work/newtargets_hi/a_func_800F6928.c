typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef struct { float x, y; } Pt;
extern Pt D_80114750[];
extern float D_80114744;
extern u32 D_80149B88;
void func_8008A644(u16 a);
void func_800F6928(float x) {
    s32 i = 0;
    while (D_80114750[i].x < x && D_80114750[i].x < 32000.0f) {
        i++;
    }
    if (i > 0) {
        x = (x - D_80114750[i - 1].x) * (D_80114750[i].y - D_80114750[i - 1].y) / (D_80114750[i].x - D_80114750[i - 1].x) + D_80114750[i - 1].y;
    }
    D_80114744 = x;
    if (x > 0.0f) {
        D_80149B88 |= 0x10;
        func_8008A644((u32)x);
    } else {
        D_80149B88 &= ~0x10;
    }
}
