# gen2.py: structure-wrap sweep for func_800A1BB4 (w14p round 2): retail's early exit is an inline jr ra on the
# active==0 fall-through, so try the body under `if (active != 0)` with no early return.
import itertools, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import gen1
BODY = {
 'b1': "    p = D_80144D68[index].head;\n    while (1) { if (p == 0) break;\n        data = p->data;\n        next = data->next;\n        if (data->enabled != 0) {\n            if (D_80144030[index].sub[data->wheel].on != 0) break;\n        }\n        p = next;\n    }\n    if (p == 0) D_80144030[index].active = 0;\n",
 'b2': "    p = D_80144D68[index].head;\n    while (p != 0) {\n        data = p->data;\n        if (data->enabled != 0) {\n            if (D_80144030[index].sub[data->wheel].on != 0) break;\n        }\n        p = p->data->next;\n    }\n    if (p == 0) D_80144030[index].active = 0;\n",
 'b3': "    p = D_80144D68[index].head;\n    while (p != 0) {\n        data = p->data;\n        next = data->next;\n        if (data->enabled != 0) {\n            if (D_80144030[index].sub[data->wheel].on != 0) break;\n        }\n        p = next;\n    }\n    if (p == 0) D_80144030[index].active = 0;\n",
}
WRAP = {
 'w1': "    if (D_80144030[index].active != 0) {\n{BODY}    }\n",
 'w2': "    if (D_80144030[index].active) {\n{BODY}    }\n",
 'w3': "    if (D_80144030[index].active != 0) {\n{BODY}    } else { }\n",
}
def make(w, b):
    return gen1.HEAD + WRAP[w].replace("{BODY}", BODY[b]) + "}\n"
if __name__ == '__main__':
    out = sys.argv[1]; os.makedirs(out, exist_ok=True)
    n = 0
    for w, b in itertools.product(WRAP, BODY):
        open(os.path.join(out, f"v{n:04d}_{w}_{b}.c"), "w").write(make(w, b)); n += 1
    print(n)
