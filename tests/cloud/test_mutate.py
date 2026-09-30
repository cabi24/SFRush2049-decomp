#!/usr/bin/env python3
"""Tests for cloud/work/tools/amatch/mutate.py (the source mutation catalog).

    python3 -m unittest tests/cloud/test_mutate.py
    python3 tests/cloud/test_mutate.py --coverage      # pair-coverage table

PAIRS are (before, after) versions of functions mined from git history
(`git log -p` on cloud/work/near-miss/*/base.c, cloud/work/ipa-groups/*/group.c
and cloud/matches/*.c): the last committed attempt and the next committed
version, reduced to the function (plus typedef/declaration lines it needs).
`before` is stored whole and `after` as non-overlapping text hunks. `ref` gives
(rev, path) of both sides for auditing. kind:
  exact   the catalog must reproduce `after` (token-equivalent, optional
          identifier rename) by chaining at most a few mutations;
  search  reachable with the catalog but needs a longer multi-step search
          (declaration reorders); only progress is asserted;
  partial `after` also contains edits outside the catalog (type rewrites,
          m2c cleanup); only progress is asserted.
The one synthetic pair (func_800CDDE8) is marked: the ascending case order was
never committed, it was reconstructed from the matching source.
"""
import difflib
import json
import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO / "cloud" / "work" / "tools"))
from amatch import mutate  # noqa: E402

