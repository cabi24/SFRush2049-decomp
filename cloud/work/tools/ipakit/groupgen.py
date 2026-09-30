#!/usr/bin/env python3
"""groupgen.py: propose and emit an IPA group directory from seed function names (stdlib only).

    python3 cloud/work/tools/ipakit/groupgen.py SEED... [--mode direct|chain] [--name NAME] [--out DIR]
                                                [--m2c auto|on|off] [--no-compile] [--force] [--json] [--plan]
    python3 cloud/work/tools/ipakit/groupgen.py --compare GROUP... [--json]     regenerate + diff real groups

Pipeline:  deps.py closure (mode direct|chain)  ->  roots / helpers / stand-ins  ->  sigs.py prototypes
           ->  group.json + group.c  ->  builder.compile_group (skipped without IDO)  ->  report.

Root vs helper decision per closure node (evidence from the retail words, see deps.py / analyze.py):
  * address-taken (code `lui/addiu` of its address or a data word)        -> keep   (root, ABI)
  * IPA leaf (reads non-ABI registers at entry / writes s-regs unsaved)   -> NEVER keep: it needs IPA registers,
        every caller must be in the group; with fewer than --min-sites (3) call sites inside the group it gets a
        STAND-IN caller (`__standin_NAME`, --standin-sites (2) calls, itself in keep) so -O3 does not inline it
  * everything else (ABI: saves its own s-registers and is called normally, or an IPA caller)
        -> keep: it keeps the standard calling convention for the callers outside the group.
Callers of IPA leaves that the closure pulls in are members too (real code that must match); callees outside the
closure are declared only (static library and ABI callees).
Seeds inside UNREGISTERED heads (func_80107EDC ...) need no special handling: the corpus is loaded with the
heads audit (`load_corpus(heads=True)`), the extent comes from the inflated image, and group.json gets a
`targets` entry ({name: {"addr", "words"}}) for each such function as builder.extra_targets expects.

Bodies come from m2c (the repository's tools/mips_to_c with tools/m2c_patches applied to a private copy under
build/groupgen_m2c/, IPA register map passed through M2C_IPA_REGS in the order sigs.py infers); the m2c signature
is replaced by the sigs.py prototype and call sites of IPA callees are re-ordered to that order.  Without m2c (or
with --m2c off) every body is a stub marked TODO.  Raw `*(T *)0x80xxxxxx` accesses become `extern` D_XXXXXXXX
declarations.  The skeleton is a STARTING POINT: it compiles, it does not match.
"""
import argparse
import copy
import json
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from ipakit import ROOT, IMAGE_BASE  # noqa: E402
from ipakit import deps, sigs  # noqa: E402
from ipakit.analyze import CALLEE_SAVED  # noqa: E402
from ipakit.mipsdec import mask_names  # noqa: E402

GROUPS_DIR = ROOT / 'cloud' / 'work' / 'ipa-groups'
DEFAULT_FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
M2C_SRC = ROOT / 'tools' / 'mips_to_c'
M2C_PATCHES = ROOT / 'tools' / 'm2c_patches'
M2C_WORK = ROOT / 'build' / 'groupgen_m2c'
MIN_SITES = 3
STANDIN_SITES = 2

PRELUDE_MIN = """typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef double f64;
#define NULL ((void *)0)
"""
M2C_PRELUDE = '#define NULL ((void *)0)\n#define TRUE 1\n#define FALSE 0\n'
INTRINSICS = """float fabsf(float);
float sqrtf(float);
#pragma intrinsic (fabsf)
#pragma intrinsic (sqrtf)
"""


# ---------------------------------------------------------------------------------------------------------
# plan
# ---------------------------------------------------------------------------------------------------------

class Node:
    def __init__(self, name):
        self.name = name
        self.cls = 'ABI'
        self.why = []               # closure reasons
        self.decision = 'keep'      # keep | internal | standin
        self.reason = ''
        self.in_sites = 0
        self.in_callers = []
        self.ext_callers = []
        self.addr_taken = False
        self.saved_s = []
        self.discovered = False

    def to_json(self):
        return {'name': self.name, 'class': self.cls, 'decision': self.decision, 'reason': self.reason,
                'in_group_call_sites': self.in_sites, 'in_group_callers': self.in_callers,
                'external_callers': self.ext_callers[:8], 'address_taken': self.addr_taken,
                'saved_s': self.saved_s, 'closure_reasons': self.why[:4]}


class Plan:
    def __init__(self):
        self.seeds = []
        self.mode = 'direct'
        self.nodes = {}             # name -> Node, seeds first then by address
        self.keep = []              # roots
        self.standins = []          # names that get a stand-in
        self.notes = []
        self.targets = {}
        self.truncated = False

    def keep_list(self):
        """group.json keep: roots then stand-ins (a stand-in is itself a kept external function)."""
        return list(self.keep) + ['__standin_%s' % n for n in self.standins]

    def to_json(self):
        return {'seeds': self.seeds, 'mode': self.mode, 'members': list(self.nodes), 'keep': self.keep_list(),
                'standins': self.standins, 'targets': self.targets, 'notes': self.notes,
                'nodes': {n: v.to_json() for n, v in self.nodes.items()}}


