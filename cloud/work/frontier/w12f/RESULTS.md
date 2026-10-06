# Wave 12 — lane w12f (camera unit, continuing w11c)

Assignment: finish the camera unit (camera_look_at_point, camera_update; camera_update matching would close
camera_free_look and camera_track_spline). All flags `-g0 -O3 -mips2 -G 0 -non_shared`. Builder scratch
`watchman2:~/rush2049/scratch/frontier/w12f` (fresh copy of `base`, synced src/blob, include, tools/cloud,
asm/us/blob and the lock; trace toolkit installed with `--reuse wtk`).

No function reached a strict MATCH this wave. camera_update went from 244 to **17** words. camera_look_at_point
stayed at 38.

| Function | Bytes | State | Flags | Exact scorer output |
|---|---:|---|---|---|
| `camera_update` | 2,876 | near-miss, **17 words** (was 244); frame 248/248, length 719/719 | -O3 | `FAIL camera_update: 17 of 719 words differ` |
| `camera_look_at_point` | 796 | near-miss, 38 words (unchanged) | -O3 | `FAIL camera_look_at_point: 38 of 199 words differ` |
| `camera_free_look` | 436 | code identical, still provisional (caller camera_update unmatched) | -O3 | `EQUAL camera_free_look: 109 words (internal, c_group.c)` |
| `camera_track_spline` | 860 | code identical, still provisional (caller camera_update unmatched) | -O3 | `EQUAL camera_track_spline: 215 words (internal, c_group.c)` |
| `camera_build_view_matrix` (locked) | 612 | still EQUAL with its parameters reordered to `(Camera *cam, s32 idx)` | -O3 | `EQUAL camera_build_view_matrix: 153 words (internal, c_group.c)` |
| `camera_process_input` (locked) | 1,020 | still EQUAL (calls `camera_build_view_matrix(cam, i)`) | -O3 | `EQUAL camera_process_input: 255 words (kept, c_group.c)` |

Unit command and output (from the repo root on the Pi):
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w12f score camera_process_input func_800C15FC camera_update \
    camera_free_look camera_look_at_point camera_track_spline camera_aspect_ratio camera_fov_control \
    camera_build_view_matrix --with cloud/work/frontier/w12f/groups/camera_aspect_ratio/group.c \
    --internal func_800C15FC --neighbours
  EQUAL camera_process_input: 255 words (kept, c_group.c)
  EQUAL func_800C15FC: 2 words (internal, c_group.c)
  FAIL camera_update: 17 of 719 words differ
  EQUAL camera_free_look: 109 words (internal, c_group.c)
  FAIL camera_look_at_point: 38 of 199 words differ
  EQUAL camera_track_spline: 215 words (internal, c_group.c)
  EQUAL camera_aspect_ratio: 50 words (internal, c_group.c)
  EQUAL camera_fov_control: 60 words (internal, c_group.c)
  EQUAL camera_build_view_matrix: 153 words (internal, c_group.c)
  locked bodies that differ in this unit: 0
