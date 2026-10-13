#!/usr/bin/env python3
"""Decode the CodeWarrior DWARF v1 ``.debug``/``.line`` sections of the GameCube ``rush_d.elf``.

Writes (default ``docs/external/gc/``):
  types.h        every struct/union/enum, grouped by the first source file that defines it,
                 with member byte offsets and sizes as trailing comments
  functions.json per function: file, address, size, lines, signature, params, locals
  globals.json   typed globals and file/function statics with fixed addresses

Usage: python3 -m tools.gcref.dwarf1 [--elf rush_d.elf] [--out docs/external/gc] [--summary]

CodeWarrior pitfalls handled here (writeup.md):
  * a linker-stripped entry has tag 0 and a length covering its whole subtree; its own attributes
    are still readable and end at an attribute id of 0x0000;
  * ``.rela.debug`` is NOT applied: the stored values are the linker's final ones;
  * there are no TAG_typedef / TAG_pointer_type entries: pointers, const and volatile are
    modifiers (AT_mod_fund_type / AT_mod_u_d_type), and typedef names are not recorded.
Standard library only, Python 3.9+.
"""
import argparse
import json
import os
import struct
import sys
from collections import OrderedDict, defaultdict

if __package__ in (None, ''):
    sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..')))
from tools.gcref.elf import Elf, DEFAULT_ELF, REPO  # noqa: E402

TAG = {
    0x01: 'array_type', 0x02: 'class_type', 0x03: 'entry_point', 0x04: 'enumeration_type',
    0x05: 'formal_parameter', 0x06: 'global_subroutine', 0x07: 'global_variable', 0x0a: 'label',
    0x0b: 'lexical_block', 0x0c: 'local_variable', 0x0d: 'member', 0x0f: 'pointer_type',
    0x10: 'reference_type', 0x11: 'compile_unit', 0x13: 'structure_type', 0x14: 'subroutine',
    0x15: 'subroutine_type', 0x16: 'typedef', 0x17: 'union_type', 0x18: 'unspecified_parameters',
    0x1d: 'inlined_subroutine',
}
AT_SIBLING, AT_LOCATION, AT_NAME = 0x0012, 0x0023, 0x0038
AT_FUND_TYPE, AT_MOD_FUND_TYPE, AT_USER_DEF_TYPE, AT_MOD_U_D_TYPE = 0x0055, 0x0063, 0x0072, 0x0083
AT_SUBSCR_DATA, AT_BYTE_SIZE, AT_BIT_OFFSET, AT_BIT_SIZE = 0x00a3, 0x00b6, 0x00c5, 0x00d6
AT_ELEMENT_LIST_B4, AT_ELEMENT_LIST_B2 = 0x00f4, 0x00f3
AT_STMT_LIST, AT_LOW_PC, AT_HIGH_PC = 0x0106, 0x0111, 0x0121
AT_MW_MANGLED = 0x2008

# Fundamental types (DWARF v1 FT_*, plus the MW long long extensions).
FUND = {
    0x0001: ('char', 1), 0x0002: ('signed char', 1), 0x0003: ('unsigned char', 1),
    0x0004: ('short', 2), 0x0005: ('signed short', 2), 0x0006: ('unsigned short', 2),
    0x0007: ('int', 4), 0x0008: ('signed int', 4), 0x0009: ('unsigned int', 4),
    0x000a: ('long', 4), 0x000b: ('signed long', 4), 0x000c: ('unsigned long', 4),
    0x000d: ('void *', 4), 0x000e: ('float', 4), 0x000f: ('double', 8), 0x0010: ('long double', 8),
    0x0014: ('void', 0), 0x0015: ('bool', 1),
    0x8008: ('long long', 8), 0x8208: ('unsigned long long', 8),
}
MOD = {1: 'pointer', 2: 'reference', 3: 'const', 4: 'volatile'}


