"""Where IDO put a translation unit's function-local statics (recomp Phase 10, lane w30r).

IDO 5.3/7.1 write NO symbol for a function-local static (initialised or not) nor for a
file-scope `static` object: the code reaches them through the `.data`/`.bss` section
symbol plus an offset. replace.py's static binding (`--statics NAME=0xADDR`, entry
"statics") therefore had no name to match for a draft's own statics (w29d: SaveServer's
sInitCountdown/sLastPak/sReadingPak stayed private; the lane needed a `dc.c` with
externs). This module recovers the layout from the preprocessed source:

- every object DEFINITION in the TU, in source order: file-scope objects (not `extern`,
  not `typedef`, not functions) and the `static` objects inside function bodies;
- initialised objects go to `.data`, uninitialised `static` ones to `.bss`
  (an uninitialised non-static file-scope object is COMMON: no section);
- a `static` object whose name appears nowhere else in the TU is dropped (IDO
  removes unreferenced statics without leaving a gap);
- each object starts at its section's cursor aligned to max(4, natural alignment)
  (IDO -G 0; checked on rocky: char statics 4 bytes apart, a double at 8).

Sizes come from a small type evaluator (C scalar names, the TU's own scalar
typedefs, pointers, arrays with constant or initialiser-counted bounds). An object
whose size is unknown (a struct, an unknown typedef) stops the layout of its
section after it: later objects of that section get no offset. The layout is
checked against the object: every named object symbol of the section must sit at
its computed offset, and the cursor must not pass the section size; integer
initialisers of scalar statics must match the section bytes. A failed check gives
no layout for that section (the static then stays private, as before).

stdlib only; `layout(pp_text, elf)` -> [Obj]."""
import re
import struct

SCALARS = {
    "char": 1, "signed char": 1, "unsigned char": 1,
    "short": 2, "short int": 2, "signed short": 2, "unsigned short": 2, "unsigned short int": 2,
    "int": 4, "signed": 4, "signed int": 4, "unsigned": 4, "unsigned int": 4,
    "long": 4, "long int": 4, "signed long": 4, "unsigned long": 4, "unsigned long int": 4,
    "long long": 8, "signed long long": 8, "unsigned long long": 8,
    "float": 4, "double": 8, "long double": 8,
}
_QUALIFIERS = {"const", "volatile", "register", "auto"}
_KEYWORDS = {"static", "extern", "void", "char", "short", "int", "long", "float", "double", "signed", "unsigned",
             "struct", "union", "enum"}
_TOKEN = re.compile(r"[A-Za-z_]\w*|0[xX][0-9A-Fa-f]+[uUlL]*|\d+\.\d*(?:[eE][-+]?\d+)?[fFlL]?|\d+[uUlL]*"
                    r"|\.\.\.|->|<<|>>|[-+*/%&|^~!<>=]=?|&&|\|\||[{}()\[\];,.?:]|\S")


class Obj:
    """One object definition: name, function (None at file scope), section
    ('.data', '.bss' or None for COMMON), offset (None when not placed), size,
    init (list of int for a scalar/array of integer literals, else None)."""

    def __init__(self, name, function, section, size, align, init, scalar):
        self.name, self.function, self.section = name, function, section
        self.size, self.align, self.init, self.scalar = size, align, init, scalar
        self.offset = None

    def key(self):
        return ("%s:%s" % (self.function, self.name)) if self.function else self.name

    def __repr__(self):
        return "Obj(%s %s+%s size %s)" % (self.key(), self.section, self.offset, self.size)


def _strip(text):
    """Comments and string/char literals out (literals become a placeholder
    token that keeps their length), preprocessor lines out."""
    out, i, n = [], 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith("/*", i):
            j = text.find("*/", i + 2)
            i = n if j < 0 else j + 2
            out.append(" ")
        elif text.startswith("//", i):
            j = text.find("\n", i)
            i = n if j < 0 else j
        elif c in "\"'":
            j, s = i + 1, []
            while j < n and text[j] != c:
                if text[j] == "\\" and j + 1 < n:
                    s.append(text[j:j + 2])
                    j += 2
                else:
                    s.append(text[j])
                    j += 1
            body = "".join(s)
            length = len(bytes(body, "latin-1", "replace").decode("unicode_escape", "replace")) if c == '"' else 1
            out.append(' __STR%d__ ' % length if c == '"' else " 0 ")
            i = j + 1
        elif c == "#" and (not out or "".join(out[-1:]).endswith("\n") or i == 0
                           or text[text.rfind("\n", 0, i) + 1:i].strip() == ""):
            j = text.find("\n", i)
            while j > 0 and text[j - 1] == "\\":
                j = text.find("\n", j + 1)
            i = n if j < 0 else j
        else:
            out.append(c)
            i += 1
    return "".join(out)