def make_plan(seeds, mode='direct', dmodel=None, min_sites=MIN_SITES, max_funcs=400):
    dm = dmodel or deps.model_cache()
    plan = Plan()
    plan.mode = mode
    c = dm.corpus
    clean = []
    for s in seeds:
        if s not in c.funcs:
            raise SystemExit('unknown function %s (not in asm/us/blob and not a discovered head; see heads.py)' % s)
        f = c.funcs[s]
        if f.tail_of:
            plan.notes.append('%s is a label inside %s, not a head: using %s' % (s, f.tail_of, f.tail_of))
            s = f.tail_of
        if s not in clean:
            clean.append(s)
    plan.seeds = clean
    reasons, notes = dm.minimal_closure(clean, mode, max_funcs=max_funcs)
    plan.truncated = bool(notes.get('truncated'))
    if plan.truncated:
        plan.notes.append('closure truncated at %d functions: the real closure is larger' % max_funcs)
    for u in notes.get('unresolved_targets', [])[:5]:
        plan.notes.append('unresolved call target 0x%08X in %s (opaque image run)' % (u[2], u[0]))
    if notes.get('indirect_calls'):
        plan.notes.append('indirect (jalr) calls in: %s' % ', '.join(sorted(notes['indirect_calls'])[:6]))
    names = [n for n in reasons if not c.funcs[n].tail_of]
    order = clean + sorted((n for n in names if n not in clean), key=lambda n: c.funcs[n].addr)
    nodeset = set(order)
    for n in order:
        nd = Node(n)
        nd.why = reasons.get(n, [])
        nd.cls, _ = dm.classify(n)
        nd.discovered = c.funcs[n].discovered
        info = dm.infos[n]
        nd.saved_s = mask_names(info.saved & CALLEE_SAVED)
        cal = dm.callers.get(n, {})
        nd.in_callers = sorted(x for x in cal if x in nodeset and x != n)
        nd.ext_callers = sorted(x for x in cal if x not in nodeset and x != n)
        nd.in_sites = sum(len(cal[x]) for x in nd.in_callers)
        nd.addr_taken = n in dm.addr_taken
        leaf = nd.cls in ('IPA-leaf', 'IPA-both')
        if nd.addr_taken:
            nd.decision, nd.reason = 'keep', 'address-taken root (%s)' % '; '.join(dm.addr_taken[n][:2])
        elif leaf:
            if nd.in_sites < min_sites:
                nd.decision = 'standin'
                nd.reason = 'IPA leaf with %d in-group call site(s): stand-in caller keeps it out of line' % nd.in_sites
            else:
                nd.decision, nd.reason = 'internal', 'IPA leaf with %d in-group call sites' % nd.in_sites
            if nd.ext_callers:
                plan.notes.append('%s is an IPA leaf but %d caller(s) lie outside the group (%s): they must set its '
                                  'registers too (closure gap or path-correlated liveness)' % (
                                      n, len(nd.ext_callers), ', '.join(nd.ext_callers[:3])))
        else:
            why = []
            if nd.ext_callers:
                why.append('%d external caller(s)' % len(nd.ext_callers))
            if not nd.in_callers and not nd.ext_callers:
                why.append('no known callers (entry point)')
            if nd.saved_s:
                why.append('saves %s itself' % ','.join(nd.saved_s[:3]))
            nd.decision = 'keep'
            nd.reason = 'ABI root: ' + (', '.join(why) if why else 'standard calling convention (%s)' % nd.cls)
        plan.nodes[n] = nd
        if nd.decision == 'keep':
            plan.keep.append(n)
        elif nd.decision == 'standin':
            plan.standins.append(n)
        if nd.discovered:
            f = c.funcs[n]
            plan.targets[n] = {'addr': '0x%08X' % f.addr, 'words': len(f.words)}
    return plan


# ---------------------------------------------------------------------------------------------------------
# m2c
# ---------------------------------------------------------------------------------------------------------

def ensure_m2c():
    """Path of a patched private m2c copy (or None).  The submodule tree is never touched."""
    override = os.environ.get('GROUPGEN_M2C')
    if override and (Path(override) / 'm2c.py').exists():
        return Path(override)
    if not (M2C_SRC / 'm2c.py').exists():
        return None
    marker = M2C_WORK / '.patched'
    if not marker.exists():
        try:
            shutil.rmtree(M2C_WORK, ignore_errors=True)
            M2C_WORK.mkdir(parents=True)
            shutil.copytree(M2C_SRC / 'm2c', M2C_WORK / 'm2c', ignore=shutil.ignore_patterns('__pycache__'))
            shutil.copytree(M2C_SRC / 'm2c_pycparser', M2C_WORK / 'm2c_pycparser', ignore=shutil.ignore_patterns('__pycache__'))
            for f in ('m2c.py', 'm2c_macros.h'):
                shutil.copy(M2C_SRC / f, M2C_WORK / f)
            subprocess.run(['git', 'init', '-q'], cwd=M2C_WORK, capture_output=True)   # so `git apply` uses this directory as root
            for p in sorted(M2C_PATCHES.glob('*.patch')):
                r = subprocess.run(['git', 'apply', '--include=m2c/*', '--include=m2c.py', str(p)], cwd=M2C_WORK,
                                   capture_output=True, text=True)
                if r.returncode != 0:
                    raise RuntimeError('patch %s: %s' % (p.name, r.stderr.strip()[:200]))
            if not (M2C_WORK / 'm2c' / 'rush_ipa.py').exists():
                raise RuntimeError('IPA patch did not apply')
            marker.write_text('ok\n')
        except (OSError, RuntimeError) as exc:
            shutil.rmtree(M2C_WORK, ignore_errors=True)
            sys.stderr.write('groupgen: m2c unavailable (%s)\n' % exc)
            return None
    return M2C_WORK


_ctx_cache = {}


def m2c_context():
    """(path, text) of the preprocessed type context (autodecomp._context(include_protos=False))."""
    if 'c' not in _ctx_cache:
        sys.path.insert(0, str(ROOT))
        try:
            from tools.conveyor.pipeline import autodecomp as ad
            _ctx_cache['c'] = ad._context(include_protos=False)
        except Exception as exc:                # noqa: BLE001 - tooling optional
            sys.stderr.write('groupgen: no m2c context (%s)\n' % exc)
            _ctx_cache['c'] = (None, '')
    return _ctx_cache['c']


