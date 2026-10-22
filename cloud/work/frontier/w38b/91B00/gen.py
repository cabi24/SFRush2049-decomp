import json, os, sys, itertools
W='cloud/work/frontier/w38b/91B00'
outs=[]
for src in ('best0','holder0'):
    base=open(f'{W}/{src}/group.c').read(); spec=json.load(open(f'{W}/{src}/group.json'))
    def emit(name,s):
        d=f'{W}/v/{name}'; os.makedirs(d,exist_ok=True)
        open(f'{d}/group.c','w').write(s); json.dump(spec,open(f'{d}/group.json','w'),indent=1); outs.append(d)
    ns=base.replace('static int animStuntCoins','int func_80391640').replace('static int animBattleCoins','int func_80391648').replace('animStuntCoins(','func_80391640(').replace('animBattleCoins(','func_80391648(')
    emit(f'{src}_ns',ns)
    # void return
    v=ns.replace('int func_80391640(void *unused)','void func_80391640(void *unused)').replace('int func_80391648(void *unused)','void func_80391648(void *unused)')
    emit(f'{src}_nsvoid',v)
print(' '.join(outs))
