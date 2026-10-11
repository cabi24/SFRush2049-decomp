# w16f (wave 16): pause_quit group retry via w15j gate; audio_frame_update; init_state_continue

| Function | Bytes | State | Flags | Evidence |
|---|---:|---|---|---|
| drone_throttle_calc (= pause_quit) | 1024 | **provisional** (code identical with stand-in callers; w15j own-data gate PASSES) | -O3 group | `groups/frontier_pause_quit/` (claims []) |
| audio_frame_update | 600 | 21/150 words off in the unit (unchanged; stop rule) | -O3 unit | `audio_frame_update/best.c` (= w4b) |
| init_state_continue | 712 | **provisional, code identical** (score.py group MATCH with stand-in caller; was 50 rows) | -O3 group | `groups/frontier_init_state_continue/` (claims []) |

## Task 1: pause_quit (= drone_throttle_calc, 0x800D8C58, 256 words)

**Answer: the own-data gate passes; the group still cannot be installed, because the function is
provisional (stand-in callers), not because of its literals.**

### Where the source was
`pause_quit` is the alias of `drone_throttle_calc` (names_audit row 800D8C58). w6d's evidence is
`cloud/work/frontier/w6d/drone_throttle_calc/best_group/` (group.c with stand-in callers `zz_caller`/`zz_caller2`,
`char pad[12]` frame filler). `frontier show drone_throttle_calc`: `provisional: yes -- proven with stand-in callers
only`; real caller func_800D91A0 (3,860 B) unmatched (w9g/w15f: 603 rows off, 6 unmatched blockers).

### Re-score in the current whole-program unit (before changing anything)
w6d source with the stand-ins removed (`pause_quit/v/w6d_nocaller.c`):
```
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w16f --remote-dir rush2049/scratch/frontier/w16f --jobs 2 score drone_throttle_calc --with .../w6d_nocaller.c --neighbours
  FAIL drone_throttle_calc: 251 of 256 words differ; compiled body is 261 words, target 256; ...
       defined by c_w6d_nocaller.c (kept)
  locked bodies that differ in this unit: 1: func_800F0674
$ ... same --internal drone_throttle_calc --neighbours
  FAIL drone_throttle_calc: 2 of 256 words differ; compiled body is 2 words, target 256
       defined by c_w6d_nocaller.c (internal)
  locked bodies that differ in this unit: 2: func_800F0674, func_80109468
```
Kept -> standard convention (stack frame, s-register saves, a0 parameter); internal with no C caller -> umerge
deletes the body. The retail convention (no callee saves, s8 player read as `lb 115(sp)` from the caller's
outgoing slot) only appears with a C caller in the unit, and the only real caller is the unmatched
func_800D91A0. So the unit cannot confirm it today. (func_800F0674's `.bss overlap` and func_80109468's stub-tail
note are the known `--with` layout artefacts (w4a, w8c, w10g); baseline `blob_unit check`: 958/958 equal.)

### Group score (builder, current tree, w6d group unchanged)
```
$ IDO_DIR=... python3 tools/cloud/score.py group cand/pq_w6d
drone_throttle_calc:
  NOT VERIFIED (20 section-relative relocations unverified: .rodata+0x0 at +0x158, ... ; own .rodata: this function's
  references disagree on the section's image address (0x80120254 for +0x0/+0x10/+0x18, 0x801241E0 for
  +0x1c/+0x20/+0x24/+0x28/+0x2c/+0x30/+0x34): the literals are not laid out as in retail)
```
Code identical; only the 20 own-literal relocations unverified (as in wave 6).

### Clean-up of the source (w16f)
- `char pad[12]` (pad array, not acceptable) replaced by three unused `s32` locals: code identical. Frame
  arithmetic: retail frame 112; without `k` and the 12 bytes the frame is 96 and 9 words differ (frame and
  `lb 99(sp)` etc.); so `k,u0,u1,u2` (4 slots, 16 B) are the exact frame residual. Disclosed in the header.
- Prototypes aligned with the locked definitions (w6d had `s16 func_8008E26C`, `s32 func_800B24EC()`, unprototyped
  model_* ): now `s32 func_8008E26C(s32, void *, s16, s32)`, `u32 func_800B24EC(char *, s16 *, s8, s8, s32)` (u32 as in
  the locked groups for its NameEntry * return), `void model_data_load/model_transform_setup(s32, s32, u32)`,
  `string_copy_format(char *, s8, s8, s8)`. Still code identical.

