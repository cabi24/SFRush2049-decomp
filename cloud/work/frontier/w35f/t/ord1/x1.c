extern int g1, g2, g3;
extern void ext(int); extern int ext2(int,int);
void f_c(int a) { int i; for (i = 0; i < a; i++) { g1 += i * 3; ext(i); g2 = g1 - a; ext(g2); if (g3 > i) g3 = ext2(i, g1); g3 += 3; } }
void f_a(int a) { int i; for (i = 0; i < a; i++) { g1 += i * 5; ext(i); g2 = g1 - a; ext(g2); if (g3 > i) g3 = ext2(i, g1); g3 += 5; } }
void f_b(int a) { int i; for (i = 0; i < a; i++) { g1 += i * 7; ext(i); g2 = g1 - a; ext(g2); if (g3 > i) g3 = ext2(i, g1); g3 += 7; } }
void f_d(int a) { int i; for (i = 0; i < a; i++) { g3 += i * 9; ext(g3); g1 = a; ext(a); f_c(a); ext(g1); g2 = ext2(g1,g2);} }
