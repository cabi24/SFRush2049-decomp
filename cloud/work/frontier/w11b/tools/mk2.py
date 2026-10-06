import sys
base=open(sys.argv[1]).read()
C='D_80152818[car->id].b857 == 0 && D_80152818[car->id].b856 == 0'
V={
'M1':[(C,'(&D_80152818[car->id])->b857 == 0 && (&D_80152818[car->id])->b856 == 0')],
'M2':[(C,'D_80152818[(s32)car->id].b857 == 0 && D_80152818[(s32)car->id].b856 == 0')],
'M3':[(C,'(D_80152818 + car->id)->b857 == 0 && (D_80152818 + car->id)->b856 == 0')],
'M4':[('(*surf == 3 || *surf == 4 || *surf == 7)','(surf[0] == 3 || surf[0] == 4 || surf[0] == 7)')],
'M5':[('            if (*surf == 4) {','            if (4 == *surf) {')],
'M6':[('            } else if (*surf == 7) {','            } else if (7 == *surf) {')],
'M7':[('    if ((*surf == 3 || *surf == 4 || *surf == 7) && out[1] < 0.25f) {','    if ((*surf == 3 || *surf == 4 || *surf == 7) && out[1] < 0.25f)\n    {')],
}
for k,reps in V.items():
    s=base
    for a,b in reps:
        assert a in s,(k,a)
        s=s.replace(a,b)
    open('v/%s.c'%k,'w').write(s)
