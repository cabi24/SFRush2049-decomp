import random, itertools, sys, collections
import srch1, sc
pre=srch1.pre
ARMA=['s = k1 + 1.0f;','s = 1.0f + k1;','s = k1; s += 1.0f;','s = 1.0f; s += k1;','s = k1; s = s + 1.0f;']
ARMB=['s = d * k1 / k2 + 1.0f;','s = 1.0f + d * k1 / k2;','s = k1 * d / k2 + 1.0f;','s = d / k2 * k1 + 1.0f;','s = d * k1 / k2; s += 1.0f;','s = d * (k1 / k2) + 1.0f;','s = (d / k2) * k1 + 1.0f;']
CMP=['d > k2','k2 < d','!(d <= k2)','d >= k2']
ONE=['s = (1.0f - s) * 0.5f + 1.0f;','s = 1.0f + (1.0f - s) * 0.5f;','s = (1.0f - s) / 2 + 1.0f;','s = 1.0f - s; s = s * 0.5f + 1.0f;','s = (1.0f - s) * 0.5f; s += 1.0f;','s = 1.5f - s * 0.5f;','s = (1.0f + s) * 0.5f + 0.5f;']
UPD=['D_8014A250[i].scale = D_8014A250[i].scale * D_80124318 + D_8012431C * s;',
     'D_8014A250[i].scale = D_8014A250[i].scale * D_80124318 + s * D_8012431C;',
     'D_8014A250[i].scale = D_80124318 * D_8014A250[i].scale + D_8012431C * s;',
     'D_8014A250[i].scale = D_8012431C * s + D_8014A250[i].scale * D_80124318;']
MODE=['D_80150F14 == 1','1 == D_80150F14','D_80150F14 - 1 == 0']
DECL=['    s16 i;\n    s16 best;\n    s32 j;\n    f32 s;\n    f32 d;\n    f32 bd;\n    f32 k1;\n    f32 k2;\n']
def mk(a,b,c,o,u,m):
    return pre+f'''void func_800DE860(void)
{{
{DECL[0]}
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
        bd = player_array[best].dist;
        k2 = D_80124314;
        k1 = D_80124310;
        for (i = 0; i < 6; i++) {{
            if (D_8014A250[i].active != 0 && player_array[i].state < 2 &&
                (D_8014A250[i].kind == 2 || D_80152030 == 5)) {{
                d = bd - player_array[i].dist;
                if ({c}) {{
                    {a}
                }} else {{
                    {b}
                }}
                if ({m}) {{
                    {o}
                }}
                {u}
            }}
        }}
    }}
}}
'''
res=collections.Counter()
best=[]
for combo in itertools.product(ARMA,ARMB,CMP,ONE,UPD,MODE):
    pass
random.seed(1)
allc=list(itertools.product(ARMA,ARMB,CMP,ONE,UPD,MODE))
random.shuffle(allc)
for combo in allc[:int(sys.argv[1])]:
    src=mk(*combo); open('w/r.c','w').write(src)
    r=sc.sc('w/r.c','func_800DE860')
    res[r]+=1
    if r[0]>=174: best.append((r,combo))
for k,v in res.most_common(): print(k,v)
for b in best[:5]: print(b)
