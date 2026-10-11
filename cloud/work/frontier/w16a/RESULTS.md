# Wave 16: lane w16a (camera_update)

**camera_update matches: 5 -> 0 words.** It is EQUAL in the whole-program unit (719/719, frame 248/248). The
superseding `camera_aspect_ratio` group claims camera_update, camera_free_look and camera_track_spline
(+4,172 B game). `score.py group` exits 0 with all 8 members MATCH and own .rodata verified.

Work base: `w15a/groups/camera_aspect_ratio` (5/719). Builder scratch `watchman2:~/rush2049/scratch/frontier/w16a`: a
fresh copy of base, synced (src/blob, include, tools/cloud, tools/conveyor, asm/us/blob, lock), with the toolkit
installed `--reuse wtk`. Flags: `-g0 -O3 -mips2 -G 0 -non_shared`.

| Function | Bytes | State | Flags | Exact scorer output |
|---|---:|---|---|---|
| `camera_update` | 2,876 | **MATCH** (group; unit EQUAL) | -O3 | `EQUAL camera_update: 719 words (kept, c_group.c)` / group: `camera_update: MATCH  own .rodata verified at 0x80123E8C..0x80123E90` |
| `camera_free_look` | 436 | **MATCH** (group; unit EQUAL, internal) | -O3 | `EQUAL camera_free_look: 109 words (internal, c_group.c)` / group: `MATCH  own .rodata verified at 0x80123E84..0x80123E88` |
| `camera_track_spline` | 860 | **MATCH** (group; unit EQUAL, internal) | -O3 | `EQUAL camera_track_spline: 215 words (internal, c_group.c)` / group: `MATCH  own .rodata verified at 0x80123E88..0x80123E8C` |
| `camera_look_at_point` | 796 | context, not worked | -O3 | `FAIL camera_look_at_point: 38 of 199 words differ` |

## Confirmation

Whole-program unit, run from the repo root on the Pi against the current tree:
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w16a score camera_process_input func_800C15FC camera_update \
  camera_free_look camera_look_at_point camera_track_spline camera_aspect_ratio camera_fov_control \
  camera_build_view_matrix --with cloud/work/frontier/w16a/groups/camera_aspect_ratio/group.c \
  --internal func_800C15FC --neighbours
  EQUAL camera_process_input: 255 words (kept, c_group.c)
  EQUAL func_800C15FC: 2 words (internal, c_group.c)
  EQUAL camera_update: 719 words (kept, c_group.c)
  EQUAL camera_free_look: 109 words (internal, c_group.c)
  FAIL camera_look_at_point: 38 of 199 words differ
  EQUAL camera_track_spline: 215 words (internal, c_group.c)
  EQUAL camera_aspect_ratio: 50 words (internal, c_group.c)
  EQUAL camera_fov_control: 60 words (internal, c_group.c)
  EQUAL camera_build_view_matrix: 153 words (internal, c_group.c)
  locked bodies that differ in this unit: 0
