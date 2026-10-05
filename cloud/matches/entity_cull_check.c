/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Display-list segment rebase over [start, end). Quirk: the block-local copy `off = base` only
 * serves to found base's web before the w1 load (base then lands in a3); it is copy-propagated away. */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

typedef struct Gfx {
    u32 w0;
    u32 w1;
} Gfx;

void entity_cull_check(Gfx *start, Gfx *end, s32 base) {
    Gfx *gfx;
    u8 cmd;
    u8 next;
    u32 val;
    s32 off;

    for (gfx = start; gfx < end; gfx++) {
        cmd = (gfx->w0 & 0xFF000000) >> 24;
        if ((cmd & 0xC0) != 0x40 && (cmd & 0xC0) != 0x80 && (cmd < 9 || cmd >= 0x40) && (cmd < 0xC0 || cmd >= 0xD6)) {
            next = (gfx[1].w0 & 0xFF000000) >> 24;
            if (cmd == 0xFD) {
                off = base;
                val = (gfx->w1 & 0x0F000000) | ((gfx->w1 + off) & 0x00FFFFFF);
                gfx->w1 = val;
            }
            if (cmd == 0xE1 && next == 4) {
                off = base;
                val = (gfx->w1 & 0x0F000000) | ((gfx->w1 + off) & 0x00FFFFFF);
                gfx->w1 = val;
            }
        }
    }
}
