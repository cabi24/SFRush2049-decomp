#!/usr/bin/env python3
"""scanpat.py IMG: find 'lw rX; beq rX,zero; li at,K' (delay-slot li) vs 'lw rX; li at,K; beq rX,zero; nop' in retail IMG."""
import re,sys,os,json
img=sys.argv[1]
R='/home/cburnes/projects/rush2049-decomp/asm/us/'+img
f=[R+'/'+x for x in os.listdir(R) if x.endswith('.s')][0]
cur=None; words={}
for l in open(f):
    m=re.match(r'^(func_[0-9A-Fa-f]+|[A-Za-z_]\w*):\s*$',l)
    if m: cur=m.group(1); words[cur]=[]; continue
    m=re.match(r'\s*\.word 0x([0-9A-Fa-f]{8})',l)
    if m and cur: words[cur].append(int(m.group(1),16))
def op(w): return w>>26
def rs(w): return (w>>21)&31
def rt(w): return (w>>16)&31
A=[];B=[]
for fn,ws in words.items():
    for i in range(len(ws)-3):
        a,b,c,d=ws[i:i+4]
        # lw rt ; beq rt,zero ; li at,K (addiu at,zero,K)
        if op(a)==0x23 and op(b)==4 and rs(b)==rt(a) and rt(b)==0 and (c>>16)==0x2401:
            A.append((fn,i))
        if op(a)==0x23 and (b>>16)==0x2401 and op(c)==4 and rs(c)==rt(a) and rt(c)==0 and d==0:
            B.append((fn,i))
print('delay-slot li (retail AD57C kind):',len(A)); print(' ',sorted(set(f for f,_ in A)))
print('hoisted li + nop (ours kind):',len(B)); print(' ',sorted(set(f for f,_ in B)))
