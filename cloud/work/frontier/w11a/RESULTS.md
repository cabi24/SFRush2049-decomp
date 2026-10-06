# Wave 11, lane w11a: nearest near-misses

Date 2026-10-06. Builder scratch `watchman2:~/rush2049/scratch/frontier/w11a`, copied from `base`. I rsynced
src/blob, include, tools/cloud, asm/us/blob and `blob_matched.lock.json` from the Pi. The traced `uopt`/`ugen`
binaries were copied from w10b's build into my scratch. I used at most 2 cores. Nothing was committed, spliced or
pushed, and nothing outside this directory and `cloud/matches/` was edited. No permission denials.

All unit scores come from the Pi with `python3 -m tools.conveyor.pipeline.blob_unit --tag w11a score …`.

| Function | Bytes | State | Flags | Scorer output (exact) | File |
|---|---:|---|---|---|---|
| `func_80109A60` | 1,268 | **strict MATCH** (was 26 words) | `-g0 -O3 -mips2 -G 0 -non_shared` | `score.py fn`: `MATCH`; unit: `EQUAL func_80109A60: 317 words (kept, c_func_80109A60.c)`, `locked bodies that differ in this unit: 0` | `cloud/matches/func_80109A60.c` |
| `camera_update_a` | 128 | **strict MATCH** (was 12 rows) | `-O3` (also MATCH at `-O2`) | `score.py fn`: `MATCH`; unit: `EQUAL camera_update_a: 32 words (kept, c_camera_update_a.c)`, `locked bodies that differ in this unit: 0` | `cloud/matches/camera_update_a.c` |
| `menu_back` | 132 | **provisional** (body EQUAL; caller unmatched) | `-O3` | unit: `EQUAL menu_back: 33 words (internal, c_best.c)`, `locked bodies that differ in this unit: 0` | `menu_back/best.c` |
| `entity_process_main` | 1,424 | 2 words off (was 3) | `-O3` | `FAIL entity_process_main: 2 of 356 words differ`, `locked bodies that differ in this unit: 0` | `entity_process_main/best.c` |
| `func_800E7A98` | 172 | 14 words off, body length now right (was 28 words, 42-word body) | `-O3`, unit | `FAIL func_800E7A98: 14 of 43 words differ` | `func_800E7A98/best.c` |
| `func_800CBF2C` | 276 | not attempted beyond a check (57/69, unchanged) | — | `FAIL func_800CBF2C: 57 of 69 words differ; compiled body is 68 words, target 69` | — |

Joint check of both matches:
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w11a score func_80109A60 camera_update_a \
    --with cloud/matches/func_80109A60.c --with cloud/matches/camera_update_a.c --neighbours
  EQUAL func_80109A60: 317 words (kept, c_func_80109A60.c)
  EQUAL camera_update_a: 32 words (kept, c_camera_update_a.c)
  locked bodies that differ in this unit: 0
