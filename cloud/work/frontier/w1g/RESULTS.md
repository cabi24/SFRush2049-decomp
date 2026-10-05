# Wave 1, agent w1g: joint (register-parameter) units

Builder scratch: `watchman2:~/rush2049/scratch/frontier/w1g`. Helpers in this directory:
`sg.sh GROUP` (push `groups/GROUP` and run `tools/cloud/score.py group`), `gf.sh GROUP FN[,FN] [--all|--sum]`
(instruction-aligned diff of group members via `gfull.py`), `mk.sh GROUP PARTSDIR ...` (concatenate
numbered part files into `groups/GROUP/group.c`, then `gf.sh`), `var.sh` (score a list of variant files).

## Unit 1: func_800AD4C8 + func_800C3AD0 + input_process_controller  (3 functions, 3,164 bytes)

Group: `cloud/work/frontier/w1g/groups/func_800AD4C8/` (`claims: [func_800AD4C8]`, `provisional`: the two callers).

| Function | Bytes | State | Flags |
|---|---:|---|---|
| `func_800AD4C8` | 264 | **strict MATCH**, real callers in the unit | `-g0 -O3 -mips2 -G 0 -non_shared` group |
| `func_800C3AD0` | 1,448 | MATCH, **provisional** (its own callers are stand-ins) | same |
| `input_process_controller` | 1,452 | MATCH, **provisional** (its own caller is a stand-in) | same |

Scorer (run in the scratch copy): `python3 tools/cloud/score.py group cand/func_800AD4C8`

```
Members:
func_800AD4C8:
  MATCH
func_800C3AD0:
  MATCH
input_process_controller:
  MATCH

Context (informational; excluded from exit status):
func_800AD650:
  MATCH
func_800AD5D0:
  MATCH
```
`--claims` judges `func_800AD4C8` only and also exits 0.

Why two are provisional: `func_800C3AD0` is called by `camera_trigger_check`, `camera_victory`, `entity_update`
and `input_process_controller` by `input_deadzone_apply`; all four are unmatched (300-900 words each, with their
own blockers), so each root has two stand-in call sites (`__standin_*`) to keep it out of line and non-`keep`.
`func_800AD4C8`'s only callers are these two functions, both byte-identical here, so its claim does not rest
on a stand-in. It is spliceable only together with the two callers' real closure, though (the group build needs
them present), so the maintainer decides whether to land it as a group with provisional context.

What it took (previous state 44/66, 317/362, 308/363 words off):

1. **`func_800AD4C8`: an inlined static.** `den = a.z*a.z + a.x*a.x` in `$f0`, evaluated before the numerator,
   only appears when the squared length is a tiny static function (`lsq(f32 *v)`) that umerge inlines; a named
   local is forward-substituted and the numerator is evaluated first. Operand order inside it: `v[0]*v[0] + v[2]*v[2]`
   emits z first.
2. **IPA parameter registers come from the callee's colouring, argument evaluation order from the source.**
   `func_800AD650` is locked as `(out, in)`; retail callers emit `addiu a1,poly,4; jal; move a0,mat`. Declaring it
   `(s16 *in, f32 *out)` (callee bytes unchanged, still MATCH: `out`->a0, `in`->a1 by colouring) gives the retail
   order in both callers. So for an internal callee, the order in which callers set up argument registers is
   the source parameter order, and the registers say nothing about it.
3. **Two zero constants.** `0.0f` (stores, and float compares in `input_process_controller`) and the cross-product
   compare zero are different registers in retail (`$f2`/`$f12`, `$f22`/`$f2`). `c < 0` (int literal) against a
   named `f32 c` gives that; `c < 0.0f` shares one register. Unnamed, IDO folds `(a - b) < 0` into `c.lt.s a,b`.