def _split_top(tokens, sep=","):
    """Split a token list at `sep` outside (), [], {}."""
    parts, cur, depth = [], [], 0
    for t in tokens:
        if t in "([{":
            depth += 1
        elif t in ")]}":
            depth -= 1
        if t == sep and depth == 0:
            parts.append(cur)
            cur = []
        else:
            cur.append(t)
    parts.append(cur)
    return parts


def _const(tokens):
    """Value of an integer constant expression, else None."""
    expr = []
    for t in tokens:
        if re.match(r"^(0[xX][0-9A-Fa-f]+|\d+)[uUlL]*$", t):
            expr.append(str(int(t.rstrip("uUlL"), 0)))
        elif t in ("+", "-", "*", "/", "%", "(", ")", "<<", ">>", "&", "|", "^", "~"):
            expr.append("//" if t == "/" else t)
        else:
            return None
    if not expr:
        return None
    try:
        return int(eval("".join(expr), {"__builtins__": {}}, {}))   # digits and operators only
    except Exception:
        return None


def _init_count(tokens):
    """Elements of a braced initialiser (top level), or a string literal's size."""
    if not tokens:
        return None
    if tokens[0] == "{" and tokens[-1] == "}":
        inner = tokens[1:-1]
        parts = [p for p in _split_top(inner) if p]
        return len(parts)
    m = re.match(r"^__STR(\d+)__$", tokens[0])
    if m and len(tokens) == 1:
        return int(m.group(1)) + 1
    return None


def _init_values(tokens):
    """Integer literal values of an initialiser (scalar or flat braced list), else None."""
    if tokens and tokens[0] == "{" and tokens[-1] == "}":
        parts = [p for p in _split_top(tokens[1:-1]) if p]
    else:
        parts = [tokens]
    vals = []
    for p in parts:
        v = _const(p)
        if v is None:
            return None
        vals.append(v)
    return vals


class _Types:
    def __init__(self):
        self.typedefs = {}      # name -> (size, align) or None (unknown)

    def base(self, words):
        """(size, align) of a base type given its words, else None."""
        ws = [w for w in words if w not in _QUALIFIERS and w not in ("static", "extern")]
        if not ws:
            return None
        if ws[0] in ("struct", "union", "enum"):
            return (4, 4) if ws[0] == "enum" else None
        key = " ".join(ws)
        if key in SCALARS:
            return (SCALARS[key], min(SCALARS[key], 8))
        if len(ws) == 1 and ws[0] in self.typedefs:
            return self.typedefs[ws[0]]
        return None


def _parse_declarator(tokens):
    """(name, pointer, dims, is_function, init tokens) of one declarator."""
    init = None
    if "=" in tokens:
        k = tokens.index("=")
        tokens, init = tokens[:k], tokens[k + 1:]
    name, pointer, dims, func = None, False, [], False
    i = 0
    while i < len(tokens):
        t = tokens[i]
        if t == "*":
            pointer = True
        elif t == "(":
            # function declarator `name(` or pointer-to-function `(*name)(`
            if name is not None:
                func = not pointer
                break
            if i + 1 < len(tokens) and tokens[i + 1] == "*":
                pointer = True
        elif t == "[":
            j = i + 1
            depth = 1
            while j < len(tokens) and depth:
                depth += {"[": 1, "]": -1}.get(tokens[j], 0)
                j += 1
            dims.append(tokens[i + 1:j - 1])
            i = j
            continue
        elif re.match(r"^[A-Za-z_]\w*$", t) and t not in _QUALIFIERS and name is None:
            name = t
        i += 1
    return name, pointer, dims, func, init


