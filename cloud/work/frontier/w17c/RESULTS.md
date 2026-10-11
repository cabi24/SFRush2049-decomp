# w17c RESULTS (wave 17): anim_state_update

| Function | Bytes | State | Flags | Deliverable |
|---|---:|---|---|---|
| anim_state_update | 1840 | **MATCH** (unit EQUAL; `score.py group` MATCH, own .rodata verified) | `-g0 -O3 -mips2 -G 0 -non_shared` (group flags add `-Xcpluscomm -Wab,-r4300_mul`) | `groups/codex_visual_tire_module_20261002/` + `groups/func_8008B640/` |

Start: w16h `anim_state_update/best.c`, re-measured in the current unit: `FAIL anim_state_update: 39 of 460 words
differ | words 21 ops 2 norm 2 | frame 224/224` (8 of the 21 rows are unresolved own-rodata relocations).

## Evidence

Whole-program unit (Pi repo root, tree at 66fe4d4f, deliverable files only):
```
$ G=cloud/work/frontier/w17c/groups
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w17c score anim_state_update matrix_scale_apply model_bounds_calc \
    physics_velocity_integrate_a physics_velocity_integrate_b physics_velocity_integrate_c physics_velocity_integrate_d \
    physics_velocity_integrate_e physics_velocity_integrate_f func_8008B640 entity_anim_texture \
    --with $G/codex_visual_tire_module_20261002/anim_tire_donor.c --with $G/codex_visual_tire_module_20261002/visibility_wrappers.c \
    --with $G/codex_visual_tire_module_20261002/entity_anim_texture.c --with $G/func_8008B640/group.c --neighbours
  EQUAL anim_state_update: 460 words (kept, c_anim_tire_donor.c)
  EQUAL matrix_scale_apply: 25 words (internal, c_visibility_wrappers.c)
  EQUAL model_bounds_calc: 18 words (internal, c_visibility_wrappers.c)
  EQUAL physics_velocity_integrate_a: 178 words (internal, c_group.c)
  EQUAL physics_velocity_integrate_b: 72 words (kept, c_group.c)
  EQUAL physics_velocity_integrate_c: 66 words (kept, c_group.c)
  EQUAL physics_velocity_integrate_d: 66 words (kept, c_group.c)
  EQUAL physics_velocity_integrate_e: 66 words (kept, c_group.c)
  EQUAL physics_velocity_integrate_f: 66 words (kept, c_group.c)
  EQUAL func_8008B640: 23 words (internal, c_group.c)
  FAIL entity_anim_texture: 143 of 159 words differ; compiled body is 154 words, target 159
  locked bodies that differ in this unit: 0
blob_unit score: 10/11 equal; object build/blob_unit/w17c/unit.o (4.6s)
```
entity_anim_texture is an unmatched frontier function (context in the tire group). It is `143 of 159` in the
baseline unit too, so the argument swap does not change it.

Standalone groups (builder `~/rush2049/scratch/frontier/w17c`, `python3 tools/cloud/score.py group cand/groups/<name>`):
```
codex_visual_tire_module_20261002
Members:
matrix_scale_apply:
  MATCH
anim_state_update:
  MATCH
    own .rodata verified at 0x8012393C..0x8012394C

Context (informational; excluded from exit status):
model_bounds_calc:
  MATCH
entity_anim_texture:
  148/159 words differ
func_8008B2B4:
  12/12 words differ
func_8008B2E4:
  MATCH

func_8008B640
Members:
model_bounds_calc:  MATCH
physics_velocity_integrate_b..f:  MATCH (each)
func_8008B640:  MATCH
physics_velocity_integrate_a:  MATCH
```
(The locked tire group scores the same context lines: entity_anim_texture 148/159, func_8008B2B4 12/12,
anim_state_update 429/460.)

## Integration notes
- **Supersede two locked groups, same names:**
  - `src/blob/groups/codex_visual_tire_module_20261002/` <- `w17c/groups/codex_visual_tire_module_20261002/`.
    Members: matrix_scale_apply (old) and anim_state_update (new, claim). Context: model_bounds_calc,
    entity_anim_texture, func_8008B2B4, func_8008B2E4 (new: an unchanged copy of `src/blob/func_8008B2E4.c`, i.e.
    Random, which umerge inlines into anim_state_update). Keep: anim_state_update, entity_anim_texture,
    func_8008B2E4. The files are `anim_tire_donor.c` (now the new anim_state_update body, `= w17c/anim_state_update/best.c`),
    `rand.c` (unchanged), `visibility_wrappers.c`, `entity_anim_texture.c`, and `func_8008B2E4.c`.
  - `src/blob/groups/func_8008B640/` <- `w17c/groups/func_8008B640/`. The only change is the parameter order of
    model_bounds_calc, in three lines: the prototype, the definition and the call in physics_velocity_integrate_a.
    All 8 members still MATCH. `claims: []`, and a note was added.
