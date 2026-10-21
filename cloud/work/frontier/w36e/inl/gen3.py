#!/usr/bin/env python3
"""gen3.py NBLOCKS : caller with NBLOCKS if-blocks calling clear twice; plus small caller calling clear"""
import sys, os, json
nb=int(sys.argv[1]); tag=sys.argv[2] if len(sys.argv)>2 else ''
d=f'b_{nb}{tag}'; os.makedirs(d, exist_ok=True)
src=['typedef struct Blit Blit;','extern Blit *gA, *gB; extern int gX[64]; extern volatile short gWait; extern int gObj;','extern void RemoveBlit(Blit *); extern void Other(int); extern void RemoveObj(short);','']
src.append('void unload(void)\n{\n    if (gA) { RemoveBlit(gA); gA = 0; }\n    if (gB) { RemoveBlit(gB); gB = 0; }\n}\nvoid clear(void)\n{\n    unload();\n    if (gObj != -1) { RemoveObj(gObj); gObj = -1; }\n    while (gWait) {}\n}\n')
body=''.join(f'    if (gX[{k%64}] > {k}) Other({k});\n' for k in range(nb))
src.append(f'void big(void)\n{{\n{body}    if (gX[3]) clear();\n    if (gX[4]) clear();\n}}\n')
src.append('void small(void)\n{\n    gX[1] = 2;\n    clear();\n    Other(9);\n}\n')
src.append('void root(void)\n{\n    big();\n    big();\n    small();\n    small();\n}\n')
open(f'{d}/group.c','w').write('\n'.join(src))
json.dump({'members':[],'context':[],'files':['group.c'],'keep':['root'],'flags':'-g0 -O3 -mips2 -G 0 -non_shared'},open(f'{d}/group.json','w'))
print(d)
