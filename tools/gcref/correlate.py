#!/usr/bin/env python3
"""Candidate table: N64 game/overlay function -> GameCube rush_d.elf function(s).

Anchors (each weighted, each reported in the evidence string):
  str    shared string literal, direct or through a pointer table (one level); weight falls with
         the number of functions that use the string on either side
  call   callee/caller anchors: an N64 callee already paired with a GC callee (seeds: libc/libm and
         inflate, same-named game functions, then every confident pair found -- iterated)
  imm    shared 32-bit immediate (lis/ori, li >= 256, compare immediates) rare on both sides
  flt    shared float literal (bit pattern after rounding to float32) rare on both sides
  jt     switch table of the same size (>= 5 entries)
  file   N64 neighbours (+-4 functions in the image) already paired into the candidate's GC file
  fld    struct-offset fingerprint: rare (offset, width) field accesses off a pointer register shared
         by both functions (tools/gcref/fields.py; IDF-weighted cosine, capped)
  size   size similarity (weak tie-breaker, only added when there is other evidence)

Outputs docs/external/gc/correlation.md and correlation.json. Precision is measured on held-out
pairs (see --eval and README.md). Standard library only; needs reference/gc/features.json from
``python3 -m tools.gcref.disasm`` (built on the fly if missing, which needs capstone).

Usage: python3 -m tools.gcref.correlate [--n64-root DIR] [--out docs/external/gc] [--eval]
"""
import argparse
import glob
import json
import math
import os
import re
import struct
import subprocess
import sys
from collections import OrderedDict, defaultdict

if __package__ in (None, ''):
    sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..')))
from tools.gcref.elf import REPO  # noqa: E402

GAME_BASE = 0x80086A50
SEED_NAMES = ['sprintf', 'strlen', 'strchr', 'memcpy', 'memset', 'memchr', 'sinf', 'cosf', 'modf',
              'huft_build', 'inflate_block', 'inflate_dynamic', 'inflate_fixed', 'inflate_stored',
              'AdjustSteer', 'no_catchup']
# The coordinator's list also named CheckCPs/DoDrones/InitDrones/MaxPathControls/find_best_gear: in
# symbols.json those sit at placeholder addresses (0x800C0000, 0x800A0000 ...) that are not function
# starts in asm/us/blob, so they cannot seed; inflate_codes/inflate_entry do not exist on GC.
# N64 AdjustSpeed (0x800A2680) was renamed pak_delete_file_retry by lane w29n (docs/external/gc/names.md).
GAME_SEEDS = ['AdjustSteer', 'no_catchup', 'pak_delete_file_retry']
# Struct-offset channel (w29n): weight x similarity (an IDF cosine, <= 1), for the FLD_TOP most similar GC
# functions with similarity >= FLD_MIN. It re-ranks candidates only: confident pairs (and the call-graph
# propagation built on them) are decided without it unless --fld-confident (see README "Correlation").
FLD_WEIGHT = 1.0
FLD_MIN = 0.5
FLD_TOP = 3


def sext16(v):
    return v - 0x10000 if v & 0x8000 else v


def f32key(kind, hexbytes):
    """Normalise a float literal to a float32 bit pattern (doubles only if exactly representable)."""
    b = bytes.fromhex(hexbytes)
    if kind == 'f32':
        return b.hex()
    v = struct.unpack('>d', b)[0]
    try:
        f = struct.pack('>f', v)
    except OverflowError:
        return None
    return f.hex() if struct.unpack('>f', f)[0] == v else None


TRIVIAL_FLOATS = set(struct.pack('>f', x).hex() for x in (0.0, 1.0, -1.0, 0.5, 2.0, -0.5, 4.0, 10.0, 100.0,
                                                           255.0, 256.0, 1000.0, 0.25, 3.0, 60.0, 30.0))


def find_n64_root(hint=None):
    cands = [hint] if hint else []
    cands.append(REPO)
    try:
        common = subprocess.check_output(['git', '-C', REPO, 'rev-parse', '--git-common-dir'],
                                         stderr=subprocess.DEVNULL).decode().strip()
        cands.append(os.path.dirname(os.path.abspath(os.path.join(REPO, common))))
    except Exception:
        pass
    for c in cands:
        if c and os.path.exists(os.path.join(c, 'build', 'game_code.bin')):
            return c
    raise SystemExit('build/game_code.bin not found (pass --n64-root; worktrees fall back to the main checkout)')