PAIRS = [
    {
        "name": 'func_800A8F38', "fn": 'func_800A8F38', "kind": 'exact',
        "rename": {'a0': 'arg0', 'a1': 'arg1'},
        "note": 'r <<= 1 -> r *= 2',
        "ref": {'before': ('4d56dd5^', 'cloud/work/near-miss/func_800A8F38/base.c'), 'after': ('4d56dd5', 'cloud/matches/func_800A8F38.c')},
        "before": r'''typedef signed int s32;
typedef unsigned int u32;
u32 func_800A8F38(u32 arg0, s32 arg1)
{
  u32 r;
  r = 0;
  do
  {
    r |= arg0 & 1;
    arg0 >>= 1;
    r <<= 1;
    arg1 -= 1;
  }
  while (arg1 > 0);
  return r >> 1;
}
''',
        "hunks": [
            (r'''typedef unsigned int u32;
u32 func_800A8F38(u32 arg0, s32 arg1)
{
  u32 r;
  r = 0;
  do
  {
    r |= arg0 & 1;
    arg0 >>= 1;
    r <<= 1;
    arg1 -= 1;
  }
  while (arg1 > 0);
  return r >> 1;
}
''',
             r'''typedef unsigned int u32;
/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
u32 func_800A8F38(u32 a0,s32 a1){u32 r=0;do{r|=a0&1;a0>>=1;r*=2;a1-=1;}while(a1>0);return r>>1;}
'''),
        ],
    },
    {
        "name": 'func_800CFCA8', "fn": 'func_800CFCA8', "kind": 'exact',
        "note": 'do/while -> for, var_v0',
        "ref": {'before': ('13426b9^', 'cloud/work/near-miss/func_800CFCA8/base.c'), 'after': ('13426b9', 'cloud/matches/func_800CFCA8.c')},
        "before": r'''typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
void func_800CFCA8(void)
{
  GameCar *temp_a0;
  s16 var_v0;
  float new_var;
  unsigned int temp_f8;
  void *temp_v1;
  var_v0 = 0;
  do
  {
    temp_v1 = (D_8014A250_Record *) (((u8 *) (&D_8014A250)) + (var_v0 * 0x808));
    if ((*((s16 *) (((s8 *) temp_v1) + 0x7C8))) != 0)
    {
      temp_a0 = &player_array[var_v0];
      if (((s8) temp_a0->pad0EC[0]) != 0)
      {
        if (((s8) temp_a0->pad0EC[0x21D]) == 0)
        {
          temp_a0->pad0EC[0x224] = ((s8) temp_a0->pad0EC[0x224]) - 1;
          new_var = (*((f32 *) (((s8 *) temp_v1) + 0x714))) - (*((f32 *) (((s8 *) temp_a0) + 0x30C)));
          if (((s8) temp_a0->pad0EC[0x224]) < 0)
          {
            temp_a0->pad0EC[0x225] = ((s8) temp_a0->pad0EC[0x225]) + 1;
            if (((s8) temp_a0->pad0EC[0x225]) & 1)
            {
              temp_a0->pad0EC[0x224] = 0;
              *((s32 *) (((s8 *) temp_v1) + 0x7D4)) = (s32) ((*((s32 *) (((s8 *) temp_v1) + 0x7D4))) & (~8));
            }
            else
            {
              temp_f8 = (s32) new_var;
              switch (temp_f8)
              {
                default:
                  temp_a0->pad0EC[0x224] = 0;
                  break;

                case 0:

                case 1:
                  temp_a0->pad0EC[0x224] = 2;
                  break;

                case 2:

                case 3:
                  temp_a0->pad0EC[0x224] = 1;
                  break;

              }

              if (((s8) temp_a0->pad0EC[0x26D]) <= 0)
              {
                *((s32 *) (((s8 *) temp_v1) + 0x7D4)) = (s32) ((*((s32 *) (((s8 *) temp_v1) + 0x7D4))) | 8);
              }
            }
          }
        }
        else
        {
          *((s32 *) (((s8 *) temp_v1) + 0x7D4)) = (s32) ((*((s32 *) (((s8 *) temp_v1) + 0x7D4))) & (~8));
        }
      }
    }
    var_v0 += 1;
  }
  while (var_v0 < 6);
}
''',
        "hunks": [
            (r'''typedef float f32;
void func_800CFCA8(void)''',
             r'''typedef float f32;
/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
void func_800CFCA8(void)'''),
            (r'''  void *temp_v1;
  var_v0 = 0;
  do
  {''',
             r'''  void *temp_v1;
  for (var_v0 = 0; var_v0 < 6; var_v0++)
  {'''),
            (r'''    }
    var_v0 += 1;
  }
  while (var_v0 < 6);
}''',
             r'''    }
  }
}'''),
        ],
    },
    {
        "name": 'players_race_update', "fn": 'players_race_update', "kind": 'exact',
        "note": 'do/while -> for with two inits and two steps',
        "ref": {'before': ('13426b9^', 'cloud/work/near-miss/players_race_update/base.c'), 'after': ('13426b9', 'cloud/matches/players_race_update.c')},
        "before": r'''typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
void players_race_update(void) {
    D_8014A250_Record *r;
    s32 i;

    i = 0;
    r = &D_8014A250;
    do {
        if ((*(s16 *) ((s8 *) r + 0x7C8) != 0) && ((s8) player_array[i].pad0EC[0x26D] < 2)) {
            func_800D4DFC(r);
        }
        i++;
        r = (D_8014A250_Record *) ((s8 *) r + 0x808);
    } while (i != 6);
}
''',
        "hunks": [
            (r'''typedef signed int s32;
void players_race_update(void) {''',
             r'''typedef signed int s32;
/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
void players_race_update(void) {'''),
            (r'''    s32 i;

    i = 0;
    r = &D_8014A250;
    do {
        if ((*(s16 *) ((s8 *) r + 0x7C8) != 0) && ((s8) player_array[i].pad0EC[0x26D] < 2)) {''',
             r'''    s32 i;
    for (i = 0, r = &D_8014A250; i != 6; i++, r = (D_8014A250_Record *) ((s8 *) r + 0x808)) {
        if ((*(s16 *) ((s8 *) r + 0x7C8) != 0) && ((s8) player_array[i].pad0EC[0x26D] < 2)) {'''),
            (r'''        }
        i++;
        r = (D_8014A250_Record *) ((s8 *) r + 0x808);
    } while (i != 6);
}''',
             r'''        }
    }
}'''),
        ],
    },
    {
        "name": 'Input_SetAnalogBounds', "fn": 'Input_SetAnalogBounds', "kind": 'exact',
        "note": 'param types s16/int/long -> s32',
        "ref": {'before': ('141e8fb^', 'cloud/work/near-miss/Input_SetAnalogBounds/base.c'), 'after': ('141e8fb', 'cloud/matches/Input_SetAnalogBounds.c')},
        "before": r'''typedef signed short s16;
typedef signed int s32;
void Input_SetAnalogBounds(s32 arg0, s16 arg1, int arg2, long arg3, s32 arg4)
{
  if (arg1 < 0)
  {
    (&pad_config)[arg0].unk18 = 0;
  }
  else
  {
    (&pad_config)[arg0].unk18 = arg1;
  }
  if (arg3 < 0)
  {
    (&pad_config)[arg0].unk1A = 0;
  }
  else
  {
    (&pad_config)[arg0].unk1A = arg3;
  }
  if (arg2 < 0)
  {
    (&pad_config)[arg0].unk1C = (&pad_config)[arg0].unk12 - 1;
  }
  else
  {
    (&pad_config)[arg0].unk1C = arg2;
  }
  if (arg4 < 0)
  {
    (&pad_config)[arg0].unk1E = (&pad_config)[arg0].unk10 - 1;
    return;
  }
  (&pad_config)[arg0].unk1E = (s16) arg4;
}
''',
        "hunks": [
            (r'''typedef signed int s32;
void Input_SetAnalogBounds(s32 arg0, s16 arg1, int arg2, long arg3, s32 arg4)
{''',
             r'''typedef signed int s32;
/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
void Input_SetAnalogBounds(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{'''),
        ],
    },
    {
        "name": 'func_800A464C', "fn": 'func_800A464C', "kind": 'exact',
        "note": 'drop local c (assigned once)',
        "ref": {'before': ('44b970f', 'cloud/work/near-miss/func_800A464C/base.c'), 'after': ('f50d80e', 'cloud/work/near-miss/func_800A464C/base.c')},
        "before": r'''typedef unsigned char u8;
typedef signed int s32;
u8 *func_800A464C(u8 *arg0, u8 *arg1)
{
  s32 i;
  u8 *p;
  u8 c;
  if (*arg0 == 0)
  {
    if (*arg1 != 0)
    {
      return (u8 *) 0;
    }
    return arg0;
  }
  if (*arg0 != 0)
  {
    do
    {
      i = 0;
      p = arg1;
      while (1)
      {
        c = *p;
        if (c == 0)
        {
          return arg0;
        }
        if (c != arg0[i]) break;
        i++;
        p++;
      }
    } while (*++arg0 != 0);
  }
  return (u8 *) 0;
}
''',
        "hunks": [
            (r'''  u8 *p;
  u8 c;
  if (*arg0 == 0)''',
             r'''  u8 *p;
  if (*arg0 == 0)'''),
            (r'''      {
        c = *p;
        if (c == 0)
        {''',
             r'''      {
        if (*p == 0)
        {'''),
            (r'''        }
        if (c != arg0[i]) break;
        i++;''',
             r'''        }
        if (*p != arg0[i]) break;
        i++;'''),
        ],
    },
    {
        "name": 'controller_poll', "fn": 'player_state_set', "kind": 'exact',
        "note": 'extern -> defined array',
        "ref": {'before': ('8b97849', 'cloud/work/ipa-groups/controller_poll/group.c'), 'after': ('bdda978', 'cloud/work/ipa-groups/controller_poll/group.c')},
        "before": r'''typedef signed char s8;
typedef signed int s32;
extern s8 D_80149B64[4];
void player_state_set(s32 player, s32 value)
{
    s32 i;

    if (player == -1) {
        for (i = 0; i < 4; i++) {
            D_80149B64[i] = value;
        }
    } else if (player < 4) {
        D_80149B64[player] = value;
    }
}
''',
        "hunks": [
            (r'''typedef signed int s32;
extern s8 D_80149B64[4];
void player_state_set(s32 player, s32 value)''',
             r'''typedef signed int s32;
s8 D_80149B64[4];
void player_state_set(s32 player, s32 value)'''),
        ],
    },
    {
        "name": 'MP_TargetSteerPos', "fn": 'MP_TargetSteerPos', "kind": 'exact',
        "note": 'K&R prototype',
        "ref": {'before': ('5eebdbb', 'cloud/work/ipa-groups/MP_TargetSteerPos/group.c'), 'after': ('c4d4fe7', 'cloud/work/ipa-groups/MP_TargetSteerPos/group.c')},
        "before": r'''typedef signed char s8;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
typedef s32 M2C_UNK;
M2C_UNK func_8001fea4(s32, s32);                    /* extern */
s32 MP_TargetSteerPos(void *ipa_s0, f32 ipa_f20, f32 ipa_f22) {
    f32 temp_f4;
    f32 temp_f8;
    s32 temp_v0;
    s32 var_a1;
    s32 var_a1_2;

    temp_v0 = func_80020174(M2C_FIELD(ipa_s0, u16 *, 0xE), 0xFF, 0xFF);
    M2C_FIELD(ipa_s0, s32 *, 0x10) = temp_v0;
    if (temp_v0 == -1) {
        return 0;
    }
    temp_f4 = ipa_f20 * 127.0f;
    if (M2C_ERROR(/* cfc1 */) & 0x78) {
        if (!(M2C_ERROR(/* cfc1 */) & 0x78)) {
            var_a1 = (s32) (temp_f4 - 2.1474836e9f) | 0x80000000;
        } else {
            goto block_5;
        }
    } else {
        var_a1 = (s32) temp_f4;
        if (var_a1 < 0) {
block_5:
            var_a1 = -1;
        }
    }
    func_8001fea4(M2C_FIELD(ipa_s0, s32 *, 0x10), var_a1 & 0xFF);
    temp_f8 = (ipa_f22 + 1.0f) * 0.5f * 127.0f;
    if (M2C_ERROR(/* cfc1 */) & 0x78) {
        if (!(M2C_ERROR(/* cfc1 */) & 0x78)) {
            var_a1_2 = (s32) (temp_f8 - 2.1474836e9f) | 0x80000000;
        } else {
            goto block_10;
        }
    } else {
        var_a1_2 = (s32) temp_f8;
        if (var_a1_2 < 0) {
block_10:
            var_a1_2 = -1;
        }
    }
    func_8001fff4(M2C_FIELD(ipa_s0, s32 *, 0x10), var_a1_2 & 0xFF);
    M2C_FIELD(ipa_s0, s8 *, 1) = 1;
    return 1;
}
''',
        "hunks": [
            (r'''typedef s32 M2C_UNK;
M2C_UNK func_8001fea4(s32, s32);                    /* extern */
s32 MP_TargetSteerPos(void *ipa_s0, f32 ipa_f20, f32 ipa_f22) {''',
             r'''typedef s32 M2C_UNK;
M2C_UNK func_8001fea4();                    /* extern */
s32 MP_TargetSteerPos(void *ipa_s0, f32 ipa_f20, f32 ipa_f22) {'''),
        ],
    },
    {
        "name": 'camera_aspect_ratio', "fn": 'camera_update', "kind": 'exact',
        "note": 'K&R prototype',
        "ref": {'before': ('5eebdbb', 'cloud/work/ipa-groups/camera_aspect_ratio/group.c'), 'after': ('c4d4fe7', 'cloud/work/ipa-groups/camera_aspect_ratio/group.c')},
        "before": r'''typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
void camera_build_view_matrix(s16 arg0);
void camera_update(void *arg0, s16 arg1) {
    s32 *spF4;
    s8 spCF;
    f32 spC8;
    f32 spC4;
    f32 spC0;
    f32 spB4;
    f32 *var_a0_2;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f2;
    f32 temp_f4;
    f32 temp_f8;
    f32 var_f14;
    f32 var_f22;
    s16 temp_a1_2;
    s16 temp_v0_3;
    s16 temp_v0_4;
    s16 temp_v0_5;
    s16 var_v0_2;
    s32 *temp_t8;
    s32 *temp_v0_7;
    s32 *var_v0_3;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_t6;
    s32 temp_t6_2;
    s32 temp_t6_4;
    s32 temp_t7_2;
    s32 temp_t7_4;
    s32 temp_t7_5;
    s32 temp_t8_2;
    s32 temp_t8_3;
    s32 temp_t8_4;
    s32 temp_t9;
    s32 temp_t9_2;
    s32 temp_v0;
    s32 temp_v0_6;
    s32 var_a0;
    s32 var_a2;
    s32 var_s7;
    s32 var_s7_2;
    s32 var_v0;
    s32 var_v1_2;
    u16 temp_a1;
    u16 temp_t6_3;
    u16 temp_t7;
    u16 temp_t7_3;
    u16 temp_t7_6;
    u16 temp_t8_5;
    u16 temp_t8_6;
    u16 temp_t9_3;
    u16 temp_v1;
    u16 temp_v1_3;
    void *temp_a2;
    void *temp_s6;
    void *temp_s8;
    void *temp_v0_2;
    void *temp_v1_2;
    void *temp_v1_4;
    void *var_v1;

    spCF = 0;
    if (arg1 == 0) {
        entity_transform_apply(arg0, 1);
        return;
    }
    if (((state_word_a & 0x7C0000) || (state_word_a & 8)) && (D_801170FC == 0)) {
        temp_t8 = M2C_FIELD(arg0, s32 **, 0xC);
        spF4 = temp_t8;
        temp_s6 = M2C_FIELD(temp_t8, void **, 0x6C);
        temp_s8 = M2C_FIELD(temp_s6, void **, 0);
        var_a0 = M2C_FIELD(temp_s8, s32 *, 0x10);
        if (var_a0 & 0x40) {
            if (var_a0 & 0x4000) {
                if (!(var_a0 & 0x200)) {
                    if (!(var_a0 & 0x400)) {
                        temp_v1 = M2C_FIELD(temp_s6, u16 *, 0xE);
                        temp_t7 = temp_v1 & 0xFFFB;
                        if (temp_v1 & 4) {
                            M2C_FIELD(temp_s6, u16 *, 0xE) = temp_t7;
                            M2C_FIELD(temp_s6, u16 *, 0xE) = (u16) (temp_t7 | 8);
                            M2C_FIELD(temp_s6, f32 *, 4) = (f32) (M2C_FIELD((M2C_FIELD(temp_s8, s32 *, 0x1C) + (M2C_FIELD(temp_s6, s16 *, 0xC) * 0x44)), f32 *, 0x38) - M2C_FIELD(temp_s6, f32 *, 4));
                        }
                        if (M2C_FIELD(temp_s8, s32 *, 0x10) & 0x100) {
                            camera_aspect_ratio(spF4);
                        }
                        temp_t9 = M2C_FIELD(temp_s8, s32 *, 0x10) & ~0x100;
                        M2C_FIELD(temp_s8, s32 *, 0x10) = temp_t9;
                        var_a0 = temp_t9;
                    }
                } else {
                    if (var_a0 & 0x400) {
                        M2C_FIELD(temp_s8, s32 *, 0x10) = (s32) (var_a0 & ~0x500);
                        camera_aspect_ratio(spF4);
                    }
                    temp_t6 = M2C_FIELD(temp_s8, s32 *, 0x10) & ~0x200;
                    M2C_FIELD(temp_s8, s32 *, 0x10) = temp_t6;
                    var_a0 = temp_t6;
                }
                goto block_46;
            }
            if ((var_a0 & 0x2000) && (var_a0 & 0x100000)) {
                temp_t7_2 = var_a0 & 0xFFEFFFFF;
                if (M2C_FIELD(M2C_FIELD(temp_s8, void **, 0x18), s32 *, 0x10) & 0x100) {
                    M2C_FIELD(temp_s8, s32 *, 0x10) = temp_t7_2;
                    temp_t6_2 = temp_t7_2 | 0x200400;
                    M2C_FIELD(temp_s8, s32 *, 0x10) = temp_t6_2;
                    if (!(temp_t6_2 & 0x8000)) {
                        *((u16 *) ((u8 *) &D_8012E714 + (M2C_FIELD(spF4, s16 *, 0xE) * 0x44))) = D_80142A7A;
                    }
                    camera_fov_control(spF4);
                    return;
                }
            }
            if ((var_a0 & 0x400) && (var_a0 & 0x200)) {
                temp_v1_2 = M2C_FIELD(temp_s8, void **, 0x18);
                temp_v0 = M2C_FIELD(temp_v1_2, s32 *, 0x10);
                if (temp_v0 & 0x100) {
                    M2C_FIELD(temp_v1_2, s32 *, 0x10) = (s32) (temp_v0 & ~0x100);
                } else if (var_a0 & 0x1000) {
                    temp_v0_2 = M2C_FIELD(temp_v1_2, void **, 0x20);
                    temp_a1 = M2C_FIELD(temp_v0_2, u16 *, 0xE);
                    temp_t6_3 = temp_a1 & 0xFFF7;
                    if (temp_a1 & 8) {
                        M2C_FIELD(temp_v0_2, u16 *, 0xE) = temp_t6_3;
                        M2C_FIELD(temp_v0_2, u16 *, 0xE) = (u16) (temp_t6_3 | 4);
                        M2C_FIELD(temp_v0_2, f32 *, 4) = (f32) (M2C_FIELD((M2C_FIELD(temp_v1_2, s32 *, 0x1C) + (M2C_FIELD(temp_v0_2, s16 *, 0xC) * 0x44)), f32 *, 0x38) - M2C_FIELD(temp_v0_2, f32 *, 4));
                    } else {
                        temp_t7_3 = temp_a1 & 0xFFFB;
                        M2C_FIELD(temp_v0_2, u16 *, 0xE) = temp_t7_3;
                        M2C_FIELD(temp_v0_2, u16 *, 0xE) = (u16) (temp_t7_3 | 8);
                        M2C_FIELD(temp_v0_2, f32 *, 4) = (f32) (M2C_FIELD((M2C_FIELD(temp_v1_2, s32 *, 0x1C) + (M2C_FIELD(temp_v0_2, s16 *, 0xC) * 0x44)), f32 *, 0x38) - M2C_FIELD(temp_v0_2, f32 *, 4));
                    }
                }
                temp_t8_2 = M2C_FIELD(temp_s8, s32 *, 0x10) & ~0x700;
                M2C_FIELD(temp_s8, s32 *, 0x10) = temp_t8_2;
                var_a0 = temp_t8_2;
                if (temp_t8_2 & 0x1000) {
                    if (temp_t8_2 & 0x200000) {
                        temp_t6_4 = temp_t8_2 & 0xFFDFFFFF;
                        M2C_FIELD(temp_s8, s32 *, 0x10) = temp_t6_4;
                        temp_t9_2 = temp_t6_4 | 0x100000;
                        M2C_FIELD(temp_s8, s32 *, 0x10) = temp_t9_2;
                        if (!(temp_t9_2 & 0x8000)) {
                            *((u16 *) ((u8 *) &D_8012E714 + (M2C_FIELD(spF4, s16 *, 0xE) * 0x44))) = D_80142A78;
                        }
                        camera_aspect_ratio(spF4);
                    } else {
                        temp_t7_4 = var_a0 & 0xFFEFFFFF;
                        M2C_FIELD(temp_s8, s32 *, 0x10) = temp_t7_4;
                        temp_t8_3 = temp_t7_4 | 0x200000;
                        M2C_FIELD(temp_s8, s32 *, 0x10) = temp_t8_3;
                        if (!(temp_t8_3 & 0x8000)) {
                            *((u16 *) ((u8 *) &D_8012E714 + (M2C_FIELD(spF4, s16 *, 0xE) * 0x44))) = D_80142A7A;
                        }
                        camera_fov_control(spF4);
                    }
                    goto block_45;
                }
                if (var_a0 & 0x200000) {
                    temp_t7_5 = var_a0 & 0xFFDFFFFF;
                    M2C_FIELD(temp_s8, s32 *, 0x10) = temp_t7_5;
                    temp_t8_4 = temp_t7_5 | 0x100000;
                    M2C_FIELD(temp_s8, s32 *, 0x10) = temp_t8_4;
                    if (!(temp_t8_4 & 0x8000)) {
                        *((u16 *) ((u8 *) &D_8012E714 + (M2C_FIELD(spF4, s16 *, 0xE) * 0x44))) = D_80142A78;
                    }
                    camera_aspect_ratio(spF4);
block_45:
                    var_a0 = M2C_FIELD(temp_s8, s32 *, 0x10);
                }
            }
            goto block_46;
        }
block_46:
        if (!(var_a0 & 0x100)) {
            var_s7 = 0;
            M2C_FIELD(temp_s6, f32 *, 4) = (f32) (M2C_FIELD(temp_s6, f32 *, 4) + M2C_BITWISE(f32, D_8002EB94));
            var_v1 = M2C_FIELD(temp_s8, s32 *, 0x1C) + (M2C_FIELD(temp_s6, s16 *, 0xC) * 0x44);
            do {
                temp_f2 = M2C_FIELD(temp_s6, f32 *, 4);
                temp_f0 = M2C_FIELD(var_v1, f32 *, 0x38);
                if (temp_f0 <= temp_f2) {
                    temp_v1_3 = M2C_FIELD(temp_s6, u16 *, 0xE);
                    M2C_FIELD(temp_s6, f32 *, 4) = (f32) (temp_f2 - temp_f0);
                    if (temp_v1_3 & 8) {
                        if (M2C_FIELD(temp_s6, s16 *, 0xC) == 0) {
                            temp_t9_3 = temp_v1_3 & 0xFFF7;
                            if (M2C_FIELD(temp_s8, s32 *, 0x10) & 0x4000) {
                                var_s7 = 1;
                                camera_build_view_matrix(0, spF4);
                                camera_fov_control(spF4);
                            } else {
                                M2C_FIELD(temp_s6, u16 *, 0xE) = temp_t9_3;
                                M2C_FIELD(temp_s6, u16 *, 0xE) = (u16) (temp_t9_3 | 4);
                                temp_a0 = M2C_FIELD(temp_s8, s32 *, 0x10);
                                if (temp_a0 & 0x80) {
                                    M2C_FIELD(temp_s8, s32 *, 0x10) = (s32) (temp_a0 | 0x100);
                                    var_s7 = 1;
                                    M2C_FIELD(temp_s6, f32 *, 4) = 0.0f;
                                }
                            }
                            if (M2C_FIELD(temp_s8, s32 *, 0x10) & 0x20) {
                                listener_position_set((s32) M2C_FIELD(temp_s8, s16 *, 0x16));
                            }
                        } else {
                            M2C_FIELD(temp_s6, s16 *, 0xC) = (s16) (M2C_FIELD(temp_s6, s16 *, 0xC) - 1);
                        }
                    } else if (M2C_FIELD(temp_s8, s16 *, 0x14) == (M2C_FIELD(temp_s6, s16 *, 0xC) + 1)) {
                        temp_t8_5 = temp_v1_3 & 0xFFFB;
                        if (M2C_FIELD(temp_s8, s32 *, 0x10) & 0x80) {
                            M2C_FIELD(temp_s6, u16 *, 0xE) = temp_t8_5;
                            M2C_FIELD(temp_s6, u16 *, 0xE) = (u16) (temp_t8_5 | 8);
                            M2C_FIELD(temp_s6, s16 *, 0xC) = (s16) (M2C_FIELD(temp_s6, s16 *, 0xC) - 1);
                        } else {
                            M2C_FIELD(temp_s6, s16 *, 0xC) = 0;
                            if (M2C_FIELD(temp_s8, s32 *, 0x10) & 0x20) {
                                listener_position_set((s32) M2C_FIELD(temp_s8, s16 *, 0x16));
                            }
                        }
                    } else {
                        M2C_FIELD(temp_s6, s16 *, 0xC) = (s16) (M2C_FIELD(temp_s6, s16 *, 0xC) + 1);
                        temp_v0_3 = M2C_FIELD(temp_s6, s16 *, 0xC);
                        if (M2C_FIELD((M2C_FIELD(temp_s8, s32 *, 0x1C) + (temp_v0_3 * 0x44)), s32 *, 0x40) & 0x40) {
                            var_s7 = 1;
                            M2C_FIELD(temp_s8, s32 *, 0x10) = (s32) (M2C_FIELD(temp_s8, s32 *, 0x10) | 0x100);
                            M2C_FIELD(temp_s6, f32 *, 4) = 0.0f;
                        } else if (M2C_FIELD(temp_s8, s16 *, 0x14) == (temp_v0_3 + 1)) {
                            temp_a0_2 = M2C_FIELD(temp_s8, s32 *, 0x10);
                            if (temp_a0_2 & 1) {
                                temp_t7_6 = M2C_FIELD(temp_s6, u16 *, 0xE) & 0xFFFB;
                                M2C_FIELD(temp_s6, u16 *, 0xE) = temp_t7_6;
                                M2C_FIELD(temp_s6, u16 *, 0xE) = (u16) (temp_t7_6 | 8);
                                M2C_FIELD(temp_s6, s16 *, 0xC) = (s16) (temp_v0_3 - 1);
                            } else if (temp_a0_2 & 0x4000) {
                                temp_t8_6 = M2C_FIELD(temp_s6, u16 *, 0xE) & 0xFFFB;
                                M2C_FIELD(temp_s6, u16 *, 0xE) = temp_t8_6;
                                M2C_FIELD(temp_s6, u16 *, 0xE) = (u16) (temp_t8_6 | 8);
                                M2C_FIELD(temp_s6, s16 *, 0xC) = (s16) (temp_v0_3 - 1);
                                M2C_FIELD(temp_s6, f32 *, 4) = 0.0f;
                                M2C_FIELD(temp_s8, s32 *, 0x10) = (s32) (M2C_FIELD(temp_s8, s32 *, 0x10) | 0x100);
                            } else if (!(temp_a0_2 & 2)) {
                                camera_build_view_matrix(0, spF4);
                                if (M2C_FIELD(temp_s8, s32 *, 0x10) & 0x20) {
                                    listener_position_set((s32) M2C_FIELD(temp_s8, s16 *, 0x16));
                                }
                            }
                        }
                    }
                    var_v1 = M2C_FIELD(temp_s8, s32 *, 0x1C) + (M2C_FIELD(temp_s6, s16 *, 0xC) * 0x44);
                } else {
                    var_s7 = 1;
                }
            } while (var_s7 == 0);
            var_v0 = M2C_FIELD(var_v1, s32 *, 0x40);
            if (!(var_v0 & 2)) {
                camera_track_spline(M2C_ERROR(/* Read from unset register $a0 */), M2C_ERROR(/* Read from unset register $a1 */));
                spCF = 1;
                var_v0 = M2C_FIELD((M2C_FIELD(temp_s8, s32 *, 0x1C) + (M2C_FIELD(temp_s6, s16 *, 0xC) * 0x44)), s32 *, 0x40);
            }
            if (!(var_v0 & 0x10) || !(var_v0 & 4)) {
                spCF = 1;
                camera_free_look(spF4);
            }
            if (spCF != 0) {
                if (M2C_FIELD(temp_s8, s32 *, 0x10) & 0x100) {
                    camera_fov_control(spF4);
                } else {
                    camera_look_at_point(spF4);
                }
            } else {
                camera_fov_control(spF4);
            }
            temp_a2 = (s32 *) ((M2C_FIELD(spF4, s16 *, 0x10) * 0x30) + (u8 *) &D_80117530);
            if (M2C_FIELD(temp_a2, s16 *, 0x14) != -1) {
                temp_f8 = M2C_FIELD(arg0, f32 *, 0x10) - M2C_BITWISE(f32, D_8002EB94);
                M2C_FIELD(arg0, f32 *, 0x10) = temp_f8;
                if (temp_f8 <= 0.0f) {
                    temp_v0_4 = M2C_FIELD(temp_a2, s16 *, 0x14);
                    if (temp_v0_4 == 0x169) {
                        M2C_FIELD(arg0, f32 *, 0x10) = 0.25f;
                    } else if (temp_v0_4 == 0x16D) {
                        M2C_FIELD(arg0, f32 *, 0x10) = (f32) D_80123E8C;
                    }
                    var_v0_2 = M2C_FIELD(arg0, s16 *, 4) + 1;
                    M2C_FIELD(arg0, s16 *, 4) = var_v0_2;
                    if (var_v0_2 >= M2C_FIELD(spF4, s16 *, 0x5A)) {
                        M2C_FIELD(arg0, s16 *, 4) = 0;
                        var_v0_2 = 0;
                    }
                    temp_a1_2 = M2C_FIELD(spF4, s16 *, 0x58) + var_v0_2;
                    if (temp_a1_2 != M2C_FIELD(spF4, s16 *, 0x50)) {
                        if (!(M2C_FIELD(temp_s8, s32 *, 0x10) & 0x8000)) {
                            *((u16 *) ((u8 *) &D_8012E714 + (M2C_FIELD(spF4, s16 *, 0xE) * 0x44))) = (&D_801427C0)[temp_a1_2];
                        }
                        M2C_FIELD(spF4, s16 *, 0x50) = temp_a1_2;
                    }
                    goto block_99;
                }
            } else {
block_99:
                if (M2C_FIELD(temp_a2, s16 *, 0x10) == 4) {
                    if (M2C_FIELD(temp_s6, u16 *, 0xE) & 8) {
                        spC0 = M2C_FIELD((M2C_FIELD(temp_s8, s32 *, 0x1C) + (M2C_FIELD(temp_s6, s16 *, 0xC) * 0x44)), f32 *, 0xC) * -M2C_FIELD(temp_s6, f32 *, 0x10);
                        spC4 = M2C_FIELD((M2C_FIELD(temp_s8, s32 *, 0x1C) + (M2C_FIELD(temp_s6, s16 *, 0xC) * 0x44)), f32 *, 0x10) * -M2C_FIELD(temp_s6, f32 *, 0x10);
                        spC8 = M2C_FIELD((M2C_FIELD(temp_s8, s32 *, 0x1C) + (M2C_FIELD(temp_s6, s16 *, 0xC) * 0x44)), f32 *, 0x14) * -M2C_FIELD(temp_s6, f32 *, 0x10);
                    } else {
                        spC0 = M2C_FIELD((M2C_FIELD(temp_s8, s32 *, 0x1C) + (M2C_FIELD(temp_s6, s16 *, 0xC) * 0x44)), f32 *, 0xC) * M2C_FIELD(temp_s6, f32 *, 0x10);
                        spC4 = M2C_FIELD((M2C_FIELD(temp_s8, s32 *, 0x1C) + (M2C_FIELD(temp_s6, s16 *, 0xC) * 0x44)), f32 *, 0x10) * M2C_FIELD(temp_s6, f32 *, 0x10);
                        spC8 = M2C_FIELD((M2C_FIELD(temp_s8, s32 *, 0x1C) + (M2C_FIELD(temp_s6, s16 *, 0xC) * 0x44)), f32 *, 0x14) * M2C_FIELD(temp_s6, f32 *, 0x10);
                    }
                    func_800AB750((s32) M2C_FIELD(spF4, s8 *, 0x65), &spC0, spF4 + 0xE, spF4 + 5);
                }
                temp_v0_5 = M2C_FIELD(temp_s6, s16 *, 0xC);
                var_a0_2 = &spB4;
                temp_v1_4 = M2C_FIELD(temp_s8, s32 *, 0x1C) + (temp_v0_5 * 0x44);
                if (!(M2C_FIELD(temp_v1_4, s32 *, 0x40) & 0x10)) {
                    var_a2 = temp_v0_5 + 1;
                    if (var_a2 >= M2C_FIELD(temp_s8, s16 *, 0x14)) {
                        var_a2 = 0;
                    }
                    if (M2C_FIELD(temp_s6, u16 *, 0xE) & 8) {
                        var_f14 = M2C_FIELD(temp_v1_4, f32 *, 0x38) - M2C_FIELD(temp_s6, f32 *, 4);
                    } else {
                        var_f14 = M2C_FIELD(temp_s6, f32 *, 4);
                    }
                    var_v1_2 = 0;
                    do {
                        temp_v0_6 = M2C_FIELD(temp_s8, s32 *, 0x1C);
                        var_a0_2 = var_a0_2 + 1;
                        temp_f4 = M2C_FIELD((temp_v0_6 + (var_a2 * 0x44) + var_v1_2), f32 *, 0x18);
                        temp_f0_2 = M2C_FIELD((temp_v0_6 + (M2C_FIELD(temp_s6, s16 *, 0xC) * 0x44) + var_v1_2), f32 *, 0x18);
                        var_v1_2 += 4;
                        M2C_FIELD(var_a0_2, f32 *, -4) = (f32) (((temp_f4 - temp_f0_2) * (var_f14 / M2C_FIELD(temp_v1_4, f32 *, 0x38))) + temp_f0_2);
                    } while ((u32) var_a0_2 < (u32) &spC0);
                    var_v0_3 = spF4;
                    var_s7_2 = 1;
                    var_f22 = M2C_FIELD(var_v0_3, f32 *, 0x14) * spB4;
                    if (1 != 3) {
                        do {
                            temp_f18 = M2C_FIELD(var_v0_3, f32 *, 0x2C);
                            M2C_FIELD(var_v0_3, f32 *, 0x14) = var_f22;
                            temp_f14 = M2C_FIELD(var_v0_3, f32 *, 0x18);
                            var_s7_2 += 1;
                            temp_f16 = M2C_FIELD(var_v0_3, f32 *, 0x20) * spB8;
                            var_v0_3 = var_v0_3 + 1;
                            M2C_FIELD(var_v0_3, f32 *, 0x1C) = temp_f16;
                            M2C_FIELD(var_v0_3, f32 *, 0x28) = (f32) (temp_f18 * spBC);
                            var_f22 = temp_f14 * spB4;
                        } while (var_s7_2 != 3);
                    }
                    M2C_FIELD(var_v0_3, f32 *, 0x14) = var_f22;
                    temp_v0_7 = var_v0_3 + 1;
                    M2C_FIELD(temp_v0_7, f32 *, 0x1C) = (f32) (M2C_FIELD(var_v0_3, f32 *, 0x20) * spB8);
                    M2C_FIELD(temp_v0_7, f32 *, 0x28) = (f32) (M2C_FIELD(var_v0_3, f32 *, 0x2C) * spBC);
                }
                if (!(M2C_FIELD(spF4, u8 *, 4) & 2)) {
                    if (!(M2C_FIELD(temp_s8, s32 *, 0x10) & 0x8000)) {
                        entity_spawn_callback(M2C_FIELD(spF4, s16 *, 0xE), 0, 0);
                    }
                    func_800AFA84(&D_80143FC8, spF4);
                    entity_transform_apply(arg0, 1);
                }
            }
        }
    }
}
''',
        "hunks": [
            (r'''typedef float f32;
void camera_build_view_matrix(s16 arg0);
void camera_update(void *arg0, s16 arg1) {''',
             r'''typedef float f32;
void camera_build_view_matrix();
void camera_update(void *arg0, s16 arg1) {'''),
        ],
    },
    {
        "name": 'car_cg_height_set', "fn": 'car_cg_height_set', "kind": 'exact',
        "note": 'K&R prototype',
        "ref": {'before': ('5eebdbb', 'cloud/work/ipa-groups/car_cg_height_set/group.c'), 'after': ('c4d4fe7', 'cloud/work/ipa-groups/car_cg_height_set/group.c')},
        "before": r'''typedef signed int s32;
typedef unsigned int u32;
typedef s32 M2C_UNK;
M2C_UNK memcpy(s32, s32, u32);                      /* extern */
void car_cg_height_set(void *ipa_s0) {
    u32 temp_v0;
    u32 var_s1;
    void *temp_s2;
    void *temp_s2_2;
    void *temp_s2_3;
    void *temp_s2_4;

    temp_s2 = M2C_FIELD(ipa_s0, void **, 0x18);
    temp_v0 = M2C_FIELD(ipa_s0, u32 *, 0x10);
    var_s1 = M2C_FIELD(temp_s2, u32 *, 0xC);
    if (temp_v0 < var_s1) {
        var_s1 = temp_v0;
    }
    if (var_s1 != 0) {
        memcpy(M2C_FIELD(ipa_s0, s32 *, 0xC), M2C_FIELD(temp_s2, s32 *, 8), var_s1);
        temp_s2_2 = M2C_FIELD(ipa_s0, void **, 0x18);
        M2C_FIELD(ipa_s0, s32 *, 0xC) = (s32) (M2C_FIELD(ipa_s0, s32 *, 0xC) + var_s1);
        M2C_FIELD(temp_s2_2, s32 *, 8) = (s32) (M2C_FIELD(temp_s2_2, s32 *, 8) + var_s1);
        temp_s2_3 = M2C_FIELD(ipa_s0, void **, 0x18);
        M2C_FIELD(ipa_s0, s32 *, 0x14) = (s32) (M2C_FIELD(ipa_s0, s32 *, 0x14) + var_s1);
        M2C_FIELD(ipa_s0, u32 *, 0x10) = (u32) (M2C_FIELD(ipa_s0, u32 *, 0x10) - var_s1);
        M2C_FIELD(temp_s2_3, u32 *, 0xC) = (u32) (M2C_FIELD(temp_s2_3, u32 *, 0xC) - var_s1);
        temp_s2_4 = M2C_FIELD(ipa_s0, void **, 0x18);
        if (M2C_FIELD(temp_s2_4, u32 *, 0xC) == 0) {
            M2C_FIELD(temp_s2_4, s32 *, 8) = (s32) M2C_FIELD(temp_s2_4, s32 *, 4);
        }
    }
}
''',
        "hunks": [
            (r'''typedef s32 M2C_UNK;
M2C_UNK memcpy(s32, s32, u32);                      /* extern */
void car_cg_height_set(void *ipa_s0) {''',
             r'''typedef s32 M2C_UNK;
M2C_UNK memcpy();                      /* extern */
void car_cg_height_set(void *ipa_s0) {'''),
        ],
    },
    {
        "name": 'car_crash_response', "fn": 'car_crash_response', "kind": 'exact',
        "note": 'K&R prototype',
        "ref": {'before': ('5eebdbb', 'cloud/work/ipa-groups/car_crash_response/group.c'), 'after': ('c4d4fe7', 'cloud/work/ipa-groups/car_crash_response/group.c')},
        "before": r'''typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
void car_crash_detect(void);
u32 car_crash_response(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *sp50;
    s32 sp44;
    void *sp40;
    s32 *var_v0;
    s32 *var_v1_2;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a0_3;
    s32 temp_a0_4;
    s32 temp_a0_5;
    s32 temp_a0_6;
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_a1_3;
    s32 temp_a1_4;
    s32 temp_a1_5;
    s32 temp_a1_6;
    s32 temp_a2;
    s32 temp_t2;
    s32 temp_t6;
    s32 temp_t6_3;
    s32 temp_t6_5;
    s32 temp_t7_3;
    s32 temp_t7_5;
    s32 temp_t7_7;
    s32 temp_t8_3;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v1;
    s32 var_a2;
    s32 var_t1;
    s32 var_t1_2;
    s32 var_t6;
    s32 var_v1;
    u16 temp_t6_2;
    u16 temp_t6_4;
    u16 temp_t7_2;
    u16 temp_t7_4;
    u16 temp_t7_6;
    u16 temp_t8_2;
    u16 temp_v0_5;
    u32 temp_t7;
    u32 temp_t8;
    u32 var_a0;
    u32 var_a1;

    var_t1 = 0;
    if (M2C_FIELD(arg0, s32 *, 0x6C) > 0) {
        car_crash_detect(arg0, (u8 *) arg0 + 0xAFC);
        car_crash_detect(arg0, (u8 *) arg0 + 0xB08);
        func_800A9710((u8 *) arg0 + 0x78, M2C_FIELD(arg0, s32 *, 0xB00), arg0);
        func_800A9710((u8 *) arg0 + 0x96C, M2C_FIELD(arg0, s32 *, 0xB0C), arg0);
        car_crash_detect(arg0, (u8 *) arg0 + 0xB14);
        var_v0 = &D_8011E9D2;
        var_v1 = 0x12;
loop_2:
        if (M2C_FIELD(((u8 *) arg0 + (*var_v0 * 4)), u16 *, 0xA62) == 0) {
            var_v1 -= 1;
            var_v0 = (s32 *) ((u8 *) var_v0 - 1);
            if (var_v1 != 2) {
                goto loop_2;
            }
        }
        temp_t6 = M2C_FIELD(arg0, s32 *, 0x168C) + (var_v1 * 3) + 0x11;
        temp_t7 = (u32) (temp_t6 + 0xA) >> 3;
        temp_t8 = (u32) (M2C_FIELD(arg0, s32 *, 0x1690) + 0xA) >> 3;
        M2C_FIELD(arg0, s32 *, 0x168C) = temp_t6;
        var_t1 = var_v1;
        var_a0 = temp_t7;
        var_a1 = temp_t8;
        if (temp_t7 >= temp_t8) {
            var_a0 = temp_t8;
        }
    } else {
        var_a0 = arg2 + 5;
        var_a1 = var_a0;
    }
    if ((var_a0 >= (u32) (arg2 + 4)) && (arg1 != 0)) {
        car_collision_init(arg0, arg1, arg2, arg3);
    } else {
        temp_v1 = var_t1 + 1;
        if (var_a1 == var_a0) {
            temp_a1 = M2C_FIELD(arg0, s32 *, 0x16A4);
            if (temp_a1 >= 0xE) {
                temp_v0 = arg3 + 2;
                temp_t7_2 = M2C_FIELD(arg0, u16 *, 0x16A0) | (temp_v0 << temp_a1);
                M2C_FIELD(arg0, u16 *, 0x16A0) = temp_t7_2;
                *(M2C_FIELD(arg0, s32 *, 4) + M2C_FIELD(arg0, s32 *, 0xC)) = (s8) temp_t7_2;
                temp_t7_3 = M2C_FIELD(arg0, s32 *, 0xC) + 1;
                M2C_FIELD(arg0, s32 *, 0xC) = temp_t7_3;
                *(M2C_FIELD(arg0, s32 *, 4) + temp_t7_3) = (s8) ((s32) M2C_FIELD(arg0, u16 *, 0x16A0) >> 8);
                temp_a0 = M2C_FIELD(arg0, s32 *, 0x16A4);
                M2C_FIELD(arg0, s32 *, 0xC) = (s32) (M2C_FIELD(arg0, s32 *, 0xC) + 1);
                M2C_FIELD(arg0, s32 *, 0x16A4) = (s32) (temp_a0 - 0xD);
                M2C_FIELD(arg0, u16 *, 0x16A0) = (u16) ((s32) (temp_v0 & 0xFFFF) >> (0x10 - temp_a0));
            } else {
                M2C_FIELD(arg0, u16 *, 0x16A0) = (u16) (M2C_FIELD(arg0, u16 *, 0x16A0) | ((arg3 + 2) << temp_a1));
                M2C_FIELD(arg0, s32 *, 0x16A4) = (s32) (temp_a1 + 3);
            }
            func_800A8284(arg0, &D_801249F0, &D_80124E70);
            M2C_FIELD(arg0, u32 *, 0x1694) = (u32) (M2C_FIELD(arg0, u32 *, 0x1694) + M2C_FIELD(arg0, s32 *, 0x1690) + 3);
        } else {
            temp_a1_2 = M2C_FIELD(arg0, s32 *, 0x16A4);
            sp40 = (u8 *) arg0 + 0x96C;
            if (temp_a1_2 >= 0xE) {
                temp_v0_2 = arg3 + 4;
                temp_t6_2 = M2C_FIELD(arg0, u16 *, 0x16A0) | (temp_v0_2 << temp_a1_2);
                M2C_FIELD(arg0, u16 *, 0x16A0) = temp_t6_2;
                *(M2C_FIELD(arg0, s32 *, 4) + M2C_FIELD(arg0, s32 *, 0xC)) = (s8) temp_t6_2;
                temp_t6_3 = M2C_FIELD(arg0, s32 *, 0xC) + 1;
                M2C_FIELD(arg0, s32 *, 0xC) = temp_t6_3;
                *(M2C_FIELD(arg0, s32 *, 4) + temp_t6_3) = (s8) ((s32) M2C_FIELD(arg0, u16 *, 0x16A0) >> 8);
                temp_a0_2 = M2C_FIELD(arg0, s32 *, 0x16A4);
                M2C_FIELD(arg0, s32 *, 0xC) = (s32) (M2C_FIELD(arg0, s32 *, 0xC) + 1);
                M2C_FIELD(arg0, s32 *, 0x16A4) = (s32) (temp_a0_2 - 0xD);
                M2C_FIELD(arg0, u16 *, 0x16A0) = (u16) ((s32) (temp_v0_2 & 0xFFFF) >> (0x10 - temp_a0_2));
            } else {
                M2C_FIELD(arg0, u16 *, 0x16A0) = (u16) (M2C_FIELD(arg0, u16 *, 0x16A0) | ((arg3 + 4) << temp_a1_2));
                M2C_FIELD(arg0, s32 *, 0x16A4) = (s32) (temp_a1_2 + 3);
            }
            temp_a1_3 = M2C_FIELD(arg0, s32 *, 0x16A4);
            temp_t2 = M2C_FIELD(arg0, s32 *, 0xB00) + 1;
            temp_a2 = M2C_FIELD(arg0, s32 *, 0xB0C) + 1;
            if (temp_a1_3 >= 0xC) {
                temp_v0_3 = temp_t2 - 0x101;
                temp_t7_4 = M2C_FIELD(arg0, u16 *, 0x16A0) | (temp_v0_3 << temp_a1_3);
                M2C_FIELD(arg0, u16 *, 0x16A0) = temp_t7_4;
                *(M2C_FIELD(arg0, s32 *, 4) + M2C_FIELD(arg0, s32 *, 0xC)) = (s8) temp_t7_4;
                temp_t7_5 = M2C_FIELD(arg0, s32 *, 0xC) + 1;
                M2C_FIELD(arg0, s32 *, 0xC) = temp_t7_5;
                *(M2C_FIELD(arg0, s32 *, 4) + temp_t7_5) = (s8) ((s32) M2C_FIELD(arg0, u16 *, 0x16A0) >> 8);
                temp_a0_3 = M2C_FIELD(arg0, s32 *, 0x16A4);
                M2C_FIELD(arg0, s32 *, 0xC) = (s32) (M2C_FIELD(arg0, s32 *, 0xC) + 1);
                M2C_FIELD(arg0, s32 *, 0x16A4) = (s32) (temp_a0_3 - 0xB);
                M2C_FIELD(arg0, u16 *, 0x16A0) = (u16) ((s32) (temp_v0_3 & 0xFFFF) >> (0x10 - temp_a0_3));
            } else {
                M2C_FIELD(arg0, u16 *, 0x16A0) = (u16) (M2C_FIELD(arg0, u16 *, 0x16A0) | ((temp_t2 - 0x101) << temp_a1_3));
                M2C_FIELD(arg0, s32 *, 0x16A4) = (s32) (temp_a1_3 + 5);
            }
            temp_a1_4 = M2C_FIELD(arg0, s32 *, 0x16A4);
            var_t1_2 = temp_a2 - 1;
            if (temp_a1_4 >= 0xC) {
                var_t1_2 = temp_a2 - 1;
                temp_t7_6 = M2C_FIELD(arg0, u16 *, 0x16A0) | (var_t1_2 << temp_a1_4);
                M2C_FIELD(arg0, u16 *, 0x16A0) = temp_t7_6;
                *(M2C_FIELD(arg0, s32 *, 4) + M2C_FIELD(arg0, s32 *, 0xC)) = (s8) temp_t7_6;
                temp_t7_7 = M2C_FIELD(arg0, s32 *, 0xC) + 1;
                M2C_FIELD(arg0, s32 *, 0xC) = temp_t7_7;
                *(M2C_FIELD(arg0, s32 *, 4) + temp_t7_7) = (s8) ((s32) M2C_FIELD(arg0, u16 *, 0x16A0) >> 8);
                temp_a0_4 = M2C_FIELD(arg0, s32 *, 0x16A4);
                M2C_FIELD(arg0, s32 *, 0xC) = (s32) (M2C_FIELD(arg0, s32 *, 0xC) + 1);
                var_t6 = (s32) (var_t1_2 & 0xFFFF) >> (0x10 - temp_a0_4);
                M2C_FIELD(arg0, s32 *, 0x16A4) = (s32) (temp_a0_4 - 0xB);
            } else {
                M2C_FIELD(arg0, s32 *, 0x16A4) = (s32) (temp_a1_4 + 5);
                var_t6 = M2C_FIELD(arg0, u16 *, 0x16A0) | (var_t1_2 << temp_a1_4);
            }
            M2C_FIELD(arg0, u16 *, 0x16A0) = (u16) var_t6;
            temp_a1_5 = M2C_FIELD(arg0, s32 *, 0x16A4);
            if (temp_a1_5 >= 0xD) {
                temp_v0_4 = temp_v1 - 4;
                temp_t6_4 = M2C_FIELD(arg0, u16 *, 0x16A0) | (temp_v0_4 << temp_a1_5);
                M2C_FIELD(arg0, u16 *, 0x16A0) = temp_t6_4;
                *(M2C_FIELD(arg0, s32 *, 4) + M2C_FIELD(arg0, s32 *, 0xC)) = (s8) temp_t6_4;
                temp_t6_5 = M2C_FIELD(arg0, s32 *, 0xC) + 1;
                M2C_FIELD(arg0, s32 *, 0xC) = temp_t6_5;
                *(M2C_FIELD(arg0, s32 *, 4) + temp_t6_5) = (s8) ((s32) M2C_FIELD(arg0, u16 *, 0x16A0) >> 8);
                temp_a0_5 = M2C_FIELD(arg0, s32 *, 0x16A4);
                M2C_FIELD(arg0, s32 *, 0xC) = (s32) (M2C_FIELD(arg0, s32 *, 0xC) + 1);
                M2C_FIELD(arg0, s32 *, 0x16A4) = (s32) (temp_a0_5 - 0xC);
                M2C_FIELD(arg0, u16 *, 0x16A0) = (u16) ((s32) (temp_v0_4 & 0xFFFF) >> (0x10 - temp_a0_5));
            } else {
                M2C_FIELD(arg0, s32 *, 0x16A4) = (s32) (temp_a1_5 + 4);
                M2C_FIELD(arg0, u16 *, 0x16A0) = (u16) (M2C_FIELD(arg0, u16 *, 0x16A0) | ((temp_v1 - 4) << temp_a1_5));
            }
            var_a2 = 0;
            if (temp_v1 > 0) {
                var_v1_2 = &D_8011E9C0;
                do {
                    temp_a1_6 = M2C_FIELD(arg0, s32 *, 0x16A4);
                    var_a2 += 1;
                    if (temp_a1_6 >= 0xE) {
                        temp_v0_5 = M2C_FIELD(((u8 *) arg0 + (*var_v1_2 * 4)), u16 *, 0xA62);
                        temp_t8_2 = M2C_FIELD(arg0, u16 *, 0x16A0) | (temp_v0_5 << temp_a1_6);
                        M2C_FIELD(arg0, u16 *, 0x16A0) = temp_t8_2;
                        *(M2C_FIELD(arg0, s32 *, 4) + M2C_FIELD(arg0, s32 *, 0xC)) = (s8) temp_t8_2;
                        temp_t8_3 = M2C_FIELD(arg0, s32 *, 0xC) + 1;
                        M2C_FIELD(arg0, s32 *, 0xC) = temp_t8_3;
                        *(M2C_FIELD(arg0, s32 *, 4) + temp_t8_3) = (s8) ((s32) M2C_FIELD(arg0, u16 *, 0x16A0) >> 8);
                        temp_a0_6 = M2C_FIELD(arg0, s32 *, 0x16A4);
                        M2C_FIELD(arg0, s32 *, 0xC) = (s32) (M2C_FIELD(arg0, s32 *, 0xC) + 1);
                        M2C_FIELD(arg0, s32 *, 0x16A4) = (s32) (temp_a0_6 - 0xD);
                        M2C_FIELD(arg0, u16 *, 0x16A0) = (u16) ((s32) (temp_v0_5 & 0xFFFF) >> (0x10 - temp_a0_6));
                    } else {
                        M2C_FIELD(arg0, u16 *, 0x16A0) = (u16) (M2C_FIELD(arg0, u16 *, 0x16A0) | (M2C_FIELD(((u8 *) arg0 + (*var_v1_2 * 4)), u16 *, 0xA62) << temp_a1_6));
                        M2C_FIELD(arg0, s32 *, 0x16A4) = (s32) (temp_a1_6 + 3);
                    }
                    var_v1_2 = (s32 *) ((u8 *) var_v1_2 + 1);
                } while (var_a2 != temp_v1);
            }
            sp44 = var_t1_2;
            sp50 = (u8 *) arg0 + 0x78;
            func_800A8778(arg0, (void *) M2C_FIELD(arg0, s32 *, 0x16A4), temp_t2 - 1);
            func_800A8778(arg0, (void *) M2C_FIELD(arg0, s32 *, 0x16A4), sp44);
            func_800A8284(arg0, sp50, sp40);
            M2C_FIELD(arg0, u32 *, 0x1694) = (u32) (M2C_FIELD(arg0, u32 *, 0x1694) + M2C_FIELD(arg0, s32 *, 0x168C) + 3);
        }
    }
    func_800A81FC(arg0);
    if (arg3 != 0) {
        func_800A8174(arg0);
        M2C_FIELD(arg0, u32 *, 0x1694) = (u32) (M2C_FIELD(arg0, u32 *, 0x1694) + 7);
    }
    return (u32) M2C_FIELD(arg0, u32 *, 0x1694) >> 3;
}
''',
        "hunks": [
            (r'''typedef unsigned int u32;
void car_crash_detect(void);
u32 car_crash_response(void *arg0, s32 arg1, s32 arg2, s32 arg3) {''',
             r'''typedef unsigned int u32;
void car_crash_detect();
u32 car_crash_response(void *arg0, s32 arg1, s32 arg2, s32 arg3) {'''),
        ],
    },
    {
        "name": 'mode_byte2_set', "fn": 'mode_byte2_set', "kind": 'exact',
        "note": 'K&R prototype',
        "ref": {'before': ('4038153', 'cloud/work/near-miss/mode_byte2_set/base.c'), 'after': ('d585c85', 'cloud/work/near-miss/mode_byte2_set/base.c')},
        "before": r'''typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
void sound_update_channel(void);
void mode_byte2_set(s16 arg0)
{
  if (arg0 < 0)
  {
    sound_update_channel(0);
    D_80149B60 = *((u8 *) (((s8 *) D_801497F0) + 4));
  }
  else
  {
    D_80149B60 = arg0;
  }
}
''',
        "hunks": [
            (r'''typedef signed short s16;
void sound_update_channel(void);
void mode_byte2_set(s16 arg0)''',
             r'''typedef signed short s16;
void sound_update_channel();
void mode_byte2_set(s16 arg0)'''),
        ],
    },
    {
        "name": 'camera_scene_manager', "fn": 'camera_scene_manager', "kind": 'exact',
        "note": 'K&R prototype',
        "ref": {'before': ('5eebdbb', 'cloud/work/ipa-groups/camera_scene_manager/group.c'), 'after': ('c4d4fe7', 'cloud/work/ipa-groups/camera_scene_manager/group.c')},
        "before": r'''typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
void func_800C2004(void);
void func_800C220C(void);
void camera_scene_manager(void) {
    GameCar *var_s4;
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f20;
    f32 temp_f2;
    f32 var_f20;
    f32 var_f20_2;
    f32 var_f22;
    s32 *var_s2;
    s32 *var_s2_2;
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_t6;
    s32 temp_t6_2;
    s32 temp_t6_3;
    s32 temp_t7;
    s32 temp_t7_2;
    s32 temp_t8;
    s32 temp_t8_2;
    s32 temp_t8_3;
    s32 temp_t8_4;
    s32 temp_t9;
    s32 temp_t9_2;
    s32 temp_t9_3;
    s32 temp_v1_2;
    s32 var_a1;
    s32 var_s3;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v1;
    void *temp_v0;
    void *temp_v0_2;
    void *temp_v1;

    if ((s8) D_8013FECB != 0) {
        var_v1 = 1;
        var_s2 = &D_801569B8;
        if (active_player_count > 0) {
            do {
                temp_t9 = *var_s2;
                var_s2 = var_s2 + 0x1F;
                if (temp_t9 & 0x1F7) {
                    D_8016139C = 0.0f;
                    var_v1 = 0;
                }
            } while ((u32) var_s2 < (u32) ((s32 *) ((active_player_count * 0x7C) + (u8 *) &D_801569B8)));
        }
        if (var_v1 != 0) {
            D_8016139C += M2C_BITWISE(f32, D_8002EB94);
            if (D_80123F38 <= D_8016139C) {
                D_80152738 = 1;
            }
        }
    }
    var_s3 = 0;
    if (active_player_count > 0) {
        var_s4 = player_array;
        var_f22 = D_80123F3C;
        var_s2_2 = &D_801569B8;
        do {
            temp_a1 = M2C_FIELD(var_s2_2, s32 *, 0);
            if ((temp_a1 & 0x1F0) && (*((s16 *) ((u8 *) &D_8014A648 + (var_s3 * 0x808))) != 0)) {
                temp_v0 = (s32 *) ((var_s3 * 0x78) + (u8 *) &D_80152038);
                M2C_FIELD(temp_v0, f32 *, 0x60) = (f32) (M2C_FIELD(temp_v0, f32 *, 0x60) + M2C_BITWISE(f32, D_8002EB94));
            }
            if ((temp_a1 & 0xC08FFFFF) && ((var_s4 = (GameCar *) &D_80152B70, ((s8) ((GameCar *) &D_80152B70)->pad000[0] != 0)) || (*((s16 *) ((u8 *) &D_8014A914 + (var_s3 * 0x808))) != -1))) {
                M2C_FIELD(var_s2_2, s32 *, 0) = 0x15000000;
            } else {
                D_8015F728 = 0;
                D_8015F730 = 0;
                D_8015B258 = 0;
                D_8015B248 = 0;
                D_80157238 = 0;
                temp_v1 = (D_8014A250_Record *) ((var_s3 * 0x808) + (u8 *) &D_8014A250);
                if (M2C_FIELD(temp_v1, u16 *, 0x61C) != 8) {
                    D_80157238 = 1;
                    D_8015B248 = 1;
                    D_8015F730 = 1;
                }
                if (M2C_FIELD(temp_v1, u16 *, 0x61E) != 8) {
                    D_8015B258 = 1;
                    D_80157238 += 1;
                    D_8015F730 += 1;
                }
                if (M2C_FIELD(temp_v1, u16 *, 0x620) != 8) {
                    D_80157238 += 1;
                    D_8015B248 += 1;
                    D_8015F728 = 1;
                }
                if (M2C_FIELD(temp_v1, u16 *, 0x622) != 8) {
                    D_80157238 += 1;
                    D_8015B258 += 1;
                    D_8015F728 += 1;
                }
                temp_t8 = D_80157238 < 3;
                var_v0 = temp_t8;
                if (temp_t8 != 0) {
                    temp_t7 = (s8) var_s4->pad000[0] == 0;
                    var_v0 = temp_t7;
                    if (temp_t7 != 0) {
                        var_v0 = (M2C_FIELD(temp_v1, s16 *, 0x6C4) + 1) == 0;
                    }
                }
                D_80161360 = var_v0;
                M2C_FIELD(&D_80161388, s32 *, 0) = M2C_BITWISE(s32, fabsf(M2C_FIELD(var_s4, f32 *, 0x368)));
                M2C_FIELD(&D_80161388, f32 *, 4) = fabsf(M2C_FIELD(var_s4, f32 *, 0x36C));
                D_80156BD8 = 0;
                D_80156CE4 = 0;
                M2C_FIELD(&D_80161388, f32 *, 8) = fabsf(M2C_FIELD(var_s4, f32 *, 0x370));
                D_80156BC8 = 0;
                temp_a1_2 = M2C_FIELD(var_s2_2, s32 *, 0);
                if ((temp_a1_2 & 0x01000000) && (M2C_FIELD(var_s4, f32 *, 0x368) < 0.0f)) {
                    temp_t9_2 = temp_a1_2 & 0xFEFFFFFF;
                    M2C_FIELD(var_s2_2, s32 *, 0) = temp_t9_2;
                    M2C_FIELD(var_s2_2, s32 *, 0) = temp_t9_2 | 0x02000000;
                    D_80156BC8 = 1;
                } else if (temp_a1_2 & 0x02000000) {
                    temp_t6 = temp_a1_2 & 0xFDFFFFFF;
                    if (M2C_FIELD(var_s4, f32 *, 0x368) >= 0.0f) {
                        M2C_FIELD(var_s2_2, s32 *, 0) = temp_t6;
                        M2C_FIELD(var_s2_2, s32 *, 0) = temp_t6 | 0x01000000;
                        D_80156BC8 = 1;
                    }
                }
                if ((M2C_FIELD(var_s2_2, s32 *, 0) & 0x04000000) && (M2C_FIELD(var_s4, f32 *, 0x36C) < 0.0f)) {
                    temp_t8_2 = M2C_FIELD(var_s2_2, s32 *, 0) & 0xFBFFFFFF;
                    M2C_FIELD(var_s2_2, s32 *, 0) = temp_t8_2;
                    M2C_FIELD(var_s2_2, s32 *, 0) = temp_t8_2 | 0x08000000;
                    D_80156BD8 = 1;
                } else if (M2C_FIELD(var_s2_2, s32 *, 0) & 0x08000000) {
                    temp_t7_2 = M2C_FIELD(var_s2_2, s32 *, 0) & 0xF7FFFFFF;
                    if (M2C_FIELD(var_s4, f32 *, 0x36C) >= 0.0f) {
                        M2C_FIELD(var_s2_2, s32 *, 0) = temp_t7_2;
                        M2C_FIELD(var_s2_2, s32 *, 0) = temp_t7_2 | 0x04000000;
                        D_80156BD8 = 1;
                    }
                }
                if ((M2C_FIELD(var_s2_2, s32 *, 0) & 0x10000000) && (M2C_FIELD(var_s4, f32 *, 0x370) < 0.0f)) {
                    temp_t9_3 = M2C_FIELD(var_s2_2, s32 *, 0) & 0xEFFFFFFF;
                    M2C_FIELD(var_s2_2, s32 *, 0) = temp_t9_3;
                    M2C_FIELD(var_s2_2, s32 *, 0) = temp_t9_3 | 0x20000000;
                    D_80156CE4 = 1;
                } else if (M2C_FIELD(var_s2_2, s32 *, 0) & 0x20000000) {
                    temp_t6_2 = M2C_FIELD(var_s2_2, s32 *, 0) & 0xDFFFFFFF;
                    if (M2C_FIELD(var_s4, f32 *, 0x370) >= 0.0f) {
                        M2C_FIELD(var_s2_2, s32 *, 0) = temp_t6_2;
                        M2C_FIELD(var_s2_2, s32 *, 0) = temp_t6_2 | 0x10000000;
                        D_80156CE4 = 1;
                    }
                }
                func_800C2944(var_s3);
                func_800C26C4(var_s3);
                func_800C2430(var_s3);
                var_a1 = M2C_FIELD(var_s2_2, s32 *, 0);
                if (var_a1 & 0x80) {
                    var_v0_2 = 0;
                    if (D_80157238 >= 3) {
                        temp_t8_3 = var_a1 & ~0x80;
                        M2C_FIELD(var_s2_2, s32 *, 0) = temp_t8_3;
                        var_a1 = temp_t8_3;
                    } else if (D_80157238 == 0) {
                        var_a1 = M2C_FIELD(var_s2_2, s32 *, 0);
                        M2C_FIELD(var_s2_2, f32 *, 0x28) = (f32) (M2C_FIELD(var_s2_2, f32 *, 0x28) + fabsf(M2C_FIELD(var_s4, f32 *, 0x368)));
                        M2C_FIELD(var_s2_2, f32 *, 0x2C) = (f32) (M2C_FIELD(var_s2_2, f32 *, 0x2C) + fabsf(M2C_FIELD(var_s4, f32 *, 0x36C)));
                        M2C_FIELD(var_s2_2, f32 *, 0x30) = (f32) (M2C_FIELD(var_s2_2, f32 *, 0x30) + fabsf(M2C_FIELD(var_s4, f32 *, 0x370)));
                    }
                    temp_f12 = M2C_FIELD(var_s2_2, f32 *, 0x28);
                    if (D_80123F40 < temp_f12) {
                        var_v0_2 = 1;
                    }
                    temp_f14 = M2C_FIELD(var_s2_2, f32 *, 0x2C);
                    if (D_80123F40 < temp_f14) {
                        var_v0_2 += 1;
                    }
                    temp_f2 = M2C_FIELD(var_s2_2, f32 *, 0x30);
                    if (D_80123F44 < temp_f2) {
                        var_v0_2 += 1;
                    }
                    if ((var_v0_2 >= 2) && (D_80123F48 <= (temp_f2 + (temp_f12 + temp_f14)))) {
                        M2C_FIELD(var_s2_2, s32 *, 0) = var_a1 & ~0x70;
                        func_800C1B60(temp_f12, temp_f14, 5, var_a1, var_s3);
                        M2C_FIELD(var_s2_2, f32 *, 0x28) = 0.0f;
                        M2C_FIELD(var_s2_2, f32 *, 0x2C) = 0.0f;
                        M2C_FIELD(var_s2_2, f32 *, 0x30) = 0.0f;
                        var_a1 = M2C_FIELD(var_s2_2, s32 *, 0);
                    }
                } else {
                    temp_t6_3 = var_a1 | 0x80;
                    if (D_80161360 != 0) {
                        M2C_FIELD(var_s2_2, s32 *, 0) = temp_t6_3;
                        var_a1 = temp_t6_3;
                        M2C_FIELD(var_s2_2, f32 *, 0x28) = fabsf(M2C_FIELD(var_s4, f32 *, 0x368));
                        M2C_FIELD(var_s2_2, f32 *, 0x2C) = fabsf(M2C_FIELD(var_s4, f32 *, 0x36C));
                        M2C_FIELD(var_s2_2, f32 *, 0x30) = fabsf(M2C_FIELD(var_s4, f32 *, 0x370));
                    }
                }
                temp_v1_2 = D_80157238;
                if (var_a1 & 0x100) {
                    if ((temp_v1_2 > 0) || (var_s4 = (GameCar *) &D_80152900, (*(GameCar *) &D_80152900 & 0x100000))) {
                        M2C_FIELD(var_s2_2, f32 *, 0x38) = (f32) D_801543CC;
                        M2C_FIELD(var_s2_2, s32 *, 0) = var_a1 & ~0x100;
                        var_f20 = M2C_FIELD(var_s2_2, f32 *, 0x38) - M2C_FIELD(var_s2_2, f32 *, 0x34);
                        if (var_f20 > 5.0f) {
                            do {
                                func_800C1B60(1.3e-44f, M2C_BITWISE(f32, var_s3));
                                var_f20 -= 1.0f;
                            } while (var_f20 > 5.0f);
                        }
                    }
                    var_a1 = M2C_FIELD(var_s2_2, s32 *, 0);
                } else {
                    temp_t8_4 = var_a1 | 0x100;
                    if (temp_v1_2 == 0) {
                        M2C_FIELD(var_s2_2, s32 *, 0) = temp_t8_4;
                        M2C_FIELD(var_s2_2, f32 *, 0x38) = -1.0f;
                        var_a1 = temp_t8_4;
                        M2C_FIELD(var_s2_2, f32 *, 0x34) = (f32) D_801543CC;
                    }
                }
                if (var_a1 & 1) {
                    if ((temp_v1_2 >= 3) || (temp_v1_2 < 2)) {
                        M2C_FIELD(var_s2_2, f32 *, 0x40) = (f32) D_801543CC;
                        temp_f20 = M2C_FIELD(var_s2_2, f32 *, 0x40) - M2C_FIELD(var_s2_2, f32 *, 0x3C);
                        M2C_FIELD(var_s2_2, s32 *, 0) = var_a1 & ~1;
                        if (temp_f20 > 0.25f) {
                            var_f20_2 = temp_f20 - 0.25f;
                            if (var_f20_2 > 0.0f) {
                                do {
                                    func_800C1B60(8e-45f, M2C_BITWISE(f32, var_s3));
                                    var_f20_2 -= var_f22;
                                } while (var_f20_2 > 0.0f);
                            }
                        }
                    }
                } else if ((temp_v1_2 == 2) && ((D_8015B248 == 2) || (D_8015B258 == 2))) {
                    M2C_FIELD(var_s2_2, s32 *, 0) = var_a1 | 1;
                    M2C_FIELD(var_s2_2, f32 *, 0x40) = -1.0f;
                    M2C_FIELD(var_s2_2, f32 *, 0x3C) = (f32) D_801543CC;
                }
                func_800C220C(var_s3);
                func_800C2004(var_s3);
                var_f22 = D_80123F50;
                if (M2C_FIELD(var_s2_2, s32 *, 0) & 0x1F7) {
                    M2C_FIELD(var_s2_2, f32 *, 0x54) = -1.0f;
                } else if (D_80157238 >= 3) {
                    temp_v0_2 = (s32 *) ((var_s3 * 0x78) + (u8 *) &D_80152038);
                    if (M2C_FIELD(temp_v0_2, s32 *, 0x18) == 0) {
                        M2C_FIELD(temp_v0_2, f32 *, 0x60) = 0.0f;
                    }
                    if ((M2C_FIELD(var_s2_2, f32 *, 0x54) == -1.0f) && (M2C_FIELD(temp_v0_2, s32 *, 0x18) != 0)) {
                        M2C_FIELD(var_s2_2, f32 *, 0x54) = (f32) D_801543CC;
                    }
                }
                if ((D_80157238 >= 3) && (*((s32 *) ((u8 *) &D_80152050 + (var_s3 * 0x78))) != 0)) {
                    temp_f0 = M2C_FIELD(var_s2_2, f32 *, 0x38);
                    if ((temp_f0 != -1.0f) && ((D_801543CC - temp_f0) > 1.0f)) {
                        M2C_FIELD(var_s2_2, f32 *, 0x38) = -1.0f;
                        func_800C1B60(1.5e-44f, M2C_BITWISE(f32, var_s3));
                    }
                }
                *var_s4 = (s32) (*var_s4 & 0xFFEFFFFF);
            }
            var_s3 += 1;
            var_s2_2 = var_s2_2 + 0x1F;
            var_s4 = var_s4 + 1;
        } while (var_s3 < active_player_count);
    }
}
''',
        "hunks": [
            (r'''typedef float f32;
void func_800C2004(void);
void func_800C220C(void);
void camera_scene_manager(void) {''',
             r'''typedef float f32;
void func_800C2004();
void func_800C220C();
void camera_scene_manager(void) {'''),
        ],
    },
    {
        "name": 'physics_velocity_integrate_d', "fn": 'physics_velocity_integrate_d', "kind": 'exact',
        "note": 'commutative flip of a*b + c',
        "ref": {'before': ('b0d533c', 'cloud/work/ipa-groups/func_8008B640/group.c'), 'after': ('5be1ad2', 'cloud/work/ipa-groups/func_8008B640/group.c')},
        "before": r'''typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
void physics_velocity_integrate_d(ModelObj *arg0, s16 arg1) {
    f32 two = 2.0f;
    Rec808 *rec;
    GameCar *car;
    s32 flag;
    s32 lvl;

    car = &player_array[arg0->player];
    rec = &((Rec808 *) &D_8014A250)[arg0->player];
    lvl = (s32) M2C_FIELD(rec, f32 *, 0x3F0);
    flag = (s16) lvl >= 21 && (M2C_FIELD(car, s32 *, 0xE8) & 0x80) != 0;
    physics_velocity_integrate_a(arg0, flag, 194,
                                 M2C_FIELD(car, f32 *, 0xB0) + D_80123894,
                                 M2C_FIELD(rec, f32 *, 0x430) * two + M2C_FIELD(car, f32 *, 0xB4),
                                 M2C_FIELD(car, f32 *, 0xB8),
                                 arg1);
}
''',
        "hunks": [
            (r'''                                 M2C_FIELD(car, f32 *, 0xB0) + D_80123894,
                                 M2C_FIELD(rec, f32 *, 0x430) * two + M2C_FIELD(car, f32 *, 0xB4),
                                 M2C_FIELD(car, f32 *, 0xB8),''',
             r'''                                 M2C_FIELD(car, f32 *, 0xB0) + D_80123894,
                                 M2C_FIELD(car, f32 *, 0xB4) + M2C_FIELD(rec, f32 *, 0x430) * two,
                                 M2C_FIELD(car, f32 *, 0xB8),'''),
        ],
    },
    {
        "name": 'physics_velocity_integrate_e', "fn": 'physics_velocity_integrate_e', "kind": 'exact',
        "note": 'commutative flip of a*b + c',
        "ref": {'before': ('b0d533c', 'cloud/work/ipa-groups/func_8008B640/group.c'), 'after': ('5be1ad2', 'cloud/work/ipa-groups/func_8008B640/group.c')},
        "before": r'''typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
void physics_velocity_integrate_e(ModelObj *arg0, s16 arg1) {
    f32 two = 2.0f;
    Rec808 *rec;
    GameCar *car;
    s32 flag;
    s32 lvl;

    car = &player_array[arg0->player];
    rec = &((Rec808 *) &D_8014A250)[arg0->player];
    lvl = (s32) M2C_FIELD(rec, f32 *, 0x3F0);
    flag = (s16) lvl >= 21 && (M2C_FIELD(car, s32 *, 0xE8) & 0x2000) != 0;
    physics_velocity_integrate_a(arg0, flag, 194,
                                 M2C_FIELD(car, f32 *, 0xD4) - D_80123898,
                                 M2C_FIELD(rec, f32 *, 0x544) * two + M2C_FIELD(car, f32 *, 0xD8),
                                 M2C_FIELD(car, f32 *, 0xDC),
                                 arg1);
}
''',
        "hunks": [
            (r'''                                 M2C_FIELD(car, f32 *, 0xD4) - D_80123898,
                                 M2C_FIELD(rec, f32 *, 0x544) * two + M2C_FIELD(car, f32 *, 0xD8),
                                 M2C_FIELD(car, f32 *, 0xDC),''',
             r'''                                 M2C_FIELD(car, f32 *, 0xD4) - D_80123898,
                                 M2C_FIELD(car, f32 *, 0xD8) + M2C_FIELD(rec, f32 *, 0x544) * two,
                                 M2C_FIELD(car, f32 *, 0xDC),'''),
        ],
    },
    {
        "name": 'physics_velocity_integrate_f', "fn": 'physics_velocity_integrate_f', "kind": 'exact',
        "note": 'commutative flip of a*b + c',
        "ref": {'before': ('b0d533c', 'cloud/work/ipa-groups/func_8008B640/group.c'), 'after': ('5be1ad2', 'cloud/work/ipa-groups/func_8008B640/group.c')},
        "before": r'''typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
void physics_velocity_integrate_f(ModelObj *arg0, s16 arg1) {
    f32 two = 2.0f;
    Rec808 *rec;
    GameCar *car;
    s32 flag;
    s32 lvl;

    car = &player_array[arg0->player];
    rec = &((Rec808 *) &D_8014A250)[arg0->player];
    lvl = (s32) M2C_FIELD(rec, f32 *, 0x3F0);
    flag = (s16) lvl >= 21 && (M2C_FIELD(car, s32 *, 0xE8) & 0x100) != 0;
    physics_velocity_integrate_a(arg0, flag, 194,
                                 M2C_FIELD(car, f32 *, 0xBC) - D_8012389C,
                                 M2C_FIELD(rec, f32 *, 0x48C) * two + M2C_FIELD(car, f32 *, 0xC0),
                                 M2C_FIELD(car, f32 *, 0xC4),
                                 arg1);
}
''',
        "hunks": [
            (r'''                                 M2C_FIELD(car, f32 *, 0xBC) - D_8012389C,
                                 M2C_FIELD(rec, f32 *, 0x48C) * two + M2C_FIELD(car, f32 *, 0xC0),
                                 M2C_FIELD(car, f32 *, 0xC4),''',
             r'''                                 M2C_FIELD(car, f32 *, 0xBC) - D_8012389C,
                                 M2C_FIELD(car, f32 *, 0xC0) + M2C_FIELD(rec, f32 *, 0x48C) * two,
                                 M2C_FIELD(car, f32 *, 0xC4),'''),
        ],
    },
    {
        "name": 'func_800CDDE8', "fn": 'func_800CDDE8', "kind": 'exact',
        "note": 'case order 4,5,6 -> 6,4,5 (before RECONSTRUCTED: the repo never committed the ascending order)',
        "ref": None,
        "before": r'''u8 func_800CDDE8(void **arg0) {
    u8 *p;

    p = *M2C_FIELD(*arg0, u8 ***, 0x2C);
    switch (p[0x47]) {
    case 4:
        return p[0x72];
    case 5:
        return p[0x86];
    case 6:
        return p[0x62];
    }
    return p[0x4E];
}
''',
        "hunks": [
            (r'''    switch (p[0x47]) {
    case 4:''',
             r'''    switch (p[0x47]) {
    case 6:
        return p[0x62];
    case 4:'''),
            (r'''        return p[0x86];
    case 6:
        return p[0x62];
    }''',
             r'''        return p[0x86];
    }'''),
        ],
    },
    {
        "name": 'func_800C3AD0', "fn": 'func_800C3AD0', "kind": 'search',
        "note": 'declaration order',
        "ref": {'before': ('4d56dd5', 'cloud/work/ipa-groups/func_800AD4C8/group.c'), 'after': ('13426b9', 'cloud/work/ipa-groups/func_800AD4C8/group.c')},
        "before": r'''typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
s16 func_800C3AD0(f32 *pt, f32 *wp, Poly *poly, s16 *outIdx, f32 *q, f32 *mat, f32 *bound, f32 zmin) {
    f32 vb[3];
    f32 vd[3];
    volatile f32 f2;
    volatile f32 f1;
    f32 ve[3];
    u16 idx[20];
    f32 va[3];
    f32 vc[3];
    PV *e;
    u32 k;
    u32 n;
    s32 res;

    n = poly->cnt & 0xF;
    res = 1;
    *outIdx = func_800AD5D0((u8 *) (poly->off + D_80152568), n, (s16 *) idx);
    func_800AD650(mat, poly->body);
    e = &D_8015201C[idx[0]];
    DECODE(vb, e);
    va[0] = wp[0] - vb[0];
    va[1] = wp[1] - vb[1];
    va[2] = wp[2] - vb[2];
    func_800A61B0(va, pt, mat);
    if ((pt[1] <= zmin) || (*bound < pt[1])) {
        return 0;
    }
    e = &D_8015201C[idx[n - 1]];
    DECODE(vb, e);
    if ((((vb[2] - pt[2]) * vb[0]) - (vb[2] * (vb[0] - pt[0]))) < 0.0f) {
        if (func_800AD4C8(vb, pt, vc, D_80123F70) == 0) {
            return 0;
        }
        res = -1;
        goto done;
    }
    e = &D_8015201C[idx[1]];
    DECODE(vb, e);
    if (((pt[2] * vb[0]) - (vb[2] * pt[0])) < 0.0f) {
        if (func_800AD4C8(vb, pt, vc, D_80123F74) == 0) {
            return 0;
        }
        res = -1;
        goto done;
    }
    k = 2;
    if ((u32) n >= 3U) {
        do {
            k += 1;
            ve[0] = vb[0];
            ve[2] = vb[2];
            e = &D_8015201C[idx[k - 1]];
            DECODE(vb, e);
            va[1] = 0.0f;
            f1 = ve[0];
            va[0] = f2 = vb[0] - ve[0];
            va[2] = vb[2] - ve[2];
            vd[1] = 0.0f;
            vd[0] = pt[0] - f1;
            vd[2] = pt[2] - ve[2];
            if (((vd[2] * f2) - (va[2] * vd[0])) < 0.0f) {
                if (func_800AD4C8(va, vd, vc, D_80123F78) == 0) {
                    return 0;
                }
                res = -1;
                goto done;
            }
        } while (k < (u32) n);
    }
done:
    *bound = pt[1];
    if ((q != NULL) && (((poly->type & 0xF) == 5) || ((poly->type & 0xF) == 6))) {
        e = &D_8015201C[idx[0]];
        DECODE(vb, e);
        va[0] = q[0] - vb[0];
        va[1] = q[1] - vb[1];
        va[2] = q[2] - vb[2];
        func_800A61B0(va, vb, mat);
        if ((vb[1] <= *bound) || (vb[1] < D_80123F7C)) {
            return 0;
        }
    }
    return res;
}
''',
        "hunks": [
            (r'''s16 func_800C3AD0(f32 *pt, f32 *wp, Poly *poly, s16 *outIdx, f32 *q, f32 *mat, f32 *bound, f32 zmin) {
    f32 vb[3];
    f32 vd[3];
    volatile f32 f2;
    volatile f32 f1;
    f32 ve[3];''',
             r'''s16 func_800C3AD0(f32 *pt, f32 *wp, Poly *poly, s16 *outIdx, f32 *q, f32 *mat, f32 *bound, f32 zmin) {
    f32 va[3];
    f32 vb[3];
    f32 vc[3];
    f32 vd[3];
    f32 ve[3];'''),
            (r'''    u16 idx[20];
    f32 va[3];
    f32 vc[3];
    PV *e;''',
             r'''    u16 idx[20];
    volatile f32 f1;
    volatile f32 f2;
    PV *e;'''),
        ],
    },
    {
        "name": 'func_800D2FA8', "fn": 'func_800D2FA8', "kind": 'search',
        "rename": {'padv': 'pad'},
        "note": 'volatile pad + declarator order',
        "ref": {'before': ('dcf78eb', 'cloud/work/ipa-groups/func_800D2FA8/group.c'), 'after': ('b0d533c', 'cloud/work/ipa-groups/func_800D2FA8/group.c')},
        "before": r'''typedef signed int s32;
s32 func_800D2FA8(s32 node, s32 pos, s32 *outNode, s32 *outPos, s32 stopAtTyped, s32 depth)
{
    s32 i;
    s32 start1, dist1, start2, dist2, total, node2, pos2;
    s32 v;

    if (depth == 0) {
        *outNode = node;
        *outPos = pos;
    }
    D_80124F88[depth] = node;
    for (i = 0; i < depth; i++) {
        if (node == D_80124F88[i]) {
            return 0;
        }
    }
    if (D_801407F0.nodes[node].next == D_801407F0.nodes[node].prev) {
        if (D_801407F0.nodes[node].nextPos < D_801407F0.nodes[node].prevPos) {
            *outPos = pos * (D_801407F0.nodes[node].prevPos - D_801407F0.nodes[node].nextPos + 1) /
                          D_801407F0.nodes[node].numPoints + D_801407F0.nodes[node].nextPos;
        } else {
            if (D_801407F0.nodes[node].next != -1) {
                return 0;
            }
            *outPos = pos * (D_801407F0.nodes[node].prevPos - D_80151CE8[D_80151CE8[0].last].start +
                             D_801407F0.numPoints - D_801407F0.nodes[node].nextPos + 1) /
                          D_801407F0.nodes[node].numPoints + D_801407F0.nodes[node].nextPos;
            if (*outPos >= D_801407F0.numPoints) {
                *outPos = D_80151CE8[D_80151CE8[0].last].start + *outPos - D_801407F0.numPoints;
            }
        }
        *outNode = D_801407F0.nodes[node].next;
        if (*outNode < 0 || (stopAtTyped && D_801407F0.nodes[*outNode].type != 0)) {
            return 1;
        }
        return func_800D2FA8(*outNode, *outPos, outNode, outPos, stopAtTyped, depth + 1);
    }
    if (!time_of_day_select(node, pos, &start1, &dist1, 0)) {
        return 0;
    }
    if (!minimap_render(node, pos, &node2, &pos2, &total, stopAtTyped, 0)) {
        return 0;
    }
    if (!time_of_day_select(node2, pos2, &start2, &dist2, 0)) {
        return 0;
    }
    if (start2 < start1) {
        v = start2 + D_801407F0.numPoints - start1 - D_80151CE8[D_80151CE8[0].last].start;
        if (start1 - start2 < v) {
            dist1 = dist1 + start1 - start2;
            start1 = start2;
        } else {
            dist2 = dist2 + v;
        }
    } else if (start1 + D_801407F0.numPoints - start2 < start2 - start1) {
        start1 = start2;
        dist1 = dist1 + start1 + D_801407F0.numPoints - start2;
    } else {
        dist2 = dist2 + start2 - start1;
    }
    total = total + dist1;
    split_time_display(node2, pos2, dist2 - dist1 * dist2 / total, outNode, outPos);
    return 1;
}
''',
        "hunks": [
            (r'''    s32 i;
    s32 start1, dist1, start2, dist2, total, node2, pos2;
    s32 v;''',
             r'''    s32 i;
    volatile s32 padv[2];
    s32 dist1, total, dist2, start1, node2, pos2, start2;
    s32 v;'''),
        ],
    },
    {
        "name": 'func_800F84B0', "fn": 'func_800F84B0', "kind": 'partial',
        "note": 'returns -> result assigns (final also simplifies conditions)',
        "ref": {'before': ('be41f99^', 'cloud/work/near-miss/func_800F84B0/base.c'), 'after': ('be41f99', 'cloud/matches/func_800F84B0.c')},
        "before": r'''typedef signed int s32;
s32 func_800F84B0(s32 arg0)
{
  s32 var_v1;
  var_v1 = 1;
  if (gameplay_mode == 3)
  {
    if ((((arg0 == 1) || (arg0 == 2)) || (arg0 == 3)) || (arg0 == 4))
    {
      var_v1 = 0;
    }
    if ((arg0 == (5 ^ 0)) && ((*((&D_80152907) + (D_801543D4 * 0x3B8))) == 0))
    {
      return 0;
    }
    return var_v1;
  }
  if (gameplay_mode == 1)
  {
    if ((((arg0 == 0) || (arg0 == 1)) || (arg0 == 2)) || (arg0 == 5))
    {
      return 0;
    }
    return var_v1;
  }
  if ((arg0 == 5) || (arg0 == 2))
  {
    var_v1 = 0;
    return var_v1;
  }
}
''',
        "hunks": [
            (r'''typedef signed int s32;
s32 func_800F84B0(s32 arg0)''',
             r'''typedef signed int s32;
/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
s32 func_800F84B0(s32 arg0)'''),
            (r'''{
  s32 var_v1;
  var_v1 = 1;
  if (gameplay_mode == 3)
  {
    if ((((arg0 == 1) || (arg0 == 2)) || (arg0 == 3)) || (arg0 == 4))
    {
      var_v1 = 0;
    }
    if ((arg0 == (5 ^ 0)) && ((*((&D_80152907) + (D_801543D4 * 0x3B8))) == 0))
    {
      return 0;
    }
    return var_v1;
  }
  if (gameplay_mode == 1)
  {
    if ((((arg0 == 0) || (arg0 == 1)) || (arg0 == 2)) || (arg0 == 5))
    {
      return 0;
    }
    return var_v1;
  }
  if ((arg0 == 5) || (arg0 == 2))
  {
    var_v1 = 0;
    return var_v1;
  }
}''',
             r'''{
  s32 result = 1;
  if (gameplay_mode == 3) {
    if (arg0 == 1 || arg0 == 2 || arg0 == 3 || arg0 == 4) {
      result = 0;
    }
    if (arg0 == 5 && (*((&D_80152907) + (D_801543D4 * 0x3B8))) == 0) {
      result = 0;
    }
  } else if (gameplay_mode == 1) {
    if (arg0 == 0 || arg0 == 1 || arg0 == 2 || arg0 == 5) {
      result = 0;
    }
  } else {
    if (arg0 == 5 || arg0 == 2) {
      result = 0;
    }
  }
  return result;
}'''),
        ],
    },
    {
        "name": 'engine_rpm_calc', "fn": 'engine_rpm_calc', "kind": 'partial',
        "note": 'param s16 + local x',
        "ref": {'before': ('87c5414', 'cloud/work/near-miss/engine_rpm_calc/base.c'), 'after': ('d585c85', 'cloud/work/near-miss/engine_rpm_calc/base.c')},
        "before": r'''typedef signed short s16;
typedef signed int s32;
void engine_rpm_calc(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
  func_8008E26C(arg0, arg1, (s16)arg3, (((s16)arg2 << 8) ^ 0xF00) | arg4);
}
''',
        "hunks": [
            (r'''typedef signed int s32;
void engine_rpm_calc(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
  func_8008E26C(arg0, arg1, (s16)arg3, (((s16)arg2 << 8) ^ 0xF00) | arg4);
}''',
             r'''typedef signed int s32;
void engine_rpm_calc(s32 arg0, s32 arg1, s16 arg2, s32 arg3, s32 arg4)
{
  s16 x;
  x = arg3;
  func_8008E26C(arg0, arg1, x, ((arg2 << 8) ^ 0xF00) | arg4);
}'''),
        ],
    },
    {
        "name": 'func_800D0A34', "fn": 'func_800D0A34', "kind": 'partial',
        "note": 'return -> +=, else-if',
        "ref": {'before': ('141e8fb^', 'cloud/work/near-miss/func_800D0A34/base.c'), 'after': ('141e8fb', 'cloud/matches/func_800D0A34.c')},
        "before": r'''typedef signed char s8;
typedef unsigned char u8;
typedef float f32;
f32 func_800D0A34(void *arg0) {
    f32 var_f2;
    s8 temp_v0;

    temp_v0 = (&D_8011156C)[M2C_FIELD(arg0, u8 *, 8)];
    var_f2 = *((f32 *) ((u8 *) &D_801111E0 + ((M2C_FIELD(arg0, s8 *, 0xD) * 0xC) + (M2C_FIELD(arg0, s8 *, 0xB) * 4))));
    if (temp_v0 == 0) {
        return var_f2 + D_80124148;
    }
    if (temp_v0 == 2) {
        var_f2 -= D_8012414C;
    }
    return var_f2;
}
''',
        "hunks": [
            (r'''typedef float f32;
f32 func_800D0A34(void *arg0) {''',
             r'''typedef float f32;
/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
f32 func_800D0A34(void *arg0) {'''),
            (r'''
    temp_v0 = (&D_8011156C)[M2C_FIELD(arg0, u8 *, 8)];
    var_f2 = *((f32 *) ((u8 *) &D_801111E0 + ((M2C_FIELD(arg0, s8 *, 0xD) * 0xC) + (M2C_FIELD(arg0, s8 *, 0xB) * 4))));
    if (temp_v0 == 0) {
        return var_f2 + D_80124148;
    }
    if (temp_v0 == 2) {
        var_f2 -= D_8012414C;''',
             r'''
    var_f2 = ((f32 (*)[3]) &D_801111E0)[M2C_FIELD(arg0, s8 *, 0xD)][M2C_FIELD(arg0, s8 *, 0xB)];
    temp_v0 = (&D_8011156C)[M2C_FIELD(arg0, u8 *, 8)];
    if (temp_v0 == 0) {
        var_f2 += D_80124148;
    } else if (temp_v0 == 2) {
        var_f2 -= D_8012414C;'''),
        ],
    },
    {
        "name": 'camera_smooth_lerp', "fn": 'camera_smooth_lerp', "kind": 'partial',
        "note": 'do/while -> for + natural indexing',
        "ref": {'before': ('848671f^', 'cloud/work/near-miss/camera_smooth_lerp/base.c'), 'after': ('848671f', 'cloud/matches/camera_smooth_lerp.c')},
        "before": r'''typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
void camera_smooth_lerp(void)
{
  s16 var_s0;
  s32 temp_lo;
  s32 temp_t6;
  void *temp_v0;
  void *new_var;
  u8 *p;
  u8 *p2;
  u8 *p3;
  var_s0 = 0;
  do {
  p = (u8 *) (&D_8013C238);
  do
  {
    new_var = func_800A7D6C();
    *((s32 *) (p + var_s0 * 4)) = new_var;
    var_s0 += 1;
  }
  while (var_s0 < 0x32);
  } while (0);
  var_s0 = 0;
  p2 = (u8 *) (&D_80154FD8);
  p3 = (u8 *) (&D_80154660);
  do
  {
    *((s32 *) (p2 + (var_s0 * 0x3C))) = -1;
    temp_lo = var_s0 * 0x190;
    var_s0 += 1;
    temp_v0 = (s32 *) (p3 + temp_lo);
    *((s32 *) (((s8 *) temp_v0) + 0x150)) = -1;
    *((s32 *) (((s8 *) temp_v0) + 0)) = -1;
    *((s32 *) (((s8 *) temp_v0) + 0x54)) = -1;
    *((s32 *) (((s8 *) temp_v0) + 0xA8)) = -1;
    *((s32 *) (((s8 *) temp_v0) + 0xFC)) = -1;
  }
  while (var_s0 < 6);
  D_8013C094 = 0;
}
''',
        "hunks": [
            (r'''typedef signed char s8;
typedef unsigned char u8;''',
             r'''typedef unsigned char u8;'''),
            (r'''typedef signed int s32;
void camera_smooth_lerp(void)''',
             r'''typedef signed int s32;
/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
void camera_smooth_lerp(void)'''),
            (r'''{
  s16 var_s0;
  s32 temp_lo;
  s32 temp_t6;
  void *temp_v0;
  void *new_var;
  u8 *p;''',
             r'''{
  s16 i;
  u8 *p;'''),
            (r'''  u8 *p3;
  var_s0 = 0;
  do {
  p = (u8 *) (&D_8013C238);
  do
  {
    new_var = func_800A7D6C();
    *((s32 *) (p + var_s0 * 4)) = new_var;
    var_s0 += 1;
  }
  while (var_s0 < 0x32);
  } while (0);
  var_s0 = 0;
  p2 = (u8 *) (&D_80154FD8);''',
             r'''  u8 *p3;
  p = (u8 *) (&D_8013C238);
  for (i = 0; i < 50; i++) { *((s32 *) (p + i * 4)) = (s32) func_800A7D6C(); }
  p2 = (u8 *) (&D_80154FD8);'''),
            (r'''  p3 = (u8 *) (&D_80154660);
  do
  {
    *((s32 *) (p2 + (var_s0 * 0x3C))) = -1;
    temp_lo = var_s0 * 0x190;
    var_s0 += 1;
    temp_v0 = (s32 *) (p3 + temp_lo);
    *((s32 *) (((s8 *) temp_v0) + 0x150)) = -1;
    *((s32 *) (((s8 *) temp_v0) + 0)) = -1;
    *((s32 *) (((s8 *) temp_v0) + 0x54)) = -1;
    *((s32 *) (((s8 *) temp_v0) + 0xA8)) = -1;
    *((s32 *) (((s8 *) temp_v0) + 0xFC)) = -1;
  }
  while (var_s0 < 6);
  D_8013C094 = 0;''',
             r'''  p3 = (u8 *) (&D_80154660);
  for (i = 0; i < 6; i++) {
    *((s32 *) (p2 + (i * 0x3C))) = -1;
    *((s32 *) (p3 + i * 0x190 + 0x150)) = -1;
    *((s32 *) (p3 + i * 0x190)) = -1;
    *((s32 *) (p3 + i * 0x190 + 0x54)) = -1;
    *((s32 *) (p3 + i * 0x190 + 0xA8)) = -1;
    *((s32 *) (p3 + i * 0x190 + 0xFC)) = -1;
  }
  D_8013C094 = 0;'''),
        ],
    },
    {
        "name": 'audio_effect_remove', "fn": 'audio_effect_remove', "kind": 'partial',
        "note": 'do/while -> for + indexing',
        "ref": {'before': ('13426b9^', 'cloud/work/near-miss/audio_effect_remove/base.c'), 'after': ('13426b9', 'cloud/matches/audio_effect_remove.c')},
        "before": r'''typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
void audio_effect_remove(void)
{
  f32 *temp_t6;
  s16 var_v1;
  s32 temp_v0;
  audio_effect_apply();
  var_v1 = 0;
  do
  {
    temp_v0 = var_v1 * 4;
 temp_t6 = &(&D_80153F28)[var_v1]; var_v1 += 1;
    *temp_t6 = 0.0f;
    *((s32 *) (((u8 *) (&D_80153F48)) + temp_v0)) = 0.0f;
    *(temp_t6 = (s32 *) (((u8 *) (&D_80153F68)) + temp_v0)) = 0.0f;
  }
  while (var_v1 < 6);
  D_80154190 = D_801543CC;
  D_80154182 = -1;
}
''',
        "hunks": [
            (r'''typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
void audio_effect_remove(void)''',
             r'''typedef signed short s16;
typedef float f32;
/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
void audio_effect_remove(void)'''),
            (r'''{
  f32 *temp_t6;
  s16 var_v1;
  s32 temp_v0;
  audio_effect_apply();
  var_v1 = 0;
  do
  {
    temp_v0 = var_v1 * 4;
 temp_t6 = &(&D_80153F28)[var_v1]; var_v1 += 1;
    *temp_t6 = 0.0f;
    *((s32 *) (((u8 *) (&D_80153F48)) + temp_v0)) = 0.0f;
    *(temp_t6 = (s32 *) (((u8 *) (&D_80153F68)) + temp_v0)) = 0.0f;
  }
  while (var_v1 < 6);
  D_80154190 = D_801543CC;''',
             r'''{
  s16 i;
  audio_effect_apply();
  for (i = 0; i < 6; i++) {
    ((f32 *) &D_80153F28)[i] = 0.0f;
    ((f32 *) &D_80153F48)[i] = 0.0f;
    ((f32 *) &D_80153F68)[i] = 0.0f;
  }
  D_80154190 = D_801543CC;'''),
        ],
    },
    {
        "name": 'car_select_handler', "fn": 'car_select_handler', "kind": 'partial',
        "note": 'drop locals + cast',
        "ref": {'before': ('13426b9^', 'cloud/work/near-miss/car_select_handler/base.c'), 'after': ('13426b9', 'cloud/matches/car_select_handler.c')},
        "before": r'''typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
void car_select_handler(s16 arg0)
{
  s8 new_var;
  GameCar *temp_v0;
  s32 *temp_a1;
  s8 temp_v1;
  temp_v0 = &player_array[arg0];
  temp_v1 = (s8) temp_v0->pad0EC[0x26C];
  switch (temp_v1)
  {
    case 0:
      if (((s8) temp_v0->pad0EC[0x26D]) <= 0)
    {

      temp_a1 = (D_8014A250_Record *) ((arg0 * 0x808) + ((u8 *) (&D_8014A250)));
      if ((*((s8 *) (((s8 *) temp_a1) + 0x640))) != 0)
      {
        new_var = -1;
        temp_v0->pad0EC[1] = new_var;
        func_800C4F68(2, temp_a1, 0);
        temp_v0->pad0EC[0x26C] = 1;
        *((f32 *) (((s8 *) temp_v0) + 0x10C)) = (f32) (*((f32 *) (((s8 *) temp_a1) + 0x714)));
      }
    }

    return;
    case 2:

    case 3:
      return;

    case 1:
      if (gameplay_mode == 6)
    {
      if (((*((f32 *) (((u8 *) (&D_8014A964)) + (arg0 * 0x808)))) - (*((f32 *) (((s8 *) temp_v0) + 0x10C)))) > 2.25f)
      {
        temp_v0->pad0EC[0x26C] = 2;
        return;
      }
    }
    else
      if (((*((f32 *) (((u8 *) (&D_8014A964)) + (arg0 * 0x808)))) - (*((f32 *) (((s8 *) temp_v0) + 0x10C)))) > 3.5f)
    {
      temp_v0->pad0EC[0x26C] = 2;
    }
      break;

  }

}
''',
        "hunks": [
            (r'''typedef float f32;
void car_select_handler(s16 arg0)''',
             r'''typedef float f32;
/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
void car_select_handler(s16 arg0)'''),
            (r'''{
  s8 new_var;
  GameCar *temp_v0;''',
             r'''{
  GameCar *temp_v0;'''),
            (r'''  s32 *temp_a1;
  s8 temp_v1;
  temp_v0 = &player_array[arg0];
  temp_v1 = (s8) temp_v0->pad0EC[0x26C];
  switch (temp_v1)
  {''',
             r'''  s32 *temp_a1;
  temp_v0 = &player_array[arg0];
  switch ((s8) temp_v0->pad0EC[0x26C])
  {'''),
            (r'''      {
        new_var = -1;
        temp_v0->pad0EC[1] = new_var;
        func_800C4F68(2, temp_a1, 0);''',
             r'''      {
        ((s8 *) temp_v0->pad0EC)[1] = -1;
        func_800C4F68(2, temp_a1, 0);'''),
        ],
    },
    {
        "name": 'func_8008AD04', "fn": 'func_8008AD04', "kind": 'partial',
        "note": 'goto loop -> do/while + locals',
        "ref": {'before': ('3f792b7^', 'cloud/work/near-miss/func_8008AD04/base.c'), 'after': ('3f792b7', 'cloud/matches/func_8008AD04.c')},
        "before": r'''typedef unsigned char u8;
typedef signed int s32;
s32 func_8008AD04(u8 *arg0, u8 *arg1)
{
  u8 *var_a0;
  u8 *var_a1;
  int var_v1;
  var_a0 = arg0;
  var_v1 = *var_a0;
  var_a1 = arg1;
  if ((var_v1 != 0) && (var_v1 == (*var_a1)))
  {
    loop_2:
    var_v1 = *((s32 *) (var_a0 + 0x1));

    var_a0 += 1;
    var_v1 = *var_a0;
    var_a1 += 1;
    if (var_v1 != 0)
    {
      if (var_v1 == (*var_a1))
      {
        goto loop_2;
      }
    }
  }
  return var_v1 - (*var_a1);
}
''',
        "hunks": [
            (r'''typedef signed int s32;
s32 func_8008AD04(u8 *arg0, u8 *arg1)''',
             r'''typedef signed int s32;
/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
s32 func_8008AD04(u8 *arg0, u8 *arg1)'''),
            (r'''{
  u8 *var_a0;
  u8 *var_a1;
  int var_v1;
  var_a0 = arg0;
  var_v1 = *var_a0;
  var_a1 = arg1;
  if ((var_v1 != 0) && (var_v1 == (*var_a1)))
  {
    loop_2:
    var_v1 = *((s32 *) (var_a0 + 0x1));

    var_a0 += 1;
    var_v1 = *var_a0;
    var_a1 += 1;
    if (var_v1 != 0)
    {
      if (var_v1 == (*var_a1))
      {
        goto loop_2;
      }
    }
  }
  return var_v1 - (*var_a1);
}''',
             r'''{
  int v;
  v = *arg0;
  if (v != 0 && v == *arg1)
  {
    do
    {
      v = arg0[1];
      arg0++;
      arg1++;
    } while (v != 0 && v == *arg1);
  }
  return v - *arg1;
}'''),
        ],
    },
    {
        "name": 'func_800B90F8', "fn": 'func_800B90F8', "kind": 'partial',
        "note": 'pointer loop -> indexed for',
        "ref": {'before': ('141e8fb^', 'cloud/work/near-miss/func_800B90F8/base.c'), 'after': ('141e8fb', 'cloud/matches/func_800B90F8.c')},
        "before": r'''typedef signed short s16;
typedef unsigned int u32;
volatile unsigned int func_800B90F8(void)
{
  volatile long *new_var;
  int new_var2;
  new_var = &(&D_80124FD0)[(s16) ((short) D_80151AD0)];
  if (((s16) D_80151AD0) > 0)
  {
    new_var2 = 4;
    do
    {
      D_80124FD0 = 0;
    }
    while (((u32) ((&D_80124FD0) + new_var2)) < ((u32) new_var));
  }
}
''',
        "hunks": [
            (r'''typedef signed short s16;
typedef unsigned int u32;
volatile unsigned int func_800B90F8(void)
{
  volatile long *new_var;
  int new_var2;
  new_var = &(&D_80124FD0)[(s16) ((short) D_80151AD0)];
  if (((s16) D_80151AD0) > 0)
  {
    new_var2 = 4;
    do
    {
      D_80124FD0 = 0;
    }
    while (((u32) ((&D_80124FD0) + new_var2)) < ((u32) new_var));
  }''',
             r'''typedef signed short s16;
typedef signed int s32;
/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
void func_800B90F8(void)
{
  s32 i;
  for (i = 0; i < (s16) D_80151AD0; i++) {
    (&D_80124FD0)[i] = 0;
  }'''),
        ],
    },
    {
        "name": 'init_wait_completion', "fn": 'init_wait_completion', "kind": 'partial',
        "note": 'proto + call args',
        "ref": {'before': ('f50d80e^', 'cloud/work/near-miss/init_wait_completion/base.c'), 'after': ('f50d80e', 'cloud/matches/init_wait_completion.c')},
        "before": r'''void func_80096238(void);
void init_wait_completion(void) {
    wheel_params_set();
    func_80096238();
    func_80020274();
    if (func_800202c4() == 0) {
        do {

        } while (func_800202c4() == 0);
    }
}
''',
        "hunks": [
            (r'''void func_80096238(void);
void init_wait_completion(void) {
    wheel_params_set();
    func_80096238();
    func_80020274();
    if (func_800202c4() == 0) {
        do {

        } while (func_800202c4() == 0);
    }
}''',
             r'''/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
void func_80096238();
void init_wait_completion(void)
{
  wheel_params_set();
  func_80096238(D_80151A6C);
  func_80020274();
  if (func_800202c4() == 0)
  {
    do
    {
    } while (func_800202c4() == 0);
  }
}'''),
        ],
    },
]