4. **Frame = named scalars + uopt temp words.** The 24(sp).. area holds uopt temporaries (two used as float spills
   in the edge loop). Naming the vertex pointer (`PV *e`) *removes* one temp word; naming the index (`s32 i`, needed
   to get it into `$a3`) adds a normal slot. With both named and `u16 idx[16]` the retail layout falls out in both
   functions (`input_process_controller`: `k, n` above `idx`, `t, c, i, e` between `vprev` and `vd`). Probe used:
   add one dummy `s32` at the top and watch whether the frame grows, which separates real size from 8-byte padding
   (padding sits at the bottom of the locals).
5. `s32 k` compared with `u32 n` keeps `sltu at; bnez at` (with `u32 k` IDO rewrites the exit test to `bne`).
6. `i + D_8015201C` vs `&D_8015201C[i]`, `D_80152568 + poly->off`, `out[n] = out[n] + v0[n]` (not `+=`, not
   `v0[n] + out[n]`): commutative operand order follows the source but not as a simple rule; the six products of the
   three cross tests were settled by a 64-variant search (`ad4c8v/m4`, one variant strict).

Recovered types: `Poly { u16 type; u16 cnt; s16 rot[9]; u16 off; }` (0x18), `PV { s16 x, y, z; u16 w; }` at
`*D_8015201C` (5 fraction bits of each coordinate packed in `w`), `D_80152568` = base of packed index lists,
`D_8011418C` = `f32[][3]` whose element 3 is the zero vector, `D_80123F70..7C` = per-edge radius-squared floats.

## Unit 2: func_800E4300 + func_800E451C  (2 functions, 2,128 bytes)

Group: `cloud/work/frontier/w1g/groups/func_800E4300/` (`claims: [func_800E4300]`, `provisional: [func_800E451C]`).

| Function | Bytes | State | Flags |
|---|---:|---|---|
| `func_800E4300` | 540 | **strict MATCH**, its only (real) caller in the unit | `-g0 -O3 -mips2 -G 0 -non_shared` group |
| `func_800E451C` | 1,588 | **code identical, own-rodata unverified** (10 relocations, 5 float literals) and **provisional** (caller `func_800E4B58` is a stand-in) | same |

Scorer: `python3 tools/cloud/score.py group cand/func_800E4300`

```
Members:
func_800E4300:
  MATCH
func_800E451C:
  MATCH (10 section-relative relocations unverified: .rodata+0x0 at +0x120, .rodata+0x0 at +0x148, .rodata+0x4 at +0x1b8, .rodata+0x4 at +0x1bc, .rodata+0x8 at +0x2e0, .rodata+0x8 at +0x2f8, .rodata+0xc at +0x334, .rodata+0xc at +0x354, .rodata+0x10 at +0x380, .rodata+0x10 at +0x384)
```
(exit 1 because of the unverified member; `--claims` judges `func_800E4300` only: `MATCH`, exit 0.)
The five literals are in retail order and at the right relative offsets (`.rodata+0..0x10` = `0x80124440..50`:
2500, 73.33333, 0.01, 0.1, 1.4666667); `func_800E451C` closes with workstream B (own rodata) plus its caller.

Previous state: 80/135 and 388/397 words off after three cloud rounds and a codex packet.

**Arcade ancestor (proven by structure and constants):** `func_800E451C` is `MP_IntervalPos(m, cp, change_flag)`
from `reference/repos/rushtherock/game/maxpath.c:893`, with the static `mp_interval_pos()` (line 1133) inlined
at its end. The caller-less `jr ra` stub `func_800E4B50` directly after it is the deleted `mp_interval_pos`.
`func_800E4B58` calls it three times (`true`, then `false` after next/prev) = `MP_FindInterval` inlined into
its caller. Following the arcade statement order and **its local declaration list** took the function from
388/397 to code-identical: the "35 unused 4-byte slots" noted in the old STATUS are the arcade's named locals
(each named scalar keeps a slot) plus the inlined function's two parameters and locals.

What it took:

