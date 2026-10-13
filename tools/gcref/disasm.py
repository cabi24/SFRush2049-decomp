#!/usr/bin/env python3
"""Symbolised capstone disassembly of every linked function in the GameCube ``rush_d.elf``.

The ELF still carries its ``.rela.text``/``.rela.data``/``.rela.rodata``/``.rela.sdata*`` sections, so
every ``bl``/``b`` target, ``lis``/``addi`` (``@ha``/``@l``) pair and r13/r2 small-data access
(``R_PPC_EMB_SDA21``) is resolved from its relocation, i.e. exactly; there is no register tracking
heuristic.  String literals are inlined as comments, float literals (``lfs``/``lfd`` from
``.sdata2``/``.rodata``) are rendered as numbers, switch tables in ``.data`` become labels.

Writes:
  reference/gc/disasm/<source_file>/<function>.s   (ignored bulk dump; --no-dump skips it)
  reference/gc/features.json                       (per-function features for correlate.py)
  docs/external/gc/callgraph.json                  (callers/callees per function)
  docs/external/gc/strings_xref.json               (string -> functions, direct and via tables)

Usage: python3 -m tools.gcref.disasm [--elf rush_d.elf] [--no-dump] [--only NAME]
Needs capstone (Pi: 5.0.6). Mode is CS_MODE_32|CS_MODE_BIG_ENDIAN|CS_MODE_PS: without CS_MODE_PS the
Gekko paired-single opcodes (psq_l/psq_st, ps_*) decode as VSX/altivec garbage.
"""
import argparse
import bisect
import csv
import json
import os
import re
import struct
import sys
from collections import OrderedDict, defaultdict

if __package__ in (None, ''):
    sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..')))
from tools.gcref.elf import Elf, DEFAULT_ELF, REPO, SDA_BASE, SDA2_BASE  # noqa: E402

R_ADDR32, R_ADDR16_LO, R_ADDR16_HI, R_ADDR16_HA, R_REL24, R_REL14, R_SDA21 = 1, 4, 5, 6, 10, 11, 109
DATA_SECTIONS = ('.rodata', '.data', '.sdata', '.sdata2', '.ctors', '.dtors')
LIB_PREFIXES = ('OS', 'GX', 'DVD', 'AI', 'AX', 'DSP', 'CARD', 'EXI', 'VI', 'PAD', 'TRK', 'SI', 'AR', 'DB',
                'MTX', 'PSMTX', 'C_MTX', 'PSVEC', 'C_VEC', 'RAD', 'YUV', 'Bink', 'GD', 'THP', '__')
FLOAT_LOADS = {'lfs': 4, 'lfsu': 4, 'lfsx': 4, 'lfd': 8, 'lfdu': 8, 'psq_l': 4}


def cs_ppc():
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_PPC, capstone.CS_MODE_32 | capstone.CS_MODE_BIG_ENDIAN | capstone.CS_MODE_PS)
    md.skipdata = True
    return md


def sext16(v):
    return v - 0x10000 if v & 0x8000 else v


def fmt_float(x):
    if x != x or x in (float('inf'), float('-inf')):
        return repr(x)
    s = '%.9g' % x
    if 'e' not in s and '.' not in s:
        s += '.0'
    return s