def disasm_for_m2c(corpus, f):
    """(asm text for m2c, observations) for a corpus function, from its retail words."""
    sys.path.insert(0, str(ROOT))
    from tools.conveyor.pipeline import disasm
    with tempfile.TemporaryDirectory(prefix='gg-') as tmp:
        b = Path(tmp) / 'f.bin'
        b.write_bytes(struct.pack('>%dI' % len(f.words), *f.words))
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-b', 'binary', '-m', 'mips:4300', '-EB',
                              '--adjust-vma=0x%x' % f.addr, str(b)], capture_output=True, text=True).stdout
    targets = {a: n for n, a in corpus.symbols.items() if IMAGE_BASE <= a < IMAGE_BASE + 4 * len(corpus.image)}
    obs = []
    asm = disasm.normalize_objdump(out, f.name, targets, observations=obs)
    return asm, obs


def run_m2c(m2c_dir, asm, name, ctx_path, ipa_map_path):
    with tempfile.TemporaryDirectory(prefix='gg-') as tmp:
        s = Path(tmp) / (name + '.s')
        s.write_text(asm)
        cmd = [sys.executable, str(m2c_dir / 'm2c.py'), str(s), '-f', name, '--valid-syntax']
        if ctx_path:
            cmd += ['--context', ctx_path]
        env = dict(os.environ, M2C_IPA_REGS=str(ipa_map_path))
        try:
            p = subprocess.run(cmd, capture_output=True, text=True, timeout=120, env=env)
        except subprocess.TimeoutExpired:
            return None
    body = p.stdout.strip()
    if p.returncode != 0 or not body or body.startswith('def '):
        return None
    return body


def split_args(text):
    out, depth, cur = [], 0, ''
    for ch in text:
        if ch in '([':
            depth += 1
        elif ch in ')]':
            depth -= 1
        if ch == ',' and depth == 0:
            out.append(cur.strip())
            cur = ''
        else:
            cur += ch
    if cur.strip() or out:
        out.append(cur.strip())
    return out


def find_calls(body, name):
    """[(start, end, args_text)] of each call `name(...)` in body (balanced parens)."""
    res = []
    for m in re.finditer(r'\b%s\s*\(' % re.escape(name), body):
        i = m.end()
        depth = 1
        j = i
        while j < len(body) and depth:
            depth += {'(': 1, ')': -1}.get(body[j], 0)
            j += 1
        if depth == 0:
            res.append((m.start(), j, body[i:j - 1]))
    return res


def reorder_calls(body, callee, csig, arity=None):
    """Rewrite m2c call sites of `callee` (ABI args in a-register order, then IPA registers in csig order) into the
    sigs.py parameter order.  -> (new body, n_rewritten, n_todo)."""
    ipa = [p.reg for p in csig.params if p.reg and p.reg not in sigs.A_REGS and p.reg not in ('f12', 'f14')]
    if not ipa:
        # plain ABI callee: m2c passes a0..a3 (the registers the caller set); fit the count to the inferred arity
        if arity is None and (any(p.reg is None and p.source.startswith('stack-load') for p in csig.params) or len(csig.params) > 4):
            return body, 0, 0
        n_par = len(csig.params) if arity is None else arity
        rew = 0
        for start, end, args_text in reversed(find_calls(body, callee)):
            line_start = body.rfind('\n', 0, start) + 1
            tail = body[end:end + 3].lstrip()
            if re.match(r'^\s*[A-Za-z_][\w \*]*$', body[line_start:start]) and (tail.startswith('{') or tail.startswith(';')):
                continue
            args = split_args(args_text) if args_text.strip() else []
            if len(args) != n_par:
                args = (args + ['0'] * n_par)[:n_par]
                body = body[:start] + '%s(%s)' % (callee, ', '.join(args)) + body[end:]
                rew += 1
        return body, rew, 0
    if any(p.reg is None and p.source.startswith('stack-load') for p in csig.params) or any(p.reg in ('f12', 'f14') for p in csig.params):
        return body, 0, len(find_calls(body, callee))
    calls = find_calls(body, callee)
    rew = todo = 0
    for start, end, args_text in reversed(calls):
        # skip the definition line `type callee(params) {` and prototypes
        tail = body[end:end + 3].lstrip()
        line_start = body.rfind('\n', 0, start) + 1
        if re.match(r'^\s*[A-Za-z_][\w \*]*$', body[line_start:start]) and (tail.startswith('{') or tail.startswith(';')):
            continue
        args = split_args(args_text)
        k = len(args) - len(ipa)
        if k < 0 or k > 4:
            todo += 1
            continue
        by_reg = {sigs.A_REGS[i]: args[i] for i in range(k)}
        for j, r in enumerate(ipa):
            by_reg[r] = args[k + j]
        new = []
        ok = True
        for p in csig.params:
            if p.reg is not None:
                new.append(by_reg.get(p.reg, '0'))
            else:
                new.append('0')
        if not ok:
            todo += 1
            continue
        body = body[:start] + '%s(%s)' % (callee, ', '.join(new)) + body[end:]
        rew += 1
    return body, rew, todo


_RAW = re.compile(r'\*\((s8|u8|s16|u16|s32|u32|f32|f64|void) \*\)0x(80[0-9A-Fa-f]{6})\b')
_DECL_LINE = re.compile(r'(?m)^[A-Za-z_][\w \t\*]*\b\w+\s*\([^{};]*\)\s*;[ \t]*(?:/\*.*?\*/)?[ \t]*\n')
_EXTERN_LINE = re.compile(r'(?m)^extern\s+[^;\n]*?\b(\w+)\s*(?:\[[^\]]*\])?\s*;[ \t]*\n')


