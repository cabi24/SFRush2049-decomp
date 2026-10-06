# w10c results (wave 10, 2026-10-06)

Assignment: component 9 after w9a's steering_sensitivity match — `camera_trigger_check` (unit with
provisional `func_800C3AD0`), `entity_update`, `input_deadzone_apply` (unit with provisional
`input_process_controller`).

| Function | Bytes | State | Flags | Scorer output |
|---|---:|---|---|---|
| `camera_trigger_check` | 1,232 | **strict MATCH** in a real -O3 group (no stand-in callers of it; whole-program unit EQUAL, 0 locked bodies differ) | `-g0 -O3 -mips2 -G 0 -non_shared` | `camera_trigger_check:` / `  MATCH` (exit 0) |
| `entity_update` | 1,564 | near-miss, 27/391 words (only temp-slot numbers +4 and one v0/v1 swap; opcode-identical) | same | `FAIL entity_update: 27 of 391 words differ` |
| `input_deadzone_apply` | 3,580 | near-miss, 895/895 words long, opcode-identical except ~12 rows; 365 positional words differ (frame 408 vs 400, FP temp permutation) | same | `FAIL input_deadzone_apply: 365 of 895 words differ` |

No permission check denied anything. (One slip: I captured the group score into a file in the builder's
`/tmp` once and deleted it immediately; everything else ran in `~/rush2049/scratch/frontier/w10c`.)

## camera_trigger_check — strict MATCH (group)

Deliverable: `groups/camera_trigger_check/` (`group.json` with `"claims": ["camera_trigger_check"]`,
`group.c`, `camera_trigger_check.c`). Body alone with its header comment: `camera_trigger_check/best.c`.

Group scorer (builder scratch copy synced to the current tree):

```
ssh watchman2 'cd ~/rush2049/scratch/frontier/w10c && IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py group cand/ctc --claims'
Members:
camera_trigger_check:
  MATCH

Context (informational; excluded from exit status):
func_800AD650:
  MATCH
handbrake_apply:
  MATCH
func_800C36A0:
  MATCH
func_800AD4C8:
  MATCH
func_800C3AD0:
  MATCH
input_process_controller:
  MATCH
func_800AD5D0:
  MATCH
func_800AC9BC:
  52/56 words differ
exit=0
```
(`func_800AC9BC` is context and differs the same way in the locked `src/blob/groups/func_800AD4C8`;
it is locked elsewhere.)

Whole-program unit (Pi, repo root):

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w10c score camera_trigger_check func_800C3AD0 func_800AD4C8 input_process_controller func_800AD650 handbrake_apply func_800C36A0 --with cloud/work/frontier/w10c/groups/camera_trigger_check/group.c --with cloud/work/frontier/w10c/groups/camera_trigger_check/camera_trigger_check.c --neighbours
  EQUAL camera_trigger_check: 308 words (kept, c_camera_trigger_check.c)
  EQUAL func_800C3AD0: 362 words (internal, c_group.c)
  EQUAL func_800AD4C8: 66 words (internal, c_group.c)
  EQUAL input_process_controller: 363 words (internal, c_group.c)
  EQUAL func_800AD650: 57 words (internal, c_group.c)
  EQUAL handbrake_apply: 54 words (kept, c_group.c); stub tail: 2 words of deleted-procedure stubs follow (NOT as in the image)
  EQUAL func_800C36A0: 268 words (internal, c_group.c)
  locked bodies that differ in this unit: 0
