# gen1.py: return-merge sweep for func_800A1BB4 (w14p round 1): final-exit forms x trailing shapers x loop forms
import itertools, os, sys
HEAD = """/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef int s32;
typedef struct Sub { s8 pad0[5]; s8 on; s8 pad6[34]; } Sub;
typedef struct Rec { s8 pad0[11]; s8 active; s8 pad12[116]; Sub sub[16]; s8 pad[4]; } Rec;
typedef struct ModelLink { struct ModelData *data; } ModelLink;
typedef struct ModelData { ModelLink *next; s8 pad4[13]; u8 wheel; s8 pad18[54]; s32 enabled; } ModelData;
typedef struct ModelList { ModelLink *head; s32 pad[3]; } ModelList;
extern Rec D_80144030[];
extern ModelList D_80144D68[];

void func_800A1BB4(s32 index) {
    ModelData *data;
    ModelLink *p, *next;
"""
FINAL = {
 'f1': "    if (p == 0) D_80144030[index].active = 0;\n",
 'f2': "    if (p == 0) { D_80144030[index].active = 0; }\n    return;\n",
 'f3': "    if (p != 0) return;\n    D_80144030[index].active = 0;\n",
 'f4': "    if (p == 0) { D_80144030[index].active = 0; return; }\n    return;\n",
}
TRAIL = {
 't0': "",
 't1': "    if (index) {}\n",
 't2': "    if (data) {}\n",
}
EARLY = {
 'e1': "    if (D_80144030[index].active == 0) return;\n",
 'e2': "    if (D_80144030[index].active == 0) { return; }\n",
 'e3': "    if (!D_80144030[index].active) return;\n",
}
LOOP = {
 'l1': "    while (1) { if (p == 0) break;\n        data = p->data;\n        next = data->next;\n        if (data->enabled != 0) {\n            if (D_80144030[index].sub[data->wheel].on != 0) break;\n        }\n        p = next;\n    }\n",
 'l2': "    while (p != 0) {\n        data = p->data;\n        next = data->next;\n        if (data->enabled != 0) {\n            if (D_80144030[index].sub[data->wheel].on != 0) break;\n        }\n        p = next;\n    }\n",
}
PINIT = {
 'p1': "    p = D_80144D68[index].head;\n    if (p == 0) { D_80144030[index].active = 0; return; }\n",
 'p2': "    p = D_80144D68[index].head;\n",
}
def make(e, p, l, f, t):
    return (HEAD + EARLY[e] + PINIT[p] + LOOP[l] + FINAL[f] + TRAIL[t] + "}\n")
if __name__ == '__main__':
    out = sys.argv[1]; os.makedirs(out, exist_ok=True)
    n = 0
    for e, p, l, f, t in itertools.product(EARLY, PINIT, LOOP, FINAL, TRAIL):
        open(os.path.join(out, f"v{n:04d}_{e}_{p}_{l}_{f}_{t}.c"), "w").write(make(e, p, l, f, t))
        n += 1
    print(n)