def raw_to_externs(body, obs_types, decls):
    def sub(m):
        t, a = m.group(1), m.group(2).upper()
        t = 's32' if t == 'void' else t
        name = 'D_%s' % a
        if name not in decls:
            decls[name] = (t, False)
        return name
    return _RAW.sub(sub, body)


_ACC_TYPE = {'lb': 's8', 'lbu': 'u8', 'sb': 'u8', 'lh': 's16', 'lhu': 'u16', 'sh': 'u16', 'lw': 's32', 'sw': 's32',
             'lwc1': 'f32', 'swc1': 'f32', 'ldc1': 'f64', 'sdc1': 'f64'}


def access_types(observations):
    """{address: most common scalar type} from normalize_objdump observations (kind == access)."""
    cnt = {}
    for o in observations:
        if o.get('kind') != 'access':
            continue
        t = _ACC_TYPE.get(o['mnemonic'])
        if t:
            cnt.setdefault(o['address'], {}).setdefault(t, 0)
            cnt[o['address']][t] += 1
    return {a: max(v.items(), key=lambda kv: kv[1])[0] for a, v in cnt.items()}


# ---------------------------------------------------------------------------------------------------------
# emission
# ---------------------------------------------------------------------------------------------------------

def _zero_for(p):
    return {'f32': '0.0f', 'f64': '0.0'}.get(p.kind, '0')


def _stub_body(sig, name, words, addr):
    ret = sig.ret_ctype()
    lines = ['/* TODO: decompile %s (%d words at 0x%08X); the m2c seed was unavailable */' % (name, words, addr)]
    if ret == 'void':
        body = '{\n}\n'
    elif ret == 'f32':
        body = '{\n    return 0.0f;\n}\n'
    elif ret == 'f64':
        body = '{\n    return 0.0;\n}\n'
    else:
        body = '{\n    return 0;\n}\n'
    return '\n'.join(lines) + '\n' + sig.prototype('m2c').rstrip(';') + ' ' + body


def standin_text(name, sig, sites):
    calls = '\n'.join('    %s(%s);' % (name, ', '.join(_zero_for(p) for p in sig.params)) for _ in range(sites))
    return ('/* stand-in caller: %d call sites keep %s out of line under -O3 (never spliced) */\n'
            'void __standin_%s(void)\n{\n%s\n}\n' % (sites, name, name, calls))


DEAD_SWITCH = """    if (0) {
        switch (0) {
        case 0: break;
        case 1: break;
        case 2: break;
        }
    }
"""


_MISSING_LOCAL = re.compile(r'\b(?:unk)?sp[0-9A-Fa-f]{1,4}\b|\bsaved_reg_\w+\b')


def declare_missing(body):
    """m2c leaves stack slots (`sp1A0`, `unksp48`) and saved registers undeclared in valid-syntax mode."""
    head_end = body.find('{')
    if head_end < 0:
        return body
    names = sorted(set(_MISSING_LOCAL.findall(body[head_end:])))
    decl = []
    for n in names:
        if re.search(r'(?m)^\s+(?!return\b|else\b|goto\b|case\b)[A-Za-z_][\w ]*?[\s\*]+%s\s*(?:\[[^\]]*\])?\s*(?:;|=)' % re.escape(n),
                     body[head_end:]):
            continue
        arr = re.search(r'\b%s\s*\[' % re.escape(n), body) or re.search(r'&\(&%s' % re.escape(n), body)
        decl.append('    s32 %s%s;' % (n, '[64]' if arr else ''))
    if not decl:
        return body
    return body[:head_end + 1] + '\n' + '\n'.join(decl) + body[head_end + 1:]


def replace_signature(body, name, sig):
    """Replace m2c's `type name(params) {` head with the sigs.py prototype (names arg<N>/ipa_<reg>)."""
    m = re.search(r'(?m)^[^\n;{}]*\b%s\s*\([^;{}]*\)\s*\{' % re.escape(name), body)
    if not m:
        return body, False
    head = sig.prototype('m2c').rstrip(';') + ' {'
    return body[:m.start()] + head + body[m.end():], True


