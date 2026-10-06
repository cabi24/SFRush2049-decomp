#!/usr/bin/env python3
"""Instrument the recompiled IDO 5.3 uopt.c for pre-colouring traces (w5d profile).

    instrument_w5d.py uopt.c uopt.w5d.c

Applies, in order:
  1. the vendored workbench `globalcolor` profile (web colouring trace, CDX_* env), unchanged;
  2. the w5d hooks below (W5D_* env).  With W5D_LEVEL and W5D_OUT unset every hook is a no-op
     that returns the value it was given, so the output is byte-identical to the stock pass
     (fidelity is checked by run_fidelity.sh on the whole-program unit).

w5d hooks
  * uopt's own listing dumps.  IDO uopt has a debug level (global 0x1001eaf8, set by the
    `-zdbug:N` option, written to the `-l FILE` listing).  Every read of it is routed through
    w5d_dbg(): with W5D_LEVEL=N it reads N for the procedure named by W5D_PROC (all procs if
    unset) and 0 for every other procedure.  Levels (equality tests in uopt):
      1 printtab   expression hash table after local optimisation
      2 printitab + printlinfo (loop info)
      3 printitab + printcm: per basic block PRE vectors (antlocs avlocs alters absalters antin
        antout ppin ppout insert delete subinsert subdelete ...), then the register-candidate
        sets (@ iscolored 1/2, colorcand, ...) from register-allocation preparation
      4 printscm (store code motion)   5 printregs   6 colouring detail   7 interproc/save
      21 printhoist (findinduct)   25 printprecm (codemotion, before insert/delete)
  * W5D_OUT=FILE: our own trace lines, all prefixed "[W5D]":
      bit   -- every expression bit (ichain) with its full expression text rendered recursively
               (variables as mtype/block/offset, constants by value, addresses as lda(block+off)),
               after code motion (at=cm) and at globalcolor entry (at=gc, includes the bits that
               register-allocation preparation gave to constants and addresses).
      regcand -- every constant / address operand that register-allocation preparation tests for
               a register candidate (makelivranges: constinreg / ldainreg), with the parent op,
               the basic block, and the decision (1 = becomes a live-range candidate, 0 =
               rematerialised at the use).
      ordinal -- the procedure's globalcolor ordinal (use it as CDX_PROC for CDX_FORCE); with W5D_PROC
               set and CDX_PROC unset the workbench colouring trace is restricted to that procedure.
  * W5D_NOPRE=b1,b2 (oracle, diagnostic only, like CDX_FORCE): after code motion clear those expression
    bits from the selected procedure's node vectors (W5D_NOPRE_VEC letters: d delete [default],
    s subdelete, i insert, u subinsert, h hoistedexp).  "d" answers "what if PRE had not removed these
    occurrences?".  Clearing an insertion (i/h) is not supported (codemotion has already materialised it).
  * W5D_RAWOP=1: append raw ichain words +28..+40 to every operator (layout research).
"""
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[5]
sys.path.insert(0, str(ROOT / 'third_party' / 'n64-decomp-workbench' / 'src'))

MARK = 'W5D_UOPT_PRE_V1'

