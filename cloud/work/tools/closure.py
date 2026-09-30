#!/usr/bin/env python3
"""closure.py [GROUP...]: approximate IPA closure of each group in
cloud/work/ipa-groups (all groups if none named).

A function "receives registers" if it reads a non-ABI register before writing
it (linear scan of its retail words; approximate). Starting from a group's
members and context, it adds every caller of such a function and every
callee that is one, repeatedly, then prints the functions the group is
missing. Those are the closure gaps listed in CloudHandoffV2.md. Needs
binutils-mips-linux-gnu. Cloud Lane A helper; expect false positives.
"""
import json, re, struct, subprocess, sys, tempfile, glob, os
from pathlib import Path
ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'tools' / 'cloud')); import score
syms = {k: int(v, 16) for k, v in json.load(open(ROOT / 'asm/us/blob/symbols.json'))['symbols'].items()}
T = score.targets(); addr = {n: syms[n] for n in T if n in syms}; byaddr = {a: n for n, a in addr.items()}
ABI = {'a0','a1','a2','a3','sp','ra','zero','$f12','$f14','gp'}
REG = r'(\$f\d+|\b(?:[astvk][0-9]|s8|fp|at|v[01]|zero|sp|ra|gp)\b)'
def dis(words, va):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack(f'>{len(words)}I', *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump','-D','-b','binary','-m','mips:4300','-EB',f'--adjust-vma=0x{va:x}',f.name],capture_output=True,text=True).stdout
    return [re.sub(r'\s+',' ',m.group(3)) for m in (re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', l) for l in out.splitlines()) if m]
def livein(name):
    ins = dis(T[name], addr[name]); written=set(); live=[]
    for i in ins:
        op,_,rest = i.partition(' '); regs = re.findall(REG, rest)
        if not regs: continue
        if op in ('sw','sdc1','swc1','sd') and rest.endswith('(sp)') and re.match(r'(s[0-8]|fp|ra|\$f(2[0-9]|3[01]))\b', regs[0]): continue
        if op.startswith(('sw','sh','sb','sd','swc1','sdc1','b','j','mt','ctc1')) or op in ('jr','jalr'):
            srcs, dst = regs, None
            if op.startswith('mtc1'): srcs, dst = regs[:1], regs[1]
        else:
            dst, srcs = regs[0], regs[1:]
            if op in ('lui','li','mflo','mfhi'): srcs=[]
        for r in srcs:
            if r not in written and r not in ABI and r not in live and r not in ('at','v0','v1'): live.append(r)
        if dst: written.add(dst)
        if op == 'jal': written.update({'v0','v1','a0','a1','a2','a3','at','t0','t1','t2','t3','t4','t5','t6','t7','t8','t9'})
    return live
calls = {n: [byaddr.get(0x80000000 | ((w & 0x3FFFFFF) << 2)) for w in ws if w >> 26 == 3] for n, ws in T.items()}
callers = {}
for n, cs in calls.items():
    for c in cs:
        if c: callers.setdefault(c, set()).add(n)
LI = {}
def ipa(n):
    if n not in LI: LI[n] = livein(n) if n in T and n in addr else []
    return LI[n]
for g in sorted(glob.glob(str(ROOT / 'cloud/work/ipa-groups/*/group.json'))):
    name = os.path.basename(os.path.dirname(g))
    if name in sys.argv[1:] or not sys.argv[1:]:
        j = json.load(open(g)); S = set(j['members'] + j.get('context', []))
        work = list(S)
        while work:
            n = work.pop()
            if ipa(n):   # n receives registers: all its callers belong
                for c in callers.get(n, ()):
                    if c not in S: S.add(c); work.append(c)
            for c in calls.get(n, []):
                if c and ipa(c) and c not in S: S.add(c); work.append(c)
        extra = sorted(S - set(j['members'] + j.get('context', [])))
        print(f'{name}: {len(S)} fns {sum(len(T[x]) for x in S if x in T)}w; missing {extra}')
