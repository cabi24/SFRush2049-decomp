#!/usr/bin/env python3
"""merge.py THEIRS MINE OUT : replace w36e-owned bodies in THEIRS with MINE's versions."""
import sys, re
theirs = open(sys.argv[1]).read(); mine = open(sys.argv[2]).read()
def body(s, name):
    m = re.search(r'\n(void|s32) ' + name + r'\((void|s32 \w+)\)\n\{', s)
    a = m.start() + 1
    b = s.index('\n}\n', a) + 3
    return a, b
out = theirs
for name in ['func_8038AEB0', 'func_8038B3EC', 'func_8038C5EC', 'func_8038A820', 'func_8038AEA8']:
    ma, mb = body(mine, name)
    ta, tb = body(out, name)
    new = mine[ma:mb]
    if name == 'func_8038A820':
        new = new.replace('D_803BAAE8[mapIdx].numPack', 'D_803BAAE8[mapIdx].displayList')
        new = new.replace('pStrip = D_803BAAE8[mapIdx].verts;', 'pStrip = (MapVtx *)D_803BAAE8[mapIdx].verts;')
        new = ('typedef union MapColor { u8 c[4]; u32 w; } MapColor;\n'
               'typedef struct MapVtx { s16 ob[3]; u16 flag; s16 tc[2]; u8 cn[4]; } MapVtx;\n' + new)
    if name == 'func_8038C5EC':
        new = 'typedef struct UVMatrix { f32 uvs[3][3]; f32 pos[3]; } UVMatrix;   /* tUVMatrix */\n' + new
    out = out[:ta] + '/* w36e final body (cloud/work/frontier/w36e/groups/best_ts) */\n' + new + out[tb:]
open(sys.argv[3], 'w').write(out)
