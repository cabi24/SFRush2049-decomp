# func_80109A60 — minimap dot callback (arcade `game/hud.c:AnimateDot`) — w7b fork

**State:** NOT a match. Structurally complete; residual is register colouring (lane: uopt colouring
order/priority). 317 target words.

**Flags:** `-g0 -O3 -mips2 -G 0 -non_shared`, whole-program only (Hidden must be inlined by umerge).

## Scores (quoted)

```
# whole-program unit (Hidden made static in best_unit.c: state_update_global.c already defines a kept Hidden)
python3 -m tools.conveyor.pipeline.blob_unit --tag w7bdot score func_80109A60 --with cloud/work/frontier/w7b/func_80109A60/best_unit.c
  FAIL func_80109A60: 304 of 317 words differ; compiled body is 309 words, target 317
python3 cloud/work/frontier/w7b/func_80109A60/tools/udiff.py func_80109A60 --mnem
  want 317 words, got 309; differing rows 35
# single-file -O3 group (keep = func_80109A60, Hidden internal)
sh tools/full.sh best.c func_80109A60 --keep func_80109A60 --mnem   -> want 317 words, got 310; differing rows 36
# plain score.py fn (Hidden NOT inlined standalone, so meaningless here)
score.py fn best.c func_80109A60 --flags '-g0 -O3 -mips2 -G 0 -non_shared'  -> 312/317 words differ
```
`natural.c` (no artificial block): `--keep` mnem rows 52.

Starting point (prior packet `module_campaign_20261002/.../animate_dot_hidden.c`): 315/317 words, frame 32.

## What was found (each moved the opcode-aligned diff)

1. **Hidden is arcade `hud.c:Hidden`, defined in the file and inlined** (umerge). Calling the kept
   `input_new_data_wrapper` (0x80094F88) leaves a `jal` in the unit — retail inlines a same-file copy,
   exactly as `state_update_global` does. Landing needs the Hidden definition `static` or a
   `prefer_definition`, since `state_update_global.c` already defines a kept `Hidden`.
2. **Frame 40 and `slot` at sp+26 come from declaration order**: s16 locals always get homes, laid
   out from the top in declaration order. Arcade `MODELDAT *m; CAR_DATA *c;` declared **first**,
   then `S16 b, flash, slot;` then `s16 size;` gives retail's 40-byte frame and slot home 26.
   (32-bit locals declared after the s16s do not grow the frame.)
3. **`slot` is not register-allocated in retail** (store + immediate reload, then `lh` per use). In
   the trace slot's web (raw10 0xfffffff2) has save 8/8 = 1.0 and is coloured; one more block in
   its range (save 0.889) makes uopt split it -> retail's memory-resident slot. Forcing
   `p1:w0=s` (split) is the oracle. The block used in best.c (`if (flash) { }` after the flash
   computation) is artificial; `if (blt->X < 0) { }` before flash also works (43 rows). No natural
   source found yet.
4. With slot in memory, `m`/`car` pointer variables (`m = &D_8014A250[slot]; car = &D_80152818[slot];`
   after the Hidden test, arcade order) and direct indexing compile identically (copy-propagated):
   retail recomputes the model address in the flash chain and again for `mode`.
5. **Both centering offsets are named locals computed before the X/Y stores**
   (`xo = (D_801161C4 - map_width - 8)/2 + 4; yo = ...`), then `blt->X = origin.x + xo - (H + D_801161C4)/2`
   (global re-read for Y after the X store, as retail).
6. **`blt->X += term` (not `blt->X = blt->X + term`)** gives retail's `add.s fd, X, term` operand order.
7. **Tail is `if (slot < D_801543CA) { alpha; mode==2/1 -> stat_race_update } else return Hidden(blt, 1);
   Input_ApplyPadConfig(blt); return 1;`** — retail places UpdateBlit + `return 1` after the else.
   (-7 rows.)
8. stat_race_update(blt, slot, H, H) for mode 2 (human) and (blt, 7, H, H) for mode 1 (arcade
   SelectBlit(blt, 1+slot / 0, 7, 7)); alpha `(tick & 8) && flash ? 64 : 255`.

## Residual (35 rows, all register names / one `move`)

Retail colours `blt` (param web w2, save ≈2.36–2.45) to **t0** with `move t0,a0` and spills to its arg
home sp+40; ours keeps it in **a0**. Forcing `p1:w2=c7` (t0) in the colouring oracle cuts the
exact diff a lot but exposes the next permutation: world_width a0 vs retail a1, xo a1 vs retail a0
(both save 2.0, tie broken by web number: ww w52 < xo w85), H/count t3/t4 swapped, map_width/size
v0/v1 swapped, temp ring starts t6 vs t7. Retail also uses `size-8` as a separate temp web copied
into map_width/map_height (`move v1,a0` / `move t1,a0`). This looks like retail has a different
block count / web priority overall, not a single lever.

Tried without movement (on the r3 base): alpha as if/else, `!(hit_target == -1)`, `size` as s32 /
moved declaration, xo/yo declared first, explicit Hidden bodies instead of the helper (worse: 52),
separate `Blit *blt = arg` copy (copy-propagated, identical), volatile slot (every use reloads — wrong).

**Best next hypothesis:** find the real source of the extra block(s): candidates are an N64 remnant
of the arcade `slot == this_node` local-car case or the `mpath_edit || gFlyMode` terms of the flash
predicate as compiled-out globals/macros, placed so that both slot (needs save < 1) and blt
(needs save < 2.0 so xo can take a0 first) lose priority. Run the colouring trace (`tools/ctrace.sh
func_80109A60 <unit-cand> LABEL 910`; proc ordinal 910 in tag w7bdot) after each candidate.

## Recovered layouts
- Blit (hud/blit LIB layout as in state_update_global): Hide s8 +0x1A, Alpha +0x18, Height +0x16,
  X/Y +0x0E/+0x10, AnimID +0x2C, render slot u16 +0x34.
- Car (D_80152818, stride 952): position f32[3] +8, dead s8 +856, active s8 +857.
- Model (D_8014A250, stride 2056): crash s8 +1600, hit_target s16 +1732, mode s8 +1996, hide s8 +2015,
  collidable s8 +2027.
- D_80142DB4 s8[] dot order list; D_80151AD0 s16 view count; D_80156BDC s8 map enable; D_801407B4/
  D_801407D4 world max/min {s16 x,y,z}; D_801161C4 s32 map size; D_801160A8 {s32 x,y}[] per-view
  origin; D_80140A04 s8 mirror; D_80140BF0 32-byte render records (flags +21); D_801543CA s16 active
  count; D_8002E8E8 clock record (tick u32 +636).

Files: `best.c` (single/`--keep` form), `best_unit.c` (static Hidden for blob_unit), `natural.c`
(no artificial block), `sw/` (all variants), `tools/` (w7b tools retargeted to tag w7bdot,
`udiff.py --mnem` added).
