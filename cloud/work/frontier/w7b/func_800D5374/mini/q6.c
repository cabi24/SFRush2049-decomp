typedef int s32; typedef short s16; typedef unsigned int u32;
typedef struct E { s32 a[17]; s32 o; s32 b; } E;
extern s16 N; extern E A[];
s32 g(s32);
void func_800D5374(void) { s32 i; i = 0; while (i < N) { if (A[i].o != -1) g(A[i].o); A[i++].o = -1; } }