def _form_size(d, p, form):
    """Return (value, new_pos) for one attribute value of the given form."""
    if form in (1, 2, 6):
        return struct.unpack_from('>I', d, p)[0], p + 4
    if form == 5:
        return struct.unpack_from('>H', d, p)[0], p + 2
    if form == 7:
        return struct.unpack_from('>Q', d, p)[0], p + 8
    if form == 3:
        n = struct.unpack_from('>H', d, p)[0]
        return bytes(d[p + 2:p + 2 + n]), p + 2 + n
    if form == 4:
        n = struct.unpack_from('>I', d, p)[0]
        return bytes(d[p + 4:p + 4 + n]), p + 4 + n
    if form == 8:
        q = d.index(b'\0', p)
        return d[p:q].decode('latin-1'), q + 1
    raise ValueError('unknown DWARF v1 form %d at 0x%x' % (form, p))


class Entry(object):
    __slots__ = ('off', 'length', 'tag', 'attrs', 'children', 'parent', 'stripped')

    def __init__(self, off, length, tag, attrs, stripped):
        self.off, self.length, self.tag, self.attrs = off, length, tag, attrs
        self.children, self.parent, self.stripped = [], None, stripped

    @property
    def name(self):
        return self.attrs.get(AT_NAME)

    @property
    def tagname(self):
        return TAG.get(self.tag, 'tag_0x%x' % self.tag)