def _declarations(text):
    """Yield (function or None, tokens of one declaration statement) for every
    file-scope declaration and every declaration starting with `static` inside a
    function body, in source order."""
    tokens = _TOKEN.findall(_strip(text))
    i, n = 0, len(tokens)
    while i < n:
        # one file-scope item: up to ';' at depth 0, or a function body
        start, depth = i, 0
        while i < n:
            t = tokens[i]
            if t in "([":
                depth += 1
            elif t in ")]":
                depth -= 1
            elif t == "{" and depth == 0:
                prev = tokens[i - 1] if i > start else ""
                if prev == ")":
                    break                       # a function definition's body
                # an aggregate body or initialiser: skip it whole
                d = 0
                while i < n:
                    d += {"{": 1, "}": -1}.get(tokens[i], 0)
                    i += 1
                    if d == 0:
                        break
                continue
            elif t == ";" and depth == 0:
                break
            i += 1
        if i >= n:
            break
        if tokens[i] == ";":
            yield None, tokens[start:i]
            i += 1
            continue
        # function body at tokens[i] == '{'
        head = tokens[start:i]
        fname = None
        for k in range(len(head) - 1, -1, -1):
            if head[k] == "(":
                # name before the parameter list (first '(' at depth 0 from the left)
                pass
        d = 0
        for k, t in enumerate(head):
            if t == "(" and d == 0 and k > 0 and re.match(r"^[A-Za-z_]\w*$", head[k - 1]):
                fname = head[k - 1]
                break
            d += {"(": 1, ")": -1}.get(t, 0)
        d, j = 0, i
        stmt_start = True
        while j < n:
            t = tokens[j]
            if t == "{":
                d += 1
                stmt_start = True
                j += 1
                continue
            if t == "}":
                d -= 1
                j += 1
                stmt_start = True
                if d == 0:
                    break
                continue
            if t == ";":
                stmt_start = True
                j += 1
                continue
            if stmt_start and t == "static":
                k, pd = j, 0
                while k < n:
                    if tokens[k] in "([{":
                        pd += 1
                    elif tokens[k] in ")]}":
                        pd -= 1
                    elif tokens[k] == ";" and pd == 0:
                        break
                    k += 1
                yield fname, tokens[j:k]
                j = k + 1
                stmt_start = True
                continue
            stmt_start = False
            j += 1
        i = j


def _first_declarator(toks):
    """Index where the first declarator starts (its leading '*'/'(' included),
    or None for a declaration without one."""
    depth, stop = 0, len(toks)
    for k, t in enumerate(toks):
        if t in "([{":
            depth += 1
        elif t in ")]}":
            depth -= 1
        elif depth == 0 and t in ("=", ","):
            stop = k
            break
    head = toks[:stop]
    # the declarator name: identifier right after '(' '*' (pointer to function),
    # else the last identifier at bracket depth 0 that is not followed by '(' at a
    # depth where it would be a parameter
    name_at, depth = None, 0
    for k, t in enumerate(head):
        if t in "[(":
            if t == "(" and name_at is not None and depth == 0 and head[k + 1:k + 2] != ["*"]:
                break                   # parameter list after the name: function or fp
            depth += 1
            continue
        if t in "])":
            depth -= 1
            continue
        if k > 0 and head[k - 1] in ("struct", "union", "enum"):
            continue                    # a tag, not a declarator
        if re.match(r"^[A-Za-z_]\w*$", t) and t not in _QUALIFIERS and t not in _KEYWORDS \
                and (depth == 0 or (k >= 2 and head[k - 1] == "*" and head[k - 2] == "(")):
            name_at = k
    if name_at is None:
        return None
    k = name_at
    while k > 0 and head[k - 1] in ("*", "(") or (k > 0 and head[k - 1] in _QUALIFIERS
                                                   and k > 1 and head[k - 2] == "*"):
        k -= 1
    return k


