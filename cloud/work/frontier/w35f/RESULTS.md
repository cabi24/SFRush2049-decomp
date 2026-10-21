# w35f results (MAT3-donor matching, image B shots.c / weapons.c)

Builder scratch: watchman2 `~/rush2049/scratch/frontier/w35f` (copy of base; src/include/tools/asm/lock files
synced from the Pi at session start). Helpers in `tools/`: `g.sh GROUPDIR [args]` (rsync + `gscore.py group` on the
builder: per-member word counts + aligned diff via `--diff NAME [--norm]`), `c1.sh FILE.c` (compile one file on the
builder and objdump it), `addrs.py`, root `odis.py FN` (image-B disassembly), `odata.py ADDR N` (image-B data words).

## 0. Status (updated as work proceeds)

| function | GC identity | bytes | state |
|---|---|---|---|
| func_8038FCE0 | ShotUpdate (shots.c) CONFIRMED | 3056 | in progress |
| func_8038E114 | UpdateShots (shots.c) CONFIRMED | 5196 | in progress |
| func_8038AA8C | AnimateWeapon (weapons.c) CONFIRMED | 7812 | MATCH in the combined image-B unit (groups/ovlb), see 2.2 |

## 1. Key structural finding: FCE0/E114 cannot be matched alone

The whole shots.c static cluster is one private-IPA unit. The retail code shows IDO -O3 register-parameter
conventions (callee params live in callee-saved registers, no save/restore, caller saves only what the callee
clobbers). Identified (all confirmed against MAT3 `shots.c`):

| N64 | GC | private convention seen in retail |
|---|---|---|
| func_8038D200 | `_UpdateShotObject(shot, unt)` | shot in s1, unt in a0 |
| func_8038D328 | `_CreateShotObject(idx, parent, flags, o2)` | s1,s2,s3,s4; returns the object |
| func_8038D498 | `SonicCarCollision(shot)` | shot in s2 |
| func_8038DA78 | `ShotToCarCollision(shot, old)` | s1, s2; returns hit car |
| func_8038E088 | `ShotDone(shot)` | shot in s1 |
| func_8038E114 | `UpdateShots()` | void, clobbers s0-s8 / f20-f30, saves only ra |
| func_8038F568 | `ElevateGun(car)` | ordinary (a0), saves everything |
| func_8038F938 | `UpdateDefense(car, m, defense, i)` | car s2, model s3, defense s0, i s5 |
| func_8038FCE0 | `ShotUpdate()` | root; inlines AnimateCase and _InitNewShot |

Already-matched partners (context, unchanged): D054 NewStreak, CB20 UpdateStreaks, D3A4 BattleDamage,
D798 ProximityDamage, DDDC MissileLock; game callees: input_deadzone_apply = ShotToWorldCollision,
save_write_data = StartBlast, transmission_ratio_get = SetupTarget, camera_collision_check = EmitGameSound,
func_800A61B0 = BodyVector, func_8009E820 = WorldVector, func_80090F44/E9C = PitchUV/YawUV, func_8009EA68 = RollUV,
math_utility = CopyUV, func_8008B2E4 = Random, func_8008C768 = fatan2, func_8008B32C = ScaleUV,
sound_call_minimal = ZOID_RemoveObject (s16), func_8008E06C = ZOID_SetColor (s16), func_8008D870 = ZOID_SetTexture,
sign_extend_call = MBOX_NewObject (returns the index in v0), func_8008E3C0/func_800AFA84 = Get/FreeStatLLE.

Working group: `groups/shots/` (`group.c`, `group.json`). Score with `tools/g.sh $PWD/groups/shots`.
First compile of the whole cluster: D200 and D328 MATCH in the group (private conventions reproduced).

## 2. Progress log (appended as work proceeds)

### 2.1 Group-level IPA works: strict matches inside the shots cluster (builder, `score.py group`-equivalent via tools/g.sh)

