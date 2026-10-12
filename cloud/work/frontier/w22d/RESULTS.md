# w22d RESULTS (wave 22, game): cpak_read, buffer_swap, func_80090308

| function | bytes | state | flags | scorer output |
|---|---:|---|---|---|
| cpak_read | 848 | **MATCH** (standalone and unit), one disclosed device | -g0 -O3 -mips2 -G 0 -non_shared | Builder `score.py fn cand/cpak_read.c cpak_read` -> `MATCH` / `own .rodata verified at 0x80123C10..0x80123C1C`. Pi `blob_unit --tag w22d score cpak_read --with cloud/matches/cpak_read.c --neighbours` -> `EQUAL cpak_read: 212 words (kept, c_cpak_read.c)` / `locked bodies that differ in this unit: 0` |
| buffer_swap | 1676 | 43 words off (was 418/419 in B137), body 419/419, frame 240/240 | same | Pi `blob_unit --tag w22d score buffer_swap --with cloud/work/frontier/w22d/buffer_swap/best.c --neighbours` -> `FAIL buffer_swap: 43 of 419 words differ` (8 of them own-.rodata relocations) / `locked bodies that differ in this unit: 0` |
| func_80090308 | 1128 | not matched: frame 168 vs 272, 299 rows | same | `--internal func_80090308` with real callers entity_physics_update (7 rows, colour only) and save_write_data (153 rows); `locked bodies that differ in this unit: 0` |

## cpak_read (arcade UpdateVisuals): `cloud/matches/cpak_read.c`
The first draft was 97 words off. Four findings closed it:
- `extern volatile u32 D_80124F70`. Retail's `lui/addiu/lw 0(t9)` is IDO's form for a volatile access. Non-volatile is 89 words off.
- `cpak_init` is declared `void cpak_init(s32 slot)`, the type of its locked definition (frontier_skid_marks group.c). The `s16` prototype that other files carry re-narrows a0, which costs 2 words.
- The comparison is written `D_8002AFB4 == 50.0f`. This operand order puts 50.0 in $f4.
- **Compiler-runtime names (tooling gap, disclosed device).** IDO lowers `(u64)D_80124F70 * 64 / 3000` -> f32 to calls of
  `__ll_mul`, `__ull_div` and `__ull_to_f` (ultralib ll.c/llcvt.c, w16g). This repo names those static functions
  `__muldi3` 0x8000DA58, `__udivdi3` 0x8000D958 and `__floatundisf` 0x80009BC8. blob.ld and symbols.json only PROVIDE those names.
  - The natural form is `cpak_read/natural.c`. score.py fn gives `3/212 words differ (unresolved symbols: __ll_mul+0x0 at
    +0x5c, __ull_div+0x0 at +0x70, __ull_to_f+0x0 at +0x7c)` with own .rodata verified.
  - The delivered file writes the three calls explicitly, under the repo names, with prototypes that match the locked
    definitions. That scores MATCH.
  - **Coordinator choice:** land the explicit form now, or add link aliases (`__ll_mul = 0x8000DA58`,
    `__ull_div = 0x8000D958`, `__ull_to_f = 0x80009BC8`, or the ultralib rename, cf. w22g) and land natural.c instead.
- Integration: a plain game single (`splice_singles.py cpak_read`). It needs no unit_overrides, no group and no data-symbol change.
  Its own .rodata (36000/45000/30000 us budgets) was verified by score.py.

## buffer_swap: `buffer_swap/best.c` (43 words, header lists every device)
The B137 draft was 418/419 words off. Starting from it, five steps brought it to 43.
1. **Correct prototypes.** `model_bounds_calc(Visual *v, s32 on)` follows the w17c CheckVisible order. D_8013FECB is `s8`.
2. **Inlined entity_hierarchy_update (x2).** In the unit, umerge never inlines the kept locked one. A cfe `__inline` copy is needed (w10g precedent).
   - The copy is renamed `ehu_inline`, so the locked symbol is not redefined. An `__inline entity_hierarchy_update` also scores the
     same, but it needs the prefer_definition override that frontier_mode_select uses.
   - Result: body 387 -> 415 words, frame 176 -> 192.
3. **Named-frame residual `pX`, `pY[3]`.** Retail has position at sp+208 and reverse at sp+192.
4. **Inline-area layout from `__inline` helpers.** The helpers are tire_extend (block 2: transform setup, flags, index, time, sound start),
   tire_sound_on and tire_sound_off (stops). With them, the frame is 240 and the handle homes are 136/112, as in retail.
   - Block 1's sound start must stay written in place. Its helper area would otherwise land before ehu#1 (h1 at 128).
   - `static` helpers (umerge-inlined) are far worse (134 words). The `__inline` (cfe) form is what matches.
   - **Trap:** my first helper name `sound_stop` collided with the real kept `sound_stop`, and 10 locked bodies broke. Renamed.
