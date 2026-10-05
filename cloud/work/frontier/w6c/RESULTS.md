# Frontier wave 6 — agent w6c results

Builder scratch `~/rush2049/scratch/frontier/w6c` (copied from `base` 2026-10-05; `uopt` -> `../w3a/uopt`,
`uopt5` -> `../w5d/uopt`). Unit tag `w6c`. Nothing committed, spliced or copied into `src/blob`, `asm/`,
`tools/`, `include/` or `cloud/matches/`. func_80087110 and stat_race_update/func_800FE5B0 not touched.
Tools: `tools/` = the w5c set retagged (`sc.sh`, `u.sh`, `ub.sh`, `grp.sh`, `full.sh`, `bs.sh`) plus the w5d
pre-colouring tracer retargeted (`pretrace.sh`, `pforce.sh`, `prereport.py`) and `udiffm.py` (unit aligned diff with
`--mnem`: registers normalised, which is the useful view above ~300 words).

| # | Function | Bytes | State | Best source |
|---|---|---:|---|---|
| 7 | `func_800EF62C` | 712 | **code identical, own literals unverified** (scorer `NOT VERIFIED`, unit `EQUAL`) | `func_800EF62C/best.c` |
| 1 | `mode_select_input` | 152 | **provisional**: `EQUAL` in the unit with stand-in caller chain | `mode_select_input/best_unit.c` |
| 1 | `func_800DFBA0` | 1,192 | provisional lane, 24/298 words (int colouring only) | `mode_select_input/best_unit.c` |
| 3 | `func_800C8918` | 628 | 21/157 words in the unit (one loop + temp ring) | `func_800C8918/best.c` |
| 6 | `func_80109468` | 1,528 | 249/382 words; 12 normalised rows (register colouring only) | `func_80109468/best.c` |
| 2 | `func_800E847C` | 2,100 | 232/525 words; 21 normalised rows (s-register rotation, loop counter piece) | `func_800E847C/best_group.c` |
| 2 | `func_800E7FA0` | 1,244 | 38/311 words with the **real** caller (was 40 with a stand-in) | same file |
| 4 | `func_800D6914` | 1,260 | provisional lane, first draft, 47 normalised rows | `func_800D6914/best.c` |
| 5 | `func_800DA2C0` | 2,316 | not drafted (classified only; provisional-only, see below) | — |

No strict MATCH this wave. One code-identical body (`func_800EF62C`), one function `EQUAL` only with
stand-ins (`mode_select_input`), and five near-misses whose residual is register colouring.

---

## func_800EF62C — code identical, own literals unverified

Split-screen HUD tachometer Blit AnimFunc (N64-only; arcade `AnimateTach`/`AnimateNeedle` in game/hud.c draw a
needle object instead; the `rpm * 1.35` "fake rpm boost" is the arcade constant). Sibling of the locked
odometer AnimFunc `func_80106D94` — same Blit layout (X 0x0E, Y 0x10, Width 0x14, Height 0x16, Alpha 0x18,
Hide 0x1A, Top/Bot/Left/Right 0x1C..0x22, AnimDTA 0x28, AnimID 0x2C) and view-slot conventions. Semantics in
the file header.