1. `func_800E4300`: (a) write every table use as `D_8012E5E8[cur].field` with **no pointer or count local**; the
   `tbl`/`n` locals made uopt hoist the table address into `$s1` across both branches and rotated every loop
   invariant. Retail's else-branch `addu a1,idx*8,base` is PRE re-materialising the entry. (b) `win = win*2+1`
   reuses `win` as the trip count (the separate `cnt` cost a register and moved `ptIdx` from `$t5` to `$s0`) and
   comes after the second wrap loop.
2. `func_800E451C`: arcade order and names; N64 differences: `S32 i` with `index = i`; `nmpi` s32; `cur_time`
   is the f32 clock; nearest point through `func_800E4300` on an `S16 pos[3]` copy; weights are constants.
3. **Inlined-callee parameter**: `mp_interval_pos(m, cp)` taking the car, not `m->RWR`: with a pointer-to-field
   argument the inlined parameter stays a register (`addiu v0,s5,556`); a plain variable argument is
   copy-propagated (`lwc1 556(s5)`).
4. **Dead code still shapes register colouring.** The arcade's `invdist = (len > 0.01) ? 1/len : 0` is dead in the
   N64 version (the vector is normalised by `func_800BDD90`), emits nothing, but the block boundary it leaves is what
   lets `x`/`y` take `$f0/$f2` (otherwise `$f2/$f12`: the allocator works per basic block, and `$f0` is the call's
   return register in that block). A final residual that is "two float variables one register off after a call"
   should make one look for a deleted conditional in the ancestor.
5. `num_drones` (`D_80152768`) is re-read after every store to `current_drone` in retail; only `volatile` on the
   extern reproduced that (a pointer to `current_drone` did not). Recorded as a type fact, cause unknown.
6. Literal types: `(1 - x)` int 1 gives the freshly materialised `1.0f`, `1.0f` gives the shared `$f28` copy;
   `xy_speed == 0` int 0; 73.33333f used twice is the loop-invariant `$f30`. Extern floats inside the loop get
   their address hoisted (`lwc1 0(s8)`), so the literals are natural here.
7. `func_800E4300` stays out of line with a single call site (no stand-in needed for it).

Recovered types: `Nav` = arcade `MPCTL` (`xrel` 0, `yrel` 4, `cyrel` 8, `len` 0xC, `interval_time` f32 0x20,
`mpi` s16 0x24, `new_mpi` 0x26, `mpath_index` 0x28, `default_path` 0x2A); `TrackPt` = N64 `MPATH`
(`s16 pos[3]`, `u8 speed` mph at +6, 8 bytes); `Track { u16 numPoints; TrackPt *points; }` ×4 at `D_8012E5E8`;
car/`MODELDAT` (0x808): `RWV` f32[3] +0x220, `RWR` +0x22C, `reckon.RWR` +0x794, `net_node` s16 +0x7C6,
`we_control` s16 +0x7CA, path row s16 +0x7E2. Globals: `D_801543CC` f32 time, `D_80154190` last_mpath_time,
`D_80154182` s16 current_drone, `D_80152768` s16 num_drones, `D_801527D8` s16 drones[].

## Unit 3: cpak_init + func_800AF8C0 + save_validate  (3 functions, 2,052 bytes)

Group: `cloud/work/frontier/w1g/groups/cpak_init/` (`claims: [func_800AF8C0, save_validate]`).
**Closed real unit: all eight callees are present as unchanged context, no stand-ins anywhere.**

| Function | Bytes | State | Flags |
|---|---:|---|---|
| `func_800AF8C0` | 452 | **strict MATCH** | `-g0 -O3 -mips2 -G 0 -non_shared` group |
| `save_validate` | 540 | **strict MATCH** | same |
| `cpak_init` | 1,060 | **code identical, own-rodata unverified** (2 relocations: the `0.1f` literal at `0x80123C0C`) | same |

Scorer: `python3 tools/cloud/score.py group cand/cpak_init`

