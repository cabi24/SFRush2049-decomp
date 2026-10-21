#!/usr/bin/env python3
import sys, os, json
k=int(sys.argv[1]); kind=sys.argv[2]
d=f'k_{kind}_{k}'; os.makedirs(d, exist_ok=True)
extra={'st':''.join(f'    gX[{i}] = {i};\n' for i in range(k)),
       'dead':'    if (0) {\n'+''.join(f'        gX[{i}] = {i};\n' for i in range(k))+'    }\n',
       'call':''.join(f'    Other({i});\n' for i in range(k))}[kind]
src=f'''typedef struct Blit Blit;
extern Blit *gA, *gB; extern int gX[64];
extern void RemoveBlit(Blit *); extern void Other(int);
void unload(void)
{{
    if (gA) {{ RemoveBlit(gA); gA = 0; }}
    if (gB) {{ RemoveBlit(gB); gB = 0; }}
{extra}}}
void c1(void) {{ int i; for (i = 0; i < 9; i++) Other(i); unload(); Other(1); }}
void c2(void) {{ int i; for (i = 0; i < 9; i++) Other(i); unload(); Other(2); }}
void root(void) {{ c1(); c1(); c2(); c2(); }}
'''
open(f'{d}/group.c','w').write(src)
json.dump({'members':[],'context':[],'files':['group.c'],'keep':['root'],'flags':'-g0 -O3 -mips2 -G 0 -non_shared'},open(f'{d}/group.json','w'))
print(d)