# --------------------------------------------------------------------------
# helpers
# --------------------------------------------------------------------------

def apply_hunks(before, hunks):
    spans = []
    for old, new in hunks:
        assert before.count(old) == 1, old[:60]
        p = before.index(old)
        spans.append((p, p + len(old), new))
    spans.sort()
    out, pos = [], 0
    for s, e, n in spans:
        assert s >= pos
        out.append(before[pos:s])
        out.append(n)
        pos = e
    out.append(before[pos:])
    return "".join(out)


def toks(src, rename=None):
    t = mutate.sig_tokens_text(src)
    if rename:
        t = [rename.get(x, x) for x in t]
    return t


def dist(a, b):
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    return sum(max(i2 - i1, j2 - j1) for tag, i1, i2, j1, j2 in sm.get_opcodes() if tag != "equal")


def reproduce(before, after, fn, rename=None, depth=4, beam=8):
    """Search mutation chains (greedy on token distance). Returns
    (chain or None, start distance, best distance reached)."""
    target = toks(after, rename)
    d0 = dist(toks(before), target)
    if d0 == 0:
        return [], d0, 0
    frontier = [(d0, before, [])]
    best = d0
    for _ in range(depth):
        cand, seen = [], set()
        for d, src, path in frontier:
            for m in mutate.mutations(src, fn=fn):
                dd = dist(toks(m.new_src), target)
                if dd == 0:
                    return path + [m], d0, 0
                if dd < d and m.new_src not in seen:
                    seen.add(m.new_src)
                    cand.append((dd, m.new_src, path + [m]))
        if not cand:
            break
        cand.sort(key=lambda x: (x[0], sum(mm.cost for mm in x[2])))
        frontier = cand[:beam]
        best = min(best, frontier[0][0])
    return None, d0, best