# ---------------------------------------------------------------------------------------------------
# N64 side
# ---------------------------------------------------------------------------------------------------
class N64Image(object):
    """One code image (game code or an overlay) with its functions and data symbols."""

    def __init__(self, tag, data, base, funcs, data_syms):
        self.tag, self.data, self.base = tag, data, base
        self.end = base + len(data)
        self.funcs = funcs            # list of (addr, size, name)
        self.text_end = max(a + s for a, s, n in funcs)
        self.sym_addrs = sorted(data_syms)
        self.data_syms = data_syms    # addr -> name

    def word(self, a):
        o = a - self.base
        return struct.unpack_from('>I', self.data, o)[0] if 0 <= o <= len(self.data) - 4 else None

    def string_at(self, a):
        o = a - self.base
        if a < self.text_end or o < 0 or o >= len(self.data) or a & 3:
            return None
        if o > 0 and self.data[o - 1] != 0:
            return None
        e = self.data.find(b'\0', o, o + 512)
        if e < 0 or e - o < 2:
            return None
        s = self.data[o:e]
        if not all(0x20 <= c < 0x7f or c in (9, 10, 13) for c in s):
            return None
        return s.decode('ascii')

    def obj_of(self, a):
        """(start, end) of the data object containing a, from symbol boundaries."""
        import bisect
        i = bisect.bisect_right(self.sym_addrs, a) - 1
        if i < 0:
            return None
        start = self.sym_addrs[i]
        end = self.sym_addrs[i + 1] if i + 1 < len(self.sym_addrs) else self.end
        return start, end


def load_n64(root, build_root=None):
    """Return (images, all_function_names_by_addr) for the game code and overlays A/B.

    root holds asm/us/{blob,ovl_a,ovl_b}; build_root (default root) holds build/game_code.bin and build/ovl."""
    build_root = build_root or root
    syms = json.load(open(os.path.join(root, 'asm', 'us', 'blob', 'symbols.json')))['symbols']
    by_addr = {}
    for n, a in syms.items():
        by_addr.setdefault(int(a, 16), n)
    # game functions: .section .text.NAME in asm/us/blob/*.s, sized by their .word count
    funcs = []
    for path in sorted(glob.glob(os.path.join(root, 'asm', 'us', 'blob', 'blob_*.s'))):
        cur, cnt = None, 0
        region = int(re.search(r'blob_([0-9a-f]+)\.s', path).group(1), 16)
        pc = region
        for line in open(path):
            if line.startswith('.section'):
                if cur is not None:
                    funcs.append((cur[0], cnt * 4, cur[1]))
                m = re.match(r'\.section \.text\.(\S+),', line)
                cur = (pc, m.group(1)) if m else None
                if cur and m.group(1) in syms:
                    cur = (int(syms[m.group(1)], 16), m.group(1))
                    pc = cur[0]
                cnt = 0
            elif line.strip().startswith('.word'):
                cnt += 1
                pc += 4
        if cur is not None:
            funcs.append((cur[0], cnt * 4, cur[1]))
    game = open(os.path.join(build_root, 'build', 'game_code.bin'), 'rb').read()
    dsyms = {int(a, 16): n for n, a in syms.items() if GAME_BASE <= int(a, 16) < GAME_BASE + len(game)}
    images = [N64Image('game', game, GAME_BASE, sorted(funcs), dsyms)]
    for t in ('a', 'b'):
        ext_p = os.path.join(root, 'asm', 'us', 'ovl_' + t, 'extents.json')
        img_p = os.path.join(build_root, 'build', 'ovl', t, 'image.bin')
        if not (os.path.exists(ext_p) and os.path.exists(img_p)):
            continue
        ext = json.load(open(ext_p))
        data = open(img_p, 'rb').read()
        base = int(ext['base'], 16)
        of = [(int(f['address'], 16), f['size'], 'ovl_%s:%s' % (t, f['name'])) for f in ext['functions']]
        osyms = {}
        sp = os.path.join(root, 'asm', 'us', 'ovl_' + t, 'symbols.json')
        if os.path.exists(sp):
            for n, a in json.load(open(sp))['symbols'].items():
                if base <= int(a, 16) < base + len(data):
                    osyms[int(a, 16)] = n
        images.append(N64Image('ovl_' + t, data, base, sorted(of), osyms))
    return images, by_addr