class Dwarf1(object):
    """Parsed .debug tree: ``cus`` (compile units) and ``entries`` (offset -> Entry)."""

    def __init__(self, elf):
        self.elf = elf
        self.d = elf.section_bytes('.debug')
        self.line = elf.section_bytes('.line')
        self.entries = {}
        self.cus = []
        self.canon = {}
        self._parse()

    # ---- raw entries -------------------------------------------------------------------------
    def _entry(self, pos):
        d = self.d
        length = struct.unpack_from('>I', d, pos)[0]
        if length < 8:
            return None, pos + (length if length >= 4 else 4)
        tag = struct.unpack_from('>H', d, pos + 4)[0]
        attrs = OrderedDict()
        p, end = pos + 6, pos + length
        while p + 2 <= end:
            at = struct.unpack_from('>H', d, p)[0]
            p += 2
            if at == 0:          # CodeWarrior: attribute id 0 ends the entry (stripped subtrees)
                break
            v, p = _form_size(d, p, at & 15)
            attrs[at] = v
        e = Entry(pos, length, tag, attrs, tag == 0)
        self.entries[pos] = e
        return e, pos + length

    def _children(self, start, end, parent):
        """Walk a sibling chain from start; a null entry ends it."""
        pos = start
        while pos < end:
            e, nxt = self._entry(pos)
            if e is None:
                return
            e.parent = parent
            parent.children.append(e)
            sib = e.attrs.get(AT_SIBLING)
            if not e.stripped and sib and sib > nxt:
                self._children(nxt, sib, e)
                nxt = sib
            pos = nxt

    def _parse(self):
        pos, n = 0, len(self.d)
        while pos < n:
            e, nxt = self._entry(pos)
            if e is None:
                pos = nxt
                continue
            sib = e.attrs.get(AT_SIBLING) or n
            if e.tag == 0x11:
                self.cus.append(e)
                self._children(nxt, sib, e)
                pos = sib
            else:
                pos = nxt

    def walk(self, e=None):
        stack = list(reversed(self.cus)) if e is None else [e]
        while stack:
            x = stack.pop()
            yield x
            stack.extend(reversed(x.children))

    # ---- line table --------------------------------------------------------------------------
    def line_rows(self, cu):
        off = cu.attrs.get(AT_STMT_LIST)
        if off is None:
            return []
        length, base = struct.unpack_from('>II', self.line, off)
        rows = []
        p = off + 8
        while p + 10 <= off + length:
            ln, col, delta = struct.unpack_from('>IHI', self.line, p)
            rows.append((base + delta, ln))
            p += 10
        return rows

    # ---- types -------------------------------------------------------------------------------
    def type_of(self, e):
        """Type descriptor of an entry: ('fund', ft) | ('ref', off) | ('mod', [mods], base) | None (void)."""
        a = e.attrs
        if AT_FUND_TYPE in a:
            return ('fund', a[AT_FUND_TYPE])
        if AT_USER_DEF_TYPE in a:
            return ('ref', a[AT_USER_DEF_TYPE])
        if AT_MOD_FUND_TYPE in a:
            b = a[AT_MOD_FUND_TYPE]
            return ('mod', list(b[:-2]), ('fund', struct.unpack('>H', b[-2:])[0]))
        if AT_MOD_U_D_TYPE in a:
            b = a[AT_MOD_U_D_TYPE]
            return ('mod', list(b[:-4]), ('ref', struct.unpack('>I', b[-4:])[0]))
        return None

    def array_info(self, e):
        """(dims, element type) of an array_type entry; dims are element counts (None = unknown)."""
        b = e.attrs.get(AT_SUBSCR_DATA, b'')
        p, dims, elem = 0, [], None
        while p < len(b):
            fmt = b[p]
            p += 1
            if fmt == 8:                                  # FMT_ET: element type attribute follows
                at = struct.unpack_from('>H', b, p)[0]
                v, p = _form_size(b, p + 2, at & 15)
                elem = self.type_of(Entry(0, 0, 0, {at: v}, False))
                break
            if fmt in (0, 1, 2, 3):
                p += 2                                    # index fundamental type
            else:
                p += 4                                    # index user-defined type
            bounds = []
            for kind in ((fmt & 2) >> 1, fmt & 1):        # lower, upper: 0 = const, 1 = location block
                if kind == 0:
                    bounds.append(struct.unpack_from('>i', b, p)[0])
                    p += 4
                else:
                    n = struct.unpack_from('>H', b, p)[0]
                    bounds.append(None)
                    p += 2 + n
            lo, hi = bounds
            dims.append(hi - lo + 1 if lo is not None and hi is not None else None)
        return dims, elem

    def enum_values(self, e):
        b = e.attrs.get(AT_ELEMENT_LIST_B4) or e.attrs.get(AT_ELEMENT_LIST_B2) or b''
        size = 4
        p, out = 0, []
        while p + size <= len(b):
            v = struct.unpack_from('>i', b, p)[0]
            p += size
            q = b.index(b'\0', p)
            out.append((b[p:q].decode('latin-1'), v))
            p = q + 1
        out.reverse()                                     # DWARF v1 lists enumerators last-first
        return out

    def sizeof(self, t, depth=0):
        if t is None:
            return 0
        if t[0] == 'fund':
            return FUND.get(t[1], ('', 4))[1]
        if t[0] == 'mod':
            if any(m in (1, 2) for m in t[1]):
                return 4
            return self.sizeof(t[2], depth + 1)
        e = self.entries.get(t[1])
        if e is None or depth > 20:
            return None
        if e.tag in (0x13, 0x17, 0x04, 0x02):
            return e.attrs.get(AT_BYTE_SIZE)
        if e.tag == 0x01:
            dims, elem = self.array_info(e)
            n = 1
            for x in dims:
                if x is None:
                    return None
                n *= x
            es = self.sizeof(elem, depth + 1)
            return None if es is None else n * es
        if e.tag == 0x15:
            return 4
        return None

    def tagname_of(self, e):
        kind = {0x13: 'struct', 0x17: 'union', 0x04: 'enum', 0x02: 'struct'}[e.tag]
        return '%s %s' % (kind, e.name or self.canon.get(e.off) or anon_name(e))

    def canonicalize(self):
        """Give identical per-CU copies of anonymous aggregates one name (iterate to a fixpoint)."""
        aggs = [e for e in self.walk() if not e.stripped and e.tag in (0x13, 0x17, 0x04)]
        ngroups = None
        while True:
            groups = OrderedDict()
            for e in aggs:
                groups.setdefault(aggregate_key(self, e), []).append(e)
            if len(groups) == ngroups:
                return groups
            ngroups = len(groups)
            for k, members in groups.items():
                if members[0].name is None:
                    nm = anon_name(members[0])
                    for e in members:
                        self.canon[e.off] = nm

    def decl(self, t, inner='', depth=0):
        """C declaration of type t around declarator ``inner`` (e.g. a member name)."""
        sp = (' ' + inner) if inner else ''
        if t is None:
            return 'void' + sp
        if depth > 30:
            return '/*recursive*/ void' + sp
        if t[0] == 'fund':
            nm = FUND.get(t[1], ('__fund_0x%x' % t[1], 4))[0]
            if nm.endswith('*'):
                return nm + inner
            return nm + sp
        if t[0] == 'mod':
            mods, base = t[1], t[2]
            # Modifiers are listed outermost first: [pointer, const] char == const char *.
            cur = inner
            pre = []
            for m in mods:
                if m == 1:
                    cur = '*' + (' '.join(pre) + ' ' if pre else '') + cur
                    pre = []
                elif m == 2:
                    cur = '&' + cur
                else:
                    pre.append(MOD[m])
            if pre:
                # qualifiers applying to the base type itself
                return ' '.join(pre) + ' ' + self.decl(base, cur, depth + 1).lstrip()
            return self.decl(base, cur, depth + 1)
        e = self.entries.get(t[1])
        if e is None:
            return '/*?0x%x*/ void%s' % (t[1], sp)
        if e.tag in (0x13, 0x17, 0x04, 0x02):
            return self.tagname_of(e) + sp
        if e.tag == 0x01:
            dims, elem = self.array_info(e)
            if inner.startswith('*') or inner.startswith('&'):
                inner = '(' + inner + ')'
            return self.decl(elem, inner + ''.join('[%s]' % ('' if x is None else x) for x in dims), depth + 1)
        if e.tag == 0x15:
            params = [self.decl(self.type_of(c), c.name or '') for c in e.children if c.tag == 0x05]
            if inner.startswith('*'):
                inner = '(' + inner + ')'
            return self.decl(self.type_of(e), '%s(%s)' % (inner, ', '.join(params) or 'void'), depth + 1)
        return '/*%s*/ void%s' % (e.tagname, sp)

    def type_str(self, t):
        return self.decl(t, '').strip()

    # ---- locations ---------------------------------------------------------------------------
    @staticmethod
    def location(b):
        """Evaluate a DWARF v1 location block -> dict(kind=reg|stack|addr|const, ...)."""
        if not b:
            return None
        p, st, reg = 0, [], None
        while p < len(b):
            op = b[p]
            p += 1
            if op in (1, 2, 3, 4):
                v = struct.unpack_from('>I', b, p)[0]
                p += 4
                if op == 1:
                    st.append(('reg', v))
                elif op == 2:
                    st.append(('basereg', v))
                elif op == 3:
                    st.append(('addr', v))
                else:
                    st.append(('const', v - (1 << 32) if v & 0x80000000 else v))
            elif op == 7 and len(st) == 1 and st[0][0] == 'const':
                pass                                      # member offset: base + const
            elif op == 7 and len(st) >= 2:                # OP_ADD
                b2, a2 = st.pop(), st.pop()
                if a2[0] == 'basereg' and b2[0] == 'const':
                    st.append(('stack', a2[1], b2[1]))
                elif a2[0] == 'const' and b2[0] == 'const':
                    st.append(('const', a2[1] + b2[1]))
                else:
                    st.append(('expr', a2, b2))
            elif op in (5, 6):
                st.append(('deref', st.pop() if st else None))
            else:
                return {'kind': 'raw', 'bytes': b.hex()}
        if not st:
            return None
        x = st[-1]
        if x[0] == 'reg':
            r = x[1]
            return {'kind': 'reg', 'reg': ('r%d' % r) if r < 32 else ('f%d' % (r - 32))}
        if x[0] == 'stack':
            return {'kind': 'stack', 'base': 'r%d' % x[1], 'offset': x[2]}
        if x[0] == 'basereg':
            return {'kind': 'stack', 'base': 'r%d' % x[1], 'offset': 0}
        if x[0] == 'addr':
            return {'kind': 'addr', 'addr': '0x%08X' % x[1]}
        if x[0] == 'const':
            return {'kind': 'const', 'value': x[1]}
        return {'kind': 'raw', 'bytes': b.hex()}