HEADER = r'''
/* W5D_UOPT_PRE_V1 -- rush2049 frontier w5d pre-colouring trace hooks */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int w5d_ready, w5d_level;
static int dkwb_cdx_proc;                  /* tentative: defined by the workbench header */
static int dkwb_cdx_globalcolor_ordinal;   /* tentative: defined by the workbench header */
static char w5d_proc[256];
static FILE *w5d_out;
static void w5d_init(void) {
    const char *v;
    if (w5d_ready) return;
    w5d_ready = 1;
    v = getenv("W5D_LEVEL");
    w5d_level = v ? atoi(v) : 0;
    v = getenv("W5D_PROC");
    if (v) { strncpy(w5d_proc, v, sizeof(w5d_proc) - 1); }
    v = getenv("W5D_OUT");
    if (v && *v) w5d_out = fopen(v, "w");
}
static int w5d_name(uint8_t *mem, char *buf, int size) {
    int n = (int)MEM_U32(0x1001c8d0), i;
    if (n < 0 || n >= size) n = size - 1;
    for (i = 0; i < n; i++) buf[i] = (char)MEM_S8(0x1001c4d0 + i);
    buf[n] = 0;
    while (n > 0 && buf[n - 1] == ' ') buf[--n] = 0;
    return n;
}
static int w5d_selected(uint8_t *mem) {
    char name[1100];
    if (!w5d_proc[0]) return 1;
    w5d_name(mem, name, sizeof(name));
    return strcmp(name, w5d_proc) == 0;
}
/* every read of uopt's debug level goes through here */
static uint32_t w5d_dbg(uint8_t *mem, uint32_t value) {
    w5d_init();
    if (w5d_level <= 0) return value;
    return w5d_selected(mem) ? (uint32_t)w5d_level : 0;
}
static int w5d_ptr(uint32_t v) { return v >= 0x10000000U; }
static const char *w5d_op(uint8_t *mem, int op) {
    static char b[16];
    int i;
    if (op < 0 || op > 0xa0) { snprintf(b, sizeof(b), "op%d", op); return b; }
    for (i = 0; i < 7; i++) { b[i] = (char)MEM_S8(0x10000240 + 8 * op + i); if (!b[i]) break; }
    b[7] = 0;
    return b;
}
static const char w5d_dt[] = "ACFGHIJKLMNPQRSWXZ";
static char w5d_dtype(int d) { return d >= 0 && d < 18 ? w5d_dt[d] : '?'; }
static const char w5d_mt[] = "ZMPRSA";
static char w5d_mtype(int d) { return d >= 0 && d < 6 ? w5d_mt[d] : '?'; }
static void w5d_render(uint8_t *mem, uint32_t ic, char *buf, int size, int depth) {
    int kind, n;
    char l[2048], r[2048];
    if (size < 32) { if (size > 0) buf[0] = 0; return; }
    if (!w5d_ptr(ic)) { snprintf(buf, size, "nil"); return; }
    if (depth > 12) { snprintf(buf, size, "..."); return; }
    kind = MEM_U8(ic + 0);
    switch (kind) {
    case 1: /* islda: +24 offset, +28 block (dense symbol number), +30 memory type */
        snprintf(buf, size, "lda%c(%d%+d)", w5d_mtype(MEM_U8(ic + 30)), (int)MEM_U16(ic + 28),
                 (int)MEM_U32(ic + 24));
        break;
    case 2: /* isconst */
        snprintf(buf, size, "%d", (int)MEM_U32(ic + 16));
        break;
    case 3: /* isvar */
        snprintf(buf, size, "var%c(%d%+d)%s", w5d_mtype(MEM_U8(ic + 22)), (int)MEM_U16(ic + 20),
                 (int)MEM_U32(ic + 16), MEM_U8(ic + 25) ? "r" : "");
        break;
    case 4: /* isop */
        w5d_render(mem, MEM_U32(ic + 20), l, sizeof(l), depth + 1);
        if (getenv("W5D_RAWOP")) {
            n = (int)strlen(l);
            snprintf(l + n, sizeof(l) - n, "|%x,%x,%x,%x", (unsigned)MEM_U32(ic + 28), (unsigned)MEM_U32(ic + 32),
                     (unsigned)MEM_U32(ic + 36), (unsigned)MEM_U32(ic + 40));
        }
        {
            /* memory ops carry an offset: ilod/ilod-like at +28, istr at +36 (low half) */
            char on[16];
            char off[24] = "";
            strncpy(on, w5d_op(mem, MEM_U8(ic + 16)) + 1, sizeof(on) - 1);
            on[sizeof(on) - 1] = 0;
            if (!strcmp(on, "ilod") || !strcmp(on, "isld") || !strcmp(on, "ildv"))
                snprintf(off, sizeof(off), "@%d", (int)MEM_U32(ic + 28));
            else if (!strcmp(on, "istr") || !strcmp(on, "isst") || !strcmp(on, "istv"))
                snprintf(off, sizeof(off), "@%d", (int)(int16_t)MEM_U16(ic + 38));
            if (MEM_U32(ic + 24)) {
                w5d_render(mem, MEM_U32(ic + 24), r, sizeof(r), depth + 1);
                snprintf(buf, size, "%s.%c%s(%s,%s)", on, w5d_dtype(MEM_U8(ic + 1)), off, l, r);
            } else {
                snprintf(buf, size, "%s.%c%s(%s)", on, w5d_dtype(MEM_U8(ic + 1)), off, l);
            }
        }
        break;
    case 6: /* issvar: variable based on another ichain */
        w5d_render(mem, MEM_U32(ic + 28), l, sizeof(l), depth + 1);
        snprintf(buf, size, "svar%c(%d%+d)[%s]", w5d_mtype(MEM_U8(ic + 22)), (int)MEM_U16(ic + 20),
                 (int)MEM_U32(ic + 16), l);
        break;
    default:
        n = snprintf(buf, size, "k%d<", kind);
        {
            int i;
            for (i = 16; i < 40 && n < size - 12; i += 4)
                n += snprintf(buf + n, size - n, "%x,", (unsigned)MEM_U32(ic + i));
        }
        if (n < size - 2) snprintf(buf + n, size - n, ">");
        break;
    }
}
/* W5D_NOPRE=b1,b2,...: oracle -- after code motion, clear these expression bits from every block's
 * delete vector (W5D_NOPRE_VEC selects others) (procedure W5D_PROC only), i.e. ask "what would the
 * code be if PRE had left this expression alone?".  Diagnostic only, like CDX_FORCE. */
static void w5d_clearbit(uint8_t *mem, uint32_t bv, int b) {
    uint32_t nblk = MEM_U32(bv), base = MEM_U32(bv + 4), w;
    if (b < 0 || (uint32_t)(b >> 7) >= nblk || !base) return;
    w = base + (uint32_t)(b >> 7) * 16 + (uint32_t)((b & 127) >> 5) * 4;
    MEM_U32(w) = MEM_U32(w) & ~(1u << (31 - (b & 31)));
}
static void w5d_nopre(uint8_t *mem) {
    const char *v = getenv("W5D_NOPRE");
    uint32_t node;
    if (!v || !*v || !w5d_proc[0] || !w5d_selected(mem)) return;
    for (node = MEM_U32(0x1001c8f8); node; node = MEM_U32(node + 12)) {
        const char *c = v;
        while (*c) {
            char *end;
            long b = strtol(c, &end, 10);
            if (end == c) break;
            /* node vectors (offsets from printcm): h 0xfc hoistedexp, i 0x164 insert, d 0x124 delete,
             * s 0x144 subdelete, u 0x14c subinsert.  W5D_NOPRE_VEC picks them; default "d". */
            const char *m = getenv("W5D_NOPRE_VEC");
            if (!m) m = "d";
            if (strchr(m, 'h')) w5d_clearbit(mem, node + 0xfc, (int)b);
            if (strchr(m, 'i')) w5d_clearbit(mem, node + 0x164, (int)b);
            if (strchr(m, 'd')) w5d_clearbit(mem, node + 0x124, (int)b);
            if (strchr(m, 's')) w5d_clearbit(mem, node + 0x144, (int)b);
            if (strchr(m, 'u')) w5d_clearbit(mem, node + 0x14c, (int)b);
            c = *end == ',' ? end + 1 : end;
        }
    }
    if (w5d_out) fprintf(w5d_out, "[W5D] nopre applied bits=%s\n", v);
}
static void w5d_bits(uint8_t *mem, const char *tag) {
    char name[1100];
    char text[8192];
    uint32_t n, tab, b;
    w5d_init();
    if (!w5d_out || !w5d_selected(mem)) return;
    w5d_name(mem, name, sizeof(name));
    n = MEM_U32(0x1001cb38);
    tab = MEM_U32(0x1001cc30);
    if (!w5d_ptr(tab)) return;
    for (b = 0; b < n; b++) {
        uint32_t ic = MEM_U32(tab + 8 * b);
        if (!w5d_ptr(ic)) continue;
        w5d_render(mem, ic, text, sizeof(text), 0);
        fprintf(w5d_out, "[W5D] bit at=%s proc=%s bit=%u kind=%d dtype=%c ref=%d|%d text=%s\n",
                tag, name, (unsigned)b, (int)MEM_U8(ic), w5d_dtype(MEM_U8(ic + 1)),
                (int)MEM_U16(ic + 4), (int)MEM_U16(ic + 6), text);
    }
    fflush(w5d_out);
}
/* globalcolor entry: name the procedure's colouring ordinal, and (W5D_PROC set, CDX_PROC unset)
 * restrict the workbench colouring trace to that procedure */
static void w5d_gc(uint8_t *mem) {
    char name[1100];
    int sel;
    w5d_init();
    if (!w5d_proc[0]) { w5d_bits(mem, "gc"); return; }
    sel = w5d_selected(mem);
    if (!getenv("CDX_PROC")) dkwb_cdx_proc = sel ? dkwb_cdx_globalcolor_ordinal : 0x7fffffff;
    if (sel && w5d_out) {
        w5d_name(mem, name, sizeof(name));
        fprintf(w5d_out, "[W5D] ordinal proc=%s cdx_proc=%d\n", name, dkwb_cdx_globalcolor_ordinal);
    }
    w5d_bits(mem, "gc");
}
/* makelivranges: a constant / address operand tested for a register live range */
static void w5d_cand(uint8_t *mem, const char *what, uint32_t ic, uint32_t frame, int parent,
                     uint32_t decision, uint32_t value) {
    char name[1100];
    char text[4096];
    uint32_t bb;
    w5d_init();
    if (!w5d_out || !w5d_selected(mem)) return;
    w5d_name(mem, name, sizeof(name));
    w5d_render(mem, ic, text, sizeof(text), 0);
    bb = frame ? MEM_U32(frame - 4) : 0;
    fprintf(w5d_out, "[W5D] regcand proc=%s what=%s bb=%d ctx=%d decision=%u text=%s\n",
            name, what, bb ? (int)MEM_U16(bb + 8) : -1, parent, (unsigned)(decision & 0xff), text);
}
'''


