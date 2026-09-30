typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef signed long long s64;
typedef unsigned long long u64;
typedef struct { u32 w0; u32 w1; } Gfx;

extern Gfx *D_80149438;
extern u32 D_8012E608;
extern s32 D_8014A248;
extern u32 D_8012E684;
extern s64 D_8012E688;

extern void func_80086A50();
extern void pad_use();
extern void func_800878E0();
extern s32 func_80087804();

void object_render(u32 tex, u16 fmt, u16 mode, u16 pitch, u16 h, u16 x1, u16 y0, u16 x0, u16 y1, u16 p9, s32 flag)
{
    s32 sp274;
    u16 sp272;
    s64 key;
    u64 c1, c2, c3;
    char pad[552];

    if (flag == 0) {
        if (mode == 3) {
            tex += (y0 * pitch) * 4;
        } else if (mode == 2) {
            tex += (y0 * pitch) * 2;
        } else if (mode == 1) {
            tex += y0 * pitch;
        } else {
            tex += ((s32)(y0 * pitch)) / 2;
        }
        h = y1 - y0 + 1;
    } else {
        c1 = (u64)y0 << 32;
        c2 = (u64)x1 << 48;
        c3 = (u64)x0 << 16;
        flag = (s32)c3 + (s32)c2 + (s32)c1 + y1;
    }
    if (fmt == 0 || fmt == 2) {
        u32 v = D_8012E608;
        if ((v & 0x20) || (v & 0x10) || (v & 0x8000)) sp274 = 1; else sp274 = 0;
    } else if (fmt == 5) {
        sp274 = 4;
    } else if (fmt == 4 || fmt == 3) {
        sp274 = 2;
        if (D_8012E608 & 1) sp274 = 3;
    }
    if (fmt == 3) func_800878E0(0x20);
    if (sp274 != D_8014A248) func_80086A50(sp274);
    key = flag;
    if (tex == D_8012E684 && D_8012E688 == key) return;
    if (flag != 0) {
        sp272 = func_80087804(x1 - x0);
        func_80087804(y0 - y1);
    }
    if (fmt == 99) pad_use(pad, sp272);
}
