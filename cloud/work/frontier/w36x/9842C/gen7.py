import itertools, re
W='cloud/work/frontier/w36x/9842C'
exec(open(W+'/gen2.py').read().split("n=0; man=[]")[0])  # reuse head/pre/post/inl/hdef/order/cases
subsets={'none':[], 'all':order, 'WC':['controlWait','controlControls'], 'PP':['controlPakFull','controlPakDamaged'],
         'WPP':['controlWait','controlPakFull','controlPakDamaged'], 'C':['controlControls'], 'W':['controlWait']}
allr_old="""        for (playerIdx = 0; playerIdx < D_8014A108; playerIdx++)
            if (D_803BACE0[playerIdx] != 1) allReady = 0;"""
allr_new="""        s32 i;
        for (i = 0; i < D_8014A108; i++)
            if (D_803BACE0[i] != 1) allReady = 0;"""
pur_old="""            for (player = D_8012E6E0; player; player = player->body->next) {
                for (p = 0; p < D_8014A108; p++)
                    if (player == D_8014A118[p].owner) break;
                if (p >= D_8014A108) object_render_cleanup(player);
            }"""
pur_new="""            for (player = D_8012E6E0; player; player = player->body->next) {
                s32 i;
                for (i = 0; i < D_8014A108; i++)
                    if (player == D_8014A118[i].owner) break;
                if (i >= D_8014A108) object_render_cleanup(player);
            }"""
assert allr_old in post and pur_old in post
n=0; man=[]
for sk, A, P, U in itertools.product(subsets, [0,1],[0,1],[1,2,3]):
    H=subsets[sk]
    r=pre.replace('    s32 u0,u1,u2,u3,u4,u5,u6;\n', ('    s32 '+','.join(f'u{k}' for k in range(U))+';\n') if U else '').replace('    s8 q;\n','')
    s='        switch (D_803BACE0[playerIdx]) {\n'
    for c in range(7):
        s+=f'        case {c}:\n'
        if c==0: s+='            func_80396FA0(playerIdx);\n'
        elif c==3: s+='            func_803968C0(playerIdx);\n'
        elif c==4: s+='            func_80395E8C(playerIdx);\n'
        else:
            nm=cases[c]; s+= f'            {nm}(playerIdx);\n' if nm in H else inl[nm]
        s+='            break;\n'
    s+='        default:\n            break;\n        }\n        func_80394DD8(playerIdx);\n    }\n'
    po=post
    if A: po=po.replace(allr_old,allr_new)
    if P: po=po.replace(pur_old,pur_new)
    protos=''.join(f'void {h}(s32 p);\n' for h in H)
    out=head+protos+r+s+po+''.join(hdef[h] for h in H)
    fn=f'v{n:04d}.c'; open(f'{W}/b7/{fn}','w').write(out); man.append(f'{fn}\th={sk} allr_i={A} purge_i={P} unused={U}'); n+=1
open(f'{W}/b7/manifest.tsv','w').write('\n'.join(man)+'\n'); print(n)
