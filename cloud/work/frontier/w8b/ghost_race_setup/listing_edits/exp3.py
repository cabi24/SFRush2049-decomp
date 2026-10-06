from exp import *
import subprocess
def raw(words):
    out=[]
    for i,w in enumerate(words):
        op, rs, rt, rd = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        if op in WR and rt==31 and not (op in(35,55) and rs==29): out.append(i)
        if op==0 and (w&63) not in F._NO_RD and w!=0 and rd==31 and (w&63)!=9: out.append(i)
    return out
for grp,label in ((singles,'single'),(groupm,'group'),(unm,'unm')):
    for n in grp:
        r=raw(W[n])
        if r or FE[n]['desc']>=2: print(label,n,'ra@',r,'desc',FE[n]['desc'],len(W[n]))
