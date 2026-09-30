"""ev.py: in-process evaluator. import ev; ev.evaluate(src_text) -> dict"""
import sys, tempfile, difflib, pathlib, os
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud')
import score
FN='net_session_update'
FLAGS='-g0 -O2 -mips2 -G 0 -non_shared'
WANT=score.targets()[FN]
SYMS=score.image_symbols()
def shape(w):
    op=w>>26
    if op==0: return (0,w&0x3f)
    if op==1: return (1,(w>>16)&0x1f)
    return (op,)
WS=[shape(w) for w in WANT]
def compile_words(text, flags=FLAGS, tag=None):
    d=tempfile.mkdtemp(prefix='ev_',dir=os.environ.get('EVTMP','/tmp'))
    try:
        src=pathlib.Path(d)/'a.c'; src.write_text(text); obj=pathlib.Path(d)/'o.o'
        score.compile_single(str(src),flags,obj)
        words=score.text_words(obj); fns=score.symbols(obj); st=fns[FN]
        end=min((o for o in fns.values() if o>st),default=len(words)*4)
        res,masks,*_=score.relocate(obj,words,st,end,SYMS)
        got=res[st//4:end//4]
        gm=[g&masks.get(st+4*i,0xFFFFFFFF) for i,g in enumerate(got)]
        return gm, masks, st
    finally:
        import shutil; shutil.rmtree(d,ignore_errors=True)
_wm=None
def evaluate(text, flags=FLAGS):
    try:
        got,masks,st=compile_words(text,flags)
    except BaseException as e:
        return None
    # want masked similarly: use same masks (offsets relative)
    want=[w&masks.get(st+4*i,0xFFFFFFFF) for i,w in enumerate(WANT)]
    n=len(want)
    strict=sum(1 for i in range(min(len(got),n)) if got[i]==want[i])
    sm=difflib.SequenceMatcher(None,want,got,autojunk=False)
    ex=sum(b.size for b in sm.get_matching_blocks())
    sm2=difflib.SequenceMatcher(None,WS,[shape(w) for w in got],autojunk=False)
    sh=sum(b.size for b in sm2.get_matching_blocks())
    return dict(n=len(got),strict=strict,exact=ex,shape=sh)
if __name__=='__main__':
    print(evaluate(open(sys.argv[1]).read()))