Group dir `groups/shots/` = my new functions in `group.c` + the five already-matched partners as separate files
(`ctx_cb20.c`, `ctx_d054.c`, `ctx_d3a4.c`, `ctx_d798.c`, `ctx_dddc.c`, unchanged copies of src/ovl_b/groups/*), flags
`-g0 -O3 -mips2 -G 0 -non_shared`, keep = FCE0 + the five partners. Files order [cb20,d054,d3a4,d798,dddc,group.c]
gives emission order CB20 D200 D328 D3A4 D498 D798 DA78 DDDC E088 E114 (D054) F568 F938 FCE0, very close to retail.

| function | state in the group build |
|---|---|
| func_8038D200 `_UpdateShotObject` (296) | MATCH (private convention shot=s1, unt=a0 reproduced) |
| func_8038D328 `_CreateShotObject` (124) | MATCH |
| func_8038E088 `ShotDone` (140) | MATCH (needs a local `s32 idx = o->mbIdx` before `sound_call_minimal(idx)`, see 3.1) |
| func_8038D498 `SonicCarCollision` (768) | MATCH |
| func_8038F938 `UpdateDefense` (928) | MATCH |
| the five context partners | still MATCH in the enlarged unit |
| func_8038DA78, F568, E114, FCE0 | in progress |

### 3.1 Lessons that generalise (all confirmed by compile)

* `gFrameSeconds` (0x8002EB94) is `extern volatile f32`: that is what makes retail load its address with
  `lui/addiu` + `lwc1 0(reg)` per use (each volatile access recomputes its own address web) instead of IDO's folded
  `lui at; lwc1 off(at)` for plain externs. `gNumCars` (0x801543CA) is `volatile s16` too (reloaded every iteration).
  Plain `extern f32` (gGravity 0x80142764) keeps the folded form.
* Private-IPA functions (D200/D328/E088/D498/F938) are reproduced by putting them in the same `uld -kp` unit as their
  callers and leaving them OFF the keep list; parameter registers (s1..s5) come out like retail.
* Stack-slot arithmetic: every named local costs a 4-byte slot (arrays 4*n), first declared = highest address
  (function scope). A GC local that disappears in the N64 build moves frames by 4 (e.g. D498: `m` is assigned AFTER
  BodyVector, `radsq` is declared but unused, no `o` local: `shot->obj->...` is re-read each time).
* A variable that retail keeps in memory (D498 `our` at sp+96) is just a web that lost the colouring; it was a plain
  local, not an expression.
* `x = x + y` vs `x += y` flips commutative operand order (D498 centerForce/centerMoment).
* UpdateDefense's f10 field (Defense+0x10) is an extra N64 "wobble" angle: `PitchUV(d->w, mat); d->w -= 0.005f`.
* UpdateDefense indexes `D_80394F90[i]` everywhere (no `shield`/`mat` pointers): keeping them cost two slots.

### 2.2 AnimateWeapon (func_8038AA8C) MATCHES; the "FP temp rotation" was an operand-ORDER difference

The long-standing "constant 2-position FP temp rotation" residual in AA8C was NOT a ugen free-list artefact. The
register-normalised diff (`norm 0`) hid that 54 of the 73 `add.s` had swapped commutative operands (retail
`add.s d,mul,load`, mine `add.s d,load,mul`). Different operand roles change the order temps are freed, which rotates
the FP temp ring for the whole rest of the function (shift varying by region, 14-row runs), hence ~406 differing rows.
Source fix (all 54 statements): write the accumulation as `pos = pos + uvs * off` instead of `pos = off*uvs + pos`:
for plain `=` IDO emits the add with its operands reversed relative to source, and the multiply with its operands
reversed when the add's left operand is a bare load (see lessons 3.2). Result: `func_8038AA8C: MATCH`, 1953 words,
frame -136 (still needs two unused 4-byte locals pad1/pad2 after off[3]; disclosed exact frame residual).

Tool: `tools/roles.py WANT.dis GOT.txt` compares FP dataflow ROLES (which earlier row defines each FP source operand)
over a difflib alignment; `tools/rc.sh GROUPDIR FUNC DISFILE` dumps our rows and runs it. Run it whenever a function
is "structurally identical but register-different": a role-differing row count > 0 is the lever, not the ring.

### 2.3 Landed (coordinator, 2026-10-20, after the lane stopped at Sonnet's session limit)

AnimateWeapon does not need the shots cluster: `groups/w35f_weapons/` (weapons.c alone; members/claims AA8C, AA14,
A95C; keep AA8C, which StartWeapon takes the address of) scores MATCH for all three, and `ovl_splice group` installed it
as `src/ovl_b/groups/w35f_weapons` with own data verified (image B gate byte-identical, ROM SHA-1 exact; log
`w35i/splice_w35f_weapons.log`, `w35i/gates_batch4.log`). `groups/ovlb` stays the working unit for the shots functions;
drop weapons.c and ctx_a408.c from it (or keep them as context) when resuming.
