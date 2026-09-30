import sys,tempfile,difflib
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud'); import score
src,fn=sys.argv[1:3]; flags=sys.argv[3] if len(sys.argv)>3 else "-g0 -O2 -mips2 -G 0 -non_shared"
out=tempfile.mktemp(suffix='.o'); score.compile_single(src,flags,out)
words=score.text_words(out); fs=score.symbols(out); st=fs[fn]; want=score.targets()[fn]
end=min([o for o in fs.values() if o>st],default=len(words)*4)
res,masks,*_=score.relocate(out,words,st,min(st+4*len(want),end),score.image_symbols())
got=res[st//4:end//4]
while got and got[-1]==0: got=got[:-1]
def op(w):
    o=w>>26; return (o,w&0x3f) if o in (0,17) else o
def lcs(a,b):
    return sum(m.size for m in difflib.SequenceMatcher(None,a,b,autojunk=False).get_matching_blocks())
a=[op(w) for w in want]; b=[op(w) for w in got]
print(f"{fn}: target {len(want)}w got {len(got)}w | opcode-LCS {lcs(a,b)}/{len(want)} ({100*lcs(a,b)//len(want)}%) | exact-word-LCS {lcs(list(want),list(got))}/{len(want)}")
