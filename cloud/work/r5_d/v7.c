/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
extern s16 D_80151AD0;
extern s8 D_80143F54[];
extern s32 D_8014A110;
extern s16 D_8014A108;
extern u8 D_8014A250[];
extern u8 D_80152038[];
extern u8 D_80152818[];
extern s8 D_8012E77C[];
extern s8 D_8015256C[];
extern s32 D_80150B60;
extern s8 D_80150B68[];
#define IDX(a) (*(s16 *)(D_8014A250 + (a) * 2056 + 1990))
#define K4(i) (*(s32 *)(D_80152038 + (i) * 120 + 20))
#define K6(i) (*(s8 *)(D_80152818 + (i) * 952 + 931))
#define K6B(i) D_8015256C[D_8012E77C[i]]
#define K0(i) (*(s8 *)(D_80152818 + (i) * 952 + 238))
void func_800F7F3C(void)
{
  s32 i;
  s32 j;
  s8 a;
  s8 b;
  s16 ia;
  s16 ib;
  for (i = 0; i < D_80151AD0; i++) {
    D_80143F54[i] = i;
  }
  if (D_8014A110 == 4) {
    for (i = 0; i < D_8014A108 - 1; i++) {
      for (j = 0; j < D_8014A108 - 1; j++) {
        a = D_80143F54[j];
        b = D_80143F54[j + 1];
        ia = IDX(a);
        ib = IDX(b);
        if (K4(ia) < K4(ib)) {
          D_80143F54[j + 1] = a;
          D_80143F54[j] = b;
        }
      }
    }
    for (i = 1; i < D_8014A108; i++) {
      if (K4(IDX(D_80143F54[0])) == K4(IDX(D_80143F54[i]))) {
        D_80150B68[i] = 1;
        D_80150B60++;
      }
    }
  } else if (D_8014A110 == 6) {
    for (i = 0; i < D_8014A108 - 1; i++) {
      for (j = 0; j < D_8014A108 - 1; j++) {
        a = D_80143F54[j];
        b = D_80143F54[j + 1];
        ia = IDX(a);
        ib = IDX(b);
        if (K6(ia) < K6(ib)) {
          D_80143F54[j + 1] = a;
          D_80143F54[j] = b;
        }
      }
    }
    i = 0;
    for (i = 0; i < D_8014A108 - 1; i++) {
      for (j = 0; j < D_8014A108 - 1; j++) {
        a = D_80143F54[j];
        b = D_80143F54[j + 1];
        if (K6B(IDX(a)) < K6B(IDX(b))) {
          D_80143F54[j + 1] = a;
          D_80143F54[j] = b;
        }
      }
    }
    for (i = 1; i < D_8014A108; i++) {
      if (K6(IDX(D_80143F54[0])) == K6(IDX(D_80143F54[i]))) {
        D_80150B68[i] = 1;
        D_80150B60++;
      }
    }
  } else {
    for (i = 0; i < D_8014A108 - 1; i++) {
      for (j = 0; j < D_8014A108 - 1; j++) {
        a = D_80143F54[j];
        b = D_80143F54[j + 1];
        ia = IDX(a);
        ib = IDX(b);
        if (K0(ia) < K0(ib)) {
          D_80143F54[j + 1] = a;
          D_80143F54[j] = b;
        }
      }
    }
    for (i = 1; i < D_8014A108; i++) {
      if (K0(IDX(D_80143F54[0])) == K0(IDX(D_80143F54[i]))) {
        D_80150B68[i] = 1;
        D_80150B60++;
      }
    }
  }
}
