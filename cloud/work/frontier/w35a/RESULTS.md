# w35a: image-A carscr buttons (MAT3 initButtons / updateButtons), both MATCH

Lane dir `cloud/work/frontier/w35a/`, builder scratch `~/rush2049/scratch/frontier/w35a`. Population: runtime image A
(overlay at 0x8038A400). Nothing outside this directory was edited.

## Result

| function | GC identity | bytes | state | flags |
|---|---|---|---|---|
| func_8039FF04 | initButtons@carscr.c (MAT3 0x80093b10, 0x564 B) confirmed | 1428 | MATCH (code identical, own data verified locally, see below) | `-g0 -O3 -mips2 -G 0 -non_shared`, group only |
| func_8039D6A4 | updateButtons@carscr.c (MAT3 0x80094108, 0x8b8 B) confirmed | 2908 | MATCH (code identical, .data/.rodata verified locally, 2 own .bss statics unverified) | same, group only |

Deliverable: the superseding group `groups/dot_cleanup_colortrace_20261011/` (`group.json` + `group.c`, same name as the
accepted group, all six old members unchanged and still MATCH, `claims` = the two new functions, `allow_unverified: true`
because `asm/us/ovl_a` has no retail data tree). Neither function can be scored alone: both are private-ABI
functions of the IPA group (FF04 receives its s8 parameter in the caller's stack home, D6A4 in `s8`; both clobber
s0-s8/f20-f30 unsaved). Standalone `score.py fn` of the same bodies gives frame -96 / -240 instead of -80 / -152.

Exact scorer output (`score_group_final.txt` has the full transcript; `IDO_DIR` = toolkit 796ae99a…, on the builder):

```
$ python3 tools/cloud/score.py group cand/final --targets asm/us/ovl_a
Members:
func_8039D300:  MATCH
func_8039D05C:  MATCH
func_8039D494:  MATCH
func_8039ED20:  MATCH
func_8039E200:  MATCH
func_8039E3BC:  MATCH
func_8039FF04:
  MATCH (30 section-relative relocations unverified: .data+0x0 at +0x4, .data+0x0 at +0xc, .data+0x4 at +0x3c, ... .rodata+0x9c/0xa0 ...)
func_8039D6A4:
  MATCH (20 section-relative relocations unverified: .data+0x8 at +0x20, ... .rodata+0x0 .. +0x14 ..., .bss+0x0 at +0x1ec, .bss+0x10 at +0x53c ...)
exit=1   (plain mode: unverified own data counts as not accepted)
$ python3 tools/cloud/score.py group cand/final --targets asm/us/ovl_a --claims     -> both claims MATCH, exit=0
```

The context functions of the group (func_803A0498, 0FD8, 1EAC, 8039EF08) are the earlier nonclaiming stand-ins and stay
non-matching (154/720, 873/949, 1719/1776, 960/1023 words differ); they only supply the call conventions.

### Own data (local verification; image A has no `asm/us/ovl_a_data`)

`tools/owncheck.py OBJ NAME...` runs `score.compare` with the PRIVATE decompressed image A (`build/ovl/a/image.bin`, not in
git) supplied as retail data, so the overlay's own sections are actually proven:

```
func_8039FF04 : MATCH
    own .data verified at 0x803B305C (4 bytes)       <- buttonColor initialiser {255,255,255,255}
    own .data verified at 0x803B3060 (4 bytes)       <- function-local static float zoom = 1.0f
    own .rodata verified at 0x803B96F8..0x803B9720   <- 10 float literals
func_8039D6A4 : MATCH
    own .data verified at 0x803B3064 (4 bytes)       <- buttonColor initialiser
    own .rodata verified at 0x803B9610..0x803B9628
    not verified: own .bss+0x0  (statics cursorSpeed[4], retail 0x803BA200, 16 B): not in a known game .bss range
    not verified: own .bss+0x10 (static scrollSpeed[4], retail 0x803BA210): same
```

The two statics are the only unverified items; they are the GC `cursorSpeed$N / scrollSpeed$N` function-local statics
(`static float cursorSpeed[4]; static float scrollSpeed[4];`), adjacent in .bss in that order, as the retail addresses show
(0x803BA200 / 0x803BA210). Overlay .bss placement is the integrator's step (score.py's GAME_BSS covers only the game image).

## Identity confirmation (step 2a)

Both pairings from `docs/external/gc/correlation.md` are right.
- Strings: the image-A table at 0x803AF984 is exactly MAT3 `buttonNames` (BUTTON_S2G1 x3, CURSORG1, AB_BUTTONG1, ARROW_BGG1,
  ARROW_FGG1, BAR_BGG1, BAR_FGG1); texture names BUTTON_TITLE (GC: BUTTON), CURSOR, A1, B1, BUTTON_SELECT, BUTTON.
