typedef struct S { int x; unsigned mb; } S;
typedef struct T { int x; int mb; } T;
extern void f(short);
extern void fi(int);
void g5(S *p) { f((int)p->mb); }
void g6(S *p) { fi((short)p->mb); }
void g7(S *p) { f(p->mb + 0); }
void g9(S *p) { short q = p->mb; f(q); }
void g10(S *p) { int q = p->mb; f(q); }