5. **Laundered model pointer** `(Model2056 *)(u32)&D_8014A250[slot]`. This makes the position[0] store kill the `model->type`
   load, as retail does (lbu reload plus multu by a hoisted 12): 63 -> 43 words.

Remaining residual:
- **Spill home:** model is at 88 in ours and 92 in retail. Spill webs 10/15 take home 0 first (`spill.sh`). That is 16 rows.
- **mul.s operand order** of sign*table: retail has `$f0,$f4`, ours has `$f4,$f0`. Source order and an expression-for-sign
  form do not change it.
- **as1 order** in the three time updates: `sh zero,4(s1)` / `lh index` against the D_801543CC + 0.015 loads.
  Same-line statements made no difference. Helpers defined after the function cannot be `__inline`.

Next hypothesis: identify webs 10/15 (both have table 554; probably the `v->slot` and `v->flags` loads) and find
why they get spill homes before model's web. Then check the as1 ties with `as1t.sh`.

## func_80090308 group (+ entity_physics_update, save_write_data): drafts in `func_80090308/`
- **Why a group.** func_80090308 is internal. It saves no s-registers, and its callers pass the s16 car index in their
  outgoing 0(sp) slot (`sw t8,0(sp)`). Both real callers save s0-s8/f20-f30.
  - With one caller, umerge inlines it.
  - With the two real callers (`--internal func_80090308`), the convention reproduces: no saves, param read at 274(sp).
  - A group needs all three: func_80090308, entity_physics_update (0x800908A0, kept; save_write_data stores its address)
    and save_write_data (0x800AF06C).
  - It does not need entity_spawn_init's group as context. entity_spawn_init is called by jal.
- **Identified callee:** **func_80090770** (locked, 11 words, between func_80090308 and entity_physics_update) is the texture
  setter that entity_physics_update inlines. It took entity_physics_update from 71 to 23 rows.
- **entity_physics_update: 7 rows, colour only.**
  - `force.sh "p1:w9=c4"` (slot web -> a1) leaves only the 2 own-rodata rows.
  - Structure that works: the kill block is placed first under `if (!update)`, with `goto kill` from the inactive and count==30 paths.
    The condition is `slot<0 || (slot>=0 && value<=0)`, which gives retail's redundant `bltzl`. D_8002EB94 is `volatile`.
  - Retail colours slot after id/tex (func_80090770's params, save 2.0); a0 is forbidden for slot. The w9 save must be
    < 2.0 while slot is also reloaded after `count++`. b2 (no reload) has save 1.8 and gives a1. b1 (reload) has save 2.5 and gives v0.
- **func_80090308:** the frame is the blocker (ours 168 or less, retail 272). In retail, node is at 268 (first local),
  color at 212 (15th word), 47 words below.
  - Identified: func_8008E06C (locked colour setter, `&color` keeps color in memory) and func_8008B2E4 x4.
  - The seed is not reloaded between rand 1-3, so the three values are computed before the stores.
  - `puff_init` (an inlined per-particle helper; s5 = its param copy) is a hypothesis.
  - Still 10 named words plus 64 bytes of inline area unexplained.
- **save_write_data:** first draft only (153 rows, frame 192 vs 352). Arcade ancestor not found. Its sound tail is the same
  "if (D_8010FFC0 == 0) return; if (id == -1) return; camera_target_track(...)" helper as camera_aspect_ratio.
- **Next:** frame-map func_80090308 with `spill.sh` / w12h `mu.sh`, then a priority lever for the entity_physics_update slot web.
  The group is 3 KB, so plan a full lane for it.

## Integration notes
- cpak_read: see above. No other deliverable is ready to land, and no groups or overrides are proposed.

## What generalises
- `lui/addiu/lw 0(reg)` on a lone global is a `volatile` read. It appeared three times here (D_80124F70, D_8002EB94, D_801543CC in buffer_swap).
- IDO -O3 u64 arithmetic calls ultralib's `__ll_*`/`__ull_*` names. Game C with u64 math needs link aliases to the
  libgcc-named statics.
- Name every scratch helper uniquely and check it against symbols.json. A collision silently redefines a locked function.
- `__inline` (cfe) helpers and `static` (umerge) helpers produce different code and frame layouts. Retail's inline-area
  order can tell which statements were helper calls.
- A small kept setter next to an unmatched function (func_80090770, func_8008E06C) is often inlined by its neighbours.
  Grep the ≤40-word functions for the store pattern.

No permission denials. Builder scratch: watchman2:~/rush2049/scratch/frontier/w22d. Variants are in `*/v/` (ignored).
