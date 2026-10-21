import itertools
W='cloud/work/frontier/w36x/9842C'
src=open(W+'/g0/group.c').read()
i=src.index('void func_8039842C(void)\n{')
head, root = src[:i], src[i:]
sw_start = root.index('        switch (D_803BACE0[playerIdx]) {')
sw_end = root.index('        func_80394DD8(q);\n    }\n') + len('        func_80394DD8(q);\n    }\n')
pre = root[:sw_start]; post = root[sw_end:]
inl = {
 'controlWait': """            if (!D_8014A118[playerIdx].owner ||
                (D_8014A118[playerIdx].owner->body->selection &&
                 func_800A361C(D_8014A118[playerIdx].owner->body->selection->item->player)) ||
                (D_8014A118[playerIdx].flags & 4)) {
                if (D_8014A118[playerIdx].flags & 4) resource_type_select(D_8014A118[playerIdx].flags);
                func_80394564(-1);
                func_80394B00(playerIdx, 0);
            }
""",
 'controlControls': """            if (!D_8015418C[playerIdx]) {
                visual_objects_update(1);
                func_800A3424((void **)D_8014A118[playerIdx].owner->body->selection, 0);
                object_render_cleanup(D_8014A118[playerIdx].owner);
                func_80394564(-1);
                func_80394B00(playerIdx, 0);
                func_80394DD8(playerIdx);
            }
""",
 'controlPakFull': """            if (D_8014A118[playerIdx].flags & 7) {
                resource_type_select(D_8014A118[playerIdx].flags);
                func_80394B00(playerIdx, 0);
            }
""",
}
inl['controlPakDamaged']=inl['controlPakFull']
hdef = {k: "void %s(s32 p)\n{\n%s}\n" % (k, v.replace('playerIdx','p').replace('            ','    ',).replace('\n    ','\n')) for k,v in inl.items()}
# fix indentation crudely (cosmetic only)
order=['controlWait','controlControls','controlPakFull','controlPakDamaged']
cases={1:'controlWait',2:'controlControls',5:'controlPakFull',6:'controlPakDamaged'}
n=0; man=[]
for mask, qmode, shp in itertools.product(range(16),[0,1],[0,1]):
    H=[order[k] for k in range(4) if mask>>k&1]
    r=pre.replace('    s32 u0,u1,u2,u3,u4,u5,u6;\n','')
    if qmode: r=r.replace('    s8 q;\n','')
    shl='    do {} while (0); /* SHAPING (context): compiled-out block boundary, see header */\n'
    if not shp: r=r.replace(shl,'')
    Q='' if qmode else '            q = playerIdx;\n'
    s='        switch (D_803BACE0[playerIdx]) {\n'
    for c in range(7):
        s+=f'        case {c}:\n'
        if c==0: s+='            func_80396FA0(playerIdx);\n'
        elif c==3: s+='            func_803968C0(playerIdx);\n'
        elif c==4: s+='            func_80395E8C(playerIdx);\n'
        else:
            nm=cases[c]
            s+= f'            {nm}(playerIdx);\n' if nm in H else inl[nm]
        s+=Q+'            break;\n'
    s+='        default:\n'+Q+'            break;\n        }\n'
    s+='        func_80394DD8(%s);\n    }\n' % ('playerIdx' if qmode else 'q')
    protos=''.join(f'void {h}(s32 p);\n' for h in H)
    out=head+protos+r+s+post+''.join(hdef[h] for h in H)
    fn=f'v{n:04d}.c'; open(f'{W}/b2/{fn}','w').write(out); man.append(f'{fn}\thelpers={",".join(H)} q={qmode} shp={shp}'); n+=1
open(f'{W}/b2/manifest.tsv','w').write('\n'.join(man)+'\n'); print(n)
