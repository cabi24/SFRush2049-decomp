#!/usr/bin/env python3
"""Set a packet's group.json: add NEW members (stubs/helpers), claims = all members.
Usage: pkt.py PACKET_DIR NEW_MEMBER... [--note TEXT]"""
import json, sys
from pathlib import Path
args = sys.argv[1:]
note = None
if '--note' in args:
    i = args.index('--note'); note = args[i + 1]; del args[i:i + 2]
d = Path(args[0]); new = args[1:]
spec = json.loads((d / 'group.json').read_text())
for n in new:
    if n in spec.get('context', []):
        spec['context'].remove(n)
    if n not in spec['members']:
        spec['members'].append(n)
spec['claims'] = list(spec['members'])
if note:
    spec['provenance'] = (spec.get('provenance', '') + ' ' + note).strip()
order = ['name', 'members', 'claims', 'context', 'files', 'keep', 'flags', 'allow_unverified', 'provenance']
out = {k: spec[k] for k in order if k in spec}
out.update({k: v for k, v in spec.items() if k not in out})
(d / 'group.json').write_text(json.dumps(out, indent=1) + '\n')
print('members', len(out['members']), 'claims', len(out['claims']))
