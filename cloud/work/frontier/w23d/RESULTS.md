# w23d RESULTS (wave 23, game): buffer_swap, func_80090308 group

| function | bytes | state | flags | scorer output |
|---|---:|---|---|---|
| buffer_swap | 1676 | **MATCH as group member** (supersedes codex_visual_tire_module_20261002), unit EQUAL; disclosed devices | -g0 -O3 -mips2 -G 0 -non_shared (group flags as before: + -Xcpluscomm -Wab,-r4300_mul) | builder `score.py group groups/codex_visual_tire_module_20261002` -> `matrix_scale_apply: MATCH` / `anim_state_update: MATCH` / `buffer_swap: MATCH`, `own .rodata verified at 0x801239F0..0x80123A00`; `--claims` rc=0. Pi `blob_unit --tag w23d score buffer_swap --with cloud/work/frontier/w23d/buffer_swap/best.c --neighbours` -> `EQUAL buffer_swap: 419 words (kept, c_best.c)` / `locked bodies that differ in this unit: 0` |
| func_80090308 group | 1128 + 712 + 1200 | not matched. entity_physics_update unit EQUAL with a disclosed device (provisional: its internal callee is a draft); func_80090308 210/282; save_write_data 223/300 (w22d draft, untouched) | same | Pi `blob_unit --tag w23d score func_80090308 entity_physics_update save_write_data --with .../func_80090308/best_effects.c --with .../best_swd.c --internal func_80090308 --neighbours` -> `FAIL func_80090308: 210 of 282 words differ` / `EQUAL entity_physics_update: 178 words (kept, c_best_effects.c)` / `FAIL save_write_data: 223 of 300 words differ` / `locked bodies that differ in this unit: 0` |

## buffer_swap: `groups/codex_visual_tire_module_20261002/` (source = `buffer_swap/best.c`)
Started from w22d best.c (43 words). Four source changes:
1. **as1 order.** Each of the three time updates writes `v->time = D_801543CC + 0.015f;` before the index update
   (`v->index = 0/++/--`). This took it from 43 to 15 words.
2. **Spill home.** `model = (Model2056 *)(u32)&D_8014A250[slot];` is moved ahead of `reverse = ...`.
   - spill.sh showed that f_spilltemps gives home 0 (sp+92) to webs 10 (v->object, op==0 block) and 15 (the v->flags load).
     Web 15 conflicts with web 31, the laundered model expression, so model fell to home 1 (sp+88).
   - Once model's expression is created first, it takes sp+92 as in retail. This took it from 15 to 1 word.
3. **mul.s operand order.** `position[0] = D_8011B4B4[model->type][0] * (0, sign);` uses the comma operator, a disclosed device.
   - pretrace shows that the mpy is then created late (bit 245) as mpy(varM sign, ilod), which gives retail's `mul.s $f6,$f0,$f4`.
   - Inert: both source orders, `+sign`, `(f32)sign`, `sign*1.0f`, `register`, a table pointer local, and the `x = x*s` / `x *= s` forms.
     The last two add a store and reload.
   - Dropping the named `sign` for `(reverse ? 1.0f : -1.0f)` gives the right code. The frame comes out wrong, though:
     the top locals shift by +4 and a 4-byte temp goes below.
   - Result: unit EQUAL.
4. **Group compile.** A first group (the tire module plus a codex_transform_b109 copy) put buffer_swap 283 words off.
   - The prologue was already right; the difference was a PRE hoist of v->flags.
   - Comparing umerge output (`merged` ucode, unit against group) showed the two identical except for **Uloc records**:
     - umerge drops the Ulocs of a leaf inlinee (ehu_inline, tire_sound_off) in both builds.
     - The Ulocs of a nested inlinee (tire_extend, which inlined tire_sound_on, which inlined ehu_inline) are kept in
       the small group compile and dropped in the whole-program unit.
   - Proof: stripping the two closing-brace Ulocs (lines 73 and 80) from the group's merged ucode and rerunning
     uopt/ugen/as1 gives 0 differing words. Stripping only one of them does not.
   - Those two Ulocs sit at the if-join after `if (model->sound == -1) {...}`. They keep the join block non-empty,
     which adds a CFG block, and PRE then hoists the flags load.
   - Lever: tire_extend becomes a leaf. The caller writes `tire_extend(v, model); if (model->sound == -1) tire_sound_on(model);`
     and tire_sound_on no longer contains the `if`. Its closing Uloc then falls in straight-line code.
   - Result: unit EQUAL and group MATCH.
   - The file order puts `buffer_swap.c` first so that uld emits it last. Otherwise the deleted `rand` stub of
     func_8008B2E4.c follows it and score.py reports `MISMATCH (1 extra words (nonzero beyond target length))`.
