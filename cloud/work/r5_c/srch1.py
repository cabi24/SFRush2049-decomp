import itertools, sys, tempfile, os
from pathlib import Path
import sc
pre=open('s0.c').read(); pre=pre[:pre.index('void func_800DE860(void)\n{')]
def body(decl_order, usek, usebd, used, uses, one):
    fl={'s':'f32 s;','d':'f32 d;','bd':'f32 bd;','k1':'f32 k1;','k2':'f32 k2;'}
    order=[n for n in decl_order if (n=='s' and uses) or (n=='d' and used) or (n=='bd' and usebd) or (n in('k1','k2') and usek)]
    decls='    s16 i;\n    s16 best;\n    s32 j;\n'+''.join('    '+fl[n]+'\n' for n in order)
    K1='k1' if usek else 'D_80124310'; K2='k2' if usek else 'D_80124314'
    BD='bd' if usebd else 'player_array[best].dist'
    O='1.0f'
    D='d' if used else f'({BD} - player_array[i].dist)'
    S='s' if uses else None
    pl=''
    if usek: pl+=f'        k1 = D_80124310;\n        k2 = D_80124314;\n'
    if usebd: pl+='        bd = player_array[best].dist;\n'
    if used: dl=f'                d = {BD} - player_array[i].dist;\n'
    else: dl=''
    if uses:
        lb=dl+f'''                if ({D} > {K2}) {{
                    s = {K1} + {O};
                }} else {{
                    s = {D} * {K1} / {K2} + {O};
                }}
                if (D_80150F14 == 1) {{
                    s = ({O} - s) * 0.5f + {O};
                }}
                D_8014A250[i].scale = D_8014A250[i].scale * D_80124318 + D_8012431C * s;
'''
    else:
        lb=dl+f'''                if ({D} > {K2}) {{
                    D_8014A250[i].scale = D_8014A250[i].scale * D_80124318 + D_8012431C * ({K1} + {O});
                }} else {{
                    D_8014A250[i].scale = D_8014A250[i].scale * D_80124318 + D_8012431C * ({D} * {K1} / {K2} + {O});
                }}
'''
    return f'''void func_800DE860(void)
{{
{decls}
    if ((active_player_count == 1 && D_80152030 < 5) || D_80150F14 == 0) {{
        for (j = 0; j < 6; j++) {{
            if (D_80153E88[j].flag == 0 || D_80153E88[j].flag == 6) {{
                D_8014A250[j].scale = 1.0f;
            }}
        }}
    }} else {{
        best = -1;
        for (i = 0; i < 6; i++) {{
            if (D_8014A250[i].active != 0 && player_array[i].state < 2 &&
                (D_8014A250[i].kind == 2 || D_80152030 == 5)) {{
                if (best == -1) {{
                    best = i;
                }} else if (player_array[best].dist < player_array[i].dist) {{
                    best = i;
                }}
            }}
        }}
{pl}        for (i = 0; i < 6; i++) {{
            if (D_8014A250[i].active != 0 && player_array[i].state < 2 &&
                (D_8014A250[i].kind == 2 || D_80152030 == 5)) {{
{lb}            }}
        }}
    }}
}}
'''
if __name__=='__main__':
    res=[]
    os.makedirs('w',exist_ok=True)
    for usek in (1,0):
      for usebd in (1,0):
        for used in (1,0):
          for uses in (1,):
            names=['s','d','bd','k1','k2']
            seen=set()
            for perm in itertools.permutations(names):
                key=tuple(n for n in perm if (n=='s' and uses) or (n=='d' and used) or (n=='bd' and usebd) or (n in('k1','k2') and usek))
                if key in seen: continue
                seen.add(key)
                src=pre+body(perm,usek,usebd,used,uses,1)
                open('w/t.c','w').write(src)
                r=sc.sc('w/t.c','func_800DE860')
                res.append((r,(usek,usebd,used,uses,key)))
                if r[2]>0 and r[1]==211 and r[0]==211: print('MATCH',key,usek,usebd,used); open('w/match_%d.c'%len(res),'w').write(src)
    res.sort(key=lambda x:(-x[0][0]))
    import collections
    c=collections.Counter((r[0],r[1][:4]) for r in res)
    for k,v in sorted(c.items(),key=lambda x:-x[0][0][0]): print(k,v)
