"""Run inside gdb:  gdb -batch -x arcade_layout_gdb.py layout.o
layout.o = arcade game/*.h compiled with mips-linux-gnu-gcc -mabi=32 -g, one
global `T x_T;` per struct typedef.  Writes arcade_layouts.json: leaves per type."""
import gdb, json, os
def leaves(t, base, path, out):
    t = t.strip_typedefs()
    if t.code == gdb.TYPE_CODE_STRUCT or t.code == gdb.TYPE_CODE_UNION:
        for f in t.fields():
            if f.bitsize: 
                out.append((base + f.bitpos // 8, 0, "bitfield", path + "." + (f.name or "?"))); continue
            leaves(f.type, base + f.bitpos // 8, path + "." + (f.name or "?"), out)
    elif t.code == gdb.TYPE_CODE_ARRAY:
        lo, hi = t.range(); el = t.target()
        for i in range(lo, hi + 1):
            leaves(el, base + i * el.sizeof, f"{path}[{i}]", out)
    else:
        k = {gdb.TYPE_CODE_FLT: "f", gdb.TYPE_CODE_PTR: "p"}.get(t.code)
        if k is None:
            k = ("u" if getattr(t, "is_signed", None) is False or "unsigned" in str(t) else "s")
        out.append((base, t.sizeof, k + str(t.sizeof), path))
res = {}
for sym in gdb.execute("info variables ^x_", to_string=True).split():
    name = sym.rstrip(";")
    if not name.startswith("x_"): continue
    try:
        t = gdb.lookup_global_symbol(name).type
    except Exception: continue
    out = []; leaves(t, 0, "", out)
    res[name[2:]] = dict(size=t.sizeof, leaves=out)
json.dump(res, open(os.environ["ARC_OUT"], "w"))
print(len(res), "types")
