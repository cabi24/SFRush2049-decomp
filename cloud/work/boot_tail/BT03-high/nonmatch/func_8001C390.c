/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: pointer loop allocation/schedule residual. */
extern unsigned char D_8004FA18; extern unsigned char D_8004FA50[][24]; void func_8001C390(void) { unsigned char (*item)[24]; int count; item = D_8004FA50; count = D_8004FA18; if (count > 0) { do { (*item)[0] = 0; item++; } while (item < D_8004FA50 + count); } }
