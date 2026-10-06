from exp import *
POOL=(31,13,12,11,10,9,8)
def runs(words, gap):
    """best (length) of descending load run into POOL followed by stores of the same regs in same order"""
    best=0; n=len(words); i=0
    while i<n:
        op, rt = words[i]>>26, (words[i]>>16)&31
        if 32<=op<=39 and rt in POOL:
            seq=[rt]; j=i+1; g=0
            while j<n and g<=gap:
                o2, r2 = words[j]>>26, (words[j]>>16)&31
                if 32<=o2<=39 and r2 in POOL and r2<seq[-1]:
                    seq.append(r2); g=0
                elif 32<=o2<=39 and r2 in POOL: break
                else: g+=1
                j+=1
            # stores
            k=j-g if g else j
            k=i+1
            st=[ (words[m]>>16)&31 for m in range(i+1,min(n,i+len(seq)*3+gap+8)) if (words[m]>>26) in (40,41,43) and ((words[m]>>16)&31) in seq and ((words[m]>>21)&31)!=29]
            if len(seq)>=2 and st[:len(seq)]==seq: best=max(best,len(seq))
            i=j if len(seq)>1 else i+1
        else: i+=1
    return best
for gap in (0,2,4):
  R={n:runs(w,gap) for n,w in W.items()}
  for k in (2,3,4):
    s=[n for n in singles if R[n]>=k]; g=[n for n in groupm if R[n]>=k]; u=[n for n in unm if R[n]>=k]
    print(f"gap {gap} len>={k}: singles {len(s)}/{len(singles)} group {len(g)}/{len(groupm)} unm {len(u)}/{len(unm)} {s[:4]} {g[:4]} {u[:8]}")