### Gate dry run (w15j method, nothing installed, nothing committed)
Script `pause_quit/dryrun.py` (w15j's dryrun.py generalised: compiles the group dir with
`blob_group.builder_script` in builder scratch `~/rush2049/scratch/frontier/w16f/gdry/`, fetches the object to session
scratch, then runs `blob_group.group_bodies` = the gate `splice()` runs, plus `verify_own_data` with and without
data runs). Output `pause_quit/dry_final.txt`, verbatim:
```
source_sha 70ab8d6a616c81393111f10986b36a7597cfe9d9b5325288e90ded1f8a4a856e
build: DONE
members relocated: ['drone_throttle_calc']
image mismatches: none
word diffs: {'drone_throttle_calc': 0}
owndata only ok= False failures= ["own .rodata: this function's references disagree on the section's image address (0x80120254 for +0x0/+0x10/+0x18, 0x801241E0 for +0x1c/+0x20/+0x24/+0x28/+0x2c/+0x30/+0x34): the literals are not laid out as in retail"] unverified= []
  placements= {}
  notes= []
data_runs ok= True failures= [] unverified= []
  placements= {'.rodata': [('0x0', '0x10', '0x80120254', 'rodata'), ('0x10', '0x18', '0x80120264', 'rodata'), ('0x18', '0x1c', '0x8012026c', 'rodata'), ('0x1c', '0x20', '0x801241fc', 'rodata'), ('0x20', '0x24', '0x80124200', 'rodata'), ('0x24', '0x28', '0x80124204', 'rodata'), ('0x28', '0x2c', '0x80124208', 'rodata'), ('0x2c', '0x30', '0x8012420c', 'rodata'), ('0x30', '0x34', '0x80124210', 'rodata'), ('0x34', '0x38', '0x80124214', 'rodata')]}
  notes= ['own .rodata verified at 0x80120254..0x80120270', 'own .rodata verified at 0x801241FC..0x80124218']
```
(The w6d group as-is gives the identical gate result; `dry_w6d` in session scratch.)

Windows and retail contents (image words decoded):
| object .rodata | retail | contents |
|---|---|---|
| +0x00..0x10 | 0x80120254 | "BUTTON_TITLE\0\0\0\0" |
| +0x10..0x18 | 0x80120264 | "CURSOR\0\0" |
| +0x18..0x1c | 0x8012026C | "B1\0\0" |
| +0x1c | 0x801241FC | 0xbfc90fdb -1.5707964f |
| +0x20 | 0x80124200 | 0xc2963333 -75.1f |
| +0x24 | 0x80124204 | 0x4248cccd 50.2f |
| +0x28 | 0x80124208 | 0x425ccccd 55.2f |
| +0x2c | 0x8012420C | 0xc2ae3333 -87.1f |
| +0x30 | 0x80124210 | 0x4248cccd 50.2f |
| +0x34 | 0x80124214 | 0x425ccccd 55.2f |
No jump table, no relocated words, no overlap, every window inside an opaque run: none of w15j's conditions fails.
Body word diff: 0.

### Deliverable
`groups/frontier_pause_quit/` (group.c, group.json): **claims []**, members [drone_throttle_calc], keep = the two
stand-ins. It is the work base for func_800D91A0: when that caller is matched, add it as a member (or this file to
its group), drop zz_caller/zz_caller2, and the own-data part is already proven to pass. Integration today: none.
`provisional.json` entry for drone_throttle_calc can point to this dir instead of w6d's (evidence path only).

## Task 2a: audio_frame_update (0x800B0C48, 600 B / 150 w) -- 21 words off, stop rule reached

Best is still w2h/w4b's source (copied unchanged to `audio_frame_update/best.c`). Re-score in today's unit:
```
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w16f --remote-dir rush2049/scratch/frontier/w16f --jobs 2 score audio_frame_update --with cloud/work/frontier/w16f/audio_frame_update/v/w4b.c --neighbours
  FAIL audio_frame_update: 21 of 150 words differ
       defined by c_w4b.c (kept)
  locked bodies that differ in this unit: 0
```
Note: a locked group named `audio_frame_update` exists (member func_800B0A88; audio_frame_update and func_800B08FC
are context there, kept by stand-ins). A match would supersede it under the SAME name with func_800B0A88 kept as a
member.

