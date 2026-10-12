#!/usr/bin/env python3
"""w22b: write the func_80023E9C draft (dispatch table from the retail jump table via m2c.c, prototypes from
lib_22300's definitions by struct tag). usage: python3 gen23e9c.py BODY_TEMPLATE OUT.c"""
import re, sys
R = "/home/cburnes/projects/rush2049-decomp"
m2c = open(R + "/cloud/work/frontier/w22b/func_80023E9C/m2c.c").read()
tu = open(R + "/src/rom/lib_22300.c").read()
cases = re.findall(r"case (0x[0-9A-F]+):\n\s+var_v1_2 = (func_[0-9A-F]+)\(arg0, &D_80056228(?:, (\d))?\)", m2c)
protos, tags, seen = [], [], set()
extra = {"func_80022874": "u8 func_80022874(struct MacroState_80022874 *state, MacroCommand *command)"}
for _, fn, _ in cases:
    if fn in seen: continue
    seen.add(fn)
    m = re.search(r"^u8 %s\(([^)]*)\)" % fn, tu, re.M)
    sig = ("u8 %s(%s)" % (fn, m.group(1))) if m else extra[fn]
    params = [p.strip() for p in re.search(r"\((.*)\)", sig).group(1).split(",")]
    types = []
    for p in params:
        t = re.sub(r"\s*\b\w+$", "", p).strip()
        if t.endswith("*"): t = t[:-1].strip() + " *"
        mm = re.match(r"(?:struct )?(\w+) \*", t)
        if mm and mm.group(1) not in ("MacroState", "MacroCommand"):
            tags.append(mm.group(1)); t = "struct %s *" % mm.group(1)
        types.append(t)
    protos.append((fn, types))
out = []
for t in dict.fromkeys(tags): out.append("struct %s;" % t)
for fn, types in protos: out.append("extern u8 %s(%s);" % (fn, ", ".join(types)))
decl = "\n".join(out)
sw = []
for op, fn, k in cases:
    types = dict(protos)[fn]
    a0 = "(%s)voice" % types[0]
    a1 = "&cstep" if types[1] == "MacroCommand *" else "(%s)&cstep" % types[1]
    args = a0 + ", " + a1 + ((", " + k) if k else "")
    sw.append("        case %s:\n            ex = %s(%s);\n            break;" % (op, fn, args))
body = open(sys.argv[1]).read().replace("@DECLS@", decl).replace("@SWITCH@", "\n".join(sw))
open(sys.argv[2], "w").write(body)
