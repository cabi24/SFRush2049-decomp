import sys,difflib,subprocess,tempfile,struct
from pathlib import Path
import sc
def dis(words):
    with tempfile.TemporaryDirectory() as t:
        p=Path(t)/'a.bin'; p.write_bytes(b''.join(struct.pack('>I',w) for w in words))
        out=subprocess.run(['mips-linux-gnu-objdump','-D','-b','binary','-mmips:isa32','-EB','-M','reg-names=o32',str(p)],capture_output=True,text=True).stdout
    return [ ' '.join(l.split('\t')[2:]).strip() for l in out.splitlines() if '\t' in l and len(l.split('\t'))>2]
gm,wm=sc.got_words(sys.argv[1],'func_800DE860')
g=dis(gm); w=dis(wm)
sm=difflib.SequenceMatcher(None,w,g,autojunk=False)
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal': continue
    print('--',tag,i1,j1)
    for x in w[i1:i2]: print('  T',x)
    for x in g[j1:j2]: print('  G',x)
