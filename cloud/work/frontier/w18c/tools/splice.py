#!/usr/bin/env python3
"""w18c (copy of lane/tools/splice.py, sources path changed): promote_batch.splice() replica into a SCRATCH copy of src/rom (never the main tree).
    python3 cloud/work/frontier/lane/tools/splice.py CTX.jsonl OUTDIR fn [fn...]
For each fn: its passthrough slot in src/rom/<tu>.c is replaced by provenance header + the ctx row's preamble
statements not already present in the TU (preprocessor lines kept, #include deduped against text before the
slot) + the body extracted from lane/sources/fn.c. Writes OUTDIR/<tu>.c for touched TUs."""
import json, sys
from pathlib import Path
sys.path.insert(0, str(Path.cwd()))
from tools.conveyor.seeds.extract_candidates import extract_named_function  # noqa: E402
ctx = {r['function']: r for r in map(json.loads, open(sys.argv[1]))}
out = Path(sys.argv[2]); out.mkdir(parents=True, exist_ok=True)
norm = lambda s: ' '.join(s.split())
stm = lambda t: {norm(l) for l in t.splitlines() if l.strip()}
texts = {}
for fn in sys.argv[3:]:
    tus = [p for p in Path(__import__('os').environ.get('TUSRC', 'src/rom')).glob('*.c') if f'/{fn}.s")' in p.read_text()]
    assert len(tus) == 1, fn
    tu = tus[0]; text = texts.get(tu) or tu.read_text()
    pragma = f'#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/{tu.stem}/{fn}.s")'
    assert text.count(pragma) == 1, fn
    row = ctx[fn]; assert row['status'] == 'ok', fn
    before = stm(text[:text.index(pragma)]); have = before  # prefix dedup (promote_batch --prefix-dedup)
    decls = [s for s in row['preamble'] if (s.startswith('#') and not (s.startswith('#include') and norm(s) in before))
             or (not s.startswith('#') and norm(s) not in have)]
    body = extract_named_function(Path(f'lane/sources/{fn}.c'), fn)
    hdr = (f"/* PROMOTED (w18c dry run) — {fn}\n * Source:   lane/sources/{fn}.c\n"
           f" * Flags:    -g0 -O2 -mips2 -G 0 -non_shared (IDO 7.1)\n */\n")
    texts[tu] = text.replace(pragma, hdr + ''.join(d + '\n' for d in decls) + body.rstrip() + '\n', 1)
for tu, t in texts.items():
    (out / tu.name).write_text(t); print('wrote', out / tu.name)
