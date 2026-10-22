import json, os, sys, itertools, shutil
W='cloud/work/frontier/w38b'
B=f'{W}/base/ovl_b/w36h_shots'
base=open(f'{B}/shots.c').read(); spec=json.load(open(f'{B}/group.json'))
a=base.index('void func_8038FCE0(void)\n{')
loop='''    for (casing = D_8039AE80.head; casing; casing = next) {
        next = casing->next;
        casing->time -= D_8002EB94;
        if (casing->time <= 0) {
            idx = casing->mbIdx;
            sound_call_minimal(idx);
            func_800AFA84(&D_8039AE80, casing);
        } else {
            casing->vel[1] -= (D_80142764 * D_8002EB94) * 2.5f;
            casing->mat.pos[0] = casing->mat.pos[0] + casing->vel[0] * D_8002EB94;
            casing->mat.pos[1] = casing->mat.pos[1] + casing->vel[1] * D_8002EB94;
            casing->mat.pos[2] = casing->mat.pos[2] + casing->vel[2] * D_8002EB94;
        }
    }
'''
assert base.index(loop,a)>a
decl='''    ShotCase *casing, *next;
    s32 idx;
'''
def make(keepidx, pos, callerdecl):
    s=base
    s=s.replace(loop,'    func_8038FCD8();\n',1)
    if not callerdecl:
        s=s.replace(decl,'',1)
    hl=loop if keepidx else loop.replace('            idx = casing->mbIdx;\n            sound_call_minimal(idx);\n','            sound_call_minimal(casing->mbIdx);\n')
    h='/* AnimateCase (MAT3 shots.c): inlined into ShotUpdate func_8038FCE0; deleted stub 0x8038FCD8 (w38b). Loop moved verbatim. */\nvoid func_8038FCD8(void)\n{\n    ShotCase *casing, *next;\n'+('    s32 idx;\n' if keepidx else '')+'\n'+hl+'}\n\n'
    if pos=='before':
        k=s.index('void func_8038FCE0(void)\n{')
        k=s.rfind('\n\n',0,k)+2
    else:
        # first function definition in the file (MAT3 order: AnimateCase first)
        k=s.index('void func_8038F568(')
        k=s.rfind('\n\n',0,k)+2
    s=s[:k]+'void func_8038FCD8(void);\n'+h+s[k:]
    return s
outs=[]
for ki,pos,cd in itertools.product((1,0),('before','top'),(0,1)):
    d=f'{W}/v/sh/k{ki}{pos}c{cd}'; os.makedirs(d,exist_ok=True)
    for f in spec['files']:
        if f!='shots.c': shutil.copy(f'{B}/{f}',d)
    open(f'{d}/shots.c','w').write(make(ki,pos,cd))
    sp=dict(spec); sp['members']=spec['members']+['func_8038FCD8']
    json.dump(sp,open(f'{d}/group.json','w'),indent=1); outs.append(d)
print(' '.join(outs)) if len(sys.argv)<2 else None
def blk(s, offb, hpb, szb):
    a=s.index('void func_8038FCE0(void)\n{'); b=s.index('\n}\n',a)+3
    f=s[a:b]
    if offb:
        f=f.replace('    f32 off[3];\n','',1)
        f=f.replace('                h = func_8008E3C0(&D_8039AE80);\n                if (h) {\n','                h = func_8008E3C0(&D_8039AE80);\n                if (h) {\n                    f32 off[3];\n',1)
        f=f.replace('            {\n                if (car->battle.type >= 8) {\n','            {\n                f32 off[3];\n                if (car->battle.type >= 8) {\n',1)
    if hpb:
        f=f.replace('    HitPoly *hp, *nexth;\n','',1)
        old='    for (hp = D_8039A5F8.head; hp; hp = nexth) {'
        i=f.index(old); j=f.index('\n    }\n',i)+len('\n    }\n')
        f=f[:i]+'    {\n    HitPoly *hp, *nexth;\n'+f[i:j]+'    }\n'+f[j:]
    if szb:
        f=f.replace('    ColSize *sizep;\n','',1)
        f=f.replace('                f32 z;\n','                f32 z;\n                ColSize *sizep;\n',1)
    return s[:a]+f+s[b:]
