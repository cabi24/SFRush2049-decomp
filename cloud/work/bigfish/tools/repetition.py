import sys,re,collections
sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__file__),'..','..','..','..','tools','cloud')); import score
for name in sys.argv[1:]:
    w=score.targets()[name]
    def sh(x):
        op=x>>26
        if op in(0,17): return x&0xfc00003f
        return x&0xfc000000
    k=12
    seq=[sh(x) for x in w]
    c=collections.Counter(tuple(seq[i:i+k]) for i in range(len(seq)-k))
    cov=[0]*len(seq)
    for i in range(len(seq)-k):
        if c[tuple(seq[i:i+k])]>1:
            for j in range(i,i+k): cov[j]=1
    lui=collections.Counter(x&0xffff for x in w if x>>26==15)
    gbi=[hex(v) for v,cn in lui.most_common(12)]
    br=sum(1 for x in w if x>>26 in(4,5,6,7,20,21,22,23) or (x>>26==1) or (x>>26==17 and (x>>21)&31==8))
    fp=sum(1 for x in w if x>>26 in(17,49,53,57,61))
    mem=sum(1 for x in w if x>>26 in(32,33,35,36,37,40,41,43))
    print(f'{name}: {len(w)}w; {sum(cov)/len(w):.0%} of words in 12-word shape repeats; branches {br} ({br/len(w):.0%}), fp {fp}, mem {mem}, jal {sum(1 for x in w if x>>26==3)}, jr(non-ra) {sum(1 for x in w if x>>26==0 and x&0x3f==8 and (x>>21)&31!=31)}')
    print('   top lui imms:',gbi)
