#!/usr/bin/env python3
"""analyze.py: per-function static analysis of retail words for IPA evidence.

    python3 cloud/work/tools/ipakit/analyze.py NAME... [--json] [--sites]
    python3 cloud/work/tools/ipakit/analyze.py --all [--json]

Per function (instruction-level CFG with MIPS delay slots, branch-likely and switch tables):
  entry_reads      registers read before any write on some path, O32 inputs (a0-a3, f12-f15, sp, ra, gp)
                   and callee-saved saves excluded; non-ABI ones (t*, v*, at, s*, f0-f11, f16+) are the
                   IPA-parameter evidence.  Calls count as reading the callee's own entry reads, so pass-through
                   parameters are found (iterated to a fixpoint over the call graph).
  callee-saved     saved (store to sp of a still-unmodified register), restored (matching load), used, and
                   `unsaved_written` = written with no save: IPA evidence (callers keep nothing there).
  frame            `addiu sp,sp,-N`.
  call sites       per jal/jalr/tail-j: target, resolution (func / alt = callee+4 copied-first-instruction /
                   interior / opaque / external static code), and the caller-save registers LIVE ACROSS the
                   call (backward liveness over the CFG: live after the call; a call kills only v0/v1/f0/f1
                   so a register kept across it stays live through it) plus return-register use.
Stdlib only; needs no IDO.
"""
import argparse
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from ipakit import load_corpus  # noqa: E402
from ipakit import mipsdec  # noqa: E402
from ipakit.mipsdec import REGBIT, REGNAMES, mask_names, names_mask  # noqa: E402

GPR_CALLER = ['at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3'] + ['t%d' % i for i in range(10)]
CALLER_SAVED = names_mask(GPR_CALLER + ['f%d' % i for i in range(20)])
CALLEE_SAVED = names_mask(['s%d' % i for i in range(8)] + ['s8'] + ['f%d' % i for i in range(20, 32)])
RA = names_mask(['ra'])
STRIP = names_mask(['zero', 'sp', 'gp', 'k0', 'k1', 'hi', 'lo', 'fcc'])
NOT_A_PARAM = names_mask(['at'])        # assembler temporary: never carries an IPA parameter
ABI_INPUT = names_mask(['a0', 'a1', 'a2', 'a3', 'sp', 'ra', 'zero', 'gp', 'f12', 'f13', 'f14', 'f15', 'hi', 'lo', 'fcc'])
CALL_ARGS = names_mask(['a0', 'a1', 'a2', 'a3', 'f12', 'f13', 'f14', 'f15'])
RET_REGS = names_mask(['v0', 'v1', 'f0', 'f1'])
SP = REGBIT['sp']


class Site:
    __slots__ = ('pc', 'kind', 'target', 'res', 'callee', 'alt', 'alt_verified', 'live_across', 'live_maybe', 'ret_use', 'passes')

    def to_json(self):
        return {'pc': '0x%08X' % self.pc, 'kind': self.kind, 'target': None if self.target is None else '0x%08X' % self.target,
                'resolution': self.res, 'callee': self.callee, 'alt_entry': self.alt, 'alt_verified': self.alt_verified,
                'live_across': self.live_across, 'live_maybe': self.live_maybe, 'ret_use': self.ret_use, 'passes': self.passes}


class FuncInfo:
    def __init__(self, f):
        self.name, self.addr, self.nwords = f.name, f.addr, len(f.words)
        self.discovered = f.discovered
        self.frame = 0
        self.entry_reads_mask = 0
        self.entry_live_mask = 0
        self.saved = self.restored = self.used_saved = self.unsaved_written = 0
        self.local_defs = 0                 # every register the body writes (restores excluded)
        self.sites = []
        self.indirect_jumps = []            # unresolved `jr rX` (pc)
        self.switch_tables = 0
        self.has_ret = False

    @property
    def unresolved(self):
        """Call/jump targets inside the game image that are neither a function head nor head+4."""
        return [s for s in self.sites if s.res in ('opaque', 'interior')]

    @property
    def alt_entries(self):
        return [s for s in self.sites if s.res == 'alt']

    @property
    def entry_reads(self):
        return mask_names(self.entry_reads_mask)

    def to_json(self):
        return {'name': self.name, 'addr': '0x%08X' % self.addr, 'words': self.nwords, 'frame': self.frame,
                'entry_reads': self.entry_reads, 'saved': mask_names(self.saved), 'restored': mask_names(self.restored),
                'used_callee_saved': mask_names(self.used_saved), 'unsaved_written': mask_names(self.unsaved_written),
                'local_defs': mask_names(self.local_defs & ~STRIP), 'indirect_jumps': ['0x%08X' % p for p in self.indirect_jumps],
                'switch_tables': self.switch_tables,
                'unresolved_targets': ['0x%08X' % s.target for s in self.unresolved],
                'alt_entries': [s.to_json() for s in self.alt_entries], 'sites': [s.to_json() for s in self.sites]}


