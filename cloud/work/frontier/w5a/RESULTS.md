# Wave 5, agent w5a — results

Builder scratch `~/rush2049/scratch/frontier/w5a` (copied from `base`; `uopt` and `as1` are symlinks to the
w3a/w4a traced builds). Unit runs use `--tag w5a`. Tools in `tools/` are the w3a/w4a/w1b kits retagged for w5a,
plus `u.sh NAME cand.c [blob_unit args]` (unit score summary). `ctrace.sh`/`ulist.sh` accept `EXTRA="--internal FN"`.

| # | Function | Bytes | State | Flags | Where |
|---|---|---:|---|---|---|
| 1 | func_80092278 | 232 | **strict MATCH in a real group** with its two callers (also matched) | -O3 | `groups/list_alloc_sound/` |
| 1b | entity_flags_apply | 292 | **strict MATCH** (group member, claimed) | -O3 | same group |
| 1c | high_scores_display | 468 | **strict MATCH** (group member, claimed) | -O3 | same group |
| 2 | sound_stop | 160 | 12 of 40 words off | -O3 | `sound_stop/best.c` |
| 3 | func_800EF5B0 | 124 | **strict MATCH** | -O3 only | `cloud/matches/func_800EF5B0.c` |
| 4 | func_8008E408 | 1,544 | ~85 aligned rows off (175/386 positional), 2 words short | -O3 | `func_8008E408/best.c` |
| 5 | entity_tick_main + drone_ai_update | 5,964 | not attempted beyond literals/frame analysis | -O3 | `drone_pair/best.c` |
| 6 | func_800E1F80 | 1,060 | **strict MATCH**, own rodata verified | -O3 (also -O2) | `cloud/matches/func_800E1F80.c` |
| 7 | sfx_position_3d | 692 | **strict MATCH** | -O3 (also -O2) | `cloud/matches/sfx_position_3d.c` |

Strict: 6 functions, 2,868 bytes (3 singles 1,876 + one group of 3, 992). Nothing committed or spliced.

---

## 1. func_80092278 + entity_flags_apply + high_scores_display — group, all three strict MATCH

```
score.py group cand/list_alloc_sound --claims
Members:
func_80092278:
  MATCH
entity_flags_apply:
  MATCH
high_scores_display:
  MATCH
Context (informational; excluded from exit status):
func_8009211C:
  MATCH
func_80091FBC:
  MATCH

blob_unit --tag w5a score func_80092278 entity_flags_apply high_scores_display \
    --with groups/list_alloc_sound/group.c --internal func_80092278 --neighbours
  EQUAL func_80092278: 58 words (internal, c_group.c)
  EQUAL entity_flags_apply: 73 words (kept, c_group.c)
  EQUAL high_scores_display: 117 words (kept, c_group.c)
  locked bodies that differ in this unit: 0
```
Group = the A54 closure (`cloud/work/ipa-groups/codex_list_alloc_a54`, 5 words off) with three fixes, each found
with the traced tools:
- **func_80092278** (5 words, as1 order of four `la`): the traced as1 showed a tie on `aftercycles` broken by
  line; editing the listing proved that only the free-list `la` must carry an earlier `.loc` than the three
  tag-statement addresses. Source form: a compiled-out check after the free-list read, `if (node == 0) {}`,
  ends the block, so the other addresses are materialised on the next statement's line. Any condition works.
- **entity_flags_apply** (38 words): the uopt trace showed the two stores of `1` (state, live) sharing one
  constant web in v1 that pushed `index` to t0; `force.sh … p1:w37=s` (split) reproduced retail except two
  float stores. Natural source: `Node68.state` is **`s32`** (the bytes flag/live are u8) — then the constants
  are separate webs and retail's `li 1` per store appears. The float stores `f8/f12/f16` go on separate lines.
- **high_scores_display** (82 words): each clamp is one `?:` expression
  (`f = (x < lo) ? lo : ((hi < x) ? hi : x)`), giving retail's single store after the select.

Integration: `func_80092278` must be **internal** (`--internal` in the unit; add a `forced_internal` override, or
leave it out of `keep` as in group.json). Third caller `camera_target_track` is unmatched (its retail prologue
saves s0/s1, consistent with the unsaved-s0/s1 IPA contract). Group context `func_8009211C`/`func_80091FBC` are
the locked list remove/insert, unchanged.
Types recovered: `Link {next, prev}`, `List {u8 indirect, doubly; u32 count; Link *head, *tail}`, 68-byte node
(`tag` +12, `state` s32 +16, bytes +24..27, floats +32..+48, index/value +52/+56), 24-byte command
(`kind` +2, `value` +4, floats +8/+12/+16, `node` +20).

## 3. func_800EF5B0 — strict MATCH (first compile)

```
score.py fn cand/func_800EF5B0.c func_800EF5B0 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_800EF5B0:
  MATCH
blob_unit: EQUAL func_800EF5B0: 31 words (kept)   (-O2: 28/31 differ)
```
Arcade `RenameBlit` (LIB/blit.c) pasted verbatim: `MBOX_FindTexture` inlined as `func_800B24EC(name, &idx, 0,
count - 1, MBOX_WARN)` (retail stores 1 in the fifth slot), `InitBlit` = collision_sound_play,
`UpdateBlit` = Input_ApplyPadConfig. The compiled-out `if (!ti) FatalMsg` is kept (bytes identical without it).

## 6. func_800E1F80 — strict MATCH, own rodata verified