- Callees map one to one: MBOX_FindObject_WORK = string_copy_format, MBOX_NewObject = sign_extend_call (returns a short:
  `sll/sra 16` before ZOID_SetColor = func_8008E06C), MBOX_HideObject = model_data_load(h,1,15), MBOX_ShowObject =
  model_transform_setup(h,0,1<<p), ZOID_SetTexture = func_8008D870, MBOX_FindTexture_WORK = func_800B24EC, WPitchUV /
  WRollUV = func_800B5898 / func_800B5940, ScaleUV = func_8008B32C, ZeroCameras = particle_velocity_set, validOption =
  func_8039D494 (retail argument registers are permuted by IPA; source order (player, item) is right).
- Structure: same statement order, same `gNumWindows > 1 ? 105 : 115` style selects, same cursor / scroll speed clamps,
  same A/B arrow handling after the loop.

## N64 differences from the MAT3 bodies (all from the retail displacements/constants)

- Record table D_803AF9A8 is `[4][44]` x 64 B (kind +0, handle +4, angle +8, matrix +0xC, position +0x30, alpha +0x3C).
  MAT3 has 40 records (17+17+6); N64 has 44 (19+19+6). GC records 34..39 are 38..43 here; the option loop runs 19 times and
  the right-hand copies are at +19 (GC +17).
- initButtons: no `4.0f +` on the y of records 40..43; first texture BUTTON_TITLE; zoom applies to records 38, 42, 43 only (as GC).
- updateButtons: unselected buttons settle to angle 0.0f (GC 0.08726646f); alpha ramps linearly over one pitch,
  `255.0f - (pos - maxY) * 255 / (gButtonSpace * gButtonScale)` (GC: constant 80); expression association is
  `(n-1) * gButtonSpace * gButtonScale` (GC `scale * (space * x)`), `fabsf(d) * 60 * scale / 10` for the cursor and
  `fabsf(d) * 60 / 10` for the scroll speed.
- Floats are single precision throughout (GC uses double 60.0 / 10.0 literals); no byte swaps.
- Names kept from MAT3: p, buttonIdx, prevPos, buttonY, selectedPos, clearScrollSpeed, tex, texIdx, buttonColor, zoom,
  cursorSpeed, scrollSpeed. Globals stay `D_xxxxxxxx` per the group's house style (gButtonScale = D_803B25AC, gButtonSpace =
  D_803B25B0, buttonX = D_803B25B4, maxY/minY = D_803B25B8/BC, buttonZ = D_803B25C0, cursorZ = D_803B25C4,
  gActiveOption = D_803B28B8, gNumOptions = D_803BA050, gOpts = D_803BA060[4][19], gCarLocked = D_803BA028,
  allLocked = D_803BA02C, buttonsInited = D_803B9B90, buttonsJustInited = D_803B9BA0, gFrameSeconds = D_8002EB94,
  gNumModels = D_80140BDC, gNumWindows = D_80151AD0, gRace = D_8014A110, unitmatrix = D_8011418C).

## The levers that closed them (each found by tracing, none is a compiled-out device)

FF04 (159 -> 0 differing words):
1. `D_80140BDC` (gNumModels) is `volatile u8`: every retail use is `lui t; addiu t; lbu 0(t)`; a plain global gives
   `lui; lbu lo(reg)` and a smaller frame. (Found with a toy compile: only `volatile` produces that address form.)
   The only other user in the group is the nonclaiming caller func_803A0FD8, so the old members are unaffected.
2. `if (gNumWindows == 1) zoom = 1.0;` (double literal) with `(1.0f / zoom)` kept float: different uopt constants, so the
   store keeps its own `$f4` instead of sharing the hoisted 1.0f in `$f30`. `zoom = 1`, `1.f`, `1/zoom`, `1.f/zoom`
   also close it; the double spelling was chosen.

