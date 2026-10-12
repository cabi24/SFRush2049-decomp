#!/usr/bin/env python3
"""pre71.py IN.c OUT.c -- add a PRE / code-motion oracle to the traced IDO 7.1 uopt (w24c).

Applied on top of trace71's uopt71.trace.c (instrument71_uopt.py + spill71.py).  Port of the 5.3 w5d hooks
(../../tools/trace/patches/instrument_w5d.py) to the 7.1 addresses:
    5.3 node list head 0x1001c8f8 -> 7.1 0x1001c358     (printcm's first load)
    5.3 bit count      0x1001cb38 -> 7.1 0x1001c390     (newbit)
    5.3 bit table      0x1001cc30 -> 7.1 0x1001c3a0     (newbit; 8 bytes per bit: ichain, ref)
    node vectors: identical offsets in both printcm's (antlocs 104 avlocs 114 alters 10c absalters 11c antin 154
    antout 15c iv 134 cand 13c ppin 12c ppout 16c hoisted fc insert 164 delete 124 subdelete 144 subinsert 14c)
    procedure name: 7.1 0x1001eb00, length word 0x1001c330 (as trace71's w71_name)
    hook: oneproc right after codemotion/printcm (label L4576c8), and at globalcolor entry (L469280).
Environment (all off by default, so the binary is the traced one otherwise):
    P71_OUT=FILE   write [P71] records for procedure P71_FN (or all): bit table + every node's 15 vectors,
                   at "cm" (after code motion) and "gc" (globalcolor entry)
    P71_FN=name    select the procedure
    P71_CLR=SPEC   oracle (diagnostic only): after code motion clear bits from vectors of selected nodes.
                   SPEC = items "VEC:BIT[@NODE]" joined by ',' (VEC one of the names above, NODE omitted = all);
    P71_SET=SPEC   same syntax, sets the bits instead.
    P71_PRECLR / P71_PRESET: the same, applied right BEFORE codemotion (to the local vectors antlocs/avlocs/alters...);
    P71_PREDUMP=1 dumps the vectors there too (at=pre).
"""
import sys

