#!/usr/bin/env python3
"""close.py C_FILE FN [--flags ...]: instruction-aligned closeness. Compiles, then LCS-aligns
compiled vs target words under 3 normalisations: exact (imm/jal-target masked), opcode+regs (imm masked),
opcode-only (mnemonic class)."""
import sys,tempfile,difflib,argparse
from pathlib import Path
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud'); import score
ap=argparse.ArgumentParser();ap.add_argument('c');ap.add_argument('fn');ap.add_argument('--flags',default='-g0 -O2 -mips2 -G 0 -non_shared');a=ap.parse_args()
with tempfile.TemporaryDirectory() as t:
    obj=Path(t)/'o.o'; p=score.compile_single(a.c,a.flags,obj) if True else None
    if p is not None and getattr(p,'returncode',0): print('compile failed',p.stderr[:500] if hasattr(p,'stderr') else p);sys.exit(1)
    fns=score.symbols(obj); words=score.text_words(obj)
    order=sorted(fns.items(),key=lambda x:x[1])
    o=fns[a.fn]; nxt=[x for n,x in order if x>o]; e=nxt[0] if nxt else len(words)*4
    mine=words[o//4:e//4]
tgt=score.targets()[a.fn]
def norm(w,mode):
    op=w>>26
    if mode==0:
        if op in(2,3): return w>>26
        return w&0xffff0000 if op not in(0,17) else w   # keep op+rs+rt, drop imm16
    if mode==1:
        return w if op==0 or op==17 else (w&0xffff0000 if op not in (2,3) else op)
    if mode==2: # mnemonic class
        if op==0: return ('r',w&0x3f)
        if op==17: return ('c',(w>>21)&0x1f,w&0x3f)
        return op
res=[]
for m,name in ((1,'op+regs (imm masked)'),(2,'opcode only')):
    A=[norm(w,m) for w in mine];B=[norm(w,m) for w in tgt]
    sm=difflib.SequenceMatcher(None,A,B,autojunk=False)
    lcs=sum(b.size for b in sm.get_matching_blocks())
    print(f'{a.fn}: mine={len(mine)} target={len(tgt)} {name}: LCS {lcs}/{len(tgt)} = {100*lcs/len(tgt):.1f}%')
A=list(mine);B=list(tgt)
sm=difflib.SequenceMatcher(None,A,B,autojunk=False); lcs=sum(b.size for b in sm.get_matching_blocks())
print(f'   exact-word LCS {lcs}/{len(tgt)}; strict positional equal {sum(1 for x,y in zip(A,B) if x==y)}')
if len(sys.argv)>0:
    import os
    pre=int(os.environ.get('PREFIX','0'))
    if pre:
        B=list(tgt[:pre]);
        for m,name in ((1,'op+regs'),(2,'opcode')):
            A2=[norm(w,m) for w in mine];B2=[norm(w,m) for w in B]
            sm=difflib.SequenceMatcher(None,A2,B2,autojunk=False)
            l=sum(b.size for b in sm.get_matching_blocks()); print(f'  prefix[{pre}] {name}: {l}/{pre} = {100*l/pre:.1f}% (mine {len(mine)})')
        sm=difflib.SequenceMatcher(None,list(mine),B,autojunk=False); print('  prefix exact-word LCS',sum(b.size for b in sm.get_matching_blocks()))
