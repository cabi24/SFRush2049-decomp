typedef int s32; typedef short s16; typedef unsigned int u32;
typedef struct E { s32 a[17]; s32 o; s32 b; } E;
extern s16 N; extern E A[];
s32 g(s32);
void func_800D5374(void) { E *e; if (N > 0) for (e = A; e < &A[N]; e++) { if (e->o != -1) g(e->o); e->o = -1; } }
