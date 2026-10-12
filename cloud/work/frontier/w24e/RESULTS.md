# w24e RESULTS (wave 24, game near-misses, IDO 5.3)

Targets: the func_80090308 + entity_physics_update + save_write_data group (start: w23d best_effects.c /
best_swd.c) and camera_play_script (start: w17a best.c, 27 rows). Nothing matched, so nothing is delivered for
integration: no cloud/matches file, no group dir, no unit_overrides, no data-symbol changes.

| function | bytes | before -> after | flags | scorer line (Pi, whole-program unit, current tree) |
|---|---:|---|---|---|
| func_80090308 | 1128 | 216 -> **198** aligned rows (ops 90 -> 87); body length 285 -> 282 (now retail's); frame 160 vs 272 is unchanged and now accounted for (below) | -g0 -O3 -mips2 -G 0 -non_shared | `FAIL func_80090308: 210 of 282 words differ; +0x1a8: .rodata+0x118: retail words encode 0x005439b8, outside the image; ...` |
| entity_physics_update | 712 | EQUAL-with-device (w23d) -> **21 words with no device and no `slot` variable** (natural form). It is still the same single colour decision. | same | `FAIL entity_physics_update: 21 of 178 words differ` |
| save_write_data | 1200 | 223/300, not worked (w22d draft) | same | `FAIL save_write_data: 223 of 300 words differ; compiled body is 303 words, target 300; ...` |
| camera_play_script | 3520 | 27 rows, unchanged | same | `base.c: FAIL camera_play_script: 793 of 880 words differ; ... \| words 27 ops 1 norm 1 \| frame 608/608` |

Command for the group rows: `python3 -m tools.conveyor.pipeline.blob_unit --tag w24e --jobs 2 score func_80090308
entity_physics_update save_write_data --with cloud/work/frontier/w24e/f308/best_effects.c --with
cloud/work/frontier/w24e/f308/swd.c --internal func_80090308 --neighbours`. It printed
`locked bodies that differ in this unit: 0`. `swd.c` is byte-identical to w23d's `best_swd.c`.
Shortcuts: `f308/us3.sh FILE` runs that command, and `f308/q.sh FN FILE...` adds the aligned row counts.

## func_80090308: the frame gap of 112 B, explained

Tools: `workbench.py diagnose` gave `verdict=structure-mismatch ... frame mismatch: target=-272 candidate=-160
(112-byte difference)` and `save-slots=4->4 bytes; non-save=268->156`. The target object came from the retail words
via `.word` (scratch `mkobj.py`). I also used `spill.sh` (AREA/HOME) and the Mmt offsets in the merged ucode
(`w23d/tools/dp2.py`, block 5028).

Our frame is 160 = 24 (args + ra) + 132 of local area, rounded to 8. The local area is made up of:

| part | bytes | contents |
|---|---:|---|
| named locals | 32 | node -4, i, p, vehicle (unused), a, b, c, color -32 |
| chain-assignment temps | 8 | -36/-40. **cfe gives every link of a chained assignment (`m00=m11=m22=x`) a 4-byte memory temp (Uvreg) in the frame.** |
| 4 inlined func_8008B2E4 calls | 64 | 16 B each: max and rannum, plus the nested rand, aligned to 8 |
| inlined func_8008E06C | 8 | s16 id and the pointer |
| spill homes | 20 | 5 homes, none referenced by the final code |

Retail's constraints come from its only sp references (20, 212-215, 268, 274):
- node is at -4 and color at -60, so **15 named words**, 7 more than ours (+28 B).
- The rest of the local area (inline areas, temps and homes) is 181-188 B against our 100 (+81..88 B, about double).

So the gap is not padding. Retail has 7 more named words above `color` and about twice our inline/temp area.
The inline/temp area has no code trace. A helper inlined in retail but not in ours would explain it (one such inline
level costs 8-16 B per call site). Evidence for a helper or pointer:
- In the extra block, retail keeps `li a0,255` (and `li t7,95`) as coloured constant live ranges for the three colour
  bytes. pretrace shows that a store to the local's bytes is constant context 0 (`remat`). Only stores through a
  pointer (`Color *cp=&color; cp->c.r=255;...`, `v/x5.c`) give a LIVE-RANGE, reproducing `li a0,255` (ops 87 -> 77,
  but words 198 -> 210 because of the frame/name shift).
- Retail also reloads the seed for rand4 only. That needs a store between rand3 and rand4 that uopt cannot
  disambiguate from D_8011735C.

Both point to the extra-block colour, and probably the stretch/spin stores, going through an opaque pointer: most
likely an inlined helper's `Color *` / particle parameter. That would also supply inline area.
Not delivered: the 7 named words cannot be identified without that helper, and unused locals are acceptable only as
the exact residual of an otherwise matching function.

Levers measured (unit, aligned rows / ops):

| variant | change | rows / ops |
|---|---|---|
| eD0 (base) | w23d draft + natural entity_physics_update | 216 / 90 |
| x4 (best) | extra block: separate `color.c.r=255; ...` statements, and `D_8012E700[h].color.word=color.word` (retail `lw t9,212(sp); sw t9,60(v1)`) | **198 / 87**, length 282 |
| x1 | the colour via an inlined func_8008E06C | 211 strict; wrong (retail uses the v1 base) |
| x2 | matrix diagonal as three statements | three reloads of p->size, worse |
| x3 | chain order reversed | identical to eD0 |
| x5 | colour through `Color *cp` | 210 / 77 (the constant 255 in a0 as retail; extra named word) |
| x7 | laundered `p` | 204 / 93 |

## entity_physics_update: a natural form, but still a device short of EQUAL

Natural form (`v/eD0.c`, in best_effects.c):
- No `slot` variable; `node->state` is read directly everywhere (sections 1-3), with no explicit reload.
- uopt's PRE inserts retail's `lh a1,8(a3)` at the end of the section-1 then-block, and the structure is retail's.

The residual is 21 words, colour only. SAVELOG (w17f `uopt_or`, `tools/oracle.sh`) shows that the `ilod.J@8(node)`
web has exactly w23d's slot unit: blocks 7, 8, 10, 11, 12, 17 and 23, card 5, nocs 4, tot 10, save 2.5. That is above
the 2.0 of func_80090770's inlined parameters, so the web is decided first and takes v0; retail has a1.

Measured inert (21 words):
- `s16 slot`
- nested ifs for the fx test (eN1, eN2)
- an early `return` in section 2 or 3 (eD1, eD2)
- the empty-then `if (active) {} else goto kill;` (eD3)
- the slot assignment folded into the section-2 condition (eF2, eF4)

Measured worse:

| variant | change | result |
|---|---|---|
| eF1 | the condition assignment plus a section-3 variable | 19 words, but two webs (v0, v1) |
| eF3 | | 152 words |
| eA, eB*, eC1 | mixed variable/expression forms | v0 plus a `move a1,v0`, or a 1-word shift |
| eF1 (helper) | `return a && (b \|\| c)` fx_ok helper | 74 words (materialised boolean) |

Note: w23d's h1/h2 fx_ok helpers were self-recursive (a sed accident), so only the h3 if/return form had been
measured before.

**Best next hypothesis:** retail's state web has save < 2.0, so it needs n >= 18 live blocks or tot <= 7. Block-adding
constructs do not add blocks inside its live range here: cfe merges consecutive labels, and empty-then and else forms
fold. If an inlined helper (see func_80090308) sits between the count++ and the fx test, its entry and exit labels
and Ulocs could add the blocks.

## camera_play_script: unchanged (27 rows), one new measurement

- Re-scored base: `words 27 ops 1 norm 1 | frame 608/608`.
- New: a pointer loop variable for the corner pass (`for (k=0,t2=pts[0]; k<4; k++, t2+=3) func_800A61B0(d,t2,mtx)`,
  `cps/v/p1.c`): `words 167 ops 4 norm 4 | frame 608/616`.
  - The extra named local shifts the frame by 8.
  - The loop test moves to the D_801174C4 IV.
  - The pointer is still the a1 argument. So a named pointer keeps the argument copy preference, the same as the bare
    IV. Not pursued: the frame has no room for another named local.
- Best next hypothesis is unchanged from w18d: retail's corner pass does not copy the IV straight into a1 (an inlined
  helper parameter?), and it ranks the table pointer at or above the IV.

