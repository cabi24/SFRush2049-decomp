import json, struct, sys
from pathlib import Path
from tools.conveyor.pipeline import frontier as F
doc = json.loads(F.LAYOUT_JSON.read_text())
funcs = F.functions(doc); base = int(doc["image"]["base"],16)
image = (F.REPO/doc["image"]["path"]).read_bytes()
lock = json.loads(F.LOCKFILE.read_text())
W = {}
for v,s,n in funcs:
    o=v-base; W[n]=struct.unpack(f">{s//4}I", image[o:o+s-s%4])
ipa = json.loads(F.MEMBERS_JSON.read_text())
WR = F._WRITES_RT
def int_writes(words):
    """[(index, reg, kind)]"""
    out=[]
    for i,w in enumerate(words):
        op, rs, rt, rd = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        if op == 0:
            if (w & 63) not in F._NO_RD and w != 0: out.append((i,rd,'r'))
        elif op in WR: out.append((i,rt,'load' if op>=32 else 'imm', ))
        elif op == 17 and rs in (0,2): out.append((i,rt,'mf'))
    return out
def feats(words):
    ws = int_writes(words)
    regs = [r for _,r,*_ in ws]
    ring = [r for r in regs if 24<=r<=25 or 14<=r<=15]
    low = set(r for r in regs if 8<=r<=13)
    order = {14:0,15:1,24:2,25:3}
    wraps = sum(1 for a,b in zip(ring,ring[1:]) if a==25 and b==14)
    seqok = sum(1 for a,b in zip(ring,ring[1:]) if (order[a]+1)%4==order[b])
    # ra writes other than lw ra,x(sp)
    ra = 0
    for i,w in enumerate(words):
        op, rs, rt, rd = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        if op in WR and rt==31 and not (op==35 and rs==29): ra+=1
        if op==0 and (w&63) not in F._NO_RD and w!=0 and rd==31 and (w&63)!=9: ra+=1
    # descending load runs
    best=0; run=0; prev=None
    for w in words:
        op, rt = w>>26, (w>>16)&31
        if 32<=op<=39 and rt in (31,13,12,11,10,9,8):
            if prev is not None and rt<prev: run+=1
            else: run=1
            prev=rt
        else:
            run=0; prev=None
        best=max(best,run)
    return dict(n=len(ring), low=len(low), wraps=wraps, seqok=seqok, ra=ra, desc=best)
FE = {n:feats(w) for n,w in W.items()}
singles=[n for n,v in lock.items() if 'group' not in v and n in W]
groupm=[n for n,v in lock.items() if 'group' in v and n in W]
unm=[n for n in W if n not in lock]
members=set(ipa['members'])
if __name__=="__main__":
    print(len(singles),len(groupm),len(unm))
    for ex in ['func_80099B30','func_80091B00','func_800B66B0','func_800D1AB0','Input_ApplyPadConfig']: print(ex,FE[ex], ex in members, F.unsaved_callee_writes(W[ex]))
    def cal(label,pred):
        s=[n for n in singles if pred(FE[n])]; g=[n for n in groupm if pred(FE[n])]; u=[n for n in unm if pred(FE[n])]
        print(f"{label:40s} singles {len(s):3d}/{len(singles)} group {len(g):3d}/{len(groupm)} unmatched {len(u):3d}/{len(unm)}  {s[:6]}")
    for k in (5,6,8,10,12):
        cal(f"n>={k} low==0", lambda f:f['n']>=k and f['low']==0)
        cal(f"n>={k} low<6", lambda f:f['n']>=k and f['low']<6)
    for k in (1,2,3):
        cal(f"wraps>={k} low==0", lambda f:f['wraps']>=k and f['low']==0)
        cal(f"wraps>={k} low<6", lambda f:f['wraps']>=k and f['low']<6)
    cal("ra>=1", lambda f:f['ra']>=1)
    for k in (2,3,4,5):
        cal(f"desc>={k}", lambda f:f['desc']>=k)
        cal(f"desc>={k} & ra", lambda f:f['desc']>=k and f['ra'])
