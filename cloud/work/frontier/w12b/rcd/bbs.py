import sys,re
L=open(sys.argv[1]).read().split('\n')
start=[i for i,l in enumerate(L) if l.startswith('\t.frame')][0]
br=re.compile(r'^\t(b|beq|bne|blt|bge|ble|bgt|bltu|bgeu|bleu|bgtu|beqz|bnez|j|jal|jr|bltz|bgez|blez|bgtz|bc1t|bc1f)\t')
bbs=[[]]; 
for i in range(0,len(L)):
    l=L[i]
    if l.endswith(':') and not l.startswith('\t'):
        if bbs[-1]: bbs.append([])
        bbs[-1].append(l); continue
    if not l.startswith('\t') or l.strip().startswith('.'): 
        continue
    bbs[-1].append(l.strip())
    if br.match(l) and (len(sys.argv)<3 or not l.startswith('\tjal')):
        bbs.append([])
bbs=[b for b in bbs if b]
for k,b in enumerate(bbs): print(k,' | '.join(b)[:150])