def build_sources(plan, dm, sm, m2c='auto', dead_switch=False, log=None, stub=()):
    """-> (group.c text, info dict)"""
    log = log or (lambda *a: None)
    c = dm.corpus
    sgs = {n: copy.copy(sm.sig(n)) for n in plan.nodes}
    for v in sgs.values():
        v.override = {}
    m2c_dir = None
    ctx_path, ctx_text = (None, '')
    if m2c != 'off':
        m2c_dir = ensure_m2c()
        if m2c_dir is not None:
            ctx_path, ctx_text = m2c_context()
        if m2c == 'on' and m2c_dir is None:
            raise SystemExit('--m2c on: m2c is not usable')
    ctx_text_full = ctx_text
    # the hand-authored context (include/game_types.h) declares many game functions; the group defines its own
    # members, so those lines go (a redeclaration with another prototype is a cfe error)
    if ctx_text:
        member_rx = re.compile(r'(?m)^\s*extern\b[^\n;]*\b(?:%s)\s*\([^\n]*\)\s*;[ \t]*$' % '|'.join(
            re.escape(n) for n in plan.nodes))
        ctx_text = member_rx.sub('', ctx_text)

    def ctx_arity(name):
        m = re.search(r'(?m)^\s*extern\b[^\n;]*\b%s\s*\(([^\n]*)\)\s*;' % re.escape(name), ctx_text)
        if not m:
            return None
        raw = m.group(1).strip()
        if raw in ('', 'void'):
            return 0
        if '...' in raw:
            return None
        return len(split_args(raw))
    ipa_map = {n: [p.reg for p in s.params if p.reg and p.reg not in sigs.A_REGS and p.reg not in ('f12', 'f14')]
               for n, s in sgs.items()}
    # callees outside the group are named in the map too (their IPA registers decide call-site arguments)
    callee_names = set()
    for n in plan.nodes:
        for s in dm.infos[n].sites:
            if s.callee and s.callee not in plan.nodes:
                callee_names.add(s.callee)
    callee_sigs = {n: sm.sig(n) for n in sorted(callee_names) if n in sm.infos}
    for n, s in callee_sigs.items():
        regs = [p.reg for p in s.params if p.reg and p.reg not in sigs.A_REGS and p.reg not in ('f12', 'f14')]
        if regs:
            ipa_map[n] = regs
    decls = {}
    bodies = {}
    ext_lines = {}                  # extern data lines m2c wrote in the bodies, hoisted (first wins)
    info = {'m2c': {}, 'todo_calls': 0, 'reordered_calls': 0, 'stubbed': []}
    obs_all = []
    with tempfile.TemporaryDirectory(prefix='gg-') as tmp:
        mp = Path(tmp) / 'ipa_map.json'
        mp.write_text(json.dumps({k: ['fp' if r == 's8' else r for r in v] for k, v in ipa_map.items() if v}))
        for n in plan.nodes:
            f = c.funcs[n]
            sig = sgs[n]
            body = None
            if m2c_dir is not None:
                try:
                    asm, obs = disasm_for_m2c(c, f)
                    obs_all.extend(obs)
                    body = run_m2c(m2c_dir, asm, n, ctx_path, mp)
                except Exception as exc:        # noqa: BLE001
                    log('m2c failed for %s: %s' % (n, exc))
                    body = None
            if body is not None:
                lines = [l for l in body.splitlines() if not l.lstrip().startswith(('Warning:', 'Error:', 'GLOBAL_ASM'))]
                body = '\n'.join(lines)
                body = re.sub(r'([(,]\s*)\?(\s+\w)', r'\1s32\2', body).replace('(? (*)', '(s32 (*)')
                body = _DECL_LINE.sub('', body + '\n').rstrip('\n')          # m2c's own prototypes: replaced by ours
                body = _EXTERN_LINE.sub(lambda m: ext_lines.setdefault(m.group(1), m.group(0).strip()) and '', body + '\n').rstrip('\n')
                pd = sigs.parse_definition(body, n)
                if pd is not None:
                    mt = {pn: t for t, pn in pd[1]}
                    # parameters m2c declares that the retail-evidence signature lacks (written before read, unused)
                    have = {sig.m2c_name(p, i) for i, p in enumerate(sig.params)}
                    for t, pn in pd[1]:
                        mm = re.fullmatch(r'arg(\d+)', pn)
                        if pn in have or not mm or int(mm.group(1)) >= sigs.MAX_SLOT:
                            continue
                        k = int(mm.group(1))
                        q = sigs.Param(reg=sigs.A_REGS[k] if k < 4 else None, slot=k, source='m2c-extra' if k < 4 else 'stack-load')
                        q.kind = 'ptr' if '*' in t else ('f32' if 'f32' in t else 'int')
                        q.guessed = True
                        q.evidence.append('m2c declares %s; no retail evidence for it' % pn)
                        if any(x.slot == k for x in sig.params):
                            continue
                        sig.params.append(q)
                        sig.params.sort(key=lambda x: x.slot)
                        sig.ipa_regs = [x.reg for x in sig.params if x.reg and x.reg not in sigs.A_REGS and x.reg not in ('f12', 'f14')]
                    for i, p in enumerate(sig.params):
                        t = mt.get(sig.m2c_name(p, i), '')
                        if p.kind == 'ptr' and '*' in t and '(' not in t:
                            sig.override[i] = t.strip()
                if sig.ret == 'void' and re.search(r'\breturn\s+[^;\s]', body):
                    sig.ret = 's32'
                    sig.ret_evidence.append('m2c body returns a value although no caller reads it')
                body, ok = replace_signature(body, n, sig)
                if ok:
                    body = declare_missing(body)
                else:
                    body = None
            if body is not None and n in stub:
                body = None
                info['stubbed'].append(n)
            info['m2c'][n] = body is not None
            if body is None:
                body = _stub_body(sig, n, len(f.words), f.addr)
            bodies[n] = body
        types = access_types(obs_all)
        for n in list(bodies):
            b = bodies[n]
            for callee, cs in list(sgs.items()) + list(callee_sigs.items()):
                if callee == n:
                    continue
                b, rew, todo = reorder_calls(b, callee, cs, ctx_arity(callee) if callee in callee_sigs else None)
                info['reordered_calls'] += rew
                info['todo_calls'] += todo
                if todo:
                    b = '/* TODO: %d call(s) to %s keep m2c argument order (stack/float IPA params) */\n' % (todo, callee) + b
            b = raw_to_externs(b, types, decls)
            if dead_switch and n in plan.standins and info['m2c'][n]:
                i = b.find('{', b.find(n + '('))
                if i >= 0:
                    b = b[:i + 1] + '\n' + DEAD_SWITCH.rstrip('\n') + b[i + 1:]
            bodies[n] = b
    # externs for D_ tokens the bodies use (m2c names or the conversion above)
    text_all = '\n'.join(bodies.values())
    for tok in sorted(set(re.findall(r'\bD_[0-9A-F]{8}\b', text_all))):
        if tok not in decls and tok not in ext_lines:
            a = int(tok[2:], 16)
            decls[tok] = (types.get(a, 's32'), bool(re.search(r'\b%s\s*\[' % tok, text_all)))
    known = {k: v for k, v in c.symbols.items()}
    for tok in sorted(set(re.findall(r'\b[A-Za-z_]\w*\b', text_all))):
        a = known.get(tok)
        if a is None or tok in decls or tok in plan.nodes or tok in callee_sigs:
            continue
        if re.search(r'\b%s\b' % re.escape(tok), ctx_text or ''):
            continue
        if a in c.by_addr or IMAGE_BASE <= a < IMAGE_BASE + 4 * len(c.image) and c.containing(a) is not None:
            continue
        if tok.startswith('func_') or re.search(r'\b%s\s*\(' % re.escape(tok), text_all):
            continue
        decls[tok] = (types.get(a, 's32'), bool(re.search(r'\b%s\s*\[' % re.escape(tok), text_all)))
    # prototypes: group functions, then the game callees their bodies mention
    protos = ['%s' % sgs[n].prototype('m2c') for n in plan.nodes]
    mentioned = [n for n in callee_sigs if re.search(r'\b%s\s*\(' % re.escape(n), text_all)
                 and ctx_arity(n) is None]
    # out-of-group callees: a full prototype only when a float parameter needs it (K&R would promote f32 to double),
    # otherwise an unprototyped declaration: m2c's call sites and ours can disagree on argument types and counts
    def _callee_decl(n):
        sg = callee_sigs[n]
        if any(p.kind in ('f32', 'f64') for p in sg.params) or any(p.reg and p.reg not in sigs.A_REGS and p.reg not in ('f12', 'f14')
                                                                  for p in sg.params):
            return sg.prototype('m2c')
        return '%s %s();' % (sg.ret_ctype(), n)
    cproto = [_callee_decl(n) for n in mentioned]
    out = []
    out.append('/* generated by cloud/work/tools/ipakit/groupgen.py: seeds %s, mode %s.  SKELETON: compiles, does not match.\n'
               ' * Prototypes come from ipakit/sigs.py (home-slot order; IPA registers as ipa_<reg>); bodies are m2c seeds\n'
               ' * (m2c: %d/%d) or TODO stubs.  Stand-ins at the end keep IPA functions out of line and are never spliced. */' % (
                   ', '.join(plan.seeds), plan.mode, sum(info['m2c'].values()), len(info['m2c'])))
    out.append('/* flags: %s */' % DEFAULT_FLAGS)
    if ctx_text:
        out.append(M2C_PRELUDE + ctx_text.strip() + '\n')
        try:
            out.append((M2C_SRC / 'm2c_macros.h').read_text())
        except OSError:
            pass
    else:
        out.append(PRELUDE_MIN)
    out.append(INTRINSICS)
    if decls or ext_lines:
        out.append('/* referenced data (types guessed from access width) */')
        for name in sorted(ext_lines):
            if not re.search(r'(?m)^\s*extern\b[^;\n]*\b%s\b' % re.escape(name), ctx_text or ''):
                out.append(ext_lines[name])
        for name in sorted(decls):
            if name in ext_lines or re.search(r'(?m)^\s*extern\b[^;\n]*\b%s\b' % re.escape(name), ctx_text or ''):
                continue
            t, arr = decls[name]
            out.append('extern %s %s%s;' % (t, name, '[]' if arr else ''))
        out.append('')
    if cproto:
        out.append('/* callees outside the group */')
        out.extend(cproto)
        out.append('')
    out.append('/* group functions */')
    out.extend(protos)
    out.append('')
    body_idx = {}
    for n in plan.nodes:
        body_idx[n] = len(out)
        out.append(bodies[n].rstrip() + '\n')
    for n in plan.standins:
        out.append(standin_text(n, sgs[n], STANDIN_SITES_CFG[0]))
    info['externs'] = len(decls)
    info['m2c_available'] = m2c_dir is not None
    # line span of each body in the emitted file (for the error -> function fallback)
    starts = {}
    line = 1
    for i, chunk in enumerate(out):
        for n, bi in body_idx.items():
            if bi == i:
                starts[n] = (line, line + chunk.count('\n'))
        line += chunk.count('\n') + 1
    info['spans'] = starts
    return '\n'.join(out) + '\n', info