```
tools/sc.sh func_800EF62C/best.c func_800EF62C --flags '"-g0 -O3 -mips2 -G 0 -non_shared"'
func_800EF62C:
  NOT VERIFIED (8 section-relative relocations unverified: .rodata+0x0 at +0x110, .rodata+0x0 at +0x118,
  .rodata+0x10 at +0x148, .rodata+0x10 at +0x14c, .rodata+0x20 at +0x214, .rodata+0x20 at +0x218,
  .rodata+0x24 at +0x220, .rodata+0x24 at +0x224; own .rodata: this function's references disagree on the
  section's image address (0x80120D9C for +0x0/+0x10, 0x80124584 for +0x20/+0x24): the literals are not laid
  out as in retail)
python3 -m tools.conveyor.pipeline.blob_unit --tag w6c score func_800EF62C --with cloud/work/frontier/w6c/func_800EF62C/best.c
  EQUAL func_800EF62C: 178 words (kept, c_best.c)
```
All 178 words equal; the only open item is literal ownership. The object's single `.rodata` holds the two
strings and the two floats; retail has the strings in the TU data area and the floats in the per-function
literal pool, so the scorer cannot assign one base. Bytes checked by hand (object `objdump -s -j .rodata` vs
`build/game_code.bin`): `"TACHOMETER_MD\0\0\0"` = 0x80120D9C, `"TACHOMETER_SM\0\0\0"` = 0x80120DAC,
`3FACCCCD` (1.35f) = 0x801245A4, `38D1B717` (0.0001f) = 0x801245A8. **Integrator:** this is the first function
with both own strings and own floats; whether `blob_splice`'s own-data placement handles two windows for one
`.rodata` section is untested. Not put in `cloud/matches/` because CI rescoring would see `NOT VERIFIED`.
Shaping: Right/Top/Bot stores in that order (the other five orders permute t8/t9/t0).

## mode_select_input + func_800DFBA0 — provisional (real caller func_800E05F0 is unmatched, L3)

`best_unit.c` contains `entity_hierarchy_update` (redefined, `__inline`), `mode_select_input`, `func_800DFBA0`
and the A169 reconstruction of the caller `func_800E05F0` (stand-in quality, 300+/332 off).
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w6c score mode_select_input func_800DFBA0 \
    --internal mode_select_input --internal func_800DFBA0 --with cloud/work/frontier/w6c/mode_select_input/best_unit.c
  EQUAL mode_select_input: 38 words (internal, c_best_unit.c)
  FAIL func_800DFBA0: 24 of 298 words differ
```
- **mode_select_input** inlines `entity_hierarchy_update` with all of its statements on the *call's* line. That
  only happens when the callee is a *kept* function defined `__inline` in the same file **and no plain prototype
  of it precedes the definition** (with a prototype first umerge does not inline it at all; an internal static
  helper is inlined but keeps its own `.loc` lines, and then as1 puts `lw s1,0(s0)` before the `jal` instead of in
  its delay slot — the 2-word residual of every earlier attempt). Landing needs `entity_hierarchy_update`'s
  locked definition (`codex_transform_b109`) to become `__inline`; it still scores EQUAL as a kept function.
- **func_800DFBA0** (tyre-squeal surface mixer: per wheel, contact 0/1/2-3 → three weighted sums with
  `1-(1-f)^2` curves; the loudest layer is started/updated, the other two stopped): from 181 to 24 words by
  (a) `fraction /= n == 1 ? 2.0f : n == 2 ? 4.0f : 8.0f;` in all three cases (an `if (n == 1) f /= 2.0f` arm is
  turned into `* 0.5f` by uopt; retail divides), (b) the last call's style argument written `1` (int) — with
  `1.0f` it shares the loop's 1.0 constant web and lands in f28 for the whole function, (c) counters initialised
  by statements after the declarations. Residual (24 words, lane = phase-1 colouring): constants 1 and 2
  (save 40/15 = 2.67) beat the counters (41/16 = 2.56) to v1/a0; retail has the counters in v1/a0/a1, i a2, 1 a3,
  2 t0. Not found: a source form that lowers the constants' priority (switch on contact: worse; a deleted-static
  scale helper `func_800E0048(count)`: not inlined).

## func_800C8918 — 21/157 words (unit)

Audio/resource shutdown: frees the two handles at D_8011025C/0260, sends message types 1 and 7, busy-waits until
all 128 24-byte messages at D_80142DD8 are consumed, clears D_8011023C, `init_wait_completion()`, frees
D_80110244/0248/0270.
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w6c score func_800C8918 --with cloud/work/frontier/w6c/func_800C8918/best.c
  FAIL func_800C8918: 21 of 157 words differ
```
Found:
- The three wrappers (`audio_effect_process`, `object_type1_create`, `object_type7_create`, all kept and locked)
  are inlined into it in retail. They inline in the unit only when the file defines them `__inline` (kept) with
  no earlier plain prototype (same rule as above). Their EQUAL scores as kept bodies are unaffected.
