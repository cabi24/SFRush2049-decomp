import sys,tempfile,difflib,pathlib
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud')
import score
src=sys.argv[1]; lo=int(sys.argv[2]); hi=int(sys.argv[3]); fn='net_session_update'
flags=sys.argv[4] if len(sys.argv)>4 else '-g0 -O2 -mips2 -G 0 -non_shared'
want=score.targets()[fn]
def shape(w):
    op=w>>26
    if op==0: return (0,w&0x3f)
    if op==1: return (1,(w>>16)&0x1f)
    return (op,)
with tempfile.TemporaryDirectory() as t:
    obj=pathlib.Path(t)/'o.o'; score.compile_single(src,flags,obj)
    words=score.text_words(obj); fns=score.symbols(obj); st=fns[fn]
    end=min((o for o in fns.values() if o>st),default=len(words)*4)
    res,masks,*_=score.relocate(obj,words,st,end,score.image_symbols())
    got=res[st//4:end//4]
sm=difflib.SequenceMatcher(None,[shape(w) for w in want],[shape(w) for w in got],autojunk=False)
rows=[]
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    for k in range(max(i2-i1,j2-j1)):
        a=want[i1+k] if i1+k<i2 else None; b=got[j1+k] if j1+k<j2 else None
        rows.append(('=' if a==b and a is not None else ('~' if tag=='equal' else '!'),i1+k if a is not None else None,score.disasm_word(a) if a is not None else '',j1+k if b is not None else None,score.disasm_word(b) if b is not None else ''))
for r in rows[lo:hi]:
    print(f"{r[0]} {str(r[1]):>4} {r[2]:30s} | {str(r[3]):>4} {r[4]}")
