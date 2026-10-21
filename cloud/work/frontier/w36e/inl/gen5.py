#!/usr/bin/env python3
import sys, os, json
kind=sys.argv[1]
d=f'm_{kind}'; os.makedirs(d, exist_ok=True)
pre={'unused':'    int i;\n','unusedinit':'    int i = 0;\n'}.get(kind,'')
post={'ret':'    return;\n','emptyif':'    if (gX[0]) {}\n','semi':'    ;\n','selfassign':'    gA = gA;\n','castvoid':'    (void)gX[0];\n','elseif':''}.get(kind,'')
mid='''    if (gA) { RemoveBlit(gA); gA = 0; }
    if (gB) { RemoveBlit(gB); gB = 0; }
'''
if kind=='elseif': mid='''    if (gA) { RemoveBlit(gA); gA = 0; }
    if (gB != 0) { RemoveBlit(gB); gB = 0; }
'''
if kind=='nested': mid='''    if (gA) { RemoveBlit(gA); gA = 0; }
    if (gB) { RemoveBlit(gB); gB = (Blit *)0; }
'''
if kind=='twofn': mid='''    Blit *b;
    if ((b = gA)) { RemoveBlit(b); gA = 0; }
    if ((b = gB)) { RemoveBlit(b); gB = 0; }
'''
src=f'''typedef struct Blit Blit;
extern Blit *gA, *gB; extern int gX[64];
extern void RemoveBlit(Blit *); extern void Other(int);
void unload(void)
{{
{pre}{mid}{post}}}
void c1(void) {{ int i; for (i = 0; i < 9; i++) Other(i); unload(); Other(1); }}
void c2(void) {{ int i; for (i = 0; i < 9; i++) Other(i); unload(); Other(2); }}
void root(void) {{ c1(); c1(); c2(); c2(); }}
'''
open(f'{d}/group.c','w').write(src)
json.dump({'members':[],'context':[],'files':['group.c'],'keep':['root'],'flags':'-g0 -O3 -mips2 -G 0 -non_shared'},open(f'{d}/group.json','w'))
print(d)