Devices, all disclosed in the file header: ehu_inline (a renamed `__inline` copy of the locked entity_hierarchy_update),
the `__inline` helpers tire_extend/tire_sound_on/tire_sound_off, the unused locals `pX` and `pY[3]` (exact frame residual:
240 with them, 224 without, 4 slots; unit `frame 240/224`), the laundered model pointer, and `(0, sign)`.

### Integration (buffer_swap)
- Supersede `src/blob/groups/codex_visual_tire_module_20261002` with `cloud/work/frontier/w23d/groups/codex_visual_tire_module_20261002`.
  - The group keeps the same name and every old member, and adds member buffer_swap.
  - Claims: matrix_scale_apply, anim_state_update, buffer_swap.
  - New files: `buffer_swap.c` and `codex_transform_b109.c`. The latter is a byte-identical copy of
    `src/blob/groups/codex_transform_b109/group.c`, added as context.
  - The other five .c files are byte-identical to the current group's.
  - Added keep entries: buffer_swap, client_sync, entity_hierarchy_update, scheduler_recv, stat_lap_complete, game_timer_resume.
    All of these are kept in the unit as well.
- Install with blob_group. No unit_overrides are needed. No data-symbol changes.
- Whether the old single/group lock rows for buffer_swap need reverting: buffer_swap is not locked today (it was B137 ipa-group evidence only).

## func_80090308 group: drafts in `func_80090308/` (best_effects.c, best_swd.c)
Time: about 2 h. The stop rule was applied, and the group is far from closing.

### entity_physics_update: 7 rows -> unit EQUAL (device)
- Trace: the w23d instrumented-uopt oracle (`bin/uopt_or`, copied from w17f; `SAVELOG=1`) shows that `slot` is ONE
  allocation unit, lr 9, across its three defs. Its live blocks are 7, 8, 10, 11, 12, 17 and 23, card 5, so n=12.
  That gives nocs = 2+((n-2)>>2) = 4 and tot = 7 uses + 3 defs = 10, so save = 2.5.
- func_80090770's inlined parameters, id and tex, have save 2.0. Retail needs slot coloured after them: v0/v1 are then
  taken, a0 is forbidden, and slot gets a1. That needs save < 2.0, i.e. n >= 18 or tot <= 7.
- `force.sh p1:w9=c4` leaves only the 2 relocation-blind rodata rows.
- Lever: six empty `goto gN; gN:;` after the reload `slot=node->state;`. They are code-neutral and give n=18, nocs=6,
  save=1.67, and the unit reports EQUAL. This is a shaping device: it is disclosed and has no natural reading.
- Other forms tried:

  | form | result |
  |---|---|
  | a separate `s2` variable for the reload | slot unit drops to n=5, save 3.5, v0, and the bnezl delay-slot preload is lost |
  | reading `node->state` directly | 152 words off |
  | `if ((slot=node->state)>=0)` | same as e1 (21 words) |
  | an fx_ok() static helper for the multiplayer test | 47-91 words off |

- Do not land this with the gotos unless the owner accepts them. Best next hypothesis: a natural source shape whose
  CFG has 6 more blocks in slot's live range between the reload and the func_8038A408 call. For example the
  multiplayer test could be a helper or macro with its own block structure that compiles to the same code; h3 shows
  that umerge-inlined helpers change the code.

### func_80090308: 265 -> 210 words (structure), frame 160 vs 272
Rewrite of w22d's `puff_init` draft:
- An in-place `for (i=0;i<4;i++)` loop with direct `D_80154660[car].particles[i]` and `player_array[car]` indexing.
  This reproduces retail's strength-reduced induction variables exactly:

  | register | holds |
  |---|---|
  | s0 | particle |
  | s2 | texture, read as `lhu a0,316(s2)`, with the end test against D_801427C0+8 |
  | s3 | matrix |
  | s6 | player |
  | s7 | 0x42000 |
  | s4 | LCG multiplier |
  | s8 | 128 |
  | s1 | seed address |

  The loop top, the first two calls, the pos loads-before-stores, and the loop tail (timer and colour copy) now match.
