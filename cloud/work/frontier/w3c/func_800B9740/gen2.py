import itertools
HEAD=open('p0.c').read().split('void func_800B9740')[0].replace('-O2','-O3').replace('extern Vertex *D_801409E8;','extern VTYPE D_801409E8;')
def body(vt, inner, axis, cache, flag):
    if vt=='vp': VT='Vertex *'; VX='D_801409E8 + i'; AX='(*vertex)[axis]'; VD='Vertex *vertex;'; CMPV='vertex'
    elif vt=='sp': VT='s16 *'; VX='&D_801409E8[i * 3]'; AX='vertex[axis]'; VD='s16 *vertex;'; CMPV='(Vertex *)vertex'
    lim_t = 'total' if cache else 'D_801527A4'
    lim_p = 'primary' if cache else 'D_801407F0.primary_count'
    decl = 'int total, primary;' if cache else ''
    pre = 'total = D_801527A4;\n    primary = D_801407F0.primary_count;' if cache else ''
    if inner=='ptr':
        IN='''range = D_801407F0.ranges;
            for (j = 0; j < D_801407F0.range_count; j++, range++) {
                if (%s >= range->points &&
                    %s < range->points + range->count && range->kind == 1)
                    eligible = 1;
            }'''%(CMPV,CMPV)
    else:
        IN='''for (j = 0; j < D_801407F0.range_count; j++) {
                if (%s >= D_801407F0.ranges[j].points &&
                    %s < D_801407F0.ranges[j].points + D_801407F0.ranges[j].count && D_801407F0.ranges[j].kind == 1)
                    eligible = 1;
            }'''%(CMPV,CMPV)
    if axis=='idx':
        AXL='''for (axis = 0; axis < 3; axis++) {
                D_801407D4[axis] = D_801407D4[axis] < %s ? D_801407D4[axis] : %s;
                D_801407B4[axis] = %s < D_801407B4[axis] ? D_801407B4[axis] : %s;
            }'''%(AX,AX,AX,AX)
    else:
        AXL='''for (axis = 0; axis < 3; axis++) {
                if (D_801407D4[axis] < %s) D_801407D4[axis] = D_801407D4[axis]; else D_801407D4[axis] = %s;
                if (%s < D_801407B4[axis]) D_801407B4[axis] = D_801407B4[axis]; else D_801407B4[axis] = %s;
            }'''%(AX,AX,AX,AX)
    cond = 'eligible == 1' if flag else 'eligible'
    return HEAD.replace('VTYPE', VT.strip()+(' ' if VT.strip()=='Vertex *' else ' ') ) + '''void func_800B9740(void)
{
    int i, j, axis, eligible;
    VertexRange *range;
    %s
    %s
    D_801407D4[0] = D_801407D4[1] = D_801407D4[2] = 32767;
    D_801407B4[0] = D_801407B4[1] = D_801407B4[2] = -32767;
    %s
    for (i = 0; i < %s; i++) {
        if (i < %s) {
            eligible = 1;
        } else {
            eligible = 0;
            vertex = %s;
            %s
        }
        if (%s) {
            vertex = %s;
            %s
        }
    }
}
''' % (VD, decl, pre, lim_t, lim_p, VX, IN, cond, VX, AXL)
n=0
for combo in itertools.product(['vp','sp'],['ptr','idx'],['idx','if'],[0,1],[0,1]):
    s=body(*combo)
    s=s.replace('extern Vertex * D_801409E8;','extern Vertex *D_801409E8;').replace('extern s16 * D_801409E8;','extern s16 *D_801409E8;')
    open('b2/%s.c'%'_'.join(map(str,combo)),'w').write(s)
