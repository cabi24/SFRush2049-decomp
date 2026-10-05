"""Duplication census across matched game sources (src/blob/*.c, src/blob/groups/*/*.c):
struct typedefs and conflicting D_XXXXXXXX declarations."""
import re, json, collections, hashlib
from common import *
files = sorted((REPO / "src/blob").glob("*.c")) + sorted((REPO / "src/blob/groups").glob("*/*.c"))
singles = [p for p in files if p.parent.name == "blob"]; groups = [p for p in files if p.parent.name != "blob"]
print("files:", len(files), "single", len(singles), "group", len(groups),
      "bytes single", sum(p.stat().st_size for p in singles), "group", sum(p.stat().st_size for p in groups))
def strip(t):
    t = re.sub(r"/\*.*?\*/", " ", t, flags=re.S); return re.sub(r"//[^\n]*", " ", t)
def norm(s): return re.sub(r"\s+", " ", s).strip()
def typedef_structs(t):
    out = []
    for m in re.finditer(r"\btypedef\s+(struct|union)\b[^;{]*\{", t):
        i = m.end(); d = 1
        while i < len(t) and d:
            d += (t[i] == "{") - (t[i] == "}"); i += 1
        j = t.find(";", i)
        if j < 0: continue
        name = norm(t[i:j]); body = norm(t[m.end():i - 1])
        out.append((name, body))
    return out
def top_split(s):
    parts, d, cur = [], 0, ""
    for ch in s:
        if ch in "([{": d += 1
        if ch in ")]}": d -= 1
        if ch == "," and d == 0: parts.append(cur); cur = ""
        else: cur += ch
    parts.append(cur); return parts
DN = re.compile(r"\bD_([0-9A-Fa-f]{8})\b")
def d_decls(t):
    out = []
    for m in re.finditer(r"\bextern\s+([^;{}]*?);", t):
        stmt = norm(m.group(1))
        if not DN.search(stmt): continue
        parts = top_split(stmt)
        m0 = re.match(r"^((?:const\s+|volatile\s+|unsigned\s+|signed\s+|struct\s+|union\s+|enum\s+)*[A-Za-z_]\w*)\s*(.*)$", parts[0])
        if not m0: continue
        base = m0.group(1); decls = [m0.group(2)] + parts[1:]
        for dcl in decls:
            n = DN.search(dcl)
            if n: out.append((int(n.group(1), 16), norm(base + " " + DN.sub("$", dcl.strip()))))
    return out
for label, fl in (("single-file sources", singles), ("group sources", groups), ("all", files)):
    occ = 0; names = collections.defaultdict(set); bodies = set(); where = collections.defaultdict(set)
    for p in fl:
        for name, body in typedef_structs(strip(p.read_text(errors="replace"))):
            occ += 1; h = hashlib.sha1(body.encode()).hexdigest()[:12]
            names[name].add(h); bodies.add(h); where[name].add(str(p.relative_to(REPO)))
    conf = {n: b for n, b in names.items() if len(b) > 1}
    print(f"[{label}] struct/union typedef occurrences {occ}; distinct names {len(names)}; distinct bodies {len(bodies)}; "
          f"distinct (name,body) {sum(len(b) for b in names.values())}; names with >1 body {len(conf)}")
    if label == "single-file sources":
        top = sorted(where.items(), key=lambda kv: -len(kv[1]))[:12]
        print("   most repeated names:", ", ".join(f"{n}x{len(w)}({len(names[n])} bodies)" for n, w in top))
        one = sum(1 for n, w in where.items() if len(w) == 1)
        print(f"   names used by exactly one file: {one}; by >=2 files: {len(where) - one}")
        anon = sum(1 for n in names if re.match(r"^(S|T|Struct|Unk|struct_|Obj|Entry|Item|Node|Rec|Data)\w{0,3}$", n))
        print("   generic-looking names (S, T, Entry, Node...):", anon)
decl = collections.defaultdict(lambda: collections.defaultdict(set))
for p in files:
    for a, ty in d_decls(strip(p.read_text(errors="replace"))):
        decl[a][ty].add(str(p.relative_to(REPO)))
