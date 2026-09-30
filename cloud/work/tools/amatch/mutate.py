#!/usr/bin/env python3
"""Deterministic source-to-source mutation catalog for IDO C89 sources.

    from amatch.mutate import mutations, apply
    muts = mutations(src, fn="func_800A8F38")      # list[Mutation]
    new_src = apply(src, muts[0].id)

    python3 cloud/work/tools/amatch/mutate.py FILE FUNC [--list] [--json]

Every mutation is a *textual* edit of the original source: a tokenizer and
bracket matcher, a small statement parser (if/loops/switch/decls/expression
statements) and a precedence-climbing expression parser are used only to find
edit sites; nothing is re-printed wholesale, so the layout of everything a
mutation does not touch (IDO's scheduling is line-layout sensitive) is kept.
Not an LLM, not a C front end: anything the parsers cannot understand is
skipped, never guessed.

Scope: with `fn` only that function body (plus the declarations it references
for the file-level families `extern_toggle` and `knr_proto`) is mutated;
with `fn=None` every function definition in the translation unit is.

Mutation fields: name (family), new_src, cost (1 cheap .. 9 expensive; risky
ones carry +3), target (diff shape it addresses: frame, register, order,
loop, branch, literal, type, linkage, callsetup, spelling), id (stable for a
given source: `name@fn#hash`), flags (('semantic_risk',) when the rewrite is
not provably value-preserving), fn, desc.

Families (see CATALOG): loop_do_to_for, loop_do_to_while, loop_for_to_do,
loop_for_to_while, loop_while_to_for, loop_to_goto, loop_from_goto,
counter_type, local_inline, decl_order, pad_local, extern_toggle,
ptr_launder, knr_proto, param_type, shared_exit, mul2_shift, incr_form,
commute, cmp_flip, literal_spelling, andor_nest, ifelse_swap, switch_perm,
stmt_swap.

Known gaps are listed in docs at the bottom of this file (GAPS).
"""
import argparse
import difflib
import hashlib
import itertools
import json
import os
import re
import sys
from functools import lru_cache

# --------------------------------------------------------------------------
# tokenizer
# --------------------------------------------------------------------------

TOKEN_RE = re.compile(
    r"""
    (?P<pp>^[ \t]*\#(?:\\\n|/\*.*?\*/|[^\n])*)
   |(?P<ws>[ \t\r\f\v]+|\n)
   |(?P<lc>//[^\n]*)
   |(?P<bc>/\*.*?\*/)
   |(?P<str>"(?:\\.|[^"\\\n])*")
   |(?P<chr>'(?:\\.|[^'\\\n])*')
   |(?P<num>0[xX][0-9a-fA-F]+[uUlL]*
           |(?:\d+\.\d*|\.\d+)(?:[eE][+-]?\d+)?[fFlL]?
           |\d+[eE][+-]?\d+[fFlL]?
           |\d+[uUlL]*)
   |(?P<id>[A-Za-z_]\w*)
   |(?P<op>>>=|<<=|\.\.\.|->|\+\+|--|<<|>>|<=|>=|==|!=|&&|\|\||[-+*/%&|^]=
           |[-+*/%&|^~!<>=?:;,.(){}\[\]])
   |(?P<other>.)
    """,
    re.X | re.S | re.M,
)


class Tok(object):
    __slots__ = ("k", "t", "s", "e")

    def __init__(self, k, t, s, e):
        self.k, self.t, self.s, self.e = k, t, s, e

    def __repr__(self):
        return "Tok(%s,%r)" % (self.k, self.t)


def tokenize(src):
    """Significant tokens (no whitespace / comments); preprocessor lines are
    single 'pp' tokens."""
    out = []
    for m in TOKEN_RE.finditer(src):
        k = m.lastgroup
        if k in ("ws", "lc", "bc"):
            continue
        out.append(Tok(k, m.group(), m.start(), m.end()))
    return out


def sig_tokens_text(src):
    """Token texts only (used by tests for token-equivalence)."""
    return [t.t for t in tokenize(src)]


def balanced(text):
    depth = {"(": 0, "[": 0, "{": 0}
    close = {")": "(", "]": "[", "}": "{"}
    for t in tokenize(text):
        if t.k != "op":
            continue
        if t.t in depth:
            depth[t.t] += 1
        elif t.t in close:
            depth[close[t.t]] -= 1
            if depth[close[t.t]] < 0:
                return False
    return not any(depth.values())


TYPE_WORDS = set(
    """void char short int long float double signed unsigned const volatile struct
    union enum register static extern auto u8 s8 u16 s16 u32 s32 u64 s64 f32 f64
    M2C_UNK size_t uintptr_t intptr_t""".split()
)
STORAGE = {"static", "extern", "register", "auto", "inline", "typedef"}
INT_TYPES = {"s8", "u8", "s16", "u16", "s32", "u32", "int", "short", "long", "char"}
FLOAT_TYPES = {"f32", "f64", "float", "double"}
KEYWORDS = {
    "if", "else", "while", "for", "do", "switch", "case", "default", "return",
    "break", "continue", "goto", "sizeof",
}
PURE_CALLS = {"M2C_FIELD", "fabsf", "sqrtf", "sizeof"}
ASSIGN_OPS = {"=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>="}
CMP_MIRROR = {"<": ">", ">": "<", "<=": ">=", ">=": "<=", "==": "==", "!=": "!="}
CMP_NEG = {"<": ">=", ">": "<=", "<=": ">", ">=": "<", "==": "!=", "!=": "=="}
BINPREC = {
    "||": 4, "&&": 5, "|": 6, "^": 7, "&": 8, "==": 9, "!=": 9, "<": 10, ">": 10,
    "<=": 10, ">=": 10, "<<": 11, ">>": 11, "+": 12, "-": 12, "*": 13, "/": 13, "%": 13,
}


class ParseError(Exception):
    pass


# --------------------------------------------------------------------------
# translation-unit scan
# --------------------------------------------------------------------------

class Func(object):
    def __init__(self, name, a, ni, lp, rp, bo, bc):
        self.name, self.a, self.ni, self.lp, self.rp, self.bo, self.bc = name, a, ni, lp, rp, bo, bc


class Parsed(object):
    def __init__(self, src):
        self.src = src
        self.sig = sig = tokenize(src)
        n = len(sig)
        self.match = match = [-1] * n
        stack = []
        pairs = {")": "(", "]": "[", "}": "{"}
        for i, t in enumerate(sig):
            if t.k != "op":
                continue
            if t.t in "([{" and len(t.t) == 1:
                stack.append(i)
            elif t.t in pairs and len(t.t) == 1:
                # pop to the matching opener kind (tolerate stray tokens)
                for j in range(len(stack) - 1, -1, -1):
                    if sig[stack[j]].t == pairs[t.t]:
                        o = stack[j]
                        del stack[j:]
                        match[o] = i
                        match[i] = o
                        break
        self.funcs = {}
        self.func_list = []
        self.items = []
        self._scan_top()
        self.types = set()
        self._collect_typedefs()
        self._bodies = {}

    def text(self, a, b):
        """Source text of sig tokens [a, b)."""
        if b <= a:
            return ""
        return self.src[self.sig[a].s:self.sig[b - 1].e]

    def _scan_top(self):
        sig, match = self.sig, self.match
        n = len(sig)
        i = 0
        start = 0
        while i < n:
            t = sig[i]
            if t.k == "pp":
                i += 1
                start = i
                continue
            x = t.t
            if t.k == "op" and x in ("(", "["):
                i = match[i] + 1 if match[i] >= 0 else i + 1
                continue
            if t.k == "op" and x == "{":
                close = match[i]
                if close < 0:
                    break
                prev = sig[i - 1] if i > start else None
                if prev is not None and prev.t == ")" and match[i - 1] >= 0:
                    lp = match[i - 1]
                    if lp > 0 and sig[lp - 1].k == "id":
                        f = Func(sig[lp - 1].t, start, lp - 1, lp, i - 1, i, close)
                        self.funcs[f.name] = f
                        self.func_list.append(f)
                        i = close + 1
                        start = i
                        continue
                i = close + 1
                continue
            if t.k == "op" and x == ";":
                self.items.append((start, i + 1))
                i += 1
                start = i
                continue
            i += 1

    def _collect_typedefs(self):
        sig = self.sig
        for a, b in self.items:
            if sig[a].t != "typedef":
                continue
            name = None
            a0 = a
            for j in range(b - 2, a, -1):
                if sig[j].t == "}" and sig[j].k == "op":
                    a0 = j
                    break
            for j in range(a0, b - 1):
                if sig[j].t == "(" and j + 2 < b and sig[j + 1].t == "*" and sig[j + 2].k == "id":
                    name = sig[j + 2].t
                    break
            if name is None:
                for j in range(b - 2, a0, -1):
                    if sig[j].k == "id":
                        name = sig[j].t
                        break
            if name:
                self.types.add(name)

    def is_type_word(self, x):
        return x in TYPE_WORDS or x in self.types


@lru_cache(maxsize=6)
def parse_source(src):
    return Parsed(src)


# --------------------------------------------------------------------------
# statement parser
# --------------------------------------------------------------------------

class N(object):
    """Statement node."""
    __slots__ = ("k", "a", "b", "s", "e", "ch", "parent", "pos", "d")

    def __init__(self, k, a, b, sig):
        self.k, self.a, self.b = k, a, b
        self.s, self.e = sig[a].s, sig[b - 1].e
        self.ch = []
        self.parent = None
        self.pos = -1
        self.d = {}


class StmtParser(object):
    def __init__(self, parsed):
        self.P = parsed
        self.sig = parsed.sig
        self.match = parsed.match

    def _end_semi(self, i):
        sig, match = self.sig, self.match
        j = i
        n = len(sig)
        while j < n:
            t = sig[j]
            if t.k == "op":
                if t.t == ";":
                    return j
                if t.t in ("(", "[", "{"):
                    if match[j] < 0:
                        raise ParseError("unbalanced")
                    j = match[j] + 1
                    continue
            j += 1
        raise ParseError("no semicolon")

    def block(self, i):
        close = self.match[i]
        if close < 0:
            raise ParseError("block")
        n = N("block", i, close + 1, self.sig)
        j = i + 1
        while j < close:
            st, j = self.stmt(j)
            st.parent = n
            st.pos = len(n.ch)
            n.ch.append(st)
        return n

    def _sub(self, node, key, i):
        st, j = self.stmt(i)
        st.parent = node
        st.pos = -1
        node.d[key] = st
        return j

    def stmt(self, i):
        sig = self.sig
        if i >= len(sig):
            raise ParseError("eof")
        t = sig[i]
        x = t.t
        if t.k == "pp":
            return N("pp", i, i + 1, sig), i + 1
        if t.k == "op":
            if x == "{":
                nd = self.block(i)
                return nd, nd.b
            if x == ";":
                return N("empty", i, i + 1, sig), i + 1
        if t.k == "id":
            if x in ("if", "while", "switch"):
                if sig[i + 1].t != "(":
                    raise ParseError("cond")
                cj = self.match[i + 1]
                nd = N(x, i, cj + 1, sig)
                nd.d["cond"] = (i + 2, cj)
                nd.d["lp"] = i + 1
                nd.d["rp"] = cj
                j = self._sub(nd, "then" if x == "if" else "body", cj + 1)
                if x == "if" and j < len(sig) and sig[j].t == "else":
                    nd.d["else_kw"] = j
                    j = self._sub(nd, "else", j + 1)
                else:
                    nd.d["else"] = None
                nd.b = j
                nd.e = sig[j - 1].e
                return nd, j
            if x == "do":
                nd = N("do", i, i + 1, sig)
                j = self._sub(nd, "body", i + 1)
                if sig[j].t != "while" or sig[j + 1].t != "(":
                    raise ParseError("do-while")
                cm = self.match[j + 1]
                if sig[cm + 1].t != ";":
                    raise ParseError("do-while ;")
                nd.d["cond"] = (j + 2, cm)
                nd.d["while_kw"] = j
                nd.b = cm + 2
                nd.e = sig[cm + 1].e
                return nd, nd.b
            if x == "for":
                if sig[i + 1].t != "(":
                    raise ParseError("for")
                m = self.match[i + 1]
                parts = []
                s = i + 2
                j = i + 2
                while j < m:
                    if sig[j].t in ("(", "[", "{") and sig[j].k == "op":
                        j = self.match[j] + 1
                        continue
                    if sig[j].t == ";" and sig[j].k == "op":
                        parts.append((s, j))
                        s = j + 1
                    j += 1
                parts.append((s, m))
                if len(parts) != 3:
                    raise ParseError("for header")
                nd = N("for", i, m + 1, sig)
                nd.d["init"], nd.d["cond"], nd.d["step"] = parts
                nd.d["lp"] = i + 1
                nd.d["rp"] = m
                j = self._sub(nd, "body", m + 1)
                nd.b = j
                nd.e = sig[j - 1].e
                return nd, j
            if x == "case":
                j = i + 1
                while j < len(sig) and not (sig[j].t == ":" and sig[j].k == "op"):
                    if sig[j].k == "op" and sig[j].t in ("(", "[", "{"):
                        j = self.match[j] + 1
                        continue
                    j += 1
                nd = N("case", i, j + 1, sig)
                nd.d["r"] = (i + 1, j)
                return nd, j + 1
            if x == "default" and sig[i + 1].t == ":":
                nd = N("default", i, i + 2, sig)
                return nd, i + 2
            if x in ("break", "continue"):
                if sig[i + 1].t != ";":
                    raise ParseError(x)
                return N(x, i, i + 2, sig), i + 2
            if x == "goto":
                j = self._end_semi(i)
                nd = N("goto", i, j + 1, sig)
                nd.d["name"] = sig[i + 1].t
                return nd, j + 1
            if x == "return":
                j = self._end_semi(i)
                nd = N("return", i, j + 1, sig)
                nd.d["r"] = (i + 1, j)
                return nd, j + 1
            if (x not in KEYWORDS and i + 1 < len(sig) and sig[i + 1].t == ":"
                    and sig[i + 1].k == "op" and not self.P.is_type_word(x)):
                nd = N("label", i, i + 2, sig)
                nd.d["name"] = x
                return nd, i + 2
            if (self.P.is_type_word(x) and i + 1 < len(sig) and (
                    sig[i + 1].k == "id" or sig[i + 1].t in ("*", "("))) or self._looks_decl(i):
                j = self._end_semi(i)
                nd = N("decl", i, j + 1, sig)
                nd.d["info"] = self.parse_decl(i, j)
                return nd, j + 1
        j = self._end_semi(i)
        nd = N("expr", i, j + 1, sig)
        nd.d["r"] = (i, j)
        return nd, j + 1

    def _looks_decl(self, i):
        """`Name x;` / `Name *x;` with an unknown typedef name."""
        sig = self.sig
        j = i + 1
        if j >= len(sig):
            return False
        stars = 0
        while j < len(sig) and sig[j].t == "*" and sig[j].k == "op":
            j += 1
            stars += 1
        if j + 1 >= len(sig) or sig[j].k != "id" or sig[j].t in KEYWORDS or self.P.is_type_word(sig[j].t):
            return False
        nt = sig[j + 1]
        if j == i + 1 and stars == 0:
            return nt.k == "op" and nt.t in (";", ",", "=", "[")
        return nt.k == "op" and nt.t in (";", ",", "[", "=") and sig[i].t not in STORAGE

    def parse_decl(self, a, b):
        """Declaration tokens [a, b) (b excludes ';')."""
        sig, match = self.sig, self.match
        segs = []
        s = a
        j = a
        while j < b:
            tk = sig[j]
            if tk.k == "op" and tk.t in ("(", "[", "{"):
                if match[j] < 0:
                    raise ParseError("decl")
                j = match[j] + 1
                continue
            if tk.k == "op" and tk.t == ",":
                segs.append((s, j))
                s = j + 1
            j += 1
        segs.append((s, b))
        decls = []
        base = None
        for si, (sa, sb) in enumerate(segs):
            k = sa
            eq = None
            while k < sb:
                tk = sig[k]
                if tk.k == "op" and tk.t in ("(", "[", "{"):
                    k = match[k] + 1
                    continue
                if tk.k == "op" and tk.t == "=":
                    eq = k
                    break
                k += 1
            le = eq if eq is not None else sb
            dims = []
            m = le
            while m > sa and sig[m - 1].t == "]" and match[m - 1] >= 0:
                o = match[m - 1]
                dims.insert(0, (o, m))
                m = o
            cplx = False
            if m > sa and sig[m - 1].k == "id":
                nm = m - 1
            else:
                nm = None
                cplx = True
            if nm is not None:
                # anything like "( * name )" means a function pointer
                for q in range(sa, nm):
                    if sig[q].t == "(" and sig[q].k == "op":
                        cplx = True
                stars = 0
                q = nm - 1
                while q >= sa and (sig[q].t == "*" or sig[q].t in ("const", "volatile")):
                    if sig[q].t == "*":
                        stars += 1
                    q -= 1
                ds = q + 1
            else:
                stars = 0
                ds = sa
            if si == 0:
                base = (a, ds)
                if ds <= a:
                    cplx = True
            decls.append({
                "name": sig[nm].t if nm is not None else None,
                "nm": nm, "stars": stars, "dims": dims, "ds": ds, "a": sa, "b": sb,
                "init": (eq + 1, sb) if eq is not None else None, "complex": cplx,
            })
        return {"base": base, "decls": decls, "segs": segs}


