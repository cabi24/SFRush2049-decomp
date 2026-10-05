#!/usr/bin/env python3
"""Parse one procedure's section of a uopt listing (-l FILE -zdbug:3, printcm) into
expression bits and per-node PRE vectors.  Library + CLI:

    cmparse.py LISTING PROC [--bits 80,81] [--w5d W5DTRACE]   # per-bit summary
"""
import re, sys, argparse

VEC = ('antlocs', 'avlocs', 'alters', 'absalters', 'antin', 'antout', 'iv', 'cand',
       'ppin', 'ppout', 'insert', 'delete', 'subdelete', 'subinsert')


def section(text, proc):
    """Lines from 'LOCAL OPTIMIZATION OF proc' up to 'REEMISSION OF proc'."""
    out, on = [], False
    for line in text.splitlines():
        if not on and re.search(r'SECONDS IN LOCAL OPTIMIZATION OF %s$' % re.escape(proc), line):
            on = True
        if on:
            out.append(line)
            if re.search(r'SECONDS IN REEMISSION OF %s$' % re.escape(proc), line):
                break
    return out


def bitset(s):
    s = s.strip().strip('[]')
    r = set()
    for part in s.split(','):
        part = part.strip()
        if not part:
            continue
        if '..' in part:
            a, b = part.split('..')
            r.update(range(int(a), int(b) + 1))
        else:
            r.add(int(part))
    return r


def parse(lines):
    """-> (table {bit: (ref, kind, rest)}, nodes {node: {vec: set}}, sets {name: set})"""
    table, nodes, sets = {}, {}, {}
    node = None
    tab_re = re.compile(r'^\{\s*(\d+)\|(\d+)\}\s+(\d+)\s+(\w+)\s*(.*)$')
    cont_re = re.compile(r'^"""\s+(\d+)')
    for line in lines:
        m = tab_re.match(line)
        if m:
            table[int(m.group(3))] = ('%s|%s' % (m.group(1), m.group(2)), m.group(4), m.group(5).strip())
            continue
        m = cont_re.match(line)
        if m:
            table[int(m.group(1))] = ('', 'ivar2', '(second bit of the variable above)')
            continue
        m = re.match(r'^! ! ! ! ! node\s+(\d+)', line)
        if m:
            node = int(m.group(1)); nodes[node] = {}
            continue
        m = re.match(r'^(\w+) --\(\s*\d+\) (\[.*\])', line)
        if m and node is not None:
            nodes[node][m.group(1)] = bitset(m.group(2))
            continue
        m = re.match(r'^(@ [\w ]+?)\s*\(\s*\d+\) (\[.*\])', line) or re.match(r'^(\w+) \*\*\*\*\(\s*\d+\) (\[.*\])', line)
        if m:
            sets[m.group(1).strip()] = bitset(m.group(2))
    return table, nodes, sets


def flowgraph(lines):
    """node -> (successors, loopdepth-ish label) from the 'flow graph for' block."""
    succ, cur = {}, None
    on = False
    for line in lines:
        if 'flow graph for' in line:
            on = True; continue
        if not on:
            continue
        if line.startswith(' * *') or line.lstrip().startswith('index'):
            break
        m = re.match(r'^\s+(\d+)\s+(\d+)$', line)
        if m:
            cur = int(m.group(1)); succ.setdefault(cur, [])
            continue
        m = re.match(r'^suc::::\s+(\d+)', line)
        if m and cur is not None:
            succ[cur].append(int(m.group(1)))
    return succ


def summary(table, nodes, sets, bits=None, names=None):
    out = []
    for b in sorted(table):
        if bits and b not in bits:
            continue
        ref, kind, rest = table[b]
        txt = names.get(b, '') if names else ''
        row = ['%4d %-8s %-6s %-34s %s' % (b, ref, kind, rest[:34], txt)]
        for vec in ('antlocs', 'avlocs', 'insert', 'delete', 'subinsert', 'subdelete', 'alters'):
            ns = [n for n in sorted(nodes) if b in nodes[n].get(vec, ())]
            if ns:
                row.append('     %-9s %s' % (vec, ' '.join(map(str, ns))))
        flags = [k for k, v in sets.items() if b in v]
        if flags:
            row.append('     sets      %s' % ', '.join(flags))
        out.append('\n'.join(row))
    return '\n'.join(out)


if __name__ == '__main__':
    ap = argparse.ArgumentParser()
    ap.add_argument('listing'); ap.add_argument('proc')
    ap.add_argument('--bits', default='')
    a = ap.parse_args()
    lines = section(open(a.listing, errors='replace').read(), a.proc)
    t, n, s = parse(lines)
    bits = set(int(x) for x in a.bits.split(',') if x) if a.bits else None
    print(summary(t, n, s, bits))