def n64_features(img, ext_names):
    """Per-function features of one image (MIPS words decoded directly)."""
    local = {a: n for a, s, n in img.funcs}
    # pointer tables: data words pointing at strings, grouped by containing data object
    table_strings = defaultdict(set)
    for a in range(img.text_end, img.end - 3, 4):
        w = img.word(a)
        if w and img.base <= w < img.end:
            s = img.string_at(w)
            if s is not None:
                ob = img.obj_of(a)
                if ob:
                    table_strings[ob[0]].add(s)
    feats = OrderedDict()
    for addr, size, name in img.funcs:
        hi = {}
        callees, fn_refs, strings, via, floats, imms, tables, jts = set(), set(), set(), set(), set(), set(), set(), []
        last_sltiu = None
        for pc in range(addr, addr + size, 4):
            w = img.word(pc)
            if w is None:
                break
            op, rs, rt, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 0xffff
            ref = None
            if op in (2, 3):
                t = (pc & 0xF0000000) | ((w & 0x3FFFFFF) << 2)
                if op == 3 or not (addr <= t < addr + size):
                    n = local.get(t) if img.base <= t < img.end else None
                    n = n or ext_names.get(t)
                    if n:
                        callees.add(n)
                continue
            if op == 15:
                hi[rt] = imm << 16
                continue
            if op == 0:
                funct, rd = w & 63, (w >> 11) & 31
                if funct == 0x21 and (rs in hi or rt in hi):          # addu propagates %hi
                    hi[rd] = hi[rs] if rs in hi else hi[rt]
                    continue
                if funct == 8 and rs != 31 and last_sltiu is not None:  # jr via jump table
                    jts.append(last_sltiu)
                    last_sltiu = None
                hi.pop(rd, None)
                continue
            if op == 17 and rs == 4 and rt in hi:                    # mtc1 of a lui'd float
                fl = struct.pack('>I', hi[rt]).hex()
                floats.add(fl)
                continue
            if op == 9 or op == 13:                                  # addiu / ori
                if rs == 0:
                    v = sext16(imm) if op == 9 else imm
                    if abs(v) >= 256:
                        imms.add(v & 0xffffffff)
                elif rs in hi:
                    v = (hi[rs] + sext16(imm)) & 0xffffffff if op == 9 else hi[rs] | imm
                    if 0x80000000 <= v < 0x80800000:
                        ref = v
                    else:
                        imms.add(v)
                hi.pop(rt, None)
            elif op in (10, 11):
                v = sext16(imm)
                if op == 11:
                    last_sltiu = v
                if abs(v) >= 256:
                    imms.add(v & 0xffffffff)
                hi.pop(rt, None)
            elif 32 <= op <= 63 and rs in hi:
                ref = (hi[rs] + sext16(imm)) & 0xffffffff
                if op in (49, 53) and img.base <= ref < img.end:     # lwc1/ldc1 literal
                    b = img.data[ref - img.base: ref - img.base + (4 if op == 49 else 8)]
                    k = f32key('f32' if op == 49 else 'f64', b.hex())
                    if k:
                        floats.add(k)
                    ref = None
                if op < 40 or op in (49, 53):
                    hi.pop(rt, None)
            elif op not in (4, 5, 6, 7, 1, 20, 21, 22, 23, 17):
                hi.pop(rt, None)
            if ref is not None:
                if ref in local and ref != addr:
                    fn_refs.add(local[ref])
                elif img.base <= ref < img.end:
                    s = img.string_at(ref)
                    if s is not None:
                        strings.add(s)
                    ob = img.obj_of(ref)
                    if ob and ob[0] in table_strings:
                        via |= table_strings[ob[0]]
                        tables.add(ob[0])
                elif ref in ext_names:
                    fn_refs.add(ext_names[ref])
        feats[name] = {'name': name, 'image': img.tag, 'address': addr, 'size': size,
                       'callees': sorted(callees - {name}), 'fn_refs': sorted(fn_refs - {name}),
                       'strings': sorted(strings), 'strings_via_tables': sorted(via - strings),
                       'floats': sorted(floats - TRIVIAL_FLOATS), 'imms': sorted(imms),
                       'jump_tables': sorted(j for j in jts if j >= 1)}
    return feats


