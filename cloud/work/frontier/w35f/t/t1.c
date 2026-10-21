typedef struct S { int x; unsigned mb; } S;
typedef struct T { int x; int mb; } T;
extern void f(short);
void g1(S *p) { f(p->mb); }
void g2(T *p) { f(p->mb); }
void g3(S *p) { f((short)p->mb); }
void g4(S *p) { int i = p->mb; f(i); }
