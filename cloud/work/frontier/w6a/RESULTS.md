# w6a results — particle-system knot (2026-10-05)

Builder scratch `~/rush2049/scratch/frontier/w6a` (copied from `base`), unit tag `w6a`. Nothing committed or
spliced; nothing under `src/`, `tools/`, `include/`, `asm/` or `tests/` touched; `func_80087110` and
`stat_race_update`/`func_800FE5B0` not touched. Real group: `groups/particle_knot/` (group.json + group.c);
working parts in `dev/` (`dev/build.sh` assembles them), variant sweeps in `dev/v/`. Structure, types and
every shaping fact: `STRUCTURE.md`. Tools: `tools/README.md`.

## Table

| Function | Bytes | State | Deliverable |
|---|---:|---|---|
| `render_display_list` | 10,236 | code identical in the real group and the whole-program unit; **provisional** (callers below) | group |
| `func_80099B30` | 204 | code identical (real callers now); **provisional** | group |
| `particle_system` | 2,900 | code identical, own .rodata verified; **provisional** | group |
| `track_collision_wall` | 2,712 | code identical; **provisional** | group |
| `func_8009C3F8` | 452 | code identical, own .rodata verified; **provisional** (callers `camera_follow_path`, `func_800BFD8C` absent) | group |
| `camera_update_c` | 36 | code identical (kept member; new, was outside the assignment) | group |
| `select_screen_update` | 36 | code identical (kept member; new) | group |
| `func_8009F058` | 5,228 | **22/1307 words off**: only the stack offset of one uopt spill temp (72 retail, 68 ours) | group |

Flags: `-g0 -O3 -mips2 -G 0 -non_shared` for all (whole-program group). Strict bytes claimable now: **0** —
`func_8009F058` is not a match and its only caller `Input_ProcessGameplayPad` (2,720 B, unmatched) is a stand-in
(`zz_caller`/`zz_caller2`), so every member stays provisional under the brief's rule. `group.json` has `claims: []`.

### Scorer output (exact)

```
cloud/work/frontier/w6a/tools/grp.sh cloud/work/frontier/w6a/groups/particle_knot
Members:
render_display_list:
  MATCH
func_80099B30:
  MATCH
func_8009C3F8:
  MATCH
    own .rodata verified at 0x80123ABC..0x80123AEC
camera_update_c:
  MATCH
select_screen_update:
  MATCH
particle_system:
  MATCH
    own .rodata verified at 0x80123AFC..0x80123B00
track_collision_wall:
  MATCH
func_8009F058:
  22/1307 words differ
    own .rodata verified at 0x80123B60..0x80123B68

python3 -m tools.conveyor.pipeline.blob_unit --tag w6a score render_display_list func_80099B30 particle_system \
  track_collision_wall func_8009F058 func_8009C3F8 camera_update_c select_screen_update \
  --with cloud/work/frontier/w6a/groups/particle_knot/group.c --internal render_display_list --internal func_80099B30 \
  --internal particle_system --internal track_collision_wall --internal func_8009F058 --internal func_8009C3F8 --neighbours
  EQUAL render_display_list: 2559 words (internal, c_group.c)
  EQUAL func_80099B30: 51 words (internal, c_group.c)
  EQUAL particle_system: 725 words (internal, c_group.c)
  EQUAL track_collision_wall: 678 words (internal, c_group.c)
  FAIL func_8009F058: 22 of 1307 words differ
  EQUAL func_8009C3F8: 113 words (internal, c_group.c)
  EQUAL camera_update_c: 9 words (kept, c_group.c)
  EQUAL select_screen_update: 9 words (kept, c_group.c)
  locked bodies that differ in this unit: 0
blob_unit score: 7/8 equal; object build/blob_unit/w6a/unit.o (3.7s)
```

## Can a subset be claimed as a real group?

