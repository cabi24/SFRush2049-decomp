# w17d RESULTS (wave 17)

Target: entity_render_setup (2,048 B). Start: w16h best.c (394/512 words, 171 rows, frame 184 vs 200).

| Function | Bytes | State | Deliverable |
|---|---:|---|---|
| entity_render_setup | 2048 | near-miss: 68 rows (words), ops 26 (was 171/60), frame 184/184 (was 200), body 518/512 | `w17d/entity_render_setup/best.c` |

Final unit score (current tree, `--neighbours`):
```
$ TAG=w17d cloud/work/frontier/tools/trace/us.sh entity_render_setup cloud/work/frontier/w17d/entity_render_setup/best.c
best.c: FAIL entity_render_setup: 420 of 512 words differ; compiled body is 518 words, target 512; +0x354: .rodata+0xd | words 68 ops 26 norm 26 | frame 184/184
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w17d score entity_render_setup --with cloud/work/frontier/w17d/entity_render_setup/best.c --neighbours
  FAIL entity_render_setup: 420 of 512 words differ; compiled body is 518 words, target 512; +0x354: .rodata+0xd0: retail words encode 0x39041821, outside the image; +0x3b4: .rodata+0xd4: retail words encode 0x3908004c, outside the image (+11 more)
  locked bodies that differ in this unit: 0
blob_unit score: 0/1 equal; object build/blob_unit/w17d/unit.o (3.8s)
```
(The 420 and the .rodata notes are the 6-word misalignment; ~26 of the 68 aligned rows are own-rodata
`lui at`/`lwc1 (at)` relocation rows, not real differences.)

Arcade ancestor: none for the body (w16h's search confirmed; arcade hud.c AnimateMarker is a 2D blit). The
signature is the arcade visual-callback shape `(Visual *v, S16 op)` (visuals.c), proven by the s16 home store.

## Step 1 (frame fixed, 171 -> 106 rows): `view`, `camera`, `size` are not named locals
Spill-temp census (`spill.sh`): our 4 coloured expression-web slots (webs 25/39/105/139/146/231/252/297) sit
below the 132-byte cfe area; camera's spill at 60 is temp1, basis at 52 is temp3. Retail's camera@56 and
basis@48 are the same temp1/temp3 with a cfe area 12 bytes smaller: retail has no named `view`, `camera`,
`size` locals (all three propagated expressions; size = `view->scale` expression web spilled to its temp).
Writing `D_80150B70[type].` / `D_8017A510[type].` directly: frame 184/184, rows 171 -> 106.

## Step 2 (106 -> 123 words but structurally right): second parameter is `s16 op`, read once
The `sw a1,188(sp)` entry store is IDO's s16-parameter home (cfe stores the int and reads back a short;
locked render_mode_select / func_8010E828 and retail anim_state_update = arcade AnimateTire(Visual *v, S16 op)
show it). A pointer formal never produces it (tested `if (vehicle) {}` before its assignment: no store); an
unused `s16 op` gives no store; `s16 op` with one compiled-out read `if (op) {}` gives exactly retail's bare
store (no sll/sra). So the callback is arcade-shaped `(Effect *, S16 op)` and `vehicle` is a named local
(retail slot 176). That costs a slot, so `gain` is not a variable (2/pi literal written per use; retail loads
a separate 2/pi literal per arm anyway) and `len` moves below `x` (named slots then line up exactly:
record 180, vehicle 176, quad 128, scale 124, type 120, i 116, color 112, delta 100, ... slope 76, threshold 72).
Frame 184/184; word count rose only because vehicle/D_8014A110 swapped a1/a2 (traced next).

## Step 3: the read is `op;` at the top (105 rows)
`if (op) {}` anywhere in block 0 rotates ugen's temp ring by two (the dead sll/sra pair), and at the end it
keeps a1 (op) live, so vehicle loses a1 to a2. The expression statement `op;` (also `op = op;`) at the top
gives the bare `sw a1,188(sp)`, no ring rotation and vehicle in a1: the first 0x170 bytes now match exactly.
Shaping device (disclosed): the compiled-out read `op;`.

## HUD FP colours are colour-only (force.sh)
`force.sh k2 "p1:w143=c27,p1:w365=c28,p1:w140=c29"` (yaw $f14, 2/pi constant $f16, radius $f18) clears every
HUD FP-register row. Ours colours by priority const 1.5 > radius 1.4 > yaw 1.2; retail needs yaw first.

## Step 4: single `angle` variable, draw-loop order x,y,z, laundered view pointer (94 rows, ops 26)
- `yaw` is not a variable: one `angle` (call result $f0, split piece $f14 via the B31 `mov.s`) gives the same
  code and frees a named slot, so `gain` is a variable again (per-arm `gain = 2/pi`).