STANDIN_SITES_CFG = [STANDIN_SITES]


def group_json(plan, name, generated=''):
    return {
        'members': list(plan.nodes),
        'files': ['group.c'],
        'keep': plan.keep_list(),
        'flags': DEFAULT_FLAGS,
        'generated': generated or 'groupgen.py seeds=%s mode=%s' % (','.join(plan.seeds), plan.mode),
        'unprototyped': [],
        'context': [],
        'claims': [],
        **({'targets': plan.targets} if plan.targets else {}),
    }


def emit(plan, out_dir, dm=None, sm=None, m2c='auto', dead_switch=False, force=False, log=None, stub=()):
    dm = dm or deps.model_cache()
    sm = sm or sigs.SigModel(dm.corpus, dm.infos)
    out_dir = Path(out_dir)
    if out_dir.exists() and any(out_dir.iterdir()) and not force:
        raise SystemExit('%s exists and is not empty (use --force, or --out to write elsewhere)' % out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)
    text, info = build_sources(plan, dm, sm, m2c=m2c, dead_switch=dead_switch, log=log, stub=stub)
    (out_dir / 'group.c').write_text(text)
    (out_dir / 'group.json').write_text(json.dumps(group_json(plan, out_dir.name), indent=2) + '\n')
    return info


def failing_functions(err, spans):
    """Functions whose body span contains a `cfe: Error` line of a compile error text."""
    bad = set()
    for m in re.finditer(r'cfe: Error: group\.c, line (\d+)', err or ''):
        ln = int(m.group(1))
        for n, (a, b) in spans.items():
            if a <= ln <= b:
                bad.add(n)
    return bad