blob_unit score: 7/7 equal; object build/blob_unit/w10c/unit.o (4.5s)
```

Re-run at 09:48 after the integrator spliced nine other wave-10 singles into the tree (lock and
`asm/us/blob` changed underneath; not by this lane): still `EQUAL` for camera_trigger_check,
func_800C3AD0, func_800AD650, handbrake_apply, func_800C36A0, `locked bodies that differ in this unit: 0`.

### Integration notes

- **Supersedes `src/blob/groups/func_800AD4C8`.** Its members (func_800AD650, handbrake_apply,
  func_800C36A0) and its whole keep list are kept; `camera_trigger_check` is added to members, keep and
  claims. `group.c` is the locked group.c with the three context bodies `func_800AD4C8`,
  `func_800C3AD0`, `input_process_controller` replaced by w1g's sources
  (`cloud/work/frontier/w1g/groups/func_800AD4C8`), with `func_800C3AD0`'s parameters in their real
  order `(poly, wp, q, pt, bound, outIdx, mat, zmin)`. The member bodies and the locked prelude are
  unchanged. Procedure: `blob_group revert func_800AD4C8`, copy this dir to `src/blob/groups/` (drop
  `claims`), `blob_group splice`, then the usual checks.
- **Stub tail note:** w1g's `func_800AD4C8` uses a tiny static `lsq()` that umerge inlines; its deleted
  2-word stub lands after `handbrake_apply` in the unit (retail has no stub there; the note is
  informational in `blob_unit score`, and only members are spliced). Check `blob_unit check` after the
  splice; if the stub tail is refused, the context `func_800AD4C8` needs an lsq-free spelling.
- **Stand-ins that remain** (all for *other* functions' missing callers, none for camera_trigger_check):
  `__standin_func_800C3AD0_a/_b` stand for `entity_update` and `camera_victory` (unmatched), the
  `input_process_controller` pair for `input_deadzone_apply`, `__standin_func_800AD4C8` as in the
  locked group.
- **`func_800C3AD0`'s provisional entry cannot close yet:** it is now byte-identical with one real
  caller (camera_trigger_check) and the real parameter order is proven by that caller's argument
  set-up order (entity_update's retail set-up s4, s2, s6, s0, s8, s5, s7 agrees), but entity_update and
  camera_victory are still unmatched. Update its note/evidence to this group.
- Unblocks `camera_follow_path` (its sole blocker per `frontier show`).

### Semantics / how it closed

See the header comment in `camera_trigger_check.c`. N64-only road-surface query (no arcade ancestor
found): quadtree cell of floor(pos) (`handbrake_apply` on `D_80124EEC`), polygon list from
`D_80152460` expanded by `func_800ADCE0`, every polygon not of type 5/6/15 tested by `func_800C3AD0`
(zmin −20, bound = best so far), smallest |height| wins; flags 0x2000/0x1000 refine with
steering_sensitivity / traction_control; a miss nudges the probe (+x, −x+z, −2z) and retries.

The residual that held all three functions for most of the session was the loop prologue: retail keeps
the polygon count in `s1` across `func_800ADCE0` (`move s1,a1` in the delay slot, `beqz s1`, then
`sw s1,136(sp)` and `addiu s1,sp,172`). It appears only with **`u8 n` + `u32 i` + `i < n` +
`&D_801497F8[list[i]]`** (no pointer local; the strength-reduced pointer then takes s1) and
`n = *p++; func_800ADCE0(p, n, ...)` (p stays in a0). s32/u32 n spill before the call; u8 i adds
`andi 0xff`. The other levers, in order: func_800C3AD0's real parameter order (argument set-up order),
`bestH = 250` (int) vs `bestAbs = 250.0f` (two constant webs), best/bestIdx/bestAbs/bestH assigned
before the out[] copies, `fabsf(h)` written twice (no local: abs in $f2, h stays in the ugen temp $f4),
`mat[1][k] * -out[1]`, `pt[k] + pos[k]`, and the declaration list (u8 n in the pad after `quad`).
About 160 compiles, most of them on the loop prologue; the traced uopt (copied from w9a) identified n's
split piece but did not by itself give the fix.

## entity_update — near-miss, 27/391 words

`entity_update/best.c` (header comment has semantics, levers and residual). Score with
`./us.sh entity_update/best.c entity_update` (unit, tag w10c):

```
  FAIL entity_update: 27 of 391 words differ
       defined by c_best.c (kept)
       umerge inlined into it: effect_cleanup, func_800B61A8
  locked bodies that differ in this unit: 0