def parse_body(parsed, f):
    """Block node of function f (cached); ParseError if not understood."""
    key = f.name + "@%d" % f.bo
    if key in parsed._bodies:
        r = parsed._bodies[key]
        if isinstance(r, ParseError):
            raise r
        return r
    sp = StmtParser(parsed)
    try:
        b = sp.block(f.bo)
    except (ParseError, IndexError) as ex:
        r = ParseError(str(ex))
        parsed._bodies[key] = r
        raise r
    parsed._bodies[key] = b
    return b


def walk(node):
    yield node
    for c in node.ch:
        for x in walk(c):
            yield x
    for key in ("then", "else", "body"):
        c = node.d.get(key)
        if isinstance(c, N):
            for x in walk(c):
                yield x


# --------------------------------------------------------------------------
# expression parser
# --------------------------------------------------------------------------

class E(object):
    __slots__ = ("k", "a", "b", "s", "e", "op", "ch", "name", "ta", "tb", "parent")

    def __init__(self, k, a, b, sig, op=None, ch=None, name=None):
        self.k, self.a, self.b = k, a, b
        self.s, self.e = sig[a].s, sig[b - 1].e
        self.op = op
        self.ch = ch or []
        self.name = name
        self.ta = self.tb = None
        self.parent = None


class ExprParser(object):
    def __init__(self, parsed, a, b):
        self.P = parsed
        self.sig = parsed.sig
        self.match = parsed.match
        self.i = a
        self.a = a
        self.b = b

    def parse(self):
        if self.a >= self.b:
            raise ParseError("empty expr")
        e = self.expr(1)
        if self.i != self.b:
            raise ParseError("trailing tokens")
        _link(e)
        return e

    def cur(self):
        return self.sig[self.i] if self.i < self.b else None

    def expr(self, minp):
        left = self.unary()
        sig = self.sig
        while self.i < self.b:
            t = sig[self.i]
            if t.k != "op":
                break
            x = t.t
            if x == "?" and minp <= 3:
                self.i += 1
                mid = self.expr(1)
                if self.cur() is None or self.cur().t != ":":
                    raise ParseError("ternary")
                self.i += 1
                right = self.expr(3)
                left = E("ternary", left.a, right.b, sig, "?:", [left, mid, right])
                continue
            if x in ASSIGN_OPS and minp <= 2:
                self.i += 1
                right = self.expr(2)
                left = E("assign", left.a, right.b, sig, x, [left, right])
                continue
            if x == "," and minp <= 1:
                self.i += 1
                right = self.expr(2)
                left = E("comma", left.a, right.b, sig, ",", [left, right])
                continue
            p = BINPREC.get(x)
            if p is None or p < minp:
                break
            self.i += 1
            right = self.expr(p + 1)
            left = E("bin", left.a, right.b, sig, x, [left, right])
        return left

    def _is_type_range(self, a, b):
        sig = self.sig
        if b <= a:
            return False
        if not self.P.is_type_word(sig[a].t):
            # unknown typedef name: `(Name *)` / `(Name **)` cannot be an expression
            j = a
            while j < b and sig[j].k == "id" and sig[j].t not in KEYWORDS:
                j += 1
            if j > a and j < b and all(sig[q].t == "*" for q in range(j, b)) and \
                    (j - a == 1 or all(self.P.is_type_word(sig[q].t) for q in range(a, j - 1))):
                return True
            return False
        prev_tag = False
        for j in range(a, b):
            t = sig[j]
            if t.k == "id":
                if prev_tag:
                    prev_tag = False
                    continue
                if not self.P.is_type_word(t.t):
                    return False
                if t.t in ("struct", "union", "enum"):
                    prev_tag = True
            elif t.k == "op" and t.t in ("*", "(", ")", ",", "[", "]"):
                continue
            elif t.k == "num":
                continue
            else:
                return False
        return True

    def unary(self):
        sig = self.sig
        if self.i >= self.b:
            raise ParseError("operand")
        t = sig[self.i]
        i0 = self.i
        if t.k == "op" and t.t in ("-", "+", "!", "~", "*", "&", "++", "--"):
            self.i += 1
            operand = self.unary()
            return E("un", i0, operand.b, sig, t.t, [operand])
        if t.k == "id" and t.t == "sizeof":
            self.i += 1
            nt = self.cur()
            if nt is not None and nt.t == "(" and self.match[self.i] >= 0 and self._is_type_range(self.i + 1, self.match[self.i]):
                j = self.match[self.i]
                self.i = j + 1
                e = E("sizeof", i0, j + 1, sig)
                return self.postfix(e) if False else e
            operand = self.unary()
            return E("sizeof", i0, operand.b, sig, "sizeof", [operand])
        if t.k == "op" and t.t == "(":
            j = self.match[self.i]
            if j < 0 or j >= self.b:
                raise ParseError("paren")
            if self._is_type_range(self.i + 1, j) and j + 1 < self.b:
                nt = sig[j + 1]
                if not (nt.k == "op" and nt.t in (")", "]", ",", ";", "?", ":") or
                        (nt.k == "op" and nt.t in BINPREC and nt.t not in ("-", "+", "*", "&"))):
                    ta, tb = self.i + 1, j
                    self.i = j + 1
                    operand = self.unary()
                    e = E("cast", i0, operand.b, sig, "cast", [operand])
                    e.ta, e.tb = ta, tb
                    return e
            sub = ExprParser(self.P, self.i + 1, j)
            inner = sub.parse() if j > self.i + 1 else None
            if inner is None:
                raise ParseError("empty paren")
            self.i = j + 1
            e = E("paren", i0, j + 1, sig, "()", [inner])
            return self.postfix(e)
        if t.k == "num":
            self.i += 1
            return self.postfix(E("num", i0, i0 + 1, sig, name=t.t))
        if t.k in ("str", "chr"):
            self.i += 1
            return self.postfix(E("str", i0, i0 + 1, sig, name=t.t))
        if t.k == "id":
            self.i += 1
            return self.postfix(E("ident", i0, i0 + 1, sig, name=t.t))
        raise ParseError("unexpected %r" % t.t)

    def postfix(self, e):
        sig = self.sig
        while self.i < self.b:
            t = sig[self.i]
            if t.k != "op":
                break
            if t.t == "[":
                j = self.match[self.i]
                if j < 0 or j >= self.b:
                    raise ParseError("index")
                inner = ExprParser(self.P, self.i + 1, j).parse()
                self.i = j + 1
                e = E("idx", e.a, j + 1, sig, "[]", [e, inner])
            elif t.t == "(":
                j = self.match[self.i]
                if j < 0 or j >= self.b:
                    raise ParseError("call")
                args = []
                s = self.i + 1
                k = s
                while k <= j:
                    if k == j or (sig[k].t == "," and sig[k].k == "op"):
                        if k > s:
                            try:
                                args.append(ExprParser(self.P, s, k).parse())
                            except ParseError:
                                args.append(E("raw", s, k, sig))
                        s = k + 1
                        k += 1
                        continue
                    if sig[k].k == "op" and sig[k].t in ("(", "[", "{"):
                        k = self.match[k] + 1
                        continue
                    k += 1
                self.i = j + 1
                e = E("call", e.a, j + 1, sig, "()", [e] + args)
            elif t.t in (".", "->"):
                if self.i + 1 >= self.b or sig[self.i + 1].k != "id":
                    raise ParseError("member")
                nm = sig[self.i + 1].t
                self.i += 2
                e = E("mem", e.a, self.i, sig, t.t, [e], name=nm)
            elif t.t in ("++", "--"):
                self.i += 1
                e = E("post", e.a, self.i, sig, t.t, [e])
            else:
                break
        return e


def _link(e, parent=None):
    e.parent = parent
    for c in e.ch:
        _link(c, e)


def ewalk(e):
    yield e
    for c in e.ch:
        for x in ewalk(c):
            yield x


def strip_paren(e):
    while e.k == "paren":
        e = e.ch[0]
    return e


# --------------------------------------------------------------------------
# Mutation record
# --------------------------------------------------------------------------

class Mutation(object):
    __slots__ = ("name", "new_src", "cost", "target", "id", "fn", "desc", "flags", "edits")

    def __init__(self, name, new_src, cost, target, id, fn, desc, flags, edits):
        self.name, self.new_src, self.cost, self.target = name, new_src, cost, target
        self.id, self.fn, self.desc, self.flags, self.edits = id, fn, desc, tuple(flags), edits

    @property
    def semantic_risk(self):
        return "semantic_risk" in self.flags

    def to_dict(self, with_src=False, src=None):
        d = {"id": self.id, "name": self.name, "target": self.target, "cost": self.cost,
             "fn": self.fn, "flags": list(self.flags), "desc": self.desc}
        if with_src:
            d["new_src"] = self.new_src
        elif src is not None:
            d["diff"] = "".join(difflib.unified_diff(
                src.splitlines(True), self.new_src.splitlines(True), "a", "b", n=0))
        return d

    def __repr__(self):
        return "Mutation(%s cost=%d target=%s%s)" % (
            self.id, self.cost, self.target, " RISK" if self.semantic_risk else "")


# --------------------------------------------------------------------------
# per-function context
# --------------------------------------------------------------------------

RISK_COST = 3


