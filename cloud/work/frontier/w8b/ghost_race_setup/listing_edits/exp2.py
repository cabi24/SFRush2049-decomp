from exp import *
def likely(w):
    op=w>>26
    return op in (20,21,22,23) or (op==1 and ((w>>16)&31) in (2,3,18,19)) or (op==17 and ((w>>21)&31)==8 and ((w>>16)&2))
def f2(words):
    ws=int_writes(words)
    skip={i+1 for i,w in enumerate(words) if likely(w)}
    ring=[r for i,r,*_ in ws if r in (14,15,24,25) and i not in skip]
    lowset={r for _,r,*_ in ws if 8<=r<=13}
    wraps=sum(1 for a,b in zip(ring,ring[1:]) if a==25 and b==14)
    return dict(n=len(ring),wraps=wraps,low0=not lowset,t5=13 not in lowset,some=len(lowset)<6)
G={n:f2(w) for n,w in W.items()}
ex=['func_80099B30','func_80091B00','func_800B66B0','func_800D1AB0']
for e in ex: print(e,G[e])
def cal(label,pred):
    s=[n for n in singles if pred(G[n])]; g=[n for n in groupm if pred(G[n])]; u=[n for n in unm if pred(G[n])]
    print(f"{label:28s} singles {len(s):3d}/{len(singles)} group {len(g):3d}/{len(groupm)} unm {len(u):3d}/{len(unm)} ex {''.join('Y' if pred(G[e]) else '-' for e in ex)} {s[:5]}")
for c in ('low0','t5','some'):
    for wv in (1,2,3,4):
        cal(f"wraps>={wv} {c}", lambda f:f['wraps']>=wv and f[c])
# how many group members are 'internal' by other signatures
mem=set(ipa['members'])
un={n for n in W if F.unsaved_callee_writes(W[n])}
print('group members ipa-member or unsaved:', sum(1 for n in groupm if n in mem or n in un), 'singles', sum(1 for n in singles if n in mem or n in un))
for c,wv in (('low0',1),('t5',2),('t5',1)):
    p=lambda f:f['wraps']>=wv and f[c]
    new=[n for n in unm if p(G[n]) and n not in mem and n not in un]
    print(c,wv,'new unmatched beyond ipa/unsaved:',len(new))
    print('  group hits not otherwise detected', sum(1 for n in groupm if p(G[n]) and n not in mem and n not in un))
