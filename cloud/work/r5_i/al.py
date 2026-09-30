import sys, tempfile, difflib, shutil
from pathlib import Path
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud'); import score
def aligned(gd_json, src, fns):
    t=Path(tempfile.mkdtemp())
    try:
        shutil.copy(gd_json,t/'group.json'); (t/'group.c').write_text(src)
        obj=t/'o.o'
        try: score.compile_group(t,obj)
        except SystemExit as e: return None
        words=score.text_words(obj); f=score.symbols(obj); out={}
        for fn in fns:
            start=f[fn]; end=min((o for o in f.values() if o>start),default=len(words)*4)
            res,masks,*_=score.relocate(obj,words,start,end,score.image_symbols())
            got=res[start//4:end//4]; want=score.targets()[fn]
            gm=[g&masks.get(start+4*i,0xFFFFFFFF) for i,g in enumerate(got)]
            wm=[w&masks.get(start+4*i,0xFFFFFFFF) for i,w in enumerate(want)]
            sm=difflib.SequenceMatcher(None,wm,gm,autojunk=False)
            eq=sum(b.size for b in sm.get_matching_blocks())
            out[fn]=(len(wm)-eq, max(len(gm)-eq,0))
        return out
    finally: shutil.rmtree(t)