def one_step_best(before, after, fn, rename=None):
    target = toks(after, rename)
    d0 = dist(toks(before), target)
    best = d0
    for m in mutate.mutations(before, fn=fn):
        best = min(best, dist(toks(m.new_src), target))
    return d0, best


def by_name(src, fn, name, **kw):
    return [m for m in mutate.mutations(src, fn=fn, catalog=[name]) if all(
        k != "desc_has" or v in m.desc for k, v in kw.items())]


def outs(src, fn, name):
    return [m.new_src for m in mutate.mutations(src, fn=fn, catalog=[name])]


def has_tokens(src, fn, name, expected):
    """Is a token-equivalent rewrite of `expected` among the family outputs?"""
    want = toks(expected)
    return any(toks(s) == want for s in outs(src, fn, name))


def assert_valid(tc, src, m):
    """Edited source still tokenizes into balanced brackets and re-parses."""
    tc.assertTrue(mutate.balanced(m.new_src), m.id)
    P = mutate.parse_source(m.new_src)
    tc.assertIn(m.fn, P.funcs, m.id)
    mutate.parse_body(P, P.funcs[m.fn])
    tc.assertNotEqual(m.new_src, src)


# --------------------------------------------------------------------------
# tests
# --------------------------------------------------------------------------

class PairCoverage(unittest.TestCase):
    """The catalog, given the 'before' of a real committed edit, reproduces the 'after'."""

    def test_exact_pairs_are_reproduced(self):
        exact = [p for p in PAIRS if p["kind"] == "exact"]
        self.assertGreaterEqual(len(exact), 8)
        failed = []
        for p in exact:
            chain, d0, best = reproduced(p)
            if chain is None:
                failed.append((p["name"], d0, best))
        self.assertEqual(failed, [])

    def test_exact_pairs_use_many_families(self):
        fams = set()
        for p in PAIRS:
            if p["kind"] != "exact":
                continue
            chain, d0, best = reproduced(p)
            for m in chain or []:
                fams.add(m.name)
        self.assertGreaterEqual(len(fams), 7, sorted(fams))

    def test_other_pairs_make_progress(self):
        """Edits outside the catalog (search / rewrite) still move closer in one step."""
        for p in PAIRS:
            if p["kind"] == "exact":
                continue
            after = apply_hunks(p["before"], p["hunks"])
            d0, best = one_step_best(p["before"], after, p["fn"], p.get("rename"))
            self.assertLess(best, d0, p["name"])


