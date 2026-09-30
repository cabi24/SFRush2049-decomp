import sys,re
name=sys.argv[1]
L=open(sys.argv[2]).read().splitlines()[1:]
ins=[]
for l in L:
    m=re.match(r'\s*([0-9a-f]+):\s+(\S+)\s*(.*)',l)
    if m: ins.append((int(m.group(1),16),m.group(2),m.group(3)))
targets=set()
for a,op,ar in ins:
    m=re.search(r'0x([0-9a-f]+)\s*$',ar.split('<')[0].strip()) if (op.startswith('b') or op in('j',)) else None
    if m: targets.add(int(m.group(1),16))
R=r'\b(zero|at|v[01]|a[0-3]|t[0-9]|s[0-8]|k[01]|gp|sp|fp|ra)\b'
out=['.set noat','.set noreorder','',f'glabel {name}']
for a,op,ar in ins:
    if a in targets: out.append(f'.L{a:08X}:')
    nm=re.search(r'<([^,>]*)',ar)
    ar=re.sub(r'\s*<.*>','',ar)
    if op in('jal','j') and nm and nm.group(1)!='?':
        ar=nm.group(1)
    elif op in('jal','j'):
        t=int(ar.strip(),16) if ar.strip().startswith('0x') else None
        ar=f'func_{t:08X}' if t else ar
    elif op.startswith('b') and re.search(r'0x[0-9a-f]+$',ar):
        ar=re.sub(r'0x([0-9a-f]+)$',lambda m:f'.L{int(m.group(1),16):08X}',ar)
    ar=re.sub(R,r'$\1',ar)
    ar=ar.replace('$f','$f')
    out.append(f'/* {a:08X} */ {op} {ar}'.replace(',',', ') if False else f'    {op} {ar}')
open(sys.argv[3],'w').write('\n'.join(out)+'\n')