def anon_name(e):
    return '__anon_%x' % e.off


def cu_file(cu):
    path = cu.name or ''
    return path.replace('\\', '/').split('/')[-1], path


def short_dir(path):
    parts = path.replace('\\', '/').split('/')
    if 'GameSource' in parts:
        return 'GameSource/' + '/'.join(parts[parts.index('GameSource') + 1:])
    if 'NGC' in parts:
        return 'NGC/' + '/'.join(parts[parts.index('NGC') + 1:])
    return path


# ---- renderers ---------------------------------------------------------------------------------
def render_aggregate(dw, e, indent=''):
    """C text of a struct/union/enum definition with offset/size comments."""
    lines = []
    if e.tag == 0x04:
        lines.append('%senum %s { /* size 0x%x */' % (indent, e.name or anon_name(e), e.attrs.get(AT_BYTE_SIZE, 0)))
        for nm, v in dw.enum_values(e):
            lines.append('%s    %s = %d,' % (indent, nm, v))
        lines.append(indent + '};')
        return '\n'.join(lines)
    size = e.attrs.get(AT_BYTE_SIZE)
    lines.append('%s%s { /* size 0x%x */' % (indent, dw.tagname_of(e), size or 0))
    for m in e.children:
        if m.tag != 0x0d:
            continue
        t = dw.type_of(m)
        loc = dw.location(m.attrs.get(AT_LOCATION))
        off = loc['value'] if loc and loc['kind'] == 'const' else None
        decl = dw.decl(t, m.name or '')
        if AT_BIT_SIZE in m.attrs:
            decl += ' : %d' % m.attrs[AT_BIT_SIZE]
            note = 'bit_offset %d' % m.attrs.get(AT_BIT_OFFSET, 0)
        else:
            note = ''
        msz = dw.sizeof(t)
        cm = '/* 0x%03x' % off if off is not None else '/* ?'
        cm += ' size 0x%x' % msz if msz is not None else ''
        cm += (' ' + note if note else '') + ' */'
        lines.append('%s    %s; %s' % (indent, decl, cm))
    lines.append(indent + '};')
    return '\n'.join(lines)