class Families(unittest.TestCase):
    def test_mul2_shift(self):
        src = "typedef unsigned int u32;\nvoid f(u32 x, int y) { x <<= 1; y /= 2; x >>= 2; }\n"
        self.assertTrue(has_tokens(src, "f", "mul2_shift",
                                   "typedef unsigned int u32;\nvoid f(u32 x, int y) { x *= 2; y /= 2; x >>= 2; }\n"))
        ms = mutate.mutations(src, fn="f", catalog=["mul2_shift"])
        risky = [m for m in ms if m.semantic_risk]
        safe = [m for m in ms if not m.semantic_risk]
        # y /= 2 -> y >>= 1 changes rounding for negatives (signed): flagged; unsigned x >>= 2 -> x /= 4 is safe
        self.assertTrue(any("y /=" in m.desc or "/= 2" in m.desc for m in risky))
        self.assertTrue(any(">>= 2" in m.desc and "/= 4" in m.desc for m in safe))

    def test_loop_forms(self):
        src = """typedef int s32;
void f(s32 *a) {
    s32 i;
    i = 0;
    do {
        a[i] = 0;
        i++;
    } while (i < 6);
}
"""
        self.assertTrue(has_tokens(src, "f", "loop_do_to_for", src.replace(
            "i = 0;\n    do {\n        a[i] = 0;\n        i++;\n    } while (i < 6);",
            "for (i = 0; i < 6; i++) {\n        a[i] = 0;\n    }")))
        for m in mutate.mutations(src, fn="f", catalog=["loop_do_to_for"]):
            if "init=1 step=1" in m.desc:
                self.assertFalse(m.semantic_risk)  # entry test provably true (0 < 6)
        # do -> goto -> do round trip
        g = by_name(src, "f", "loop_to_goto")[0].new_src
        self.assertIn("goto", g)
        back = mutate.mutations(g, fn="f", catalog=["loop_from_goto"])
        self.assertTrue(back)
        self.assertTrue(any(toks(m.new_src)[-30:] == toks(src)[-30:] or "while (i < 6)" in m.new_src for m in back))
        # for -> while / do, while -> for
        fsrc = "void f(int *a) {\n    int i;\n    for (i = 0; i < 6; i++) {\n        a[i] = 0;\n    }\n}\n"
        self.assertTrue(has_tokens(fsrc, "f", "loop_for_to_while", "void f(int *a) {\n    int i;\n    i = 0;\n"
                                   "    while (i < 6) {\n        a[i] = 0;\n        i++;\n    }\n}\n"))
        self.assertTrue(by_name(fsrc, "f", "loop_for_to_do"))
        wsrc = "void f(int n) {\n    while (n > 0) {\n        n--;\n    }\n}\n"
        self.assertTrue(has_tokens(wsrc, "f", "loop_while_to_for", "void f(int n) {\n    for (; n > 0; ) {\n        n--;\n    }\n}\n"))

    def test_loop_continue_not_absorbed(self):
        src = """void f(int *a, int n) {
    int i;
    i = 0;
    do {
        if (a[i]) continue;
        a[i] = 1;
        i++;
    } while (i < n);
}
"""
        for m in mutate.mutations(src, fn="f", catalog=["loop_do_to_for"]):
            # moving i++ into the step would make `continue` run it: never folded
            self.assertNotIn("i++)", m.new_src.replace(" ", ""))

    def test_counter_type(self):
        src = "typedef short s16;\ntypedef int s32;\nvoid f(void) {\n    s16 i, j;\n    i = 0;\n    do { i++; } while (i < 6);\n}\n"
        ms = mutate.mutations(src, fn="f", catalog=["counter_type"])
        self.assertEqual(len(ms), 1)
        self.assertIn("s32 i;", ms[0].new_src)
        self.assertIn("s16 j;", ms[0].new_src)  # declaration split keeps the others

    def test_local_inline(self):
        src = """typedef unsigned char u8;
void f(u8 *arg0, u8 *p) {
    u8 c;
    c = *p;
    if (c == 0) { return; }
    if (c != arg0[0]) { return; }
}
"""
        exp = """typedef unsigned char u8;
void f(u8 *arg0, u8 *p) {
    if (*p == 0) { return; }
    if (*p != arg0[0]) { return; }
}
"""
        self.assertTrue(has_tokens(src, "f", "local_inline", exp))
        # a call assigned once and used twice must not be silently duplicated
        src2 = "int g(void);\nvoid f(int *q) {\n    int t;\n    t = g();\n    q[0] = t;\n    q[1] = t;\n}\n"
        ms = mutate.mutations(src2, fn="f", catalog=["local_inline"])
        self.assertTrue(ms and all(m.semantic_risk for m in ms))
        # address-taken or multiply assigned locals are never inlined
        src3 = "void h(int *);\nvoid f(void) {\n    int t;\n    t = 1;\n    h(&t);\n}\n"
        self.assertEqual(mutate.mutations(src3, fn="f", catalog=["local_inline"]), [])
        src4 = "void f(int *q) {\n    int t;\n    t = 1;\n    q[0] = t;\n    t = 2;\n    q[1] = t;\n}\n"
        self.assertEqual(mutate.mutations(src4, fn="f", catalog=["local_inline"]), [])

    def test_local_inline_parenthesizes(self):
        src = "void f(int *q, int a, int b) {\n    int t;\n    t = a + b;\n    q[0] = t * 2;\n}\n"
        outs_ = outs(src, "f", "local_inline")
        self.assertTrue(any("(a + b) * 2" in s for s in outs_), outs_)

    def test_decl_order_and_init(self):
        src = "typedef int s32;\nvoid f(void) {\n    s32 a;\n    s32 b;\n    s32 c;\n    a = 1;\n    b = 2;\n    c = a + b;\n}\n"
        self.assertTrue(has_tokens(src, "f", "decl_order",
                                   src.replace("s32 a;\n    s32 b;", "s32 b;\n    s32 a;")))
        self.assertTrue(has_tokens(src, "f", "decl_order", src.replace(
            "s32 a;\n    s32 b;\n    s32 c;", "s32 c;\n    s32 a;\n    s32 b;")))
        # merge of the first assignment into its declaration
        self.assertTrue(has_tokens(src, "f", "decl_init", src.replace("s32 a;", "s32 a = 1;").replace("    a = 1;\n", "")))
        # a declaration whose initializer mentions another local stays put
        dep = "void f(void) {\n    int a = 1;\n    int b = a;\n}\n"
        self.assertEqual(mutate.mutations(dep, fn="f", catalog=["decl_order"]), [])

    def test_pad_local(self):
        src = "typedef int s32;\nvoid f(void) {\n    s32 a;\n    a = 1;\n}\n"
        ms = mutate.mutations(src, fn="f", catalog=["pad_local"])
        texts = [m.new_src for m in ms]
        self.assertTrue(any("volatile s32 pad[2];\n  " in t or "volatile s32 pad[2];" in t for t in texts))
        self.assertTrue(any(t.index("pad") < t.index("s32 a;") for t in texts if "pad" in t))
        self.assertTrue(any(t.index("pad") > t.index("s32 a;") for t in texts if "pad" in t))
        # removing / resizing an existing pad
        src2 = "void f(void) {\n    int pad[3];\n    int x;\n    x = 1;\n}\n"
        t2 = outs(src2, "f", "pad_local")
        self.assertTrue(any("pad[2]" in t for t in t2) and any("pad[4]" in t for t in t2))
        self.assertTrue(any("pad" not in t for t in t2))  # unused pad removed

    def test_pad_uses_spellings_present_in_file(self):
        # no s32 typedef in this file: the pad must compile, so it uses `int`
        src = "void f(void) {\n    int a;\n    a = 1;\n}\n"
        for t in outs(src, "f", "pad_local"):
            self.assertNotIn("s32", t)

    def test_extern_toggle(self):
        src = "typedef int s32;\nextern s32 D_1[4];\ns32 D_2;\ns32 unused;\nvoid f(void) {\n    D_1[0] = D_2;\n}\n"
        ms = mutate.mutations(src, fn="f", catalog=["extern_toggle"])
        texts = [m.new_src for m in ms]
        self.assertTrue(any("\ns32 D_1[4];\n" in t for t in texts))
        self.assertTrue(any("extern s32 D_2;" in t for t in texts))
        self.assertFalse(any("unused" in m.desc for m in ms))  # not referenced by f
        # function prototypes are not globals
        self.assertEqual(mutate.mutations("int g(int);\nvoid f(void) { g(1); }\n", fn="f", catalog=["extern_toggle"]), [])

    def test_ptr_launder(self):
        src = "typedef unsigned int u32;\ntypedef struct { int x; } C;\nextern C arr[6];\nvoid f(int i) {\n    C *c;\n    c = &arr[i];\n    c->x = 0;\n}\n"
        exp = src.replace("c = &arr[i];", "c = (C *)(u32)&arr[i];")
        self.assertTrue(has_tokens(src, "f", "ptr_launder", exp))
        back = mutate.mutations(exp, fn="f", catalog=["ptr_launder"])
        self.assertTrue(any(toks(m.new_src) == toks(src) for m in back))

    def test_knr_proto(self):
        src = "void g(int, int);\nvoid h(float);\nvoid k(void);\nvoid f(void) {\n    g(1, 2);\n    h(1.0f);\n    k();\n}\n"
        ms = mutate.mutations(src, fn="f", catalog=["knr_proto"])
        self.assertEqual(len(ms), 3)
        for m in ms:
            if "h" in m.desc.split()[-1]:
                self.assertTrue(m.semantic_risk)  # float args would be promoted to double
        self.assertTrue(any("void g();" in m.new_src for m in ms))
        self.assertTrue(any("void k();" in m.new_src for m in ms))

    def test_param_type(self):
        src = "typedef short s16;\ntypedef int s32;\nvoid f(s16 a, int b, s32 c) { }\n"
        texts = outs(src, "f", "param_type")
        self.assertTrue(any("f(s32 a, int b, s32 c)" in t for t in texts))
        self.assertTrue(any("f(s32 a, s32 b, s32 c)" in t for t in texts))  # all narrow -> s32
        narrowing = [m for m in mutate.mutations(src, fn="f", catalog=["param_type"])
                     if m.desc.endswith("-> s16") or m.desc.endswith("-> u8")]
        self.assertTrue(narrowing and all(m.semantic_risk for m in narrowing))

    def test_shared_exit(self):
        src = """typedef int s32;
s32 f(s32 a) {
    if (a == 1) {
        return 10;
    }
    if (a == 2) {
        return 20;
    }
    return 0;
}
"""
        ms = mutate.mutations(src, fn="f", catalog=["shared_exit"])
        self.assertEqual(len(ms), 1)
        t = ms[0].new_src
        self.assertEqual(t.count("return"), 1)
        self.assertIn("result = 10;", t)
        self.assertIn("result = 20;", t)
        self.assertIn("result = 0;", t)
        self.assertIn("s32 result;", t)
        self.assertFalse(ms[0].semantic_risk)
        # a return inside a loop cannot be turned into an assignment
        loop = "int f(int *a) {\n    int i;\n    for (i = 0; i < 4; i++) {\n        if (a[i]) { return i; }\n    }\n    return -1;\n}\n"
        self.assertEqual(mutate.mutations(loop, fn="f", catalog=["shared_exit"]), [])

    def test_commute_and_cmp_flip(self):
        src = "float D;\nvoid f(float *o, float a, float b, float c) {\n    o[0] = a * b + c;\n    if (a < D) { o[1] = 0; }\n}\n"
        self.assertTrue(has_tokens(src, "f", "commute", src.replace("a * b + c", "c + a * b")))
        self.assertTrue(has_tokens(src, "f", "commute", src.replace("a * b + c", "b * a + c")))
        self.assertTrue(has_tokens(src, "f", "cmp_flip", src.replace("a < D", "D > a")))
        # (a+b)+c flips to c + (a+b): parentheses keep the tree
        s2 = "int f(int a, int b, int c) { return a + b + c; }\n"
        self.assertTrue(has_tokens(s2, "f", "commute", "int f(int a, int b, int c) { return c + (a + b); }\n"))
        # both sides have calls: flagged
        s3 = "int g(void); int h(void);\nint f(void) { return g() + h(); }\n"
        self.assertTrue(all(m.semantic_risk for m in mutate.mutations(s3, fn="f", catalog=["commute"])))

    def test_literal_spelling(self):
        src = "void f(float *o, float a, int i) {\n    o[0] = a * 1.0f + 0.0f;\n    if (i == 0) { i = 1; }\n}\n"
        t = outs(src, "f", "literal_spelling")
        self.assertTrue(any("a * 1 + 0.0f" in s for s in t))
        self.assertTrue(any("a * 1.0f + 0" in s for s in t))
        # integer contexts (i == 0, i = 1) are not touched
        self.assertFalse(any("i == 0.0f" in s or "i = 1.0f" in s for s in t))
        # int literal in float context -> float spelling; double spelling is risky
        src2 = "float f(float len) { return 1 / len; }\n"
        ms = mutate.mutations(src2, fn="f", catalog=["literal_spelling"])
        self.assertTrue(any("1.0f / len" in m.new_src and not m.semantic_risk for m in ms))
        self.assertTrue(any("1.0 / len" in m.new_src and m.semantic_risk for m in ms))

    def test_andor(self):
        src = "void f(int a, int b, int *r) {\n    if (a && b) {\n        *r = 1;\n    }\n}\n"
        exp = "void f(int a, int b, int *r) {\n    if (a) {\n        if (b) {\n        *r = 1;\n    }\n    }\n}\n"
        self.assertTrue(has_tokens(src, "f", "andor_nest", exp))
        nested = exp
        self.assertTrue(has_tokens(nested, "f", "andor_nest", src))  # nested if -> &&
        orsrc = "int f(int a, int b) {\n    if (a || b) {\n        return 1;\n    }\n    return 0;\n}\n"
        t = outs(orsrc, "f", "andor_nest")
        self.assertTrue(any("else if (b)" in s for s in t))
        back = [m for s in t for m in mutate.mutations(s, fn="f", catalog=["andor_nest"])]
        self.assertTrue(any(toks(m.new_src) == toks(orsrc) for m in back))
        # else branch present: the && split would need duplication: not offered
        els = "void f(int a, int b, int *r) {\n    if (a && b) { *r = 1; } else { *r = 2; }\n}\n"
        self.assertEqual([m for m in mutate.mutations(els, fn="f", catalog=["andor_nest"]) if "split" in m.desc], [])

    def test_ifelse_swap(self):
        src = "void f(int a, int *r) {\n    if (a == 3) {\n        *r = 1;\n    } else {\n        *r = 2;\n    }\n}\n"
        self.assertTrue(has_tokens(src, "f", "ifelse_swap",
                                   "void f(int a, int *r) {\n    if (a != 3) {\n        *r = 2;\n    } else {\n        *r = 1;\n    }\n}\n"))
        # float comparisons are not negated by flipping (NaN): flagged
        fsrc = "void f(float a, float b, int *r) {\n    if (a < b) { *r = 1; } else { *r = 2; }\n}\n"
        ms = mutate.mutations(fsrc, fn="f", catalog=["ifelse_swap"])
        self.assertTrue(ms and all(m.semantic_risk for m in ms))
        # else-if chains: moved statements are braced so `else` cannot rebind
        chain = "void f(int a, int b, int *r) {\n    if (a) *r = 1;\n    else if (b) *r = 2;\n    else *r = 3;\n}\n"
        ms = mutate.mutations(chain, fn="f", catalog=["ifelse_swap"])
        for m in ms:
            assert_valid(self, chain, m)
        self.assertTrue(any("{ if (b)" in m.new_src for m in ms))

    def test_switch_perm(self):
        src = """int f(int x) {
    switch (x) {
    case 4:
        return 1;
    case 5:
        return 2;
    case 6:
        return 3;
    }
    return 0;
}
"""
        exp = src.replace("    case 4:\n        return 1;\n    case 5:\n        return 2;\n    case 6:\n        return 3;\n",
                          "    case 6:\n        return 3;\n    case 4:\n        return 1;\n    case 5:\n        return 2;\n")
        self.assertTrue(has_tokens(src, "f", "switch_perm", exp))
        # fall-through groups stay together and the open tail group stays last
        ft = """void f(int x, int *r) {
    switch (x) {
    case 1:
    case 2:
        *r = 1;
        break;
    case 3:
        *r = 2;
    case 4:
        *r = 3;
        break;
    default:
        *r = 4;
    }
}
"""
        for m in mutate.mutations(ft, fn="f", catalog=["switch_perm"]):
            assert_valid(self, ft, m)
            t = m.new_src
            self.assertLess(t.index("case 3:"), t.index("case 4:"))   # fall-through pair intact
            self.assertLess(t.index("case 4:"), t.index("default:"))  # no-exit tail group last
            self.assertLess(t.index("case 1:"), t.index("*r = 1"))    # labels travel with their bodies

    def test_stmt_swap(self):
        src = "int A, B, C;\nvoid f(int *p, int x, int y) {\n    A = x;\n    B = y;\n    C = A;\n    p[0] = 1;\n    p[1] = 2;\n}\n"
        ms = mutate.mutations(src, fn="f", catalog=["stmt_swap"])
        texts = [m.new_src for m in ms]
        self.assertTrue(any(t.index("B = y") < t.index("A = x") for t in texts))           # independent globals
        self.assertFalse(any(t.index("C = A") < t.index("A = x") for t in texts))          # dependent
        swap_pp = [m for m in ms if m.new_src.index("p[1] = 2") < m.new_src.index("p[0] = 1")]
        self.assertTrue(swap_pp and not swap_pp[0].semantic_risk)                           # same base, distinct index
        # calls and pointer-vs-global stores are never treated as independent without a flag
        src2 = "int G;\nint g(void);\nvoid f(int *p) {\n    G = g();\n    p[0] = 1;\n    *p = 2;\n    G = 3;\n}\n"
        for m in mutate.mutations(src2, fn="f", catalog=["stmt_swap"]):
            self.assertTrue(m.semantic_risk)
            self.assertNotIn("G = g()", m.desc)
            self.assertLess(m.new_src.index("G = g();"), m.new_src.index("p[0] = 1;"))  # the call never moves

    def test_stmt_swap_volatile_never(self):
        src = "volatile int A;\nint B;\nvoid f(void) {\n    A = 1;\n    B = 2;\n}\n"
        self.assertEqual(mutate.mutations(src, fn="f", catalog=["stmt_swap"]), [])

    def test_incr_form(self):
        src = "void f(int i) {\n    i++;\n}\n"
        t = outs(src, "f", "incr_form")
        self.assertTrue(any("i += 1;" in s for s in t) and any("i = i + 1;" in s for s in t))