MARK = 'W24C_UOPT71_PRE_V1'
HEADER = r'''
/* W24C_UOPT71_PRE_V1 -- rush2049 frontier w24c PRE oracle for IDO 7.1 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int p71_ready;
static FILE *p71_out;
static char p71_fn[256];
static uint32_t p71_optab;
static const struct { const char *n; int off; } p71_vecs[] = {
    {"antlocs", 0x104}, {"avlocs", 0x114}, {"alters", 0x10c}, {"absalters", 0x11c}, {"antin", 0x154},
    {"antout", 0x15c}, {"iv", 0x134}, {"cand", 0x13c}, {"ppin", 0x12c}, {"ppout", 0x16c}, {"hoisted", 0xfc},
    {"insert", 0x164}, {"delete", 0x124}, {"subdelete", 0x144}, {"subinsert", 0x14c}, {0, 0}};
static void p71_init(uint8_t *mem) {
    const char *v;
    uint32_t a;
    if (p71_ready) return;
    p71_ready = 1;
    v = getenv("P71_OUT"); if (v && *v) p71_out = fopen(v, "w");
    v = getenv("P71_FN"); if (v) strncpy(p71_fn, v, sizeof(p71_fn) - 1);
    /* opcode name table: 8-byte entries "uabs", "uadd" (7.1: 0x1000ff70), ... found by scanning the data image */
    for (a = 0x10000000; a < 0x10020000; a += 4) {
        if (MEM_S8(a) == 'u' && MEM_S8(a + 1) == 'a' && MEM_S8(a + 2) == 'b' && MEM_S8(a + 3) == 's' && MEM_S8(a + 4) == 0 &&
            MEM_S8(a + 8) == 'u' && MEM_S8(a + 9) == 'a' && MEM_S8(a + 10) == 'd' && MEM_S8(a + 11) == 'd') {
            p71_optab = a; break; }
    }
}
static int p71_name(uint8_t *mem, char *buf, int size) {
    int n = (int)MEM_U32(0x1001c330), i;
    if (n < 0 || n >= size) n = size - 1;
    for (i = 0; i < n; i++) buf[i] = (char)MEM_S8(0x1001eb00 + i);
    buf[n] = 0;
    while (n > 0 && buf[n - 1] == ' ') buf[--n] = 0;
    return n;
}
static int p71_selected(uint8_t *mem) {
    char name[1100];
    if (!p71_fn[0]) return 1;
    p71_name(mem, name, sizeof(name));
    return strcmp(name, p71_fn) == 0;
}
static int p71_ptr(uint32_t v) { return v >= 0x10000000U; }
static const char *p71_op(uint8_t *mem, int op) {
    static char b[16];
    int i;
    if (!p71_optab || op < 0 || op > 0xa0) { snprintf(b, sizeof(b), "Uop%d", op); return b; }
    for (i = 0; i < 7; i++) { b[i] = (char)MEM_S8(p71_optab + 8 * op + i); if (!b[i] || b[i] == ' ') break; }
    b[i < 7 ? i : 7] = 0;
    return b;
}
static const char p71_dt[] = "ACFGHIJKLMNPQRSWXZ";
static char p71_dtype(int d) { return d >= 0 && d < 18 ? p71_dt[d] : '?'; }
static const char p71_mt[] = "ZMPRSA";
static char p71_mtype(int d) { return d >= 0 && d < 6 ? p71_mt[d] : '?'; }
static void p71_render(uint8_t *mem, uint32_t ic, char *buf, int size, int depth) {
    int kind, n;
    char l[2048], r[2048];
    if (size < 32) { if (size > 0) buf[0] = 0; return; }
    if (!p71_ptr(ic)) { snprintf(buf, size, "nil"); return; }
    if (depth > 12) { snprintf(buf, size, "..."); return; }
    kind = MEM_U8(ic + 0);
    switch (kind) {
    /* 7.1 packs the address word (isvarihash): blockno = w >> 11, mtype = (w >> 8) & 7; var at +20, lda at +28 */
    case 1: snprintf(buf, size, "lda%c(%d%+d)", p71_mtype((MEM_U32(ic + 28) >> 8) & 7), (int)(MEM_U32(ic + 28) >> 11), (int)MEM_U32(ic + 24)); break;
    case 2: snprintf(buf, size, "%d", (int)MEM_U32(ic + 16)); break;
    case 3: snprintf(buf, size, "var%c(%d%+d)%s", p71_mtype((MEM_U32(ic + 20) >> 8) & 7), (int)(MEM_U32(ic + 20) >> 11),
                     (int)MEM_U32(ic + 16), MEM_U8(ic + 25) ? "r" : ""); break;
    case 4: {
        char on[16]; char off[24] = "";
        p71_render(mem, MEM_U32(ic + 20), l, sizeof(l), depth + 1);
        strncpy(on, p71_op(mem, MEM_U8(ic + 16)) + 1, sizeof(on) - 1); on[sizeof(on) - 1] = 0;
        if (!strcmp(on, "ilod") || !strcmp(on, "isld") || !strcmp(on, "ildv"))
            snprintf(off, sizeof(off), "@%d", (int)MEM_U32(ic + 28));
        else if (!strcmp(on, "istr") || !strcmp(on, "isst") || !strcmp(on, "istv"))
            snprintf(off, sizeof(off), "@%d", (int)(int16_t)MEM_U16(ic + 38));
        if (MEM_U32(ic + 24) && p71_ptr(MEM_U32(ic + 24))) {
            p71_render(mem, MEM_U32(ic + 24), r, sizeof(r), depth + 1);
            snprintf(buf, size, "%s.%c%s(%s,%s)", on, p71_dtype(MEM_U8(ic + 1)), off, l, r);
        } else snprintf(buf, size, "%s.%c%s(%s)", on, p71_dtype(MEM_U8(ic + 1)), off, l);
        break; }
    case 6: p71_render(mem, MEM_U32(ic + 28), l, sizeof(l), depth + 1);
        snprintf(buf, size, "svar%c(%d%+d)[%s]", p71_mtype((MEM_U32(ic + 20) >> 8) & 7), (int)(MEM_U32(ic + 20) >> 11), (int)MEM_U32(ic + 16), l); break;
    default: {
        int i;
        n = snprintf(buf, size, "k%d<", kind);
        for (i = 16; i < 40 && n < size - 12; i += 4) n += snprintf(buf + n, size - n, "%x,", (unsigned)MEM_U32(ic + i));
        if (n < size - 2) snprintf(buf + n, size - n, ">");
        break; }
    }
}
static int p71_getbit(uint8_t *mem, uint32_t bv, int b) {
    uint32_t nblk = MEM_U32(bv), base = MEM_U32(bv + 4), w;
    if (b < 0 || (uint32_t)(b >> 7) >= nblk || !base) return 0;
    w = base + (uint32_t)(b >> 7) * 16 + (uint32_t)((b & 127) >> 5) * 4;
    return (MEM_U32(w) >> (31 - (b & 31))) & 1;
}
static void p71_putbit(uint8_t *mem, uint32_t bv, int b, int on) {
    uint32_t nblk = MEM_U32(bv), base = MEM_U32(bv + 4), w;
    if (b < 0 || (uint32_t)(b >> 7) >= nblk || !base) return;
    w = base + (uint32_t)(b >> 7) * 16 + (uint32_t)((b & 127) >> 5) * 4;
    if (on) MEM_U32(w) = MEM_U32(w) | (1u << (31 - (b & 31)));
    else MEM_U32(w) = MEM_U32(w) & ~(1u << (31 - (b & 31)));
}
static void p71_dump(uint8_t *mem, const char *tag) {
    char name[1100]; char text[8192];
    uint32_t n, tab, b, node;
    int v;
    p71_init(mem);
    if (!p71_out || !p71_selected(mem)) return;
    p71_name(mem, name, sizeof(name));
    n = MEM_U32(0x1001c390); tab = MEM_U32(0x1001c3a0);
    fprintf(p71_out, "[P71] proc at=%s name=%s nbits=%u optab=%x\n", tag, name, (unsigned)n, (unsigned)p71_optab);
    if (p71_ptr(tab))
        for (b = 0; b < n; b++) {
            uint32_t ic = MEM_U32(tab + 8 * b);
            if (!p71_ptr(ic)) { fprintf(p71_out, "[P71] bit at=%s bit=%u ic=%x\n", tag, (unsigned)b, (unsigned)ic); continue; }
            p71_render(mem, ic, text, sizeof(text), 0);
            fprintf(p71_out, "[P71] bit at=%s bit=%u kind=%d dtype=%c aux=%x text=%s", tag, (unsigned)b,
                    (int)MEM_U8(ic), p71_dtype(MEM_U8(ic + 1)), (unsigned)MEM_U32(tab + 8 * b + 4), text);
            if (getenv("P71_RAW")) { int k; fprintf(p71_out, " raw="); for (k = 0; k < 48; k += 4) fprintf(p71_out, "%08x ", (unsigned)MEM_U32(ic + k)); }
            fprintf(p71_out, "\n");
        }
    for (node = MEM_U32(0x1001c358); p71_ptr(node); node = MEM_U32(node + 12)) {
        /* init_graphnode: +20 / +24 are edge lists of {node, next} cells (assumed pred / succ) */
        int k; uint32_t c;
        fprintf(p71_out, "[P71] node at=%s node=%d", tag, (int)MEM_U16(node + 8));
        for (k = 20; k <= 24; k += 4) {
            fprintf(p71_out, k == 20 ? " l20=" : " l24=");
            for (c = MEM_U32(node + k); p71_ptr(c); c = MEM_U32(c + 4))
                fprintf(p71_out, "%d,", p71_ptr(MEM_U32(c)) ? (int)MEM_U16(MEM_U32(c) + 8) : -1);
        }
        fprintf(p71_out, " s28=%x s32=%x op28=%s", (unsigned)MEM_U32(node + 28), (unsigned)MEM_U32(node + 32),
                p71_ptr(MEM_U32(node + 28)) ? p71_op(mem, MEM_U8(MEM_U32(node + 28))) : "-");
        fprintf(p71_out, " op32=%s\n", p71_ptr(MEM_U32(node + 32)) ? p71_op(mem, MEM_U8(MEM_U32(node + 32))) : "-");
        for (v = 0; p71_vecs[v].n; v++) {
            uint32_t bv = node + p71_vecs[v].off, nb = MEM_U32(bv) * 128;
            int first = 1;
            fprintf(p71_out, "[P71] vec at=%s node=%d %s=", tag, (int)MEM_U16(node + 8), p71_vecs[v].n);
            if (MEM_U32(bv + 4))
                for (b = 0; b < nb && b < n; b++)
                    if (p71_getbit(mem, bv, (int)b)) { fprintf(p71_out, first ? "%u" : ",%u", (unsigned)b); first = 0; }
            fprintf(p71_out, "\n");
        }
    }
    fflush(p71_out);
}
static void p71_apply(uint8_t *mem, const char *env, int on) {
    const char *spec = getenv(env), *c;
    uint32_t node;
    if (!spec || !*spec || !p71_selected(mem)) return;
    for (c = spec; *c; ) {
        char vec[32]; int i = 0, v, bit, nodeno = -1; char *end;
        while (*c && *c != ':' && i < 31) vec[i++] = *c++;
        vec[i] = 0;
        if (*c != ':') break;
        c++;
        bit = (int)strtol(c, &end, 10); c = end;
        if (*c == '@') { nodeno = (int)strtol(c + 1, &end, 10); c = end; }
        for (v = 0; p71_vecs[v].n; v++) if (!strcmp(p71_vecs[v].n, vec)) break;
        if (p71_vecs[v].n)
            for (node = MEM_U32(0x1001c358); p71_ptr(node); node = MEM_U32(node + 12))
                if (nodeno < 0 || nodeno == (int)MEM_U16(node + 8)) p71_putbit(mem, node + p71_vecs[v].off, bit, on);
        if (p71_out) fprintf(p71_out, "[P71] oracle %s %s:%d@%d %s\n", env, vec, bit, nodeno, p71_vecs[v].n ? "ok" : "UNKNOWN-VEC");
        if (*c == ',') c++;
    }
}
static void p71_precm(uint8_t *mem) {
    p71_init(mem);
    if (getenv("P71_PREDUMP")) p71_dump(mem, "pre");
    p71_apply(mem, "P71_PRECLR", 0);
    p71_apply(mem, "P71_PRESET", 1);
}
static void p71_cm(uint8_t *mem) {
    p71_init(mem);
    p71_dump(mem, "cm");
    p71_apply(mem, "P71_CLR", 0);
    p71_apply(mem, "P71_SET", 1);
    if (getenv("P71_CLR") || getenv("P71_SET")) p71_dump(mem, "cm2");
}
'''