```
Members:
cpak_init:
  MATCH (2 section-relative relocations unverified: .rodata+0x20 at +0x94, .rodata+0x20 at +0x98)
func_800AF8C0:
  MATCH
save_validate:
  MATCH

Context (informational; excluded from exit status):
func_800AF844:
  MATCH
vector_copy_scale:
  MATCH
func_8008B424:
  MATCH
func_800A78BC:
  MATCH
func_8008D0C0:
  MATCH
func_800AFA84:
  MATCH
func_8008E3C0:
  MATCH
func_8008C074:
  MATCH (2 section-relative relocations unverified: .rodata+0x0 at +0xe8, .rodata+0x0 at +0xf0)
func_800AFB30:
  MATCH
func_800AFD54:
  MATCH
```
(exit 1 for the unverified literal; `--claims` exits 0 with both claims `MATCH`.) `cpak_init` closes with
workstream B; an extern for `0.1f` is not an option (not hoisted out of the loop, frame and registers change).

Previous state: 13/113, 123/135, 245/265 words off.

**Arcade ancestor (structure, constants, statement order):** the skid-mark code in
`reference/repos/rushtherock/game/visuals.c`: `func_800AF8C0` = `UpdateSkid`, `save_validate` = `StartSkid`
with `GetSkid` inlined, `cpak_init` = `DoSkid` for the four tires with `ContinueSkid` inlined,
`func_800AF844` = `StopSkid`. The three caller-less retail stubs here are the deleted inlined functions:
`func_800AFD54` (between `StartSkid` and the driver) is `ContinueSkid`, `func_800AFB30`/`func_800AFB28`
(between the unlink helper and `StartSkid`) are `GetSkid` and one more.

What it took:

1. **Inlined functions written as non-static, non-kept functions named after the retail stub.** A `static`
   that umerge inlines leaves a `jr ra; nop` stub with no global symbol, and the scorer then counts it as
   "1 extra word" of the preceding member. Declared as a normal function `func_800AFD54`/`func_800AFB30`
   and left out of `keep`, it is still inlined (one call site), the stub gets its symbol, lands where retail
   has it, and scores `MATCH` as context. This is direct evidence for plan decision D3.