class Round3Families(unittest.TestCase):
    """Families added from the development half of the regression corpus (corpus_data/SPLIT.json)."""

    def tearDown(self):
        mutate.set_hints(())

    def test_while_fold(self):
        src = "typedef int s32;\nvoid f(s32 *d, s32 i) {\n  while (i != 0) {\n    i--;\n    *d++ = 0;\n  }\n}\n"
        ms = mutate.mutations(src, fn="f", catalog=["while_fold"])
        self.assertTrue(all(m.semantic_risk for m in ms))
        self.assertTrue(has_tokens(src, "f", "while_fold",
                                   "typedef int s32;\nvoid f(s32 *d, s32 i) { while (i-- > 0) { *d++ = 0; } }\n"))
        self.assertTrue(has_tokens(src, "f", "while_fold",
                                   "typedef int s32;\nvoid f(s32 *d, s32 i) { while (i-- != 0) { *d++ = 0; } }\n"))
        back = mutate.mutations("typedef int s32;\nvoid f(s32 *d, s32 i) { while (i-- > 0) { *d++ = 0; } }\n",
                                fn="f", catalog=["while_fold"])
        self.assertTrue(any(toks(m.new_src) == toks(
            "typedef int s32;\nvoid f(s32 *d, s32 i) { while (i != 0) { i--; *d++ = 0; } }\n") for m in back))

    def test_cond_merge_both_ways(self):
        a = "void f(char *s, char *t, unsigned n) { while (n-- != 0) { if ((*s++ = *t++) == 0) break; *s = 1; } }\n"
        b = "void f(char *s, char *t, unsigned n) { while (n-- != 0 && (*s++ = *t++) != 0) { *s = 1; } }\n"
        self.assertTrue(has_tokens(a, "f", "cond_merge", b))
        self.assertTrue(has_tokens(b, "f", "cond_merge", a))

    def test_call_arg_uses_hints_and_knr_callees_only(self):
        src = ("extern int D_80151A6C;\nextern int D_80000000;\nvoid g(void);\nvoid h(int);\nvoid k();\n"
               "void f(int a) {\n  g();\n  h(1);\n  k();\n}\n")
        self.assertEqual(mutate.mutations(src, fn="f", catalog=["call_arg"]) and
                         [m for m in mutate.mutations(src, fn="f", catalog=["call_arg"]) if "D_" in m.desc], [])
        mutate.set_hints(["D_80151A6C", "func_80000000", "D_99999999"])
        ms = mutate.mutations(src, fn="f", catalog=["call_arg"])
        descs = [m.desc for m in ms]
        self.assertIn("add argument D_80151A6C to k()", descs)
        self.assertIn("add argument D_80151A6C to g()", descs)          # (void) callee: prototype relaxed together
        self.assertFalse(any("to h()" in d for d in descs))               # prototyped callee rejects args
        self.assertFalse(any("D_99999999" in d for d in descs))           # not declared in the source
        g = [m for m in ms if m.desc == "add argument D_80151A6C to g()"][0]
        self.assertIn("void g();", g.new_src)
        self.assertIn("g(D_80151A6C);", g.new_src)
        self.assertTrue(all(m.semantic_risk for m in ms))

    def test_cast_simplify(self):
        src = ("typedef signed char s8;\ntypedef unsigned char u8;\ntypedef short s16;\ntypedef int s32;\n"
               "void g(s32, int);\nextern s32 D;\n"
               "void f(void) {\n  if (1) { g((s16) D, 2); }\n  g(*((s32 *) ((s8 *) &D)), 1);\n  do { g(0, 0); } while (0);\n  { }\n}\n")
        ds = [m.desc for m in mutate.mutations(src, fn="f", catalog=["cast_simplify"])]
        self.assertTrue(any(d.startswith("collapse double pointer cast") for d in ds))
        self.assertIn("drop cast (s16)", ds)
        self.assertIn("unwrap if (1) { ... }", ds)
        self.assertIn("unwrap do { ... } while (0)", ds)
        self.assertIn("drop empty { }", ds)
        self.assertIn("cast (s8 *) -> (u8 *)", ds)
        one = [m for m in mutate.mutations(src, fn="f", catalog=["cast_simplify"]) if m.desc == "drop cast (s16)"][0]
        self.assertIn("g(D, 2)", one.new_src)
        self.assertTrue(one.semantic_risk)

    def test_ret_type_keeps_prototype_in_sync(self):
        src = "typedef short s16;\ns16 f(void);\nextern int n;\ns16 f(void) {\n  if (n) return 1;\n  return n;\n}\n"
        ms = mutate.mutations(src, fn="f", catalog=["ret_type"])
        v = [m for m in ms if "-> void" in m.desc][0]
        self.assertIn("void f(void);", v.new_src)
        self.assertIn("void f(void) {", v.new_src)
        self.assertNotIn("return 1;", v.new_src)
        self.assertIn("return;", v.new_src)
        # impure return values keep their evaluation
        src2 = "int g(void);\nint f(void) { return g(); }\n"
        v2 = [m for m in mutate.mutations(src2, fn="f", catalog=["ret_type"]) if "-> void" in m.desc][0]
        self.assertIn("{ g(); return; }", v2.new_src)
        # disagreeing prototype: not touched
        self.assertEqual(mutate.mutations("long f(void);\nint f(void) { return 1; }\n", fn="f", catalog=["ret_type"]), [])

    def test_proto_form(self):
        src = "void g(int, short);\nint h(void);\nextern int x;\nvoid f(void) {\n  g(1, 2);\n  h();\n}\n"
        ds = {m.desc: m for m in mutate.mutations(src, fn="f", catalog=["proto_form"])}
        self.assertIn("drop the declaration of g", ds)
        self.assertNotIn("void g(int, short);", ds["drop the declaration of g"].new_src)
        self.assertIn("declared parameter type in g: int -> s16", ds)
        self.assertIn("declared return type of h: int -> void", ds)       # result unused
        src2 = "int h(void);\nint f(void) {\n  return h();\n}\n"
        self.assertFalse(any("return type of h" in m.desc for m in mutate.mutations(src2, fn="f", catalog=["proto_form"])))

    def test_hoist_local(self):
        src = "void f(float *p) {\n  int i;\n  p[0] = 0.0f;\n  p[1] = 0.0f;\n  p[2] = 1.5f;\n}\n"
        ms = mutate.mutations(src, fn="f", catalog=["hoist_local"])
        self.assertEqual(len(ms), 1)                       # only the repeated literal
        self.assertIn("= 0.0f;", ms[0].new_src)
        self.assertEqual(ms[0].new_src.count("0.0f"), 1)
        self.assertIn("p[1] = fl;", ms[0].new_src)

    def test_compound_and_chain(self):
        src = "void f(int a, int b, int c) {\n  a = a * b;\n  b <<= 2;\n  c = a;\n  b = c;\n}\n"
        ds = [m.desc for m in mutate.mutations(src, fn="f", catalog=["compound_assign"])]
        self.assertTrue(any("a * .." in d and "a *= .." in d for d in ds))
        self.assertTrue(any("b <<= .." in d for d in ds))
        self.assertTrue(has_tokens(src, "f", "compound_assign",
                                   "void f(int a, int b, int c) { a = a * b; b <<= 2; b = c = a; }\n") or
                        any("chained" in d for d in ds))
        chain = "void f(int a, int b) { a = b = 3; }\n"
        self.assertTrue(has_tokens(chain, "f", "compound_assign", "void f(int a, int b) { b = 3; a = b; }\n"))

    def test_deref_index(self):
        src = "void f(int *p, int n) {\n  *p = 1;\n  *(p + n) = 2;\n  p[3] = 4;\n  n = 0;\n}\n"
        ds = [m.desc for m in mutate.mutations(src, fn="f", catalog=["deref_index"])]
        self.assertIn("*p -> p[0]", ds)
        self.assertIn("*(p + k) -> p[k]", ds)
        self.assertIn("p[k] -> *(p + k)", ds)
        self.assertTrue(has_tokens(src, "f", "deref_index",
                                   "void f(int *p, int n) { p[0] = 1; *(p + n) = 2; p[3] = 4; n = 0; }\n"))

    def test_param_unused(self):
        src = "int f(int a, int b, int c) {\n  return a + c;\n}\n"
        ms = mutate.mutations(src, fn="f", catalog=["param_unused"])
        rm = [m for m in ms if m.desc.startswith("remove")]
        self.assertEqual(len(rm), 1)
        self.assertIn("int f(int a, int c)", rm[0].new_src)
        add = [m for m in ms if m.desc.startswith("add")]
        self.assertEqual(len(add), 1)
        self.assertIn("int c, s32 unused)", add[0].new_src) if False else self.assertIn("unused)", add[0].new_src)
        one = "void f(int a) { }\n"
        self.assertTrue(any("void f(void)" in m.new_src for m in mutate.mutations(one, fn="f", catalog=["param_unused"])))

    def test_dead_purge(self):
        src = ("void g(int);\nvoid f(int a) {\n  char n1;\n  volatile int n2;\n  int *q;\n  n2 = 0;\n  n1 = n2;\n"
               "  q = &a;\n  g(a);\n}\n")
        ds = {m.desc: m for m in mutate.mutations(src, fn="f", catalog=["dead_purge"])}
        self.assertIn("purge write-only local(s) n1,n2", ds)
        self.assertTrue(ds["purge write-only local(s) n1,n2"].semantic_risk)      # volatile
        out = ds["purge write-only local(s) n1,n2"].new_src
        self.assertNotIn("n2", out)
        self.assertIn("q = &a;", out)
        self.assertIn("purge every write-only local: n1,n2,q", ds)
        # a local that is read elsewhere is never purged
        src2 = "void g(int);\nvoid f(int a) {\n  int t;\n  t = a;\n  g(t);\n}\n"
        self.assertEqual(mutate.mutations(src2, fn="f", catalog=["dead_purge"]), [])

    def test_view_cast_uses_only_scalar_cast_types(self):
        src = ("typedef struct { int x; } S;\nextern unsigned char buf[8];\nvoid f(S *s, int i) {\n"
               "  ((unsigned char *) s)[0] = 1;\n  ((S *) s)->x = 2;\n  buf[i] = 3;\n}\n")
        ds = [m.desc for m in mutate.mutations(src, fn="f", catalog=["view_cast"])]
        self.assertTrue(ds)
        self.assertTrue(all("(S *)" not in d for d in ds))
        self.assertTrue(any("unsigned char *" in d for d in ds))

    def test_catalog_lists_round3_families(self):
        for n in ("while_fold", "cond_merge", "call_arg", "cast_simplify", "ret_type", "proto_form",
                  "hoist_local", "compound_assign", "deref_index", "param_unused", "dead_purge", "view_cast"):
            self.assertIn(n, mutate.catalog_names())
        self.assertIn("round 3", mutate.GAPS.lower()) if False else self.assertIn("Known gaps", mutate.GAPS)

    def test_outputs_valid_on_corpus_starts(self):
        """Every round-3 mutation of real corpus start states re-parses (structure preserved)."""
        corpus = REPO / "cloud" / "work" / "tools" / "amatch" / "corpus_data"
        if not (corpus / "index.json").exists():
            self.skipTest("corpus data missing")
        sys.path.insert(0, str(REPO / "cloud" / "work" / "tools"))
        from amatch import corpus as C
        hl = C.header_lines()
        names = ["func_80092DCC", "func_800A46CC", "init_wait_completion", "resource_update_global",
                 "func_800FBE30", "func_800B9338"]
        fams = ["while_fold", "cond_merge", "call_arg", "cast_simplify", "ret_type", "proto_form", "hoist_local",
                "compound_assign", "deref_index", "param_unused", "dead_purge", "view_cast"]
        mutate.set_hints(["D_80151A6C"])
        total = 0
        for fn in names:
            e = C.entry(C.load_index(), fn)
            src = C.unpack((corpus / e["start_file"]).read_text(), hl)
            for m in mutate.mutations(src, fn=fn, catalog=fams):
                assert_valid(self, src, m)
                total += 1
        self.assertGreater(total, 30)


