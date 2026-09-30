import sys,tempfile,pathlib,collections,re
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud'); import score
src,fn,flags=sys.argv[1:4]
want=score.targets()[fn]
with tempfile.TemporaryDirectory() as t:
    obj=pathlib.Path(t)/'o.o'; score.compile_single(src,flags,obj)
    words=score.text_words(obj); fns=score.symbols(obj); st=fns[fn]
    end=min((o for o in fns.values() if o>st),default=len(words)*4)
    got=words[st//4:end//4]
def slots(ws):
    c=collections.Counter()
    for w in ws:
        op=w>>26
        if (op in (0x23,0x2b,0x31,0x39,0x21,0x29,0x20,0x28,0x25,0x24) ) and ((w>>21)&0x1f)==29: c[w&0xffff]+=1
    return c
a,b=slots(want),slots(got)
print('target',sorted((k,v) for k,v in a.items()))
print('mine  ',sorted((k,v) for k,v in b.items()))
