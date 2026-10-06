src=open('best_body.c').read()
S="D_80152818[car->id]"
cond="        if (D_80152818[car->id].b857 == 0 && D_80152818[car->id].b856 == 0 && car->s1732 == -1) {\n"
V={}
reads={'p0':S+'.pad0[0]','p8':S+'.pad858[0]','ad':'&'+S,'ad2':'(u32)&'+S}
pos={'in':(cond,cond+"            if (%s) {}\n"),
     'pre':(cond,"        if (%s) {}\n"+cond),
     'arm4':("                effect_cleanup(car->id, car->id, -1);\n","                if (%s) {}\n                effect_cleanup(car->id, car->id, -1);\n"),
     'arm7':("                car->b1741 = 1;\n                func_800C54F0(car->id, 1);\n            } else {","                if (%s) {}\n                car->b1741 = 1;\n                func_800C54F0(car->id, 1);\n            } else {"),
}
for rk,r in reads.items():
    for pk,(a,b) in pos.items():
        assert a in src
        V['c_%s_%s'%(rk,pk)]=src.replace(a,b%r,1)
for k,v in V.items(): open(k+'.c','w').write(v)
