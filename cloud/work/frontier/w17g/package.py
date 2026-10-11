#!/usr/bin/env python3
"""w17g: package the D10 boot-tail "nonmatch" sources that are exact under IDO 7.1 (bt71.log) into
cloud/work/frontier/w17g/sources/NAME.c. Body and declarations are byte-for-byte the D10 file; only the leading
comment lines are replaced by a flags line + provenance header (semantics = the D10 ledger's name hypothesis)."""
import csv, json, re, sys
from pathlib import Path
R = Path.cwd(); W = R / 'cloud/work/frontier/w17g'
st = {r['name']: r for r in csv.DictReader(open(R / 'cloud/work/boot_tail/STATUS.csv'))}
lay = json.load(open(R / 'build/layout.us.json'))
seg = {f['name']: s for s in lay['segments'] for f in s['functions']}
size = {f['name']: f['size'] for s in lay['segments'] for f in s['functions']}
for fn in (W / 'match71.txt').read_text().split():
    r = st[fn]; src = R / r['source_path']; text = src.read_text()
    lines = text.splitlines(True)
    while lines and (lines[0].startswith('/*') and lines[0].rstrip().endswith('*/')):
        lines.pop(0)            # drop old single-line flag / status comments only
    s = seg[fn]
    shaping = []
    if 'volatile' in text: shaping.append('`volatile` on a global (as in the D10 source; busy/handshake flag)')
    hdr = (f"/* flags: -g0 -O2 -mips2 -G 0 -non_shared */\n"
           f"/* {fn} (static, boot {r['address']}, {size[fn]} B, segment {s['yaml_name']} -> src/{s['rom_tu']}.c, "
           f"{'converted' if s['converted'] else 'NOT converted'})\n"
           f" * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0\n"
           f" * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores {r['diff_words']} word(s) off, which\n"
           f" * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md \"IDO 7.1\".\n"
           f" * Semantics (D10 ledger name hypothesis, unchanged): {r['name_hypothesis']}.\n"
           f" * Body + declarations verbatim from {r['source_path']} ({r['owner']}).\n"
           f" * Shaping devices: {'; '.join(shaping) if shaping else 'none'}. */\n")
    (W / 'sources' / (fn + '.c')).write_text(hdr + ''.join(lines))
    print(fn, s['yaml_name'], s['converted'], size[fn])