blob_unit score: 7/9 equal
```
Standalone group compile on the builder (`score.py group cand/camera_aspect_ratio`): all five members `MATCH`
(process_input: `own .data verified at 0x8011750C`, `own .rodata verified at 0x80123E90..0x80123E94`);
context `camera_free_look: MATCH`, `camera_track_spline: MATCH`, `camera_look_at_point: 38/199 words differ`,
`camera_update: 17/719 words differ`.

## Files

- `groups/camera_aspect_ratio/group.c` + `group.json`: the camera cluster with the new camera_update body.
  Same name, same five members as the locked group, **`"claims": []`**. Nothing new is claimable, so **do not
  splice it this wave**. It is the work base for the next attempt. Its only change to locked text is the
  parameter order of camera_build_view_matrix (definition, prototype, and the call in camera_process_input);
  the code of every member is unchanged. If it is ever spliced it needs the same override as the locked group
  (`prefer_definition` func_800C15FC → the group file); nothing else.
- `camera_update/best.c`: the body, with a header listing every shaping choice. `base2.c` is the cluster
  without it; `tools/s3run.sh FILE.c` (env `FN`, `BASE`) splices a body into `base2.c` and unit-scores it.
- `camera_look_at_point/best.c`: w11c's body, unchanged.
- `variants/` in each function dir: every variant mentioned below. `camera_update/retail.s` is the retail
  disassembly.
- `tools/`: `fl.sh`, `s3run.sh` (splice+score), `fnget.py`/`fnset.py`, `vgen.py`/`vrun.sh` (variant
  batches), `area.sh` (frame census: cfe locals area, spill homes, gettemps), `laforce.sh` (look_at_point:
  build, trace, force f/r, score), `udn.py` (dump one procedure's ucode by name from a snapshot's
  `merged`/`opt.o`), `argscan.py` (scan retail for IPA s-register argument order).

## camera_update — 244 → 17 words

Each step below was measured in the whole-program unit. "Rows" means aligned differing rows from
`udiff.py`.

1. **Setter at the s50 site.** The fifth slot store is the inlined setter `func_800C15FC(cam->slot, m)`,
   not a direct store. Retail has slot in v0 and the value in a0. With a direct store, slot is a ring
   temporary. The setter's frame cost is paid for by the variable merges in step 5.
2. **`m = (&D_801427C0)[na];` before the flag test.** This gives retail's `sra t7; move a1,t7` copy pattern.
   `u16 m` packs with `s16 na` into the 4-byte gap above `idx`, so it costs no frame slot.
3. **`if (sc->flags & 0x8000) { } else { func_800C15FC(cam->slot, m); }`.** The empty-then if/else leaves a
   ucode label in front of the inlined setter. ugen then drops its cached cam register and reloads cam
   (`lw t8,244(sp)`). With `if (!(sc->flags & 0x8000))`, ugen keeps cam in t6 and the t6–t9 ring stays one
   step off for about 120 words. This was w11c's main residual.
   - Diagnosis: an inlined getter between the compare and the setter also forced the reload, so the cause
     is a label.
   - Unused labels and `do {} while (0)` are removed by cfe/uopt; only the empty-then if/else survives.
4. **`camera_build_view_matrix(Camera *cam, s32 idx)`.** Retail loads the IPA s3 argument (cam) before
   `move a0,zero`. uopt keeps parameters in source order (checked in `opt.u`), so cam must be the first
   parameter. The callee's IPA still puts cam in s3 and idx in a0. camera_build_view_matrix and
   camera_process_input stay EQUAL with the swapped order. This fixed 6 rows.
5. **Frame 248 with the locked 4-slot setter.** Measured with `tools/area.sh`, the frame is
   align8(cfe locals + inline blocks + spill homes). Only three 4-byte locals fit below `sv`, and at most
   two spill homes. The source therefore uses one variable per register:
   - `idx` is also the key flags, `tb->s14`, the `++node->s04` counter and block_99's index. All of these
     are v0 in retail.
   - `fr` is the do-loop "t" (`fr = ctl->t; f = dur; if (f <= fr) ctl->t = fr - f;`, f2) and block_99's
     fraction (`fr = tt / f;`, f2). This also fixes the loop-top load order.
   - `f` is dur in the do-loop and in block_99, where it is assigned in both arms of the mode&8 test
     (retail loads dur in each arm).
   - `tt` is the pre-division value.

   Two dead ends:
   - A 3-slot setter also gives 248 for camera_update, but camera_process_input then needs another unused
     local, so I kept the locked setter.
   - Making the fraction an expression, or making k a variable, adds a third conflicting spill home and
     gives frame 256.
6. **block_99 index via `idx`, k as an expression `sc->keys[idx]`.** This gives retail's k=v1, nx=a2, and
   the sv/IV inits (as1 hoists `addiu a0,sp,180` only when k is not in a0).
7. **`cam->m[k][i] *= sv[k];`.** The compound assignment gives retail's operand order (m*sv) and the
   f14–f22 sequence. `x = x * sv` gives sv*m. This fixed 16 rows.
8. **`node->f10 -= dt; if (node->f10 <= 0.0f)`.** This uses no f temporary (retail compares the ring
   register f8).
9. **`D_80123E8C` is still extern.** A `0.0425f` literal gives identical code (`unverified 2`). I kept the
   extern so that process_input's verified own .rodata layout is not disturbed. The integrator should switch
   it to the literal when camera_update is claimed.

### Residual (17 words), with oracle evidence

- **na/tb/m colours (8 rows).** Retail has a1/a2/a0; ours has v1/a1/a0, plus the same shift at the tb uses.
  `force.sh bst camera_update "p1:w277=c4,p1:w242=c5,p1:w286=c3"` fixes all 8 rows. Retail must have a
  web holding v1 across the s14/s50 region that interferes with all three. No retail instruction uses v1
  there, so it is a hidden web (like the `idx+1` web that as1 folds away). The p1dec record for na shows
  only v0, s6 and s8 forbidden. Tried:
  - declaration-order permutations;
  - na/m spellings (`na = (s16)…`, `+=`, compare order, `==` + goto);
  - setter-with-test-inside (`func_800C15FC(sc, slot, v)`, 179 rows);
  - restructuring the `goto block_99`;
  - the getter diagnostic.

  None moved it.
- **tt in f12 (4 rows).** Retail has f14. `p1:w344=c27` fixes these rows. Retail f12 is never used in
  camera_update, so again some hidden FP web holds f12 in the tt region. Ternary forms make a cfe temporary
  (also f12, frame 256).
- **Do-loop compares (5 rows).** At both `idx+1`/`count` compares, retail allocates count before `idx + 1`.
  The ugen listing shows ours evaluates `idx + 1` first at both sites, and the int ring is identical before
  them.
  - uopt canonicalises the compare: site 1 becomes `equ(idx, count + -1)`, and `count - 1 == idx`,
    `idx == count - 1` and `1 + idx` give the same output.
  - Writing site 1 as `sc->count == idx + 1` makes uopt form a coloured `idx+1` CSE (save 16.5, v0) that
    shifts the whole function (496 rows). Unsigned and cast spellings do the same.
  - All four forces together leave exactly these 5 rows.

  Best next hypothesis: uopt's commutative-operand ordering follows its expression numbering, so a source
  form in which `sc->count` is entered before `idx` (or before the `idx+1` add) is what is needed. Look for
  one through `udn.py` on `opt.o`.

About 120 variants this wave. Those since step 8 are in `camera_update/variants/`.

## camera_look_at_point — 38 words (no movement)

- **Degenerate `bgez v1,+8`:** no natural form found (about 15 forms):
  - empty if, `do {} while (0)` + break, goto to the next label, empty if/else, a switch;
  - dead stores to x or y followed by overwrite (all removed together with the branch);
  - FP flags (generate code).

  Only a flag tested in an empty `if` survives. Reusing `y` as the flag (`y = 0; if (b < 0) y = 1;
  if (y) {}`, `variants/g_g2.c`) gives the same 38 words without w11c's extra `tt` local. Both are equally
  artificial; best.c keeps w11c's text.
- **f/r colours:** `laforce.sh` confirms that forcing `r=f2,f=f0` leaves 28 rows. Tried:
  - two-step r, `r += .5f`, constant-first products;
  - tests on f instead of r, four clamp forms, if-statement abs, ternary abs;
  - compiled-out `if (f) {}` (FP compares are compiled out, but they add blocks and lower f's save).

  None changed the r-expression save (2.5) against f (2.0).
- **Ring residual (28 rows after the colour force):** one extra int ring temporary and one extra FP ring
  temporary are allocated in retail's then-part. The int ring at the else part (`lh t7` vs `t6`) needs t6
  to be the most recently freed register, and the FP LRU at the 0x61 call (`f8` vs `f4`) cannot come from
  the r computation's register sequence alone. So retail has hidden ugen allocations there (temps that ugen
  folds into the final register, as for the trunc/mfc1 pair). The source construct behind them was not
  found.

## Integration notes

- Nothing to splice from this lane. The group dir supersedes nothing in practice (`claims: []`). It keeps
  all five locked members under the same name, with identical code and the reordered
  camera_build_view_matrix parameters.
- When camera_update is finished:
  - add it to `members`/`claims`;
  - add camera_free_look and camera_track_spline (EQUAL, internal) to `claims`;
  - switch `D_80123E8C` to `0.0425f` and check the own .rodata at 0x80123E8C.
- No unit_overrides changes beyond the locked group's existing `prefer_definition` for func_800C15FC.

Permission denials: none.

## What generalises

1. **IPA s-register argument order is source order.** uopt emits register-parameter stores in argument
   order, and ugen evaluates them in that order. If retail loads a callee's IPA s-register argument (for
   example `lw s3,244(sp)`) before `move a0,zero`, the callee's parameter order is reversed relative to
   the assumed signature. Swap the definition's parameters: the callee's code is unchanged because IPA keeps
   the same registers. `tools/argscan.py` scans retail for the pattern.
2. **An empty-then if/else (`if (c) {} else { body }`) is a ucode label in front of `body`.** ugen's cache
   of memory-resident variables (spilled locals, reloaded from the stack) does not survive a label, so the
   body reloads. Use it when retail reloads a spilled variable that ours reuses from a temp, a mismatch that
   also shifts the t6–t9 ring. Unused C labels and `do {} while (0)` are removed earlier and do not work.
3. **Frame = align8(cfe locals + inline blocks + spill homes).**
   - Spill homes are given out greedily in web-number order. A web takes a new home only if it shares a
     block with webs holding every existing home.
   - gettemps that find a free home do not grow the frame.
   - Turning a variable into an expression web can therefore grow the frame: one fewer local, but a third
     conflicting home.
   - `tools/area.sh` prints the steps (`f_readnxtinst` area, each `f_spilltemps` home, `f_gettemp`).
4. **Compound assignment fixes FP multiply operand order.** `a[i] *= s` gives `mul.s d,a,s`;
   `a[i] = a[i] * s` gave `mul.s d,s,a` in this unit.
5. **Merging purpose-variables saves frame slots** at no code cost when retail uses one register for all of
   them (one variable = one register), for example a loop index reused as flags, a counter and a later
   index.
6. **Hidden webs.** When a colour force shows that retail's register choice needs a forbidden register that
   no retail instruction uses in that region, the holder is a web whose code was folded. as1 or ugen
   eliminate it, as with the `idx + 1` CSE whose `addu v1` as1 removes. Search for source forms that create
   such a web; do not search for colour levers.