def _is_sp_slot(d):
    return d.mem is not None and d.mem[0] == 29


def analyze_function(corpus, f, callee_reads=None):
    """FuncInfo for corpus function f.  callee_reads: {callee name: mask of registers live at its entry}."""
    callee_reads = callee_reads or {}
    info = FuncInfo(f)
    ins = mipsdec.decode_words(f.words, f.addr)
    n = len(ins)
    pc2i = {d.pc: i for i, d in enumerate(ins)}
    hi_addr = f.end

    # frame size first: a store of an argument register to sp+off with off >= frame is a home-slot spill of
    # the incoming value (the caller's frame), a weak read that does not make the register a real input.
    for d in ins[:12]:
        if d.mnem == 'addiu' and d.rs == 29 and d.rt == 29 and d.imm < 0:
            info.frame = -d.imm
            break
        if d.delay:
            break
    # -- call-site skeletons and CFG ------------------------------------------------------------
    def new_site(d, kind, target, slot):
        s = Site()
        s.pc, s.kind, s.target = d.pc, kind, target
        s.alt = s.alt_verified = False
        s.callee, s.res, s.live_across, s.live_maybe, s.ret_use, s.passes = None, 'indirect', [], [], [], []
        if target is not None:
            res, cf, alt = corpus.resolve(target)
            s.res, s.alt = res, alt
            if cf is not None:
                s.callee = cf.name
                if alt:
                    s.alt_verified = slot is not None and slot == cf.words[0]
        return s

    # Each control-transfer instruction i has its delay slot at i+1.  `slot_succ[i+1]` is where control
    # goes after the slot runs; `succ[i]` is where it goes from the branch itself (into the slot, or past
    # it when a branch-likely is not taken).  Every other instruction simply falls through.
    slot_succ = {}
    node_succ = [[i + 1] if i + 1 < n else [] for i in range(n)]
    extra = []                      # virtual call / tail-call nodes
    sites_at = {}

    def add_extra(i, st, after):
        extra.append({'slot': i + 1, 'site': st, 'succ': after})
        sites_at[i] = st

    for i, d in enumerate(ins):
        if not d.delay:
            continue
        si = i + 1
        has_slot = si < n
        fall = si + 1 if si + 1 < n else None
        slot_word = ins[si].word if has_slot else None
        if d.kind in ('branch', 'b', 'j'):
            tgt = d.target
            inside = f.addr <= tgt < hi_addr
            outs = []
            if inside:
                outs.append(pc2i[tgt])
            else:
                add_extra(i, new_site(d, 'tail', tgt, slot_word), [])
                outs.append('tail')
            if d.kind == 'branch' and not d.likely and fall is not None:
                outs.append(fall)
            slot_succ[si] = outs
            if d.kind == 'branch' and d.likely:
                node_succ[i] = ([si] if has_slot else []) + ([fall] if fall is not None else [])
        elif d.kind in ('jal', 'jalr'):
            st = new_site(d, d.kind, d.target, slot_word)
            add_extra(i, st, [fall] if fall is not None else [])
            slot_succ[si] = ['call']
        elif d.kind == 'ret':
            info.has_ret = True
            slot_succ[si] = []
        elif d.kind == 'jr':
            tb = mipsdec.jump_table(ins, i, corpus.word_at, f.addr, hi_addr)
            if tb:
                info.switch_tables += 1
                slot_succ[si] = [pc2i[t] for t in sorted(set(tb)) if t in pc2i]
            else:
                info.indirect_jumps.append(d.pc)
                slot_succ[si] = []
    for si, outs in slot_succ.items():
        node_succ[si] = [o for o in outs if not isinstance(o, str)]
    use = [d.uses & ~STRIP for d in ins]
    dfn = [d.defs & ~STRIP for d in ins]
    for k, e in enumerate(extra):
        vid = n + k
        node_succ.append(e['succ'])
        e['vid'] = vid
        if e['slot'] < n:
            node_succ[e['slot']] = node_succ[e['slot']] + [vid]
        use.append(0)
        dfn.append(RET_REGS if e['site'].kind != 'tail' else 0)
    total = n + len(extra)
    # -- arguments each call consumes.  Known callee: the registers its own entry code reads (ABI args and
    # non-ABI IPA registers alike).  Unknown callee (static library code, indirect): O32 heuristic from the
    # argument registers freshly written since function entry / the previous call (a preserved lower
    # argument register below a fresh higher one is passed through).
    fresh = [None] * total          # argument registers written on EVERY path since entry / the last call (must)
    fresh[0] = 0
    work = [0]
    while work:
        x = work.pop()
        cf = fresh[x]
        of = cf | (dfn[x] & CALL_ARGS) if x < n else cf & ~CALL_ARGS
        for t in node_succ[x]:
            nf = of if fresh[t] is None else (fresh[t] & of)
            if nf != fresh[t]:
                fresh[t] = nf
                work.append(t)
    # registers this function itself has written on every path to the call
    lmd = [None] * total
    lmd[0] = 0
    work = [0]
    while work:
        x = work.pop()
        out = lmd[x] | dfn[x]
        for t in node_succ[x]:
            new = out if lmd[t] is None else (lmd[t] & out)
            if new != lmd[t]:
                lmd[t] = new
                work.append(t)
    A_REGS = [REGBIT['a%d' % j] for j in range(4)]
    for e in extra:
        st = e['site']
        fr = (fresh[e['vid']] or 0) & CALL_ARGS
        ed = (lmd[e['vid']] or 0) & CALL_ARGS
        top = -1
        for j in range(4):
            if fr >> A_REGS[j] & 1:
                top = j
        # O32 positional rule: a fresh higher argument register implies the lower ones are passed too
        # (a preserved value below it), except where the slot holds a float argument in f12 / f14.
        heur = fr
        for j in range(top):
            if (j == 0 and fr >> REGBIT['f12'] & 1) or (j == 1 and fr >> REGBIT['f14'] & 1):
                continue
            heur |= 1 << A_REGS[j]
        if st.callee is not None and st.callee in callee_reads:
            cr = callee_reads[st.callee] & ~STRIP & ~RA
            # non-ABI registers the callee reads are read by construction; an ABI argument register counts
            # only if this function has written it on every path to the call, so a callee reading a
            # parameter nobody passed (or a path-correlated temporary) does not keep the register live up
            # to the entry.
            u = (cr & ~CALL_ARGS) | (cr & CALL_ARGS & (heur | ed))
        else:
            u = heur
        use[e['vid']] = u & ~STRIP
    # -- callee-saved save / restore detection (forward must-analysis of "unmodified") ---------------
    track = CALLEE_SAVED | RA
    is_restore_ld = [False] * n
    for i, d in enumerate(ins):
        if d.kind == 'load' and _is_sp_slot(d) and d.defs & track:
            is_restore_ld[i] = True
    unm = [None] * total
    unm[0] = track if total else 0
    work = [0]
    while work:
        x = work.pop()
        cur = unm[x]
        out = cur
        if x < n:
            out = cur & ~(0 if is_restore_ld[x] else dfn[x])
        else:
            out = cur & ~RA                      # a call overwrites ra
        for s in node_succ[x]:
            new = out if unm[s] is None else (unm[s] & out)
            if new != unm[s]:
                unm[s] = new
                work.append(s)
    saves = {}                      # (regmask) -> slot offset
    is_save = [False] * n
    for i, d in enumerate(ins):
        if d.kind == 'store' and _is_sp_slot(d) and unm[i] is not None:
            r = d.uses & track & ~g_sp_mask()
            if r and (r & unm[i]) == r and d.mnem in ('sw', 'swc1', 'sdc1', 'sd'):
                is_save[i] = True
                saves[r] = d.mem[1]
                info.saved |= r
    for i, d in enumerate(ins):
        if is_restore_ld[i]:
            for r, off in saves.items():
                if d.mem[1] == off and d.defs & r:
                    info.restored |= r
    # uses/defs adjusted: saves are not reads
    for i, d in enumerate(ins):
        if is_save[i]:
            use[i] &= ~info.saved
        elif d.kind == 'store' and _is_sp_slot(d) and d.mem[1] >= info.frame and d.mnem in ('sw', 'swc1', 'sdc1', 'sd'):
            use[i] &= ~CALL_ARGS                      # home-slot spill of an incoming argument
    # -- liveness (backward) ---------------------------------------------------------------------
    live_in = [0] * total
    live_out = [0] * total
    preds = [[] for _ in range(total)]
    for x in range(total):
        for s in node_succ[x]:
            preds[s].append(x)
    work = list(range(total))
    inq = [True] * total
    while work:
        x = work.pop()
        inq[x] = False
        lo = 0
        for s in node_succ[x]:
            lo |= live_in[s]
        live_out[x] = lo
        li = use[x] | (lo & ~dfn[x])
        if li != live_in[x]:
            live_in[x] = li
            for p in preds[x]:
                if not inq[p]:
                    inq[p] = True
                    work.append(p)
    # must-defined registers: written (or an O32 input) on EVERY path to the node.  A register live across a
    # call but not must-defined there is read on a path-correlated branch (defined only under the same
    # condition), which liveness alone cannot tell from a real preserved value; it is reported apart.
    mdef = [None] * total
    mdef[0] = ABI_INPUT | STRIP
    work = [0]
    while work:
        x = work.pop()
        out = mdef[x] | dfn[x]
        for t in node_succ[x]:
            new = out if mdef[t] is None else (mdef[t] & out)
            if new != mdef[t]:
                mdef[t] = new
                work.append(t)
    info._graph = (ins, node_succ, use, dfn, live_in, live_out, n)
    info.entry_live_mask = live_in[0] & ~STRIP if total else 0
    info.entry_reads_mask = live_in[0] & ~ABI_INPUT & ~NOT_A_PARAM if total else 0
    # -- per-site results ----------------------------------------------------------------------------
    for k, e in enumerate(extra):
        s = e['site']
        if s.kind != 'tail':
            lo = live_out[e['vid']]
            cand = lo & CALLER_SAVED & ~RET_REGS
            md = mdef[e['vid']] or 0
            s.live_across = mask_names(cand & md)
            s.live_maybe = mask_names(cand & ~md)
            s.ret_use = mask_names(lo & RET_REGS)
        if s.callee and s.callee in callee_reads:
            s.passes = mask_names(callee_reads[s.callee] & ~ABI_INPUT)
    info.sites = [sites_at[i] for i in sorted(sites_at)]
    # -- local defs and frame -------------------------------------------------------------------------
    ld = 0
    for i, d in enumerate(ins):
        if not is_restore_ld[i]:
            ld |= d.defs
    info.local_defs = ld & ~STRIP
    used = 0
    for i, d in enumerate(ins):
        if is_save[i] or is_restore_ld[i]:
            continue
        used |= (d.uses | d.defs) & CALLEE_SAVED
    info.used_saved = used
    info.unsaved_written = (ld & CALLEE_SAVED) & ~info.saved
    return info