def aggregate_key(dw, e):
    """Structural identity for de-duplicating per-CU copies of one type."""
    if e.tag == 0x04:
        return ('enum', e.name, tuple(dw.enum_values(e)))
    mem = []
    for m in e.children:
        if m.tag == 0x0d:
            loc = dw.location(m.attrs.get(AT_LOCATION))
            mem.append((m.name, dw.type_str(dw.type_of(m)), loc['value'] if loc and loc['kind'] == 'const' else None))
    return (e.tag, e.name, e.attrs.get(AT_BYTE_SIZE), tuple(mem))


def build(dw):
    """Collect types, functions and globals into plain data."""
    dw.canonicalize()
    types_by_key = OrderedDict()
    functions, globals_ = [], []
    counts = defaultdict(int)
    for e in dw.walk():
        counts[e.tagname if not e.stripped else 'stripped'] += 1
    for cu in dw.cus:
        fname, fpath = cu_file(cu)
        rows = dw.line_rows(cu)
        for e in dw.walk(cu):
            if e.stripped:
                continue
            if e.tag in (0x13, 0x17, 0x04):
                k = aggregate_key(dw, e)
                if k not in types_by_key:
                    types_by_key[k] = {'entry': e, 'file': fname, 'path': fpath, 'files': [fname]}
                elif fname not in types_by_key[k]['files']:
                    types_by_key[k]['files'].append(fname)
            elif e.tag in (0x06, 0x14) and AT_LOW_PC in e.attrs:
                functions.append(function_record(dw, e, fname, fpath, rows))
            elif e.tag in (0x07, 0x0c):
                loc = dw.location(e.attrs.get(AT_LOCATION))
                if loc and loc['kind'] == 'addr':
                    owner = e.parent
                    while owner is not None and owner.tag not in (0x06, 0x14, 0x11):
                        owner = owner.parent
                    t = dw.type_of(e)
                    globals_.append(OrderedDict([
                        ('name', e.name), ('address', loc['addr']),
                        ('section', dw.elf.section_of(int(loc['addr'], 16))),
                        ('scope', 'global' if e.tag == 0x07 else ('function_static' if owner is not None and owner.tag != 0x11 else 'file_static')),
                        ('function', owner.name if owner is not None and owner.tag != 0x11 else None),
                        ('file', fname), ('type', dw.type_str(t)), ('decl', dw.decl(t, e.name or '')),
                        ('size', dw.sizeof(t)), ('debug_offset', '0x%X' % e.off)]))
    return types_by_key, functions, globals_, counts


