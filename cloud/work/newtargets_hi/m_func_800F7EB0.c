typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct { s8 b[5]; } E5;
extern s16 D_8014A108;
extern float D_80144DA8[];
extern s8 D_80144018[];
extern float D_80124618;
/*@1: extern E5 D_80151AC0[3];\n#define AC(j,k) D_80151AC0[j].b[k]\n || extern s8 D_80151AC0[];\n#define AC(j,k) D_80151AC0[(j)*5+(k)]\n */
void func_800F7EB0(void) {
/*@2: s32 i; s32 j; s32 k; || s32 j; s32 k; s32 i; || s32 k; s32 i; s32 j; */
/*@3: for (i = 0; i < D_8014A108; i++) { D_80144018[i] = 0; D_80144DA8[i] = D_80124618; } || i = 0; if (D_8014A108 > 0) do { D_80144018[i] = 0; D_80144DA8[i] = D_80124618; i++; } while (i < D_8014A108); || for (i = 0; i < D_8014A108; ++i) { D_80144DA8[i] = D_80124618; D_80144018[i] = 0; } */
/*@4: for (j = 0; j < 3; j++) { for (k = 0; k < 5; k++) AC(j,k) = -1; } || for (j = 0; j < 3; j++) for (k = 0; k < 5; k++) D_80151AC0[j].b[k] = -1; */
}
