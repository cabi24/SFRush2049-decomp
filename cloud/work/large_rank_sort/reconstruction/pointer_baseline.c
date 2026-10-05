/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct { u8 pad0[0x7C6]; s16 vehicle_index; u8 pad1[0x808 - 0x7C8]; } PlayerRecord;
typedef struct { u8 pad0[20]; s32 score; u8 pad1[0x78 - 24]; } RankingEntry;
typedef struct { u8 pad0[238]; s8 rank; u8 pad1[931 - 239]; s8 mode6_score; u8 pad2[952 - 932]; } CarRanking;
extern s16 D_80151AD0;
extern s8 D_80143F54[];
extern s32 D_8014A110;
extern s16 D_8014A108;
extern PlayerRecord D_8014A250[];
extern RankingEntry D_80152038[];
extern CarRanking D_80152818[];
extern s8 D_8012E77C[];
extern s8 D_8015256C[];
extern s32 D_80150B60;
extern s8 D_80150B68[];

void func_800F7F3C(void)
{
  s32 i;
  s8 *p;
  s32 n;
  for (i = 0; i < D_80151AD0; i++) {
    D_80143F54[i] = i;
  }
  if (D_8014A110 == 4) {
    n = D_8014A108;
    for (i = 0; i < n - 1; i++) {
      for (p = D_80143F54; p < D_80143F54 + n - 1; p++) {
        if (D_80152038[D_8014A250[p[0]].vehicle_index].score < D_80152038[D_8014A250[p[1]].vehicle_index].score) {
          s8 a = p[0];
          s8 b = p[1];
          p[1] = a;
          p[0] = b;
        }
      }
    }
    for (i = 1; i < n; i++) {
      if (D_80152038[D_8014A250[D_80143F54[0]].vehicle_index].score == D_80152038[D_8014A250[D_80143F54[i]].vehicle_index].score) {
        D_80150B68[i] = 1;
        D_80150B60++;
      }
    }
  } else if (D_8014A110 == 6) {
    n = D_8014A108;
    for (i = 0; i < n - 1; i++) {
      for (p = D_80143F54; p < D_80143F54 + n - 1; p++) {
        if (D_80152818[D_8014A250[p[0]].vehicle_index].mode6_score < D_80152818[D_8014A250[p[1]].vehicle_index].mode6_score) {
          s8 a = p[0];
          s8 b = p[1];
          p[1] = a;
          p[0] = b;
        }
      }
    }
    for (i = 0; i < n - 1; i++) {
      for (p = D_80143F54; p < D_80143F54 + n - 1; p++) {
        if (D_8015256C[D_8012E77C[D_8014A250[p[0]].vehicle_index]] < D_8015256C[D_8012E77C[D_8014A250[p[1]].vehicle_index]]) {
          s8 a = p[0];
          s8 b = p[1];
          p[1] = a;
          p[0] = b;
        }
      }
    }
    for (i = 1; i < n; i++) {
      if (D_80152818[D_8014A250[D_80143F54[0]].vehicle_index].mode6_score == D_80152818[D_8014A250[D_80143F54[i]].vehicle_index].mode6_score) {
        D_80150B68[i] = 1;
        D_80150B60++;
      }
    }
  } else {
    n = D_8014A108;
    for (i = 0; i < n - 1; i++) {
      for (p = D_80143F54; p < D_80143F54 + n - 1; p++) {
        if (D_80152818[D_8014A250[p[1]].vehicle_index].rank < D_80152818[D_8014A250[p[0]].vehicle_index].rank) {
          s8 a = p[0];
          s8 b = p[1];
          p[1] = a;
          p[0] = b;
        }
      }
    }
    for (i = 1; i < n; i++) {
      if (D_80152818[D_8014A250[D_80143F54[0]].vehicle_index].rank == D_80152818[D_8014A250[D_80143F54[i]].vehicle_index].rank) {
        D_80150B68[i] = 1;
        D_80150B60++;
      }
    }
  }
}
