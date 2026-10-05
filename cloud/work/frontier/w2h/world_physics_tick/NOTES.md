# world_physics_tick (0x800EC2F8, 1,564 bytes) - structurally right, 60 aligned rows / +3 words

Real semantics: the N64 descendant of arcade `init_cars()` (game/mdrive.c): for each of D_801543CA players
clear model[i] (D_8014A250, 2056 bytes, preserving the f32 at +1816) and game_car[i] (player_array /
D_80152818, 952 bytes) byte by byte, initialise them from the link record D_80153E88[i] (8 bytes:
body at +1, flags at +6, owner at +7), call func_800B27E4(gc); in-game cars get model[num_active++].slot = i
(num_active = D_80152744, s8), difficulty/handicap bytes from five s8[][13] tables at 0x8011103C.. indexed
[slot+1][body] (or [0][body]), next checkpoint = inlined get_next_checkpoint (`idx+1 == count ? first :
idx+1` over D_80151CE8), func_800EC270(m, gc), and for remote cars the arcade BODYR/colrad block from
collsize D_8011F844[body] (front, rear, side, height; colrad = sqrt(max(front,rear)^2+side^2+height^2)).
Then cars i >= count are reset and a few globals set (D_80153E84 = D_8015274C (+7 if D_80153FD2 >= 2) ...).

best.c: `blob_unit --tag w2h score world_physics_tick --with best.c` ->
`FAIL world_physics_tick: 305 of 391 words differ; compiled body is 394 words, target 391`
(positional count is meaningless with the +3 shift; `udis.py` aligned: 60 differing rows, relocation
addends masked).  Starting draft was the C52 -O2 source (363/391).

What moved it (in order of effect):
- `volatile s16` for D_801543CA, D_80153E84, D_80153FD2 (retail `lui; addiu; lh 0(reg)` on each read and
  the `+= 7` re-reads).
- the 0x80146180 test is a `switch` (case 1 / case 2), not if/else: frees the s8 that `112` stole and
  &num_active lands in s8 as in retail; the two `6` constants must not be one web either (`== 6U`).
- the five tables are `s8 T[][13]` indexed `[car->half1990 + 1][(u8) car->byte8]` - the `(u8)` re-read of
  the stored s8 is retail's `andi v1,t4,0xff`; reading half1990 directly (no `index` local) gives v0/v1 right.
- `get_next_checkpoint` as an inlined helper returning `cp + 1` twice (retail computes idx+1 twice).
- statement order: `car->object = D_80110D08;` before `half1994 = ...` (temp-ring phase), and
  `byte2027 = 1; byte2026 = 1; byte777 = byte2027; byte785 = -1`.
- `#pragma intrinsic (sqrtf)`.
Remaining (each a few rows):
1. both byte-clear loops: retail puts the store in the delay slot (`sb zero,-1(v1)` / `sb zero,-4(v1)`),
   ours the pointer increment.  Pointer, `*p++`, `while (n--)`, pre-increment and u8 forms tried: no fix.
2. retail hoists &D_80151CE8 into a0 before `if (half1992)` and sets a0 = m in the jal delay slot; ours
   materialises it twice inside the block (volatile / pointer-local forms worse).
3. the `li at,6 / li t3,6 / lui t4` group is scheduled earlier in ours (as1), and `sh 1994` / `sw 0` order.
4. byte1997..1999: retail loads the three config bytes before storing byte15 (alias/scheduling); struct copy
   and u8 types tried, no fix.
5. tail: retail shares one `lui at` for model[0].byte9/byte10 and loads D_8015274C before the D_80152015 store.
The helper `next_cp` is a stand-in for the inlined get_next_checkpoint; retail's own copy (if any) is not
identified (no stub neighbour), so a final source must name or make it static.