class GcProgram(object):
    """Symbols, relocations and per-function facts of rush_d.elf."""

    def __init__(self, elf_path=DEFAULT_ELF, symbols_csv=None):
        self.elf = Elf(elf_path)
        self.files = self._source_files(symbols_csv or os.path.join(os.path.dirname(os.path.abspath(elf_path)),
                                                                     'rush_d_symbols.csv'))
        syms = self.elf.symbols
        funcs = OrderedDict()
        for s in syms:
            if s.type == 'FUNC' and s.size and s.value and s.value not in funcs:
                funcs[s.value] = s
        names = defaultdict(int)
        for s in funcs.values():
            names[s.name] += 1
        self.funcs = []           # (addr, size, key, name, file); key = name, or name@file for duplicates
        used = set()
        for a in sorted(funcs):
            s = funcs[a]
            sfile = self.file_of(s.name, a, s.file)
            key = s.name if names[s.name] == 1 else '%s@%s' % (s.name, sfile)
            if key in used:
                key = '%s@%08X' % (s.name, a)
            used.add(key)
            self.funcs.append((a, s.size, key, s.name, sfile))
        self.func_addrs = [f[0] for f in self.funcs]
        self.func_by_addr = {f[0]: f for f in self.funcs}
        self.func_by_key = {f[2]: f for f in self.funcs}
        # containment lookup over named, sized OBJECT symbols (plus functions)
        objs = {}
        for s in syms:
            if s.type in ('OBJECT', 'FUNC') and s.value and s.name:
                cur = objs.get(s.value)
                if cur is None or (cur.size == 0 and s.size) or (cur.name.startswith('.') and not s.name.startswith('.')):
                    objs[s.value] = s
        self.obj_addrs = sorted(objs)
        self.objs = [objs[a] for a in self.obj_addrs]
        self.relocs = {}          # word address -> (type, symbol, target)
        self.data_relocs = []     # (address, target, symbol) in data sections
        for sec in self.elf.sections:
            if sec.type != 4 or not sec.name.startswith('.rela.'):
                continue
            tgt_sec = sec.name[5:]
            if tgt_sec in ('.debug', '.line'):
                continue          # writeup.md: never apply .rela.debug
            for i in range(sec.size // 12):
                off, info, add = struct.unpack_from('>IIi', self.elf.data, sec.offset + i * 12)
                sym = syms[info >> 8]
                target = (sym.value + add) & 0xffffffff
                if tgt_sec in ('.text', '.init'):
                    self.relocs[off & ~3] = (info & 255, sym, target, add)
                elif tgt_sec in DATA_SECTIONS and info & 255 == R_ADDR32:
                    self.data_relocs.append((off, target, sym, add))
        self.data_relocs.sort(key=lambda r: r[0])

    # ---- naming --------------------------------------------------------------------------------
    @staticmethod
    def _source_files(path):
        out = {}
        if os.path.exists(path):
            with open(path) as f:
                for r in csv.DictReader(f):
                    if r['type'] in ('FUNC', 'OBJECT') and r['source_file']:
                        out[(r['name'], int(r['address'], 16))] = r['source_file']
        return out

    def file_of(self, name, addr, symtab_file=None):
        f = self.files.get((name, addr)) or symtab_file
        if f:
            return f
        for p in LIB_PREFIXES:
            if name.startswith(p):
                return '_sdk'
        return '_lib'

    def func_at(self, addr):
        i = bisect.bisect_right(self.func_addrs, addr) - 1
        if i >= 0:
            f = self.funcs[i]
            if f[0] <= addr < f[0] + f[1]:
                return f
        return None

    def obj_at(self, addr):
        """(symbol, offset) of the named object/function containing addr (nearest below if unsized)."""
        i = bisect.bisect_right(self.obj_addrs, addr) - 1
        while i >= 0:
            s = self.objs[i]
            if s.value <= addr < s.value + max(s.size, 1):
                return s, addr - s.value
            if s.size:
                # sized symbol below that does not contain addr: try an unsized one further down? no
                return None, None
            i -= 1
        return None, None

    def label(self, sym, target):
        """Readable label for a relocation target."""
        if sym.name and not sym.name.startswith('...') and sym.type in ('OBJECT', 'FUNC', 'NOTYPE') \
                and sym.value <= target < sym.value + max(sym.size, 1):
            off = target - sym.value
            return sym.name if off == 0 else '%s+0x%X' % (sym.name, off)
        s, off = self.obj_at(target)
        if s is not None:
            return s.name if off == 0 else '%s+0x%X' % (s.name, off)
        sec = self.elf.section_of(target)
        return '%s_%08X' % ((sec or 'abs').lstrip('.'), target)

    def literal(self, target, mnemonic):
        """Comment text for a string or float literal at target."""
        if mnemonic in FLOAT_LOADS:
            n = FLOAT_LOADS[mnemonic]
            b = self.elf.read(target, n)
            if b is not None:
                v = struct.unpack('>f' if n == 4 else '>d', b)[0]
                return fmt_float(v), ('f32' if n == 4 else 'f64', b.hex())
        s = self.string_at(target)
        if s is not None:
            return json.dumps(s), ('str', s)
        return None, None

    def string_at(self, target):
        """String literal at target: an anonymous compiler literal (@N, ...rodata.0+x) or a char
        array whose symbol is about the string's size -- not any data that happens to be printable."""
        s = self.elf.cstring(target)
        if s is None:
            return None
        o, off = self.obj_at(target)
        if o is None or o.name.startswith('@') or o.name.startswith('...') or o.size == 0:
            return s
        if off == 0 and len(s) + 1 <= o.size <= len(s) + 4:
            return s
        if o.type == 'OBJECT' and off > 0 and self.elf.read(target - 1, 1) == b'\0':
            return s if o.size >= 64 and self._mostly_text(o) else None
        return None

    def _mostly_text(self, o):
        b = self.elf.read(o.value, min(o.size, 4096))
        if b is None:
            return False
        txt = sum(1 for c in b if 0x20 <= c < 0x7f or c == 0)
        return txt >= 0.9 * len(b)


def jump_tables(prog):
    """Switch tables: runs of .data/.rodata words relocated to function+offset of one function."""
    tables = defaultdict(list)    # func key -> [entry counts]
    run_func, run_len, last = None, 0, None
    for off, target, sym, add in prog.data_relocs:
        f = prog.func_at(target)
        is_jt = f is not None and target != f[0]
        key = f[2] if is_jt else None
        if is_jt and key == run_func and last is not None and off == last + 4:
            run_len += 1
        else:
            if run_func is not None and run_len >= 2:
                tables[run_func].append(run_len)
            run_func, run_len = (key, 1) if is_jt else (None, 0)
        last = off
    if run_func is not None and run_len >= 2:
        tables[run_func].append(run_len)
    return tables


def analyse(prog, md, dump_dir=None, only=None):
    """Disassemble every function; return per-function features."""
    feats = OrderedDict()
    jt = jump_tables(prog)
    jt_targets = set()
    for off, target, sym, add in prog.data_relocs:
        f = prog.func_at(target)
        if f is not None and target != f[0]:
            jt_targets.add(target)
    for (addr, size, key, name, sfile) in prog.funcs:
        if only and name != only and key != only:
            continue
        code = prog.elf.read(addr, size)
        if code is None:
            continue
        insns = list(md.disasm(code, addr))
        labels = set(t for t in jt_targets if addr <= t < addr + size)
        callees, fn_refs, data_refs, strings, floats, imms = [], [], [], [], [], []
        lines = []
        lis_val = {}
        for ins in insns:
            if ins.mnemonic in ('b', 'bl', 'bc', 'bdnz', 'bdz') or ins.mnemonic.startswith('b') and ins.op_str:
                m = re.search(r'0x([0-9a-f]+)$', ins.op_str)
                if m and ins.address not in prog.relocs:
                    t = int(m.group(1), 16)
                    if addr <= t < addr + size:
                        labels.add(t)
        for ins in insns:
            a = ins.address
            w = struct.unpack('>I', ins.bytes)[0] if len(ins.bytes) == 4 else 0
            mn, ops = ins.mnemonic, ins.op_str
            comment = ''
            rel = prog.relocs.get(a)
            if rel is None and (mn.startswith('b') or mn.startswith('bd')):
                m = re.search(r'0x([0-9a-f]+)$', ops)
                if m and addr <= int(m.group(1), 16) < addr + size:
                    ops = ops[:m.start()] + '.L%08X' % int(m.group(1), 16)
            if rel is not None:
                rtype, sym, target, add = rel
                lab = prog.label(sym, target)
                if rtype in (R_REL24, R_REL14):
                    tf = prog.func_at(target)
                    if tf is not None and tf[0] == addr and target != addr:
                        ops = re.sub(r'0x[0-9a-f]+$', '.L%08X' % target, ops)
                        labels.add(target)
                    else:
                        ops = re.sub(r'0x[0-9a-f]+$', lab, ops)
                        if tf is not None and target == tf[0]:
                            if mn.startswith('bl') or mn == 'b' or rtype == R_REL24:
                                callees.append(tf[2])
                else:
                    suffix = {R_ADDR16_HA: '@ha', R_ADDR16_HI: '@h', R_ADDR16_LO: '@l', R_SDA21: '@sda21'}.get(rtype, '')
                    ops = re.sub(r'(-?0x[0-9a-f]+|-?\b\d+\b)(?=(\(r\d+\))?$)', lab + suffix, ops, count=1)
                    tf = prog.func_at(target)
                    if tf is not None and rtype != R_ADDR16_HA:
                        fn_refs.append(tf[2])
                    elif rtype != R_ADDR16_HA and rtype != R_ADDR16_HI:
                        txt, lit = prog.literal(target, mn)
                        if lit is not None:
                            comment = txt
                            if lit[0] == 'str':
                                strings.append(lit[1])
                            else:
                                floats.append('%s:%s' % lit)
                        else:
                            s, off = prog.obj_at(target)
                            data_refs.append(s.name if s is not None else '%08X' % target)
                            if rtype == R_SDA21 or True:
                                comment = '0x%08X' % target if not comment else comment
            else:
                op = w >> 26
                rd, ra, imm = (w >> 21) & 31, (w >> 16) & 31, w & 0xffff
                if op == 15 and ra == 0:                       # lis
                    lis_val[rd] = imm << 16
                    if imm not in (0, 0x8000, 0xffff, 0x4330, 0x4338):
                        imms.append(imm << 16)
                elif op == 24 and ra in lis_val and rd == ra:  # ori after lis -> 32-bit constant (rS=rd field)
                    pass
                elif op == 24:
                    rs = (w >> 21) & 31
                    if rs in lis_val:
                        v = lis_val.pop(rs) | imm
                        imms.append(v)
                        comment = '0x%08X' % v
                elif op == 14 and ra == 0:                     # li
                    v = sext16(imm)
                    if abs(v) >= 256:
                        imms.append(v & 0xffffffff)
                elif op == 14 and ra in lis_val and rd == ra:
                    v = (lis_val.pop(ra) + sext16(imm)) & 0xffffffff
                    imms.append(v)
                    comment = '0x%08X' % v
                elif op in (10, 11):                           # cmplwi / cmpwi
                    v = imm if op == 10 else sext16(imm)
                    if abs(v) >= 256:
                        imms.append(v & 0xffffffff)
            if dump_dir is not None:
                pre = '.L%08X:\n' % a if a in labels else ''
                body = '    %-8s %s' % (mn, ops)
                lines.append('%s/* %08X %04X %s */ %s%s' % (pre, a, a - addr, ins.bytes.hex().upper(),
                                                             body, ('  # ' + comment) if comment else ''))
        feats[key] = OrderedDict([
            ('name', name), ('file', sfile), ('address', '0x%08X' % addr), ('size', size),
            ('callees', sorted(set(callees) - {key})), ('fn_refs', sorted(set(fn_refs) - {key})),
            ('data_refs', sorted(set(data_refs))), ('strings', sorted(set(strings))),
            ('floats', sorted(set(floats))), ('imms', sorted(set(imms))), ('jump_tables', jt.get(key, [])),
        ])
        if dump_dir is not None:
            d = os.path.join(dump_dir, sfile)
            os.makedirs(d, exist_ok=True)
            fn = re.sub(r'[^A-Za-z0-9_.$@-]', '_', key)
            with open(os.path.join(d, fn + '.s'), 'w') as f:
                f.write('/* %s  %s  0x%08X size 0x%X  (rush_d.elf, generated by tools/gcref/disasm.py) */\n'
                        % (name, sfile, addr, size))
                f.write('glabel %s\n' % name)
                f.write('\n'.join(lines) + '\n')
    return feats


def table_strings(prog):
    """table object -> strings it points at (pointer tables in data sections, one level)."""
    out = defaultdict(set)
    for off, target, sym, add in prog.data_relocs:
        s = prog.string_at(target)
        if s is None:
            continue
        tab, _ = prog.obj_at(off)
        if tab is not None:
            out[tab.name].add(s)
    return out


def build_outputs(prog, feats):
    tstr = table_strings(prog)
    for k, f in feats.items():
        via = set()
        tabs = []
        for d in f['data_refs']:
            if d in tstr:
                via |= tstr[d]
                tabs.append(d)
        f['strings_via_tables'] = sorted(via - set(f['strings']))
        f['tables'] = sorted(tabs)
    callers = defaultdict(set)
    for k, f in feats.items():
        for c in f['callees']:
            callers[c].add(k)
    cg = OrderedDict()
    for k, f in feats.items():
        cg[k] = OrderedDict([('file', f['file']), ('address', f['address']), ('size', f['size']),
                             ('callees', f['callees']), ('callers', sorted(callers.get(k, ()))),
                             ('fn_refs', f['fn_refs'])])
    sx = defaultdict(lambda: {'direct': set(), 'via_table': set(), 'tables': set()})
    for k, f in feats.items():
        for s in f['strings']:
            sx[s]['direct'].add(k)
        for t in f['tables']:
            for s in tstr[t]:
                if s not in f['strings']:
                    sx[s]['via_table'].add(k)
                    sx[s]['tables'].add(t)
    sxj = OrderedDict((s, OrderedDict([('direct', sorted(v['direct'])), ('via_table', sorted(v['via_table'])),
                                       ('tables', sorted(v['tables']))])) for s, v in sorted(sx.items()))
    return cg, sxj


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--elf', default=DEFAULT_ELF)
    ap.add_argument('--ref-dir', default=os.path.join(REPO, 'reference', 'gc'))
    ap.add_argument('--out', default=os.path.join(REPO, 'docs', 'external', 'gc'))
    ap.add_argument('--no-dump', action='store_true', help='skip the per-function .s dump')
    ap.add_argument('--only', help='disassemble one function and print it')
    a = ap.parse_args(argv)
    prog = GcProgram(a.elf)
    md = cs_ppc()
    if a.only:
        tmp = os.path.join(a.ref_dir, 'disasm')
        feats = analyse(prog, md, tmp, only=a.only)
        for k, f in feats.items():
            fn = re.sub(r'[^A-Za-z0-9_.$@-]', '_', k)
            print(open(os.path.join(tmp, f['file'], fn + '.s')).read())
        return 0
    feats = analyse(prog, md, None if a.no_dump else os.path.join(a.ref_dir, 'disasm'))
    cg, sx = build_outputs(prog, feats)
    os.makedirs(a.ref_dir, exist_ok=True)
    with open(os.path.join(a.ref_dir, 'features.json'), 'w') as f:
        json.dump(feats, f, separators=(',', ':'))
    os.makedirs(a.out, exist_ok=True)
    with open(os.path.join(a.out, 'callgraph.json'), 'w') as f:
        json.dump({'generated_by': 'python3 -m tools.gcref.disasm', 'functions': cg}, f, indent=0,
                  separators=(',', ':'))
    with open(os.path.join(a.out, 'strings_xref.json'), 'w') as f:
        json.dump({'generated_by': 'python3 -m tools.gcref.disasm', 'strings': sx}, f, indent=0,
                  separators=(',', ':'))
    nstr = sum(1 for v in sx.values() if v['direct'])
    print('functions %d; call edges %d; strings referenced %d (direct %d, only via tables %d)' % (
        len(feats), sum(len(f['callees']) for f in feats.values()), len(sx), nstr, len(sx) - nstr))
    return 0


if __name__ == '__main__':
    sys.exit(main())
