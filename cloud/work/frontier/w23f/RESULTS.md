# w23f: engine_sound_sync (+audio_effect_setup) logic check, then camera_play_script

Status: engine_sound_sync group MATCH (3 members); camera_play_script see below.

## engine_sound_sync: the w22r "DIFFERENT at frame 706" is a harness artifact (callee conventions), not a logic error

The w22r oracle compiled `w21h/engine_sound_sync/best.c` ALONE (replace.py mode ido). In retail, engine_sound_sync
calls two callees with whole-program IPA conventions that a standalone compile cannot know:
- `entity_transform_calc` (0x80091CA4) takes x,y,z,w in **$f22/$f24/$f26/$f28** (retail call site 0x80098F30..3C:
  `lwc1 $f22,360(sp); lwc1 $f24,356(sp); lwc1 $f26,352(sp); jal; lwc1 $f28,348(sp)`), and clobbers $f20.
  Standalone best.o passes them o32-style: `lw a1,336(sp); lw a2,332(sp); lw a3,328(sp); jal; swc1 $f18,16(sp)`.
  The retail body then compares/stores whatever the draft had in $f22..$f28: wrong message payloads and message-pool
  allocation (func_80091B00 pool D_80142DD8, 0x18-byte Msg) -- exactly the w22r word diff (links 0x80142e38,
  0x80142e50, ... in 0x18 steps vs all 0x80142dd8) and the downstream audio counters D_8011024C/D_80110288.
- `audio_effect_setup` (0x80095800) takes effect in **s0** and clobbers s1/s2 unsaved (`frontier show`: register_params s0).
- func_80091AF0/AF8 (lock/unlock) are retail `jr ra` stubs: standalone best.o calls them (`jal func_80091AF0`), so
  the replaced body ran without the slot-queue lock.
Defined functions stay private in replace.py (only defined DATA is rebound), so func_80098ADC was the draft's own.

Oracle source with consistent conventions: `engine_sound_sync/oracle.c` = the draft body verbatim + static copies of
audio_effect_setup (aes.c body), entity_transform_calc (locked codex_transform_b109 body), lock/unlock bodies and a
static func_80098ADC. Standalone IDO 5.3 compile checked: no call to the retail entity_transform_calc/audio_effect_setup
remains. **Manifest entry for the coordinator to run on rocky: `engine_sound_sync/replacements_entry.json`**
(id/set `draft_engine_sound_sync_ipa`).

Tooling note for w23r / replace.py: a replaced body that calls a function with IPA register parameters or unsaved
callee-saved clobbers (frontier `register_params`, `unsaved_callee_regs`, or a call site with float args in $f20+)
cannot be judged by a standalone compile. replace.py should refuse/warn on such callees (or compile the replacement
inside the whole-program unit).

## engine_sound_sync bytes: 8 norm rows / 117 words -> 0 norm rows / 9 words (one colour decision)