def emit_and_compile(plan, out_dir, dm=None, sm=None, m2c='auto', dead_switch=False, force=False, log=None,
                     compile_=True, fallback=True):
    """emit(), compile, and (fallback) replace the m2c bodies that cfe rejects by TODO stubs, up to 3 rounds."""
    stub = set()
    info = emit(plan, out_dir, dm, sm, m2c=m2c, dead_switch=dead_switch, force=force, log=log)
    res = try_compile(out_dir) if compile_ else None
    rounds = 0
    while res is not None and res.get('err') and fallback and rounds < 3:
        bad = failing_functions(res['err'], info['spans']) - stub
        if not bad:
            break
        stub |= bad
        rounds += 1
        info = emit(plan, out_dir, dm, sm, m2c=m2c, dead_switch=dead_switch, force=True, log=log, stub=stub)
        res = try_compile(out_dir)
    info['stubbed'] = sorted(stub)
    return info, res


def try_compile(out_dir):
    sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
    from amatch import builder
    if not builder.ido_available():
        return {'skipped': 'IDO missing (tools/cloud/setup.sh)'}
    r = builder.compile_group(str(out_dir), use_cache=False)
    out = {'err': r.get('err'), 'matched': r.get('matched'), 'secs': round(r.get('secs', 0), 1), 'members': {}}
    for n, m in (r.get('members') or {}).items():
        out['members'][n] = {'strict_diff': m.get('strict_diff'), 'target_size': m.get('target_size'), 'size': m.get('size'),
                             'err': m.get('err'), 'unresolved': len(m.get('unresolved') or [])}
    return out


# ---------------------------------------------------------------------------------------------------------
# compare with real groups
# ---------------------------------------------------------------------------------------------------------

def parse_protos(text, names):
    """{name: [param class list]} for the prototypes or definitions of `names` found in C text."""
    out = {}
    for n in names:
        pd = sigs.parse_definition(text, n)
        if pd is None:
            m = re.search(r'(?m)^[ \t]*([A-Za-z_][\w \t\*]*?)\b%s[ \t]*\(([^;{}]*?)\)\s*;' % re.escape(n), text)
            if m:
                raw = m.group(2).strip()
                ps = [] if raw in ('', 'void') else [(t.rsplit(None, 1)[0] if ' ' in t.strip() else t, '') for t in sigs._split_params(raw)]
                pd = (m.group(1).strip(), ps, raw == '')
        if pd is not None:
            out[n] = {'ret': sigs._ret_class(pd[0]), 'params': [sigs.type_class(t) for t, _ in pd[1]], 'kr': pd[2]}
    return out


def compare_group(gname, seeds_mode='members', mode='direct', dm=None, sm=None, plan_min_sites=MIN_SITES):
    """Regenerate the plan + prototypes for a real group and diff it with the real group.json / group.c."""
    dm = dm or deps.model_cache()
    sm = sm or sigs.SigModel(dm.corpus, dm.infos)
    real = json.loads((GROUPS_DIR / gname / 'group.json').read_text())
    text = '\n'.join((GROUPS_DIR / gname / f).read_text() for f in real['files'])
    rmem = [m for m in real['members'] if m in dm.corpus.funcs]
    rctx = [m for m in real.get('context', []) if m in dm.corpus.funcs]
    realset = set(rmem) | set(rctx)
    if seeds_mode == 'members':
        seeds = rmem
    elif seeds_mode == 'first':
        seeds = rmem[:1]
    else:
        seeds = [s for s in seeds_mode.split(',') if s]
    plan = make_plan(seeds, mode, dm, min_sites=plan_min_sites)
    gen_members = set(plan.nodes)
    real_keep = set(real['keep'])
    gen_keep = set(plan.keep_list())
    real_keep_fn = {k for k in real_keep if not k.startswith('__standin_')}
    gen_keep_fn = {k for k in gen_keep if not k.startswith('__standin_')}
    real_si = {k[len('__standin_'):] for k in real_keep if k.startswith('__standin_')}
    gen_si = set(plan.standins)
    # restrict the keep comparison to functions both sides model (a generated keep of a node the real group
    # does not contain is a membership difference, not a keep difference)
    both = gen_members & realset
    row = {
        'group': gname, 'mode': mode, 'seeds': seeds,
        'members_real': len(realset), 'members_gen': len(gen_members), 'members_common': len(both),
        'members_missing': sorted(realset - gen_members), 'members_extra': sorted(gen_members - realset),
        'keep_real': sorted(real_keep_fn), 'keep_gen': sorted(gen_keep_fn),
        'keep_agree_on_common': sorted(n for n in both if (n in real_keep_fn) == (n in gen_keep_fn)),
        'keep_disagree_on_common': sorted(n for n in both if (n in real_keep_fn) != (n in gen_keep_fn)),
        'standin_real': sorted(real_si), 'standin_gen': sorted(gen_si),
        'standin_agree_on_common': sorted(n for n in both if (n in real_si) == (n in gen_si)),
        'standin_disagree_on_common': sorted(n for n in both if (n in real_si) != (n in gen_si)),
    }
    # prototypes
    rp = parse_protos(text, sorted(both))
    protos = []
    for n in sorted(both):
        if n not in rp:
            continue
        g = sm.sig(n)
        gp = [p.cls for p in g.params]
        t = rp[n]
        rel = lambda cl: ['w' if x == 'ptr' else x for x in cl]          # noqa: E731
        protos.append({'name': n, 'real': '%s(%s)' % (t['ret'], ','.join(t['params'])), 'gen': '%s(%s)' % (
            sigs._ret_class(g.ret_ctype()), ','.join(gp)),
            'arity': None if t['kr'] else len(gp) == len(t['params']),
            'order_relaxed': None if t['kr'] or len(gp) != len(t['params']) else rel(gp) == rel(t['params']),
            'ret': sigs._ret_class(g.ret_ctype()) == t['ret']})
    row['protos'] = protos
    return row


