import sys
name,drop,hdef,hcall=sys.argv[1:5]
pos = sys.argv[5] if len(sys.argv)>5 else 'end'
s=open('b0.c').read()
for p in drop.split(','):
    if p: s=s.replace('    s32 %s;\n'%p,'',1)
s=s.replace('void func_8008E408(s16 idx, u32 type) {', hdef+'\nvoid func_8008E408(s16 idx, u32 type) {',1)
anchors={'end':'    o->w52 = func_8008E26C(D_8014295A[o->h86], o->m, -1, 0x40000);\n',
 'start':'    car = &D_80152818[idx];\n'}
a=anchors[pos]
if pos=='end': s=s.replace(a+'}', a+hcall+'\n}',1)
else: s=s.replace(a, hcall+'\n'+a,1)
open(name+'.c','w').write(s)
