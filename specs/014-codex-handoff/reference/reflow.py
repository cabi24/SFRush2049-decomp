import itertools, random, re, sys
from pathlib import Path
fn = sys.argv[1]; src = Path(sys.argv[2]).read_text(); out = Path(sys.argv[3]); out.mkdir(parents=True, exist_ok=True)
n = int(sys.argv[4]) if len(sys.argv) > 4 else 1500
m = re.search(r'^[^\n]*\b' + fn + r'\s*\([^)]*\)\s*\{', src, re.M)
start = m.start(); depth = 0; i = m.end() - 1
while True:
    if src[i] == '{': depth += 1
    elif src[i] == '}':
        depth -= 1
        if depth == 0: break
    i += 1
body = src[start:i + 1]
# tokens: keep identifiers/numbers/strings/punct; breakpoints after { } ; and between ) and {
toks = re.findall(r'"(?:\\.|[^"\\])*"|\w+|->|==|!=|<=|>=|&&|\|\||<<|>>|\+\+|--|[^\s\w]', body)
def join(tokens, newline_after):
    out = []
    for k, t in enumerate(tokens):
        out.append(t)
        out.append('\n' if k in newline_after else ' ')
    return ''.join(out)
brk = [k for k, t in enumerate(toks[:-1]) if t in ('{', '}', ';') or (t == ')' and toks[k + 1] == '{')]
print('tokens', len(toks), 'break points', len(brk), file=sys.stderr)
orig_nl = set(brk)
variants = {frozenset(orig_nl)}
variants.add(frozenset())                                  # everything on one line
for k in brk:                                              # single flips
    variants.add(frozenset(orig_nl ^ {k}))
for a, b in itertools.combinations(brk, 2):               # pairs
    variants.add(frozenset(orig_nl ^ {a, b}))
if len(brk) <= 12:
    for r in range(len(brk) + 1):
        for c in itertools.combinations(brk, r):
            variants.add(frozenset(c))
rng = random.Random(1)
while len(variants) < min(n, 2 ** len(brk)):
    variants.add(frozenset(k for k in brk if rng.random() < 0.5))
for idx, v in enumerate(sorted(variants, key=lambda s: (len(s), sorted(s)))[:n]):
    text = src[:start] + join(toks, set(v)).rstrip() + src[i + 1:]
    (out / f'v{idx:05d}.c').write_text(text)
print(len(list(out.glob('v*.c'))), 'variants', file=sys.stderr)
