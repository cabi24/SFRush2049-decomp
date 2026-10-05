typedef int s32; typedef short s16;
typedef struct E { s32 a[17]; s32 o; s32 b; } E;
extern s16 N; extern E A[];
void g(s32);
void func_800D5374(void) { s32 i; for (i = 0; i < N; i++) { if (A[i].o != -1) g(A[i].o); } }
