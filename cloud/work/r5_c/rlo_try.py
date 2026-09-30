import sys, os, json, difflib
from pathlib import Path
os.environ['GD']=str(Path('g2').resolve())
import rb, score
def run(src, fn='render_large_objects'):
    gm,e=rb.words_of(src,fn)
    if gm is None: return None
    _,wmask=e
    wm=[w&m for w,m in zip(score.targets()[fn],wmask)]
    sm=difflib.SequenceMatcher(None,wm,gm,autojunk=False)
    return sum(b.size for b in sm.get_matching_blocks()), len(gm)
if __name__=='__main__':
    print(run(Path(sys.argv[1]).read_text()))
