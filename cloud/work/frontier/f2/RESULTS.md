# Lane f2 (20-minute dig) — RESULTS

| function | GC identity | bytes | state | best file | scorer line |
|---|---|---|---|---|---|
| object_bytes_sum_global | TB_GetFontLineHeight | 84 | 4 words, unchanged (&gFontHeader web t2, retail t4) | cloud/work/frontier/w36w/text/best_object_bytes_sum_global.c | `FAIL object_bytes_sum_global: 4 of 21 words differ` |
| object_bytes23_sum | TB_GetFontHeight | 72 | not reworked (same t4 cause) | - | - |
| drone_collision_avoid | calcMenuRect@audioscr.c | 852 | 5 words, unchanged (menuIdx home sp+28, retail sp+32) | cloud/work/frontier/w37b/rect/v/a0.c | `FAIL drone_collision_avoid: 5 of 213 words differ` |

No MATCH. Variants are in tb/ and rect/ (rect/u.sh is w37b's scorer with --tag f2).

## object_bytes_sum_global (tb/v1-v5, h1-h4)
- Local static asc()/des() copies (sound_update_channel(0); return D_801497F0->ascent/descent), with or without
  `do {} while (0);` after the call, `do { call } while (0)`, a returned u8 local, a `FontHdr *h` local, `s16 r` temp,
  `(s8)` on lead(): the address web is always t2 (4 words). Block ends inside the helpers do not move it;
  `if (D_801497F0) {}` and the `h` local cost a word (15 rows).

## drone_collision_avoid (rect/b*, c*, d1, e*, f*, g*, k*, m*)
- The menuIdx home is in the BOTTOM spill area (ra at sp+20, then one slot at 24, then the IV at 28). It stays at 28 when
  the frame grows by 16 (k6: `double` local in du_font_set), when u0 moves within the declarations (k1-k4), with u0 as f32/s8/s16,
  with every helper parameter/return type change (f1-f8), with K&R validOption (g1: frame 136), and with the width
  `?:` assignments as if-statements (c1/c3: same 5 words, so those `?:` carry no compiler temp). Only d1 (char loop
  `?:` as if/else) moved it, by +8 together with the frame and every local (126 rows).
- So the residual is not the local/Udef area order: retail has one more 4-byte slot (or an 8-aligned IV slot) in the
  bottom area between ra (sp+20) and the IV home. Value-context `?:` forms that compile identically
  (`(s32)(c ? a : b)`) add nothing; `0 + (...)` and `(u32)` change code (176 rows).
- Next: find what puts a second 4-byte item in the bottom spill area (a second spilled web, such as a codeless one,
  or a compiler temp that uopt allocates there), not in the local areas.