def once(s, old, new, label):
    if s.count(old) != 1:
        raise SystemExit('anchor %s occurs %d times' % (label, s.count(old)))
    return s.replace(old, new, 1)


def patch(s):
    if MARK in s:
        raise SystemExit('already patched')
    s = once(s, '#include "header.h"\n', '#include "header.h"\n' + HEADER, 'header')
    s = once(s, 'L4576c8:\n//nop;\n//nop;\nv0 = f_getclock(mem, sp);',
             'L4576c8:\np71_cm(mem);\n//nop;\n//nop;\nv0 = f_getclock(mem, sp);', 'after code motion')
    s = once(s, 'L469280:\n//globalcolor:\n', 'L469280:\np71_dump(mem, "gc");\n//globalcolor:\n', 'globalcolor entry')
    s = once(s, 'f_findinduct(mem, sp);\ngoto L457670;\n//nop;\nL457670:\n// bdead 40000183 gp = MEM_U32(sp + 24);\nv0 = 0x1001c4e4;\nv0 = MEM_U8(v0 + 0);\nif (v0 != 0) {\n//nop;\ngoto L45800c;}\n//nop;\n//nop;\n//nop;\nf_codemotion(mem, sp);',
             'f_findinduct(mem, sp);\ngoto L457670;\n//nop;\nL457670:\n// bdead 40000183 gp = MEM_U32(sp + 24);\nv0 = 0x1001c4e4;\nv0 = MEM_U8(v0 + 0);\nif (v0 != 0) {\n//nop;\ngoto L45800c;}\n//nop;\n//nop;\n//nop;\np71_precm(mem);\nf_codemotion(mem, sp);', 'before code motion')
    return s


if __name__ == '__main__':
    open(sys.argv[2], 'w').write(patch(open(sys.argv[1]).read()))
    print('patched ->', sys.argv[2])