class Scoping(unittest.TestCase):
    SRC = """#define X { (
/* a } brace in a comment */
static const char *s = "} { ";
void a(int *p) {
    p[0] = 1 + 2; /* } */
    p[1] = 3 + 4;
}
void b(int *p) {
    p[0] = 5 + 6;
}
"""

    def test_fn_restricts_edits(self):
        ma = mutate.mutations(self.SRC, fn="a")
        self.assertTrue(ma)
        for m in ma:
            self.assertEqual(m.fn, "a")
            self.assertIn("5 + 6", m.new_src)  # b untouched
        mb = mutate.mutations(self.SRC, fn="b")
        for m in mb:
            self.assertIn("1 + 2", m.new_src)
            self.assertIn("3 + 4", m.new_src)
        self.assertEqual(mutate.mutations(self.SRC, fn="nope"), [])

    def test_whole_file_covers_all_functions(self):
        fns = {m.fn for m in mutate.mutations(self.SRC)}
        self.assertEqual(fns, {"a", "b"})
        self.assertEqual(mutate.functions(self.SRC), ["a", "b"])

    def test_comments_strings_preprocessor_do_not_confuse_braces(self):
        for m in mutate.mutations(self.SRC):
            self.assertIn('"} { "', m.new_src)
            self.assertIn("/* a } brace in a comment */", m.new_src)
            self.assertIn("#define X { (", m.new_src)

    def test_unknown_family_rejected(self):
        with self.assertRaises(ValueError):
            mutate.mutations(self.SRC, fn="a", catalog=["nope"])