multi = {a: t for a, t in decl.items() if sum(len(f) for f in t.values()) >= 2}
conf = {a: t for a, t in multi.items() if len(t) > 1}
print(f"D_ addresses declared (extern) anywhere: {len(decl)}; declared in >=2 files: {len(multi)}; with conflicting declared types: {len(conf)}")
def klass(ty):
    if "[" in ty: return "array"
    if "*" in ty: return "pointer"
    return ty.split(" $")[0]
hard = {a: t for a, t in conf.items() if len({klass(x) for x in t}) > 1}
print(f"   conflicts that change kind/base type (not just qualifiers/array bound spelling): {len(hard)}")
worst = sorted(conf.items(), key=lambda kv: -len(kv[1]))[:15]
for a, t in worst:
    print(f"   D_{a:08X}: {len(t)} spellings in {sum(len(f) for f in t.values())} files: " + " | ".join(f"{ty} x{len(f)}" for ty, f in sorted(t.items(), key=lambda kv: -len(kv[1]))[:5]))
where_ = collections.Counter("rodata" if 0x80123870 <= a < END else "data" if DATA_START <= a < END else "bss" if a >= END else "code" if a >= BASE else "static" for a in decl)
print("declared D_ addresses by region:", dict(where_))
# (per-address detail is written to decl_overrides.json below)

# ---- separate the generated prelude (same declaration in >=300 files) from hand-written declarations
print("\n-- prelude vs hand-written --")
sizes = sorted(p.stat().st_size for p in singles)
print("single-file source sizes: median", sizes[len(sizes)//2], " files >50KB (carry the generated context prelude):", sum(1 for s in sizes if s > 50000),
      " files <=50KB:", sum(1 for s in sizes if s <= 50000))
occ = collections.defaultdict(lambda: collections.defaultdict(set))
for p in files:
    for name, body in typedef_structs(strip(p.read_text(errors="replace"))):
        occ[name][hashlib.sha1(body.encode()).hexdigest()[:12]].add(str(p.relative_to(REPO)))
prelude_names = {n for n, b in occ.items() if max(len(f) for f in b.values()) >= 300}
hand = {n: b for n, b in occ.items() if n not in prelude_names}
print(f"struct typedef names from the shared prelude (>=300 files): {len(prelude_names)}; of those with a variant body elsewhere: "
      f"{sum(1 for n in prelude_names if len(occ[n]) > 1)}")
print(f"hand-written struct typedef names: {len(hand)}; distinct bodies {len({h for b in hand.values() for h in b})}; "
      f"names with >1 body: {sum(1 for b in hand.values() if len(b) > 1)}; names appearing in exactly 1 file: "
      f"{sum(1 for b in hand.values() if len(set().union(*b.values())) == 1)}")
# same body under different names (mechanical merge candidates)
body_names = collections.defaultdict(set)
for n, b in hand.items():
    for h in b: body_names[h].add(n)
print("   identical bodies declared under >1 name:", sum(1 for v in body_names.values() if len(v) > 1))
default = {}; overrides = {}
for a, t in decl.items():
    big = [ty for ty, f in t.items() if len(f) >= 300]
    default[a] = big[0] if big else None
    ov = {ty: f for ty, f in t.items() if len(f) < 300}
    if ov: overrides[a] = ov
print(f"D_ addresses with a prelude default declaration: {sum(1 for v in default.values() if v)}; only hand-declared: {sum(1 for v in default.values() if not v)}")
ovd = {a: o for a, o in overrides.items() if default[a]}
print(f"addresses where >=1 file overrides the prelude default with a different type: {len(ovd)}")
hh = {a: o for a, o in overrides.items() if len(o) > 1}
print(f"addresses with >=2 mutually different hand-written types: {len(hh)}")
hand_only = {a: o for a, o in overrides.items() if not default[a]}
print(f"hand-only addresses: {len(hand_only)}, of which conflicting: {sum(1 for o in hand_only.values() if len(o) > 1)}")
structy = {a for a, o in overrides.items() if any(re.match(r"^(?!s8|u8|s16|u16|s32|u32|f32|f64|char|int|short|long|unsigned|signed|void|volatile|const|float|double)\w+ ", ty) for ty in o)}
print(f"addresses given a struct-typed declaration by at least one hand source: {len(structy)}")
json.dump({f"{a:08X}": {"default": default[a], "hand": {ty: sorted(f) for ty, f in o.items()}} for a, o in overrides.items()},
          open(OUT / "decl_overrides.json", "w"), indent=0)
