/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: allocation-only residual. */
unsigned char func_8001989C(unsigned int *state) { if (*state != 0xFFFFFFFF) return (*state & 0x80000000) == 0; return 1; }
