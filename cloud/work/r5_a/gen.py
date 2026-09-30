import sys, itertools, subprocess, json, shutil, os, re
HDR='''typedef signed int s32;
typedef unsigned int u32;
typedef struct { u32 w0; u32 w1; } Gfx;

extern u32 D_8012E608;
extern Gfx *D_80149438;
extern s32 D_8014A248;

void func_80086A50(s32 mode)
{
    Gfx *g;
    switch (mode) {
'''
def push(w0, w1, order):
    a = f"        g = D_80149438; D_80149438 = g + 1;\n"
    if order == 0: return a + f"        g->w0 = {w0};\n        g->w1 = {w1};\n"
    if order == 1: return a + f"        g->w1 = {w1};\n        g->w0 = {w0};\n"
    if order == 2: return a + f"        {{ u32 x = {w1}; g->w0 = {w0}; g->w1 = x; }}\n"
    if order == 3: return a + f"        {{ u32 x = {w0}; g->w1 = {w1}; g->w0 = x; }}\n"
    if order == 4: return f"        g = D_80149438; D_80149438 = g + 1;\n        g->w0 = {w0};\n        g->w1 = {w1};\n".replace("g = D_80149438; D_80149438 = g + 1;","g = D_80149438++;")
    return a + f"        g[0].w0 = {w0};\n        g[0].w1 = {w1};\n"
def ind(s, n): return ''.join(' '*n + l + '\n' for l in s.splitlines())
def gen(o):
    # o: dict role-> order ; role keys 'c0','c1'..'c4' cond push, 'p1','p2' per case 'p1_k','p2_k'
    def P(role, w0, w1): return push(w0, w1, o.get(role, o.get(role[0], 0)))
    s = HDR
    s += "    case 0:\n        if (D_8014A248 != 0) {\n" + ind(P('c_0','0xE3000A01','0x200000'),4) + "        }\n"
    s += P('a_0','0xE200001C','0x0F0A4000') + P('b_0','0xFCFFFFFF','0xFFFCF279') + "        break;\n"
    tab = {1:[('0x00504A70','0xFC119623','0xFF2FFFFF'),('0x00504240','0xFC119623','0xFF2FFFFF'),('0x00504A70','0xFCFFFFFF','0xFFFCF279'),('0x00504240','0xFCFFFFFF','0xFFFCF279')],
           2:[('0x00504A70','0xFC119623','0xFF2FFFFF'),('0x00504240','0xFC119623','0xFF2FFFFF'),('0x00504A70','0xFC11FE23','0xFFFFF3F9'),('0x00504240','0xFC11FE23','0xFFFFF3F9')],
           3:[('0x00553078','0xFC119623','0xFF2FFFFF'),('0x0F0A7008','0xFC119623','0xFF2FFFFF'),('0x00553078','0xFC11FE23','0xFFFFF3F9'),('0x0F0A7008','0xFC11FE23','0xFFFFF3F9')]}
    conds=['(D_8012E608 & 0x10) && (D_8012E608 & 0x20)','D_8012E608 & 0x20','D_8012E608 & 0x10',None]
    for k in (1,2,3):
        s += f"    case {k}:\n        if (D_8014A248 <= 0 || D_8014A248 >= 4) {{\n" + ind(P(f'c_{k}','0xE3000A01','0'),4) + "        }\n"
        for i,(t,w0,w1) in enumerate(tab[k]):
            c=conds[i]
            head = f"        if ({c}) {{\n" if i==0 else (f"        else if ({c}) {{\n" if c else "        else {\n")
            s += head + ind(P(f'a_{k}','0xE200001C',t),4) + ind(P(f'b_{k}',w0,w1),4) + "        }\n"
        s += "        break;\n"
    s += "    case 4:\n        if (D_8014A248 < 4) {\n" + ind(P('c_4','0xE3000A01','0x100000'),4) + "        }\n"
    s += P('a_4','0xE200001C','0x00504240') + P('b_4','0xFCFFABFF','0xFFFC9238') + P('d_4','0xFB000000','0x55')
    s += "        break;\n    }\n    D_8014A248 = mode;\n}\n"
    return s
