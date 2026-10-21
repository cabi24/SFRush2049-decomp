BASE = 'work/base_w36e'
O = "                if (D_80146108[29] < 0) D_80146108[29] = 3;"
H = "extern s8 D_80146108[];            /* gOptions */"
def v(x, hdr=None):
    e = [(O, "                if (D_80146108[29] < 0) " + x + " = 3;")]
    if hdr: e.append((H, H + "\n" + hdr))
    return e
VARIANTS = {
 'base': [],
 'alias': v("D_8014611D[8]", "extern s8 D_8014611D[];"),
 'const_addr': v("*(s8 *)0x80146125"),
 'u32cast': v("*(s8 *)((u32)D_80146108 + 29)"),
 'sub21': v("((s8 *)&D_80146108[21])[8]"),
 'vol': v("*(volatile s8 *)&D_80146108[29]"),
 'struct': v("((struct { s8 pad[29]; s8 fog; } *)D_80146108)->fog"),
 'u8': v("*(u8 *)&D_80146108[29]"),
 'arr2d': v("((s8 (*)[41])D_80146108)[0][29]"),
 'idxexpr': v("D_80146108[add + 29 - add]"),
}
