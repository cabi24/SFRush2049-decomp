# w5b results (2026-10-05) — big-render lane

Builder scratch `~/rush2049/scratch/frontier/w5b` (copied from `base`; `uopt`/`as1` symlinked to the traced
builds of w3a/w4a). Unit tag `w5b`. Nothing committed or spliced; nothing under `src/blob`, `tools/`,
`include/`, `asm/` or `tests/` touched. `func_80087110` not touched.

| Function | Bytes | State | Flags | Deliverable |
|---|---:|---|---|---|
| `func_800E8F10` | 804 | **strict MATCH**, own .rodata verified, unit EQUAL | `-O3` only | `cloud/matches/func_800E8F10.c` |
| `entity_collision_detect` | 820 | **strict MATCH**, own .rodata verified, unit EQUAL | `-O3` only | `cloud/matches/entity_collision_detect.c` |
| `func_8010E0FC` | 1,000 | 65/250 words off (register colouring only) | `-O3` | `func_8010E0FC/best.c` |
| `hud_render` | 2,420 | 377/605 strict, 270 aligned (structurally complete) | `-O3` | `hud_render/best.c`, `STRUCTURE.md` |
| `render_display_list` | 10,236 | complete draft; stand-in group only, 1 word short | `-O3` group | `particle_unit/group_standin/`, `STRUCTURE.md` |
| `particle_system` | 2,900 | scouted (sections, inputs) | – | `particle_unit/STRUCTURE.md` |
| `track_collision_wall` | 2,712 | scouted | – | `particle_unit/STRUCTURE.md` |
| `func_8009C3F8` | 452 | scouted; A21 source is semantically right, needs real callers for `f16` | – | `particle_unit/STRUCTURE.md` |

Strict bytes this batch: 1,624 (two functions). Both were `-O3`-only.

## func_800E8F10 — strict MATCH

Path-following camera target (N64 attract/replay path camera; calls the follow-camera update `func_800E8D50` =
arcade `UpdateCarObj`). Semantics and every shaping quirk are in the file header.

