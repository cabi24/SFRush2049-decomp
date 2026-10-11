p="uopt.trace.c"; s=open(p).read()
st=s.index("static void f_compute_save(uint8_t *mem, uint32_t sp, uint32_t a0) {")
en=s.index("static void f_whyuncolored(", st)
body=s[st:en]
old="L46f190:\nt0 = MEM_U32(s1 + 28);\n"
assert old in body
body=body.replace(old, "L46f190:\nsave_log(mem, s1, s0, f20.f[0]);\nt0 = MEM_U32(s1 + 28);\n",1)
assert "L46f1c0:\n" in body
body=body.replace("L46f1c0:\n", "L46f1c0:\nsave_log2(mem, s1, v0, f22.f[0]);\n",1)
helper = r'''
static int cup_web2=-3;
static void save_log(uint8_t *mem, uint32_t lr, uint32_t lu, float c) {
    if (cup_web2==-3) { const char *e=getenv("CUPWEB"); cup_web2 = e ? atoi(e) : -1; }
    if (cup_web2<0) return;
    if (!dkwb_cdx_active(dkwb_cdx_globalcolor_ordinal)) return;
    if ((int)MEM_U32(lr+4)!=cup_web2) return;
    uint32_t bb = MEM_U32(lu+0);
    fprintf(stderr, "[SAVE] web=%d bb=%d uses=%d defs=%d w=%d f21=%d f22=%d f23=%d contrib=%.3f\n", (int)MEM_U32(lr+4), (int)MEM_U16(bb+8), (int)MEM_U16(lu+16), (int)MEM_U8(lu+18), (int)MEM_U32(bb+44), (int)MEM_U8(lu+21), (int)MEM_U8(lu+22), (int)MEM_U8(lu+23), (double)c);
}
static void save_log2(uint8_t *mem, uint32_t lr, uint32_t card, float tot) {
    if (cup_web2<0) return;
    if (!dkwb_cdx_active(dkwb_cdx_globalcolor_ordinal)) return;
    if ((int)MEM_U32(lr+4)!=cup_web2) return;
    fprintf(stderr, "[SAVE] web=%d units=%d bvectcard=%d total=%.3f flag34=%d\n", (int)MEM_U32(lr+4), (int)MEM_U32(lr+28), (int)card, (double)tot, (int)MEM_U8(lr+34));
}
'''
s=s[:st]+helper+body+s[en:]
open(p,"w").write(s)
print("ok")
