# w11e results (wave 11): the real callers of engine_torque_calc and drone_ai_update

Assignment: match the real callers that internal near-misses wait on: `engine_sound_update` and
`transmission_ratio_get` (callers of internal `engine_torque_calc`), then `engine_torque_calc` as a real group;
`drone_ai_update` / `entity_tick_main` second. The engine trio was the closest pair. It is now a **strict
match as a closed real group with no stand-ins**. The drone pair was scouted only (see the end).

| Function | Bytes | State | Flags | Scorer output (exact) |
|---|---:|---|---|---|
| `engine_sound_update` | 1,428 | **MATCH** (group) | `-g0 -O3 -mips2 -G 0 -non_shared` | `engine_sound_update:` / `  MATCH` |
| `transmission_ratio_get` | 1,808 | **MATCH** (group), own .rodata verified | same | `transmission_ratio_get:` / `  MATCH` / `    own .rodata verified at 0x80121D40..0x80121D4C` |
| `engine_torque_calc` | 896 | **MATCH** (group, internal) | same | `engine_torque_calc:` / `  MATCH` |
| `func_800AB7D0` (context, already locked stub) | 8 | MATCH | same | `func_800AB7D0:` / `  MATCH` |
| `func_800AC660` (context, already locked stub) | 8 | MATCH | same | `func_800AC660:` / `  MATCH` |
| `drone_ai_update` + `entity_tick_main` | 5,964 | not attempted beyond a scout (669 + 887 aligned rows with w5a's draft) | — | `FAIL drone_ai_update: 781 of 820 words differ; compiled body is 794 words, target 820; …` |

New strict coverage: 3 functions, **4,132 bytes**. It also unblocks `tire_sound_update` (sole blocker
`engine_sound_update`) and `differential_output` (sole blocker `transmission_ratio_get`).

## Evidence

Group: `cloud/work/frontier/w11e/groups/frontier_level_objects/` (`group.json` with `claims`, `group.c`).

Strict scorer (builder, own scratch `~/rush2049/scratch/frontier/w11e`):
```
IDO_DIR=…/ido python3 tools/cloud/score.py group cand/frontier_level_objects        -> exit 0
Members:
engine_sound_update:
  MATCH
engine_torque_calc:
  MATCH
transmission_ratio_get:
  MATCH
    own .rodata verified at 0x80121D40..0x80121D4C

Context (informational; excluded from exit status):
func_800AB7D0:
  MATCH
func_800AC660:
  MATCH
```
`score.py group --claims` also exits 0.

Whole-program unit (Pi, repo root):
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w11e score engine_torque_calc transmission_ratio_get \
  engine_sound_update func_800AB7D0 func_800AC660 --internal engine_torque_calc --internal func_800AB7D0 \
  --internal func_800AC660 --with cloud/work/frontier/w11e/groups/frontier_level_objects/group.c --neighbours
  EQUAL engine_torque_calc: 224 words (internal, c_group.c)
  EQUAL transmission_ratio_get: 452 words (kept, c_group.c)
  EQUAL engine_sound_update: 357 words (kept, c_group.c)
  EQUAL func_800AB7D0: 2 words (internal, c_group.c)
  EQUAL func_800AC660: 2 words (internal, c_group.c)
  locked bodies that differ in this unit: 0
blob_unit score: 5/5 equal; object build/blob_unit/w11e/unit.o (4.8s)
```
The string literals were checked against the retail image: `0x80121D40 5f425700` ("_BW"),
`0x80121D44 5f465700` ("_FW"), `0x80121D48 474f4c44` ("GOLD", then a zero word).

## Integration notes (for the integrator)

- Install the group under the **same name** `frontier_level_objects` (copy `group.json` without `"claims"`, plus
  `group.c`). It supersedes **no locked group**. `func_800AB7D0` and `func_800AC660` are locked caller-less stubs
  (empty singles). They stay as they are; the group lists them as `context` because it defines them for real.
- `src/blob/unit_overrides.json` needs entries for the shadow unit:
  - `prefer_definition`: `func_800AB7D0` and `func_800AC660` → `src/blob/groups/frontier_level_objects/group.c`
    (as for `func_800AFD54`/`func_800AFB30`). They are the retail remains of inlined helpers. With the locked
    empty definitions visible, the callers would not inline the real bodies.
  - `force_internal`: `func_800AB7D0` and `func_800AC660` (they must not be kept, so that they are inlined and
    their stubs reproduce, as for `func_80095CF4`). Add `engine_torque_calc` as well if the unit does not already
    make it internal from the group's keep list (keep = `engine_sound_update`, `transmission_ratio_get`).
    The scores above were run with exactly these three `--internal` flags.
- `transmission_ratio_get` owns three string literals at 0x80121D40..0x80121D4C (verified by the scorer).

## What closed it (in order of effect, from the w4c/w9e drafts at 204/224, 19/452, 273/357)

Start: w4c's `transmission_ratio_get`/`engine_sound_update` merged with w9e's `engine_torque_calc` +
`func_800AB7D0` helper (the merge is `eng/m0.c`; aligned rows 69/25/165).

engine_sound_update (165 → 0 rows):
1. **The slot reset is a 16-iteration loop over `((Slot136 *)D_80150E68[5])[i]`**, unrolled by 4 by uopt. That
   gives retail's 544-byte stride and 2176 bound. The offset loop with 4 explicit slots was fully unrolled.
2. **No `scene` variable.** Every access spells `((Scene36 *)((u8 *)D_801392D0 + offset))` (now a `SCENE` macro).
   The reloads after the flag store and after the call are aliasing reloads, not reassignments. With a variable,
   uopt made an address web for `&D_801392D0` and lost `-4097` in s8.
3. The found-search indexes `SCENE->resources[j]` (the induction pointer starts after the guard, as in retail).
   `zero.z = zero.x = zero.y = 0.0f` gives the store order. Declaration order `i, j, offset, enabled, other; node;
   zero; key; slot` gives the frame homes (zero at 124, node at 136). `slot` is unused but needed.
4. The last s0/s1 swap was not in this function. The cost of s1 for `node` came from the callee's IPA parameter
   register, and it was fixed in `engine_torque_calc`.

engine_torque_calc (69 → 0):
1. **One index variable** for the zeroing loop, the slot search and the truncated value, and a separate one (`j`)
   for the scene index. That gives retail's `slti` loop test and index v1 / -1 a0.
2. **`if (D_801392D0) {}` after the scene-pointer store**, a code-free dead read. It creates retail's `&D_801392D0`
   address live range (a0) and lowers node's priority (one more block), so matrix takes s0 and node s1. Node in s1
   is also the IPA register, which fixed both callers. Placed before the store it gives 127 rows instead of 10.
3. `D_80150E98[c] = D_80150E98[c] + D_80117510[c]` (not `+=`): this sets the t7/t8 temps.
4. **`func_800AB7D0` defined after `engine_torque_calc`.** The helper's `D_801174B4` test then has a later
   `.loc` than the entry constants (`la s3`/`li s4`). as1 breaks the aftercycles tie by line number, so `lui s3`
   is scheduled before `multu`. Retail has the same address order; only the line numbers move.
5. **`(node->flags & 16) == 0`** instead of `!(node->flags & 16)`. This is a ugen temp-ring pop. The traced ugen
   showed that the struct copy pops t7, t6, t9 then t9 when retail needs t8. The comparison form shifts the free
   list by one.

transmission_ratio_get (25 → 0):
1. **`md = &D_80117530[i]` reassigned at the top of every iteration**, instead of `md++`. The strength-reduced
   `&D_80117530[i]` is then a coloured expression web (uopt `f_spilltemps` gives it a temp slot). That is the
   missing early web that moves the two spill homes from 56/60 to 60/64. `md` stays a variable, so as1 still
   hoists `lw a0,4(s0)` above the spills. Indexing `D_80117530[i].f` directly gives the right homes but loses
   that schedule.
2. With that, the locals shrink by one word (`tail[8]`): cfe locals 88, then the 8-byte umerge rounding.
3. The entity-flags OR-in really is an inlined getter. Without it the a-register webs (a1 = table, a2 = 68,
   a0 = handle) are lost (63-158 rows for every tail size). A `static` getter leaves an unnamed `jr ra; nop`
   after `engine_torque_calc`, and `score.py` refuses that as "1 extra words". So it is written as the
   non-static `func_800AC660`, which reproduces that locked caller-less stub. **Attribution hypothesis:**
   `func_800AC660` sits after `differential_output`, the other caller of `transmission_ratio_get`. That makes it
   the nearest caller-less stub in the likely same file. Its body is not proved to be this getter
   (`differential_output` does not visibly inline it).

## Quirks the match depends on (also in the group.c header)

The dead read `if (D_801392D0) {}` (probably a compiled-out debug check), the `== 0` comparison form, the
definition order of the helper, the explicit `a = a + b`, and unused locals: `scene` in `func_800AB7D0`, `slot` in
`engine_sound_update`, and `pad152/pad148/pad128/pad124/tail[8]` in `transmission_ratio_get` (from w4c; retail
only shows the homes of i 156, res 144, node 136, flags 132, team 123). `D_80140BDC` must be `volatile`.

## Recovered layouts (see group.c)

Node112 (next, flags u8 @4, key @8, handle s32 @12, s16 metadata @16, matrix[9] @20, pos[3] @56, xform @68,
f80 s16, f84 f32, texture s16 @88, f90 s16, f92 s8, f96 s32, index s8 @101, f104 f32, state @108); Metadata48
(name, file, callback @8, animation @12, kind s16 @16, flags u16 @18, f20 s16, category s8 @22, element s8
@23, value f32 @24); Scene36 (name[16], flags @16, count s16 @20, action s16 @22, resources @28, key @32);
Resource68 (flags @64); Slot136 (object[3] @0, active s16 @134); Matrix64; Animation24; Pool (head @16);
Ent68 at D_8012E700 (flags @0, owner @8, id u16 @20).

## drone_ai_update + entity_tick_main (scouted only)

w5a's `drone_pair/best.c` (r5_e's full pass with literals) in the unit (`drone/d0.c`):
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w11e score drone_ai_update entity_tick_main --internal entity_tick_main --with cloud/work/frontier/w11e/drone/d0.c
  FAIL drone_ai_update: 781 of 820 words differ; compiled body is 794 words, target 820; +0x250: .rodata+0xa0: retail words encode 0x703c0000, outside the image
  FAIL entity_tick_main: 655 of 671 words differ; compiled body is 666 words, target 671; …
```
Aligned rows: 669 and 887. Frames are 224 against retail's 400 (drone_ai_update) and 216 against 368
(entity_tick_main). Retail saves only f20/f22, and the draft adds f24/f26. Retail calls entity_tick_main from
**one** site (0x80093C44), yet it stays out of line, so something else (a second, compiled-out call site, or size)
keeps umerge from inlining it. The draft has two call sites. Neither function is close. A strict match needs
the frame question (about 300 bytes of locals or inlined-helper areas in both) settled first. Next step: size both
frames with the w9e "inlined helper costs 8 bytes" sweep, using the traced uopt `spill.sh`/`AREA` lines (shared
toolkit) to see the merged Udef sizes directly.

## What generalises

1. **Fix the callee's colouring, and the callers' "s0/s1 swaps" go away.** An internal callee's IPA
   parameter register sets the cost of each s-register for the caller's variable (`p1cost` 20 against 21). Two of
   the three functions were blocked only by the callee.
2. **A family of spill homes off by +4 with an 8-rounded frame means one missing coloured expression web.**
   Reassigning a pointer from an index each iteration (`md = &T[i]`), instead of incrementing it, creates that web
   (the strength-reduced `&T[i]`) and keeps the variable for alias/schedule purposes.
3. **as1 breaks aftercycles ties by `.loc` line number.** Where an inlined helper is defined (before or after its
   caller) changes the schedule of the caller's prologue. Defining a deleted static *after* its caller is a lever.
4. **`x == 0` against `!x` is a free ugen temp pop** in a branch condition. With the traced ugen (`ugt.sh`,
   FREELIST), read the pops at the residual statement, then work out how many pops the preceding block must
   shift.
5. **A deleted static leaves an unnamed stub that `score.py` counts as extra words of the previous function.**
   Name it after the retail caller-less stub it reproduces.
6. Writing the full global expression at every use, instead of a cached pointer variable, reproduces retail's
   aliasing reloads and avoids an address web (`&D_801392D0`).

## Files

- `groups/frontier_level_objects/` — the deliverable (group.json with claims, group.c).
- `eng/` — variant history (m0 merge → m3 scene form → m9 decl order → e2 index var → g1/h2 dead read and
  operand order → x2 md reassignment → y1 helper order → ph5 temp pop → group.c); `esu.s`/`etc.s`/`trg.s` retail
  disassembly.
- `drone/d0.c`, `drone/dau.s` — the drone scout.
- `tools/` — `s3.sh` (scores the trio in the unit and prints aligned rows), `vr.sh` (batch of variants), plus
  copies of the w9e tools retargeted to tag w11e. The shared trace toolkit (`cloud/work/frontier/tools/trace`)
  is installed in my builder scratch.

No permission denials. Nothing committed, spliced or edited outside `cloud/work/frontier/w11e/` (and my builder
scratch).