- **Every inlined instance reserves its callee's locals in the caller's frame.** Retail's slots (192,168 / 140,108
  / 80,56,32, frame 208) mean each `audio_effect_process` instance takes 24 bytes (its parameter plus four
  more words) and each message helper 32 bytes (two pointer locals plus four more). The locked one-statement
  bodies have no such locals; with four extra `s32` locals in each `__inline` definition (stand-ins for the
  unknown originals, probably compiled-out debug code) every slot and the frame match exactly.
- Busy wait is `do { m = D_80142DD8; do { if (m->used) break; m++; } while (m != END); } while (m < END);`
  (a `for` adds the entry guard).

Residual (lane: address-constant webs + temp ring): retail evaluates END once as `lui t9; addiu v1,t9,lo`
(the `t9` hi part shifts the whole t6–t9 ring by one for the rest of the function) and copies it
(`move a1,v1`) for the outer compare, and rematerialises START (`lui v0; addiu v0`) at every outer iteration.
Ours colours START (a0) and uses one END web. Tried ~25 forms (END as `&D_80142DD8[128]`, `+ 128`, a local
`end` for inner/outer/both, `for`/`do`, the busy wait as the deleted static `func_800C8784` with and without
parameters). Best next hypothesis: END computed as `start + 128` from a pointer that is not an lda (e.g. a
pointer parameter of an inlined helper whose argument is START), giving the `lui t9` + `addiu v1,t9` form.
For landing: replace the three wrappers' locked sources by `__inline` versions (or `prefer_definition`).

## func_80109468 — 249/382 words, 12 normalised rows (single, kept)