Trace (`ctrace.sh`, ordinal 453): everything before the final block is identical. Two residuals in the last block:
1. **Colouring of the final loop.** Ours: car+i*24 w98 v0 (save 70), i w78 v1 (30.5), 24 w120 a0, car w64 a1,
   ctl `&D_80139320[slot]` w83 a2, anim cb w89 a3 (w83/w89 tie at 5.5, lower web number first). Retail: i a0, 24 a1,
   car a2, anim a3, ctl t0, v1 unused in the loop. So retail has a web coloured v1 before i that interferes with the
   whole loop but emits nothing in it, and anim ahead of ctl. Forcing the retail colours
   (`force.sh b0 audio_frame_update "p1:w78=c3,p1:w120=c4,p1:w64=c5,p1:w89=c6,p1:w83=c7"`) leaves 14 rows (12 with the
   `for` form), all as1 order in the preheader. (Confirms w4b.)
2. **as1 order in the preheader (new finding).** Retail: `lui t8; lui t6; addiu t8; addiu t6; lui a3; lui at; sll t9;
   sw t8,20(v0); sh s3,8(v0); lwc1 $f0; addu t0; addiu a3; move a0,zero; li a1,24`. That lui cluster appears exactly
   (registers aside) only when the float `D_801543CC` is read INSIDE the loop and hoisted by PRE (variants for2, g5):
   the hoisted invariants then come in the listing as ctl, anim, float address. With a named `ft = D_801543CC` the
   `l.s` sits where the statement is and as1 orders the cluster differently (for1/oA-oE/y1-y4/n1-n6: 21-25 words).
   But with stores through `car->` in the loop, uopt does not hoist the LOAD (only its address: +1 word, `lwc1 $f4`
   in the loop, g5 = 29 words). With direct `player_array[slot].snd[6 + i]` stores the load IS hoisted (`lwc1 $f0` in
   the preheader, cluster order right, g1), but then the m stores and the loop base no longer share `car`
   (`addu a2,t6,t7; addiu v0,a2,272` in retail): g1/g3/g4/g6/g7/g8 = 40-43 words, 157-158 words long. Defining
   D_801543CC in the unit (h1, h2) does not make the load hoistable through `car->` stores.

