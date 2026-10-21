# w36h results (wave 36, image B shots.c; continues w35f)

Builder scratch: watchman2 `~/rush2049/scratch/frontier/w36h` (copy of w35f's scratch, then src/ asm/ include/
tools/cloud tools/conveyor and the three lock files re-synced from `base` = master e68aea6e1). Lane tools copied from
w35f into `tools/` (paths retargeted to w36h): `g.sh GROUPDIR [gscore args]`, `runv.py`, `vs.sh`, `rc.sh`/`roles.py`,
`c1.sh`; `odis.py FN` (image-B disassembly), `odata.py ADDR N`.
Working unit: `groups/ovlb/` (copied from w35f).

## 0. Status

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| func_8038E114 | UpdateShots (MAT3 shots.c) confirmed by w35f | 5196 | **MATCH in the group** (two disclosed unused frame slots), see 1.11 | groups/w36h_shots/shots.c |
| func_8038FCE0 | ShotUpdate (shots.c) confirmed by w35f | 3056 | **MATCH in the group** (one disclosed compiled-out read, see 1.8) | groups/w36h_shots/shots.c |
| func_8038F560 | _InitNewShot deleted-procedure stub | 8 | **MATCH** (umerge-inlined helper's stub) | groups/w36h_shots/shots.c |
| func_8038F568 | ElevateGun (shots.c) | 976 | **MATCH** in the group and standalone (`score.py fn`), see 1.10 | groups/w36h_shots/shots.c; func_8038F568/standalone.c |
| func_8038DA78 | ShotToCarCollision (shots.c) | 868 | **MATCH in the group** | groups/w36h_shots/shots.c |
| D200/D328/E088/D498/F938 | private helpers (_UpdateShotObject, _CreateShotObject, ShotDone, SonicCarCollision, UpdateDefense) | 296+124+140+768+928 | **MATCH in the group** (claimed) | groups/w36h_shots/shots.c |

## Deliverable (current)
`groups/w36h_shots/` = shots.c + six byte-identical copies of installed partners (ctx_cb20/d054/d3a4/d798/dddc = the
dot_runtime_b_* group.c files, ctx_c910 = src/ovl_b/func_8038C910.c; `cmp` identical).
group.json: **members = claims = D200 D328 E088 D498 F938 DA78 FCE0 F560 F568 E114** (296+124+140+768+928+868+3056
+8+976+5196 = 12,360 B: every shots.c function of image B); context = the six partners; keep = FCE0 F568 + the six
partners (FCE0 is called from the game; F568 is retail's ordinary-ABI function).
Builder, WITHOUT --claims (`score.py group cand/w36h_shots --targets asm/us/ovl_b`): all 10 members
`MATCH (N section-relative relocations unverified ...)`, the six context partners MATCH; exit 1 solely from
57 x "not verified: ...: no retail data available" (the scratch has no image-B own-data artefact; 0 other unverified
reasons); `--allow-unverified` exit 0. Full output: `groups/w36h_shots_score.txt`.
Own data, checked on the Pi against build/ovl/b/image.bin (`tools/ownchk.py`, `tools/refchk.py`; output
`groups/w36h_shots_owndata.txt`): D200 D328 E088 D498 F938 DA78 F560 pass owndata outright (F938's .data incl.
gShieldSizes at 0x80394358). FCE0, F568 and E114 fail only owndata's one-base rule: they read two or three retail
.rodata regions (const tables CaseOffsets/ShotOffsets at 0x80394AFC/0x80394B08; E114's "WPR_MINE"/"HIT_WALL" at
0x80394CFC/0x80394D08; the literal pool from 0x80394E14) that retail's whole-image unit separated with other files'
rodata. Per-reference content check: FCE0 15/15, F568 4/4, E114 16/16 referenced words equal retail (jump tables
relocated, strings compared in full). So own-data contents are proven; the install needs the group path's
per-reference proof (cf. WAVE_BRIEF wave 16 on two-region .rodata). An extern D_ name for the tables does not help
(IDO still emits section-relative relocations for a table defined in the unit).
Not produced: retail stub func_8038FCD8 (AnimateCase's deleted procedure); AnimateCase is written inline because as
a helper it costs a frame quantum (1.8). Its 8 bytes stay retail.

## 1. Log

### 1.1 func_8038DA78 ShotToCarCollision: MATCH in the unit
Two source changes from w35f's draft: (1) `projected[i] = old[i] + dir[i] * t;` (operand order: retail
`add.s d,mul,load`); (2) `hit = car;` before `t0 = t;` (retail `move v1,s0` precedes `mov.s $f12,$f20`).
Builder (tools/g.sh = score.compare per member): `func_8038DA78: MATCH (4 section-relative relocations unverified ...)`,
all other members/context unchanged (D200 D328 E088 D498 F938 + six context partners MATCH).

### 1.2 func_8038F568 ElevateGun: colour-only, not yet matched
* Retail allocates s7 = weaponOffsets (0x803943A4) and s8 = a second constant 12 (body_type row stride), and leaves
  gGameCar / &gNumCars unallocated (rematerialised `lui/addiu` at the loop end; as1 then cannot fill the
  continue-branch delay slots, giving retail's `beq ..; nop` and the straight prologue). FP: retail f20 = -1.0,
  f22 = best, f24 = yaw, f26 = pitch, f28 = -0.5, f30 = 0.5, zero rematerialised in the loop.
* Our compile ranks (ctrace, standalone single-file -O3 group): gGameCar 11/7 > &gNumCars, 952, &pos, &vec,
  weaponOffsets, 12' (all 10/7, ties to lower web number), and FP best 21/7 > -1.0 20/7, zero 10/7 first of the
  three 10/7 constants.
* Force oracle `p1:w147=s,p1:w148=s,p1:w149=s,p1:w158=s,p1:w159=s,p1:w163=c21,p1:w164=c22,p1:w157=c30,p1:w0=c31,
  p1:w160=s,p1:w161=c34,p1:w162=c35` -> 5 rows left, all the yaw/pitch zero init (retail two `mtc1 zero`, ours one
  `mtc1` + `mov.s`): i.e. the body is otherwise exact; pitch/yaw initialisers are two different constants.
* A compiled-out second read of `weaponOffsets[t][bt][1]` in the else arm (lever 9) moves the integer side to
  retail's allocation (norm rows 85 -> 24): retail's weaponOffsets/12' webs carry two refs. Not acceptable as
  source; the natural form that gives them two refs is not found yet.
* Sweeps with no effect (4 outcome classes over 700 variants): zero spellings (0 / 0.0f / 0.f, mixed for
  pitch/yaw/compare), initialiser order, `-1.0f * x` vs `x * -1.0f`, `+-0.5f * v` operand order, loop forms
  (`&a[n]`, `a + n`, while/goto, nested ifs).

### 1.3 func_8038FCE0 ShotUpdate: 608 -> 73 words
* AnimateCase compare `casing->time <= 0` (int 0, as w35f had) but `_InitNewShot`'s `shot->incTime = 0;` must be the int
  spelling too: retail materialises that zero separately (`mtc1 zero,$f8`) while vel[] use the allocated f22 zero.
* `shot->vel[2] = 0.0f; shot->vel[1] = 0.0f; shot->vel[0] = 0.0f;` (reverse order; retail stores 28,24,20).
  These two changes fixed the register ring for the whole function (75 words -> 73).
* `extern volatile u8 D_80140BDC` (gNumModels) also helps (retail `lui/addiu` + `lbu 0(reg)`), FCE0 rows 66 -> 65.
* Residual: frame 200 vs retail 208 (every local home 8 bytes low; the a0 = &car->uvs spill at 116 is retail's
  frame-92) + `move a1,s0`/`move s3,zero` order + `lb v1,6(s1)` vs `lb v0`.
  Retail's two deleted-procedure stubs (func_8038F560 before ElevateGun, func_8038FCD8 before ShotUpdate) match the
  emission positions of `_InitNewShot` and `AnimateCase` when both are real inlined helpers (our compile emits the
  init stub before F568 and the anim stub before FCE0), so retail had BOTH as umerge-inlined functions. But making
  `_InitNewShot(shot, car)` a helper lowers the per-procedure callee-saved cost 28.0 -> 27.75 (one block fewer), and
  the &car->uvs web (tot 30) then beats bestcost 29.75 and takes s3 instead of a0+spill: 573 words. Adding any one
  block back (`if (car) {}` probe) restores the a0 spill and gives the hand-inlined code (66 rows, norm 3) but the
  frame becomes 224 (cfe area 96 + home 2). Measured cfe areas: hand-inlined 80, anim helper 84, init helper 88,
  both 96, both with AnimateCase's `idx` removed 88 (but then `lh a0,6(s0)` instead of retail's lw+sll+sra).
  Retail needs area + 4*(home+1) = 92. Not resolved.

### 1.4 func_8038E114 UpdateShots: 1116 -> 859 words (norm rows 21 -> 15)
* `extern volatile u8 D_80140BDC` (gNumModels): retail computes its address (`lui t9; addiu t9`, `lbu a3,0(t9)`).
  This alone re-aligned the whole integer temp naming (1102 -> 832).
* The retail keeps every `shot->obj` / `shot->obj2` load of the loaded==2 block in s0 = the same variable as `h`:
  the N64 source assigns one pointer variable before each use (`o = shot->obj; ZOID_SetColor(o->mbIdx, &clr);
  o = shot->obj2; ...`) and uses that variable for the loaded==1 `h = shot->obj2` and the wall-hit HitPoly too.
* Case 3's dead `vel = gFrameSeconds;` is a float-typed volatile read in retail (`lwc1 $f8,0(v0)`); a plain copy
  compiles to `lw zero,0(v0)`. `vel = (f32)(double)D_8002EB94;` (or `* 1.0`, `+ 0.0`) gives the lwc1. Provisional:
  not obviously original source.
* Temp register NAMES are assigned by a post-allocation renaming pass in ugen (the traced freelist allocation is
  t6,t7,t8 for the first three temps, the listing has t8,t9,t7), so any temp-count difference anywhere renames the
  whole procedure; the remaining FP ring shift follows from the residual structure below.
* Residual structure: case 3 loads (`lwc1 $f2,28(s5)` early in retail), case 4/6 `if (shot->streak)` (retail
  `beqz;nop` into a `b end` whose delay slot holds the lifeTime store; ours `beqzl` + duplicated store), case 7
  `if (!shot->obj)` (retail branches to the case's own `b end`, ours is threaded straight to the switch end with
  `bnezl`). Statement-order, else-arm, `break` placement and Olimit/opt-level probes do not change the last two.

### 1.5 Can the helpers land with stand-in callers? Only with the real (near-miss) callers.
A tiny stand-in caller (one call per helper) makes umerge INLINE D328/E088/D498/F938 (their symbols shrink to 2-word
stubs): only D200 and DA78 still MATCH. Retail keeps them out-of-line because their real callers are large
(UpdateShots 1299 words, ShotUpdate 764) and umerge's inline budget is exhausted there. So the helper conventions do
not depend on the caller's code, but their EXISTENCE as procedures does: they land only with UpdateShots/ShotUpdate
(or something as large) in the unit. The deliverable therefore keeps our near-miss E114/FCE0/F568 as unclaimed members.
Own data: the unit's .rodata also holds E114/FCE0/F568 literals; the splice must verify the helpers' literal homes.

### 1.6 UpdateShots: the final switch needs an explicit `default: break;` (859 -> 814 words, 1299 words exactly)
Retail's last switch (`switch (shot->type)` choosing `_CreateShotObject`) has every case path end with
`b end+4; lb v0,7(s5)` *including the last case*, and the out-of-range/type-5 path lands on a separate
`lb v0,7(s5)`; case 7's `if (!shot->obj)` branches to its own `b` (not threaded). That is what an explicit
`default: break;` after `case 1: case 8:` produces (the default arm becomes a real block between the last case and
the switch end, so neither the last `break` nor case 7's branch is a jump-to-next/jump-to-jump). Verified: case 7 and
the last case now match; `got` has exactly retail's 1299 words. (Assembler experiment that led here: inserting a
`nop` before case 7's `b $187` in ugen's listing stopped as1 from threading it.)
Remaining structural: case 3 (retail loads `shot->vel[2]` early into $f2, the same register as case 4's coloured
`shot->vel[2]` web; ours uses a ring temp and does not hoist), case 4/6 (retail `beqz;nop` + lifeTime store in the
`b`'s delay slot; ours `beqzl` + duplicated store; assembler experiments with the store after the if, with either
line number, still give beqzl), and the case-3 dead float read (see 1.4). Because temp names come from a global
renaming pass, these leave the FP names shifted throughout (814 words differ, almost all FP register names).

### 1.7 ShotUpdate down to 7 words (levers, not yet clean)
* Two extra unused `s32` locals after `idx` (frame residual: retail 208 = cfe area 88 + spill home at frame-92;
  ours 80 + home 0 = 200) give frame -208 and every home exact: 28 words, norm 2.
* `if (D_8039A520.head) {}` (compiled-out read of gShotList.head) anywhere around the CopyUV call fixes the case-1
  colouring (weaponOffsets base a2, 12 in a0, `move a1,s2` hoisted): 7 words.
* Force oracle (`p1:w171=c2`) proves the remaining shot->type webs (`lb v1,6(s1)` twice) are colour-only; the last
  2 words are as1 placing `move s3,zero` (StartStreak = 0, a uopt split move stamped with line 989) two slots early.
* 30 compiled-out-read placements (shot->slot/flags/next/obj/type, car->slot, list heads, before/after the
  PlayGameSound block) do not move shot->type to v1.
* Best: `func_8038FCE0/best_shots.c` (full unit file; drop-in for groups/w36h_shots/shots.c, NOT installed there
  because ShotUpdate is not claimed).

### 1.8 ShotUpdate MATCH: _InitNewShot is a real (umerge-inlined) helper, PlayGameSound returns int
Final ingredients (each measured):
1. `extern s32 func_800B61A8(...)` (PlayGameSound returns int, MAT3 prototype): the shot->type webs after the CopyUV
   call move to v1 (`lb v1,6(s1)`), as retail.
2. `_InitNewShot(shot, car)` as a non-static function `func_8038F560` called from ShotUpdate: umerge inlines it and
   leaves exactly retail's 2-word stub at that name; its two inlined parameter homes are the 8 frame bytes (frame 208,
   every home exact) - no unused locals needed. The inlined body's line numbers also fix the `move s3,zero`
   (StartStreak = 0 split move) placement. (Hand-inlined, the same result needed two unused locals and all of
   `shot->old[0..2] = ...; CopyUV(...)` joined onto one logical line.)
3. DISCLOSED LEVER: `if (D_8039A520.head) {}` right before the `_InitNewShot` call (or right after it). Without a
   compiled-out block there the procedure has one block fewer, the callee-saved cost drops 28.0 -> 27.75 and the
   &car->uvs web takes s3 instead of a0 + spill (574 words); other compiled-out reads (`if (shot) {}`, `if (car) {}`,
   `if (shot->type) {}`, `do {} while (0)`, ...) restore the cost but leave 21 words of case-1 colouring (the
   weaponOffsets base must be a2 and the row stride 12 in a0): only a reference to the gShotList address fixes both.
4. AnimateCase stays inline (as a helper it adds 4 bytes of locals -> frame 216; without `idx` the ZOID_RemoveObject
   argument narrows to `lh` instead of retail's lw+sll+sra).

### 1.9 Packet finalised (coordinator request after the usage-limit reset)
* group.json reshaped so it scores clean without --claims: members = claims (8), E114/F568 moved to context.
* gShieldSizes made a plain static (MAT3 has no const): retail keeps it in .data at 0x80394358; with `const` it went
  into our .rodata and broke UpdateDefense's one-base .rodata proof. All code still MATCH; F938 own data now verified.

### 1.10 ElevateGun MATCH: `goto next` for the first skip test
The whole residual was register priority (1.2). Any extra empty block inside the loop (`if (hit) {}` etc.) fixed the
FP order and the s7/s8 constants but left gGameCar's preheader piece in a3. The natural form: the first 5-way skip
test written `goto next;` with `next:;` as the loop body's last statement (the second test keeps `continue;`), plus
`f32 yaw = 0; f32 pitch = 0.0f;` (yaw declared first, different zero spellings -> two separate `mtc1 zero`) and
`pos[i] = pos[i] + car->uvs[1][i] * h`. Builder: `score.py fn func_8038F568/standalone.c func_8038F568 --targets
asm/us/ovl_b` -> `MATCH (8 section-relative relocations unverified ...)`; in the group: MATCH (claimed). Not put in
cloud/matches/ovl_b/ because it ships inside the group (one owner per function) and its own .rodata spans two
retail regions (single-file splice would hit the same one-base rule; the group carries it).

### 1.11 UpdateShots MATCH
On top of 1.4/1.6 (volatile gNumModels, `o`/`h` re-reads, `default: break;`):
1. Case 3 of the motion switch: `vel = shot->vel[2]*D_8002EB94;` (dead; it is case 4's line). This one change took
   E114 from 787 to 231 words: the dead multiply's operands keep retail's FP temp pattern (dead `lwc1 $f8` of
   gFrameSeconds, `shot->vel[2]` loaded early into $f2) and the global temp renaming then lines up everywhere.
2. EmitGameSound's id is `70.0f + Random(3.0f)` (retail `lui at,0x428c`; MAT3/GC has 21).
3. An explicit `break;` after SetupTarget in the collision switch's case 3. Without it ugen points that path at the
   block holding the final path's `shot->lifeTime = 1e-7` jump (`$168: b $170`), the label splits the store from
   its `b`, and as1 then turns the final path's `beqz;nop ... b;swc1` into `beqzl;swc1 ... swc1;b;lb`.
4. The w35f frame pads `padx`/`pady` are exact frame residual (retail sp+284 and sp+224 never accessed; without
   them frame -280 and 54 words shift). `h` and `o` are both real (MAT3 uses kept: h for loaded==1's obj2 and the
   wall-hit HitPoly, o for the loaded==2 re-reads).

### 1.12 Files
Variant sweeps were pruned at the end (paths like `eg/b6`, `fce0/b24`, `e114/b18` in the log no longer exist);
the decisive sources are kept in `keep/` (e114_match.c = the group's shots.c; fce0_match_lever_before_call.c;
fce0_handinlined_2pads_join.c = the hand-inlined 2-pad route; f568_match_standalone.c). `groups/ovlb` mirrors the
deliverable's shots.c; `groups/shots` is w35f's old copy.

## 2. Lessons that generalise
* A dead assignment still shapes codegen: UpdateShots' case 3 assigns `vel` a value nobody reads, and its exact
  expression (`shot->vel[2]*gFrameSeconds`, copied from case 4) decides the dead loads' types/registers and, through
  ugen's global temp renaming, every FP temp name in a 1299-word function (787 -> 231 words from one line). Try the
  neighbouring cases' expressions for dead stores.
* ugen renames temps after allocating them (traced free-list order t6,t7,t8 -> listing t8,t9,t7), so a temp-count
  difference anywhere renames the whole procedure; chase the first structural difference, not the names.
* Jump structure decides as1 delay slots: as1 threads a branch into a lone `b` and fills from the target; an
  explicit `default: break;` / `break;` that gives the jump its own block (or removes a shared label between a store
  and its `b`) restores retail's `beqz;nop` / `b;store`. Assemble an edited ugen listing (`tk.sh asm`) to find the
  needed as1 input before searching C.
* `goto next; ... next:;` closing a loop body adds an empty block: block counts move uopt's quantised live-range
  units (nocs) and the callee-saved cost, which flipped ElevateGun's priority inversions all at once.
* Callee return types are colouring inputs: PlayGameSound declared `void` vs `int` moved a web from v0 to v1.
* An umerge-inlined helper's parameters take frame homes (here exactly retail's missing 8 bytes) and its body's line
  numbers drive as1 tie-breaks; retail's caller-less 2-word stubs show which helpers existed and, by emission order,
  which is which (F560 = _InitNewShot, FCD8 = AnimateCase).
* Static tables: MAT3 `const` vs non-const decides .rodata vs .data; owndata's one-base rule rejects functions that
  read tables and literals from two retail regions; check contents per reference (`tools/refchk.py`).