# ---------------------------------------------------------------------------------------------------
# Correlation
# ---------------------------------------------------------------------------------------------------
class Correlator(object):
    def __init__(self, n64, gc, n64_order, fields=None, fld_weight=0.0, fld_min=FLD_MIN, fld_top=FLD_TOP,
                 fld_confident=False):
        self.n64, self.gc = n64, gc
        self.fields, self.fld_weight, self.fld_min, self.fld_top = fields, fld_weight, fld_min, fld_top
        # fld_confident=False: the struct-offset channel re-ranks the candidates but confident pairs (and so
        # the call-graph propagation) are decided without it, exactly as before the channel existed
        self.fld_confident = fld_confident
        self._fld = {}
        self.order = n64_order                     # N64 names in address order (per image)
        self.pos = {n: i for i, n in enumerate(n64_order)}
        for f in gc.values():
            f['floats_k'] = sorted(set(k for k in (f32key(*x.split(':')) for x in f['floats']) if k) - TRIVIAL_FLOATS)
        # inverted indices
        self.g_str, self.n_str = defaultdict(dict), defaultdict(dict)
        for side, feats, idx in (('g', gc, self.g_str), ('n', n64, self.n_str)):
            for k, f in feats.items():
                for s in f['strings_via_tables']:
                    idx[s][k] = 'table'
                for s in f['strings']:
                    idx[s][k] = 'direct'
        self.g_imm, self.n_imm = self._inv(gc, 'imms'), self._inv(n64, 'imms')
        self.g_flt, self.n_flt = self._inv(gc, 'floats_k'), self._inv(n64, 'floats')
        self.g_jt, self.n_jt = self._inv(gc, 'jump_tables'), self._inv(n64, 'jump_tables')
        self.g_callers, self.n_callers = self._callers(gc), self._callers(n64)
        self.gc_by_file = defaultdict(list)
        for k, f in gc.items():
            self.gc_by_file[f['file']].append(k)

    @staticmethod
    def _inv(feats, key):
        idx = defaultdict(set)
        for k, f in feats.items():
            for v in set(f.get(key, ())):
                idx[v].add(k)
        return idx

    @staticmethod
    def _callers(feats):
        idx = defaultdict(set)
        for k, f in feats.items():
            for c in f['callees'] + f['fn_refs']:
                idx[c].add(k)
        return idx

    def score_one(self, n, pairs, gfile_of_pair, mask_strings=False, only_strings=False, use_fld=True):
        """dict gc -> (score, [evidence])."""
        f = self.n64[n]
        cand = defaultdict(lambda: [0.0, []])

        def add(g, w, ev):
            c = cand[g]
            c[0] += w
            c[1].append((w, ev))
        if not mask_strings:
            tbl = defaultdict(list)                 # g -> [(w, s)] through pointer tables, capped
            for s in set(f['strings']) | set(f['strings_via_tables']):
                gs = self.g_str.get(s)
                if not gs or len(s) < 2:
                    continue
                nn = len(self.n_str[s])
                if len(gs) > 12 or nn > 12:
                    continue
                kind_n = 'direct' if s in f['strings'] else 'table'
                inf = string_info(s)
                if len(gs) == 1 and nn == 1 and len(s) >= 3:
                    inf = max(inf, 0.7)             # unique on both sides: specific even if short
                for g, kind_g in gs.items():
                    w = 3.0 * inf / max(len(gs), nn)
                    if kind_n == 'table' or kind_g == 'table':
                        tbl[g].append((w * 0.5, s))
                    else:
                        add(g, w, 'str %s' % json.dumps(s)[:40])
            for g, lst in tbl.items():
                lst.sort(key=lambda x: (-x[0], x[1]))
                tot = min(3.0, sum(w for w, _ in lst))
                add(g, tot, 'str(tbl) x%d %s' % (len(lst), ', '.join(json.dumps(x)[:24] for _, x in lst[:3])))
        if only_strings:
            return cand
        for v in f['imms']:
            gs = self.g_imm.get(v)
            if gs and len(gs) <= 4 and len(self.n_imm[v]) <= 4:
                for g in gs:
                    add(g, 1.5 / max(len(gs), len(self.n_imm[v])), 'imm 0x%X' % v)
        for v in f['floats']:
            gs = self.g_flt.get(v)
            if gs and len(gs) <= 5 and len(self.n_flt[v]) <= 5:
                fv = struct.unpack('>f', bytes.fromhex(v))[0]
                for g in gs:
                    add(g, 1.0 / max(len(gs), len(self.n_flt[v])), 'flt %.6g' % fv)
        for v in set(f['jump_tables']):
            gs = self.g_jt.get(v)
            if v >= 5 and gs and len(gs) <= 6:
                for g in gs:
                    add(g, 0.8 / max(len(gs), len(self.n_jt[v])), 'jt %d' % v)
        if use_fld and self.fields is not None and self.fld_weight > 0:
            if n not in self._fld:              # only the best few: a struct family shares many keys
                sims = sorted(self.fields.similar(n).items(), key=lambda kv: (-kv[1][0], kv[0]))
                self._fld[n] = sims[:self.fld_top]
            for g, (sim, keys) in self._fld[n]:
                if sim >= self.fld_min and g in self.gc:
                    add(g, self.fld_weight * sim, 'fld x%d %s' % (len(keys), ','.join(k.split('/')[0] for k in keys[:3])))
        # call-graph anchors
        for cn in f['callees'] + f['fn_refs']:
            cg = pairs.get(cn)
            if cg is None:
                continue
            ncall = len(self.n_callers[cn])
            for g in self.g_callers.get(cg, ()):
                w = 2.5 / math.sqrt(max(1, ncall) * max(1, len(self.g_callers[cg])))
                add(g, w, 'call %s' % cg.split('@')[0])
        for pn in self.n_callers.get(n, ()):
            pg = pairs.get(pn)
            if pg is None or pg not in self.gc:
                continue
            gcs = self.gc[pg]['callees'] + self.gc[pg]['fn_refs']
            nn = len(self.n64[pn]['callees']) + len(self.n64[pn]['fn_refs'])
            for g in gcs:
                w = 2.5 / math.sqrt(max(1, nn) * max(1, len(gcs)))
                add(g, w, 'caller %s' % pg.split('@')[0])
        # weak terms, only for candidates with real evidence
        i = self.pos[n]
        neigh = [self.order[j] for j in range(max(0, i - 4), min(len(self.order), i + 5)) if j != i]
        nfiles = defaultdict(int)
        for m in neigh:
            if m in gfile_of_pair and not gfile_of_pair[m].startswith('_'):
                nfiles[gfile_of_pair[m]] += 1
        for fl in nfiles:
            for g in self.gc_by_file.get(fl, ()):
                cand[g]            # file-cluster candidates (weak: file + size only, never confident alone)
        for g, c in cand.items():
            gf = self.gc[g]
            files = nfiles.get(gf['file'], 0)
            if files:
                w = min(1.5, 0.4 * files)
                c[0] += w
                c[1].append((w, 'file %s x%d' % (gf['file'], files)))
            r = abs(math.log(max(f['size'], 4) / float(max(gf['size'], 4))))
            w = 0.6 * max(0.0, 1.0 - r / 1.2)
            if w > 0.05:
                c[0] += w
                c[1].append((w, 'size %d/%d' % (f['size'], gf['size'])))
        return cand

    def run(self, seeds, rounds=6, mask=(), threshold=3.0, ratio=1.5):
        pairs = dict(seeds)
        results = {}
        for r in range(rounds):
            gfile = {n: self.gc[g]['file'] for n, g in pairs.items() if g in self.gc}
            results = {}
            best_for_g = defaultdict(lambda: (0.0, None))
            confrank = {}
            split = self.fields is not None and self.fld_weight > 0 and not self.fld_confident
            for n in self.n64:
                cand = self.score_one(n, pairs, gfile, mask_strings=n in mask)
                ranked = sorted(cand.items(), key=lambda kv: -kv[1][0])[:5]
                results[n] = ranked
                if split:
                    cand = self.score_one(n, pairs, gfile, mask_strings=n in mask, use_fld=False)
                    ranked = sorted(cand.items(), key=lambda kv: -kv[1][0])[:5]
                confrank[n] = ranked
                if ranked:
                    g, (s, ev) = ranked[0]
                    if s > best_for_g[g][0]:
                        best_for_g[g] = (s, n)
            new = 0
            for n, ranked in confrank.items():
                if n in seeds or not ranked:
                    continue
                g, (s1, ev) = ranked[0]
                s2 = ranked[1][1][0] if len(ranked) > 1 else 0.0
                conf = s1 >= threshold and s1 >= ratio * s2 and best_for_g[g][1] == n
                if conf and pairs.get(n) != g:
                    pairs[n] = g
                    new += 1
                elif not conf and n in pairs and n not in seeds:
                    del pairs[n]
                    new += 1
            if new == 0:
                break
        return results, pairs