All in the whole-program unit with `aes.c` (w21h, unchanged) and `--internal audio_effect_setup --internal func_80098ADC`.
Variants under `engine_sound_sync/v/` (us.sh lines: words / ops / norm, frame):
| step | source change (each measured on top of the previous) | us.sh |
|---|---|---|
| w21h best | - | 117 / 8 / 8, 376/376 |
| a1 | cross product in the arcade `crossprod(a,b,r)` operand order (vecmath.h: `r[0]=a[1]*b[2]-a[2]*b[1]` ...), a = basis[1], b = basis[2], r = basis[0] | 163 / 2 / 2 |
| a2 | projection as `basis[k][j]*direction[j]` (basis operand first, body_vector's uvs-first order) | **11** / 2 / 2 |
| d4 | loop-1 tail: `weight=gains[i]; panValues[i]=panValues[i]-(panValues[i]*weight); i++; total+=weight;` (`weight` was an unused local) | 12 / **0 / 0** |
| g1 | `total+=weight;` after the compiled-out compare (w21h's device) | **9 / 0 / 0** |

Findings:
- The "cross-product spill order" (w22e) was the second product's operand order: retail is the arcade crossprod
  macro order. The FP-ring "two missing allocations" (w21h/w22e) were not allocations at all: the ring offset at
  `out[0]` changed with the cross product and projection operand orders (later code), and after a2 every ring
  register matches retail.
- `weight` (a named unused local in w21h, frame residual) is a real variable: as a plain variable it goes second in
  `panValues[i]*weight` (retail `mul.s $f4,$f0,$f2` = pan, gains), and `total+=weight` after `i++` gives as1's
  retail order (addiu, addiu, mul, addiu, add.s, sub.s). In d4 its web (w160, type 3, slot 324) tied with the pan
  load (w163) at save 300 and took $f0 first; after the compare its live range spans 2 blocks (save 150) and pan gets
  $f0 as in retail. weight is no longer an unused local; pad3 remains the only one (frame 376 = retail).
- Residual (9 words): nearVolume's load web (w148, save 300, nocs 1) is decided before the gains reload web (w155,
  save 250 = 500/2) and takes $f2; retail decides gains first ($f2) and near then gets $f12 (they share the then
  block). `force.sh g1 engine_sound_sync p1:w148=c26` -> `differing rows 0 (words)`. So retail's gains web has
  save > 300 (more weighted uses) or near's is lower. Measured inert/worse: the add on gains[i] instead of weight
  (h1/h2, add moves before the mul), `gains[i++]`, one-line `total+=gains[i]; i++;`, reversed radius test (j4, 27),
  gains through a pointer (f1-f4), weight inside the pan expression (e1), `-=`/`=a+b` spellings.

## engine_sound_sync group: MATCH (engine_sound_sync + audio_effect_setup + func_80098ADC)

Two more levers on top of g1 (traced: `force.sh g1 engine_sound_sync p1:w148=c26` -> 0 rows, so one decision left):
| step | change | us.sh |
|---|---|---|
| m6 | listener loop as `for (listener=D_80146188.head; listener; listener=listener->next, i++)`, tail `weight=gains[i]; total+=gains[i]; panValues[i]=panValues[i]-(panValues[i]*weight);` | 9 / 0 / 0 (gains web tot 500 -> 600: save-300 tie with nearVolume, lost on web number 155 > 148) |
| n6 | `if (gains[i] != 0.0f) {}` before the radius test | **EQUAL** |
The early code-free read numbers the `gains[i]` load expression (PRE bit = web number) before the nearVolume load, so
the reload web wins the 300/300 tie, takes $f2, and nearVolume gets $f12 (retail). Without `weight` (q1): 1 word
(mul operand order). Merging the two compiled-out compares into one, or dropping either (p1-p4): 18-173 words.
pad3 (unused) is the exact slot residual: without it the frame still rounds to 376 but the arrays move 4 bytes (6 words).

Deliverable: `groups/engine_sound_sync/` (claims engine_sound_sync, audio_effect_setup, func_80098ADC).
Whole-program unit (Pi, current tree):
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w23f score engine_sound_sync audio_effect_setup func_80098ADC \
  --with cloud/work/frontier/w23f/groups/engine_sound_sync/audio_effect_setup_b132.c \
  --with cloud/work/frontier/w23f/groups/engine_sound_sync/engine_sound_sync.c \
  --internal audio_effect_setup --internal func_80098ADC --neighbours
  EQUAL engine_sound_sync: 309 words (kept, c_engine_sound_sync.c)
  EQUAL audio_effect_setup: 46 words (internal, c_audio_effect_setup_b132.c)
  EQUAL func_80098ADC: 2 words (internal, c_engine_sound_sync.c)
  locked bodies that differ in this unit: 0
blob_unit score: 3/3 equal
```
(Same 3/3 and 0 locked differing with w21h's `aes.c` in place of the B132 file.)
Group (builder, `score.py group cand/groups/engine_sound_sync`, exit 0; `--claims` exit 0, same lines):
```
engine_sound_sync:
  MATCH
audio_effect_setup:
  MATCH
func_80098ADC:
  MATCH
```
Packaging finding (generalises): the group first scored 16/309 words: the inlined lock's `la a0,D_80142728` stayed
after the register saves. In the unit the inlined code carries the call-site line; in a group with the helper in its
own file it carries `func_80091AF0.c` line 14, and as1's tie-break on source lines changes the prologue schedule.
Defining func_80091AF0/AF8 (same bodies) at the end of engine_sound_sync.c gives retail's +8 placement (before the
function: hoisted above `sw ra`). Context functions are not spliced.

Disclosed shaping (header of engine_sound_sync.c): two compiled-out compares (`if (gains[i] != 0.0f) {}` before the
radius test; w21h's `if (i == 5 || D_80146188.count == 1) {}` at the loop end), one unused local (pad3), helper
identity hypothesis func_80098ADC = inlined gain clamp through a pointer. No implicit int, no pad arrays, no false
prototypes.

### Integration notes
- Install `groups/engine_sound_sync/` as a new group: `install_group.py ... --revert-single func_80098ADC`
  (func_80098ADC is a locked single, `src/blob/func_80098ADC.c` = empty void stub; this group gives it the body
  `void func_80098ADC(f32 *d, f32 v)`).
- unit_overrides: `force_internal` audio_effect_setup and func_80098ADC (both internal in retail; the unit ran them
  with `--internal`). engine_sound_sync is on keep.
- Context files: `codex_transform_b109.c` = byte-identical src/blob/groups/codex_transform_b109/group.c;
  `audio_effect_setup_b132.c` = byte-identical cloud/work/lean_audio_effect_setup_20261006/group.c (its
  func_800988D8/func_80098FB8 are NONMATCH context reconstructions, as in that group; they only keep
  audio_effect_setup out of line in the group compile; in the unit its other callers are retail asm).
  func_80091AF0/AF8 are defined (context) at the end of engine_sound_sync.c. If the unit build objects to a second
  definition of the lock stubs, the in-file copies can be renamed static only after re-checking score.py group.
- audio_effect_setup's CLAIMS/provisional status: now EQUAL beside its real caller in the unit, claimed here.
- No data-symbol changes. Claims: +1236 + 184 B (+8 B stub already counted if locked).
- CLAIMS.md line: engine_sound_sync group MATCH (w23f); the oracle note below is about the old draft.

### Oracle (behavioural check)
The matched body compiles to retail's bytes, so it needs no oracle verdict. The manifest entry
`engine_sound_sync/replacements_entry.json` (oracle.c = the OLD w21h draft + correct-convention callee copies) is
still worth one run to confirm the harness diagnosis above (expected: EQUIVALENT or a difference that is not at
frame 706). Coordinator: please run set `draft_engine_sound_sync_ipa` on rocky if convenient.

## camera_play_script (0x800C5644, 3520 B): unchanged, 27 rows (time cap)

Re-scored in the current unit: `v/base.c` (= w17a/w18d/w20h best.c) `FAIL camera_play_script: 793 of 880 words differ;
compiled body is 879 words, target 880; ... | words 27 ops 1 norm 1 | frame 608/608` (unchanged).
Already tried by w15h/w16c/w17a/w18d/w20h (their RESULTS): effect_cleanup inlining, merge order, goto-end loop
weights, frame/named locals, u8 parameter, u denominators, k ternary, `n &= 0xF`, zero spellings, conv placement,
old in branch, `i = idx[0]`, c1 removal, ez before ex, product through u; corner IV: `&pts[k][0]`,
`(f32 *)&pts[k]`, `pts[0] + k*3`, `pts[e]`, `pts[ka]`, k=0 before call, do-while, ka/kb/e/i/type holders, `k != 4`,
`k <= 3`, extra pts[ka] reads, laundered argument (oracle only), reused pointer v, `*(pts + k)`, defined index tables;
PRIO/FBD oracles (items 1+2 = two decisions; item 3 = temp-home web order).
New this wave (all worse or inert; `camera_play_script/v/`):
- Expression-numbering holders (the lever that closed engine_sound_sync): `if (hz - pz != 0.0f) {}` before the
  k-loop (c2, 185 words) or inside it (c4, 194), `if (db - da != 0.0f) {}` at the e-loop top (c3, 186) or before
  `dy = db - da` (c5, 37), `if (ex != 0.0f) {}` with ex before ez (c6, 213). Here the FP holders change the code.
- Code-free uses of the pts[ka] address web to lift its priority (item 2): `if (pts[ka] == 0) {}` (e1, 37 words:
  s0/s1 swap, t0/t1 unchanged), 2 and 4 such uses (e2 229, e3 434).
- ex as an expression instead of a variable for item 4 (`(pv[k][0] - px) * (hz - pz)`): everywhere (f1, 367,
  frame 600), only in the two products (f2, 46).
Best next hypothesis: unchanged from w18d (corner pointer passed as something other than the bare IV with the
D_801174C4 pointer ranked at or above it; pts[ka] needs about twice pts[kb]'s weighted uses).

## What generalises
1. **Check the oracle harness against callee conventions before reading a DIFFERENT verdict as a logic bug.** A
   standalone IDO compile calls IPA-register-parameter callees (frontier `register_params`, float args in $f20+,
   `unsaved_callee_regs`) with o32 conventions; retail callees then read garbage. Lock/unlock stubs (`jr ra`) are
   also called instead of inlined. The w22r 677-file "frame 705" divergence may be the same class (a matched body
   compiled alone calling an IPA-convention callee).
2. **A FP ring "offset" in an otherwise structural match can come from commutative operand order elsewhere**:
   fixing the cross-product (arcade macro order) and projection operand orders moved the ring at earlier code too;
   do not hunt for phantom allocations.
3. **An early code-free read renumbers an expression's PRE bit/web** (`if (gains[i] != 0.0f) {}` before the
   computation): it wins save ties decided by web number. FP versions compile to nothing in some contexts
   (engine_sound_sync) and change code in others (camera_play_script).
4. **as1 breaks scheduling ties by source line, and inlined code carries the line of its definition in a group
   compile but the call site in the whole-program unit.** Where an inlined helper's instruction moves into the
   prologue, define the helper after the function in the same file for the group (context).
5. Loop form decides as1 order too: `for (...; p = p->next, i++)` gives the increments the loop's line, so a
   body statement after them in retail (`add.s` after the pointer increments) comes from a for loop.

## Permission denials
None.
