#!/usr/bin/env python3
"""List function definitions (in order) in each overlay group's sources."""
import json, re, sys, glob, os
pat = re.compile(r'^(static\s+)?(?:inline\s+)?([A-Za-z_][\w\s\*]*?)\b([A-Za-z_]\w*)\s*\(([^;{]*)\)\s*\{?\s*$')
def defs(path):
    out = []
    lines = open(path, errors='replace').read().split('\n')
    for i, l in enumerate(lines):
        if l.startswith((' ', '\t', '#', '/', '*', '}')) or not l.strip():
            continue
        m = pat.match(l)
        if not m: continue
        nxt = lines[i+1].strip() if i+1 < len(lines) else ''
        if not l.rstrip().endswith('{') and not nxt.startswith('{'):
            continue
        name = m.group(3)
        if name in ('if', 'while', 'for', 'switch'): continue
        out.append((i+1, 'static' if m.group(1) else 'global', name))
    return out
roots = sys.argv[1:] or sorted(glob.glob('src/ovl_a/groups/*') + glob.glob('src/ovl_b/groups/*'))
for g in roots:
    spec = json.load(open(os.path.join(g, 'group.json')))
    mem, ctx, keep = set(spec.get('members', [])), set(spec.get('context', [])), set(spec.get('keep', []))
    print('==', g, ' (keep', ' '.join(sorted(keep)) + ')')
    for f in spec['files']:
        for ln, st, name in defs(os.path.join(g, f)):
            role = 'M' if name in mem else ('C' if name in ctx else '-')
            k = 'K' if name in keep else ''
            print(f'   {f}:{ln:<5} {st:6} {role}{k:1} {name}')
