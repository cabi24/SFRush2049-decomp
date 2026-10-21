#!/usr/bin/env python3
"""mkg.py BASEDIR OUTDIR [NAME=file.c ...] [--json-add-member X] : replace function definitions in BASEDIR/group.c.
A replacement file may contain declarations before the function; they are inserted in place of the old definition."""
import json, re, sys, shutil
from pathlib import Path
base = Path(sys.argv[1]); out = Path(sys.argv[2]); out.mkdir(parents=True, exist_ok=True)
text = (base / 'group.c').read_text()
for spec in sys.argv[3:]:
    name, f = spec.split('=')
    body = Path(f).read_text()
    m = re.search(r'^[A-Za-z_][^\n;]*\b' + name + r'\([^\n;]*\)\s*\n\{', text, re.M)
    assert m, name
    end = text.index('\n}\n', m.end()) + 3
    text = text[:m.start()] + body.rstrip('\n') + '\n' + text[end:]
(out / 'group.c').write_text(text)
shutil.copy(base / 'group.json', out / 'group.json')