def string_info(s):
    """0..1 informativeness of a string: short or format-only strings carry less identity."""
    core = re.sub(r'%[-+ #0-9.]*[a-zA-Z]', '', s)
    letters = sum(1 for c in core if c.isalnum())
    if letters == 0:
        return 0.5 if len(s) >= 4 else 0.25
    return min(1.0, 0.2 + 0.2 * letters)


def evidence_str(ev, limit=6):
    agg = OrderedDict()
    for w, e in sorted(ev, key=lambda x: -x[0]):
        agg.setdefault(e, 0.0)
        agg[e] += w
    items = list(agg.items())
    out = '; '.join('%s (%.1f)' % (e, w) for e, w in items[:limit])
    if len(items) > limit:
        out += '; +%d more' % (len(items) - limit)
    return out


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--n64-root', help='checkout with build/game_code.bin and build/ovl (default: this repo, '
                                       'else the main checkout of this worktree)')
    ap.add_argument('--gc-features', default=os.path.join(REPO, 'reference', 'gc', 'features.json'))
    ap.add_argument('--out', default=os.path.join(REPO, 'docs', 'external', 'gc'))
    ap.add_argument('--no-eval', action='store_true')
    ap.add_argument('--elf', default=os.path.join(REPO, 'rush_d.elf'),
                    help='rush_d.elf for the struct-offset channel (skipped when absent)')
    ap.add_argument('--fld-weight', type=float, default=FLD_WEIGHT,
                    help='weight of the struct-offset channel (0 disables it)')
    ap.add_argument('--fld-min', type=float, default=FLD_MIN, help='minimum field similarity scored')
    ap.add_argument('--fld-top', type=int, default=FLD_TOP, help='most similar GC functions scored per N64 one')
    ap.add_argument('--fld-confident', action='store_true',
                    help='let the struct-offset channel count towards confident pairs (unvalidated; see README)')
    a = ap.parse_args(argv)
    root = find_n64_root(a.n64_root)
    if not os.path.exists(a.gc_features):
        from tools.gcref import disasm
        disasm.main(['--no-dump'])
    gc = json.load(open(a.gc_features))
    src = REPO if os.path.exists(os.path.join(REPO, 'asm', 'us', 'blob', 'symbols.json')) else root
    images, by_addr = load_n64(src, root)
    ext_names = {ad: n for ad, n in by_addr.items() if not n.startswith('D_')}
    n64, order = OrderedDict(), []
    for img in images:
        names = dict(ext_names)
        if img.tag != 'game':
            names.update({ad: n for ad, s, n in images[0].funcs})
        feats = n64_features(img, names)
        n64.update(feats)
        order.extend(sorted(feats, key=lambda k: feats[k]['address']))
    census = json.load(open(os.path.join(src, 'tools', 'recomp', 'census', 'census.json')))['functions']
    blob_lock = json.load(open(os.path.join(src, 'blob_matched.lock.json')))
    ovl_lock = json.load(open(os.path.join(src, 'ovl_matched.lock.json')))
    ovl_matched = set('ovl_%s:%s' % (t, k) for t in ('a', 'b') for k in ovl_lock.get(t, {}))
    fidx = None
    if a.fld_weight > 0 and os.path.exists(a.elf):
        from tools.gcref import fields as fieldsmod
        nf = OrderedDict()
        for img in images:
            nf.update(fieldsmod.n64_fields(img))
        fidx = fieldsmod.FieldIndex(nf, fieldsmod.gc_fields(elf_path=a.elf))
    corr = Correlator(n64, gc, order, fields=fidx, fld_weight=a.fld_weight, fld_min=a.fld_min,
                      fld_top=a.fld_top, fld_confident=a.fld_confident)
    seeds = {n: n for n in SEED_NAMES if n in gc and n not in GAME_SEEDS}   # N64 static callee == GC name

    report = OrderedDict()
    if not a.no_eval:
        report = evaluate(corr, seeds)
        for k, v in report.items():
            print('eval %s: %s' % (k, v))
    results, pairs = corr.run(seeds)

    rows = []
    for n in sorted(n64, key=lambda k: (n64[k]['image'] != 'game', n64[k]['image'], n64[k]['address'])):
        f = n64[n]
        if f['image'] == 'game':
            status = 'unmatched' if n in census else ('matched' if n in blob_lock else 'other')
        else:
            status = 'matched' if n in ovl_matched else 'unmatched'
        ranked = results.get(n, [])
        cands = []
        for g, (s, ev) in ranked[:3]:
            gf = gc[g]
            cands.append(OrderedDict([('gc', g), ('file', gf['file']), ('address', gf['address']),
                                      ('size', gf['size']), ('score', round(s, 2)), ('evidence', evidence_str(ev))]))
        rows.append(OrderedDict([('n64', n), ('image', f['image']), ('address', '0x%08X' % f['address']),
                                 ('size', f['size']), ('status', status), ('seed', n in seeds),
                                 ('confident', pairs.get(n) if n not in seeds else n), ('candidates', cands)]))
    os.makedirs(a.out, exist_ok=True)
    with open(os.path.join(a.out, 'correlation.json'), 'w') as fh:
        json.dump({'generated_by': 'python3 -m tools.gcref.correlate', 'evaluation': report,
                   'seeds': sorted(seeds), 'rows': rows}, fh, indent=0, separators=(',', ':'))
    write_md(os.path.join(a.out, 'correlation.md'), rows, report, seeds)
    conf = [r for r in rows if r['confident'] and not r['seed']]
    print('N64 functions %d (game %d, overlays %d); with any candidate %d; confident pairs %d '
          '(unmatched game %d, matched game %d, overlay %d)' % (
              len(rows), sum(1 for r in rows if r['image'] == 'game'), sum(1 for r in rows if r['image'] != 'game'),
              sum(1 for r in rows if r['candidates']), len(conf),
              sum(1 for r in conf if r['image'] == 'game' and r['status'] == 'unmatched'),
              sum(1 for r in conf if r['image'] == 'game' and r['status'] == 'matched'),
              sum(1 for r in conf if r['image'] != 'game')))
    return 0


