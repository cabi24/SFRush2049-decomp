import re,sys,json,collections
name=sys.argv[1]; sd=sys.argv[2]
L=[l for l in open(f'{sd}/{name}.dis') if re.match(r'\s+[0-9a-f]+:',l)]
ins=[list(re.match(r'\s+([0-9a-f]+):\s+(\S+)\s*(.*)',l).groups()) for l in L]
def sx(v): return v-0x10000 if v&0x8000 else v
def num(s): return int(s,0)
# label targets
targets=set()
for a,op,ops in ins:
    m=re.search(r'0x([0-9a-f]{8})',ops)
    if m and (op.startswith('b') or op=='j') : targets.add(int(m.group(1),16))
lastlui={}  # reg -> index
pair={}     # lui idx -> base addr
users={}    # idx -> (luiidx, lo)
for i,(a,op,ops) in enumerate(ins):
    m=re.match(r'(\$?\w+),0x([0-9a-f]+)$',ops)
    if op=='lui' and m:
        lastlui[m.group(1)]=i; continue
    # mem: rt,imm(base)
    m=re.match(r'(\$?\w+),(-?\d+)\((\w+)\)$',ops)
    m2=re.match(r'(\w+),(\w+),(-?\d+)$',ops)
    base=None
    if m and m.group(3) in lastlui and op[0] in 'lsc' and op not in('lui',): base=(m.group(3),int(m.group(2)))
    elif m2 and op in('addiu','ori') and m2.group(2) in lastlui:
        base=(m2.group(2),int(m2.group(3)))
    if base:
        li=lastlui[base[0]]
        hi=int(re.match(r'\w+,0x([0-9a-f]+)',ins[li][2]).group(1),16)
        addr=((hi<<16)+ (sx(base[1]&0xffff)) )&0xffffffff if op!='ori' else (hi<<16)|(base[1]&0xffff)
        users[i]=(li,addr)
        pair.setdefault(li,addr)
    # kill on write (approx)
    dst=re.match(r'(\$?\w+),',ops)
    if op in('addu','daddu') and re.match(r'(\w+),\1,',ops): continue
    if dst and op not in('sw','sh','sb','swc1','sdc1','beq','bne','beqz','bnez','bgez','bltz','blez','bgtz','jr','sd') and dst.group(1) in lastlui and not (i in users and False):
        # a write to reg that pair-user just consumed ends the range only if it's the dest
        if op!='lui': lastlui.pop(dst.group(1),None)
out=['.set noat','.set noreorder','',f'glabel {name}']
def sym(addr,base=None): return f'D_{addr:08X}'
for i,(a,op,ops) in enumerate(ins):
    if int(a,16) in targets: out.append(f'.L{int(a,16):08X}:')
    if op=='lui' and i in pair:
        r=ops.split(',')[0]; ba=pair[i]
        ops=f'{r}, %hi({sym(ba)})'
    elif i in users:
        li,addr=users[i]; ba=pair[li]; d=addr-ba
        s=sym(ba)+(f'{d:+d}' if d else '')
        m=re.match(r'(\$?\w+),(-?\d+)\((\w+)\)$',ops)
        if m: ops=f'{m.group(1)}, %lo({s})({m.group(3)})'
        else:
            m2=re.match(r'(\w+),(\w+),(-?\d+)$',ops); ops=f'{m2.group(1)}, {m2.group(2)}, %lo({s})'
    else:
        mm=re.search(r'0x([0-9a-f]{8})(\s+<(.*)>)?',ops)
        if mm and (op.startswith('b') or op=='j'):
            ops=re.sub(r'0x[0-9a-f]{8}.*',f'.L{int(mm.group(1),16):08X}',ops)
        elif mm and op=='jal':
            nm=(mm.group(3) or '').split(',')[0]
            ops=nm if nm and nm!='?' else f'func_{int(mm.group(1),16):08X}'
        else: ops=re.sub(r'\s+<.*','',ops)
    out.append(f'  {op} {ops}'.replace('\t',' ').rstrip())
open(f'{sd}/{name}.s','w').write('\n'.join(out)+'\n')
print(len(out))
