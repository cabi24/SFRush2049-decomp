typedef unsigned char u8;
extern volatile u8 V;
u8 G = 5;
static u8 S;
extern void f(int,int);
void a(void) { f(V-1, 0); f(V-1, 1); }
void b(void) { f(G-1, 0); f(G-1, 1); }
void c(void) { f(S-1, 0); f(S-1, 1); }
