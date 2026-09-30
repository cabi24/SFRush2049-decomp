import os,difflib,sys,re
from pathlib import Path
os.environ['GD']=str(Path('g2').resolve())
import rb,score
from dislib import dis
src=Path(sys.argv[1]).read_text()
gm,e=rb.words_of(src,'render_large_objects'); _,wmask=e
wm=[w&m for w,m in zip(score.targets()['render_large_objects'],wmask)]
T=dis(wm); G=dis(gm)
def op(x): return x.split()[0]
sm=difflib.SequenceMatcher(None,[op(x) for x in T],[op(x) for x in G],autojunk=False)
print('opcode LCS',sum(b.size for b in sm.get_matching_blocks()),len(T),len(G))
a=int(sys.argv[2]) if len(sys.argv)>2 else 0
n=0
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal' or i1<a: continue
    print('--',tag,i1,i2,j1,j2)
    for x in T[i1:i2][:40]: print('  T',x)
    for x in G[j1:j2][:40]: print('  G',x)
    n+=1
    if n>int(sys.argv[3] if len(sys.argv)>3 else 6): break