class Ctx(object):
    def __init__(self, src, parsed, f, catalog, seen):
        self.src = src
        self.P = parsed
        self.sig = parsed.sig
        self.match = parsed.match
        self.f = f
        self.fn = f.name
        self.catalog = catalog
        self.seen = seen
        self.out = []
        self.body = None
        self.ok = True
        try:
            self.body = parse_body(parsed, f)
        except ParseError:
            self.ok = False
        self.nodes = list(walk(self.body)) if self.ok else []
        self._ecache = {}
        self._ident_cache = None
        self._analyse()

    # --- helpers --------------------------------------------------------
    def text(self, a, b):
        return self.P.text(a, b)

    def ntext(self, n):
        return self.src[n.s:n.e]

    def want(self, name):
        return self.catalog is None or name in self.catalog

    def expr(self, a, b):
        """Parsed expression of sig range [a,b) or None."""
        key = (a, b)
        if key in self._ecache:
            return self._ecache[key]
        try:
            e = ExprParser(self.P, a, b).parse()
        except (ParseError, IndexError, RecursionError):
            e = None
        self._ecache[key] = e
        return e

    def roots(self, n):
        """Expression token ranges directly owned by statement node n."""
        k = n.k
        d = n.d
        if k == "expr":
            return [d["r"]]
        if k in ("if", "while", "switch", "do"):
            return [d["cond"]]
        if k == "for":
            return [d["init"], d["cond"], d["step"]]
        if k == "return":
            a, b = d["r"]
            return [(a, b)] if b > a else []
        if k == "decl":
            return [dc["init"] for dc in d["info"]["decls"] if dc["init"]]
        return []

    def all_exprs(self):
        """[(E, stmt)] for every expression root; None entry when one failed."""
        out = []
        for n in self.nodes:
            for (a, b) in self.roots(n):
                if b <= a:
                    continue
                e = self.expr(a, b)
                out.append((e, n))
        return out

    def _analyse(self):
        self.locals = {}
        self.params = {}
        if not self.ok:
            return
        sig = self.sig
        f = self.f
        # parameters
        a, b = f.lp + 1, f.rp
        if b > a and not (b == a + 1 and sig[a].t == "void"):
            segs = []
            s = a
            j = a
            while j < b:
                if sig[j].k == "op" and sig[j].t in ("(", "[", "{"):
                    j = self.match[j] + 1
                    continue
                if sig[j].k == "op" and sig[j].t == ",":
                    segs.append((s, j))
                    s = j + 1
                j += 1
            segs.append((s, b))
            plist = []
            for (sa, sb) in segs:
                try:
                    info = StmtParser(self.P).parse_decl(sa, sb)
                except ParseError:
                    plist = None
                    break
                dc = info["decls"][0]
                if dc["complex"] or dc["name"] is None or info["base"][1] <= info["base"][0]:
                    plist = None
                    break
                plist.append((sa, sb, info, dc))
            self.plist = plist
            if plist:
                for (sa, sb, info, dc) in plist:
                    self.params[dc["name"]] = (info, dc)
        else:
            self.plist = []
        for n in self.nodes:
            if n.k == "decl":
                info = n.d["info"]
                for dc in info["decls"]:
                    if dc["name"]:
                        self.locals[dc["name"]] = (n, info, dc)
        self.volatile = set()
        for nm, (n, info, dc) in self.locals.items():
            if any(self.sig[q].t == "volatile" for q in range(*info["base"])):
                self.volatile.add(nm)
        for a2, b2 in self.P.items:
            if self.sig[a2].t == "typedef":
                continue
            if any(self.sig[q].t == "volatile" for q in range(a2, b2)):
                try:
                    info = StmtParser(self.P).parse_decl(a2, b2 - 1)
                except ParseError:
                    continue
                for dc in info["decls"]:
                    if dc["name"]:
                        self.volatile.add(dc["name"])

    # float-typed scalar / pointer names visible in the function
    def float_names(self):
        if hasattr(self, "_fn_cache"):
            return self._fn_cache
        scal, ptr = set(), set()

        def feed(info, dc):
            if dc["name"] is None:
                return
            base = [self.sig[q].t for q in range(*info["base"])]
            if not any(w in FLOAT_TYPES for w in base):
                return
            if dc["stars"] or dc["dims"]:
                ptr.add(dc["name"])
            else:
                scal.add(dc["name"])

        for nm, (info, dc) in self.params.items():
            feed(info, dc)
        for nm, (n, info, dc) in self.locals.items():
            feed(info, dc)
        for a2, b2 in self.P.items:
            if self.sig[a2].t == "typedef":
                continue
            try:
                info = StmtParser(self.P).parse_decl(a2, b2 - 1)
            except ParseError:
                continue
            if any(self.sig[q].k == "op" and self.sig[q].t == "(" for q in range(a2, b2)):
                continue
            for dc in info["decls"]:
                if dc["name"] and dc["name"] not in self.locals and dc["name"] not in self.params:
                    feed(info, dc)
        self._fn_cache = (scal, ptr)
        return self._fn_cache

    def idents_in(self, a, b, skip_members=True):
        out = set()
        sig = self.sig
        for j in range(a, b):
            t = sig[j]
            if t.k == "id":
                if skip_members and j > 0 and sig[j - 1].t in (".", "->") and sig[j - 1].k == "op":
                    continue
                out.add(t.t)
        return out

    def has_call(self, a, b):
        sig = self.sig
        for j in range(a, b - 1):
            t = sig[j]
            if (t.k == "id" and sig[j + 1].t == "(" and t.t not in PURE_CALLS and t.t not in KEYWORDS
                    and not self.P.is_type_word(t.t)):
                return True
        return False

    def has_sideeffect(self, a, b):
        if self.has_call(a, b):
            return True
        for j in range(a, b):
            t = self.sig[j]
            if t.k == "op" and (t.t in ASSIGN_OPS or t.t in ("++", "--")):
                return True
        return False

    SPELL = {"s32": "int", "s16": "short", "u8": "unsigned char", "s8": "signed char",
             "u16": "unsigned short", "u32": "unsigned int", "f32": "float"}

    def spell(self, canon):
        """Spelling of a fixed-width type that exists in this source."""
        if canon in self.P.types or canon not in self.SPELL:
            return canon
        return self.SPELL[canon]

    def line_start(self, pos):
        return self.src.rfind("\n", 0, pos) + 1

    def indent_of(self, pos):
        ls = self.line_start(pos)
        m = re.match(r"[ \t]*", self.src[ls:pos])
        return m.group()

    def del_stmt_edit(self, n):
        """Edit deleting statement node n (whole line when it is alone on it)."""
        s, e = n.s, n.e
        ls = self.line_start(s)
        if self.src[ls:s].strip() == "":
            le = self.src.find("\n", e)
            if le < 0:
                le = len(self.src)
            if self.src[e:le].strip() == "":
                return (ls, min(le + 1, len(self.src)), "")
        return (s, e, "")

    def fresh(self, base, numbered=False):
        if self._ident_cache is None:
            self._ident_cache = set(t.t for t in self.sig if t.k == "id")
        nm = base + ("1" if numbered else "")
        k = 2
        while nm in self._ident_cache:
            nm = "%s%d" % (base, k)
            k += 1
        self._ident_cache.add(nm)
        return nm

    # --- emit -----------------------------------------------------------
    def add(self, name, edits, cost, target, desc, flags=()):
        if not self.want(name):
            return
        edits = sorted(edits, key=lambda x: (x[0], x[1]))
        prev = -1
        for s, e, t in edits:
            if s < prev:
                return
            prev = max(prev, e)
            if not balanced(t):
                return
        out = []
        pos = 0
        src = self.src
        for s, e, t in edits:
            out.append(src[pos:s])
            out.append(t)
            pos = e
        out.append(src[pos:])
        new = "".join(out)
        if new == src or new in self.seen:
            return
        self.seen.add(new)
        flags = tuple(flags)
        if "semantic_risk" in flags:
            cost += RISK_COST
        h = hashlib.sha1(repr((name, self.fn, tuple(edits))).encode("utf8", "replace")).hexdigest()[:8]
        self.out.append(Mutation(name, new, cost, target, "%s@%s#%s" % (name, self.fn, h),
                                 self.fn, desc, flags, tuple(edits)))


# --------------------------------------------------------------------------
# generic helpers on statements / expressions
# --------------------------------------------------------------------------

def simple_update(ctx, st):
    """(name, op, rhsE) when st is `x = ..`, `x op= ..`, `x++`, `x--`, else None."""
    if st.k != "expr":
        return None
    e = ctx.expr(*st.d["r"])
    if e is None:
        return None
    if e.k == "assign" and e.ch[0].k == "ident":
        return (e.ch[0].name, e.op, e.ch[1])
    if e.k in ("post", "un") and e.op in ("++", "--") and e.ch[0].k == "ident":
        return (e.ch[0].name, e.op, None)
    return None


def stmt_text_nosemi(ctx, st):
    a, b = st.d["r"]
    return ctx.text(a, b)


def contains_ctl(ctx, node, kind):
    """Does node contain an (unnested) break / continue?"""
    def rec(n, in_loop, in_switch):
        if n.k == kind:
            if kind == "continue":
                return not in_loop
            return not (in_loop or in_switch)
        il = in_loop or n.k in ("while", "for", "do")
        isw = in_switch or n.k == "switch"
        for c in n.ch:
            if rec(c, il, isw):
                return True
        for key in ("then", "else", "body"):
            c = n.d.get(key)
            if isinstance(c, N) and rec(c, il, isw):
                return True
        return False
    return rec(node, False, False)


def has_label_or_case(node):
    for n in walk(node):
        if n.k in ("label", "case", "default"):
            return True
    return False


def const_eval(ctx, e, env):
    """Tiny integer evaluator; raises KeyError/ValueError when unknown."""
    e = strip_paren(e)
    if e.k == "num":
        t = e.name.rstrip("uUlL")
        if "." in t:
            raise ValueError
        return int(t, 0)
    if e.k == "ident":
        return env[e.name]
    if e.k == "un" and e.op == "-":
        return -const_eval(ctx, e.ch[0], env)
    if e.k == "un" and e.op == "!":
        return int(not const_eval(ctx, e.ch[0], env))
    if e.k == "bin":
        a = const_eval(ctx, e.ch[0], env)
        b = const_eval(ctx, e.ch[1], env)
        ops = {
            "<": a < b, ">": a > b, "<=": a <= b, ">=": a >= b, "==": a == b, "!=": a != b,
            "+": a + b, "-": a - b, "*": a * b, "&&": int(bool(a) and bool(b)),
            "||": int(bool(a) or bool(b)),
        }
        if e.op in ops:
            return int(ops[e.op]) if e.op in ("<", ">", "<=", ">=", "==", "!=") else ops[e.op]
        raise ValueError
    raise ValueError


def entry_safe(ctx, inits, cond_range):
    """True when the loop condition is provably true after the init statements
    (so do-while <-> for/while keeps semantics); None when unknown."""
    env = {}
    for st in inits:
        su = simple_update(ctx, st)
        if not su or su[1] != "=":
            return None
        try:
            env[su[0]] = const_eval(ctx, su[2], env)
        except (KeyError, ValueError):
            pass
    if cond_range[1] <= cond_range[0]:
        return True
    ce = ctx.expr(*cond_range)
    if ce is None:
        return None
    # evaluate only over the clause parts we can: full condition must be known
    try:
        return bool(const_eval(ctx, ce, env))
    except (KeyError, ValueError):
        # partially known conjunction: require every known conjunct true and
        # every unknown conjunct to involve only unknown variables -> unknown
        return None


def step_canon(ctx, st):
    """Canonical step text for a simple update statement."""
    su = simple_update(ctx, st)
    txt = stmt_text_noesemi = ctx.text(*st.d["r"])
    if not su:
        return txt
    name, op, rhs = su
    if op in ("++", "--"):
        return txt
    if rhs is not None and rhs.k == "num" and rhs.name in ("1", "1U", "1u"):
        if op == "+=":
            return name + "++"
        if op == "-=":
            return name + "--"
    if op == "=" and rhs is not None and rhs.k == "bin" and rhs.op in ("+", "-"):
        l, r = rhs.ch
        if l.k == "ident" and l.name == name and r.k == "num" and r.name == "1":
            return name + ("++" if rhs.op == "+" else "--")
    return txt


def absorb_loop_parts(ctx, n, body, cond_range):
    """Find (inits, steps, reason-flags) to fold into a for header.

    inits: preceding sibling `x = e;` statements; steps: trailing `x++`-like
    statements of `body`. Both restricted to names used by the condition (or
    each other).
    """
    cond_ids = ctx.idents_in(*cond_range) if cond_range[1] > cond_range[0] else set()
    steps = []
    if body.k == "block":
        for st in reversed(body.ch):
            su = simple_update(ctx, st)
            if su is None:
                break
            rhs = su[2]
            if rhs is not None and ctx.has_call(rhs.a, rhs.b):
                break
            steps.insert(0, st)
    names = set(cond_ids)
    inits = []
    par = n.parent
    if par is not None and par.k == "block" and n.pos > 0:
        for _round in range(3):
            inits = []
            for st in reversed(par.ch[:n.pos]):
                su = simple_update(ctx, st)
                if su is None or su[1] != "=":
                    break
                if su[0] not in names:
                    break
                inits.insert(0, st)
            # a step statement is kept only if its variable is in cond or init
            initn = set(simple_update(ctx, s)[0] for s in inits)
            ok_steps = []
            for st in steps:
                pass
            new_names = set(cond_ids) | set(simple_update(ctx, s)[0] for s in steps)
            if new_names == names:
                break
            names = new_names
    # trim steps whose variable is neither in cond nor initialised
    initn = set(simple_update(ctx, s)[0] for s in inits)
    keep = []
    for st in reversed(steps):
        nm = simple_update(ctx, st)[0]
        if nm in cond_ids or nm in initn:
            keep.insert(0, st)
        else:
            break
    return inits, keep


def body_without_trailing(ctx, body, steps):
    """Text of block `body` with the trailing step statements removed."""
    if not steps:
        return ctx.ntext(body)
    first = steps[0]
    kept = [c for c in body.ch if c.pos < first.pos]
    src = ctx.src
    close = body.e - 1  # the '}'
    p = close
    while p > body.s and src[p - 1] in " \t":
        p -= 1
    tail = src[p:body.e]
    # keep the line break before '}' if the original had one
    if src[close - 1:close] not in ("\n",):
        ws_start = close
        while ws_start > body.s and src[ws_start - 1] in " \t\n":
            ws_start -= 1
        tail_ws = src[ws_start:close]
        tail = tail_ws + "}"
    else:
        tail = src[p:body.e]
    head_end = kept[-1].e if kept else body.s + 1
    return src[body.s:head_end] + (tail if kept or "\n" in tail else " }")