def function_record(dw, e, fname, fpath, rows):
    lo, hi = e.attrs[AT_LOW_PC], e.attrs[AT_HIGH_PC]
    lines = [ln for a, ln in rows if lo <= a < hi]
    params, locals_, blocks = [], [], []

    def visit(node, depth):
        for c in node.children:
            if c.stripped:
                continue
            if c.tag == 0x05 and depth == 0:
                t = dw.type_of(c)
                params.append(OrderedDict([('name', c.name), ('type', dw.type_str(t)),
                                           ('location', dw.location(c.attrs.get(AT_LOCATION)))]))
            elif c.tag == 0x0c:
                t = dw.type_of(c)
                loc = dw.location(c.attrs.get(AT_LOCATION))
                locals_.append(OrderedDict([('name', c.name), ('type', dw.type_str(t)), ('location', loc),
                                            ('block', depth)]))
            elif c.tag == 0x0b:
                blocks.append({'low_pc': '0x%08X' % c.attrs.get(AT_LOW_PC, 0),
                               'high_pc': '0x%08X' % c.attrs.get(AT_HIGH_PC, 0), 'depth': depth + 1})
                visit(c, depth + 1)
    visit(e, 0)
    ret = dw.type_of(e)
    sig = dw.decl(ret, '%s(%s)' % (e.name, ', '.join(dw.decl(dw.type_of(c), c.name or '')
                                                       for c in e.children if c.tag == 0x05 and not c.stripped) or 'void'))
    if e.tag == 0x14:
        sig = 'static ' + sig
    return OrderedDict([
        ('name', e.name), ('mangled', e.attrs.get(AT_MW_MANGLED)), ('file', fname), ('path', short_dir(fpath)),
        ('address', '0x%08X' % lo), ('size', hi - lo), ('scope', 'global' if e.tag == 0x06 else 'static'),
        ('line', min(lines) if lines else None), ('end_line', max(lines) if lines else None),
        ('signature', sig), ('return_type', dw.type_str(ret)), ('params', params), ('locals', locals_),
        ('blocks', blocks), ('debug_offset', '0x%X' % e.off)])


def anon_users(dw, types_by_key, globals_):
    """__anon_X -> names that use it (globals, struct members), to stand in for lost typedef names."""
    import re
    pat = re.compile(r'__anon_[0-9a-f]+')
    users = defaultdict(list)
    for g in globals_:
        for m in set(pat.findall(g['type'])):
            users[m].append(g['name'])
    for v in types_by_key.values():
        e = v['entry']
        if e.tag == 0x04:
            continue
        owner = e.name or dw.canon.get(e.off) or anon_name(e)
        for c in e.children:
            if c.tag == 0x0d:
                for m in set(pat.findall(dw.type_str(dw.type_of(c)))):
                    if m != owner:
                        users[m].append('%s.%s' % (owner, c.name))
    return users


