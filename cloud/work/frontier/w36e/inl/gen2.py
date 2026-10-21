#!/usr/bin/env python3
"""gen2.py VARIANT NSITES CALLERSIZE"""
import sys, os, json
v=sys.argv[1]; n=int(sys.argv[2]); c=int(sys.argv[3])
d=f'u_{v}_{n}_{c}'; os.makedirs(d, exist_ok=True)
src=['typedef struct Blit Blit;','extern Blit *gA, *gB; extern int gX[64]; extern volatile short gWait; extern int gObj;','extern void RemoveBlit(Blit *); extern void Other(int); extern void RemoveObj(short);','']
U={'base':'void unload(void)\n{\n    if (gA) { RemoveBlit(gA); gA = 0; }\n    if (gB) { RemoveBlit(gB); gB = 0; }\n}\n',
'static':'void unload(void)\n{\n    static int cnt;\n    cnt++;\n    if (gA) { RemoveBlit(gA); gA = 0; }\n    if (gB) { RemoveBlit(gB); gB = 0; }\n}\n',
'dead':'void unload(void)\n{\n    if (gA) { RemoveBlit(gA); gA = 0; }\n    if (gB) { RemoveBlit(gB); gB = 0; }\n    if (0) { gX[1]=1; gX[2]=2; gX[3]=3; gX[4]=4; gX[5]=5; gX[6]=6; gX[7]=7; gX[8]=8; Other(1); Other(2); Other(3); Other(4); }\n}\n',
'loop':'void unload(void)\n{\n    if (gA) { RemoveBlit(gA); gA = 0; }\n    if (gB) { RemoveBlit(gB); gB = 0; }\n    while (gWait) {}\n}\n',
'big':'void unload(void)\n{\n    if (gA) { RemoveBlit(gA); gA = 0; }\n    if (gB) { RemoveBlit(gB); gB = 0; }\n    if (gX[0]) { RemoveBlit(gA); gA = 0; }\n    if (gX[1]) { RemoveBlit(gB); gB = 0; }\n    if (gX[2]) { RemoveBlit(gA); gA = 0; }\n    if (gX[3]) { RemoveBlit(gB); gB = 0; }\n}\n',
'clear':'void unload(void)\n{\n    if (gA) { RemoveBlit(gA); gA = 0; }\n    if (gB) { RemoveBlit(gB); gB = 0; }\n}\nvoid clear(void)\n{\n    unload();\n    if (gObj != -1) { RemoveObj(gObj); gObj = -1; }\n    while (gWait) {}\n}\n',
}
src.append(U[v])
call = 'clear' if v=='clear' else 'unload'
for i in range(n):
    body=''.join(f'    gX[{(k*7+i)%64}] += gX[{(k*3+i)%64}] * {k+1};\n' for k in range(c))
    src.append(f'void caller{i}(void)\n{{\n{body}    {call}();\n    Other({i});\n}}\n')
src.append('void root(void)\n{\n'+''.join(f'    caller{i}();\n    caller{i}();\n' for i in range(n))+'}\n')
open(f'{d}/group.c','w').write('\n'.join(src))
json.dump({'members':[],'context':[],'files':['group.c'],'keep':['root'],'flags':'-g0 -O3 -mips2 -G 0 -non_shared'},open(f'{d}/group.json','w'))
print(d)
