#!/usr/bin/env python3
"""Give the recompiled libc_impl.c working ecvt/fcvt (the stock ones assert): uopt's listing
writer (write_real) needs them for timings and colouring costs. Only called while listing."""
import sys
p = sys.argv[1]
s = open(p).read()
old_e = 'uint32_t wrapper_ecvt(uint8_t* mem, double number, int ndigits, uint32_t decpt_addr, uint32_t sign_addr) {\n    assert(0);\n}'
old_f = 'uint32_t wrapper_fcvt(uint8_t* mem, double number, int ndigits, uint32_t decpt_addr, uint32_t sign_addr) {\n    assert(0);\n}'
assert s.count(old_e) == 1 and s.count(old_f) == 1
new = '''static uint32_t w5d_cvt_buf;
static uint32_t w5d_cvt(uint8_t* mem, const char* r, int decpt, int sign, uint32_t decpt_addr, uint32_t sign_addr) {
    size_t n = strlen(r), i;
    if (!w5d_cvt_buf) w5d_cvt_buf = wrapper_malloc(mem, 512);
    if (n > 510) n = 510;
    for (i = 0; i < n; i++) MEM_S8(w5d_cvt_buf + i) = r[i];
    MEM_S8(w5d_cvt_buf + n) = 0;
    MEM_S32(decpt_addr) = decpt;
    MEM_S32(sign_addr) = sign;
    return w5d_cvt_buf;
}
uint32_t wrapper_ecvt(uint8_t* mem, double number, int ndigits, uint32_t decpt_addr, uint32_t sign_addr) {
    int decpt, sign;
    char* r = ecvt(number, ndigits, &decpt, &sign);
    return w5d_cvt(mem, r, decpt, sign, decpt_addr, sign_addr);
}'''
new_f = '''uint32_t wrapper_fcvt(uint8_t* mem, double number, int ndigits, uint32_t decpt_addr, uint32_t sign_addr) {
    int decpt, sign;
    char* r = fcvt(number, ndigits, &decpt, &sign);
    return w5d_cvt(mem, r, decpt, sign, decpt_addr, sign_addr);
}'''
s = s.replace(old_e, new).replace(old_f, new_f)
open(p, 'w').write(s)
