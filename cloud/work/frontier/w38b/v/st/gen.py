import json, os, sys, itertools
W='cloud/work/frontier/w38b'
B=f'{W}/base/ovl_b/dot_runtime_b_trail_20261012'
base=open(f'{B}/group.c').read(); spec=json.load(open(f'{B}/group.json'))
a=base.index('void func_8038CB20(void)\n{'); b=base.index('\n}\n',a)+3
fn=base[a:b]
loop1_s=fn.index('    for (fade = D_80396B80.active; fade; fade = next_fade) {')
loop2_s=fn.index('    for (trail = D_80395EE8.active; trail; trail = next_trail) {')
loop1=fn[loop1_s:loop2_s]
loop2=fn[loop2_s:fn.rindex('}')]
nf_s=loop2.index('            new_fade = func_8008E3C0(&D_80396B80);')
nf_e=loop2.index('        }\n    }\n', nf_s)
nfblk=loop2[nf_s:nf_e]
def dedent(t,n):
    return '\n'.join(l[n:] if l.startswith(' '*n) else l for l in t.split('\n'))
def make(holder, unused, order, ret):
    l1=loop1 if holder else loop1.replace('        if (D_80396B80.active) {}\n','')
    anim='/* _AnimateStreakPolys (MAT3 streaks.c): inlined into UpdateStreaks; deleted stub 0x8038CB18 (w38b) */\nvoid func_8038CB18(void)\n{\n    Fade *fade, *next_fade;\n'+l1+'}\n'
    body=dedent(nfblk,8)
    if ret:
        # MAT3: if (!sp) return; then body
        body=body.replace('    new_fade = func_8008E3C0(&D_80396B80);\n    if (new_fade) {\n','    new_fade = func_8008E3C0(&D_80396B80);\n    if (!new_fade) return;\n    {\n',1)
    newp='/* _NewStreakPoly (MAT3 streaks.c, tStreak *s): inlined into UpdateStreaks; deleted stub 0x8038CB10 (w38b) */\nvoid func_8038CB10(Trail *trail)\n{\n    Fade *new_fade;\n    f32 width;\n    f32 corners[4][3];\n'+body+'}\n'
    l2=loop2[:nf_s]+'            func_8038CB10(trail);\n'+loop2[nf_e:]
    decl='    Trail *trail, *next_trail;\n    f32 width;\n    f32 x;\n'+('    s32 unused_local_0, unused_local_1;\n' if unused else '')+'    f32 y, z;\n'
    caller='void func_8038CB20(void)\n{\n'+decl+'    func_8038CB18();\n'+l2+'}\n'
    protos='void func_8038CB10(Trail *);\nvoid func_8038CB18(void);\n'
    if order=='before':
        new=protos+anim+'\n'+newp+'\n'+caller
    else:
        new=protos+caller+'\n'+anim+'\n'+newp
    return base[:a]+new+base[b:]
outs=[]
def emit(name,s,extra):
    d=f'{W}/v/st/{name}'; os.makedirs(d,exist_ok=True)
    open(f'{d}/group.c','w').write(s)
    sp=dict(spec); sp['members']=spec['members']+extra
    json.dump(sp,open(f'{d}/group.json','w'),indent=1); outs.append(d)
for h,u,o,r in itertools.product((1,0),(0,1),('before','after'),(0,1)):
    emit(f'h{h}u{u}{o}r{r}',make(h,u,o,r),['func_8038CB10','func_8038CB18'])
print(' '.join(outs)) if len(sys.argv)<2 else None
def make2(holder, cont, xyz, unused, wloc):
    s=make(1 if holder=='orig' else 0, unused, 'before', 0)
    if holder=='fade':
        s=s.replace('        next_fade = fade->next;\n','        if (fade) {}\n        next_fade = fade->next;\n',1)
    if cont:
        old='''        if (fade->lifetime <= 0.0f) {
remove_fade:
            func_8008D0C0(fade->object);
            func_800AFA84(&D_80396B80, fade);
        } else {
'''
        assert old in s
        # MAT3: continue form
        i=s.index(old); j=s.index('\n        }\n    }\n}\n',i)
        inner=s[i+len(old):j]
        inner='\n'.join(l[4:] if l.startswith('    ') else l for l in inner.split('\n'))
        s=s[:i]+'''        if (fade->lifetime <= 0.0f) {
remove_fade:
            func_8008D0C0(fade->object);
            func_800AFA84(&D_80396B80, fade);
            continue;
        }
'''+inner+s[j+len('\n        }'):]
    if not xyz:
        old='''                x = trail->origin[0];
                y = trail->origin[1];
                z = trail->origin[2];
                trail->lower[0] = x;
                trail->upper[0] = x;
                trail->lower[1] = y;
                trail->upper[1] = y;
                trail->lower[2] = z;
                trail->upper[2] = z;
'''
        assert old in s
        s=s.replace(old,'''                trail->lower[0] = trail->origin[0];
                trail->lower[1] = trail->origin[1];
                trail->lower[2] = trail->origin[2];
                trail->upper[0] = trail->origin[0];
                trail->upper[1] = trail->origin[1];
                trail->upper[2] = trail->origin[2];
''')
        s=s.replace('    f32 x;\n','').replace('    f32 y, z;\n','')
    return s
if len(sys.argv)>1 and sys.argv[1]=='2':
    outs.clear()
    for h,c,x,u in itertools.product(('none','orig','fade'),(0,1),(1,0),(0,1)):
        emit(f'm2_{h}c{c}x{x}u{u}',make2(h,c,x,u,0),['func_8038CB10','func_8038CB18'])
    print(' '.join(outs))
