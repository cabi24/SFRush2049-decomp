import re,sys,subprocess
name=sys.argv[1]
out=subprocess.run(['python3','/home/user/SFRush2049-decomp/cloud/work/tools/tdis.py',name],capture_output=True,text=True).stdout.splitlines()
rows=[]
for l in out[1:]:
    m=re.match(r'\s+([0-9a-f]+):\s+(\S+)\s*(.*)',l)
    if m: rows.append((int(m.group(1),16),m.group(2),m.group(3)))
tg=set()
for a,op,ar in rows:
    if op.startswith('b') or op in('j',):
        t=re.search(r'0x([0-9a-f]+)\s*$',ar)
        if t: tg.add(int(t.group(1),16))
import json
syms={int(v,16):k for k,v in json.load(open('/home/user/SFRush2049-decomp/asm/us/blob/symbols.json'))['symbols'].items()}
def sym(a): return syms.get(a) if a in syms and not syms[a].startswith('func_') else f'D_{a:08X}'
lui={}  # reg -> (imm, row index)
rows2=[]
for i,(a,op,ar) in enumerate(rows):
    ar=re.sub(r'\s*<.*>$','',ar)
    m=re.match(r'(\w+),0x([0-9a-f]+)$',ar)
    if op=='lui' and m:
        lui[m.group(1)]=(int(m.group(2),16),i); rows2.append([a,op,ar]); continue
    m=re.match(r'(\$?\w+),(-?\d+)\((\w+)\)$',ar)
    mi=re.match(r'(\w+),(\w+),(-?\d+)$',ar)
    if m and m.group(3) in lui:
        imm,li=lui[m.group(3)]; addr=((imm<<16)+int(m.group(2)))&0xffffffff
        rows2[li][2]=re.sub(r',0x[0-9a-f]+$',f',%hi({sym(addr)})',rows2[li][2]) if '%hi' not in rows2[li][2] else rows2[li][2]
        ar=f'{m.group(1)},%lo({sym(addr)})({m.group(3)})'
    elif mi and op=='addiu' and mi.group(2) in lui:
        imm,li=lui[mi.group(2)]; addr=((imm<<16)+int(mi.group(3)))&0xffffffff
        if '%hi' not in rows2[li][2]: rows2[li][2]=re.sub(r',0x[0-9a-f]+$',f',%hi({sym(addr)})',rows2[li][2])
        ar=f'{mi.group(1)},{mi.group(2)},%lo({sym(addr)})'
    # writes invalidate lui tracking
    mm=re.match(r'(\w+),',ar)
    if mm and op not in('sw','sb','sh','swc1','sdc1','sd') and not op.startswith('b') and mm.group(1)!=None and op!='lui':
        if mm.group(1) in lui and not (mi and op=='addiu' and mi.group(2)==mm.group(1) and False): 
            if mi is None or mi.group(2)!=mm.group(1) or True: lui.pop(mm.group(1),None) if not (op=='addiu' and mi and mi.group(1)!=mi.group(2)) else None
    rows2.append([a,op,ar])
rows=[tuple(r) for r in rows2]
res=[f'glabel {name}']
for a,op,ar in rows:
    if a in tg: res.append(f'.L{a:08X}:')
    if op=='jal':
        t=int(re.search(r'0x([0-9a-f]+)',ar).group(1),16)
        ar=f'func_{t:08X}'
    elif op.startswith('b') or op=='j':
        t=re.search(r'0x([0-9a-f]+)\s*$',ar)
        if t: ar=ar[:t.start()]+f'.L{int(t.group(1),16):08X}'
    ar=ar.replace('gpr','')
    res.append(f'  {op} {ar}')
print('\n'.join(res))
