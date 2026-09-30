from var import *
V={}
V['base']=(D0,SEL,PL0,LB0)
V['best_s32']=(D0.replace('s16 best','s32 best'),SEL,PL0,LB0)
V['best_int_u']=(D0.replace('s16 best','s16 best; s16 m1'),SEL.replace('best = -1;','m1=-1; best = m1;').replace('best == -1','best == m1'),PL0,LB0)
V['best_lt0']=(D0,SEL.replace('best == -1','best < 0'),PL0,LB0)
V['best_ne']=(D0,SEL.replace('best == -1','best == (s16)-1'),PL0,LB0)
V['best_bool']=(D0,SEL.replace('if (best == -1) {\n                    best = i;\n                } else if (','if (best == -1) {\n                    best = i;\n                } else if ('),PL0,LB0)
for k,(d,s,p,l) in V.items():
    print(k, rb.score_src(mk(d,'',s,p,l)))