def g_sp_mask():
    return 1 << SP


def analyze_all(corpus=None, max_rounds=8):
    """{name: FuncInfo} for every corpus function, with entry reads propagated through calls."""
    corpus = corpus or load_corpus()
    callee_reads = {}
    infos = {}
    for rnd in range(max_rounds):
        changed = False
        for name, f in corpus.funcs.items():
            info = analyze_function(corpus, f, callee_reads)
            infos[name] = info
            if info.entry_live_mask != callee_reads.get(name, 0):
                callee_reads[name] = info.entry_live_mask
                changed = True
        if not changed:
            break
    return infos


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('names', nargs='*')
    ap.add_argument('--all', action='store_true')
    ap.add_argument('--json', action='store_true')
    ap.add_argument('--sites', action='store_true', help='print call sites')
    a = ap.parse_args(argv)
    corpus = load_corpus()
    infos = analyze_all(corpus)
    names = sorted(infos) if a.all else a.names
    if a.json:
        print(json.dumps([infos[n].to_json() for n in names], indent=1))
        return 0
    for n in names:
        i = infos.get(n)
        if i is None:
            print('%s: unknown function' % n)
            continue
        nonabi = [r for r in i.entry_reads]
        print('%-28s 0x%08X %4dw frame %-4d entry_reads=%s unsaved_s=%s saved=%s sites=%d' % (
            i.name, i.addr, i.nwords, i.frame, nonabi, mask_names(i.unsaved_written & CALLEE_SAVED),
            mask_names(i.saved), len(i.sites)))
        for s in i.unresolved:
            print('    UNRESOLVED target 0x%08X at 0x%08X (%s)' % (s.target, s.pc, s.res))
        for s in i.alt_entries:
            print('    alternate entry %s+4 at 0x%08X (first word copied into the delay slot: %s)' % (s.callee, s.pc, s.alt_verified))
        if a.sites:
            for s in i.sites:
                print('    %08x %-4s -> %s%s %-9s live_across=%s ret_use=%s' % (
                    s.pc, s.kind, s.callee or ('0x%08X' % s.target if s.target else 'indirect'), '+4' if s.alt else '',
                    s.res, s.live_across, s.ret_use))
    if a.all:
        ipa = sum(1 for i in infos.values() if i.entry_reads_mask or i.unsaved_written)
        print('%d functions, %d with entry-read/unsaved-s evidence' % (len(infos), ipa))
        import collections
        res = collections.Counter(s.res for i in infos.values() for s in i.sites)
        print('call sites by resolution: %s; unresolved targets: %d; alternate entries: %d; unresolved jr: %d' % (
            dict(res), len(res) and (res.get('opaque', 0) + res.get('interior', 0)), res.get('alt', 0),
            sum(len(i.indirect_jumps) for i in infos.values())))
    return 0


if __name__ == '__main__':
    sys.exit(main())
