R='parts/func_8038CCE4.c'
L1=(R, "    f32 dx0, dy0, dx1, dy1, dx2, dy2;\n", "    f32 dx0, dy0, len0, dx1, dy1, len1, dx2, dy2, len2;\n")
L2=(R, "                f32 len0, len1, len2;\n", "")
VARIANTS = {
 'base': [],
 'len_fn': [L1, L2],
 'len_fn_x': [L1, L2, (R, "    f32 xoffset, yoffset, zoffset;\n    s32 trackMBIdx;\n", "    f32 pad0;\n    f32 xoffset, yoffset, zoffset;\n    s32 pad1, pad2;\n    s32 trackMBIdx;\n")],
}
