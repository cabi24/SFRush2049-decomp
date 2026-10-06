import sys
base=open(sys.argv[1]).read()
V={
'B':[('D_80152818[car->id].b857 == 0 && D_80152818[car->id].b856 == 0','!D_80152818[car->id].b857 && !D_80152818[car->id].b856')],
'F':[('    if ((*surf == 3 || *surf == 4 || *surf == 7) && out[1] < 0.25f) {\n','    if (*surf == 3 || *surf == 4 || *surf == 7) {\n    if (out[1] < 0.25f) {\n'),('    return;\n    }\n    }\nmiss:','    }\n    return;\n    }\n    }\nmiss:')],
'G':[('        car->attr[wheel] &= 0x7FF;','        car->attr[wheel] = best->cnt & 0x7FF;')],
'D':[('                func_800B61A8(21, 0, 1, 2);\n                eu_nop();\n','                func_800B61A8(21, 0, 1, 2);\n'),('            }\n        }\n    }\n    return;','            }\n        }\n        eu_nop();\n    }\n    return;')],
'I':[('(*surf == 3 || *surf == 4 || *surf == 7)','(*surf == 3 || *surf == 4 || *surf == 7) != 0')],
'J':[('    if (best->cnt & 0x30) {\n        car->attr[wheel] &= 0x7FF;\n    }\n','    if (best->cnt & 0x30) car->attr[wheel] &= 0x7FF;\n')],
'K':[('s32 *surf','u32 *surf')],
'L':[('static void eu_nop(void) {}','static void eu_nop(s32 *s) {}'),('eu_nop();','eu_nop(surf);')],
}
for k,reps in V.items():
    s=base
    for a,b in reps:
        assert a in s,(k,a)
        s=s.replace(a,b)
    open('v/%s.c'%k,'w').write(s)