blob_unit score: 8/9 equal; object build/blob_unit/w16a/unit.o (6.2s)
```
Group, on the builder (`score.py group cand/camera_aspect_ratio`, exit 0):
```
Members:
camera_aspect_ratio:      MATCH
camera_fov_control:       MATCH
camera_build_view_matrix: MATCH
func_800C15FC:            MATCH
camera_process_input:     MATCH  own .data verified at 0x8011750C (4 bytes; ...)  own .rodata verified at 0x80123E90..0x80123E94
camera_update:            MATCH  own .rodata verified at 0x80123E8C..0x80123E90
camera_free_look:         MATCH  own .rodata verified at 0x80123E84..0x80123E88
camera_track_spline:      MATCH  own .rodata verified at 0x80123E88..0x80123E8C
Context (informational; excluded from exit status):
camera_look_at_point:     38/199 words differ
```
(The scorer prints each verdict on its own line. They are joined here only to save space.)

## Traced cause: proved from the recompiled uopt source

The diagnosis used the trace toolkit (`pretrace.sh` PRE vectors and itable, `ugt.sh`, `ctrace.sh`) together with
the recompiled IDO sources on the builder (`~/rush2049/scratch/ci/tools/ido-static-recomp/build/uopt.c`). It took
about 2.5 h. Most variants were probes (~45 in total). There was no sweep.

1. **Operand order of `equ`/`neq` is set in uopt `readnxtinst`, not by the order in which expressions are created.**
   The relevant code is `f_readnxtinst` around L442798–L442c00. The opcode sets come from the data words:
   0x10000ba4 = {equ 0x23, neq 0x5f}, 0x10000b9c = {grt, leq} (swap and change the operator),
   0x10000b94 = {geq, les}.
   - `var + const` is held as the variable plus a *pending constant*.
   - For equ/neq, if op1 is not an isop, the pending constants move to op2. So `idx + 1 == count` becomes
     `equ(idx, add(count,-1))`.
   - Then the operands **keep their order only if op1 is an isvar (kind 3). Otherwise they are swapped.**
   - Result: a compare that involves a plain variable always comes out variable-first. Two non-variables come out
     in reverse source order.
   - `isopihash` is symmetric, and `isearchloop` matches swapped operands, so an existing entry keeps the order it
     was created with.
2. ugen folds `equ(A, add(B,c))` into `A - c == B` and evaluates the left operand first. That decides the temp
   ring.
   - Ours: site 1 was `equ(idx, add(count,-1))` and gave `addiu t6,v0,1; lh t8`.
   - Site 2 was `equ(add(idx,1), count)`.
   - Retail needs the count side on the left at both sites.
3. w15a's hypothesis is **partly confirmed and partly refuted**.
   - Confirmed: the order is a kind rule (variable vs non-variable).
   - Refuted: "retail keeps `ctl->idx` in v0".
     - uopt PRE is not field-sensitive for indirect accesses. Any istr, even `ctl->f10 = ...` (probe `v/pr4.c`),
       kills `ilod ctl->idx`: block 53 `alters` bit 42 in the PRE listing.
     - So at site 1, behind the `ctl->t = fr - f` store, an `ilod` form must reload (w15a `a.c`, 720 words).
     - Removing that store (`v/pr1.c`) gives count-first with no reload, which confirms the kill.
   - At site 2 an ilod form does work. `v/s2i3.c` rewrites the post-store region on `ctl->idx` and gets retail's
     order, but it costs a v0/v1 colour flip (idx's web save 17.1 -> 12.4).
4. **The lever:** make the idx side an isop at readnxtinst time that folds away before codegen.
   - `idx * 1 + 1 == sc->count` at both sites gives `equ(count, add(idx*1,1))`, which is count first.
   - `(idx ^ 0)` and `(idx >> 0)` give the same object (`v/id/dx1.c`, `ds1.c`).
   - The source order matters: `sc->count == idx * 1 + 1` leaves 3 words (`dx2`/`dm2`/`ds2`).
   - Forms that do not work:
     - `(idx | 0)` and `(idx & -1)`: CSE with the store, 718 words.
     - `-(-idx)`, `~~idx` and `(s16)idx`: code changes.
     - `u32 idx`: 488 words.
     - assignment-expression forms such as `(nx = sc->count) == idx + 1` (`v/v4.c`): right order, but nx stays a
       coloured web (a2).
5. w15a's two compiled-out reads are still needed. Without the lk holder the result is 8 words. Without the dur
   holder it is 4. Without both it is 12 (`v/id/h1-3.c`).

## Shaping devices (all disclosed in the `best.c` header)

- NEW: `idx * 1 + 1 == sc->count` at the two do-loop end tests. This is a compiled-out identity: it makes no code
  and only changes uopt's tree kind. It is not a false prototype, a pad or an unused local.
- From w15a: `lk = sc->link; if (lk) {}` after m's load, and `if (sc->keys[idx].dur != 0.0f) {}` after the division.
- From w12f: the parameter order of camera_build_view_matrix, the empty-then setter test, one variable per
  register, and the parameter `flag` reused as a local.
- No implicit int, no unused locals, no pad arrays.

## Integration notes

- **Install** `cloud/work/frontier/w16a/groups/camera_aspect_ratio/` with `blob_group`.
  - It supersedes `src/blob/groups/camera_aspect_ratio/`.
  - Same name. All five old members are kept byte-identical: camera_aspect_ratio, camera_fov_control,
    camera_build_view_matrix, func_800C15FC (locked stub given its body) and camera_process_input.
  - It adds members camera_update, camera_free_look and camera_track_spline (all three are `claims`). Context is
    camera_look_at_point only.
  - `keep` and the stand-in callers are unchanged from the locked group.
- **Keep** `unit_overrides` `prefer_definition` func_800C15FC -> `src/blob/groups/camera_aspect_ratio/group.c`.
  No other override is needed.
- **Data symbol:** `D_80123E8C` is now the natural literal `0.0425f`. Retail bits are 0x3D2E147B, checked against
  `build/game_code.bin` at image offset 0x9D43C (base 0x80086A50). The group's `extern f32 D_80123E8C;` is dropped.
  `score.py group` verifies own .rodata at 0x80123E8C..0x80123E90.
  - The next word, 0x80123E90, has the same bits but belongs to camera_process_input's rodata, which is also
    verified.
- **Provisional:** camera_track_spline is in the provisional list. Its only caller is camera_update, so it can
  leave provisional when this lands.
- **Coverage:** +4,172 B game (2,876 + 436 + 860).
- **Lane directory:** `camera_update/v/` is not covered by `.gitignore` (about 600 KB of probes). Drop it when
  integrating if you want.

## Files

- `camera_update/best.c`: the body, with the full header.
- `groups/camera_aspect_ratio/{group.c,group.json}`: the deliverable.
- `v/`: every probe named above.
- `base2.c`, `retail.s`, `tools/`: helpers copied from w15a and retargeted to the w16a tag.
- Traces are in `build/trace/w16a/` on the Pi: `pre_a`, `pre_b0`, `lane_tmp/` (PRE reports, ctrace, ugen listing).

## Permission denials and incidents

- No permission denials.
- I briefly wrote a temp file to the builder's `/tmp/w16a_grp.txt` (the score.py group output) and removed it
  straight away. It was not one of the forbidden paths.

## What generalises

1. **The compare operand-order rule (uopt readnxtinst, equ/neq): kept only when op1 is a plain variable,
   otherwise swapped; a `var + const` constant first moves across to op2.** This is the cause of every
   "compare evaluates the wrong side first" residual. A plain-variable operand is always first.
2. **The lever for it is a compiled-out identity op (`x * 1`, `x ^ 0`, `x >> 0`) on the variable side.** It turns
   the variable into an isop for readnxtinst. Source order then decides the result (non-variables come out
   reversed).
3. **uopt PRE treats every istr as killing every ilod, with no field sensitivity.** "Retail keeps `p->f` in a
   register across a store to `p->g`" is therefore never an ilod web. It must be a variable or a register
   expression.
4. The recompiled IDO sources (`ido-static-recomp/build/{uopt,ugen}.c`) can be read for exact rules. Bitmask
   opcode sets live in the `data[]` array (base 0x10000000). They are MSB-first, indexed by `(op-0x20)>>5`.
