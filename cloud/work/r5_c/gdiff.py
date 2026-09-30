import sys,difflib,os
from pathlib import Path
import rb
from dislib import dis
import score
FN=os.environ.get('FN','func_800DE860')
src=Path(sys.argv[1]).read_text()
gm,e=rb.words_of(src,FN)
if gm is None: print(e); sys.exit()
_,wmask=e
wm=[w&m for w,m in zip(score.targets()[FN],wmask)]
g=dis(gm); w=dis(wm)
sm=difflib.SequenceMatcher(None,w,g,autojunk=False)
print('LCS',sum(b.size for b in sm.get_matching_blocks()),len(g))
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal': continue
    # skip pure branch-offset diffs
    print('--',tag,i1,j1)
    for x in w[i1:i2]: print('  T',x)
    for x in g[j1:j2]: print('  G',x)
