extern int g1, g2, g3;
extern void ext(int); extern int ext2(int,int);
extern void f_a(int), f_b(int), f_c(int), f_d(int);
void f_z(int a) { int i; for (i = 0; i < a; i++) { g1 += i * 11; ext(i); g2 = g1 - a; ext(g2); if (g3 > i) g3 = ext2(i, g1); g3 += 11; } }
void root(int a) { f_b(a); f_z(a); f_d(a); f_a(a); f_b(a+1); f_a(a+2); f_d(a+3); f_z(a+5); }
