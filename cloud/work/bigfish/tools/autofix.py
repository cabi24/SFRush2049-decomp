import re,subprocess,sys
cc='/home/user/SFRush2049-decomp/tools/cloud/ido/cc'
src,dst,fn=sys.argv[1:4]
t=open(src).read().splitlines()
# strip prototypes for callees except own
for it in range(40):
    open(dst,'w').write('\n'.join(t)+'\n')
    p=subprocess.run([cc,'-c','-g0','-O2','-mips2','-G','0','-non_shared','-o','/tmp/x.o',dst],capture_output=True,text=True)
    err=(p.stderr+p.stdout).splitlines()
    errs=[(i,l) for i,l in enumerate(err) if 'Error' in l]
    if not errs: print('COMPILES after',it,'fixes'); break
    changed=False
    for i,l in errs:
        m=re.search(r'line (\d+): (.*)',l)
        ln=int(m.group(1)); msg=m.group(2)
        code=err[i+1] if i+1<len(err) else ''
        if "'sp" in msg and 'undefined' in msg:
            v=re.search(r"'(\w+)' undefined",msg).group(1)
            # declare as s32 at top of function
            k=next(j for j,x in enumerate(t) if re.match(rf'^\w[\w \*]* {fn}\(',x) and '{' in x)
            t.insert(k+1,f'    s32 {v};'); changed=True;break
        if 'Subscripting a non-array' in msg:
            names=re.findall(r'\b(sp[0-9A-Fa-f]+)\[',code)
            for nm in names:
                for j,x in enumerate(t):
                    if re.match(rf'^    \w+ {nm};$',x): t[j]=x.replace(f'{nm};',f'{nm}[16];'); changed=True
            if changed: break
        if "number of arguments" in msg:
            mm=re.search(r'(func_\w+|\w+)\(',t[ln-1])
            # find call on this line or previous
            names=re.findall(r'\b([A-Za-z_]\w*)\(',code)
            for nme in names:
                idx=[j for j,x in enumerate(t) if re.match(rf'^[A-Za-z_][\w \*]*\b{nme}\(.*\);',x)]
                if idx:
                    for j in reversed(idx): t[j]=f'int {nme}();'
                    changed=True;break
            if changed:break
    if not changed: print('STUCK',errs[:3]); break