Split-screen **minimap** Blit AnimFunc (N64-only): hidden for 5+ views or when D_80156BDC is 0; for 4 views
renames to `"MAP_MD"`; on the first update positions the blit from D_801160A8[views-1]; scales the track bounds
(max D_801407B4 − min D_801407D4, x/z) into the texture's Width (D_801161C4 = Width − 8 margin, aspect kept),
clears the 4-bit texture (`info->data`, 2 pixels per byte) and plots every main-path point
(`D_801407F0.points`, `PathGraph` from world_gravity_apply) as a 5×5 dot (inner 3×3 colour 0xE, border 0xD
unless already 0xE), mirrored in x unless D_80140A04; points of disabled side sets (found by locating
`&D_801409E8[i]` inside a set's point range) are skipped.
```
tools/sc.sh func_80109468/best.c func_80109468 --flags '"-g0 -O3 -mips2 -G 0 -non_shared"'
  249/382 words differ (2 section-relative relocations unverified: .rodata+0x0 at +0x9c, .rodata+0x0 at +0xa0)
tools/full.sh func_80109468/best.c func_80109468 --mnem   ->  want 382 words, got 382; differing rows 12
```
Found: `w = size - 8; h = dz * w / dx` (named `w`/`h`: operand order of `multu`); `row += col / 2` (a reassigned
pointer is not copy-propagated, so the pixel address is computed once at the join as retail does; a fresh `pix`
local is sunk into every arm); the border test first (`if (x <= px-2 || x >= px+2 || ...)` — retail falls through
into the border arm); `col = D_80140A04 == 0 ? size - x - 1 : x`; `&D_801409E8[i]` written inside the inner loop
condition (hoisted by uopt into the guarded preheader, as retail); `hide` declared last (no slot).
Residual (lane: colouring): retail `i` in s2 and the constant 6 in s1 (ours swapped: i 15.1 > 6's 8.3); dx/dz in
a2/a3 and the size in v1 (ours a0/a2/a1); and as1 hoists `move a0,s0` above the `beq` at the two UpdateBlit
call sites in ours but not in retail. The last two look like one cause: in retail a0 is live across those
branches (a web coloured a0 spans them). `pforce` with the retail dx/dz colours reproduces those rows.

## func_800E847C + func_800E7FA0 — real group, 232/525 and 38/311 words (unit)

Arcade ancestor of `func_800E847C` = `update_game_data` (game/mdrive.c): copy every in-game model into its
`game_car` record; `func_800E7FA0` is called where the arcade calls `CheckQuadDamage` for each quadrant (it is
the N64 per-wheel threshold/level replacement). The roll/pitch smoothing (`0.6/0.4`, `0.7/0.3` into
`func_80090F44`/`func_8009EA68`), the force sums and the curve table D_80111560[D_8011156C[body]] are N64
additions. `best_group.c` holds both functions (internal `func_800E7FA0`, kept `func_800E847C`).
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w6c score func_800E847C func_800E7FA0 --internal func_800E7FA0 \
    --with cloud/work/frontier/w6c/func_800E847C/best_group.c
  FAIL func_800E847C: 232 of 525 words differ
  FAIL func_800E7FA0: 38 of 311 words differ
udiffm.py func_800E847C --mnem  ->  want 525 words, got 525; differing rows 21
```
Found (E847C, from 520 words):
- One `Model` struct for both functions (0x808 records at D_8014A250; E7FA0's `wheel`/`flags` are the arcade
  `sviscode`/`appearance`); the loop head tests `model[i].in_game` and `gc->f359` (gc assigned first, one `||`
  test), **then** `m = &model[i];` and `m->` for the rest. Assigning `m` before the tests loses ugen's
  `.noalias m,$sp` for the force-sum block (it is emitted per basic block of the assignment) and as1 then cannot
  interleave the sums; using `model[i].` everywhere keeps the noalias but makes the sound-flag loop hoist the
  `sound_flags[k]` load out of the inner loop (retail reloads it: `m` is a plain pointer there).
- `m->f2E0[k] = 0` (int zero: a separate constant web f28 from the compare zero f24).
- Declarations `i, j, k, l, high_index, same_count[4], m, gc, temp[3], curve, value, snd_flags, bound`
  give the 184 frame, `snd_flags` at sp+134 (read before written: loaded before the loop, stored after),
  `temp` at 144, `same_count` at 164, `i` at 182.
- `curve = ...` after the sums; `temp[k] = force[0][k] + force[1][k]` then `+=` (operand order).
Residual (E847C): the six callee-saved assignments are rotated by one (retail 12/&same_count/8/m/gc/snd =
s3..s8; ours &same_count/8/m/gc/snd/12 = s3..s8 — the four tie at save 4.545), the loop counter is kept in v0
across the back edge (retail stores it and reloads it at the loop top), and one store pair order in the
collide-time block. `pforce` could not force these webs (declined).
Found (E7FA0, 40 -> 38): `curve = &D_80120EEC[i]` inside the loop instead of a pointer induction variable
(retail initialises `i`/`i*4` in the test block). Residual: retail colours car->level's induction pointer t5 and
the segment pointer/hi s0/s1 (ours s0 and t2/t3): the IV web is coloured before `lo` in retail. Declaration order
of `lo`/`segment` has no effect; a named `level` pointer IV is much worse (218).

## func_800D6914 — provisional lane, first draft (real caller func_800D7634 is L8)

Main-menu 3D buttons (strings `"BUTTON_SELECT"`/`"BUTTON"`): four 64-byte button records at D_801102B4 plus
their four shadows (+4) and a ninth spinning object. The selected button (D_80110638) turns towards π at 4π/s,
the others back to 0 (snapped when D_801105B8 == 1); each is reset to the identity, rotated by `angle − π/2`,
placed at y = 100 − 40·i (eased by 180/s), copied to its shadow, textured by angle > π/2, and pushed to the
two models. The ninth object spins at 2π/s and follows the selected button's height with an adaptive speed
D_801543D0 (clamped to [180, 540]·D_801105BC), then is scaled by 0.4.
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w6c score func_800D6914 --internal func_800D6914 \
    --keep standin_caller --block func_800D6914 --with cloud/work/frontier/w6c/func_800D6914/best.c
  FAIL func_800D6914: 282 of 315 words differ; compiled body is 320 words, target 315
udiffm.py func_800D6914 --mnem  ->  want 315 words, got 319; differing rows 47
```
Found: the frame time D_8002EB94 is `volatile` (retail reloads it at every use, even with no store between);
array indexing `D_801102B4[i]` (gives retail's four pointer IVs s0/s2/s5/s6); locals are laid out bottom-up
in declaration order — `pos[3]` at 76, `sel_y` at 68, `index` at 62 need two more words declared after `index`
and fourteen before `pos` (fillers in the draft; the originals are unknown). Residual: FP constant colouring
(π, 180, 0), `&D_801105B8` not coloured (retail s7) while `&index` is (ours s8), and the speed clamp: retail
recomputes `diff * 60 / 10` in the then-arm (no CSE) and stores through `?:`-like arms — a MIN/MAX macro with
the expression as an argument is the likely source.

## func_800DA2C0 — not drafted

Sibling of D6914 (same callees, same volatile frame time, the same adaptive-speed clamp with literal 540/180,
again recomputing `|d| * 60 / 10`); called once from `func_800DB1E0` (L8), unsaved s0–s8, so provisional at
best. Skipped for breadth after D6914; start from D6914's structure and conventions.

---

## What generalises

1. **Kept `__inline` functions are how retail inlines 3-5 statement wrappers.** In the unit, `umerge` inlines a
   *kept* (locked, externally visible) function into callers in the same file when it is defined `__inline` —
   but only if no plain prototype of it appears before the definition (one `void f(...);` line disables it),
   and the kept body is still emitted (and still scores EQUAL). In the A168 listing the inlined statements carry
   the call's `.loc`; an internal static helper is inlined with its own `.loc` lines instead, which changes
   as1's tie-breaks (the `lw s1` / `move a0` delay-slot pick in `mode_select_input`). This closed
   `mode_select_input` and made `func_800C8918`'s inlined wrappers reproducible. Landing needs the locked
   wrapper sources made `__inline` (or a `prefer_definition` entry).
2. **An inlined callee's locals take slots in the caller's frame, per inlined instance.** Gaps between spill
   homes of consecutive inlined calls give the callee's local count (24 bytes = parameter + 4 words, 32 = 2
   pointers + 4 words in C8918) — use them to size frames instead of guessing pads.
3. **`.noalias reg,$sp` is per basic block of the pointer's assignment.** If retail keeps a pointer's loads
   moving freely across stack stores in a later block, the pointer is assigned (again) at the start of that
   block — e.g. tests on `array[i].x` first, `p = &array[i]` after the last `continue`. Using `array[i].x`
   everywhere gives noalias everywhere, which is wrong where retail reloads through the pointer.
4. **`x /= c` vs `?:` divisors:** uopt rewrites `x /= 2.0f` as `x * 0.5f`; retail's `div.s` by a register 2.0
   means the divisor is a `?:` (`n == 1 ? 2.0f : n == 2 ? 4.0f : 8.0f`).
5. **Volatile globals show up as reloads with no intervening store** (frame time D_8002EB94 in the menu code).
6. **A float argument written `1` (int) is a different constant web from the loop's `1.0f`** — the cure for a
   constant pinned to an argument register (f28) for the whole function.
7. Literals: a function can own both strings (TU data area) and floats (literal pool); the scorer reports this
   as `NOT VERIFIED` because one `.rodata` maps to two image windows. Verify the bytes by hand.