def wrap_block_text(ctx, n):
    """Text of statement n as a braced block (adds braces to single statements)."""
    if n.k == "block":
        return ctx.ntext(n)
    return "{ " + ctx.ntext(n) + " }"


# --------------------------------------------------------------------------
# families: loops
# --------------------------------------------------------------------------

def fam_loops(ctx):
    if not ctx.ok:
        return
    src = ctx.src
    for n in ctx.nodes:
        if n.k == "do":
            body = n.d["body"]
            cond = n.d["cond"]
            ctext = ctx.text(*cond)
            if body.k == "block":
                inits, steps = absorb_loop_parts(ctx, n, body, cond)
                if steps and contains_ctl(ctx, body, "continue"):
                    steps = []
                    inits_c = inits
                safe = entry_safe(ctx, inits, cond) if inits else None
                flags = () if safe else ("semantic_risk",)
                if ctx.want("loop_do_to_for"):
                    start = inits[0].s if inits else n.s
                    if inits:
                        ls = ctx.line_start(start)
                        if src[ls:start].strip() == "":
                            start = ls + len(src[ls:start]) - len(src[ls:start].lstrip())
                            start = inits[0].s
                    itext = ", ".join(stmt_text_noesemi(ctx, s) for s in inits)
                    stexts = [ctx.text(*s.d["r"]) for s in steps]
                    scanon = [step_canon(ctx, s) for s in steps]
                    bt = body_without_trailing(ctx, body, steps)
                    for label, sts in (("canon", scanon), ("verbatim", stexts)):
                        if label == "verbatim" and sts == scanon:
                            continue
                        header = "for (%s; %s; %s)" % (itext, ctext, ", ".join(sts))
                        if not itext and not sts[:1]:
                            header = "for (; %s; )" % ctext
                        ctx.add("loop_do_to_for", [(start, n.e, header + " " + bt)], 3, "loop",
                                "do/while -> for (init=%d step=%d, %s)" % (len(inits), len(steps), label), flags)
                    # also: for without absorbing statements from the body
                    if steps or inits:
                        header = "for (; %s; )" % ctext
                        ctx.add("loop_do_to_for", [(n.s, n.e, header + " " + ctx.ntext(body))], 3, "loop",
                                "do/while -> for(;cond;) plain", ("semantic_risk",) if not inits else flags)
                if ctx.want("loop_do_to_while"):
                    if inits:
                        safe2 = entry_safe(ctx, inits, cond)
                    else:
                        safe2 = None
                    ctx.add("loop_do_to_while", [(n.s, n.e, "while (%s) %s" % (ctext, ctx.ntext(body)))],
                            3, "loop", "do/while -> while (entry test added)",
                            () if safe2 else ("semantic_risk",))
                if ctx.want("loop_to_goto") and not contains_ctl(ctx, body, "break") and \
                        not contains_ctl(ctx, body, "continue"):
                    lab = ctx.fresh("loop_", True)
                    ind = ctx.indent_of(n.s)
                    txt = "%s: %s\n%sif (%s) goto %s;" % (lab, ctx.ntext(body), ind, ctext, lab)
                    ctx.add("loop_to_goto", [(n.s, n.e, txt)], 4, "loop", "do/while -> goto loop")
        elif n.k == "for":
            body = n.d["body"]
            init, cond, step = n.d["init"], n.d["cond"], n.d["step"]
            ctext = ctx.text(*cond) if cond[1] > cond[0] else "1"
            has_cont = contains_ctl(ctx, body, "continue")
            if has_cont and step[1] > step[0]:
                continue
            par = n.parent
            in_block = par is not None and par.k == "block"
            istmts = _split_commas(ctx, init)
            sstmts = _split_commas(ctx, step)
            ind = ctx.indent_of(n.s)

            def pieces(body_text_with_steps_kind):
                pre = "".join("%s%s;\n" % (ind, t) for t in istmts) if in_block else ""
                return pre

            # body with steps appended
            if body.k == "block":
                close = body.e - 1
                ws = close
                while ws > body.s and src[ws - 1] in " \t\n":
                    ws -= 1
                inner = src[body.s:ws]
                bind = ind + "  "
                steps_txt = "".join("\n%s%s;" % (bind, t) for t in sstmts)
                newbody = inner + steps_txt + src[ws:body.e]
            else:
                bind = ind + "  "
                newbody = "{\n%s%s%s;\n%s}" % (bind, ctx.ntext(body),
                                               "".join("\n%s%s;" % (bind, t) for t in sstmts), ind)
            if ctx.want("loop_for_to_while"):
                pre = "".join("%s;\n%s" % (t, ind) for t in istmts)
                txt = "%swhile (%s) %s" % (pre, ctext, newbody)
                if not in_block and istmts:
                    txt = "{ " + txt + " }"
                ctx.add("loop_for_to_while", [(n.s, n.e, txt)], 3, "loop", "for -> while")
            if ctx.want("loop_for_to_do"):
                safe = entry_safe(ctx, [_fake_stmt(ctx, t) for t in istmts], cond) if False else None
                safe = _entry_safe_texts(ctx, istmts, cond)
                pre = "".join("%s;\n%s" % (t, ind) for t in istmts)
                txt = "%sdo %s while (%s);" % (pre, newbody, ctext)
                if not in_block and istmts:
                    txt = "{ " + txt + " }"
                ctx.add("loop_for_to_do", [(n.s, n.e, txt)], 3, "loop", "for -> do/while",
                        () if safe else ("semantic_risk",))
        elif n.k == "while":
            body = n.d["body"]
            cond = n.d["cond"]
            ctext = ctx.text(*cond)
            if ctx.want("loop_while_to_for"):
                ctx.add("loop_while_to_for", [(n.s, n.e, "for (; %s; ) %s" % (ctext, ctx.ntext(body)))],
                        2, "loop", "while -> for(;cond;)")
                if body.k == "block":
                    inits, steps = absorb_loop_parts(ctx, n, body, cond)
                    if steps and contains_ctl(ctx, body, "continue"):
                        steps = []
                    if inits or steps:
                        start = inits[0].s if inits else n.s
                        itext = ", ".join(ctx.text(*s.d["r"]) for s in inits)
                        sts = ", ".join(step_canon(ctx, s) for s in steps)
                        bt = body_without_trailing(ctx, body, steps)
                        ctx.add("loop_while_to_for",
                                [(start, n.e, "for (%s; %s; %s) %s" % (itext, ctext, sts, bt))],
                                3, "loop", "while -> for (init/step absorbed)")
        elif n.k == "label":
            _goto_to_do(ctx, n)


def stmt_text_noesemi(ctx, st):
    return ctx.text(*st.d["r"])


def _split_commas(ctx, rng):
    a, b = rng
    if b <= a:
        return []
    out = []
    s = a
    j = a
    sig = ctx.sig
    while j <= b:
        if j == b or (sig[j].t == "," and sig[j].k == "op"):
            out.append(ctx.text(s, j))
            s = j + 1
            j += 1
            continue
        if sig[j].k == "op" and sig[j].t in ("(", "[", "{"):
            j = ctx.match[j] + 1
            continue
        j += 1
    return out


def _entry_safe_texts(ctx, istmts, cond):
    """entry_safe over init statements given as text (for `for` headers)."""
    env = {}
    for t in istmts:
        m = re.match(r"^\s*([A-Za-z_]\w*)\s*=\s*(-?(?:0[xX][0-9a-fA-F]+|\d+))\s*$", t)
        if not m:
            continue
        env[m.group(1)] = int(m.group(2), 0)
    if cond[1] <= cond[0]:
        return True
    ce = ctx.expr(*cond)
    if ce is None:
        return None
    try:
        return bool(const_eval(ctx, ce, env))
    except (KeyError, ValueError):
        return None


def _fake_stmt(ctx, t):
    return None


def _goto_to_do(ctx, lab):
    """label: stmts...; if (cond) goto label;  ->  do { stmts } while (cond);"""
    par = lab.parent
    if par is None or par.k != "block":
        return
    if not ctx.want("loop_from_goto"):
        return
    name = lab.d["name"]
    refs = [n for n in ctx.nodes if n.k == "goto" and n.d["name"] == name]
    if len(refs) != 1:
        return
    # find the sibling `if (c) goto name;`
    for q in range(lab.pos + 1, len(par.ch)):
        st = par.ch[q]
        if st.k == "if" and st.d["else"] is None:
            th = st.d["then"]
            g = th
            if th.k == "block" and len(th.ch) == 1:
                g = th.ch[0]
            if g is refs[0]:
                region = par.ch[lab.pos + 1:q]
                if not region:
                    return
                if any(r.k in ("decl", "label", "case", "default", "pp") or has_label_or_case(r) for r in region):
                    return
                fake = N.__new__(N)
                for r in region:
                    if contains_ctl(ctx, r, "break") or contains_ctl(ctx, r, "continue"):
                        return
                ctext = ctx.text(*st.d["cond"])
                ind = ctx.indent_of(lab.s)
                inner = ctx.src[region[0].s:region[-1].e]
                txt = "do {\n%s  %s\n%s} while (%s);" % (ind, inner, ind, ctext)
                ctx.add("loop_from_goto", [(lab.s, st.e, txt)], 4, "loop", "goto loop -> do/while")
                return
        if has_label_or_case(st):
            return


# --------------------------------------------------------------------------
# family: counter type
# --------------------------------------------------------------------------

def _base_type_token(ctx, info):
    for q in range(*info["base"]):
        if ctx.sig[q].t in INT_TYPES:
            return q
    return None


def split_decl_with_types(ctx, n, newtypes):
    """Text replacing decl node n where declarator index -> new type word."""
    info = n.d["info"]
    sig = ctx.sig
    base_a, base_b = info["base"]
    bq = _base_type_token(ctx, info)
    if bq is None:
        return None
    decls = info["decls"]
    if len(decls) == 1:
        (idx, nt), = newtypes.items()
        t = sig[bq]
        return [(t.s, t.e, nt)]
    # split into separate statements preserving declarator order
    ind = ctx.indent_of(n.s)
    base_txt = ctx.text(base_a, base_b)
    parts = []
    cur = []
    for i, dc in enumerate(decls):
        if i in newtypes:
            if cur:
                parts.append((base_txt, cur))
                cur = []
            t = sig[bq]
            nb = ctx.text(base_a, bq) + (" " if bq > base_a else "") + newtypes[i] + " " + \
                ctx.text(bq + 1, base_b) if bq + 1 < base_b else \
                ctx.text(base_a, bq) + (" " if bq > base_a else "") + newtypes[i]
            parts.append((nb.strip(), [dc]))
        else:
            cur.append(dc)
    if cur:
        parts.append((base_txt, cur))
    stmts = []
    for bt, dcs in parts:
        stmts.append("%s %s;" % (bt, ", ".join(ctx.text(dc["ds"], dc["b"]) for dc in dcs)))
    return [(n.s, n.e, ("\n" + ind).join(stmts))]


def fam_counter_type(ctx):
    if not ctx.ok or not ctx.want("counter_type"):
        return
    names = set()
    for n in ctx.nodes:
        if n.k in ("do", "while", "for"):
            names |= ctx.idents_in(*n.d["cond"])
            if n.k == "for":
                names |= ctx.idents_in(*n.d["step"])
    for nm in sorted(names):
        if nm not in ctx.locals:
            continue
        dn, info, dc = ctx.locals[nm]
        if dc["stars"] or dc["dims"] or dc["complex"] or dc["init"]:
            continue
        bq = _base_type_token(ctx, info)
        if bq is None:
            continue
        cur = ctx.sig[bq].t
        if any(ctx.sig[q].t in ("unsigned", "signed") for q in range(*info["base"])):
            continue
        if cur in ("s32", "int", "long", "u32"):
            targets = [ctx.spell("s16")]
        elif cur in ("s16", "s8", "u8", "u16", "short", "char"):
            targets = [ctx.spell("s32")]
        else:
            continue
        for tgt in targets:
            if tgt == cur:
                continue
            idx = info["decls"].index(dc)
            edits = split_decl_with_types(ctx, dn, {idx: tgt})
            if edits is None:
                continue
            narrowing = tgt in ("s16", "short")
            ctx.add("counter_type", edits, 2, "loop",
                    "loop counter %s: %s -> %s" % (nm, cur, tgt),
                    ("semantic_risk",) if narrowing else ())


# --------------------------------------------------------------------------
# family: named local inline
# --------------------------------------------------------------------------

def _addr_taken(ctx, exprs, name):
    for e, st in exprs:
        if e is None:
            continue
        for x in ewalk(e):
            if x.k == "un" and x.op == "&":
                t = strip_paren(x.ch[0])
                while t.k in ("idx", "mem"):
                    t = strip_paren(t.ch[0])
                if t.k == "ident" and t.name == name:
                    return True
    return False


def _is_atom(e):
    e = e
    if e.k in ("ident", "num", "str", "paren", "call", "idx", "mem", "post"):
        return True
    if e.k == "un":
        return _is_atom_chain(e)
    if e.k == "cast":
        return _is_atom_chain(e)
    return False


def _is_atom_chain(e):
    while e.k in ("un", "cast"):
        if e.k == "un" and e.op in ("++", "--"):
            return False
        e = e.ch[0]
    return e.k in ("ident", "num", "str", "paren", "call", "idx", "mem", "post")


