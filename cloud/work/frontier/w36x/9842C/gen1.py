import itertools, os, sys
W='cloud/work/frontier/w36x/9842C'
src=open(W+'/g0/group.c').read()
i=src.index('void func_8039842C(void)\n{')
head, root = src[:i], src[i:]
# isolate pieces of the root
sw_start = root.index('        switch (D_803BACE0[playerIdx]) {')
sw_end = root.index('        func_80394DD8(q);\n    }\n') + len('        func_80394DD8(q);\n    }\n')
pre = root[:sw_start]; post = root[sw_end:]
inline_sw = root[sw_start:sw_end]
helper_sw = """        switch (D_803BACE0[playerIdx]) {
        case 0: func_80396FA0(playerIdx);QQ break;
        case 1: controlWait(playerIdx);QQ break;
        case 2: controlControls(playerIdx);QQ break;
        case 3: func_803968C0(playerIdx);QQ break;
        case 4: func_80395E8C(playerIdx);QQ break;
        case 5: controlPakFull(playerIdx);QQ break;
        case 6: controlPakDamaged(playerIdx);QQ break;
        DEFAULT
        }
        func_80394DD8(QV);
    }
"""
helpers = """void controlWait(s32 p)
{
    if (!D_8014A118[p].owner ||
        (D_8014A118[p].owner->body->selection && func_800A361C(D_8014A118[p].owner->body->selection->item->player)) ||
        (D_8014A118[p].flags & 4)) {
        if (D_8014A118[p].flags & 4) resource_type_select(D_8014A118[p].flags);
        func_80394564(-1);
        func_80394B00(p, 0);
    }
}
void controlControls(s32 p)
{
    if (!D_8015418C[p]) {
        visual_objects_update(1);
        func_800A3424((void **)D_8014A118[p].owner->body->selection, 0);
        object_render_cleanup(D_8014A118[p].owner);
        func_80394564(-1);
        func_80394B00(p, 0);
        func_80394DD8(p);
    }
}
void controlPakFull(s32 p)
{
    if (D_8014A118[p].flags & 7) {
        resource_type_select(D_8014A118[p].flags);
        func_80394B00(p, 0);
    }
}
void controlPakDamaged(s32 p)
{
    if (D_8014A118[p].flags & 7) {
        resource_type_select(D_8014A118[p].flags);
        func_80394B00(p, 0);
    }
}
"""
updp = "void updatePlayer(s32 p)\n{\n}\n"
protos = "void controlWait(s32 p);\nvoid controlControls(s32 p);\nvoid controlPakFull(s32 p);\nvoid controlPakDamaged(s32 p);\nvoid updatePlayer(s32 p);\n"
n=0; man=[]
for form, qmode, upd, shp, nunused, where in itertools.product([0,1],[0,1],[0,1],[0,1,2],[0,6,7],[0,1]):
    if form==0 and where==1: continue
    r = pre
    if nunused==0: r = r.replace('    s32 u0,u1,u2,u3,u4,u5,u6;\n','')
    elif nunused==6: r = r.replace('    s32 u0,u1,u2,u3,u4,u5,u6;\n','    s32 u0,u1,u2,u3,u4,u5;\n')
    if qmode==1: r = r.replace('    s8 q;\n','')
    shl = '    do {} while (0); /* SHAPING (context): compiled-out block boundary, see header */\n'
    assert shl in r
    r = r.replace(shl, {0:shl,1:'',2:'    if (D_8014A108) {}\n'}[shp])
    if upd: r = r.replace('    for (playerIdx = 0; playerIdx < D_8014A108; playerIdx++) {\n        switch','    for (playerIdx = 0; playerIdx < D_8014A108; playerIdx++) {\n        updatePlayer(playerIdx);\n        switch',1)
    if form==0:
        s = inline_sw
        if qmode==1:
            s = s.replace('            q = playerIdx;\n','').replace('        default:\n            break;\n','        default:\n            break;\n').replace('func_80394DD8(q);','func_80394DD8(playerIdx);')
    else:
        s = helper_sw.replace('QQ', '' if qmode else ' q = playerIdx;').replace('QV','playerIdx' if qmode else 'q')
        s = s.replace('DEFAULT', 'default: break;' if qmode else 'default: q = playerIdx; break;')
    body = r + s + post
    hdefs = (helpers if form==1 else '') + (updp if upd else '')
    if where==0: out = head + protos + hdefs + body
    else: out = head + protos + body + hdefs
    if form==0 and not upd: out = head + body
    elif form==0: out = head + protos + updp + body if where==0 else head+protos+body+updp
    fn=f'v{n:04d}.c'; open(f'{W}/b1/{fn}','w').write(out)
    man.append(f'{fn}\tform={form} q={qmode} upd={upd} shp={shp} unused={nunused} where={where}')
    n+=1
open(f'{W}/b1/manifest.tsv','w').write('\n'.join(man)+'\n')
print(n)
