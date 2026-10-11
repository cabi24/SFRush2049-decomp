#!/usr/bin/env python3
"""w17g static inventory (Pi, read-only): conveyor n64_target population=static minus matched.lock.json
target_ids, joined with build/layout.us.json (layout derive) for size/segment/converted/flagset, call graph from
the static asm (asm/us/*.s + asm/us/nonmatchings/rom/**.s, jal targets), prior attempts by grep."""
import sqlite3, json, os, re, glob, subprocess, sys
R = os.getcwd()
c = sqlite3.connect('file:' + os.path.expanduser('~/.conveyor/conveyor.db') + '?mode=ro', uri=True)
L = json.load(open('matched.lock.json'))
tids = {v.get('target_id') for v in L.values()}
rows = c.execute("select target_id,address,insn_count,tier from n64_target where population='static'").fetchall()
un = {r[0]: r for r in rows if r[0] not in tids}
lay = json.load(open('build/layout.us.json'))
fnseg = {}
for s in lay['segments']:
    for f in s['functions']:
        fnseg[f['name']] = (s, f)
# symbol map address->name
sym = {}
for l in open('symbol_addrs.us.txt'):
    m = re.match(r'\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)', l)
    if m: sym[int(m.group(2), 16)] = m.group(1)
# call graph: per function, jal targets; parse glabel blocks
calls = {}
files = glob.glob('asm/us/*.s') + glob.glob('asm/us/nonmatchings/rom/**/*.s', recursive=True)
for fp in files:
    cur = None
    for l in open(fp, errors='replace'):
        m = re.match(r'\s*glabel\s+(\w+)', l)
        if m: cur = m.group(1); calls.setdefault(cur, set()); continue
        m = re.search(r'\bjal\s+(\w+)', l)
        if m and cur: calls[cur].add(m.group(1))
callers = {}
for f, cs in calls.items():
    for t in cs: callers.setdefault(t, set()).add(f)
locked_fns = {k.split(':')[1] for k in L}
# locked C callers in src/rom
out = []
for name, r in un.items():
    s, f = fnseg.get(name, (None, None))
    size = f['size'] if f else r[2] * 4
    out.append(dict(name=name, addr=hex(r[1]), size=size, tier=r[3], seg=s['yaml_name'] if s else None,
                    converted=s['converted'] if s else None, flagset=s['flagset'] if s else None,
                    state=f['state'] if f else None,
                    callees=sorted(calls.get(name, ())), callers=sorted(callers.get(name, ())),
                    locked_callees=sorted(x for x in calls.get(name, ()) if x in locked_fns),
                    locked_callers=sorted(x for x in callers.get(name, ()) if x in locked_fns)))
out.sort(key=lambda d: d['size'])
import csv
st = {r['name']: r for r in csv.DictReader(open('cloud/work/boot_tail/STATUS.csv'))}
names = {d['name'] for d in out}
hits = {}
for fp in glob.glob('cloud/work/frontier/*/RESULTS.md') + glob.glob('cloud/work/boot_tail*/**/README.md', recursive=True):
    if '/w17g/' in fp: continue
    t = open(fp, errors='replace').read()
    for n in set(re.findall(r'\b\w+\b', t)) & names:
        hits.setdefault(n, []).append(fp)
for d in out:
    s = st.get(d['name'])
    d['boot_tail'] = dict(status=s['status'], diff_words=s['diff_words'], source=s['source_path'],
                          hypothesis=s['name_hypothesis']) if s else None
    d['prior_md'] = sorted(hits.get(d['name'], []))[:6]
json.dump(out, open(sys.argv[1], 'w'), indent=0)
fn = [d for d in out if d['name'].startswith('func_')]
print(len(out), sum(d['size'] for d in out), 'func_', len(fn), sum(d['size'] for d in fn))
