import sys
src=open('best_body.c').read()
S="D_80152818[car->id]"
cond="        if (D_80152818[car->id].b857 == 0 && D_80152818[car->id].b856 == 0 && car->s1732 == -1) {\n"
assert cond in src
V={}
V['a1']=src.replace(cond, cond+"            if (%s.b857) {}\n"%S)
V['a2']=src.replace(cond, cond+"            if (%s.b856) {}\n"%S)
V['a3']=src.replace(cond, "        if (%s.b857) {}\n"%S+cond)
V['a4']=src.replace(cond, "        if (%s.b856) {}\n"%S+cond)
V['a5']=src.replace("                effect_cleanup(car->id, car->id, -1);\n","                if (%s.b857) {}\n                effect_cleanup(car->id, car->id, -1);\n"%S)
V['a6']=src.replace("                eu_nop();\n","                eu_nop();\n                if (%s.b857) {}\n"%S)
V['a7']=src.replace(cond, "        if (D_80152818[car->id].b857 == 0) { if (%s.b856) {}\n        if (D_80152818[car->id].b856 == 0 && car->s1732 == -1) {\n"%S).replace("    return;\n    }\n    }\nmiss:","    }\n    return;\n    }\n    }\nmiss:")
V['a8']=src.replace(cond, cond.replace("== 0 &&","== 0 && (%s.b856 ? 0 : 0) == 0 &&"%S,1))
for k,v in V.items():
    assert v!=src,k
    open(k+'.c','w').write(v)