blob_unit score: 2/2 equal; object build/blob_unit/w11a/unit.o (3.4s)
```
Builder (`score.py fn cand/NAME.c NAME --flags "-g0 -O3 -mips2 -G 0 -non_shared"`): `func_80109A60: MATCH` and
`camera_update_a: MATCH`. At `-O2`, camera_update_a also prints `MATCH`, but func_80109A60 prints
`313/317 words differ`.

## Integration notes

- Both matches are plain kept singles: `splice_singles.py func_80109A60 camera_update_a` (flags on line 1). They
  need no unit_overrides entries and supersede no group. Neither has own rodata.
- func_80109A60 defines a `static` copy of arcade `Hidden`, which umerge inlines. A kept `Hidden` already exists in
  `src/blob/state_update_global.c`, so the copy must stay `static`. The callee `stat_race_update` is only
  declared here and is not worked on.
- `menu_back` must not be spliced. Its only caller, `func_800CBF2C`, is unmatched and blocked by
  `draw_ui_element` and `drone_set_catchup`, and both of those are blocked by `AdjustSpeed`. This is a long
  chain, so I did not attempt it this wave. When func_800CBF2C matches, menu_back's body is
  `menu_back/best.c` (the `if (n) { }` line).

## func_80109A60: MATCH (26 → 0 words)

I started from w10b's best. w10b's force oracle had shown the 26 words were two colour ties. Both closed:
1. **multu operand order.** `map_height = (s32)(world_height * (u32)(D_801161C4 - 8)) / world_width;` and the
   mirror form. An unsigned conversion is its own cfe node and is demoted to the right-hand operand (workbench
   law L52), so world_height comes first and the multu is retail's `multu a3,a0`. The CSE temp for
   `D_801161C4 - 8` survives (26 → 24). `(s32)((u32)world_height * (D - 8))` gives the same result. Plain
   `D - 8U` and the swapped plain order give no change.
2. **size/count tie.** A compiled-out `if (size) { }` goes where w7b's artificial `if (flash) { }` was, which
   was a read of an uninitialised local. The traced uopt shows the size web at save 8/7 = 1.14 over 7 blocks,
   above the D_80151AD0 PRE web at 1.0, so size gets t3 and the count t4 (24 → 0). Two other forms are also
   EQUAL: `s16 size = 0;` (keeping the flash check), and a dead `size = 0;` / `size = blt->Height;` at the top.
   I kept the `if (size)` form because it removes the uninitialised read.
   - Moving the real `size = blt->Height` assignment later also gets the colours right (2 rows), but as1 then
     cannot schedule the load into block 0.

## camera_update_a: MATCH (12 rows → 0)

This is a recursive in-order walk: children first, then `fn(n, *count)`, `(*count)++`, then the next sibling.
Written as plain double recursion (`if (n) { f(child); fn; ++; f(n->next); }`), uopt eliminates the tail call
and builds the loop itself. That gives retail's colouring (count s0, n s1) and the entry test on s1. Every
hand-written loop colours n first, because its traced saves are n 25.5 and count 20. Three spellings of the
recursive form are EQUAL: `if (n == 0) return;`, `if (n != 0) {…}`, and `*count += 1`.

## menu_back: provisional, body EQUAL (3 → 0 words)

The fix is a compiled-out `if (n) { }` right after `n = inflate_entry_alt(...)`. It also works between the
count store and the call, but not after the call. With it, ugen copies n into a1 before it reloads m->buffer
into a0, and as1 then puts `lw a0,76(s0)` into the jal delay slot as in retail. Casts and `n + 0` spellings did
not move it (8 variants, `menu_back/v1/`).

## entity_process_main: 2 words (was 3)

The clear arm is now `poly = &D_8015B268[v->objnum]; poly->flags = ~(1 << i) & poly->flags;`, which no longer
writes i. The shift goes to a ring temp (`sllv t9,t8,a0`) as in retail.

- **Residual:** ugen emits shl, not, load (`nor t6` / `lhu t7`), where retail emits shl, load, not
  (`lhu t6` / `nor t7`).
- **About 150 variants** (`entity_process_main/v1`–`v8`). Every plain tree form lands in one of two states:
  - mask operand first gives shl, not, load (2 words);
  - load first gives load, shl, not (18 words). This covers `&=`, the A-form, every cast placement (48-form
    sweep, `v5`), a volatile load (even on the right-hand side), and the inlined `clr()`/`bit()` helpers
    (frame compensated by dropping `pad1`/`xlu`).
- **Other forms are worse:** CSE forms colour the shift (v1), and the de Morgan forms change the opcodes.
- **Next hypothesis:** retail's interleaving is not one `and(load, not(shl))` tree. Look for a shape where the
  shl is an earlier ugen expression but not a uopt web, for example a block-local temp from a different
  statement split. Tracing the traced ugen free list (`tools/ugt.sh`) on such a candidate shows the order
  directly.

## func_800E7A98: 14 words, 43-word body (was 28 words, 42-word body)

A goto loop (`p = next; loop: if (p) { next = p->next; if (next == h) {…} else { p = next; goto loop; } }`) gives
retail's non-likely `bnez; move v1,a0` tail and the right body length.

- **Residual:** retail keeps h in v0 and next in a0, with a separate a1 copy of h (`move a1,v0`) made at the
  join, before the loop.
- **Force oracle:** h=v0, next=a0, p=v1 (`p1:w6=c1,p1:w9=c3,p1:w14=c2`, proc 839) puts the copy at the call,
  not at the join. So retail has a second web, the IPA argument, starting at the join.
- **Tried without movement:** every copy-variable spelling is copy-propagated away. That covers `addr = (u32)h`,
  `heap = h` (both directions), no-op argument expressions, compiled-out checks at every position (69-variant
  sweep), and a goto/break exit.

## What generalises

1. **Compiled-out `if (x) { }` is a priority lever.** It adds a use to x's web (tot +1) without code. It closed
   two functions this wave: func_80109A60 broke a save-1.0 tie, and menu_back changed ugen's argument order by
   turning n into a coloured web. Trace first (`ctrace.sh`), and check whether the losing web's save is a tie
   or close to one.
2. **Self-recursion on the last statement is a loop in retail.** A small tree walk whose loop colours the wrong
   s-registers may be double recursion that uopt tail-call-eliminated. Try
   `f(child); visit; f(next);` before tuning loop forms.
3. **Use unsigned casts to set multiply operand order.** `(u32)` on one factor demotes it to the right-hand
   operand, and `(s32)(...)` around the product keeps signed division. This worked even when a CSE temp was
   involved, where source order had no effect.
4. **ugen `and(load, not(shl))` order is binary in tree form.** It follows operand order, and a volatile load
   always goes first. Interleaved retail orders need a different statement shape.

## Tools (lane `tools/`)

- Copies of w10b's tools, retargeted to w11a: `ctrace.sh`, `force.sh`, `pdiff.sh`, `us.sh`, `sc.sh`, `fr.sh`,
  `udiff.py`, `udiffm.py`, `ugt.sh`, `ugt_remote.sh`.
- `vgen.py BASE OUT variants.py` writes the variant files (OLD → V dict).
- `vu.sh NAME "args" files…` prints the unit score and aligned rows for each variant.
- Proc ordinals in the current tree: func_80109A60 965, func_800E7A98 839, camera_update_a 228.
