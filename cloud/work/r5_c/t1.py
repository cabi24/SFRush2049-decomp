from var import *
V={}
V['base']=(D0,SEL,PL0,LB0)
V['nok']=(D0,SEL,'        bd = player_array[best].dist;',LB0.replace('k1','D_80124310').replace('k2','D_80124314'))
V['one']=(D0+' f32 one;',SEL,PL0+' one = 1.0f;',LB0.replace('1.0f','one'))
V['one_noK']=(D0+' f32 one;',SEL,'        bd = player_array[best].dist; one = 1.0f;',LB0.replace('1.0f','one').replace('k1','D_80124310').replace('k2','D_80124314'))
V['half']=(D0+' f32 h;',SEL,PL0+' h=0.5f;',LB0.replace('0.5f','h'))
V['vol']=(D0.replace('f32 k1; f32 k2;','volatile f32 k1; volatile f32 k2;'),SEL,PL0,LB0)
V['dbl']=(D0,SEL,PL0,LB0.replace('0.5f','0.5').replace('1.0f','1.0'))
for k,(d,s,p,l) in V.items():
    print(k, rb.score_src(mk(d,'',s,p,l)))