def write_types_h(dw, types_by_key, path, globals_=()):
    users = anon_users(dw, types_by_key, globals_)
    by_file = OrderedDict()
    for k, v in types_by_key.items():
        by_file.setdefault(v['file'], []).append(v)
    with open(path, 'w') as f:
        f.write('/* GameCube rush_d.elf (CodeWarrior DWARF v1): every struct/union/enum, de-duplicated.\n'
                ' * Generated by tools/gcref/dwarf1.py -- do not edit. Each type is listed under the first\n'
                ' * compile unit that defines it; "also in" names the other files with an identical copy.\n'
                ' * Trailing comments: byte offset and size. GC is a divergent later sibling of the N64\n'
                ' * game: layouts are leads, not facts (PowerPC/CodeWarrior alignment; N64 is IDO MIPS).\n'
                ' * DWARF v1 records no typedef names; anonymous aggregates are named __anon_<debug offset>.\n'
                ' * Not compilable as-is (forward references are not ordered). */\n\n')
        for fname, items in by_file.items():
            f.write('/* ======== %s ======== */\n\n' % fname)
            for v in items:
                e = v['entry']
                others = [x for x in v['files'] if x != fname]
                if others:
                    shown = ', '.join(others[:8]) + (' ... (+%d)' % (len(others) - 8) if len(others) > 8 else '')
                    f.write('/* also in: %s */\n' % shown)
                nm = e.name or dw.canon.get(e.off) or anon_name(e)
                if e.name is None and users.get(nm):
                    u = list(OrderedDict.fromkeys(users[nm]))
                    f.write('/* used by: %s */\n' % (', '.join(u[:6]) + (' ... (+%d)' % (len(u) - 6) if len(u) > 6 else '')))
                f.write(render_aggregate(dw, e) + '\n\n')


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--elf', default=DEFAULT_ELF)
    ap.add_argument('--out', default=os.path.join(REPO, 'docs', 'external', 'gc'))
    ap.add_argument('--summary', action='store_true', help='print counts only, write nothing')
    a = ap.parse_args(argv)
    dw = Dwarf1(Elf(a.elf))
    types_by_key, functions, globals_, counts = build(dw)
    keys = ['compile_unit', 'structure_type', 'member', 'union_type', 'enumeration_type', 'formal_parameter',
            'local_variable', 'global_subroutine', 'subroutine', 'global_variable', 'array_type',
            'subroutine_type', 'lexical_block', 'stripped']
    print('entries: ' + ', '.join('%s %d' % (k, counts.get(k, 0)) for k in keys))
    print('writeup.md expects: structure_type 2431, member 26640, union_type 178, enumeration_type 130, '
          'formal_parameter 2757, local_variable 5591')
    uniq = defaultdict(int)
    for k in types_by_key:
        uniq[k[0]] += 1
    print('unique aggregates after de-duplication: struct %d, union %d, enum %d' % (uniq[0x13], uniq[0x17], uniq['enum']))
    print('linked functions with debug info: %d (params %d, locals %d); typed fixed-address variables: %d' % (
        len(functions), sum(len(x['params']) for x in functions), sum(len(x['locals']) for x in functions), len(globals_)))
    if a.summary:
        return 0
    os.makedirs(a.out, exist_ok=True)
    write_types_h(dw, types_by_key, os.path.join(a.out, 'types.h'), globals_)
    functions.sort(key=lambda x: x['address'])
    with open(os.path.join(a.out, 'functions.json'), 'w') as f:
        json.dump({'generated_by': 'python3 -m tools.gcref.dwarf1', 'functions': functions}, f, indent=0,
                  separators=(',', ':'))
    globals_.sort(key=lambda x: x['address'])
    with open(os.path.join(a.out, 'globals.json'), 'w') as f:
        json.dump({'generated_by': 'python3 -m tools.gcref.dwarf1', 'globals': globals_}, f, indent=0,
                  separators=(',', ':'))
    for n in ('types.h', 'functions.json', 'globals.json'):
        print('wrote %s (%d bytes)' % (os.path.join(a.out, n), os.path.getsize(os.path.join(a.out, n))))
    return 0


if __name__ == '__main__':
    sys.exit(main())
