typedef unsigned char u8;
extern u8 D_80140BDC;
extern u8 arr[];
extern void f(int,int);
void a(void) { f(D_80140BDC-1, 0); f(D_80140BDC-1, 1); }
void b(void) { f(arr[0]-1, 0); f(arr[0]-1, 1); }
void c(int x) { f(D_80140BDC-1, x); }
void d(u8 *p) { f(*p-1, 0); }