def evaluate(corr, seeds):
    """Held-out precision.

    (1) label check: what the N64 functions carrying arcade names (AdjustSpeed ...) pair with.
    (2) string hold-out: pairs where a string is used by exactly one function on each side (the
        strongest anchor) form a truth set; every other one is held out with its string channel
        masked, the rest stay available as anchors, and the other channels must recover it.
    """
    rep = OrderedDict()
    res, pairs = corr.run(seeds)
    chk = []
    for n in GAME_SEEDS:
        top = res.get(n) or []
        chk.append('%s -> %s' % (n, top[0][0] if top else 'no candidate'))
    rep['n64_label_check'] = ('N64 labels from arcade names are speculative, not seeds (top-1 shown, fld re-ranking included): '
                              + '; '.join(chk) + ' (pak_delete_file_retry at 0x800A2680, labelled AdjustSpeed until'
                              ' w29n, calls osPfsDeleteFile/osPfsFreeBlocks: a controller-pak routine)')
    truth = {}
    for n in corr.n64:
        if n in seeds:
            continue
        cand = corr.score_one(n, {}, {}, only_strings=True)
        ranked = sorted(cand.items(), key=lambda kv: -kv[1][0])
        if ranked and ranked[0][1][0] >= 3.0 and (len(ranked) == 1 or ranked[0][1][0] >= 2 * ranked[1][1][0]):
            truth[n] = {ranked[0][0]}
    keys = sorted(truth)
    top1 = anyc = top3 = nconf = confok = 0
    for fold in (0, 1):                             # two-fold: each half held out once
        held = set(keys[fold::2])
        res, pairs = corr.run(seeds, mask=held)
        top1 += sum(1 for n in held if res.get(n) and res[n][0][0] in truth[n])
        anyc += sum(1 for n in held if res.get(n))
        top3 += sum(1 for n in held if any(g in truth[n] for g, _ in res.get(n, [])[:3]))
        confn = [n for n in held if n in pairs]
        nconf += len(confn)
        confok += sum(1 for n in confn if pairs[n] in truth[n])
    held = set(keys)
    rep['string_holdout_truth'] = ('%d string-anchored pairs (string score >= 3 and 2x runner-up), two-fold: each '
                                   'half held out with its string channel masked' % len(truth))
    rep['string_holdout_top1'] = '%d/%d top-1 from the other channels (%d had any candidate; top-3 %d)' % (
        top1, len(held), anyc, top3)
    rep['string_holdout_confident_precision'] = '%d/%d confident pairs correct (recall %d/%d)' % (
        confok, nconf, confok, len(held))
    # (3) precision of the string channel itself on the same truth set: the unmasked run must agree
    res2, pairs2 = corr.run(seeds)
    ok = sum(1 for n in held if pairs2.get(n) in truth[n])
    rep['unmasked_agreement'] = '%d/%d held-out truth pairs confident with all channels' % (ok, len(held))
    return rep