```

Opcode-identical (`opdiff.py entity_update`: 0 rows). Residual = one more uopt temp word than retail
at the bottom of the frame (temps at 112/116/120 vs 108/112/116; every named local and the
inline-reservation area are at their retail offsets) plus one v0/v1 swap in the tail. Levers that
closed the rest: the camera_trigger_check loop recipe; **`func_800B61A8(21, 0, 1, 2)` (the locked
SOUND wrapper, inlined) instead of an open-coded `if (D_8010FFC0) entity_flags_apply(...)`** — it gave
the retail branch layout and also stopped −5.0f being hoisted into $f26 (that constant web was
colour-marginal: savings 20 vs cost 19.75 in the trace); `if (off != 0) { … if (bestAbs != 250.0f)
{ … return; } }` falling into `miss:`; `attr[wheel] &= 0x7FF`. Next: find which expression owns the
extra temp (candidates: the slot address `D_80152818[car->id]`, the inlined effect_cleanup
arguments). This is a second real caller of func_800C3AD0; with it and camera_victory matched the
func_800C3AD0 stand-ins can go.

## input_deadzone_apply — near-miss (structure complete)

`input_deadzone_apply/best.c` (header comment). It needs `hdr_ipcorder.h` and
`w1g_part_ipcorder.c` (input_process_controller declared `(poly, p1, p2, out, vcOut, mat, flag,
outIdx, rad2)`; still EQUAL):

```
HDR=input_deadzone_apply/hdr_ipcorder.h PART=input_deadzone_apply/w1g_part_ipcorder.c ./us.sh input_deadzone_apply/best.c input_deadzone_apply input_process_controller --neighbours
  FAIL input_deadzone_apply: 365 of 895 words differ
       defined by c_best.c (kept)
  EQUAL input_process_controller: 363 words (internal, c_w1g_part_ipcorder.c)
  locked bodies that differ in this unit: 0
```

Started at 821/895 (850 words long). Now 895 words with the retail instruction sequence; the 365
positional differences are (1) frame 408 vs 400 (one extra unused temp pair at the bottom, every named
local at retail + 8) and (2) the FP temp choice from the first instruction (retail `start[0]` → $f6,
ours $f8; a consistent f4/f6/f8/f10 permutation, which also moves a few loads). Not caused by the
procedure that precedes it in the unit (tested). Semantics, recovered frame layout and next steps are
in the header comment. **input_process_controller's argument set-up order** in retail is a3, (f24),
s1, s3, s7, s8, s6, s5 → source order `(poly, p1, p2, out, vcOut, mat, flag, outIdx)` with rad2 first or
last (both reproduce it); its provisional entry stays open until input_deadzone_apply matches.

## Generalisable

- **Byte counts drive loop-prologue allocation.** A count read with `lbu` and declared `u8`, with a
  `u32` index and `i < n`, stays in a callee-saved register across the call that fills the list,
  tests with `beqz` and is spilled to a temp at loop entry. s32/u32 counts are spilled at once.
  Index the list (`list[i]`), do not walk a pointer local, or the pointer's web takes the register.
- **Argument set-up order at an IPA register-parameter call is the callee's source parameter order**
  (confirmed again: func_800C3AD0 `(poly, wp, q, pt, bound, outIdx, mat, zmin)`, input_process_controller
  poly-first). The callee bytes do not change with the order.
- **Open-coded `if (flag) call(...)` vs a locked wrapper:** look for a locked wrapper (here
  `func_800B61A8`, the SOUND wrapper) when retail sets up arguments before the test and branches
  around a `b` to the epilogue. The inlined wrapper also shifts colouring costs elsewhere in the
  function (a hoisted FP constant disappeared).
- **Frame anatomy (measured):** named locals top-down in declaration order; each inlined call site
  reserves 8 bytes *below* the named locals (adding an inlined 2-call helper grew the frame by 16 and
  shifted every named offset by 16); uopt/ugen temps sit at the bottom just above the register save
  area and do not move when named locals are added or removed.
- **One FP constant used twice per loop iteration can be colour-marginal** (trace: totalsave 20.0 vs
  bestcost 19.75 for a new callee-saved $f26): small, distant source changes flip it.

## Tools in this directory

`us.sh BODY NAME… [args]` (hdr2.h + body → `blob_unit --tag w10c score` with `w1g_part2.c`; `HDR=`/`PART=`
override), `ub.sh NAME BODY…` (batch, prints unit result + aligned row count), `udiff.py NAME [--all]`
(aligned diff vs retail from the last unit object), `opdiff.py NAME` (same with registers/offsets
normalised: structural differences only), `ctr.sh NAME BODY LABEL [PROC]` (traced uopt on the builder;
camera_trigger_check is proc 608, entity_update proc 612 in this unit), `ipcorder.py` (rewrites
input_process_controller's parameter order in body/part/header). `w1g_part2.c` = w1g's group source
(prelude + func_800AD4C8, func_800C3AD0 with the real parameter order, input_process_controller).