- Draw loop source order x, y, z (as1 then puts z's store between y's load and sub): ops 59 -> 34.
- `.noalias $3,$sp` on the view address let as1 hoist `view->scale` above the slope store; writing the view
  address laundered, `((View *)(u32)&D_8017A510[type])->`, drops the noalias and gives retail's arm order:
  ops 34 -> 26 (the w9 "launder via (T *)(u32)p" technique).

## Step 5: the HUD angle and the world-path scale are ONE variable (74 rows)
Retail's world scale is spilled to 124 (the slot after quad) and lives in $f14 -- the colour of the HUD
angle's split piece. Making the world path reuse the HUD `angle` variable (declared at the old `scale` slot)
fixes every HUD FP colour (yaw/angle $f14, 2/pi $f16, radius $f18) and the world scale register at once, with
no holder. The freed slot in 96..80 is `View *view` (assigned once, `view = (View *)(u32)&D_8017A510[type]`;
copy-propagated, unaccessed slot -- as every other slot in 96..80, 68, 64 is in retail).

## Step 6: `view` assigned with the other setup at the top (68 rows; size spill 52 = retail)
With `view = (View *)(u32)&D_8017A510[type];` anywhere before the HUD test (top, before delta, before either
call) instead of inside the HUD block, the scale-expression spill moves from temp3 (48) to temp2 (52) as in
retail. Remaining: palette base, &delta/&quad hoist, draw-loop and world-loop scheduling.

## Remaining residual (traced) and best next hypotheses
1. **Palette base** (3-4 rows, +1 word): retail materialises `&D_8011B558` in the mode-test block (`lui v1` +
   `addiu v1` in the bne delay slot) for the else arm; the then sub-arms rematerialise it. pretrace
   (makelivranges) shows ours has two candidates for the address: ctx 0 (ixa operand, sub-arm blocks) and ctx 3
   (ilod base, else block only), so the else LR never reaches the test block. Any earlier ilod-form occurrence
   extends the LR backwards and hoists it (tested: `((u8 *)D_8011B558)[idx + 20]` for the alpha table hoists it
   to the test block but merges the sub-arms too; `*(u32 *)&color = *(u32 *)&D_8011B558[8]` hoists it to entry).
   Data layout supports one table: D_8011B56C = D_8011B558+20 (alpha bytes), D_8011B578 = D_8011B558+32 (the
   default colour). Next: find the form whose LR is exactly {mode-test, else} (e.g. the default colour or the
   alpha table written relative to the palette in a way ugen still folds to `lui/lw %lo(D_8011B578)`).
2. **&delta / &quad argument hoist** (2-4 rows): retail sets a0/a1 for func_8008C544 in the delta block, before
   the `D_8014A110 == 6` branch; ours sets them after. Same mechanism as 1 (an address LR that starts in the
   branch block). An unrolled `delta[i]` loop is far worse (899 rows).
3. **Draw loop** (~8 rows): ugen listing is shape-identical; as1 places `andi t8`, one `sub.s` and
   `move a2,t0 / move a3,zero` one slot differently. Untested: `as1t.sh` + `asm.sh` with an edited listing.
4. **World loop** (~15 rows): FP temp ring state at loop entry (qx in $f6 vs retail $f4; retail allocation
   $f4,$f10,$f6,$f8) plus as1 placing the quad.y load before the x add. The ring state is inherited from the
   previously emitted HUD code, so fix 1-3 first and re-measure.

## What generalises
- **Spill-temp census decides the frame**: when the frame is too big, count `f_spilltemps` slots (spill.sh)
  before adding/removing named locals. Here retail's camera@56/basis@48 were temp1/temp3 of the SAME temp
  structure with a 12-byte-smaller cfe area: the pointer locals (view, camera, size) did not exist.
- **A bare `sw aN,home` at entry with no sign-extension = an S16 parameter read once by a statement that
  compiles to nothing** (`op;`). `if (op) {}` also gives the store but rotates ugen's temp ring by two (block 0)
  or keeps aN live (end of function); a pointer formal never gives it.
- **One variable across two paths sets the colour**: retail's HUD angle (split piece $f14) and world-path scale
  ($f14, home 124) are one variable. Reusing it fixed 25 FP-colour rows at once with no holder; look for a
  shared home slot when two paths' values have the same unexplained register.
- **Assignment placement of a copy-propagated pointer still matters**: `view = ...` at the top vs inside the HUD
  block changed which spill temp the scale expression got (48 -> 52), with identical code otherwise.
- **Laundering (`(T *)(u32)&g[i]`) removes `.noalias` and restores retail's load-after-store order** (w9 rule,
  confirmed again).
