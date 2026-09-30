import sys, tempfile, difflib
from pathlib import Path
ROOT=Path('/home/user/SFRush2049-decomp'); sys.path.insert(0,str(ROOT/'tools'/'cloud'))
import score
FLAGS='-g0 -O2 -mips2 -G 0 -non_shared'
def got_words(src_path, fn, flags=FLAGS):
    with tempfile.TemporaryDirectory() as t:
        obj=Path(t)/'o.o'
        try: score.compile_single(str(src_path), flags, obj)
        except BaseException as e: return None, None
        words=score.text_words(obj); fns=score.symbols(obj)
        st=fns[fn]; end=min((o for o in fns.values() if o>st), default=len(words)*4)
        res,masks,*_=score.relocate(obj,words,st,end,score.image_symbols())
        got=res[st//4:end//4]
        gm=[g&masks.get(st+4*i,0xFFFFFFFF) for i,g in enumerate(got)]
        want=score.targets()[fn]
        wm=[w&masks.get(st+4*i,0xFFFFFFFF) for i,w in enumerate(want)]
        return gm, wm
def sc(src_path, fn, flags=FLAGS):
    gm,wm=got_words(src_path,fn,flags)
    if gm is None: return (-1,-1,-1)
    strict=sum(1 for a,b in zip(gm,wm) if a==b) if len(gm)==len(wm) else 0
    sm=difflib.SequenceMatcher(None,wm,gm,autojunk=False)
    return (sum(b.size for b in sm.get_matching_blocks()), len(gm), strict)
if __name__=='__main__': print(sc(sys.argv[1], sys.argv[2]))
