# gen1.py: placement x default-load-form x kind-form matrix for func_8010D3C0 (w14p round 1)
import itertools, os, sys
base = open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'start.c')).read()
hdr = base[:base.index('void func_8010D3C0')]
cases = {
350: "kind=0;\n            amount=D_80121D60;\n            break;",
351: "kind=1;\n            amount=D_80121D64;\n            break;",
352: "kind=2;\n            amount=D_80121D68;\n            break;",
353: "car->timer=800; return;",
354: "car->extra_flags|=1;\n            car->state=1;\n            car->boost=30.0f;\n            car->field936=0.666667f;\n            car->color=255;\n            return;",
355: "kind=3;\n            amount=D_80121D6C;\n            break;",
356: "kind=4;\n            amount=D_80121D70;\n            break;",
357: "kind=5;\n            amount=D_80121D74;\n            break;",
358: "kind=6;\n            amount=D_80121D78;\n            break;",
359: "car->extra_flags|=10;\n            car->field916=0.0333333f;\n            car->field920=0.05f;\n            car->field924=0.0f;\n            return;",
360: "kind=7;\n            amount=D_80121D7C;\n            break;",
}
order = [350,351,352,353,354,355,356,357,358,359,360]
def case_text():
    return ''.join(f"        case {k}:\n            {cases[k]}\n" for k in order)
dload = {
 'direct': "amount=D_80121D64;",
 'ptr': "amount=*dp;",
 'vol': "amount=*(volatile s32 *)&D_80121D64;",
 'addr': "amount=*(s32 *)&D_80121D64;",
}
dkind = {'k1': "kind=1;", 'k1u': "kind=1U;"}
def make(place, dl, dk):
    dbody = f"{dkind[dk]} {dload[dl]}"
    decl = "    s32 *dp;\n" if dl == 'ptr' else ""
    pre = "        dp=&D_80121D64;\n" if dl == 'ptr' else ""
    t = base[base.index('    if(effect->flags&4) {'):]
    pre_sw = t[:t.index('        switch(effect->effect) {')]
    post = t[t.index('        if(amount) {}'):]
    if place == 'start':
        sw = "        switch(effect->effect) {\n         default: " + dbody + " break;\n" + case_text() + "        }\n"
    elif place == 'end':
        sw = "        switch(effect->effect) {\n" + case_text() + "        default: " + dbody + " break;\n        }\n"
    else:
        sw = ("        switch(effect->effect) {\n" + case_text() + "        default: goto dflt;\n        }\n"
              "        goto join;\n    dflt:\n        " + dbody + "\n    join:;\n")
    head = hdr + "void func_8010D3C0(EffectView *effect)\n{\n    Car952 *car;\n    Vehicle2056 *vehicle;\n    s32 kind,amount;\n    Limits16 *limits;\n    f32 maximum;\n" + decl
    return head + pre_sw + pre + sw + post
if __name__ == '__main__':
    out = sys.argv[1]
    os.makedirs(out, exist_ok=True)
    n = 0
    for place, dl, dk in itertools.product(['start', 'end', 'goto'], dload, dkind):
        open(os.path.join(out, f"v{n:04d}_{place}_{dl}_{dk}.c"), 'w').write(make(place, dl, dk))
        n += 1
    print(n)
