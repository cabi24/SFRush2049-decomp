BASE = 'work/base_w36e'
O = "                if (D_80146108[29] < 0) D_80146108[29] = 3;"
def v(stmt):
    return [(O, "                if (D_80146108[29] < 0) " + stmt)]
VARIANTS = {
 'blkext': v("{ extern s8 D_80146108[]; D_80146108[29] = 3; }"),
 'blkext_u8': v("{ extern u8 D_80146108[]; D_80146108[29] = 3; }"),
 'blkext_41': v("{ extern s8 D_80146108[41]; D_80146108[29] = 3; }"),
 'blkext_s16': v("{ extern s16 D_80146108[]; ((s8 *)D_80146108)[29] = 3; }"),
}
