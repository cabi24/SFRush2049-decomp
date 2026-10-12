import sys,re,subprocess,difflib,tempfile,os
obj,vram,size=sys.argv[1],int(sys.argv[2],16),int(sys.argv[3],0)
ROM='/home/cburnes/projects/rush2049-decomp/.claude/worktrees/w25a/baserom.us.z64'
def dis(args):
    d=subprocess.run(['mips-linux-gnu-objdump','-d','--no-show-raw-insn','-M','reg-names=numeric' if False else 'gpr-names=numeric']+args,capture_output=True,text=True).stdout
    out=[]
    for l in d.splitlines():
        m=re.match(r'\s+[0-9a-f]+:\t(\S+)\s*(.*)',l)
        if m: out.append((m.group(1)+' '+re.sub(r'\s*<.*>','',m.group(2))).strip())
    return out
a=dis([obj])
off=vram-0x80000400+0x1000
raw=open(ROM,'rb').read()[off:off+size]
fd,p=tempfile.mkstemp(); os.write(fd,raw); os.close(fd)
b=dis(['-b','binary','-m','mips:4000','-EB','-D',p]); os.unlink(p)
def norm(x): return re.sub(r'-?\b(0x[0-9a-f]+|\d+)\b','#',x)
na=[norm(x) for x in a]; nb=[norm(x) for x in b]
sm=difflib.SequenceMatcher(None,na,nb,autojunk=False)
eq=sum(i2-i1 for t,i1,i2,j1,j2 in sm.get_opcodes() if t=='equal')
print('ours %d retail %d equal %d nonequal_retail %d'%(len(a),len(b),eq,len(b)-eq))
if len(sys.argv)>4:
    for tag,i1,i2,j1,j2 in sm.get_opcodes():
        if tag=='equal': print('   = %d'%(i2-i1)); continue
        for k in range(max(i2-i1,j2-j1)):
            print('%s %-36s | %s'%(tag[0], a[i1+k] if i1+k<i2 else '', b[j1+k] if j1+k<j2 else ''))
