import sys,re,difflib,itertools; sys.path.insert(0,'.')
import ev
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud')
import score
def regs_of(w):
    op=w>>26
    if op==0: return [(21,5),(16,5),(11,5)]
    if op==0x11 or op in (2,3): return []
    return [(21,5),(16,5)]
def canon(text, show=25):
    got,masks,st=ev.compile_words(text)
    want=[w&masks.get(st+4*i,0xFFFFFFFF) for i,w in enumerate(ev.WANT)]
    sm=difflib.SequenceMatcher(None,[ev.norm(w) for w in want],[ev.norm(w) for w in got],autojunk=False)
    pairs=[]
    for b in sm.get_matching_blocks():
        for k in range(b.size): pairs.append((b.a+k,b.b+k))
    # co-occurrence matrix over all regs (0..31)
    from collections import Counter
    co=Counter()
    for a,b in pairs:
        for sh,_ in regs_of(want[a]):
            ra=(want[a]>>sh)&31; rb=(got[b]>>sh)&31
            co[(rb,ra)]+=1
    # greedy permutation only among s-regs 16..23, 30 (s8), 31 (ra)
    S=[16,17,18,19,20,21,22,23,30,31]
    best=None
    # assign by hungarian-ish greedy
    mp={}
    used=set()
    cand=sorted(((co[(m,r)],m,r) for m in S for r in S),reverse=True)
    for c,m,r in cand:
        if m in mp or r in used: continue
        mp[m]=r; used.add(r)
    def remap(w,frm=None):
        op=w>>26
        for sh,_ in regs_of(w):
            r=(w>>sh)&31
            if r in mp: w=(w&~(31<<sh))|(mp[r]<<sh)
        return w
    got2=[remap(w) for w in got]
    names={16:'s0',17:'s1',18:'s2',19:'s3',20:'s4',21:'s5',22:'s6',23:'s7',30:'s8',31:'ra'}
    print('s-reg map (mine->ROM):',{names[m]:names[r] for m,r in mp.items() if m!=r})
    exact=sum(1 for a,b in pairs if want[a]==got2[b])
    print('aligned exact after s-remap:',exact,'of',len(want))
    n=0
    for a,b in pairs:
        if want[a]!=got2[b]:
            print(a,score.disasm_word(want[a]),'|',score.disasm_word(got2[b])); n+=1
            if n>=show: break
if __name__=='__main__':
    canon(open(sys.argv[1]).read(), int(sys.argv[2]) if len(sys.argv)>2 else 25)
