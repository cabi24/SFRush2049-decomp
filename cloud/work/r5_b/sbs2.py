import subprocess,sys,re,difflib
src,fn=sys.argv[1],sys.argv[2]; flags=sys.argv[3:] or ['-g0','-O2','-mips2','-G','0','-non_shared']
D='/home/user/SFRush2049-decomp/tools/cloud/ido/cc'
subprocess.run([D,'-c',*flags,'-Wab,-r4300_mul','-o','/tmp/claude-0/sbs.o',src],capture_output=True)
o=subprocess.run(['mips-linux-gnu-objdump','-d','-M','reg-names=numeric,no-aliases','/tmp/claude-0/sbs.o'],capture_output=True,text=True).stdout
mine=[re.sub(r'\s+',' ',l.split('\t',2)[2]) for l in o.splitlines() if re.match(r'\s+[0-9a-f]+:\t',l) and len(l.split('\t'))>2]
mine=[re.sub(r'<.*','',m).strip() for m in mine]
t=subprocess.run(['python3','/home/user/SFRush2049-decomp/cloud/work/tools/tdis.py',fn],capture_output=True,text=True).stdout
tg=[re.sub(r'<.*','',re.sub(r'\s+',' ',l.split(': ',1)[1])).strip() for l in t.splitlines()[1:] if ': ' in l]
def op(x): return x.split(' ')[0]
sm=difflib.SequenceMatcher(None,[op(x) for x in tg],[op(x) for x in mine],autojunk=False)
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal': continue
    print('--- target',i1,i2,'mine',j1,j2,tag)
    for x in tg[i1:i2]: print('  T',x)
    for x in mine[j1:j2]: print('  M',x)