def compare_summary(rows):
    lines = []
    for r in rows:
        lines.append('== %s  (seeds %s, mode %s)' % (r['group'], ','.join(r['seeds'])[:70], r['mode']))
        lines.append('   membership: real %d, generated %d, common %d; missing %s; extra %s' % (
            r['members_real'], r['members_gen'], r['members_common'], r['members_missing'] or '-', r['members_extra'] or '-'))
        lines.append('   keep(functions): real %s' % (r['keep_real'] or '-'))
        lines.append('                    gen  %s' % (r['keep_gen'] or '-'))
        lines.append('   keep agreement on common members: %d/%d  disagree: %s' % (
            len(r['keep_agree_on_common']), len(r['keep_agree_on_common']) + len(r['keep_disagree_on_common']),
            r['keep_disagree_on_common'] or '-'))
        lines.append('   stand-ins: real %s gen %s  agreement %d/%d' % (
            r['standin_real'] or '-', r['standin_gen'] or '-', len(r['standin_agree_on_common']),
            len(r['standin_agree_on_common']) + len(r['standin_disagree_on_common'])))
        pa = [p for p in r['protos'] if p['arity'] is not None]
        lines.append('   prototypes: arity %d/%d, relaxed class order %d/%d, return class %d/%d' % (
            sum(1 for p in pa if p['arity']), len(pa), sum(1 for p in pa if p['order_relaxed']),
            len([p for p in pa if p['order_relaxed'] is not None]) or len(pa), sum(1 for p in r['protos'] if p['ret']), len(r['protos'])))
        for p in r['protos']:
            if p['arity'] is False or p['order_relaxed'] is False or not p['ret']:
                lines.append('     differs: %s real %s gen %s' % (p['name'], p['real'], p['gen']))
    return '\n'.join(lines)


# ---------------------------------------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------------------------------------

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('names', nargs='*', help='seed functions (or, with --compare, group names)')
    ap.add_argument('--mode', choices=['direct', 'chain', 'full'], default='direct')
    ap.add_argument('--name', help='group directory name (default: first seed)')
    ap.add_argument('--out', help='output directory (default cloud/work/ipa-groups/<name>)')
    ap.add_argument('--m2c', choices=['auto', 'on', 'off'], default='auto')
    ap.add_argument('--plan', action='store_true', help='only print the plan')
    ap.add_argument('--no-compile', action='store_true')
    ap.add_argument('--no-fallback', action='store_true', help='keep m2c bodies that fail to compile (default: TODO stubs)')
    ap.add_argument('--force', action='store_true')
    ap.add_argument('--dead-switch', action='store_true', help='add a dead `if (0) { switch }` to stand-in\'d callees')
    ap.add_argument('--min-sites', type=int, default=MIN_SITES)
    ap.add_argument('--standin-sites', type=int, default=STANDIN_SITES)
    ap.add_argument('--compare', action='store_true', help='regenerate the plan for real groups and diff')
    ap.add_argument('--seeds-mode', default='members', help='with --compare: members | first | comma list')
    ap.add_argument('--json', action='store_true')
    a = ap.parse_args(argv)
    STANDIN_SITES_CFG[0] = a.standin_sites
    dm = deps.model_cache()
    sm = sigs.SigModel(dm.corpus, dm.infos)
    if a.compare:
        rows = [compare_group(g, a.seeds_mode, a.mode, dm, sm, a.min_sites) for g in a.names]
        print(json.dumps(rows, indent=1) if a.json else compare_summary(rows))
        return 0
    if not a.names:
        ap.error('seed function names required')
    plan = make_plan(a.names, a.mode, dm, min_sites=a.min_sites)
    name = a.name or plan.seeds[0]
    out_dir = Path(a.out) if a.out else GROUPS_DIR / name
    if a.plan:
        if a.json:
            print(json.dumps(plan.to_json(), indent=1))
        else:
            print('seeds %s mode %s: %d functions' % (plan.seeds, plan.mode, len(plan.nodes)))
            for n, v in plan.nodes.items():
                print('  %-32s %-10s %-8s %s' % (n, v.cls, v.decision, v.reason))
            print('keep:', plan.keep_list())
            for nt in plan.notes:
                print('note:', nt)
        return 0
    info, res = emit_and_compile(plan, out_dir, dm, sm, m2c=a.m2c, dead_switch=a.dead_switch, force=a.force,
                                 log=lambda m: sys.stderr.write(m + '\n'), compile_=not a.no_compile,
                                 fallback=not a.no_fallback)
    if a.json:
        print(json.dumps({'plan': plan.to_json(), 'out': str(out_dir), 'info': info, 'compile': res}, indent=1))
        return 0
    if info.get('stubbed'):
        print('stubbed after cfe errors (m2c body rejected): %s' % ', '.join(info['stubbed']))
    print('wrote %s (%d members, keep %d, stand-ins %d, externs %d, m2c bodies %d/%d, reordered calls %d, todo calls %d)' % (
        out_dir, len(plan.nodes), len(plan.keep), len(plan.standins), info['externs'], sum(info['m2c'].values()),
        len(info['m2c']), info['reordered_calls'], info['todo_calls']))
    for n, v in plan.nodes.items():
        print('  %-32s %-10s %-8s %s' % (n, v.cls, v.decision, v.reason))
    for nt in plan.notes:
        print('note:', nt)
    if res is not None:
        if 'skipped' in res:
            print('compile: skipped (%s)' % res['skipped'])
        elif res['err']:
            print('compile: FAILED: %s' % res['err'].strip().splitlines()[-1][:200])
            print(res['err'][:1500])
        else:
            print('compile: ok in %.1fs, matched=%s' % (res['secs'], res['matched']))
            for n, m in res['members'].items():
                print('  %-32s strict_diff %s / target %s words (emitted %s)%s' % (
                    n, m['strict_diff'], m['target_size'], m['size'], '  UNRESOLVED %d' % m['unresolved'] if m['unresolved'] else ''))
    return 0


if __name__ == '__main__':
    sys.exit(main())