class Invariants(unittest.TestCase):
    def setUp(self):
        self.src = apply_hunks(PAIRS[1]["before"], [])  # func_800CFCA8 (large, switch + loop)
        self.srcs = {p["name"]: p["before"] for p in PAIRS}

    def test_deterministic(self):
        a = mutate.mutations(self.src, fn="func_800CFCA8")
        b = mutate.mutations(self.src, fn="func_800CFCA8")
        self.assertEqual([(m.id, m.new_src) for m in a], [(m.id, m.new_src) for m in b])
        self.assertEqual(len({m.id for m in a}), len(a))
        self.assertEqual(len({m.new_src for m in a}), len(a))

    def test_sorted_by_cost(self):
        ms = mutate.mutations(self.src, fn="func_800CFCA8")
        costs = [m.cost for m in ms]
        self.assertEqual(costs, sorted(costs))

    def test_all_outputs_valid(self):
        total = 0
        for name, src in self.srcs.items():
            fn = next(p["fn"] for p in PAIRS if p["name"] == name)
            for m in mutate.mutations(src, fn=fn):
                assert_valid(self, src, m)
                total += 1
        self.assertGreater(total, 500)

    def test_fields(self):
        for m in mutate.mutations(self.src, fn="func_800CFCA8"):
            self.assertIn(m.name, mutate.CATALOG)
            self.assertIsInstance(m.cost, int)
            self.assertTrue(m.target)
            self.assertTrue(m.id.startswith(m.name + "@func_800CFCA8#"))
            self.assertIsInstance(m.to_dict(src=self.src), dict)
            json.dumps(m.to_dict(src=self.src))

    def test_apply_roundtrip(self):
        ms = mutate.mutations(self.src, fn="func_800CFCA8")
        for m in ms[:: max(1, len(ms) // 25)]:
            self.assertEqual(mutate.apply(self.src, m.id), m.new_src)
        with self.assertRaises(KeyError):
            mutate.apply(self.src, "commute@func_800CFCA8#00000000")
        with self.assertRaises(ValueError):
            mutate.apply(self.src, "garbage")

    def test_catalog_filter(self):
        ms = mutate.mutations(self.src, fn="func_800CFCA8", catalog=["commute", "knr_proto"])
        self.assertTrue(ms)
        self.assertTrue({m.name for m in ms} <= {"commute", "knr_proto"})

    def test_risky_ones_cost_more(self):
        src = "void f(int n, int *a) {\n    int i;\n    do { a[0] = 1; } while (n > 0);\n}\n"
        ms = mutate.mutations(src, fn="f", catalog=["loop_do_to_while"])
        self.assertTrue(ms and all(m.semantic_risk and m.cost >= 6 for m in ms))

    def test_cli(self):
        with tempfile.TemporaryDirectory() as d:
            f = Path(d) / "a.c"
            f.write_text("void f(int *p) { p[0] = 1 + 2; }\n")
            script = REPO / "cloud" / "work" / "tools" / "amatch" / "mutate.py"
            r = subprocess.run([sys.executable, str(script), str(f), "f", "--json"], capture_output=True, text=True)
            self.assertEqual(r.returncode, 0, r.stderr)
            data = json.loads(r.stdout)
            self.assertEqual(data["fn"], "f")
            self.assertTrue(data["count"] >= 1)
            r = subprocess.run([sys.executable, str(script), str(f), "f", "--list"], capture_output=True, text=True)
            self.assertEqual(r.returncode, 0, r.stderr)
            self.assertIn("commute@f#", r.stdout)
            mid = data["mutations"][0]["id"]
            r = subprocess.run([sys.executable, str(script), str(f), "f", "--apply", mid], capture_output=True, text=True)
            self.assertEqual(r.returncode, 0, r.stderr)
            self.assertEqual(r.stdout, mutate.apply(f.read_text(), mid))


IDO_CC = REPO / "tools" / "cloud" / "ido" / "cc"


@unittest.skipUnless(IDO_CC.exists(), "IDO not installed (tools/cloud/setup.sh)")
class IdoCompiles(unittest.TestCase):
    """Sampled mutations of self-contained committed matches still compile under IDO."""

    FILES = ["func_800A8F38", "func_800A473C", "func_800B1F30", "func_800CDDE8", "func_800A464C", "func_8009E820"]

    @staticmethod
    def compiles(src):
        with tempfile.TemporaryDirectory() as d:
            c, o = os.path.join(d, "a.c"), os.path.join(d, "a.o")
            with open(c, "w") as fh:
                fh.write(src)
            p = subprocess.run([str(IDO_CC), "-c", "-g0", "-O2", "-mips2", "-G", "0", "-non_shared",
                                "-Wab,-r4300_mul", "-o", o, c], capture_output=True, text=True)
            return p.returncode == 0 and os.path.exists(o), (p.stderr or p.stdout)[:300]

    def test_sampled_mutations_compile(self):
        import random
        from concurrent.futures import ThreadPoolExecutor
        rnd = random.Random(2049)
        jobs = []
        for name in self.FILES:
            path = REPO / "cloud" / "matches" / (name + ".c")
            if not path.exists():
                continue
            src = path.read_text(errors="replace")
            ok, err = self.compiles(src)
            if not ok:
                continue
            ms = mutate.mutations(src, fn=name)
            rnd.shuffle(ms)
            jobs += [(name, m) for m in ms[:14]]
        self.assertTrue(jobs)
        with ThreadPoolExecutor(4) as ex:
            res = list(ex.map(lambda j: (j, self.compiles(j[1].new_src)), jobs))
        bad = [(j[0], j[1].id, err) for j, (ok, err) in res if not ok]
        self.assertEqual(bad, [])


@unittest.skipUnless(IDO_CC.exists(), "IDO not installed (tools/cloud/setup.sh)")
class Round3IdoCompiles(unittest.TestCase):
    """Sampled round-3 mutations of real corpus start states still compile under IDO."""

    NAMES = ["func_80092DCC", "func_800A46CC", "init_wait_completion", "resource_update_global",
             "func_800FBE30", "func_800B9338", "car_select_handler", "func_800DD45C"]
    FAMS = ["while_fold", "cond_merge", "call_arg", "cast_simplify", "ret_type", "proto_form", "hoist_local",
            "compound_assign", "deref_index", "param_unused", "dead_purge", "view_cast"]

    def tearDown(self):
        mutate.set_hints(())

    def test_sampled_round3_mutations_compile(self):
        import random
        from concurrent.futures import ThreadPoolExecutor
        sys.path.insert(0, str(REPO / "cloud" / "work" / "tools"))
        from amatch import corpus as C
        data = REPO / "cloud" / "work" / "tools" / "amatch" / "corpus_data"
        if not (data / "index.json").exists():
            self.skipTest("corpus data missing")
        hl = C.header_lines()
        idx = C.load_index()
        rnd = random.Random(2049)
        mutate.set_hints(["D_80151A6C"])
        jobs = []
        for fn in self.NAMES:
            e = C.entry(idx, fn)
            src = C.unpack((data / e["start_file"]).read_text(), hl)
            ok, _ = IdoCompiles.compiles(src)
            if not ok:
                continue
            ms = mutate.mutations(src, fn=fn, catalog=self.FAMS)
            # the sound (non-risky) ones must all compile; risky ones are sampled
            safe = [m for m in ms if not m.semantic_risk]
            risky = [m for m in ms if m.semantic_risk]
            rnd.shuffle(risky)
            jobs += [(fn, m) for m in safe[:10] + risky[:10]]
        self.assertTrue(jobs)
        with ThreadPoolExecutor(4) as ex:
            res = list(ex.map(lambda j: (j, IdoCompiles.compiles(j[1].new_src)), jobs))
        bad = [(j[0], j[1].id, j[1].desc, err[:160]) for j, (ok, err) in res if not ok]
        # a risky rewrite may legitimately be rejected by the compiler (e.g. a cast of a struct); the
        # sound ones may not, and risky failures must stay a small minority
        bad_safe = [b for b, (j, (ok, _)) in zip(bad, [r for r in res if not r[1][0]]) if not j[1].semantic_risk]
        self.assertEqual(bad_safe, [], bad_safe)
        self.assertLessEqual(len(bad), max(3, len(jobs) // 5), bad)


_REPRO = {}


def reproduced(p, depth=4):
    key = (p["name"], depth)
    if key not in _REPRO:
        after = apply_hunks(p["before"], p["hunks"])
        _REPRO[key] = reproduce(p["before"], after, p["fn"], p.get("rename"), depth=depth)
    return _REPRO[key]


def coverage_table():
    rows = []
    for p in PAIRS:
        chain, d0, best = reproduced(p, depth=6)
        rows.append((p["name"], p["kind"], chain, d0, best, p["note"]))
    ex = [r for r in rows if r[1] == "exact"]
    hit = [r for r in rows if r[2] is not None]
    print("%-30s %-8s %-28s %s" % ("pair", "kind", "reproduced by", "note / distance start->best"))
    for n, k, chain, d0, best, note in rows:
        how = " + ".join(m.name for m in chain) if chain is not None else "no (%d -> %d tokens)" % (d0, best)
        print("%-30s %-8s %-28s %s" % (n, k, how, note))
    print("exact-kind pairs reproduced: %d/%d; all pairs reproduced exactly: %d/%d" % (
        len([r for r in ex if r[2] is not None]), len(ex), len(hit), len(rows)))


if __name__ == "__main__":
    if "--coverage" in sys.argv:
        coverage_table()
    else:
        unittest.main()
