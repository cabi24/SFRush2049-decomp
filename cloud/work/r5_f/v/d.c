typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
extern u16 D_8012E67A;
typedef struct { u32 w0; u32 w1; } Gfx;
extern Gfx *D_80149438;
void func_800878E0(s32 arg0);

void func_8008A644(u16 arg0) {
    if (D_8012E67A != arg0) {
        Gfx *g = D_80149438++;
        g->w0 = 0xEE000000;
        g->w1 = arg0 << 16;
        D_8012E67A = arg0;
    }
    func_800878E0(16);
}