def objects(text):
    """[Obj] for every object definition of the (preprocessed) source, in order."""
    types = _Types()
    out = []
    for func, toks in _declarations(text):
        if not toks:
            continue
        typedef = toks[0] == "typedef"
        if typedef:
            toks = toks[1:]
        if not typedef and ("extern" in toks[:3]):
            continue
        is_static = "static" in toks[:3]
        # an inline aggregate body in the base type: size unknown
        aggregate = False
        if "{" in toks:
            k = toks.index("{")
            if k > 0 and (toks[k - 1] in ("struct", "union", "enum") or toks[k - 2:k - 1] and
                          toks[k - 2] in ("struct", "union", "enum")) and "=" not in toks[:k]:
                d, j = 0, k
                while j < len(toks):
                    d += {"{": 1, "}": -1}.get(toks[j], 0)
                    j += 1
                    if d == 0:
                        break
                kind = toks[k - 1] if toks[k - 1] in ("struct", "union", "enum") else toks[k - 2]
                toks = toks[:k] + ["{...}"] + toks[j:]
                aggregate = kind != "enum"
        first = _first_declarator(toks)
        if first is None:
            continue
        base_words = [t for t in toks[:first] if t != "{...}"]
        b = None if aggregate else types.base(base_words)
        for p in _split_top(toks[first:]):
            name, pointer, dims, func_decl, init = _parse_declarator(p)
            if not name:
                continue
            if typedef:
                size = None if func_decl else ((4, 4) if pointer else b)
                for dim in dims:
                    v = _const(dim)
                    size = (size[0] * v, size[1]) if size and v is not None else None
                types.typedefs[name] = size
                continue
            if func_decl:
                continue
            size = (4, 4) if pointer else b
            for dim in dims:
                v = _const(dim) if dim else (_init_count(init) if init else None)
                size = (size[0] * v, size[1]) if size and v is not None else None
            section = ".data" if init else (".bss" if (is_static or func) else None)
            vals = _init_values(init) if init else None
            o = Obj(name, func, section, size[0] if size else None,
                    max(4, size[1]) if size else None, vals, not pointer)
            o.static = is_static or bool(func)
            out.append(o)
    # IDO drops a static object nothing references (checked on rocky: an unused
    # function-local static leaves no gap); a name seen only in its own
    # declaration counts as unreferenced.
    counts = {}
    for t in _TOKEN.findall(_strip(text)):
        counts[t] = counts.get(t, 0) + 1
    return [o for o in out if not (o.static and counts.get(o.name, 0) <= 1)]


def layout(text, elf):
    """Place the source's objects in the object's .data/.bss (see the module
    docstring); returns ([Obj] with offsets where placed, [problem strings])."""
    objs = objects(text)
    problems = []
    sec_size = {s["name"]: s["size"] for s in elf["sections"]}
    syms = {}
    for s in elf["symbols"]:
        if s["type"] == 1 and s["section"] in (".data", ".bss") and s["name"]:
            syms[s["name"]] = (s["section"], s["value"])
    data_off = next((s["offset"] for s in elf["sections"] if s["name"] == ".data"), None)
    for sec in (".data", ".bss"):
        cursor, ok = 0, True
        for o in objs:
            if o.section != sec:
                continue
            if not ok:
                break
            if o.size is None:
                ok = False
                problems.append("%s: size of %s unknown; later %s objects not placed" % (sec, o.key(), sec))
                if o.function is None and o.name in syms:
                    pass
                break
            off = (cursor + o.align - 1) // o.align * o.align
            if o.function is None and o.name in syms:
                real = syms[o.name][1]
                if real != off:
                    problems.append("%s: %s at 0x%X, computed 0x%X: layout of %s rejected" % (
                        sec, o.name, real, off, sec))
                    ok = False
                    break
            o.offset = off
            cursor = off + o.size
        if cursor > sec_size.get(sec, 0) and ok:
            problems.append("%s: computed end 0x%X past the section (0x%X): layout rejected" % (
                sec, cursor, sec_size.get(sec, 0)))
            ok = False
        if not ok:
            for o in objs:
                if o.section == sec:
                    o.offset = None
    # initialiser check (scalars and arrays of 1/2/4-byte integers in .data)
    if data_off is not None:
        raw = elf["_data"][data_off:data_off + sec_size.get(".data", 0)] if "_data" in elf else None
        for o in objs:
            if raw is None or o.section != ".data" or o.offset is None or not o.init or not o.size:
                continue
            n = len(o.init)
            el = o.size // n if n and o.size % n == 0 else 0
            if el not in (1, 2, 4):
                continue
            fmt = {1: ">b", 2: ">h", 4: ">i"}[el]
            for i, v in enumerate(o.init):
                at = o.offset + i * el
                if at + el > len(raw):
                    break
                got = struct.unpack_from(fmt, raw, at)[0]
                if got & ((1 << (8 * el)) - 1) != v & ((1 << (8 * el)) - 1):
                    problems.append(".data: %s+%d holds 0x%X, initialiser 0x%X: layout of .data rejected" % (
                        o.key(), i * el, got & ((1 << (8 * el)) - 1), v & ((1 << (8 * el)) - 1)))
                    for x in objs:
                        if x.section == ".data":
                            x.offset = None
                    return objs, problems
    return objs, problems


def statics_by_key(objs):
    """{(section, start): (name, size, function)} for placed objects without
    an ELF symbol (function-local statics and file-scope statics)."""
    return {(o.section, o.offset): (o.name, o.size, o.function) for o in objs
            if o.offset is not None and o.section and (o.function or True)}
