/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w36p umerge inlining probe (not a candidate): which kept callees does umerge inline at 3 call sites? */
typedef signed char s8;
typedef signed int s32;
extern s32 g0, g1, g2, g3, g4, g5;
extern s8 q0[], q1[];
void ext0(void); void ext1(void); s32 ext2(void *, void *, s32); void *ext3(void);

void pk_store2(void) { g0 = 0; g1 = 0; }
void pk_store3(void) { g0 = 0; g1 = 0; g2 = 0; }
void pk_store5(void) { g0 = 0; g1 = 0; g2 = 0; g3 = 0; g4 = 0; }
void pk_call3(void) { ext0(); ext1(); ext0(); }
void pk_args3(void) { ext2(q0, 0, 1); ext2(q0, 0, 0); ext2(q1, 0, 0); }
void pk_tiny_a(void) { ext2(q0, 0, 1); }
void pk_tiny_b(void) { ext2(q0, 0, 0); }
void pk_comp3(void) { pk_tiny_a(); pk_tiny_b(); pk_tiny_a(); }
void pk_comp5(void) { pk_tiny_a(); g0 = 1; pk_tiny_b(); g1 = 1; pk_tiny_a(); }
void pk_comp5x(void) { s8 *m; pk_tiny_a(); m = ext3(); m[2] = 7; pk_tiny_b(); ext2(q1, m, 0); }
void pk_mix(void) { s8 *m; pk_tiny_a(); m = ext3(); m[2] = 7; ext2(q0, 0, 0); ext2(q1, m, 0); }
void pk_zoid(void) { g0 = 0; g1 = 0; g2 = 0; g3 = 0; g4 = 0; g5 = -1; ext0(); g0 = 1; g1 = 0; g2 = 4; }

void probe_caller(void)
{
    pk_store2(); pk_store2(); pk_store2();
    pk_store3(); pk_store3(); pk_store3();
    pk_store5(); pk_store5(); pk_store5();
    pk_call3(); pk_call3(); pk_call3();
    pk_args3(); pk_args3(); pk_args3();
    pk_comp3(); pk_comp3(); pk_comp3();
    pk_comp5(); pk_comp5(); pk_comp5();
    pk_comp5x(); pk_comp5x(); pk_comp5x();
    pk_mix(); pk_mix(); pk_mix();
    pk_zoid(); pk_zoid(); pk_zoid();
}