def fam_local_inline(ctx):
    if not ctx.ok or not ctx.want("local_inline"):
        return
    exprs = ctx.all_exprs()
    if any(e is None for e, st in exprs):
        return
    # writes per ident
    assigns = {}
    other_writes = {}
    for e, st in exprs:
        for x in ewalk(e):
            if x.k == "assign" and strip_paren(x.ch[0]).k == "ident":
                nm = strip_paren(x.ch[0]).name
                if x.op == "=" and x.parent is None and st.k == "expr" and x is e:
                    assigns.setdefault(nm, []).append((x, st))
                else:
                    other_writes[nm] = other_writes.get(nm, 0) + 1
            elif x.k in ("un", "post") and x.op in ("++", "--") and strip_paren(x.ch[0]).k == "ident":
                nm = strip_paren(x.ch[0]).name
                other_writes[nm] = other_writes.get(nm, 0) + 1
    for nm in sorted(ctx.locals):
        dn, info, dc = ctx.locals[nm]
        if dc["dims"] or dc["complex"] or dc["init"] or nm in ctx.volatile:
            continue
        if nm not in assigns or len(assigns[nm]) != 1 or other_writes.get(nm):
            continue
        if _addr_taken(ctx, exprs, nm):
            continue
        ae, ast = assigns[nm][0]
        rhs = ae.ch[1]
        # uses: ident tokens (not member names), excluding the LHS of the def
        uses = []
        sig = ctx.sig
        for q in range(ctx.f.bo, ctx.f.bc):
            t = sig[q]
            if t.k == "id" and t.t == nm and not (sig[q - 1].t in (".", "->") and sig[q - 1].k == "op"):
                uses.append(q)
        decl_toks = set(range(dc["nm"], dc["nm"] + 1))
        lhs_tok = ae.ch[0].a
        uses = [q for q in uses if q != lhs_tok and q not in decl_toks]
        if not uses:
            continue
        if any(q < ast.a for q in uses):
            continue
        par = ast.parent
        if par is None or par.k != "block":
            continue
        if any(not (par.a < q < par.b) for q in uses):
            continue
        if ctx.has_sideeffect(rhs.a, rhs.b) and not ctx.has_call(rhs.a, rhs.b) and False:
            continue
        rhs_ids = ctx.idents_in(rhs.a, rhs.b)
        risk = False
        calls = ctx.has_call(rhs.a, rhs.b)
        if any(x.k == "assign" or (x.k in ("un", "post") and x.op in ("++", "--")) for x in ewalk(rhs)):
            risk = True
        last_use = max(uses)
        first_use = min(uses)
        if calls and (len(uses) > 1):
            risk = True
        # clobbers between def and last use
        between_nodes = [m for m in ctx.nodes if m.k == "expr" and ast.e <= m.s and m.e <= sig[last_use].e]
        loads = any(x.k in ("idx", "mem") or (x.k == "un" and x.op == "*") for x in ewalk(rhs))
        globals_used = [i for i in rhs_ids if i not in ctx.locals and i not in ctx.params
                        and not ctx.P.is_type_word(i)]
        for m in between_nodes:
            me = ctx.expr(*m.d["r"])
            if me is None:
                risk = True
                continue
            for x in ewalk(me):
                if x.k == "call" and strip_paren(x.ch[0]).name not in PURE_CALLS:
                    if calls or loads or globals_used:
                        risk = True
                if x.k == "assign" or (x.k in ("un", "post") and x.op in ("++", "--")):
                    lhs = strip_paren(x.ch[0])
                    if lhs.k == "ident":
                        if lhs.name in rhs_ids:
                            risk = True
                        if lhs.name in globals_used and (loads or True) and lhs.name not in ctx.locals:
                            risk = True
                    elif loads or globals_used:
                        risk = True
        # writes of rhs variables inside an enclosing loop (value changes per iteration)
        anc = par
        in_loop = False
        while anc is not None:
            if anc.k in ("for", "while", "do"):
                in_loop = True
            anc = anc.parent
        for rid in rhs_ids:
            if other_writes.get(rid) or len(assigns.get(rid, [])) > 1:
                if in_loop:
                    risk = True
        # first use must come after at least... (uses inside nested conditionals is fine)
        rtxt = ctx.text(rhs.a, rhs.b)
        edits = [ctx.del_stmt_edit(ast)]
        # decl edit
        if len(info["decls"]) == 1:
            edits.append(ctx.del_stmt_edit(dn))
        else:
            idx = info["decls"].index(dc)
            segs = info["segs"]
            sa, sb = segs[idx]
            if idx + 1 < len(segs):
                edits.append((ctx.sig[dc["ds"]].s, ctx.sig[segs[idx + 1][0]].s, ""))
            else:
                edits.append((ctx.sig[segs[idx - 1][1]].s, ctx.sig[sb - 1].e, ""))
        for q in uses:
            t = sig[q]
            nxt = sig[q + 1].t if q + 1 < len(sig) else ""
            if rhs.k in ("ident", "num", "paren", "call", "idx", "mem", "post", "str"):
                atom = True
            elif rhs.k in ("un", "cast") and _is_atom_chain(rhs) and nxt not in ("(", "[", ".", "->", "++", "--"):
                atom = True
            else:
                atom = False
            txt = rtxt if atom else "(" + rtxt + ")"
            prevc = ctx.src[t.s - 1:t.s]
            if txt[:1] in "-+&*" and prevc == txt[:1]:
                txt = "(" + txt + ")"
            edits.append((t.s, t.e, txt))
        ctx.add("local_inline", edits, 3, "frame",
                "inline local %s (assigned once, %d use%s)" % (nm, len(uses), "s" if len(uses) > 1 else ""),
                ("semantic_risk",) if risk else ())


# --------------------------------------------------------------------------
# family: declaration order, pad locals
# --------------------------------------------------------------------------

def _top_decls(ctx):
    if not ctx.ok:
        return []
    out = []
    for st in ctx.body.ch:
        if st.k == "decl":
            out.append(st)
        elif st.k == "pp":
            continue
        else:
            break
    return out


def _perms(n):
    """Deterministic list of index permutations for n items."""
    res = []
    ident = tuple(range(n))
    seen = {ident}

    def push(p):
        p = tuple(p)
        if p not in seen:
            seen.add(p)
            res.append(p)
    for i in range(n - 1):
        p = list(ident)
        p[i], p[i + 1] = p[i + 1], p[i]
        push(p)
    for i in range(n):
        p = [x for x in ident if x != i]
        push([i] + p)
        push(p + [i])
    push(reversed(ident))
    if n <= 4:
        for p in itertools.permutations(ident):
            push(p)
    return res


def fam_decl_order(ctx):
    if not ctx.ok or not ctx.want("decl_order"):
        return
    decls = _top_decls(ctx)
    local_names = set()
    for d in decls:
        for dc in d.d["info"]["decls"]:
            if dc["name"]:
                local_names.add(dc["name"])
    # statement-level permutations
    movable = []
    for d in decls:
        info = d.d["info"]
        bad = False
        for dc in info["decls"]:
            if dc["complex"]:
                bad = True
            if dc["init"] and ctx.idents_in(*dc["init"]) & local_names:
                bad = True
        movable.append(not bad)
    if len(decls) >= 2 and all(movable):
        texts = [ctx.ntext(d) for d in decls]
        for p in _perms(len(decls)):
            edits = [(decls[k].s, decls[k].e, texts[p[k]]) for k in range(len(decls)) if p[k] != k]
            ctx.add("decl_order", edits, 2, "frame", "declaration order %s" % (list(p),))
    # declarator permutations inside a comma list
    for d in decls:
        info = d.d["info"]
        dcs = info["decls"]
        if len(dcs) < 2 or any(dc["complex"] for dc in dcs):
            continue
        if any(dc["init"] and ctx.idents_in(*dc["init"]) & local_names for dc in dcs):
            continue
        base_txt = ctx.text(*info["base"])
        dtexts = [ctx.text(dc["ds"], dc["b"]) for dc in dcs]
        for p in _perms(len(dcs)):
            txt = "%s %s;" % (base_txt, ", ".join(dtexts[i] for i in p))
            ctx.add("decl_order", [(d.s, d.e, txt)], 2, "frame", "declarator order in one declaration %s" % (list(p),))


