import re,struct,sys
lines=[l.rstrip() for l in open('rlo.dis') if re.match(r'\s+800',l)]
ins=[]
for l in lines:
    m=re.match(r'\s+([0-9a-f]+):\s+(\S+)\s*(.*)',l)
    a,op,rest=m.groups(); ins.append((int(a,16),op,rest.split('<')[0].strip()))
def f32(bits): return struct.unpack('>f',struct.pack('>I',bits))[0]
F={}; R={}
start=int(sys.argv[1],16); end=int(sys.argv[2],16)
def fv(r): return F.get(r,r)
for a,op,rest in ins:
    if a<start or a>end: continue
    args=[x.strip() for x in rest.split(',')] if rest else []
    if op=='lui': R[args[0]]=int(args[1],16)<<16; continue
    if op=='li': R[args[0]]=int(args[1]); continue
    if op=='mtc1':
        if args[0] in R and args[0]=='at': F[args[1]]='%g'%f32(R['at'] & 0xffffffff)
        elif args[0]=='zero': F[args[1]]='0.0'
        else: F[args[1]]='(f)%s'%args[0]
        print('%x  %s = %s'%(a,args[1],F[args[1]])); continue
    if op=='lwc1':
        m=re.match(r'(-?\d+)\((\w+)\)',args[1]); off=int(m.group(1)); base=m.group(2)
        if base=='at' and 'at' in R:
            addr=(R['at']+off)&0xffffffff; F[args[0]]='K%X'%addr
        else: F[args[0]]='mem[%s%+d]'%(base,off)
        print('%x  %s = %s'%(a,args[0],F[args[0]])); continue
    if op in('mul.s','add.s','sub.s','div.s'):
        s={'mul.s':'*','add.s':'+','sub.s':'-','div.s':'/'}[op]
        F[args[0]]='(%s %s %s)'%(fv(args[1]),s,fv(args[2])); print('%x  %s = %s'%(a,args[0],F[args[0]])); continue
    if op=='mov.s': F[args[0]]=fv(args[1]); print('%x  %s = %s   (mov)'%(a,args[0],F[args[0]])); continue
    if op=='jal':
        print('%x  CALL f12=%s f14=%s f16=%s f18=%s f20=%s'%(a,fv('$f12'),fv('$f14'),fv('$f16'),fv('$f18'),fv('$f20')))
        F['$f0']='RET@%x'%a; continue
    if op in('cvt.s.w',): F[args[0]]='(float)'+fv(args[1]); continue
