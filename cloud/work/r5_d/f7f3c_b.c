/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct { u8 pad0[0x7C6]; s16 idx; u8 pad1[0x808 - 0x7C8]; } Rec;
typedef struct { u8 pad0[20]; s32 key; u8 pad1[0x78 - 24]; } Ent;
typedef struct { u8 pad0[238]; s8 b238; u8 pad1[931 - 239]; s8 b931; u8 pad2[952 - 932]; } Car;
extern s16 D_80151AD0;
extern s8 D_80143F54[];
extern s32 D_8014A110;
extern s16 D_8014A108;
extern Rec D_8014A250[];
extern Ent D_80152038[];
extern Car D_80152818[];
extern s8 D_8012E77C[];
extern s8 D_8015256C[];
extern s32 D_80150B60;
extern s8 D_80150B68[];

void func_800F7F3C(void)
{
  s32 i;
  s16 j;
  s8 *p;
  s32 n;
  for (i = 0; i < D_80151AD0; i++) {
    D_80143F54[j] = i;
  }
  if (D_8014A110 == 4) {
    n = D_8014A108;
    for (i = 0; i < n - 1; i++) {
      for (p = D_80143F54; p < D_80143F54 + n - 1; p++) {
        if (D_80152038[D_8014A250[p[0]].idx].key < D_80152038[D_8014A250[p[1]].idx].key) {
          s8 a = p[0];
          s8 b = p[1];
          p[1] = a;
          p[0] = b;
        }
      }
    }
    for (j = 1; j < n; j++) {
      if (D_80152038[D_8014A250[D_80143F54[0]].idx].key == D_80152038[D_8014A250[D_80143F54[j]].idx].key) {
        D_80150B68[j] = 1;
        D_80150B60++;
      }
    }
  } else if (D_8014A110 == 6) {
    n = D_8014A108;
    for (i = 0; i < n - 1; i++) {
      for (p = D_80143F54; p < D_80143F54 + n - 1; p++) {
        if (D_80152818[D_8014A250[p[0]].idx].b931 < D_80152818[D_8014A250[p[1]].idx].b931) {
          s8 a = p[0];
          s8 b = p[1];
          p[1] = a;
          p[0] = b;
        }
      }
    }
    i = 0;
    for (i = 0; i < n - 1; i++) {
      for (p = D_80143F54; p < D_80143F54 + n - 1; p++) {
        if (D_8015256C[D_8012E77C[D_8014A250[p[0]].idx]] < D_8015256C[D_8012E77C[D_8014A250[p[1]].idx]]) {
          s8 a = p[0];
          s8 b = p[1];
          p[1] = a;
          p[0] = b;
        }
      }
    }
    for (j = 1; j < n; j++) {
      if (D_80152818[D_8014A250[D_80143F54[0]].idx].b931 == D_80152818[D_8014A250[D_80143F54[j]].idx].b931) {
        D_80150B68[j] = 1;
        D_80150B60++;
      }
    }
  } else {
    n = D_8014A108;
    for (i = 0; i < n - 1; i++) {
      for (p = D_80143F54; p < D_80143F54 + n - 1; p++) {
        if (D_80152818[D_8014A250[p[0]].idx].b238 < D_80152818[D_8014A250[p[1]].idx].b238) {
          s8 a = p[0];
          s8 b = p[1];
          p[1] = a;
          p[0] = b;
        }
      }
    }
    for (j = 1; j < n; j++) {
      if (D_80152818[D_8014A250[D_80143F54[0]].idx].b238 == D_80152818[D_8014A250[D_80143F54[j]].idx].b238) {
        D_80150B68[j] = 1;
        D_80150B60++;
      }
    }
  }
}
