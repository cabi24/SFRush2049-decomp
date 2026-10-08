# gen5.py: default outside the switch behind an explicit range test (w14p round 5)
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import gen1
src0 = gen1.make('start', 'direct', 'k1')
def variant(form):
    s = src0
    # remove the default line from the switch and build a custom head
    s = s.replace("         default: kind=1; amount=D_80121D64; break;\n", "")
    if form == 'if_else':
        s = s.replace("        switch(effect->effect) {\n",
            "        if((u32)(effect->effect-350)>10u) { kind=1; amount=D_80121D64; } else\n        switch(effect->effect) {\n")
    elif form == 'if_before_switch_goto':
        s = s.replace("        switch(effect->effect) {\n",
            "        if((u32)(effect->effect-350)>10u) { kind=1; amount=D_80121D64; goto join; }\n        switch(effect->effect) {\n")
        s = s.replace("        if(amount) {}\n", "    join:;\n        if(amount) {}\n", 1)
    elif form == 'if_else_default_break':
        s = s.replace("        switch(effect->effect) {\n",
            "        if((u32)(effect->effect-350)>10u) { kind=1; amount=D_80121D64; } else\n        switch(effect->effect) {\n         default: break;\n")
    return s
if __name__ == '__main__':
    out = sys.argv[1]; os.makedirs(out, exist_ok=True)
    for i, f in enumerate(['if_else', 'if_before_switch_goto', 'if_else_default_break']):
        open(os.path.join(out, f"v{i:04d}_{f}.c"), 'w').write(variant(f))
    print(3)
