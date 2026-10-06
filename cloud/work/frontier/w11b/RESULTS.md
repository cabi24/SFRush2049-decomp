# w11b results (wave 11, 2026-10-06)

Assignment: the two component-9 near-misses left by w10c (`entity_update`, `input_deadzone_apply`),
then `camera_victory` / `camera_follow_path` if ready.

| Function | Bytes | State | Flags | Scorer output (exact) |
|---|---:|---|---|---|
| `entity_update` | 1,564 | near-miss, **10/391** (w10c: 27). Opcode-identical; frame and temp slots now exact. Residual is one v0/v1 swap; forcing one uopt colour gives 0 rows | `-g0 -O3 -mips2 -G 0 -non_shared` (whole-program unit) | `FAIL entity_update: 10 of 391 words differ` / `umerge inlined into it: effect_cleanup, eu_nop, func_800B61A8` / `locked bodies that differ in this unit: 0` |
| `input_deadzone_apply` | 3,580 | near-miss, **122/895** (w10c: 365). Frame 400 as in retail, every stored local at its retail offset. Residual: FP register choice (uopt colours plus ugen's FP ring phase) and 12 load-placement rows | same | `FAIL input_deadzone_apply: 122 of 895 words differ` / `EQUAL input_process_controller: 363 words (internal, c_w1g_part_ipcorder.c)` / `locked bodies that differ in this unit: 0` |
| `camera_victory` | 1,468 | not attempted: `frontier show` lists `blockers: camera_play_script` (unmatched) | - | - |
| `camera_follow_path` | 1,484 | not attempted: `unit_blockers: func_800D11BC, func_800D348C, race_countdown_display` (its s0 register-parameter callers are unmatched, so at best provisional); prior frozen attempt is 250/371 (`cloud/work/near_miss_B135.md`) | - | - |

Neither function matched, so there is nothing to integrate: no `cloud/matches/` file, no group, and no
unit_overrides entries. **Neither provisional entry can close.** `func_800C3AD0` still lacks matched
entity_update and camera_victory callers, and `input_process_controller` still lacks a matched
input_deadzone_apply caller.

Permission denials: none. All builder work ran in `~/rush2049/scratch/frontier/w11b` (my own uopt builds in
`uopt2/` (temp-tracing) and `tr/` (globalcolor CDX)) and in the blob_unit stage `unit/w11b`.

## Commands (Pi, repo root; tools in `cloud/work/frontier/w11b/tools/`)

```
cloud/work/frontier/w11b/tools/us.sh cloud/work/frontier/w11b/entity_update/best.c entity_update --neighbours
  FAIL entity_update: 10 of 391 words differ
       defined by c_best.c (kept)
       umerge inlined into it: effect_cleanup, eu_nop, func_800B61A8
  locked bodies that differ in this unit: 0
blob_unit score: 0/1 equal; object build/blob_unit/w11b/unit.o (4.5s)

HDR=input_deadzone_apply/hdr_ipcorder.h PART=input_deadzone_apply/w1g_part_ipcorder.c \
  cloud/work/frontier/w11b/tools/us.sh cloud/work/frontier/w11b/input_deadzone_apply/best.c input_deadzone_apply input_process_controller --neighbours
  FAIL input_deadzone_apply: 122 of 895 words differ
       defined by c_best.c (kept)
  EQUAL input_process_controller: 363 words (internal, c_w1g_part_ipcorder.c)
  locked bodies that differ in this unit: 0
blob_unit score: 1/2 equal; object build/blob_unit/w11b/unit.o (4.5s)
```
(`us.sh` = header + body → `python3 -m tools.conveyor.pipeline.blob_unit --tag w11b score … --with <body> --with
w1g_part*.c`, as in w10c.)

## entity_update: 27 → 10

**Arcade ancestor (new):** `game/stree.c` `tiresurf(m, ipos, opos, roadcode, uvs, whl)`. It walks the quadtree leaf
from `m->lasttp[whl]`, scans the slist, keeps the best surface and sets `*roadcode = flags & SURF_MASK`. On a miss
it sets opos = ipos with the default plane 200 below and uvs = identity. The N64 version adds the hint (+1440), the
0x2000/0x1000 snaps and the 3/4/7 surface effects.

**The +4 temp-slot shift was the merged frame, not an extra coloured web.** Measured with the temp-tracing uopt
(`tools/tt.sh`, entity_update = proc 666 in the `[TMP]` numbering):
- Ours started `f_spilltemps` at disp 204, so temp0 was at sp120. Retail needs 208 (temp0 at sp116). Frame size
  is 328 either way.
- umerge appends each inlined callee's area in call-site order and aligns each area start to 8. Measured:
  cfe 172 → align8(172+4) = 176, effect_cleanup +8, func_800B61A8 +20, total 204. With func_800B61A8's call
  textually first the total is 208. Any further inlined call after the func_800B61A8 call aligns 204 to 208.
- What the extra helper costs: with no parameters, or with pointer parameters only, it adds nothing beyond the
  alignment. A float parameter adds 8 (216). Wrapping func_800B61A8 in a helper gives 212. Putting the
  effect-tail in a helper gives 220.
- So `static void eu_nop(void) {}` called after `func_800B61A8(21, 0, 1, 2)` gives exactly retail's frame and
  temps. No stub is left in the unit (checked with nm). The original was most likely a stubbed-out N64 call
  there. **Caveat for a reviewer:** this is a shaping helper whose real identity is unknown.

**Residual (10 rows): one tie in uopt p1 colouring.**
- The `*surf` reload web w177 has save 1.5 (tot 6 / nocs 4). The `D_80152818[car->id]` slot-address PRE web
  w188 also has save 1.5 (tot 3 / nocs 2).
- Equal priority is resolved by the lower web number, so `*surf` is coloured first and takes v0, and the slot
  takes v1. Retail has the opposite.
- `tools/force.sh cur 618 "p1:w177=c2"` gives `differing rows 0` (CDX proc 618). So the whole remaining
  difference is this one decision.
- Tried without movement (~45 builds): condition shapes (nested, early return, `!=` chains, one combined `if`),
  `4 == *surf`, `surf[0]`, `*(u32 *)surf` stores and loads, an `(s32)` cast on the store, slot spellings
  (`&D[..]->`, `D + id`, `(s32)` index), a slot pointer local (frame +8), empty-if and expression-statement
  probes, `attr` spellings, and moving eu_nop to other legal spots.
- `if (D_80152818[car->id].b857 == 0) {}` before the `*surf` store changes the code (147 words).

**Next hypothesis:** give the slot web a lower web number than w177, or a save above 1.5. Web numbers follow the
basic-block order of first occurrence, and the slot web is a PRE temp. Alternatively give `*surf` one more
occurrence block. Also check whether the real "eu_nop" could be a call whose inlining adds blocks that change
the numbering.

## input_deadzone_apply: 365 → 122

**The 8 extra frame bytes were the cfe temp of the float `?:` in FLOORF.**
- The cfe `Udef` was 228. Retail's temps (sp168/172 in a 400 frame) need 224.
- w10c's unused pad `s32 Y` stood exactly where a real 4-byte local belongs. Declaring `u32 off` in Y's place,
  and dropping the separate `off` from the bottom of the list, gives cfe 224. Doing the same with `u8 *p` gives
  the same result.
- The frame becomes 400 and the i-spill pattern and the s-register assignment come out as in retail.
- FLOORF is now used for all six floors; for the first two this is byte-identical to the if/else spelling.
  Replacing the ternary with if/else into x/z also gives 224 and the same 122 words.
- **Pitfall:** score this function with the ipcorder header/part (`tools/iub.sh`). The default `hdr2.h` gives
  ~790 words for the same body.

**Residual 122 rows:**
1. **uopt FP colours.** z is in $f2 (retail $f0), and end[2]'s web follows it. Forcing `p1:w36=c24,p1:w66=c25`
   (CDX proc 419) fixes only those rows (115 left).
2. **ugen FP ring phase.** From the first FP instruction, retail loads start[0] into $f6 and ours into $f8. The
   permutation runs through the whole function, and the 12 load-placement rows in face sections 2–4 follow
   from it. In the entry block's ucode the FP events are:
   - `lod a3 → str f26` (radius),
   - `rldc 0.0 → f28`,
   - three ilod/str copies,
   - `lod cur[0]`.

   So one alloc/free event differs before the first copy. Tried without movement:
   - every permutation of the three copies and `best = NULL` placement,
   - loop, `*start++` and comma copies,
   - FLOORF vs if/else at the top,
   - a float-using procedure before or after it in the same file (as the workbench law L13 says, ring state
     does not carry across procedures).

**Next hypothesis:** find which entry-block statement costs one ring event more or less than in retail. Candidates
are the radius parameter move, and whether the cur[0] compare reload is part of the entry block.

## Generalisable

- **umerge inline areas are 8-aligned per area, in call-site order.** Merged frame = align8(cfe + 4), then each
  inlined callee's area at an 8-aligned start. A residue of 4 mod 8 is possible only when the last area has an
  odd-word size (e.g. func_800B61A8's 20). So a +4 temp-home shift can be the *order* of inlined calls, or a
  missing trailing inlined call. Before looking for an extra coloured web, check `Udef Mmt` in the merged
  ucode (`tools/mu.sh`).
- **An inlined helper with no parameters or only pointer parameters costs 0 bytes** beyond that alignment; a
  float parameter costs 8.
- **Float `?:` costs a 4-byte cfe temp at the bottom of the locals** (int `?:` does not). If a function's frame
  is one quantum too big and it uses a float ternary macro, look for an unused pad that should be a real local.
- **uopt p1 ties go to the lower web number.** A v0/v1 (or any) swap between two equal-save webs is one forced
  colour (`CDX_FORCE p1:wN=cK`); check with `force.sh` before editing source.
- The per-function uopt trace numbering differs between the tracers: `[TMP] procinit` counts every procedure
  (entity_update 666, input_deadzone_apply 452), while CDX `procindex` counts coloured procedures (618, 419).
  Find a procedure by building a padded variant and diffing.

## Files

- `entity_update/best.c` (10/391, header documents the lever and the residual).
- `input_deadzone_apply/best.c` (122/895) + `hdr_ipcorder.h` + `w1g_part_ipcorder.c`.
- `variants/`: the probes quoted above.
- `tools/`:
  - `us.sh` / `ub.sh` / `iub.sh`: unit scoring.
  - `mu.sh`: cfe and merged `Udef`.
  - `tt.sh`: `f_spilltemps` trace.
  - `ctr.sh`: CDX colouring summary.
  - `force.sh`: forced colouring → aligned diff.
  - `sp.sh`: sp census.
  - `udiff.py` / `opdiff.py`: aligned diffs.
  - `ud2.py` / `udump.py` / `cfedef.py`: ucode dumps.

  All replay against the current tree; there are no lock or manifest pins.
