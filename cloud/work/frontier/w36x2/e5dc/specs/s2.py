BASE = 'work/base_w36e'
O = "                if (D_80146108[29] < 0) D_80146108[29] = 3;"
D = "    s32 add;\n    if (D_801174BC != 1) {"
def v(stmt, decl=None):
    e = [(O, "                if (D_80146108[29] < 0) " + stmt)]
    if decl: e.append((D, "    s32 add;\n" + decl + "\n    if (D_801174BC != 1) {"))
    return e
VARIANTS = {
 'blkptr': v("{ s8 *p = &D_80146108[29]; *p = 3; }"),
 'fnptr': v("{ p = &D_80146108[29]; *p = 3; }", "    s8 *p;"),
 'blkptr2': v("{ s8 *p = D_80146108 + 29; *p = 3; }"),
 'blkptr3': v("{ s8 *p = D_80146108; p[29] = 3; }"),
 'blkptr4': v("{ s8 *p = &D_80146108[21]; p[8] = 3; }"),
 'blku32': v("{ u32 a = (u32)&D_80146108[29]; *(s8 *)a = 3; }"),
}
