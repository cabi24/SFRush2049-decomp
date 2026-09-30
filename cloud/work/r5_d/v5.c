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
  s32 j;
  s16 n;
  s32 last;
  s8 *p;
  s8 *q;
  s16 f;
  s8 a;
  s8 b;
  for (i = 0; i < D_80151AD0; i++) {
    D_80143F54[i] = i;
  }
  if (D_8014A110 == 4) {
    n = D_8014A108;
    last = n - 1;
    for (i = 0; i < last; i++) {
      for (p = D_80143F54; p < D_80143F54 + last; p++) {
        a = p[0];
        b = p[1];
        if (D_80152038[D_8014A250[a].idx].key < D_80152038[D_8014A250[b].idx].key) {
          p[1] = a;
          p[0] = b;
        }
      }
    }
    i = 1;
    if (n > 1) {
    L1:
      if (D_80152038[D_8014A250[D_80143F54[0]].idx].key == D_80152038[D_8014A250[D_80143F54[i]].idx].key) {
        D_80150B68[i] = 1;
        D_80150B60++;
      }
      i++;
      if (i < n) goto L1;
    }
  } else if (D_8014A110 == 6) {
    n = D_8014A108;
    last = n - 1;
    for (i = 0; i < last; i++) {
      for (p = D_80143F54; p < D_80143F54 + last; p++) {
        a = p[0];
        b = p[1];
        if (D_80152818[D_8014A250[a].idx].b931 < D_80152818[D_8014A250[b].idx].b931) {
          p[1] = a;
          p[0] = b;
        }
      }
    }
    i = 0;
    for (i = 0; i < last; i++) {
      for (p = D_80143F54; p < D_80143F54 + last; p++) {
        a = p[0];
        b = p[1];
        if (D_8015256C[D_8012E77C[D_8014A250[a].idx]] < D_8015256C[D_8012E77C[D_8014A250[b].idx]]) {
          p[1] = a;
          p[0] = b;
        }
      }
    }
    i = 1;
    if (n > 1) {
    L2:
      if (D_80152818[D_8014A250[D_80143F54[0]].idx].b931 == D_80152818[D_8014A250[D_80143F54[i]].idx].b931) {
        D_80150B68[i] = 1;
        D_80150B60++;
      }
      i++;
      if (i < n) goto L2;
    }
  } else {
    n = D_8014A108;
    last = n - 1;
    for (i = 0; i < last; i++) {
      for (p = D_80143F54; p < D_80143F54 + last; p++) {
        a = p[0];
        b = p[1];
        if (D_80152818[D_8014A250[a].idx].b238 < D_80152818[D_8014A250[b].idx].b238) {
          p[1] = a;
          p[0] = b;
        }
      }
    }
    i = 1;
    if (n > 1) {
    L3:
      if (D_80152818[D_8014A250[D_80143F54[0]].idx].b238 == D_80152818[D_8014A250[D_80143F54[i]].idx].b238) {
        D_80150B68[i] = 1;
        D_80150B60++;
      }
      i++;
      if (i < n) goto L3;
    }
  }
}