def write_md(path, rows, report, seeds):
    with open(path, 'w') as f:
        f.write('# N64 -> GameCube function correlation\n\n')
        f.write('Generated by `python3 -m tools.gcref.correlate` (see README.md here). Candidates, not facts: the GC\n'
                'build is a divergent later sibling. **conf** = confident pair (score >= 3, 1.5x the runner-up, '
                'mutual best). Evidence weights in parentheses: `str` shared string (`(tbl)` = through a pointer '
                'table), `call`/`caller` call-graph anchor through an already-paired function, `imm`/`flt` rare '
                'shared constant, `jt` switch table size, `fld x<n> <offsets>` struct-offset fingerprint (n rare '
                'field offsets shared; re-ranks only, never makes a pair confident), `file` neighbours paired into '
                'that GC file, `size` size similarity.\n\n')
        f.write('Held-out evaluation:\n\n')
        for k, v in report.items():
            f.write('- %s: %s\n' % (k, v))
        f.write('\nSeeds (%d): %s\n\n' % (len(seeds), ', '.join(sorted(seeds))))
        cur = None
        for r in rows:
            if r['image'] != cur:
                cur = r['image']
                f.write('\n## %s\n\n' % cur)
                f.write('| N64 function | addr | size | state | conf | GC #1 | GC #2 | GC #3 |\n|---|---|---|---|---|---|---|---|\n')
            cells = []
            for c in r['candidates']:
                cells.append('`%s` [%s] %.1f: %s' % (c['gc'], c['file'], c['score'], c['evidence'].replace('|', '\\|')))
            while len(cells) < 3:
                cells.append('')
            f.write('| %s | %s | %d | %s | %s | %s |\n' % (
                r['n64'], r['address'][2:], r['size'], r['status'], 'seed' if r['seed'] else ('yes' if r['confident'] else ''),
                ' | '.join(cells)))


if __name__ == '__main__':
    sys.exit(main())
