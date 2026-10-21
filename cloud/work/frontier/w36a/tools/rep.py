#!/usr/bin/env python3
"""rep.py BASE_GROUP_DIR OUT_DIR FN=SRC.c [FN=SRC.c ...] [--sub OLD@@NEW ...]
Replace the definition of each FN in BASE/group.c with the text of SRC.c (the whole file text replaces from the
line 'void FN(' up to the closing '\\n}\\n'; any extern lines in SRC before the definition are kept, i.e. the SRC text
is inserted verbatim in place of the old definition).  --sub does literal replacements on the result.
Promotes the replaced FNs to members (for scoring) unless --ctx is given."""
import sys, json, os, re
args = sys.argv[1:]
base, out = args[0], args[1]
reps, subs, ctx = [], [], False
i = 2
while i < len(args):
    a = args[i]
    if a == '--sub': subs.append(args[i+1].split('@@', 1)); i += 2; continue
    if a == '--ctx': ctx = True; i += 1; continue
    fn, src = a.split('=', 1); reps.append((fn, src)); i += 1
b = open(os.path.join(base, 'group.c')).read()
for fn, src in reps:
    s = open(src).read()
    m = re.search(r'\n(?:static )?void ' + fn + r'\([^)]*\)\n\{', b)
    st = m.start() + 1
    en = b.index('\n}\n', st) + 3
    b = b[:st] + s + ('' if s.endswith('\n') else '\n') + b[en:]
for o, n in subs:
    if o not in b: sys.exit('sub not found: ' + o)
    b = b.replace(o, n)
os.makedirs(out, exist_ok=True)
open(os.path.join(out, 'group.c'), 'w').write(b)
j = json.load(open(os.path.join(base, 'group.json')))
if not ctx:
    for fn, _ in reps:
        if fn not in j['members']: j['members'].append(fn)
        j['context'] = [c for c in j['context'] if c != fn]
json.dump(j, open(os.path.join(out, 'group.json'), 'w'), indent=1)
