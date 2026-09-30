import sys, tempfile, difflib
from pathlib import Path
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud')
import score
FLAGS='-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
_cache={}
def evaluate(srcpath, fn, flags=FLAGS):
    want=score.targets()[fn]
    with tempfile.TemporaryDirectory() as t:
        obj=Path(t)/'o.o'
        try: score.compile_single(srcpath, flags, obj)
        except BaseException as e: return None
        words=score.text_words(obj); fns=score.symbols(obj)
        if fn not in fns: return None
        start=fns[fn]
        end=min((o for o in fns.values() if o>start), default=len(words)*4)
        res,masks,*_=score.relocate(obj,words,start,end,score.image_symbols())
        got=res[start//4:end//4]
    gm=[g&masks.get(start+4*i,0xFFFFFFFF) for i,g in enumerate(got)]
    wm=[w&masks.get(start+4*i,0xFFFFFFFF) for i,w in enumerate(want)]
    sm=difflib.SequenceMatcher(None,wm,gm,autojunk=False)
    ex=sum(b.size for b in sm.get_matching_blocks())
    strict=sum(1 for i in range(min(len(gm),len(wm))) if gm[i]==wm[i]) if len(gm)==len(wm) else 0
    return len(got),ex,strict
if __name__=='__main__':
    print(evaluate(sys.argv[1],sys.argv[2], *(sys.argv[3:4])))
