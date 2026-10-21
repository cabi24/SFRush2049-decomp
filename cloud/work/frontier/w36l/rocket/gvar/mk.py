import sys
T = "((Color *)D_8011AF90[D_8014978C])"
def body(decls, idx, sumv, blend_a, blend_b):
    i0,i1,i2,i3 = idx
    s = "static Color GetCarsIRColor(Car *car, CarState *m)\n{\n" + decls
    s += f"    {i0}=(car->pal[0]&0xf800)>>11;\n    {i1}=(car->pal[1]&0xf800)>>11;\n    {i2}=(car->pal[2]&0xf800)>>11;\n    {i3}=(car->pal[3]&0xf800)>>11;\n"
    for k in range(3):
        s += f"    {sumv} = {T}[{i0}].b[{k}]+{T}[{i1}].b[{k}]+{T}[{i2}].b[{k}]+{T}[{i3}].b[{k}];\n    rgb[{k}] = {sumv} >> 2;\n"
    s += f"""    if (m->dist >= 20.0f) {{
        d1 = 0.025f * (m->dist - 20.0f);
        if (d1 > 1.0f)
            return {T}[D_8011B468[D_8014978C]];
        if (d1 > 0.0f) {{
            sclr2.w = {T}[D_8011B468[D_8014978C]].w;
            d2 = 1.0f - d1;
"""
    for k in range(3):
        s += f"            {blend_a} = rgb[{k}];\n            {blend_b} = sclr2.b[{k}];\n            rgb[{k}] = {blend_a} * d2 + {blend_b} * d1;\n"
    s += """        }
    }
    clr.b[0] = rgb[0];
    clr.b[1] = rgb[1];
    clr.b[2] = rgb[2];
    clr.b[3] = 255;
    return clr;
}
"""
    return s
head = "    Color clr;\n    int u6;\n    Color sclr2;\n    unsigned char rgb[3];\n    float d1;\n    float d2;\n"
tail = "    int u8,u9,u10,u11,u12,u13,u14,u15;\n"
V = {
 'V1': (head+"    unsigned int a;\n    unsigned int b;\n    unsigned int i0;\n    unsigned int i1;\n    unsigned int i2;\n    unsigned int i3;\n"+tail, ('i0','i1','i2','i3'), 'b', 'a', 'b'),
 'V2': (head+"    unsigned int a;\n    unsigned int b;\n    unsigned int hobj;\n    unsigned int c;\n    unsigned int d;\n    int u7;\n"+tail, ('a','hobj','c','d'), 'b', 'a', 'b'),
 'V3': (head+"    unsigned int a;\n    unsigned int b;\n    unsigned int hobj;\n    unsigned int c;\n    unsigned int d;\n    int u7;\n"+tail, ('a','hobj','c','d'), 'b', 'c', 'b'),
 'V4': (head+"    unsigned int a;\n    unsigned int b;\n    unsigned int hobj;\n    unsigned int fourth;\n    unsigned int sum;\n    int u7;\n"+tail, ('a','b','hobj','fourth'), 'sum', 'a', 'b'),
 'V5': (head+"    unsigned int sum;\n    unsigned int i0;\n    unsigned int i1;\n    unsigned int i2;\n    unsigned int i3;\n    unsigned int a;\n    unsigned int b;\n"+tail, ('i0','i1','i2','i3'), 'sum', 'a', 'b'),
 'V6': (head+"    unsigned int a;\n    unsigned int b;\n    unsigned int hobj;\n    unsigned int fourth;\n    unsigned int sum;\n    int u7;\n"+tail, ('hobj','fourth','a','b'), 'sum', 'a', 'b'),
}
for k,(d,i,sv,ba,bb) in V.items():
    open(f'gvar/{k}.c','w').write(body(d,i,sv,ba,bb))
