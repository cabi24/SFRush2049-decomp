#!/usr/bin/env python3
"""mkg.py OUTDIR [NAME=file.c ...] [--claims a,b] [--context a,b] [--keep a,b]
Compose a superseding dot_runtime_a_text_helpers_20261006 group: the installed group.c with the named
function definitions (signature line through the column-0 closing brace) replaced by the given files.
Writes OUTDIR/group.c + OUTDIR/group.json (members = old members + claims)."""
import json, re, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[5]
BASE = ROOT / 'src/ovl_a/groups/dot_runtime_a_text_helpers_20261006'
args = sys.argv[1:]
out = Path(args.pop(0)); out.mkdir(parents=True, exist_ok=True)
opts = {'--claims': [], '--context': None, '--keep': None, '--extra': []}
reps = []
while args:
    a = args.pop(0)
    if a in opts:
        opts[a] = [x for x in args.pop(0).split(',') if x]
    else:
        reps.append(a.split('=', 1))
text = (BASE / 'group.c').read_text()
for name, f in reps:
    body = Path(f).read_text().rstrip('\n') + '\n'
    m = re.search(r'^[A-Za-z_][^\n;]*\b' + name + r'\([^\n]*\)\s*\n\{', text, re.M)
    assert m, name
    end = text.index('\n}\n', m.end()) + 3
    text = text[:m.start()] + body + text[end:]
(out / 'group.c').write_text(text)
spec = json.loads((BASE / 'group.json').read_text())
members = list(spec['members'])
for c in opts['--claims'] + opts['--extra']:
    if c not in members:
        members.append(c)
spec['members'] = members
if opts['--context'] is not None:
    spec['context'] = opts['--context']
else:
    spec['context'] = [c for c in spec['context'] if c not in members]
if opts['--keep'] is not None:
    spec['keep'] = opts['--keep']
spec['claims'] = opts['--claims']
(out / 'group.json').write_text(json.dumps(spec, indent=1) + '\n')