- `p = &particles[i]` gives retail's `move s5,s0` copy. It is used for the size reload (`lwc1 $f2,52(s0)` after the
  store), the timer, and `func_8008E06C(p->handle.half.low,&color)`.
  - The direct indexing proves that the store/reload pairs and the order of independent stores are alias questions.
  - With p copy-propagated into the IV, `p->stretch` stores do not kill the seed.
- Open:
  - Retail reloads the seed for rand4 only, after rand3. Something between rand3 and rand4 must alias D_8011735C: an
    opaque pointer store (laundered p?) for stretch/spin_y/spin_z.
  - The zero/alpha/diag store placement around rand1-3.
  - The extra block uses offsets from the pool base (s0+336..389) with s1 = s0+340. The draft matches its shape.
  - Frame:
    - retail is node 268, color 212, 13 named words between them, and about 176 B of inline area below color.
    - Ours is 160.
    - The only retail sp references are 20, 212-215, 268 and 274, so only the named-word count (node..color) and
      the total are constrained.
- The w22d hypothesis of a puff_init helper with a `Color *` parameter is refuted. Retail stores `color.c.a = 128`
  directly (`sb s8,215(sp)`), so the loop body is not a helper that takes &color.

### save_write_data
Not worked; the w22d draft is unchanged (223/300 in the current unit).

### Packaging note (when the members match)
- The group needs func_80090308, entity_physics_update and save_write_data.
- Context, as byte-identical copies:
  - func_8008E06C and func_80090770 (both locked; umerge-inlined).
  - func_8008B2E4.c (locked, with its static rand).
  - func_80090284 (locked allocator).
- unit_overrides: `force_internal func_80090308`.
- The group does NOT need entity_spawn_init's group (it is called by jal).

## Generalises
- **umerge Uloc retention differs between a small group compile and the whole-program unit.** It concerns a nested inlinee,
  meaning an `__inline` helper that itself inlined another one.
  - Its Ulocs survive in a small group and vanish in the unit.
  - A closing-brace Uloc at an if-join splits the join and changes PRE.
  - A unit EQUAL that fails in `score.py group` with a PRE-style difference: diff the `merged` ucode
    (`w23d/tools/dp2.py`, `cmpprocs.py`) and test by stripping Ulocs (`w23d/tools/strip.py`).
  - Fix: keep nested inlinees out of if-joins, or make the helper a leaf.
- Source order of a laundered pointer's assignment sets f_spilltemps home order (first-created web gets home 0).
- `(0, x)` flips uopt's commutative operand order when no plain spelling does (it delays the mpy's creation).
- A variable's allocation unit spans all its defs, so a reload is part of the same unit. Priority is
  tot/(2+((n-2)>>2)) with n = live blocks + live-through card. Empty `goto L; L:` adds exactly one block each and is
  code-neutral (w17f). Use the w17f oracle uopt (`SAVELOG=1`) to read n and tot directly.
- IDO loop strength reduction: `for (i...)` with direct `G[k].arr[i].f` indexing gives one IV register per distinct
  address expression (base, `.matrix`, `D_801427C0[158+i]` with a 316 offset). A pointer `p = &G[k].arr[i]` is
  copy-propagated into the IV. Uses after the IV increment get a `move sN,s0` copy.

## Tools (w23d/tools)
- `dp2.py MERGED BLOCKNO`: dumps a procedure's ucode from a merged file.
- `cmpprocs.py UNIT GROUP`: matches procedures by opcode signature and compares their Uloc counts.
- `strip.py` / `tstrip.sh LO HI`: removes Uloc records in a line range from a procedure and reruns uopt/ugen/as1
  (an oracle).
- `gsnap.sh GROUPDIR LABEL`: builds an st_LABEL snapshot from a group dir for the trace tools.
- `gt.sh SRC`: scores the lane's tire group with buffer_swap.c replaced.

## Permission denials
- One denial: `rm -f $G/*` (clearing a group dir before rebuilding it), blocked by the rm safety check. It was not retried.
- Instead, the extra context files of the first (full) group were moved to `buffer_swap/v/gfull/`, and the minimal
  group was rebuilt with cp.
- `cloud/work/frontier/w23d/groups/codex_visual_tire_module_20261002/` holds exactly the files its group.json lists.