if len(sys.argv)>1 and sys.argv[1]=='2':
    outs=[]
    for ob,hb,sb in itertools.product((0,1),(0,1),(0,1)):
        if not (ob or hb or sb): continue
        d=f'{W}/v/sh/b{ob}{hb}{sb}'; os.makedirs(d,exist_ok=True)
        for f in spec['files']:
            if f!='shots.c': shutil.copy(f'{B}/{f}',d)
        open(f'{d}/shots.c','w').write(blk(make(1,'before',0),ob,hb,sb))
        sp=dict(spec); sp['members']=spec['members']+['func_8038FCD8']
        json.dump(sp,open(f'{d}/group.json','w'),indent=1); outs.append(d)
    print(' '.join(outs))
def make3(form):
    s=make(1,'before',0)
    a=s.index('void func_8038FCD8(void)\n{'); b=s.index('\n}\n',a)+3
    h=s[a:b]
    if form=='blockidx':
        h=h.replace('    s32 idx;\n','').replace('            idx = casing->mbIdx;\n            sound_call_minimal(idx);\n','            s32 idx = casing->mbIdx;\n            sound_call_minimal(idx);\n')
    elif form=='s16idx':
        h=h.replace('    s32 idx;\n','    s16 idx;\n')
    elif form=='castdirect':
        h=h.replace('    s32 idx;\n','').replace('            idx = casing->mbIdx;\n            sound_call_minimal(idx);\n','            sound_call_minimal((s32)casing->mbIdx);\n')
    elif form=='nextreuse':
        h=h.replace('    s32 idx;\n','').replace('            idx = casing->mbIdx;\n            sound_call_minimal(idx);\n','            sound_call_minimal(casing->mbIdx);\n')
    elif form=='u8idx':
        h=h.replace('    s32 idx;\n','    u16 idx;\n')
    return s[:a]+h+s[b:]
if len(sys.argv)>1 and sys.argv[1]=='3':
    outs=[]
    for form in ('blockidx','s16idx','castdirect','u8idx'):
        d=f'{W}/v/sh/m3_{form}'; os.makedirs(d,exist_ok=True)
        for f in spec['files']:
            if f!='shots.c': shutil.copy(f'{B}/{f}',d)
        open(f'{d}/shots.c','w').write(make3(form))
        sp=dict(spec); sp['members']=spec['members']+['func_8038FCD8']
        json.dump(sp,open(f'{d}/group.json','w'),indent=1); outs.append(d)
    print(' '.join(outs))
def make4(form):
    s=make(0,'before',0)
    a=s.index('void func_8038FCD8(void)\n{'); b=s.index('\n}\n',a)+3
    h=s[a:b]
    old='            sound_call_minimal(casing->mbIdx);\n'
    assert old in h
    new={'vol':'            sound_call_minimal(*(volatile u32 *)&casing->mbIdx);\n',
         'plus0':'            sound_call_minimal(casing->mbIdx + 0);\n',
         'mul1':'            sound_call_minimal(casing->mbIdx * 1);\n',
         's32cast':'            sound_call_minimal((s32)casing->mbIdx);\n',
         'or0':'            sound_call_minimal(casing->mbIdx | 0);\n',
         'and':'            sound_call_minimal(casing->mbIdx & 0xFFFFFFFF);\n'}[form]
    h=h.replace(old,new)
    return s[:a]+h+s[b:]
if len(sys.argv)>1 and sys.argv[1]=='4':
    outs=[]
    for form in ('vol','plus0','mul1','s32cast','or0','and'):
        d=f'{W}/v/sh/m4_{form}'; os.makedirs(d,exist_ok=True)
        for f in spec['files']:
            if f!='shots.c': shutil.copy(f'{B}/{f}',d)
        open(f'{d}/shots.c','w').write(make4(form))
        sp=dict(spec); sp['members']=spec['members']+['func_8038FCD8']
        json.dump(sp,open(f'{d}/group.json','w'),indent=1); outs.append(d)
    print(' '.join(outs))
