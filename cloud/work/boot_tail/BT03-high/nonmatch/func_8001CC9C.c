/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: see README.md and verification.json. */
unsigned char func_8001CC9C(unsigned char value)
{ if (value > 127) return 127; return value; }
