import sys, itertools
import rb
pre=open('pre.txt').read(); post=open('post.txt').read()
def mk(decls, reset, sel, pre_loop, loop_body, loopvar='i'):
    return pre+f'''void func_800DE860(void)
{{
{decls}
    if ((active_player_count == 1 && D_80152030 < 5) || D_80150F14 == 0) {{
        for (j = 0; j < 6; j++) {{
            if (D_80153E88[j].flag == 0 || D_80153E88[j].flag == 6) {{
                D_8014A250[j].scale = 1.0f;
            }}
        }}
    }} else {{
{sel}
{pre_loop}
        for (i = 0; i < 6; i++) {{
            if (D_8014A250[i].active != 0 && player_array[i].state < 2 &&
                (D_8014A250[i].kind == 2 || D_80152030 == 5)) {{
{loop_body}
            }}
        }}
    }}
}}

'''+post
SEL='''        best = -1;
        for (i = 0; i < 6; i++) {
            if (D_8014A250[i].active != 0 && player_array[i].state < 2 &&
                (D_8014A250[i].kind == 2 || D_80152030 == 5)) {
                if (best == -1) {
                    best = i;
                } else if (player_array[best].dist < player_array[i].dist) {
                    best = i;
                }
            }
        }'''
D0='''    s16 i; s16 best; s32 j; f32 s; f32 d; f32 bd; f32 k1; f32 k2;'''
PL0='''        k1 = D_80124310; k2 = D_80124314; bd = player_array[best].dist;'''
LB0='''                d = bd - player_array[i].dist;
                if (d > k2) { s = k1 + 1.0f; } else { s = d * k1 / k2 + 1.0f; }
                if (D_80150F14 == 1) { s = (1.0f - s) * 0.5f + 1.0f; }
                D_8014A250[i].scale = D_8014A250[i].scale * D_80124318 + D_8012431C * s;'''
if __name__=='__main__':
    print(rb.score_src(mk(D0,'',SEL,PL0,LB0)))
