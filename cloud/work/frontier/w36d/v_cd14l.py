R='parts/func_8038CCE4.c'
OLD="""    f32 dx0, dy0, len0, dx1, dy1, len1, dx2, dy2, len2;
    f32 xoffset, yoffset, zoffset;
    s32 trackMBIdx;
    RomObjDef *trackRomObjDef;
    Gfx *dl;
    Vtx_t *vtx;
    s32 i;
    f32 sectVerts[8][3];
    s32 vtxSize, dlSize;
"""
def mk(a, b, c, tail=''):
    return OLD, f"""    f32 dx0, dy0, len0, dx1, dy1, len1, dx2, dy2, len2;
{a}    f32 xoffset, yoffset, zoffset;
{b}    s32 trackMBIdx;
    RomObjDef *trackRomObjDef;
    Gfx *dl;
    Vtx_t *vtx;
    s32 vtxSize, dlSize;
    s32 i;
{c}    f32 sectVerts[8][3];
{tail}"""
VARIANTS = {
 'base': [],
 'fill_a': [(R,)+mk("    f32 u0;\n", "    s32 u1, u2;\n", "    s32 u3, u4, u5, u6, u7;\n")],
 'fill_b': [(R,)+mk("    f32 u0;\n", "    s32 u1, u2;\n", "    s32 u3, u4, u5, u6;\n")],
 'fill_c': [(R,)+mk("    f32 u0;\n", "    s32 u1, u2;\n", "    s32 u3, u4, u5, u6, u7, u8;\n")],
}