```
cloud/work/frontier/w5b/tools/sc.sh cloud/matches/func_800E8F10.c func_800E8F10 --flags '"-g0 -O3 -mips2 -G 0 -non_shared"'
func_800E8F10:
  MATCH
    own .rodata verified at 0x801244C4..0x801244C8
python3 -m tools.conveyor.pipeline.blob_unit --tag w5b score func_800E8F10 --with cloud/matches/func_800E8F10.c --neighbours
  EQUAL func_800E8F10: 201 words (kept, c_func_800E8F10.c)
  locked bodies that differ in this unit: 0
```
Own literal `1.4666667f` (0x3FBBBBBC, mph→ft/s) replaces the old `extern f32 D_801244C4`. Path from the A75
packet (199/201) to the match, all found with the traced uopt and small sweeps (≈120 compiles):
`D_80138668[slot] = D_801391E8[slot] = (s32)position[0]` (chained; fixes the whole temp ring);
segment points written as array expressions, no `start`/`end` pointer locals (start address becomes an
expression web → `a1`, and the `mov.s` of `length` disappears); interpolation moved into the loop's `else`
arm before `break` (one uopt block boundary → retail's reloads of `result[0]/[1]`); `ratio*delta + start`;
left-associative `x*x+y*y+z*z`; `track` read before `amount`; `previous` loaded in the `for` initialiser
(as1 line tie-break); `result` declared before `delta`. Missing `#pragma intrinsic(sqrtf)` was the A83 bug.

## entity_collision_detect — strict MATCH

Smoke/dust puff update attached to a scene node (header has the semantics). Key forms: every access is
`D_80154FD8[node->index].field` (no `Effect *` local — uopt then keeps the address as one expression web in
v0 and re-forms it with a shift-form `×60` on the alpha edge, exactly as retail); `phase` reused for the
resource index (v1); `volatile` frame time; 12 bytes of unreferenced local storage declared first (an unused
`Vec3`; any 12-byte aggregate does it — the only frame-shaping quirk). Effect record = {id union, Mat3 @4
(diagonal grows by 1.5), position @40, alpha/phase bytes, delay @56}.

```
cloud/work/frontier/w5b/tools/sc.sh cloud/matches/entity_collision_detect.c entity_collision_detect --flags '"-g0 -O3 -mips2 -G 0 -non_shared"'
entity_collision_detect:
  MATCH
    own .rodata verified at 0x801239CC..0x801239D0
python3 -m tools.conveyor.pipeline.blob_unit --tag w5b score entity_collision_detect --with cloud/matches/entity_collision_detect.c --neighbours
  EQUAL entity_collision_detect: 205 words (kept, c_entity_collision_detect.c)
  locked bodies that differ in this unit: 0
```
The frontier "group / preserved a2" label is a false positive here (it matches alone and in the unit).

## func_8010E0FC — 65/250 (near-miss)

Gold/silver **coin** object update (names `COIN_GLOWG1`, `GOLDCOIN`, `SILVERCOIN` at 0x80121DB0..; colours
0xFFC800FF / 0xC8E6FFFF from `D_80118E14/18`): numbers the coin, looks up its models with `string_copy_format`
(= MBOX_FindObject), creates the scene record with `func_8008E26C`, makes it visible per player
(`func_800F7A98` collected-bit test, `model_data_load`), kills it when every player has it, and spins it with
`sound_position_set(spin·dt, matrix)` every frame.

```
cloud/work/frontier/w5b/tools/sc.sh cloud/work/frontier/w5b/func_8010E0FC/best.c func_8010E0FC --flags '"-g0 -O3 -mips2 -G 0 -non_shared"'
  65/250 words differ          (-O2: 177/250)
```
Residual lane: register colouring only (mnemonics identical, size equal). (1) `entity` is s3 and the
`D_8012E700` base / `alone` flag s4 — retail has them swapped (uopt priorities 28/9 vs 12/4; tried statement
order, types and use placement, no movement); (2) the colour argument of the inlined `resource_set_color`
is a parameter web (a0) where retail uses a temp, which renumbers the following temps. The two inlined
setters (`resource_set_object`, `resource_set_color`) are what put the resource index in v1 like retail
(69→65). The frame needs 116 bytes of filler below the named locals (`pad[29]`): the colours are stack arrays
(`u32 gold[1]`) because retail keeps them in memory. Next: trace `entity` vs `alone` with `ctrace.sh`, and try
the colours as a 2-element struct/array passed to a setter.

## hud_render — 377/605 (structurally complete)

Not a HUD: the exhaust/smoke puff list update (arcade relative `visuals.c AnimateSmoke`). Full section map,
types, literals and the residual list are in `hud_render/STRUCTURE.md`.
```
cloud/work/frontier/w5b/tools/sc.sh cloud/work/frontier/w5b/hud_render/best.c hud_render --flags '"-g0 -O3 -mips2 -G 0 -non_shared"'
  377/605 words differ (20 section-relative relocations unverified: …)       (-O2: 432/605)
```
bscore: mnem-missing 48, aligned-missing 270, size −1. Residuals: unexplained frame filler (pads), `car`
coloured a3 instead of split into v1, type/speed s-register swap, promoted LCG multiplier, two `add.s` operand
orders. Next: the frame filler is the lead — find the deleted helpers whose inlined parameters account for it
(each inlined call adds slots at the bottom of the frame); that also changes block counts and so the
priorities behind residuals 3–4.

## render_display_list / particle unit

Everything is in `particle_unit/STRUCTURE.md`. Summary: component 13 is one IPA knot (func_8009F058 →
track_collision_wall → particle_system → render_display_list/func_80099B30, plus func_8009C3F8 with the camera
wrappers); none of my four functions can match without `func_8009F058` (5,228 B, unassigned) and the camera
callers. I wrote the complete `render_display_list` body (SDK macros on an address-taken `gfx`, standard
`gDPLoadTextureBlock`, flag masks not bitfields, `else`-arm `cms = WRAP`, `?:` mirror form). With stand-in
callers it gets the retail parameter registers (s0,s1,s3,s4,s5) and identical mnemonics except a promoted
`0xF2000000` constant (2,558 vs 2,559 words). `func_80099B30` matches again in that group (still provisional).
```
cloud/work/frontier/w5b/tools/grp.sh cloud/work/frontier/w5b/particle_unit/group_standin
render_display_list:
  1927/2559 words differ          (positional; 1 word short — see STRUCTURE.md)
func_80099B30:
  MATCH
```

## Struct layouts and globals recovered

* `D_8012E700` Resource (68 B): f32 scale @12, s16 frame/object @20, u32 colour @60, u8 alpha @63.
* Node/entity handles are a union `{s32 handle; struct {s16 high, index;}}` (Effect @0, Puff @52, coin
  entity @12): word reads use the handle, `lh +2` the resource index.
* `D_80154FD8` puff Effect (60 B), `D_8014A250` Car (2,056 B: u8 model @8, u16 exhaust[4] @1564, s8 enabled
  @1600), `D_80152818` Player (952 B: pos @8, dir @20, Mat3 @44, exhaust Vec3[4] @116, s16 speed @248,
  s8 slot @860, s8 view @861), `D_8012E5E8` Path {u16 count; PathPoint *points} with s16 x,y,z,pad points.
* `D_8002EB94` frame time is `volatile f32` (now seen in four functions).
* `D_8013F1F0`/`D_8013F1E0` puff list / pool; `D_8011735C` LCG seed; `D_8017A638` TLUT-mode cache;
  `D_80142948` u16[2][9] frame tables.

## What generalises

1. **Inlined static helpers leave two fingerprints: v0/v1 parameter webs and frame slots at the bottom.**
   When retail keeps a single-use index or value in v0/v1 (`lh v1,14(s4)` then `sll/addu`) where natural C
   gives a temp, write the access through a small static setter (`resource_set_frame(index, frame)`). Each
   inlined call adds its parameters' slots below all named locals (two calls = 16 bytes measured), which is
   the likely explanation of "unexplained frame filler" in hud_render and func_8010E0FC.
2. **A block boundary can be source structure without a compiled-out `if`:** moving the post-loop
   computation into the loop's `else { …; break; }` arm ended a uopt block and gave retail's reloads
   (func_800E8F10). Try this before inventing `if (x) ;`.
3. **Prefer array expressions to pointer locals when retail's pointer register looks "too low"** — a pointer
   local whose web spans a call block is barred from v0–a3 (`forbidden0=0x7e…`); the same address as a CSE'd
   expression web is not (func_800E8F10 start pointer → a1; entity_collision_detect effect address → v0).
4. **Chained assignment** `a[i] = b[i] = (s32)x;` is a one-line fix for a whole shifted temp ring.
5. **Component 13 needs a coordinated lane:** func_8009F058 + track_collision_wall + particle_system +
   render_display_list + func_80099B30 (+ the asin/acos wrappers). Assign it to one agent with all five.
6. Tool: `w5b/tools/full.py --mnem` aligns by mnemonic only (useful above 2,000 words, where register
   renaming hides structure), `--dump` writes the candidate's disassembly; `bs.sh` runs bscore on a set with
   2 threads.
