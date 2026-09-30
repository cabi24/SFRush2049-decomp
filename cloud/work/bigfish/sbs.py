import sys,tempfile,difflib,re,subprocess
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud'); sys.path.insert(0,'/home/user/SFRush2049-decomp/cloud/work/bigfish')
import score
src,fn,flags=sys.argv[1],sys.argv[2],sys.argv[3]
lo=int(sys.argv[4]) if len(sys.argv)>4 else 0; hi=int(sys.argv[5]) if len(sys.argv)>5 else 80
want=score.targets()[fn]
import pathlib
with tempfile.TemporaryDirectory() as t:
    obj=pathlib.Path(t)/'o.o'; score.compile_single(src,flags,obj)
    words=score.text_words(obj); fns=score.symbols(obj); st=fns[fn]
    end=min((o for o in fns.values() if o>st),default=len(words)*4)
    res,masks,*_=score.relocate(obj,words,st,end,score.image_symbols())
    got=res[st//4:end//4]
def dis(w): return score.disasm_word(w)
sm=difflib.SequenceMatcher(None,[w>>16<<0 if False else w for w in want],got,autojunk=False)
rows=[]
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    for k in range(max(i2-i1,j2-j1)):
        a=want[i1+k] if i1+k<i2 else None; b=got[j1+k] if j1+k<j2 else None
        rows.append((tag,i1+k if a is not None else None,dis(a) if a is not None else '',j1+k if b is not None else None,dis(b) if b is not None else ''))
for r in rows[lo:hi]:
    print(f"{r[0][0]} {str(r[1]):>4} {r[2]:32s} | {str(r[3]):>4} {r[4]}")
