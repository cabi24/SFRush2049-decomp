src=open('best_body.c').read()
S="D_80152818[car->id]"
cond="        if (D_80152818[car->id].b857 == 0 && D_80152818[car->id].b856 == 0 && car->s1732 == -1) {\n"
V={}
V['d_pos_in']=src.replace(cond,cond+"            if (%s.dr_pos[0]) {}\n"%S)
V['d_place_in']=src.replace(cond,cond+"            if (%s.place) {}\n"%S)
V['d_pos_pre']=src.replace(cond,"        if (%s.dr_pos[0]) {}\n"%S+cond)
for k,v in V.items(): open(k+'.c','w').write(v)