## What generalises

1. **Chained assignments cost frame space**: cfe makes a 4-byte memory temp per link, laid out in the inline/temp
   area. Count them when doing frame arithmetic (func_80090308: `m00=m11=m22=size` = 8 B).
2. **A constant stored into a local's bytes is never a register candidate** (pretrace ctx 0, `remat`), and the same
   constant stored through a pointer is (ctx 3, `LIVE-RANGE`). A retail `li aN,K` reused for several byte stores into a
   stack struct therefore means the stores went through a pointer, such as an inlined helper's parameter.
3. **Let PRE produce the reload.** A reload the source never wrote (`lh a1,8(a3)` at the end of a then-block) is what
   PRE inserts for a load that a store killed on one path. Reading the field directly reproduces it without an extra
   variable (entity_physics_update).
4. A union copy `dst = src` of a 4-byte union compiles to `addiu tX,sp,N; lw at,0(tX)`. Retail's `lw tX,N(sp)` is
   `.word = .word`.

## Tools (lane dir)

| path | what it does |
|---|---|
| `tools/oracle.sh LABEL PROC "ENV"` | w17f `uopt_or` (PRIO/SAVELOG/SPL) on a snapshot for any procedure (builder `~/rush2049/scratch/frontier/w24e/bin/uopt_or`) |
| `f308/us3.sh` | the three-member unit score |
| `f308/q.sh` | the unit score plus aligned row counts |

Variants are in `f308/v/` and `cps/v/` (ignored). Time: about 2.5 h on the group, 20 min on camera_play_script.
Permission denials: none.