2. **The callee closure must be real, and `keep` must be exact.** With extern (or kept) `func_800AF844`,
   `cpak_init` keeps its loop state in `$s1/$s4/$s5` plus one memory slot. With the real `func_800AF844`
   in the unit and *not* in `keep`, IPA knows it preserves `$t0-$t4` and the allocator produces retail's
   `$t0/$t2/$t3/$t4` with save/restore only around the other calls. The other seven callees are kept.
   Fake leaf bodies for the callees change everything (the callees' own parameter registers move).
3. `cpak_init(int slot)`: no `s16` truncation at entry; the `S16 slot` parameters of the callees are read back
   with `lh` from the home slot. The per-tire body is in the loop itself (not an inlined `DoSkid`): `m` and
   the `NewSkid` row are computed from the untruncated `$a0`.
4. `GetSkid` as its own function gives `move v0,zero / move v0,s0; move s7,v0` (inlined return value) and the
   arcade loop `fars = list; for (s = list; s->next; s = s->next)` gives the double load of `list->next`.
5. Camera position is `D_80150B70.pos` (`+0x24` in a 3x3-plus-position block), not a separate `D_80150B94`:
   the strength-reduced loop pointer starts at the struct base.
6. Frame of `cpak_init` (216): `i, m, ns, on, laston, color`; inlined `ContinueSkid` = 4 parameters, two
   words IDO adds for the inlined call (present with 4 parameters, absent with 3), `vec[3]`, `dirpos[3]`,
   `lensq, len, ds, invlen`. No named `car` (it costs a word), squared length as a macro (an inline function
   costs its parameter word). Colour is `(sviscode == 0) ? A : B` (the `if` form has no `b` and swaps `$t0/$t2`).
7. `func_800AF8C0`: natural source has `+= 0.8f` twice (code identical, 2 unverified). Reading the same word
   through `extern f32 D_80123C08` into a local declared *above* the two vectors gives the same code strictly.

Recovered types: `NewSkid`/`WheelSlot` (0x5C): `Skid *skid; f32 start[3]; f32 end[3]; f32 lensq; f32 dir[3];
f32 vert[4][3]` at `D_80155290[car][4]`. `Skid`/`EffectObj`: `next` 0, `f32 lastTime` 8, display handle 0xC,
`f32 pos[3]` 0x10, `u8 color[4]` 0x1C, `s32` 0x20; pool at `D_80155220` (list head at +0x10). Car
(`player_array`, 0x3B8): `f32 dr_tirepos[4][3]` +0x74, `u32 appearance` +0xE8. `MODELDAT` (`D_8014A250`):
`u16 surface[4]` +0x5A0, `u16 sviscode[4]` +0x61C, `s16 resurrect.moving_state` +0x6C4, `f32 suscomp[4]`
+0x75C. `D_8011743C[4]` skid appearance masks, `D_80117438`/`D_8011AD8C` mark colours,
`D_801497F8[n].flags & 0x30` surface test, `D_8002EB90` f32 clock.

## Unit 4: func_800C1B60 and its callers  (5 functions, 4,836 bytes)

Group: `cloud/work/frontier/w1g/groups/camera_scene_manager/` (`claims: [func_800C3578]`). No stand-ins.

| Function | Bytes | State |
|---|---:|---|
| `func_800C3578` | 148 | **strict MATCH** |
| `func_800C1B60` | 1,188 | **code identical, own-rodata unverified** (4 relocations: its two jump tables) |
| `func_800C2004` | 520 | **code identical, own-rodata unverified** (2 relocations: `0.15f`) |
| `func_800C220C` | 524 | **code identical, own-rodata unverified** (2 relocations: `0.15f`) |
| `camera_scene_manager` | 2,456 | 452/614 words differ (was 552); 151 aligned rows; notes in `camera_scene_manager/NOTES.md`, source `camera_scene_manager/best.c` |

Scorer: `python3 tools/cloud/score.py group cand/camera_scene_manager`

```
Members:
func_800C1B60:
  MATCH (4 section-relative relocations unverified: .rodata+0x0 at +0xb0, .rodata+0x0 at +0xb8, .rodata+0x30 at +0x2f0, .rodata+0x30 at +0x2f8)
func_800C2004:
  MATCH (2 section-relative relocations unverified: .rodata+0x50 at +0x10c, .rodata+0x50 at +0x120)
func_800C220C:
  MATCH (2 section-relative relocations unverified: .rodata+0x54 at +0x10c, .rodata+0x54 at +0x120)
func_800C3578:
  MATCH
camera_scene_manager:
  452/614 words differ (12 section-relative relocations unverified: ...)

Context (informational; excluded from exit status):
func_800C2944:
  MATCH
func_800C26C4:
  MATCH
func_800C2430:
  MATCH
```
The unit's .rodata offsets are retail's relative layout (jump tables at +0 and +0x30, the two `0.15f` at +0x50
and +0x54 = `0x80123E94/EC4/EE4/EE8`). The three stand-in callers of the old group are gone: the three locked
members stay out of line and still match with their single real call site each.

What it took (previous state: 13, 8, 8, 2 aligned rows off; "only the register carrying add differs"):

1. **`func_800C1B60(idx, code)`, not `(code, idx)`.** All callers set `$t1` (idx) before `$a0` (code); argument
   set-up order is source order, registers are the callee's colouring. This alone fixed the `li a0,N` /
   `move t1,s1` order in `func_800C2004/220C`.
2. **A never-passed third parameter.** Retail's frame is 32 with two local words; the natural body has three
   named locals (`p`, `hits`, `i`) and gets 40. `register` does not help, and reusing `code` as the counter
   moves the incoming parameter to `$a1`. Declaring `hits` as a third parameter in an old-style definition
   (callers pass two arguments) gives the frame and keeps `$a0`/`$a1`. Whether the original really did that or
   this stands for something else is open; the bytes are identical.
3. **One variable for two jobs = one register.** The points value and the later loop counter are one variable
   (`i`): that is what puts it in `$v1` with a store to its home in every `switch` arm.
4. `func_800C2004/220C`: plain `while (r > 0.0f) { ...; r -= 0.15f; }` with literals. The hand-hoisted
   `zero`/`dt` locals of the earlier draft were the residual (`0.25f` in `$f2` instead of `$f0`).
5. `func_800C3578`: no named locals.
6. `camera_scene_manager`: chained zero assignments, `++` counters, corrected event codes and field; see NOTES.

Recovered: `Stunt` (`D_80152038`, 0x78): `count` s32 +0x18, `lastcode` +0x1C, `cnt[10]` s16 +0x20,
`f60` f32 +0x60, `mult` +0x68, flag +0x74; `Ply` (`D_801569B8`, 0x7C): `flags` s32 +0, accumulators f32
+0x28/2C/30, timer pairs +0x34..0x54. Wheel-contact counters `D_80157238` (total), `D_8015B248/B258`
(per axle), `D_8015F728/F730` (per side).

## Unit 5: controller_poll  (928 bytes, `group` recipe)

Not matched. `cloud/work/frontier/w1g/controller_poll/best.c` + `NOTES.md`; group at
`cloud/work/frontier/w1g/groups/controller_poll/` (no claims).

Scorer: `python3 tools/cloud/score.py group cand/controller_poll` ->
`controller_poll: 225/232 words differ (2 extra words (nonzero beyond target length))`
(context: `func_800C9590`, `player_mode_set`, `player_state_set` `MATCH`; `func_800E7038` 61/63,
`func_800E7134` 166/169, `process_inputs` 86/89 untouched). Instruction-aligned: 124 rows of 232 (was 212,
with 23 words missing).

Residual lane: register allocation (which sixteen values get the sixteen registers). What moved it:
`controller_poll` in `keep`; the frame counter as a `volatile` field at +636 of the block at `0x8002E8E8`
(retail `lw 636(a3)` at every use); `x`/`y` locals; `held` reloaded through `(u32 *)`. Stopped after the
message-queue address could not be kept out of a register (details and next hypothesis in NOTES).

## What generalises

1. **Argument set-up order is the source parameter order; registers are the callee's colouring.** For an
   internal callee the registers say nothing about parameter order. Three units needed a swapped signature
   (`func_800AD650(in, out)`, `func_800C1B60(idx, code)`), found from `addiu a1; jal; move a0`-style order
   at the call sites. Check this before anything else on a caller that is "one delay slot off".
2. **Caller-less `jr ra` stubs are deleted inlined functions, and they can be written as such.** Define the
   inlined function as a normal (non-static) function named after the stub and leave it out of `keep`:
   umerge inlines it into its single caller, the stub is emitted with its symbol at the retail address and
   scores `MATCH`. Done for `func_800AFD54` (ContinueSkid) and `func_800AFB30` (GetSkid); `func_800E4B50` is
   `mp_interval_pos`; `func_800C2418/2420/2428` are three more siblings of `func_800C2004`. A `static`
   leaves an unnamed stub that the scorer counts as an extra word of the previous function.
3. **Inlined bodies explain "unused" frame slots and odd temporaries.** An inlined function's parameters and
   locals take words in the caller's frame; its return value comes back through `$f0`/`$v0` and a move
   (`lsq()` in `func_800AD4C8`, `GetSkid()` in `save_validate`). A dead statement inherited from the ancestor
   (`invdist = ...` in `mp_interval_pos`) still splits a basic block and changes register colouring.
4. **Use the arcade source as the declaration list, not only for logic.** `func_800E451C` went from 388/397
   to code-identical by following `MP_IntervalPos`' statement order and its local declarations (frame 232 and
   every spilled local's slot fell out). Ancestors found this pass, all by constants and structure:
   `maxpath.c: MP_IntervalPos/mp_interval_pos`, `visuals.c: DoSkid/StartSkid/ContinueSkid/UpdateSkid/GetSkid`.
5. **Close the callee side too, and get `keep` exactly right.** `cpak_init` only took retail's `$t0-$t4`
   colouring when `func_800AF844` was present *and internal*; `controller_poll` only has the right size when it
   *is* kept. A stand-in or a wrong `keep` entry is a silent residual in the caller.
6. **Frame = named scalars + uopt temporaries, and only when the function has a locals area at all.**
   Functions with arrays (or a memory-resident scalar) give every named scalar a word; functions without
   (`camera_scene_manager`, `func_800C2004`) give none. Naming a pointer CSE removes a temp word; `idx[16]`
   vs `idx[20]`-style size questions can be settled with a one-word probe local (padding sits at the bottom).
7. **One variable, one register.** Reusing a variable for a second job (points/loop counter in
   `func_800C1B60`, window/trip count in `func_800E4300`) is what puts both uses in the same register;
   conversely a parameter reused as a local drags the incoming register with it.
8. **Chained assignments are visible in the code**: `a = b = c = 0;` stores through address registers and
   keeps zero copies in registers; separate statements use `lui at` stores. `a = a + b`, `a += b` and
   `a = b + a` are three different load orders.
9. **Literal types and volatility**: int `0`/`1` against a float gives a constant separate from `0.0f`/`1.0f`
   (two registers in retail = two spellings in source); in-loop float literals must stay literals (an extern's
   address is hoisted), which today means "own-rodata unverified" for five of this pass's functions;
   `volatile` is real in this code base (`num_drones`, the system block at `0x8002E8E8`).
10. `s32 k` compared with `u32 n` keeps `sltu; bnez`; a plain `while` with literals lets uopt hoist the
    constants itself (hand-hoisted locals were the residual in `func_800C2004/220C`).

## Summary table

| Unit | Function | Bytes | State |
|---|---|---:|---|
| 1 | `func_800AD4C8` | 264 | strict MATCH (real callers in unit) |
| 1 | `func_800C3AD0` | 1,448 | MATCH, provisional (stand-in callers) |
| 1 | `input_process_controller` | 1,452 | MATCH, provisional (stand-in caller) |
| 2 | `func_800E4300` | 540 | strict MATCH (real caller in unit) |
| 2 | `func_800E451C` | 1,588 | code identical, own-rodata unverified (10); provisional (stand-in caller) |
| 3 | `func_800AF8C0` | 452 | strict MATCH (closed real unit) |
| 3 | `save_validate` | 540 | strict MATCH (closed real unit) |
| 3 | `cpak_init` | 1,060 | code identical, own-rodata unverified (2) |
| 4 | `func_800C3578` | 148 | strict MATCH |
| 4 | `func_800C1B60` | 1,188 | code identical, own-rodata unverified (4, jump tables) |
| 4 | `func_800C2004` | 520 | code identical, own-rodata unverified (2) |
| 4 | `func_800C220C` | 524 | code identical, own-rodata unverified (2) |
| 4 | `camera_scene_manager` | 2,456 | 452/614 words (151 aligned rows) |
| 5 | `controller_poll` | 928 | 124 aligned rows of 232 |

Strict `MATCH`: 5 functions / 1,944 bytes without any stand-in dependence, plus 2 / 2,900 bytes provisional.
Code-identical pending own-rodata verification (workstream B): 5 functions / 4,880 bytes.
No file outside `cloud/work/frontier/w1g/` was changed; nothing was spliced or committed. No
`cloud/matches/*.c` were written: every result here is a group result (see each group's `claims`).
