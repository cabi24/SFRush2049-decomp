"""List unmatched callers of each provisional.json entry and rank callers by
provisional bytes they would unblock (direct + via provisional chains).
Replays against the current tree (layout, lock, provisional.json)."""
import json, sys
from pathlib import Path
sys.path.insert(0, '.')
from tools.conveyor.pipeline import frontier as F
doc = json.loads(Path(F.LAYOUT_JSON).read_text())
funcs = F.functions(doc)
ip = Path(doc["image"]["path"]); ip = ip if ip.is_absolute() else F.REPO / ip
calls, ext = F.call_graph(funcs, ip.read_bytes(), int(doc["image"]["base"], 16))
size = {n: s for _, s, n in funcs}
matched = set(json.loads(Path(F.LOCKFILE).read_text())) & set(size)
prov = {n for n in F.load_provisional() if n in size and n not in matched}
callers = {}
for n, cs in calls.items():
    for c in cs:
        callers.setdefault(c, set()).add(n)
gain = {}
for p in sorted(prov, key=lambda x: -size[x]):
    cs = sorted(c for c in callers.get(p, ()) if c not in matched)
    print(f"{p:28s} {size[p]:5d}  unmatched callers: " +
          ", ".join(f"{c}{'(P)' if c in prov else ''}[{size[c]}]" for c in cs))
# closure: matching set S of non-provisional callers closes P when every
# unmatched caller of P is in S or is a provisional that closes.
def closes(p, S, seen=()):
    if p in seen: return True
    for c in callers.get(p, ()):
        if c in matched or c in S: continue
        if c in prov and closes(c, S, seen + (p,)): continue
        return False
    return True
nonprov = sorted({c for p in prov for c in callers.get(p, ())} - matched - prov)
# also callers of provisional callers
print()
print("single real caller -> provisional bytes it alone closes:")
rows = []
for c in nonprov:
    got = [p for p in prov if closes(p, {c})]
    rows.append((sum(size[p] for p in got), c, got))
for b, c, got in sorted(rows, reverse=True):
    print(f"  {c:28s} [{size[c]:5d} B] closes {b:5d} B: {', '.join(got) or '-'}")
print()
print("all non-provisional unmatched callers reachable:")
for c in nonprov:
    print(f"  {c} [{size[c]}] -> prov callees: {sorted(calls[c] & prov)}; its unmatched callees: {sorted(x for x in calls[c] - matched - prov)}")