Tried (about 40 variants, three rounds, no strict improvement over 21): for/do/while loop forms, for-init placement
of ft and i, statement order of car/m/stores/ft, loop-body store order (retail's f4,cb,slot,idx,t,handle; worse),
named ctl/anim locals (copy-propagated, no change), compiled-out reads after the loop (`if (m) {}`/`if (car) {}` 21,
others 63-94; `if (m)` moves m to t0 but i stays v1), direct global indexing in the loop with and without a kept car.

Best next hypothesis: retail's loop stores go through a pointer uopt can prove does not alias D_801543CC while the
base `&player_array[slot]` stays one web shared with the m stores. Candidates: the loop stores through `m`-relative
addressing (`m[6 + i]` from the (u32)-laundered m, w4b tried `m[6+i]` only with a named ft), or the float read
through an inlined static getter (the four caller-less stubs func_800B08DC..F4 precede func_800B08FC in this file;
none has been tried as a helper of audio_frame_update). Run pretrace.sh on g5 to see why the D_801543CC load is not
a PRE candidate (which store kills it).

## Task 2b: init_state_continue (0x800FAF6C, 712 B / 178 w) -- PROVISIONAL, code identical

```
$ IDO_DIR=... python3 tools/cloud/score.py group cand/frontier_init_state_continue     # = groups/frontier_init_state_continue
Members:
init_state_continue:
  MATCH

Context (informational; excluded from exit status):
func_8008B2E4:
  MATCH
```
Provisional: the group keeps the stand-in `standin_caller` (w2e's). The function is frameless and writes s0-s8
unsaved (IPA-internal); its real callers display_list_flush (2,356 B) and countdown (2,672 B) are unmatched.
Whole-program unit without the stand-in (`v/t1_nocaller.c`):
```
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w16f ... score init_state_continue --with .../t1_nocaller.c --neighbours
  FAIL init_state_continue: 131 of 178 words differ; compiled body is 182 words, target 178
       defined by c_t1_nocaller.c (kept)
       umerge inlined into it: func_8008B2E4
  locked bodies that differ in this unit: 0
$ ... same --internal init_state_continue --neighbours
  FAIL init_state_continue: 6 of 178 words differ; compiled body is 6 words, target 178
       defined by c_t1_nocaller.c (internal)
       umerge inlined into it: func_8008B2E4
  locked bodies that differ in this unit: 0
```
(kept = standard frame; internal with no C caller = deleted, as for pause_quit.)

Trace and the levers (start: w2e's best, 89/178 strict but only **50 aligned rows**; the colour trace showed
every pool register already as retail, so the rows were structure):
1. **Retry-loop latch.** Retail: `beql a1,v1,exit; sb zero,7(a2); b top+4; move v1,zero`. That is a
   `while (1) { ...; for (prior = 0; ...) ...; if (slot == prior) break; }` (w3d tried while(1) and got 90 rows; the
   90 rows are lever 2, not the loop form). `slot == prior` gives retail's `a1,v1` order.
2. **`volatile s16 D_801543CA`.** With the while(1) form PRE finds the D_801543CA load available on entry (the
   `slot < D_801543CA` test) and moves the reload to the latch; the address web loses weight and drops from t0 to s1,
   which shifts t0-t5/s0-s1 (90 rows). Retail reloads it at the loop top (`lh t8,0(t0)`). Several locked files
   already declare it `extern volatile s16 D_801543CA` (retail reads it through `la; lh 0(reg)` everywhere).
   rf 90 rows -> rfv 47 rows.
3. **The locked Random() func_8008B2E4** instead of w2e's own `frand`/`func_8008B2B4` copy (WAVE_BRIEF "inlined
   locked helpers"): its redundant `& 0x07FFF` and named `rannum` put the rand value in the temp ring (retail
   `sra t9; andi t6; mtc1 t6`) instead of a v0 web, and fix every fcsr temp after it. 47 -> 4 rows.
4. **Mode 4/5/6 as an if-chain** (`bnel a0,v0`; w2e's switch gave `v0,a0`; the if-chain no longer costs an `ra`
   frame once 1-3 are in). 4 -> 2 rows.
5. **`config = D_80153E88` in the for-init after `slot = 0`** (`for (slot = 0, config = D_80153E88; ...)`): as1
   places `addiu a2,a2,16008` after the other address set-ups as in retail. 2 -> 0 rows, score.py MATCH.
Disclosed shaping: `volatile` on D_801543CA only. No unused locals (w2e's unused `selected` and the dead `pick`
helper removed), no compiled-out reads, no implicit int.

Packaging (`groups/frontier_init_state_continue/`): group.c + `func_8008B2E4.c` (byte-identical copy of
src/blob/func_8008B2E4.c, `cmp` clean), members [init_state_continue], context [func_8008B2E4], keep
[standin_caller, func_8008B2E4], **claims []**. Integration today: none. Add to provisional.json (evidence path this
dir) if the coordinator tracks it; it lands when display_list_flush or countdown is matched in the same group (the
stand-in then goes).

## Integration notes (all tasks)
- Nothing to install. Both groups are provisional (claims []); no unit_overrides, no groups superseded, no data
  symbol changes.
- provisional.json: drone_throttle_calc's evidence can move to `w16f/groups/frontier_pause_quit` (no pad array,
  correct prototypes); init_state_continue can be added with `w16f/groups/frontier_init_state_continue`.
- The w15j own-data gate is proven for drone_throttle_calc's literals (strings 0x80120254, floats 0x801241FC):
  nothing more is needed on the tooling side when its caller lands.

## What generalises
- **Count aligned rows, not strict words, for provisional bodies**: w2e's "89/178" was 50 rows, and those were
  4 structural levers. Branch-offset shifts inflate strict counts.
- **A PRE-moved reload drags register priorities**: when a loop's load is found available on entry, uopt reloads it
  at the latch and the address web's weight falls, re-colouring the whole pool. `volatile` on that global (if
  locked code already uses it) keeps retail's top-of-loop reload.
- **Use the locked helper, not a local re-implementation** (func_8008B2E4 Random, with its static rand): the
  redundant mask/named local shape the temp ring.
- An IPA-private callee whose only callers are asm cannot be confirmed in the unit: kept gives the standard
  convention and internal deletes it. Such bodies stay provisional until a caller is C.