No, not under the current rule, and the reason is the chain rather than the bodies:
- `track_collision_wall`'s callers are `func_8009F058` (not a match) and itself.
- `particle_system`'s only caller is `track_collision_wall`; `render_display_list`'s are `particle_system` and
  `func_8009F058`; `func_80099B30`'s are those two.
- `func_8009F058`'s only caller is `Input_ProcessGameplayPad` (unmatched; it depends on `func_80087110`, which is
  someone else's). `func_8009C3F8` is also called by `camera_follow_path` and `func_800BFD8C` (unmatched).
For the integrator: every call site and parameter register in the knot is now code-identical with all real bodies
present in the whole-program unit; `func_8009F058`'s 22 differing words are all `sw/lw t0,68(sp)` vs `72(sp)`
(the `&D_80150B70[view]` save across calls). `render_display_list` also matches with w5b's stand-in callers
(`dev/v/rdl_si/`), so its body does not depend on the real callers.

## Per function

**render_display_list.** w5b's draft was one word short because uopt gave the `0xF2000000` (G_SETTILESIZE)
constant web `s2` after `masks` died. Traced: `p1:w1056 save=0.09 nocs=11 color->s2`; `gforce.sh rdl0
render_display_list "p1:w1056=s"` gave retail exactly. Natural fix: the tail's clip/rect rewrite is an
int-typed form (`G_SETTILESIZE << 24 | …`), so its constant is a different (signed) constant from the
tile sections' unsigned `_SHIFTL` one and no web spans tail + sections. Parameter order `(gp, tex, rect, clip,
pals)`: particle_system's call sites set `gp` first (with `(tex, rect, clip, gp, pals)` the callee is the same but
the caller's set-up order is not).

**func_8009C3F8 + camera_update_c/select_screen_update.** A21's kernel was semantically right; three source
facts closed it (see STRUCTURE). The `$f16` parameter appears once the kernel is internal; the wrappers' order
(`mov.s $f16,$f12` then `move a0,zero`) needs the signature `(f32 x, s32 flag)`.

**particle_system** — written from w5b's scout; 9 rounds. Fixes in order: no level pointer (the 88-byte LodSet is
indexed through `set->lv[lod]`, which folds the +20 base); `while (lod != 0 && …)` inside `if (lod > 0)`
(retail's double guard); `x/y/z` locals for the distance; `parent + node` operand order; `(id & 0x3FF) +
table` texture address; frame from declaration order `gfx, set, lod, rect, dl, v, d, drawn, dir` plus named
`yaw/pitch`.

**track_collision_wall** — written from w5b's scout; the decisive changes were the explicit `goto next`
continues (PRE then keeps `node->next` in v0 on the paths that do not store), the hidden-leaf `else more = 0`
arm, the reused loop counter (stops unrolling), two separate filter ifs, and the one-line initialisation.

**func_8009F058** — written from the disassembly (the bigfish m2c seed was only used as a checklist). 1307 words,
mnemonic-identical after ~70 compiles; then the remaining differences were closed one by one:
| residual | cause found | fix |
|---|---|---|
| ortho arm: `lw s1,400(sp)` hoisted above the branch, arm reordered | ugen emitted `.noalias $16,$sp` for `cam` (assigned after the first call), so as1 let `lwc1 56(s0)` pass the outgoing-arg stores; traced as1 showed the hoist; a fake read of `s1` in the listing reproduced retail | `Camera *cam = &D_8017A510[view];` at the declaration |
| FillRect temps (whole downstream temp ring) | ugen temp allocation | `(s16)` coordinates, y term ORed first |
| `slti t7,v0,2; beqzl` | value compared as a boolean | `i = D_8014A108; if ((i < 2) != 0)` |
| texture `beq s1,…; move v1,s1` | param web vs `tex` web | texture address recomputed as the call argument |
| scale-loop store order, init order | as1 line tie-break | one-line inner loop; `fog = 1; poly = …; mode = -1;` |
| last block: as1 hoists `lui at/move a0/move a1` | candidate order = ugen order | node pointer in a block local before the two flag stores |
| `sw/lw t0,68(sp)` vs `72(sp)` (22 words) | **open**, see "frame" | — |

## Frame: what the "unexplained filler" is (new, generalises)

`tools/patch_area2.py` instruments the recompiled uopt to print every change of the procedure's local area.
For every procedure the area grows in two steps: `f_readnxtinst` (cfe's named locals, in textual declaration
order including block-scoped ones and every macro `_g`) and then `f_spilltemps`, which adds one 4-byte home per
colour class of the interference graph among "spill-set" expression bits (rematerialised constants and
addresses included, `tools/patch_sp.py` lists them). These homes sit below the named locals; the first one is
the slot used when a caller-saved web is saved across calls (`t0` here).
Measured: particle_system 4 homes, track_collision_wall 4, func_8009F058 5 (+4 alignment). Retail needs 6, 8
and 6-7: the 8 bytes of `yaw/pitch` and the 16 bytes of `pad[4]` that particle_system and track_collision_wall
need are those missing homes (equivalent there because no home is referenced). In func_8009F058 the first home
*is* referenced (`t0`), so a named-local pad cannot stand in: with the named area at retail's 324 bytes the
frame is 392 and `t0` is at 64 (top-relative right, frame 8 short); with one extra named slot the frame is 400
and `t0` is at 68 (the submitted form). Variants that did not change the home count: literal types of 1.0/16/
480/−1/32768, render-mode/combiner constants as unsigned, `/ 2.0f` for `* 0.5f`, inlined helpers (they add named
slots), different stand-in callers, physics_float_calc's draft body as context, uopt/as1 `-Olimit`.
**Best next hypothesis:** find which spill-set bits form the largest clique (extend `patch_sp.py` to print the
home index from `func_46d4c4`/`L46dafc`) and look for a source form that lengthens one of those live ranges
without changing code. The same mechanism is the likely cause of every "frame needs N bytes of unreferenced
locals" note in earlier waves (hud_render, func_8010E0FC, music_tempo_set, entity_collision_detect,
menu_load_options).

## What generalises

1. **Constant type is a web boundary.** An int-typed `X << 24` and the SDK's unsigned `_SHIFTL(X, 24, 8)` are
   different uopt constants; a promoted constant that retail rematerialises can be split off this way
   (render_display_list's one-word residual of w5b).
2. **Pointer initialised at its declaration vs assigned later changes ugen's alias directives** (`.noalias
   reg,$sp`), which decides whether as1 may move loads through that pointer above outgoing-argument stores. A
   wrong as1 schedule around a call with stack arguments is worth this test before searching line layouts.
3. **as1 fills a branch block's stall/delay slots from the fall-through in ugen order**, taking instructions
   whose destinations are dead at the target. Which instructions retail hoisted tells you what came first in its
   ugen: computing a pointer into a local before stores moved retail's index chain ahead of `lui at`/arg moves.
   Traced as1: `tools/as1t_remote.sh` (+ the rebuilt `w6a/as1/as1`).
4. **Unrolling is suppressed when a loop's counter is reused by a later loop**; `for (i…) for (j…)` followed by
   `for (j…)` keeps the inner loop rolled (retail `slti`), a private counter unrolls it by 2.
5. **`goto next` vs an else-if chain changes PRE placement** of a value loaded at the loop top: with explicit
   continues PRE keeps it in a register on paths without stores.
6. **Call-site set-up order gives the IPA parameter order** even when the registers are callee-chosen
   (render_display_list `gp` first, asin `x` first).
7. **Frame filler = spill-temp homes** (above). Block-scoped declarations take slots in textual order, so
   `uv`, `perspNorm`, `p`, `tex` placements pin which block declares each local.
8. Process: the group-level tools (`gt.sh` trace, `gforce.sh` oracle, `as1t_remote.sh`, listing edits with
   `asmt.sh`) each decided one residual here; editing the ugen listing first (e.g. adding a fake read of `s1` at
   the branch target, changing `.livereg`) told what retail's compiler state must have been before guessing C.
