/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef struct { u32 w0; u32 w1; } Gfx;
extern Gfx *D_80149438;
extern u8 *D_8012E6D0;

#define GFX_NEXT() (D_80149438++)
#define CMD(w0v, w1v) do { g = GFX_NEXT(); g->w1 = (w1v); g->w0 = (w0v); } while (0)
#define BE32(p) (((u32)(p)[0] << 24) | ((u32)(p)[1] << 16) | ((u32)(p)[2] << 8) | (u32)(p)[3])

void func_8008A148(u8 *img, s32 mode, s32 flag, s32 tile) {
    Gfx *g;
    if (img != D_8012E6D0) {
        if ((mode == 2 || mode == 5) && flag == 1) {
            CMD(0xFD100000, (u32)img);
            CMD(0xE8000000, 0);
            CMD(0xF5000100, 0x07000000);
            CMD(0xE6000000, 0);
            CMD(0xF0000000, 0x073FC000);
            CMD(0xE7000000, 0);
        } else if (mode == 2 && flag == 0) {
            CMD(0xFD100000, (u32)img);
            CMD(0xE8000000, 0);
            CMD(0xF5000000 | ((((tile & 0xF) << 4) + 256) & 0x1FF), 0x07000000);
            CMD(0xE6000000, 0);
            CMD(0xF0000000, 0x073FC000);
            CMD(0xE7000000, 0);
        } else if (mode == 4 || mode == 3) {
            g = GFX_NEXT();
            g->w0 = 0xFA000000;
            g->w1 = BE32(img);
            g = GFX_NEXT();
            g->w0 = 0xFB000000;
            g->w1 = BE32(img + 4);
        }
        D_8012E6D0 = img;
    }
}
