import json, os, sys
W='cloud/work/frontier/w38b/9DCC'
base=open(f'{W}/best0/group.c').read(); spec=json.load(open(f'{W}/best0/group.json'))
decl='    s32 u0, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10, u11;\n'
assert decl in base
outs=[]
def emit(name,s):
    d=f'{W}/v/{name}'; os.makedirs(d,exist_ok=True)
    open(f'{d}/group.c','w').write(s); json.dump(spec,open(f'{d}/group.json','w'),indent=1); outs.append(d)
for n in (13,11,10,9,8):
    names=['u%d'%i for i in range(n)]
    emit(f'n{n}',base.replace(decl,'    s32 '+', '.join(names)+';\n'))
print(' '.join(outs))
