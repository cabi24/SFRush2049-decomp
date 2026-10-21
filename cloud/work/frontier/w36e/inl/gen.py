#!/usr/bin/env python3
"""gen.py NSITES CALLERSIZE -> writes t_<n>_<c>/group.c + group.json"""
import sys, os, json
n=int(sys.argv[1]); c=int(sys.argv[2]); tag=sys.argv[3] if len(sys.argv)>3 else ''
d=f't_{n}_{c}{tag}'; os.makedirs(d, exist_ok=True)
src=['typedef struct Blit Blit;','extern Blit *gA, *gB; extern int gX[64]; extern volatile short gWait;','extern void RemoveBlit(Blit *); extern void Other(int);','']
src.append('void unload(void)\n{\n    if (gA) { RemoveBlit(gA); gA = 0; }\n    if (gB) { RemoveBlit(gB); gB = 0; }\n}\n')
for i in range(n):
    body=''.join(f'    gX[{(k*7+i)%64}] += gX[{(k*3+i)%64}] * {k+1};\n' for k in range(c))
    src.append(f'void caller{i}(void)\n{{\n{body}    unload();\n    Other({i});\n}}\n')
src.append('void root(void)\n{\n'+''.join(f'    caller{i}();\n    caller{i}();\n' for i in range(n))+'}\n')
open(f'{d}/group.c','w').write('\n'.join(src))
json.dump({'members':[],'context':[],'files':['group.c'],'keep':['root'],'flags':'-g0 -O3 -mips2 -G 0 -non_shared'},open(f'{d}/group.json','w'))
print(d)