D6A4 (512 -> 0 differing words), in the order found:
1. `volatile f32 D_8002EB94` and `volatile u8 D_80140BDC` (same address-form telltale; MAT3 also declares gFrameSeconds volatile).
2. A comparison whose LEFT operand contains a `fabsf()` (#pragma intrinsic) call makes cfe reserve a 4-byte temporary in the
   local area (cfe area 45 instead of 41, traced with `tools/trace/spill.sh`). Writing `cursorSpeed[p] < fabsf(..)*60..` and
   `scrollSpeed[p] < fabsf(..)*60/10` removes it. This is also why the optionscr sibling is written that way.
3. The retail local area holds one more 4-byte local than the MAT3 list: texIdx at sp+114 and buttonColor at sp+108 instead
   of 118/112 (homes unchanged). Adding the used local `s32 mask` (= `1 << p`, three uses) between `tex` and `texIdx` gives the
   exact layout; an unused `s32 i` in the same slot gives the identical object (`var/m/`), so the used form is delivered.
4. `255` spelling: the two alpha ramps are `255.0f - (...) * 255 / (...)` (second 255 is the int). Written with 255.0f twice,
   uopt hoists one 255.0f web into `$f28` and the pre-loop `WPitchUV(1.5707964f)` constant is left as a narrow in-loop
   web in `$f20` (instead of retail's `$f30` shared with the call, 2 words more). With the int spelling the 255.0f uses are
   two low-priority webs that lose the callee-saved race and are rematerialised (`lui at,0x437f; mtc1`) exactly as retail,
   and the pi/2 web covers the pre-loop call and the loop (`lwc1 $f30` before the jal, `mov.s $f12,$f30`).
   (Learned from the already matched optionscr sibling `src/blob/groups/frontier_optionscr_buttons/buttons.c`, whose
   header lists "255 spellings".) Diagnostic chain: `pretrace.sh` showed the pi/2 constant as two live ranges (ctx 0 block 23,
   ctx 1 blocks 54-56) and `ctrace.sh` showed the wide web 291 losing to 292/304/305 and being split into the narrow web 362.
5. `D_803AF9A8[p][i+19].alpha = buttonColor.c[3] = D_803AF9A8[p][i].alpha` (colour byte stored first, the other store in the
   jal delay slot). The GC order (`color.c[3] = other.alpha = alpha`) puts the stores the other way round.
6. mul operand order: `fabsf(d) * 60.0f * scale` and `scale * 0.4f` (retail has the fabs result first and the scale first);
   MAT3's `60.0 * fabs()` order gives the swapped `mul.s`.

Tried without effect (kept in `var/`): holding pi/2 in a named f32 local (uopt copy-propagates it back), `do {} while (0)` /
`if (x) {}` holders around the pre-loop call (the optionscr lever for a tie, not this problem), `register`, a nested-block
local, double / `-(-x)` / `3.1415927f/2` spellings of the pi/2 argument, `?:` to `if` forms for the clamps (area unchanged).

## Tooling added (all in `tools/`)

- `odis.py` overlay disassembly with symbol names; `ovtool.py` compile + scorer + aligned diff of ours vs target (`fn`/`group`);
  `vb.py`/`vb.sh`/`mkvar.py` variant batches ranked by an edit-distance metric that ignores relocations (word-by-word counts are
  useless after one inserted instruction); `mk.py` builds the superseding group from `seg/seg.c` (head patch + segment swap);
  `one.py`, `sync.sh`, `b.sh`, `pull.sh`, `sp.sh` (stack-offset summary), `ar.sh`/`mktoy.py` (cfe local-area size of a toy in 2 s),
  `owncheck.py` (own-data verification of an overlay object against the private image; get the object with `tools/one.py SEG NAMES --tag X`, it lands in `out/X.out/out.o`).
- `parts/` = head/tail of the assembled group, `seg/` = the replaced segment (`seg.c` final), `var/` = variants, `t/` toys.

## Integration notes

- Install as the superseding revision of `dot_cleanup_colortrace_20261011` (`group.json` keeps `keep: [func_803A1EAC]`, the six old
  members and the four context entries; adds FF04 and D6A4 as members and as claims). The only text outside the replaced
  Color4..CarHandles segment that changed is `D_80140BDC` (`extern u8 D_80146180[4]; extern volatile u8 D_80140BDC;`) and the
  two-line banner at the top; `Color4` became `union {u8 c[4]; u32 word;}` (the old ED20 body still MATCHes).
- No `unit_overrides` needed beyond what the group already has; FF04/D6A4 are internal, the group's `keep` is unchanged.
- Overlay .bss: D6A4's two function-local statics occupy retail 0x803BA200..0x803BA210+16 (cursorSpeed then scrollSpeed).
  .data order in the object is FF04 colour (+0, retail 0x803B305C), zoom (+4, 0x803B3060), D6A4 colour (+8, 0x803B3064).
- Suggested CLAIMS line: `w35a: A:func_8039FF04 + A:func_8039D6A4 MATCH in superseding dot_cleanup_colortrace_20261011 (MAT3 carscr initButtons/updateButtons)`.

## What generalises about the MAT3 C for N64 matching

- The MAT3 body is a statement-for-statement skeleton for the N64 function, including odd shapes (`static` per-player arrays,
  chained `a = b.c[3] = x` assignments, an initialised 4-byte colour union). What changes is arithmetic association, constants,
  table sizes and record counts: take every offset and every literal from the N64 disassembly, keep structure from MAT3.
- `volatile` in MAT3 prototypes (gFrameSeconds, GVSyncCount, Worlddlidx, gLastFrameCount, gSeconds) is a hint for N64 volatile
  globals; the address-form telltale is `lui r; addiu r; lXX 0(r)` repeated at every use. gNumModels is NOT volatile in MAT3 but IS
  on the N64 (hit here), so check the form for every small global that is read in calls' argument lists.
- A frame/offset residual of exactly one word is usually not a missing MAT3 local: trace cfe's local-area size first (`ar.sh`,
  `spill.sh`): a call-valued left comparison operand, a chained u8 assignment, etc. reserve temporaries; only then add a local.
- Look for an already matched N64 sibling of the GC function family (here the optionscr updateButtons) before tuning: its
  header names the levers (255 spellings, fabsf side, clamps) and saved most of the search.
- `owncheck.py` (private image) turns "own data unverified" into proven for image-A functions; worth running before integration.
