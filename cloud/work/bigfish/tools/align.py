import sys,subprocess,struct,difflib,json,re
sys.path.insert(0,''+__import__('os').path.abspath(__import__('os').path.join(__import__('os').path.dirname(__file__),'..','..','..','..'))+'/tools/cloud'); import score
src,name=sys.argv[1],sys.argv[2]; flags=sys.argv[3] if len(sys.argv)>3 else '-g0 -O2 -mips2 -G 0 -non_shared'
cc=''+__import__('os').path.abspath(__import__('os').path.join(__import__('os').path.dirname(__file__),'..','..','..','..'))+'/tools/cloud/ido/cc'
o=src+'.o'
r=subprocess.run([cc]+flags.split()+['-c','-o',o,src],capture_output=True,text=True)
if r.returncode: print(r.stderr[-1500:]); sys.exit(1)
out=subprocess.run(['mips-linux-gnu-objdump','-h','-t','-b','elf32-tradbigmips',o],capture_output=True,text=True).stdout
# find .text and symbol
import re
mt=re.search(r'\.text\s+([0-9a-f]+)\s',out); 
st=[l for l in out.splitlines() if l.rstrip().endswith(' '+name) and ' F ' in l or (l.rstrip().endswith(' '+name) and '.text' in l)]
addr=int(st[0].split()[0],16)
subprocess.run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',o,o+'.bin'])
b=open(o+'.bin','rb').read()
allw=struct.unpack(f'>{len(b)//4}I',b)
tgt=score.targets()[name]
# function extent: to next function symbol
syms=sorted(int(l.split()[0],16) for l in out.splitlines() if ' F ' in l and '.text' in l)
nxt=[s for s in syms if s>addr]; end=(nxt[0] if nxt else len(b))//4
w=allw[addr//4:end]
def norm(x): return x
def opk(x):
    op=x>>26
    return (op, x&0x3f) if op==0 else (op, (x>>16)&0x1f if op in(1,17) else 0)
def lcs(a,b):
    sm=difflib.SequenceMatcher(None,a,b,autojunk=False); return sum(m.size for m in sm.get_matching_blocks())
# mask reloc fields: jal target, lui/addiu immediates for reloc are already 0 in .o; mask target-side imm for lui/lw/sw/addiu whose base is non-sp? approximate: compare exact words and 'shape' (words with 16-bit imm zeroed for non-sp)
def shape(x):
    op=x>>26
    if op==3 or op==2: return x>>26
    if op in (15,) : return x&0xffe00000|0   # lui: keep reg
    rs=(x>>21)&31
    if op in (9,32,33,35,36,37,40,41,43,49,57,8,10,11,12,13,14) and rs!=29 and False: return x&0xffff0000
    return x
def opreg(x):  # opcode+regs, ignore immediates (for I-type) 
    op=x>>26
    if op==0 or op==17: return x
    if op in(2,3): return op
    return x&0xffff0000
n=len(tgt); m=len(w)
print(f'{name}: target {n} words, compiled {m} words')
print(f'  opcode-shape LCS: {lcs([opk(x) for x in tgt],[opk(x) for x in w])}/{n} = {lcs([opk(x) for x in tgt],[opk(x) for x in w])/n:.1%}')
l2=lcs([opreg(x) for x in tgt],[opreg(x) for x in w]); print(f'  opcode+regs (imm ignored) LCS: {l2}/{n} = {l2/n:.1%}')
l3=lcs(list(tgt),list(w)); print(f'  exact-word LCS: {l3}/{n} = {l3/n:.1%}')