- **Why both groups:** a callee's parameter order is global to the unit. Every caller has to agree:
  matrix_scale_apply in visibility_wrappers.c, entity_anim_texture, physics_velocity_integrate_a, and the
  `__standin_model_bounds_calc(0, 0)` stand-in (its call is unaffected).
- Splice claims anim_state_update: **+1,840 B game**. No unit_overrides needed in the run above. func_8008B2E4 has
  two definitions (the locked single and the group's context copy), handled the same way as in frontier_mode_select.
- **Data symbols:** none changed. The draft's `D_8015978C` is `D_8014978C` (`lui 0x8015; lb -26740`), as w16h found.
  The new body's header declares D_8014978C. visibility_wrappers.c still declares the unused `D_8015978C`;
  I left it as it was.
- Own .rodata: 0.6, 0.05, 0.002 and 3.7 at 0x8012393C..0x8012394C, verified by `score.py group`.

## Disclosures (also in the file header)
- `s16 i, j` are unused locals, taken from arcade AnimateTire's declaration list. They are the exact
  home-layout residual:
  - Retail homes: tire@138, slot@132, row@130, mph@128, lucent@124, controller@120, config@116.
  - Without i and j the homes move by 4. The frame stays 224 because of 8-byte rounding, and the unit diff has
    30 rows (8 of them relocations).
  - With only `i` there are 20 rows.
  - i and j are two 2-byte slots.
- The translucency flag is computed in `show` and then copied to `lucent`; `show` is reused for the visibility
  flag. This is a source form, not a compiled-out device.
- There are no compiled-out reads, no stand-ins, no implicit int, no pad arrays and no false prototypes. The
  model_bounds_calc prototype change restores the arcade order: `CheckVisible(Visual *v, BOOL on)`,
  visuals.c:2470. The body is the same: show or hide the object, then return `on`.

## How it closed (trace, 39 -> 2 -> 0 words)
1. **The lucent piece.** I built w16d's split-instrumented uopt in my scratch (`tools/patch_spl.py` ->
   `bin/uopt_spl`; `tools/oracle.sh`).
   - Lucent is sym 33. Its split starts at bb3, and the BFS adds bb4-bb10, stopping at the
     model_bounds_calc call block (cup=1).
   - From bb8 on, c3 (a0) is forbidden, because `show` is in a0. So the piece can only take c13 (ra), as w16h saw.
   - Oracle `SPL_REJ=6,7 SPL_SYM=33` (piece {3,4,5}) gives a0: 21 -> 16 rows. Only the position of
     `sw a0,124(sp)` remained.
   - None of the f_addadjacents rejects, deadout (f23) or cup stops can cut the BFS at bb5 while lucent stays
     live. So retail's a0 value cannot be lucent itself.
2. **What retail's a0 value is.** A `volatile` lucent (diagnostic only) put the store at retail's exact
   position, early in bb5: it is a def-store of a memory-resident lucent, assigned from a temporary that dies in bb5.
   - With `||` the temporary is a cfe temp; uopt copy-propagates it into the final use, which adds +8 to the frame.
   - Lever: compute the flag in `show`, which is redefined later, then `lucent = show;`. uopt cannot propagate the
     copy. Result r1: `2 of 460 words differ | words 10 ops 0 norm 0 | frame 224/224`.
3. **The last 2 words: `move a3,t0` / `move s0,a0` order.**
   - as1 trace: all ties, so instructions are picked in ugen order.
   - The uopt output ucode (`tools/uproc.py`, workbench parser) is `LOD show; STR r16; RPAR; LOD v; STR r7; RPAR; CUP`.
     The register-parameter copies are emitted in argument order.
   - Hand-swapping the two moves in the listing (`asm.sh`) gives 0 rows.
   - Arcade check: model_bounds_calc is CheckVisible, whose signature is (v, on). With that order everywhere,
     everything is EQUAL (above).

## What generalises
- **A coloured value that ends before a call, while the variable itself lives on, is often a different variable.**
  Look for a reused local holding a temporary that is then copied into a memory-resident variable. If a
  `volatile` diagnostic puts the def-store where retail has it, the a0/a1 value is a temp. To keep uopt from
  copy-propagating, use an existing variable that is redefined later (here `show`).
- **Register-parameter copies (IPA s0/a3 conventions) are emitted in argument order** (ucode `STR rN; RPAR`), and
  as1 keeps ties in emission order. If two parameter moves are swapped, check the callee's real parameter order
  against the arcade: the locked prototypes can carry a swapped order that still matches every caller in which
  as1 hoists one of the moves into a delay slot.
- The instrumented-uopt reject oracle (`SPL_REJ`) is a quick way to prove which piece retail coloured. If no
  natural add/reject rule can produce that piece, the variable structure is different, not the block order.

No permission denials.