```
score.py fn cand/func_800E1F80.c func_800E1F80 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_800E1F80:
  MATCH
    own .rodata verified at 0x801243C4..0x801243CC
blob_unit: EQUAL func_800E1F80: 265 words (kept); locked bodies that differ: 0      (-O2: MATCH)
```
Applies the car's push record (car+1608: strength bits 11..15, 0x10 direct push, 0x20 scripted-path direction
via func_800C1A00) as a body-space impulse (func_800A61B0), torque, or per-wheel force for unloaded tyres on
0x30 surfaces. Literals: `1.4666667f` (88/60, mph→ft/s), `1.0f/16384.0f`, `30.0f`.
Path: draft 81 → 21 words (tyre test `<= 0.0f`; `d = …; d *= mass;` as two statements) → the uopt trace showed
two colour-priority swaps (1/16384 vs 30.0 for f20; &out vs const 4 for s7); `force.sh` with both swapped gave
identical code; **one compiled-out check in the wheel loop** (`if (i) {}` after the body-space transform; any
condition) adds a block to the 30.0 and 4 webs and closes it.

## 7. sfx_position_3d — strict MATCH

```
score.py fn cand/sfx_position_3d.c sfx_position_3d --flags "-g0 -O3 -mips2 -G 0 -non_shared"
sfx_position_3d:
  MATCH
blob_unit: EQUAL sfx_position_3d: 173 words (kept); locked bodies that differ: 0     (-O2: MATCH)
```
N64 `InitDynamicObjs` (arcade game/visuals.c), extended with an RGBA5551 palette build (`(u16)GPACK_RGBA5551(r,
g, b, 0) | 1`), the car-part loop via sfx_volume_set, 24 rim textures, the arcade `TrkNames[i - NUM_DYN_OBJS]`
loop (one entry, i = 156) and 10 spark textures. Quirks: arcade declaration list `i, j, k, model` + an unused
buffer (≥ 13 bytes) for the 112-byte frame; slot loop uses `i`, lookup loops `k`, clear loop `j` (one shared
variable loses s0 for the slot counter — found by sweeping the loop-variable assignment, 54 variants);
`k != 157`; sfx_volume_set declared with an s16 model (caller loads `lh`).
Globals: gObjList `D_801427C0` (s16[157]), gTexList-like `D_80143F68` (TEXDEF*[24]), texture tables
`D_80151AE8` (8-byte entries, TEXDEF* first, 36-byte TEXDEF), car info `D_80153E88` (8-byte, car byte at +1).

## 2. sound_stop — 12/40 (not a match)

`score.py fn … sound_stop` → `12/40 words differ` (best.c). Residual lane: post-search-loop register webs
(see best.c header). Key experiments: the `p = &pool[i]; i = slot` form reproduces the post-loop block but
breaks the pool-count promotion; a compiled-out `if (D_80149450[i] != voice) {}` keeps the promotion and puts
&pool[i] in a1 like retail. Static-helper splits (stub `func_800B3584` precedes it) and local count caching did
not help. ~130 variants: stopped.

## 4. func_8008E408 — ~85 aligned rows (not a match)

Start: A159 source, 376/386 positional. Now 175/386 positional, 384 of 386 words, all structure right. Recovered
facts and the four open lanes are in `func_8008E408/best.c`. The decisive moves were frame reconstruction
(retail 256 bytes, all named-slot offsets reproduced), `o` reused for both particles (home 172), u32 loop
counters, an inlined `frand(range)` so `* 1.0f` survives, and two compiled-out checks. Best next hypothesis:
the extra spill slot and the surviving inner-loop pointer increment both suggest the matrix scale is an inlined
helper with its own pointer variable; try the real `func_8008B32C` body as a static with one call site per copy.

## 5. entity_tick_main + drone_ai_update — not attempted in depth

`drone_pair/best.c` = r5_e's complete first pass with its 25 fake `extern f32 D_80123Axx` replaced by literals
(entity_tick_main owns 0x80123A00..0x80123A5C, drone_ai_update starts at 0x80123A60). Notes for the next pass:
retail frame 368 with `col` at 340 copied as a **struct** through `at` (`lw at,0(t8); sw at,0(s1)`, s1 = &col
kept), `v[3]` at 324, and `x + x` (not `2.0f * x`). Unused locals declared *above* a used one keep slots; trailing
unused ones are dropped, so the 300 untouched bytes need a used low local.

## What generalises

1. **A compiled-out check moves an `la` to a later line.** Hoisted addresses are emitted at the `.loc` of the block
   they start in; an `if (x) {}` after the first statement puts the remaining address loads on a later line, which
   is as1's tie-break (func_80092278, found by editing `.loc`s in the listing first).
2. **Colour-priority swaps between two webs are closed by a block, not a variant.** When `force.sh` with two
   swapped colours gives identical code, add one empty `if (…) {}` inside the live range of the web that must
   lose: it raises `nocs` and lowers `save = totalsave / nocs` (func_800E1F80, both an FP and an int pair at once).
3. **A constant shared between two stores is a field-type question.** Two `li 1` in retail where we get one
   constant web: make one destination a different signedness (`s32 state` next to u8 flags); `force … =s`
   (split) is the quick proof.
4. **Loop-variable identity is a register lever.** Reusing `i` across loops can join webs that interfere with
   later loops' pointers; giving later loops `k` (as the arcade declaration list suggests) freed s0
   (sfx_position_3d). Sweep the assignment of i/j/k to loops mechanically.
5. **`x ? lo : (y ? hi : x)` vs if/else**: retail's single store after an FP select is a `?:` expression.
6. **`* 1.0f` survives only through an inlined parameter** (a literal folds in the front end).