def replace_once(src, old, new, label):
    n = src.count(old)
    if n != 1:
        raise SystemExit('anchor %r occurs %d times' % (label, n))
    return src.replace(old, new, 1)


def instrument(src):
    from decomp_workbench.instrument_uopt import instrument_uopt_globalcolor
    if MARK in src:
        raise SystemExit('already instrumented')
    src = instrument_uopt_globalcolor(src, allow_unverified_source=True).source
    src = replace_once(src, '#include "header.h"\n', '#include "header.h"\n' + HEADER, 'header')

    # 1. route every read of the debug level through w5d_dbg
    lines = src.split('\n')
    sites = 0
    for i, line in enumerate(lines):
        s = line.strip()
        if s.endswith('= 0x1001eaf8;') and not s.startswith('at ='):
            reg = s.split('=')[0].strip()
            for j in range(i + 1, i + 6):
                t = lines[j].strip()
                pre = ' = MEM_U32(%s + 0);' % reg
                if t.endswith(pre):
                    dst = t.split('=')[0].strip()
                    lines[j] = '%s = w5d_dbg(mem, MEM_U32(%s + 0));' % (dst, reg)
                    sites += 1
                    break
                if t.startswith('MEM_U32(%s + 0) =' % reg):
                    break               # getoption: the option store itself
            else:
                raise SystemExit('debug-level read not found after line %d' % (i + 1))
    if sites != 21:
        raise SystemExit('expected 21 debug-level reads, found %d' % sites)
    src = '\n'.join(lines)

    # 2. bit dumps: after code motion (oneproc, right after the printcm site) and at globalcolor
    src = replace_once(src, 'L45d664:\n//nop;\n//nop;\n//nop;\nv0 = f_getclock(mem, sp);',
                       'L45d664:\nw5d_bits(mem, "cm");\nw5d_nopre(mem);\n//nop;\n//nop;\n//nop;\nv0 = f_getclock(mem, sp);',
                       'bits after code motion')
    src = replace_once(src, 'L47106c:\n//globalcolor:', 'L47106c:\nw5d_gc(mem);\n//globalcolor:',
                       'bits at globalcolor')

    # 3. makelivranges register-candidate decisions for constants (constinreg) and addresses (ldainreg)
    src = replace_once(src, 'L464ee8:\n',
                       'L464ee8:\nw5d_cand(mem, "const", MEM_U32(s0 + 20), MEM_U32(sp + 92), MEM_U8(sp + 103), v0, MEM_U32(s0 + 32));\n'
                       '', 'const candidate')
    src = replace_once(src, 'L464fc8:\n',
                       'L464fc8:\nw5d_cand(mem, "lda", MEM_U32(s0 + 20), MEM_U32(sp + 92), MEM_U8(sp + 103), v0, MEM_U32(s0 + 32));\n'
                       '', 'lda candidate')
    src = replace_once(src, 'L464da8:\n', 'L464da8:\nw5d_cand(mem, "cvtconst", MEM_U32(s0 + 40), MEM_U32(sp + 68), 0, v1, MEM_U32(s0 + 24));\n',
                       'converted-constant candidate')
    return src


if __name__ == '__main__':
    src = open(sys.argv[1]).read()
    open(sys.argv[2], 'w').write(instrument(src))
    print('instrumented ->', sys.argv[2])
