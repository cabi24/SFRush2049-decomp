typedef unsigned int u32; typedef unsigned short u16; typedef signed int s32;
extern u32 *gDlp;
#define PUSH(a,b) do { u32 *p_ = gDlp; gDlp = p_ + 2; p_[0] = (a); p_[1] = (b); } while (0)
void chunk(u32 tex, u16 w, s32 x0, s32 y0, u16 x1, u16 y1, u32 v0, u32 s272, u16 a9) {
    u32 t6, s1_, s0_, a3_, t0_, t1_, t2_;
    PUSH(((w - 1) & 0xFFF) | 0xFD100000, tex);
    t6 = (((((x1 - x0) * 2 + 9) >> 3) & 0x1FF) << 9) | 0xF5100000;
    s1_ = (v0 & 0xF) << 14;
    s0_ = (s272 & 0xF) * 0x10;
    PUSH(t6, s1_ | 0x07080000 | 0x200 | s0_);
    PUSH(0xE6000000, 0);
    a3_ = ((x0 * 4) & 0xFFF) << 12;
    t0_ = (y0 * 4) & 0xFFF;
    t1_ = ((x1 * 4) & 0xFFF) << 12;
    t2_ = (y1 * 4) & 0xFFF;
    PUSH(a3_ | 0xF4000000 | t0_, t1_ | 0x07000000 | t2_);
    PUSH(0xE7000000, 0);
    PUSH(t6, ((a9 & 0xF) << 20) | 0x80000 | s1_ | 0x200 | s0_);
    PUSH(a3_ | 0xF2000000 | t0_, t1_ | t2_);
}