def fam_pad_local(ctx):
    if not ctx.ok or not ctx.want("pad_local"):
        return
    decls = _top_decls(ctx)
    bo = ctx.sig[ctx.f.bo]
    first_pos = decls[0].s if decls else None
    ind = "  "
    if decls:
        ind = ctx.indent_of(decls[0].s) or "  "
    else:
        # indentation of first statement or 2 spaces
        if ctx.body.ch:
            ind = ctx.indent_of(ctx.body.ch[0].s) or "  "
    pad = ctx.fresh("pad")
    w32 = ctx.spell("s32")
    variants = [("%s %s;" % (w32, pad), "scalar"), ("volatile %s %s[1];" % (w32, pad), "v1"),
                ("volatile %s %s[2];" % (w32, pad), "v2"), ("volatile %s %s[4];" % (w32, pad), "v4")]
    for txt, tag in variants:
        if first_pos is not None:
            ctx.add("pad_local", [(first_pos, first_pos, txt + "\n" + ind)], 2, "frame",
                    "add %s first (highest address)" % tag)
            last = decls[-1]
            ctx.add("pad_local", [(last.e, last.e, "\n" + ind + txt)], 2, "frame",
                    "add %s last (lowest address)" % tag)
        else:
            ctx.add("pad_local", [(bo.e, bo.e, "\n" + ind + txt)], 2, "frame", "add %s" % tag)
    # remove / resize existing pads and unused locals
    uses = {}
    for q in range(ctx.f.bo + 1, ctx.f.bc):
        t = ctx.sig[q]
        if t.k == "id":
            uses[t.t] = uses.get(t.t, 0) + 1
    for d in decls:
        info = d.d["info"]
        dcs = info["decls"]
        for i, dc in enumerate(dcs):
            nm = dc["name"]
            if not nm or dc["complex"]:
                continue
            declared_uses = 1
            if uses.get(nm, 0) == declared_uses and not dc["init"]:
                if len(dcs) == 1:
                    ctx.add("pad_local", [ctx.del_stmt_edit(d)], 2, "frame", "remove unused local %s" % nm)
            if re.match(r"^(pad|new_var|unused|dummy)\w*$", nm) and dc["dims"] and len(dc["dims"]) == 1 and len(dcs) == 1:
                o, c = dc["dims"][0]
                inner = ctx.expr(o + 1, c - 1)
                if inner is not None and inner.k == "num":
                    try:
                        cur = int(inner.name, 0)
                    except ValueError:
                        continue
                    sizes = sorted(set([1, 2, 3, 4, cur - 1, cur + 1, cur - 2, cur + 2, cur - 4, cur + 4, cur // 2, cur * 2]))
                    for sz in sizes:
                        if sz >= 1 and sz != cur:
                            t = ctx.sig[o + 1]
                            ctx.add("pad_local", [(t.s, t.e, str(sz))], 2, "frame",
                                    "resize %s[%d] -> [%d]" % (nm, cur, sz))


# --------------------------------------------------------------------------
# file-level families: extern toggle, K&R prototypes
# --------------------------------------------------------------------------

def body_idents(ctx):
    return ctx.idents_in(ctx.f.bo, ctx.f.bc) | ctx.idents_in(ctx.f.lp, ctx.f.rp)


def fam_extern_toggle(ctx):
    if not ctx.want("extern_toggle"):
        return
    sig = ctx.sig
    used = body_idents(ctx)
    sp = StmtParser(ctx.P)
    # count top-level declarator names to avoid double-declared symbols
    name_count = {}
    parsed_items = []
    for a, b in ctx.P.items:
        if sig[a].t in ("typedef",):
            continue
        if any(sig[q].k == "op" and sig[q].t == "(" for q in range(a, b - 1)):
            continue
        if sig[b - 2].t == "}":
            continue
        try:
            info = sp.parse_decl(a, b - 1)
        except ParseError:
            continue
        if any(dc["complex"] or dc["name"] is None for dc in info["decls"]):
            continue
        parsed_items.append((a, b, info))
        for dc in info["decls"]:
            name_count[dc["name"]] = name_count.get(dc["name"], 0) + 1
    for a, b, info in parsed_items:
        names = [dc["name"] for dc in info["decls"]]
        if not any(nm in used for nm in names):
            continue
        if any(name_count[nm] > 1 for nm in names):
            continue
        first = sig[a]
        if any(dc["init"] for dc in info["decls"]):
            continue
        if first.t == "extern":
            nxt = sig[a + 1]
            ctx.add("extern_toggle", [(first.s, nxt.s, "")], 2, "linkage",
                    "extern -> defined: %s" % ",".join(names),
                    ("semantic_risk",) if any(dc["dims"] and all(
                        (ctx.sig[d[0] + 1].t == "]") for d in dc["dims"]) for dc in info["decls"]) else ())
        elif first.t not in STORAGE and all(ctx.P.is_type_word(sig[q].t) or sig[q].k in ("id", "op") for q in range(a, b)):
            if sig[a].t in ("struct", "union", "enum") and b - a == 3:
                continue
            ctx.add("extern_toggle", [(first.s, first.s, "extern ")], 2, "linkage",
                    "defined -> extern: %s" % ",".join(names))


def fam_knr_proto(ctx):
    if not ctx.want("knr_proto"):
        return
    sig = ctx.sig
    callees = set()
    for q in range(ctx.f.bo, ctx.f.bc):
        t = sig[q]
        if t.k == "id" and sig[q + 1].t == "(" and t.t not in KEYWORDS and t.t not in PURE_CALLS \
                and not ctx.P.is_type_word(t.t):
            callees.add(t.t)
    callees.discard(ctx.fn)
    for a, b in ctx.P.items:
        if sig[a].t in ("typedef",):
            continue
        # prototype: ... name ( params ) ;
        if sig[b - 2].t != ")":
            continue
        lp = ctx.match[b - 2]
        if lp <= a or sig[lp - 1].k != "id":
            continue
        nm = sig[lp - 1].t
        if nm not in callees:
            continue
        if any(sig[q].t == "=" for q in range(a, lp)):
            continue
        if sig[lp - 2].t == "(" or sig[lp - 1].t in TYPE_WORDS:
            continue
        inner = ctx.text(lp + 1, b - 2).strip()
        if inner == "":
            continue
        risk = any(w in FLOAT_TYPES for w in re.findall(r"\w+", inner))
        ctx.add("knr_proto", [(sig[lp].e, sig[b - 2].s, "")], 1, "callsetup",
                "K&R prototype for %s" % nm, ("semantic_risk",) if risk else ())


# --------------------------------------------------------------------------
# family: pointer laundering
# --------------------------------------------------------------------------

def fam_ptr_launder(ctx):
    if not ctx.ok or not ctx.want("ptr_launder"):
        return
    sig = ctx.sig

    def ptr_type_text(nm):
        if nm in ctx.locals:
            dn, info, dc = ctx.locals[nm]
        elif nm in ctx.params:
            info, dc = ctx.params[nm]
        else:
            return None
        if dc["stars"] < 1 or dc["dims"] or dc["complex"]:
            return None
        base = ctx.text(*info["base"])
        base = re.sub(r"\b(volatile|const|register|static)\s+", "", base)
        return "%s %s" % (base, "*" * dc["stars"])

    def launder(rhs, ty):
        inner = strip_paren(rhs)
        # already (T *)(u32)X  ->  remove the laundering
        if rhs.k == "cast" and rhs.ch[0].k == "cast" and ctx.text(rhs.ch[0].ta, rhs.ch[0].tb).strip() in ("u32", "unsigned int"):
            return [(rhs.s, rhs.e, ctx.text(rhs.ch[0].ch[0].a, rhs.ch[0].ch[0].b))], "remove (u32) laundering"
        if rhs.k == "cast" and rhs.ch[0].k == "cast":
            pass
        txt = ctx.text(rhs.a, rhs.b)
        if rhs.k == "cast":
            inner_txt = ctx.text(rhs.ch[0].a, rhs.ch[0].b)
            ty2 = ctx.text(rhs.ta, rhs.tb)
            if not _is_atom_chain(rhs.ch[0]):
                inner_txt = "(" + inner_txt + ")"
            return [(rhs.s, rhs.e, "(%s)(%s)%s" % (ty2, ctx.spell("u32"), inner_txt))], "launder pointer through (u32)"
        if not _is_atom_chain(rhs):
            txt = "(" + txt + ")"
        return [(rhs.s, rhs.e, "(%s)(%s)%s" % (ty, ctx.spell("u32"), txt))], "launder pointer through (u32)"

    for n in ctx.nodes:
        for (a, b) in ctx.roots(n):
            if b <= a or n.k not in ("expr", "decl"):
                continue
            if n.k == "expr":
                e = ctx.expr(a, b)
                if e is None or e.k != "assign" or e.op != "=" or e.ch[0].k != "ident":
                    continue
                ty = ptr_type_text(e.ch[0].name)
                if ty is None:
                    continue
                edits, d = launder(e.ch[1], ty)
                ctx.add("ptr_launder", edits, 2, "order", "%s (%s)" % (d, e.ch[0].name))
            else:
                for dc in n.d["info"]["decls"]:
                    if dc["init"] and dc["stars"] and not dc["dims"] and dc["name"]:
                        e = ctx.expr(*dc["init"])
                        if e is None:
                            continue
                        ty = ptr_type_text(dc["name"])
                        if ty is None:
                            continue
                        edits, d = launder(e, ty)
                        ctx.add("ptr_launder", edits, 2, "order", "%s (%s init)" % (d, dc["name"]))


# --------------------------------------------------------------------------
# family: parameter types
# --------------------------------------------------------------------------

def fam_param_type(ctx):
    if not ctx.want("param_type"):
        return
    plist = getattr(ctx, "plist", None)
    if not plist:
        return
    sig = ctx.sig
    narrow_to_wide = {"s8", "u8", "s16", "u16", "char", "short", "int", "long"}
    wide_edits = []
    for (sa, sb, info, dc) in plist:
        if dc["stars"] or dc["dims"]:
            continue
        bq = _base_type_token(ctx, info)
        if bq is None:
            continue
        cur = sig[bq].t
        has_unsigned = any(sig[q].t in ("unsigned", "signed") for q in range(*info["base"]))
        if has_unsigned:
            continue
        t = sig[bq]
        w32 = ctx.spell("s32")
        if cur in narrow_to_wide and cur != w32:
            ctx.add("param_type", [(t.s, t.e, w32)], 2, "type",
                    "param %s: %s -> %s" % (dc["name"], cur, w32))
            wide_edits.append((t.s, t.e, w32))
        elif cur in ("s32", "int", "long"):
            for tgt in (ctx.spell("s16"), ctx.spell("u8")):
                ctx.add("param_type", [(t.s, t.e, tgt)], 2, "type",
                        "param %s: %s -> %s" % (dc["name"], cur, tgt), ("semantic_risk",))
    if len(wide_edits) > 1:
        ctx.add("param_type", wide_edits, 3, "type", "all narrow params -> s32")


# --------------------------------------------------------------------------
# family: shared exit
# --------------------------------------------------------------------------

def _always_exits(n):
    if n.k in ("return", "break", "continue", "goto"):
        return True
    if n.k == "block":
        return bool(n.ch) and _always_exits(n.ch[-1])
    if n.k == "if":
        return n.d["else"] is not None and _always_exits(n.d["then"]) and _always_exits(n.d["else"])
    return False


def _has_return(n):
    return any(x.k == "return" for x in walk(n))


def fam_shared_exit(ctx):
    if not ctx.ok or not ctx.want("shared_exit"):
        return
    f = ctx.f
    sig = ctx.sig
    rets = [n for n in ctx.nodes if n.k == "return"]
    if len(rets) < 2:
        return
    # return type text
    rt_toks = [sig[q] for q in range(f.a, f.ni) if sig[q].t not in STORAGE and sig[q].t != "volatile"]
    if not rt_toks:
        return
    ret_text = " ".join(t.t for t in rt_toks)
    if ret_text.strip() == "void":
        return
    if any(t.k == "op" and t.t != "*" for t in rt_toks):
        return
    # existing result local: `return name;` as final statement
    last = ctx.body.ch[-1] if ctx.body.ch else None
    resname = None
    new_local = False
    if last is not None and last.k == "return":
        e = ctx.expr(*last.d["r"]) if last.d["r"][1] > last.d["r"][0] else None
        if e is not None and e.k == "ident" and e.name in ctx.locals:
            resname = e.name
    if resname is None:
        resname = ctx.fresh("result")
        new_local = True
    edits = []
    count = [0]

    def ret_edit(st, parent_is_block):
        a, b = st.d["r"]
        if b <= a:
            return False
        e = ctx.expr(a, b)
        if e is None:
            return False
        count[0] += 1
        if e.k == "ident" and e.name == resname:
            edits.append(ctx.del_stmt_edit(st) if parent_is_block else (st.s, st.e, ";"))
        else:
            edits.append((st.s, st.e, "%s = %s;" % (resname, ctx.text(a, b))))
        return True

    def conv_list(lst, parent_block):
        """Convert statements of a sequence; returns True on success."""
        for idx, st in enumerate(lst):
            is_last = idx == len(lst) - 1
            if st.k == "return":
                if not ret_edit(st, parent_block is not None):
                    return False
                return True
            if not _has_return(st):
                continue
            if st.k == "block":
                if not is_last:
                    return False
                return conv_list(st.ch, st)
            if st.k == "if":
                th, el = st.d["then"], st.d["else"]
                if is_last:
                    return conv_branch(th) and (el is None or conv_branch(el))
                if el is None and _always_exits(th) and th.k in ("return", "block"):
                    rest = lst[idx + 1:]
                    if any(r.k in ("decl", "label", "case", "default", "pp") or has_label_or_case(r) for r in rest):
                        return False
                    if not conv_branch(th):
                        return False
                    # wrap then in braces if needed, add else { rest }
                    if th.k != "block":
                        edits.append((th.s, th.s, "{ "))
                        edits.append((th.e, th.e, " }"))
                    ind = ctx.indent_of(st.s)
                    edits.append((th.e + (0 if th.k == "block" else 0), th.e, ""))
                    tail = " else {"
                    edits.append((th.e, th.e, ("" if th.k == "block" else "") + tail))
                    if not conv_list(rest, None):
                        return False
                    edits.append((rest[-1].e, rest[-1].e, "\n" + ind + "}"))
                    return True
                return False
            return False
        return True

    def conv_branch(n):
        if n.k == "block":
            return conv_list(n.ch, n)
        if n.k == "return":
            return ret_edit(n, False)
        if n.k == "if":
            th, el = n.d["then"], n.d["else"]
            return conv_branch(th) and (el is None or conv_branch(el))
        if not _has_return(n):
            return True
        return False

    if not conv_list(ctx.body.ch, ctx.body):
        return
    if count[0] != len(rets):
        return
    # the original last statement `return resname;` was converted to a deletion:
    # re-add the shared return
    close = ctx.sig[f.bc]
    ls = ctx.line_start(close.s)
    ind = "  "
    if ctx.body.ch:
        ind = ctx.indent_of(ctx.body.ch[0].s) or "  "
    if ctx.src[ls:close.s].strip() == "":
        edits.append((ls, ls, "%sreturn %s;\n" % (ind, resname)))
    else:
        edits.append((close.s, close.s, " return %s; " % resname))
    if new_local:
        decls = _top_decls(ctx)
        if decls:
            last_decl = decls[-1]
            edits.append((last_decl.e, last_decl.e, "\n%s%s %s;" % (ctx.indent_of(last_decl.s), ret_text, resname)))
        else:
            bo = ctx.sig[f.bo]
            edits.append((bo.e, bo.e, "\n%s%s %s;" % (ind, ret_text, resname)))
    risk = False
    if not new_local:
        dn, info, dc = ctx.locals[resname]
        lt = " ".join(ctx.sig[q].t for q in range(*info["base"]) if ctx.sig[q].t != "volatile")
        if lt.replace(" ", "") != ret_text.replace(" ", ""):
            risk = True
    ctx.add("shared_exit", edits, 4, "branch",
            "multiple returns -> %s assignments + one return" % resname,
            ("semantic_risk",) if risk else ())


# --------------------------------------------------------------------------
# families: small expression rewrites
# --------------------------------------------------------------------------

def _unsigned_name(ctx, nm):
    if nm in ctx.locals:
        info = ctx.locals[nm][1]
        dc = ctx.locals[nm][2]
    elif nm in ctx.params:
        info, dc = ctx.params[nm]
    else:
        return False
    if dc["stars"] or dc["dims"]:
        return False
    return any(ctx.sig[q].t in ("unsigned", "u8", "u16", "u32", "u64") for q in range(*info["base"]))


def fam_mul2_shift(ctx):
    if not ctx.ok or not ctx.want("mul2_shift"):
        return
    for e, st in ctx.all_exprs():
        if e is None:
            continue
        for x in ewalk(e):
            if x.k != "assign" or x.op not in ("*=", "<<=", "/=", ">>="):
                continue
            rhs = x.ch[1]
            if rhs.k != "num":
                continue
            try:
                v = int(rhs.name.rstrip("uUlL"), 0)
            except ValueError:
                continue
            opt = None
            # token of operator
            optok = None
            for q in range(x.ch[0].b, x.ch[1].a):
                if ctx.sig[q].t == x.op:
                    optok = ctx.sig[q]
            if optok is None:
                continue
            lhs = strip_paren(x.ch[0])
            unsigned = lhs.k == "ident" and _unsigned_name(ctx, lhs.name)
            newop = newv = None
            risk = False
            if x.op in ("*=", "/=") and v >= 2 and (v & (v - 1)) == 0:
                k = v.bit_length() - 1
                newop = "<<=" if x.op == "*=" else ">>="
                newv = str(k)
                risk = x.op == "/=" and not unsigned
            elif x.op in ("<<=", ">>=") and 1 <= v <= 16:
                newop = "*=" if x.op == "<<=" else "/="
                newv = str(1 << v)
                risk = x.op == ">>=" and not unsigned
            if newop is None:
                continue
            ctx.add("mul2_shift", [(optok.s, optok.e, newop), (rhs.s, rhs.e, newv)], 1, "spelling",
                    "%s %s -> %s %s" % (x.op, rhs.name, newop, newv), ("semantic_risk",) if risk else ())


def fam_incr_form(ctx):
    if not ctx.ok or not ctx.want("incr_form"):
        return
    for n in ctx.nodes:
        if n.k != "expr":
            continue
        a, b = n.d["r"]
        e = ctx.expr(a, b)
        if e is None:
            continue
        nm = None
        sign = None
        amt = None
        if e.k in ("post", "un") and e.op in ("++", "--") and e.ch[0].k == "ident":
            nm, sign, amt = e.ch[0].name, e.op[0], "1"
        elif e.k == "assign" and e.ch[0].k == "ident" and e.op in ("+=", "-=") :
            nm, sign, amt = e.ch[0].name, e.op[0], ctx.text(e.ch[1].a, e.ch[1].b)
            if not _is_atom_chain(e.ch[1]):
                amt = "(" + amt + ")"
        elif e.k == "assign" and e.ch[0].k == "ident" and e.op == "=" and e.ch[1].k == "bin" \
                and e.ch[1].op in ("+", "-") and e.ch[1].ch[0].k == "ident" and e.ch[1].ch[0].name == e.ch[0].name:
            r = e.ch[1].ch[1]
            nm, sign, amt = e.ch[0].name, e.ch[1].op, ctx.text(r.a, r.b)
            if not _is_atom_chain(r):
                amt = "(" + amt + ")"
        if nm is None:
            continue
        forms = []
        if amt == "1":
            forms.append("%s%s%s" % (nm, sign, sign))
        forms.append("%s %s= %s" % (nm, sign, amt))
        forms.append("%s = %s %s %s" % (nm, nm, sign, amt))
        cur = ctx.text(a, b)
        for fm in forms:
            if fm.replace(" ", "") != cur.replace(" ", ""):
                ctx.add("incr_form", [(ctx.sig[a].s, ctx.sig[b - 1].e, fm)], 1, "spelling",
                        "%s -> %s" % (cur, fm))


def _side(ctx, e):
    return ctx.has_sideeffect(e.a, e.b)


def fam_commute(ctx):
    if not ctx.ok:
        return
    want_c = ctx.want("commute")
    want_f = ctx.want("cmp_flip")
    if not (want_c or want_f):
        return
    src = ctx.src
    for e0, st in ctx.all_exprs():
        if e0 is None:
            continue
        for x in ewalk(e0):
            if x.k != "bin":
                continue
            op = x.op
            l, r = x.ch
            if (op in ("+", "*") and want_c) or (op in CMP_MIRROR and want_f):
                prec = BINPREC[op]

                def side_text(c, is_new_right, prec=prec, op=op):
                    t = src[c.s:c.e]
                    need = False
                    if c.k == "bin":
                        cp = BINPREC[c.op]
                        if cp < prec or (is_new_right and cp == prec):
                            need = True
                    elif c.k in ("assign", "ternary", "comma"):
                        need = True
                    return "(" + t + ")" if need else t
                nl = side_text(r, False)
                nr = side_text(l, True)
                newop = op if op in ("+", "*") else CMP_MIRROR[op]
                # keep the original spacing around operators
                mid = src[l.e:r.s]
                new_mid = mid.replace(op, newop, 1) if newop != op else mid
                txt = nl + new_mid + nr
                risk = _side(ctx, l) and _side(ctx, r)
                if op in ("+", "*"):
                    ctx.add("commute", [(x.s, x.e, txt)], 2, "register",
                            "flip operands of '%s'" % op, ("semantic_risk",) if risk else ())
                else:
                    ctx.add("cmp_flip", [(x.s, x.e, txt)], 2, "order",
                            "mirror comparison '%s' -> '%s'" % (op, newop), ("semantic_risk",) if risk else ())


def _floaty(ctx, e, scal, ptr):
    e = e
    if e.k == "num":
        t = e.name
        return not t.lower().startswith("0x") and ("." in t or "e" in t.lower() or t.lower().endswith("f"))
    if e.k == "ident":
        return e.name in scal
    if e.k == "paren":
        return _floaty(ctx, e.ch[0], scal, ptr)
    if e.k == "cast":
        ty = ctx.text(e.ta, e.tb)
        return any(w in FLOAT_TYPES for w in re.findall(r"\w+", ty)) and "*" not in ty
    if e.k == "un":
        if e.op in ("-", "+"):
            return _floaty(ctx, e.ch[0], scal, ptr)
        if e.op == "*":
            return _floatptr(ctx, e.ch[0], scal, ptr)
        return False
    if e.k == "idx":
        return _floatptr(ctx, e.ch[0], scal, ptr)
    if e.k == "call":
        c = strip_paren(e.ch[0])
        if c.k == "ident" and c.name == "M2C_FIELD" and len(e.ch) >= 3:
            ty = ctx.text(e.ch[2].a, e.ch[2].b)
            return any(w in FLOAT_TYPES for w in re.findall(r"\w+", ty)) and "*" in ty
        if c.k == "ident" and c.name in ("fabsf", "sqrtf"):
            return True
        return False
    if e.k == "bin":
        if e.op in ("+", "-", "*", "/"):
            return _floaty(ctx, e.ch[0], scal, ptr) or _floaty(ctx, e.ch[1], scal, ptr)
        return False
    if e.k == "assign":
        return _floaty(ctx, e.ch[0], scal, ptr)
    if e.k == "ternary":
        return _floaty(ctx, e.ch[1], scal, ptr) or _floaty(ctx, e.ch[2], scal, ptr)
    return False


def _floatptr(ctx, e, scal, ptr):
    e = strip_paren(e)
    if e.k == "ident":
        return e.name in ptr
    if e.k == "cast":
        ty = ctx.text(e.ta, e.tb)
        return any(w in FLOAT_TYPES for w in re.findall(r"\w+", ty)) and "*" in ty
    if e.k == "bin" and e.op in ("+", "-"):
        return _floatptr(ctx, e.ch[0], scal, ptr)
    return False


def fam_literal_spelling(ctx):
    if not ctx.ok or not ctx.want("literal_spelling"):
        return
    scal, ptr = ctx.float_names()
    for e0, st in ctx.all_exprs():
        if e0 is None:
            continue
        for x in ewalk(e0):
            if x.k != "num":
                continue
            t = x.name
            tl = t.lower()
            if tl.startswith("0x"):
                continue
            isfloat = "." in tl or "e" in tl or tl.endswith("f")
            try:
                val = float(tl.rstrip("fl")) if isfloat else float(int(tl.rstrip("ul")))
            except ValueError:
                continue
            if val not in (0.0, 1.0):
                continue
            par = x.parent
            while par is not None and par.k == "paren":
                par = par.parent
            ctxok = False
            if par is not None:
                if par.k == "bin" and (par.op in ("+", "-", "*", "/") or par.op in CMP_MIRROR):
                    sib = par.ch[1] if strip_paren(par.ch[0]) is x or par.ch[0] is x else par.ch[0]
                    ctxok = _floaty(ctx, sib, scal, ptr)
                elif par.k == "assign" and par.ch[1] is x or (par.k == "assign" and strip_paren(par.ch[1]) is x):
                    if par.op == "=":
                        ctxok = True if isfloat else _floaty(ctx, par.ch[0], scal, ptr)
                    else:
                        ctxok = _floaty(ctx, par.ch[0], scal, ptr)
            if not ctxok:
                continue
            i = int(val)
            forms = []
            if isfloat:
                forms.append((str(i), ()))
                if tl.endswith("f"):
                    forms.append(("%d.0" % i, ("semantic_risk",)))
                else:
                    forms.append(("%d.0f" % i, ("semantic_risk",)))
                if not tl.endswith("f") and tl.rstrip("0").endswith(".") is False:
                    pass
            else:
                forms.append(("%d.0f" % i, ()))
                forms.append(("%d.0" % i, ("semantic_risk",)))
            for txt, fl in forms:
                if txt == t:
                    continue
                ctx.add("literal_spelling", [(x.s, x.e, txt)], 2, "literal", "%s -> %s" % (t, txt), fl)


# --------------------------------------------------------------------------
# families: control-flow shapes
# --------------------------------------------------------------------------

def _flatten(e, op):
    e0 = e
    if e.k == "bin" and e.op == op:
        return _flatten(e.ch[0], op) + [e.ch[1]]
    return [e]


def _neg(ctx, cond_range):
    """(text, risk) of the negation of the condition."""
    e = ctx.expr(*cond_range)
    if e is None:
        return None
    src = ctx.src
    inner = strip_paren(e)
    if inner.k == "un" and inner.op == "!":
        c = inner.ch[0]
        t = src[c.s:c.e]
        if c.k == "paren":
            t = src[strip_paren(c).s:strip_paren(c).e]
        return t, False
    if inner.k == "bin" and inner.op in CMP_NEG:
        l, r = inner.ch
        mid = src[l.e:r.s].replace(inner.op, CMP_NEG[inner.op], 1)
        scal, ptr = ctx.float_names()
        risk = inner.op in ("<", ">", "<=", ">=") and (_floaty(ctx, l, scal, ptr) or _floaty(ctx, r, scal, ptr))
        return src[l.s:l.e] + mid + src[r.s:r.e], risk
    t = src[e.s:e.e]
    if _is_atom(e) and e.k != "paren":
        return "!" + t, False
    if e.k == "paren":
        return "!" + t, False
    return "!(" + t + ")", False


def fam_ifelse_swap(ctx):
    if not ctx.ok or not ctx.want("ifelse_swap"):
        return
    for n in ctx.nodes:
        if n.k != "if" or n.d["else"] is None:
            continue
        r = _neg(ctx, n.d["cond"])
        if r is None:
            continue
        ntxt, risk = r
        th, el = n.d["then"], n.d["else"]
        src = ctx.src
        lp, rp = ctx.sig[n.d["lp"]], ctx.sig[n.d["rp"]]
        between1 = src[rp.e:th.s]
        between2 = src[th.e:el.s]
        txt = src[n.s:lp.e] + ntxt + src[rp.s:rp.e] + between1 + wrap_block_text(ctx, el) + \
            between2 + wrap_block_text(ctx, th)
        ctx.add("ifelse_swap", [(n.s, n.e, txt)], 3, "branch", "swap if/else branches, negate condition",
                ("semantic_risk",) if risk else ())


def _simple_branch(n):
    """True when n is small enough to duplicate and has no labels/decls."""
    toks = n.b - n.a
    if toks > 60:
        return False
    for x in walk(n):
        if x.k in ("label", "case", "default", "decl", "pp"):
            return False
    return True


def fam_andor_nest(ctx):
    if not ctx.ok or not ctx.want("andor_nest"):
        return
    src = ctx.src
    for n in ctx.nodes:
        if n.k != "if":
            continue
        th, el = n.d["then"], n.d["else"]
        ce = ctx.expr(*n.d["cond"])
        ind = ctx.indent_of(n.s)
        lp, rp = ctx.sig[n.d["lp"]], ctx.sig[n.d["rp"]]
        if ce is not None and el is None:
            top = strip_paren(ce) if ce.k == "paren" and False else ce
            for op in ("&&", "||"):
                if top.k == "bin" and top.op == op:
                    parts = _flatten(top, op)
                    # only split a whole chain (no mixing)
                    for k in range(1, len(parts)):
                        first = src[parts[0].s:parts[k - 1].e]
                        rest = src[parts[k].s:parts[-1].e]
                        if op == "&&":
                            inner_if = "if (%s) %s" % (rest, ctx.ntext(th))
                            newt = "if (%s) {\n%s  %s\n%s}" % (first, ind, inner_if, ind)
                            ctx.add("andor_nest", [(n.s, n.e, newt)], 3, "branch",
                                    "&& chain split after operand %d -> nested if" % k)
                        else:
                            if not _simple_branch(th):
                                continue
                            if th.k != "block" and th.k not in ("return", "break", "continue", "goto", "expr"):
                                continue
                            s1 = ctx.ntext(th)
                            newt = "if (%s) %s\n%selse if (%s) %s" % (first, s1, ind, rest, s1)
                            ctx.add("andor_nest", [(n.s, n.e, newt)], 3, "branch",
                                    "|| chain split after operand %d -> if / else if" % k)
        # reverse: nested if without else -> && chain
        if el is None:
            inner = th
            if th.k == "block" and len(th.ch) == 1:
                inner = th.ch[0]
            if inner.k == "if" and inner.d["else"] is None and inner is not n:
                c1 = ctx.text(*n.d["cond"])
                c2 = ctx.text(*inner.d["cond"])
                e1 = ctx.expr(*n.d["cond"])
                e2 = ctx.expr(*inner.d["cond"])

                def wrap(e, t):
                    return "(" + t + ")" if e is not None and e.k in ("bin",) and e.op == "||" or \
                        (e is not None and e.k in ("assign", "ternary", "comma")) else t
                txt = "if (%s && %s) %s" % (wrap(e1, c1), wrap(e2, c2), ctx.ntext(inner.d["then"]))
                ctx.add("andor_nest", [(n.s, n.e, txt)], 3, "branch", "nested if -> && chain")
        # reverse: if (A) S else if (B) S  ->  if (A || B) S
        if el is not None and el.k == "if" and el.d["else"] is None:
            if ctx.ntext(th) == ctx.ntext(el.d["then"]):
                e1 = ctx.expr(*n.d["cond"])
                e2 = ctx.expr(*el.d["cond"])
                c1 = ctx.text(*n.d["cond"])
                c2 = ctx.text(*el.d["cond"])

                def wrap2(e, t):
                    return "(" + t + ")" if e is not None and e.k in ("assign", "ternary", "comma") else t
                txt = "if (%s || %s) %s" % (wrap2(e1, c1), wrap2(e2, c2), ctx.ntext(th))
                ctx.add("andor_nest", [(n.s, n.e, txt)], 3, "branch", "if/else-if with identical bodies -> || chain")


def _switch_groups(ctx, sw):
    body = sw.d["body"]
    if body.k != "block":
        return None
    groups = []
    cur = None
    for st in body.ch:
        if st.k in ("case", "default"):
            if cur is None or cur["stmts"]:
                cur = {"labels": [], "stmts": []}
                groups.append(cur)
            cur["labels"].append(st)
        elif st.k == "pp":
            return None
        else:
            if cur is None:
                return None
            cur["stmts"].append(st)
    if not groups:
        return None
    # fuse fallthrough chains
    fused = []
    acc = None
    for g in groups:
        if acc is None:
            acc = {"labels": list(g["labels"]), "stmts": list(g["stmts"]), "parts": 1}
        else:
            acc["labels"] += g["labels"]
            acc["stmts"] += g["stmts"]
            acc["parts"] += 1
        exits = bool(g["stmts"]) and _always_exits(g["stmts"][-1])
        if exits:
            fused.append(acc)
            acc = None
    if acc is not None:
        acc["tail_open"] = True
        fused.append(acc)
    for g in fused:
        first = g["labels"][0]
        last = g["stmts"][-1] if g["stmts"] else g["labels"][-1]
        g["s"], g["e"] = first.s, last.e
    return fused


def fam_switch_perm(ctx):
    if not ctx.ok or not ctx.want("switch_perm"):
        return
    for n in ctx.nodes:
        if n.k != "switch":
            continue
        gs = _switch_groups(ctx, n)
        if not gs:
            continue
        mov = [g for g in gs if not g.get("tail_open")]
        if len(mov) < 2:
            continue
        if any(has_label_or_case_nonswitch(g) for g in mov):
            continue
        texts = [ctx.src[g["s"]:g["e"]] for g in mov]
        orders = _perms(len(mov))

        def key_of(g):
            for lb in g["labels"]:
                if lb.k == "case":
                    e = ctx.expr(*lb.d["r"])
                    if e is not None:
                        try:
                            return const_eval(ctx, e, {})
                        except (KeyError, ValueError):
                            return None
            return None
        keys = [key_of(g) for g in mov]
        if all(k is not None for k in keys):
            asc = tuple(sorted(range(len(mov)), key=lambda i: keys[i]))
            desc = tuple(sorted(range(len(mov)), key=lambda i: -keys[i]))
            for o in (asc, desc):
                if o not in orders and o != tuple(range(len(mov))):
                    orders.append(o)
        for o in orders:
            edits = [(mov[k]["s"], mov[k]["e"], texts[o[k]]) for k in range(len(mov)) if o[k] != k]
            ctx.add("switch_perm", edits, 2, "order", "case order %s" % (list(o),))


def has_label_or_case_nonswitch(g):
    for st in g["stmts"]:
        for x in walk(st):
            if x.k == "label":
                return True
    return False


def _effects(ctx, e):
    """Crude effect summary of a statement expression."""
    W, S, L, ids = set(), [], [], set()
    calls = False
    loc = set(ctx.locals) | set(ctx.params)

    def mem_key(x):
        x = strip_paren(x)
        path = []
        while True:
            if x.k == "mem":
                path.append(x.op + x.name)
                x = strip_paren(x.ch[0])
            elif x.k == "idx":
                ie = strip_paren(x.ch[1])
                path.append("[" + (ie.name if ie.k == "num" else "?") + "]")
                x = strip_paren(x.ch[0])
            elif x.k == "un" and x.op == "*":
                path.append("*")
                x = strip_paren(x.ch[0])
            elif x.k == "cast":
                x = strip_paren(x.ch[0])
            else:
                break
        base = x.name if x.k == "ident" else "?"
        return (base, "".join(reversed(path)))

    def visit(x, is_lhs=False):
        nonlocal calls
        if x.k == "ident":
            ids.add(x.name)
            if x.name not in loc and not ctx.P.is_type_word(x.name):
                (S if is_lhs else L).append((x.name, ""))
            return
        if x.k == "call":
            c = strip_paren(x.ch[0])
            if not (c.k == "ident" and c.name in PURE_CALLS):
                calls = True
            for a in x.ch:
                visit(a)
            return
        if x.k == "assign" or (x.k in ("un", "post") and x.op in ("++", "--")):
            lhs = strip_paren(x.ch[0])
            if lhs.k == "ident":
                ids.add(lhs.name)
                if lhs.name in loc:
                    W.add(lhs.name)
                else:
                    S.append((lhs.name, ""))
            else:
                S.append(mem_key(lhs))
                for c in ewalk(lhs):
                    if c.k == "ident":
                        ids.add(c.name)
                # subexpressions of lhs are loads
                for c in lhs.ch:
                    visit(c)
            if x.k == "assign":
                if x.op != "=":
                    if lhs.k == "ident" and lhs.name in loc:
                        pass
                visit(x.ch[1])
            return
        if x.k in ("mem", "idx") or (x.k == "un" and x.op == "*"):
            L.append(mem_key(x))
        for c in x.ch:
            visit(c)

    visit(e)
    return W, S, L, ids, calls


def _independent(ctx, a, b):
    ea = ctx.expr(*a.d["r"])
    eb = ctx.expr(*b.d["r"])
    if ea is None or eb is None:
        return None
    Wa, Sa, La, ia, ca = _effects(ctx, ea)
    Wb, Sb, Lb, ib, cb = _effects(ctx, eb)
    if ca or cb:
        return None
    if (ia | ib) & ctx.volatile:
        return None
    if Wa & ib or Wb & ia:
        return None
    risk = False

    def conflict(S1, acc):
        r = False
        for (b1, p1) in S1:
            for (b2, p2) in acc:
                if b1 == b2 and b1 != "?" and p1 != p2 and "?" not in p1 + p2 and "*" not in p1 + p2:
                    # distinct constant fields / indices off one base
                    if p1 and p2 and not (p1.startswith(p2) or p2.startswith(p1)):
                        continue
                    if p1 == "" or p2 == "":
                        return None  # same base, one is the whole object
                    return None
                if b1 == b2 and p1 == p2:
                    return None
                # different bases: may alias through pointers
                if b1 != b2:
                    if p1 == "" and p2 == "" and b1 != "?" and b2 != "?":
                        continue  # two distinct named globals
                    r = True
        return r
    for S1, acc in ((Sa, Sb + Lb), (Sb, Sa + La)):
        c = conflict(S1, acc)
        if c is None:
            return None
        risk = risk or c
    return ("semantic_risk",) if risk else ()


def fam_stmt_swap(ctx):
    if not ctx.ok or not ctx.want("stmt_swap"):
        return
    for n in ctx.nodes:
        if n.k != "block":
            continue
        for i in range(len(n.ch) - 1):
            a, b = n.ch[i], n.ch[i + 1]
            if a.k != "expr" or b.k != "expr":
                continue
            fl = _independent(ctx, a, b)
            if fl is None:
                continue
            ta, tb = ctx.ntext(a), ctx.ntext(b)
            ctx.add("stmt_swap", [(a.s, a.e, tb), (b.s, b.e, ta)], 3, "order",
                    "swap adjacent independent statements", fl)


# --------------------------------------------------------------------------
# catalog registry and API
# --------------------------------------------------------------------------

CATALOG = {
    "loop_do_to_for": (fam_loops, "loop", "do/while -> for (init/step folded)"),
    "loop_do_to_while": (None, "loop", "do/while -> while"),
    "loop_for_to_do": (None, "loop", "for -> do/while"),
    "loop_for_to_while": (None, "loop", "for -> while"),
    "loop_while_to_for": (None, "loop", "while -> for"),
    "loop_to_goto": (None, "loop", "do/while -> goto loop"),
    "loop_from_goto": (None, "loop", "goto loop -> do/while"),
    "counter_type": (fam_counter_type, "loop", "loop counter s16 <-> s32"),
    "local_inline": (fam_local_inline, "frame", "inline/drop a named local assigned once"),
    "decl_order": (fam_decl_order, "frame", "permute declaration order"),
    "pad_local": (fam_pad_local, "frame", "add/remove/resize unused pad locals"),
    "extern_toggle": (fam_extern_toggle, "linkage", "extern <-> defined global"),
    "ptr_launder": (fam_ptr_launder, "order", "(u32) pointer laundering"),
    "knr_proto": (fam_knr_proto, "callsetup", "K&R prototype for a callee"),
    "param_type": (fam_param_type, "type", "parameter s16/u8 <-> s32"),
    "shared_exit": (fam_shared_exit, "branch", "returns -> result assigns + one return"),
    "mul2_shift": (fam_mul2_shift, "spelling", "x *= 2 <-> x <<= 1, x /= 2 <-> x >>= 1"),
    "incr_form": (fam_incr_form, "spelling", "x++ / x += 1 / x = x + 1"),
    "commute": (fam_commute, "register", "flip operands of + and *"),
    "cmp_flip": (None, "order", "mirror comparisons"),
    "literal_spelling": (fam_literal_spelling, "literal", "0 / 0.0f / 0.0, 1 / 1.0f"),
    "andor_nest": (fam_andor_nest, "branch", "&&/|| chain <-> nested if"),
    "ifelse_swap": (fam_ifelse_swap, "branch", "swap if/else, negate condition"),
    "switch_perm": (fam_switch_perm, "order", "permute switch case groups"),
    "stmt_swap": (fam_stmt_swap, "order", "swap adjacent independent statements"),
}

# families implemented by a shared generator (run once)
_GENERATORS = [
    (("loop_do_to_for", "loop_do_to_while", "loop_for_to_do", "loop_for_to_while",
      "loop_while_to_for", "loop_to_goto", "loop_from_goto"), fam_loops),
    (("counter_type",), fam_counter_type),
    (("local_inline",), fam_local_inline),
    (("decl_order",), fam_decl_order),
    (("pad_local",), fam_pad_local),
    (("extern_toggle",), fam_extern_toggle),
    (("ptr_launder",), fam_ptr_launder),
    (("knr_proto",), fam_knr_proto),
    (("param_type",), fam_param_type),
    (("shared_exit",), fam_shared_exit),
    (("mul2_shift",), fam_mul2_shift),
    (("incr_form",), fam_incr_form),
    (("commute", "cmp_flip"), fam_commute),
    (("literal_spelling",), fam_literal_spelling),
    (("andor_nest",), fam_andor_nest),
    (("ifelse_swap",), fam_ifelse_swap),
    (("switch_perm",), fam_switch_perm),
    (("stmt_swap",), fam_stmt_swap),
]


def catalog_names():
    return sorted(CATALOG)


def functions(src):
    """Names of the function definitions in a translation unit."""
    return [f.name for f in parse_source(src).func_list]


def mutations(src, fn=None, catalog=None):
    """All catalog mutations of `src` (restricted to function `fn`).

    Deterministic order: (cost, family, source position). Duplicated results
    (same new source) are dropped; a mutation that does not change the source
    is never returned.
    """
    cat = None if catalog is None else set(catalog)
    if cat is not None:
        unknown = cat - set(CATALOG)
        if unknown:
            raise ValueError("unknown catalog families: %s" % ", ".join(sorted(unknown)))
    P = parse_source(src)
    if fn is not None:
        if fn not in P.funcs:
            return []
        funcs = [P.funcs[fn]]
    else:
        funcs = list(P.func_list)
    seen = set()
    out = []
    for f in funcs:
        ctx = Ctx(src, P, f, cat, seen)
        for names, gen in _GENERATORS:
            if cat is not None and not (cat & set(names)):
                continue
            try:
                gen(ctx)
            except (ParseError, IndexError, RecursionError, KeyError, ValueError):
                # a site the light parsers could not handle: skip the family
                if os.environ.get("AMATCH_DEBUG"):
                    raise
                continue
        out.extend(ctx.out)
    out.sort(key=lambda m: (m.cost, m.name, m.edits[0][0] if m.edits else 0, m.id))
    return out


def apply(src, mutation_id):
    """Apply the mutation with this id (as produced by mutations() on `src`)."""
    m = re.match(r"^([a-z0-9_]+)@(.+)#([0-9a-f]{8})$", mutation_id)
    if not m:
        raise ValueError("bad mutation id %r" % mutation_id)
    name, fn = m.group(1), m.group(2)
    for mu in mutations(src, fn=fn, catalog=[name]):
        if mu.id == mutation_id:
            return mu.new_src
    raise KeyError("mutation %s not applicable to this source" % mutation_id)


GAPS = """
Known gaps (not implemented, or only partly):
 - introducing a *new* named local (hoisting a repeated subexpression or a
   constant such as `zero = 0.0f`); only dropping/inlining and pad locals exist;
 - scale-then-add FP idiom and associativity regrouping ((a+b)+c -> a+(b+c));
 - pointer-stride / induction-variable rewrites (`p += 4` -> `p += 1`), m2c
   goto-loop -> indexed array loop; struct/array typing of externs;
 - typed prototype reconstruction (only `f(args)` -> `f()`), argument-count changes;
 - `-O1/-O2/-O3` flag sweeps and group-level knobs (keep membership, stand-in
   call sites): those are search.py/probe.py concerns, not source edits;
 - while/do loops whose step statements are not trailing; `continue`-bearing
   do-while bodies are skipped rather than rewritten;
 - K&R-style function definitions and macro-heavy statements are skipped.
"""


# --------------------------------------------------------------------------
# CLI
# --------------------------------------------------------------------------

def main(argv=None):
    ap = argparse.ArgumentParser(description="Mutation catalog for IDO C89 sources")
    ap.add_argument("file")
    ap.add_argument("func", nargs="?", help="function name (default: every function)")
    ap.add_argument("--list", action="store_true", help="one line per mutation")
    ap.add_argument("--json", action="store_true", help="machine-readable output")
    ap.add_argument("--catalog", help="comma-separated families")
    ap.add_argument("--with-src", action="store_true", help="include new_src in --json")
    ap.add_argument("--apply", metavar="ID", help="print the source after applying this mutation id")
    ap.add_argument("--gaps", action="store_true")
    a = ap.parse_args(argv)
    if a.gaps:
        print(GAPS)
        return 0
    with open(a.file, encoding="utf8", errors="replace") as fh:
        src = fh.read()
    if a.apply:
        sys.stdout.write(apply(src, a.apply))
        return 0
    cat = a.catalog.split(",") if a.catalog else None
    muts = mutations(src, a.func, cat)
    if a.json:
        print(json.dumps({
            "file": a.file, "fn": a.func, "count": len(muts),
            "mutations": [m.to_dict(with_src=a.with_src, src=src) for m in muts]}, indent=1))
        return 0
    if a.list:
        for m in muts:
            print("%-42s cost=%-2d %-9s %s%s" % (m.id, m.cost, m.target, m.desc,
                                                 "  [semantic_risk]" if m.semantic_risk else ""))
        return 0
    counts = {}
    for m in muts:
        c = counts.setdefault(m.name, [0, 0])
        c[0] += 1
        c[1] += m.semantic_risk
    print("%d mutations" % len(muts))
    for k in sorted(counts):
        print("  %-18s %4d  (risky %d)  target=%s" % (k, counts[k][0], counts[k][1], CATALOG[k][1]))
    return 0


if __name__ == "__main__":
    sys.exit(main())
