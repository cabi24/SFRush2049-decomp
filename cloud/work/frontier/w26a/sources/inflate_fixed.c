/* flags: -g0 -O2 -mips2 -G 0 -non_shared (compiler ido5.3s: IDO 5.3 cc -S, .loc stripped, as1 -O1 -g) */
/* inflate_fixed - lib_5610 (ido5.3s). gzip inflate.c inflate_fixed(): build the fixed literal/
 * length (288 codes: 8/9/7/8 bits) and distance (30 codes, 5 bits) tables with huft_build and
 * decode the block with them (inflate_free_window, which is gzip's inflate_codes). Rush resets
 * the bump allocator (`gDisplayListSize = 0`, gzip's huft_free) on every exit except a decode
 * error, and maps a positive decode error to 1.
 * Draft: static_C23/inflate_fixed.best.c, with gDisplayListSize declared without the
 * `volatile` rom_tu.h had (volatile turns `lui at; sw zero` into an address register).
 * No shaping devices. */
#include "types.h"

typedef struct huft { u8 e; u8 b; union { u16 n; struct huft *t; } v; } huft;

extern u16 gInflateCplens[], gInflateCplext[], gInflateCpdist[], gInflateCpdext[];
extern unsigned int gDisplayListSize;
extern s32 huft_alloc(s32);
extern s32 huft_build(u32 *, u32, u32, u16 *, u16 *, huft **, s32 *);
extern s32 inflate_free_window(huft *, huft *, s32, s32);

s32 inflate_fixed(void) {
    s32 i;
    huft *tl, *td;
    s32 bl, bd;
    u32 *l = (u32 *)huft_alloc(288 * sizeof(u32));

    for (i = 0; i < 144; i++) l[i] = 8;
    for (; i < 256; i++) l[i] = 9;
    for (; i < 280; i++) l[i] = 7;
    for (; i < 288; i++) l[i] = 8;
    bl = 7;
    if ((i = huft_build(l, 288, 257, gInflateCplens, gInflateCplext, &tl, &bl)) != 0) {
        gDisplayListSize = 0;
        return i;
    }
    for (i = 0; i < 30; i++) l[i] = 5;
    bd = 5;
    if ((i = huft_build(l, 30, 0, gInflateCpdist, gInflateCpdext, &td, &bd)) > 1) {
        gDisplayListSize = 0;
        return i;
    }
    if ((i = inflate_free_window(tl, td, bl, bd)) != 0) return i < 0 ? i : 1;
    gDisplayListSize = 0;
    return 0;
}
