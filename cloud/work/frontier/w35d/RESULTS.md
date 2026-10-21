# w35d results (runtime image A: MAT3 playerscr.c and startscr.c)

Builder scratch: `watchman2:~/rush2049/scratch/frontier/w35d`. Nothing outside this lane directory was written.
Deliverables (both are group packets; neither function can match standalone, they need IPA context):

- `groups/dot_runtime_a_availability_refresh_20261007/`  superseding playerscr group, same name as the installed
  w27p group, all four old members kept
- `groups/w35d_start_screen/`  new startscr group

Tools in this directory: `claims.sh DIR` (= `score.py group DIR --targets asm/us/ovl_a --claims` on the builder),
`cnt.sh`, `m.sh`, `vt.py` (substitution variants + counts), `gb.sh`/`gbatch.py` (vgen batches), `sbs.sh` + `sbsdiff.py`
+ `gotwords.py` + `objdiff.py` (aligned diff vs retail, `SBS_ARGS=--norm` hides register names), `spoffs.py` (sp-offset
census), `odis.py` (symbolised overlay disassembly), `rocheck.py` (own-rodata check against build/ovl/a/image.bin).

## State table

| function | GC identity | bytes | state | flags |
|---|---|---|---|---|
| func_80397FFC | `initScreen@playerscr.c` (GC 0x8007a49c), confirmed | 1072 | MATCH in group (own rodata 1.43f/1.1f hand-verified) | -g0 -O3 -mips2 -G 0 -non_shared |
| func_80390418 | `updateStartScreen` (GC 0x8008624c), confirmed | 1552 | MATCH in group (kept root); jump table + 0.05f hand-verified | same |
| func_8038F79C | `updateButtons@startscr.c` (GC 0x800871e4), confirmed | 2036 | MATCH in group (3 disclosed unused f32 locals) | same |
| func_8039842C | `updatePlayerScreen` (GC 0x80079f80), confirmed | 1804 | 122/451 words off; size, frame (192), prologue, jump table, control flow all equal; residual traced (below) | same |

Bonus (not in the assignment, needed as real IPA callees of func_8039842C; all MATCH in the group):

| function | GC identity | bytes | state |
|---|---|---|---|
| func_803968C0 | controlCreatePlayer (GC 0x8007b464) | 1560 | MATCH |
| func_80395E8C | controlDeletePlayer (GC 0x8007b9f8) | 2604 | MATCH |
| func_80394DD8 | updateButtons(signed char) @playerscr.c (GC 0x8007fc14) | 4156 | MATCH (2 disclosed unused locals) |
| func_80396FA0 | controlSelect (GC 0x8007a854) | 2872 | 262/718 words off (context, not claimed) |

Exact scorer output (`python3 tools/cloud/score.py group lane/<dir> --targets asm/us/ovl_a --claims`, exit 0,
`"allow_unverified": true` because own .rodata/.data cannot be placed by the unpatched scorer):

```
groups/dot_runtime_a_availability_refresh_20261007:
func_80394564: MATCH   func_80394834: MATCH   func_80394B00: MATCH
func_80397AD8: MATCH (22 section-relative relocations unverified ...)
func_80397FFC: MATCH (4 section-relative relocations unverified: .rodata+0xa4 at +0x29c, .rodata+0xa4 at +0x2a0, .rodata+0xa8 at +0x31c, .rodata+0xa8 at +0x320)
func_803968C0: MATCH   func_80395E8C: MATCH
func_80394DD8: MATCH (20 section-relative relocations unverified: .data+0x0 at +0xc, .data+0x0 at +0x1c, .rodata+0x30 at +0x43c ...)
Context: func_8039842C 122/451 words differ; func_803949CC MATCH; func_80396FA0 262/718 words differ;
         func_80395E24 MATCH; func_80396EE8 MATCH; func_80396F44 MATCH

groups/w35d_start_screen:
func_8038F79C: MATCH (24 section-relative relocations unverified: .rodata+0x44 at +0x1c ...)
func_80390418: MATCH (4 section-relative relocations unverified: .rodata+0x5c at +0xb8, .rodata+0x5c at +0xbc, .rodata+0x60 at +0x4f4, .rodata+0x60 at +0x4fc)
Context: func_8038F744 MATCH
```

Own-data check done by hand (`rocheck.py`): every HI16/LO16 pair into the group object's .rodata decodes, from the
retail words, to an image address whose bytes equal the object's: 21 sites (94DD8, 97FFC, 97AD8) and 14 sites
(8038F79C, 80390418, incl. the 10-entry jump table relative to the root start). The 4-byte white colour templates in
.data are the retail 0xFFFFFFFF at 0x803B5EE8 (97AD8's) and 0x803B5EEC (94DD8's); .data follows SOURCE order while
.rodata follows emission order, so group.c defines func_80397AD8 BEFORE func_80394DD8 (object .data+0 = 97AD8's,
+4 = 94DD8's, as retail). Do not reorder these two definitions.

## Integration notes

- Playerscr group: supersedes `src/ovl_a/groups/dot_runtime_a_availability_refresh_20261007` (same members + 97FFC,
  968C0, 95E8C, 94DD8; `keep` still `func_8039842C`; context 9842C, 949CC, 96FA0, 95E24, 96EE8, 96F44). The four old
  member bodies are unchanged (shared type views were extended: `Player.autoFlags`, `OwnerBody.name`,
  `Slot64.alpha`; `func_800A3424` now takes `s32` as in src/blob). The context copies of func_80397FFC / func_8039842C
  in `dot_runtime_a_menu_cleanup_20261007` and `runtime_a_profile_menu_20261006` are the old NONMATCH bodies and can be
  refreshed from this group (they are context only; 949CC/95E24/96EE8/96F44 stay claimed where they are).
- `func_80397FF4` (inlined calcSelectMenuMaxSize) and `func_80396EE0` (inlined claimPlayer) are emitted as their own
  non-static symbols, not members, not on keep, so no member sees "extra words" (the `static` spelling gives
  func_80397AD8 one extra nonzero word).
- Startscr group: new, no old members. `func_8038F744` (optionValid) is the installed body from
  `dot_runtime_a_option_eligibility_20261006`, present as context only. Without it the root keeps
  `&gStartMenuSelect` in s0 instead of a2.
- No overrides, no lock/source edits.

## func_80397FFC (initScreen): 15 words (PR #365) -> MATCH

GC identity: MAT3 `initScreen` loop order, constants and callees all match (PS_LockFile loop, updatePakInfo =
func_80394834, per-player gCarTeam/controller/select/setPlayerState, calcSelectMenuMaxSize inlined, initButtons =
func_80397AD8, updateSelectInfo(-1) = func_80394564, gNumWindows 3 -> 4 and the three layout blocks, ZOID_SplitMenuScreen
= func_800A6404 (takes a0 = num), createBG = particle_lifetime_set, InitPane = ambient_sound_set, NewMultiBlit =
sound_control, ClearRushControls = entity_audio_update). N64 has no PS_DiscardFileChanges call.

Levers (each fixed words that PR #365 could not):
1. The PR's consumed `screens` local was standing in for the inlined static `calcSelectMenuMaxSize`
   (GC 0x8007f4f0). Defining the helper restores frame 72 and the spill homes with no extra local (15 -> 12 words).
2. `func_800A6404` takes its real parameter (`func_800A6404(D_80151AD0)`): the gNumWindows value is then coloured a0 as in
   retail and the address lands in v0 (-3 words).
3. Define the helper AFTER initScreen (forward declaration), as MAT3 does. as1's list scheduler breaks ties on the lower
   source line: with the helper above, its `sb D_803BACF4` carried a lower line than the IPA spill stores of the
   following jal and was scheduled first (retail: it fills the jal delay slot); the same tie explained the
   `lw s0 / lw s3 / lw s4` swap. IDO still emits the deleted stub BEFORE the caller, so it sits at 0x80397FF4.
   (15 -> 0.)

## func_80390418 + func_8038F79C (startscr): MATCH, MATCH

IPA unit: 80390418 is the kept root and saves s0-s8 and $f20-$f30 for the unsaved registers updateButtons uses;
updateButtons (frame 96, saves only ra) is internal; uninitScreen and uninitButtons are inlined twice into the root;
initScreen (func_8038FFD8) and CheckForEgg (func_8038F648) stay external.

N64 differences from MAT3 startscr.c: `gOptions[14] <= 12` (GC 18), `AudPlaySong(6,1)` (GC 19), PlayLRSound runs before
the `gStartMenuSelect == 0` test, exit case 7 -> 0x10000000, clamp written
`if (n > max) { if (max < 1) n = 1; else n = max; }`; updateButtons lower angle limit `0.0f` (GC 0.08726646f).

Levers: `gFrameSeconds` is `extern volatile f32` (MAT3's rush_protos.h says so; `lui/addiu/lwc1 0(reg)` pairs);
`gNumModels` volatile u8; the `uvs[3][2] = cursorZ` store goes after the `[1]` statement (as1 tie-break on source
line); `(72.0f + gNumPlayers * 22.5f)` (ugen evaluates the int->float first, FP ring phase); optionValid in context.
Frame: retail 96 needs three more 4-byte slots than MAT3's locals (frame = align8(52 + Udef)); one s32 gives 88, two
give 96 with the locals 4 bytes too high, three f32 scalars (or f32[3]) give retail exactly. They are declared first
and disclosed in the header. A cursor-speed inline helper (the two blocks look alike) changes the code (297 words).

## playerscr cluster: func_8039842C and its callees

Why the root could not be reached before: it is the kept IPA root. It saves s0-s8 and $f20-$f30, passes arguments in
IPA registers (96FA0<-s0, 968C0<-s3, 95E8C<-s0, 94DD8<-stack 0(sp)) and spills s8 around the calls that clobber it. With
extern callees it can never reach the retail frame (the research body stuck at 440/451). I wrote the four IPA-internal
callees from MAT3 as real bodies (three match).

Callee levers:
- 968C0: `claimPlayer` (GC 0x8007f584) inlined: `Player.owner = player; updateSelectInfo(-1)`. The first version used
  two unused locals for the frame; they were the inlined helper's two parameters. N64: no gCreateMenuNotify, B deletes
  a character, censor(name) before CreatePlayer, `setPlayerState(p,0)` after the if for both name cases.
- 95E8C: the frame/spill-home order needs a real local `pak = gSelectPakIdx[p]` read before `origSelect`, and the
  player-on-pak compares are `item->player != gSelectPakIdx[p]` (operand order sets the temp-ring phase). 14 menu
  items (wrap at 13, MAT3 14).
- 94DD8: 22 buttons (index 14 is a "controls" arrow shown next to the selected item, 15 = second row of index 1, 16 =
  title, 17 = cursor); show-all loop reuses `buttonIdx`; the type-9 button x is `buttonX + 0 * gButtonScale` (int
  zero: uopt folds a float `0.0f * x`, and the zero must be the same constant web as the other zeros, so every zero in
  the loop is spelled `0`); 255 is spelled `255.f` in one place and `255.0f` in the other (one shared constant
  wrongly gets an FP register and spills buttonY); `fabsf(x) * 60.0f` operand order. Two unused locals declared before
  prevPos are still needed (frame 152 -> 160); they are probably the parameters of two inlined statics (stubs
  func_80395E14 / func_80395E1C follow it) but no static form that matches was found.
- 96FA0 (not matched): `claimPlayer` inlined 4x gives the 88-byte frame (research body: 56). Everything is
  structurally equal except one hunk: retail colours the `lda D_803BADB8` of the first loop store a3
  (`lui a3 / addiu a3 / addu t8,a3,s0 / sb zero,0(t8)`), we fold it into the sb offset; the lda web {loop-top store,
  two toggle blocks} is split by the colourer (tot 11 < bestcost 18, a-registers forbidden because the web spans calls)
  and constant 13 then takes a3 (`li a3,13` hoisted). Source edits on the store, its order and the 13 (400 variants in
  `vgen`) do not move it. 262/718 words differ, all downstream temp-ring naming.

Root levers (all needed, found in this order):
1. `updateButtons(playerIdx)` at the loop tail needs a signed-char VARIABLE: `s8 q;` assigned `q = playerIdx;` at the end
   of every switch arm and `default:`, passed as `func_80394DD8(q)`. A plain argument (or a cast, or a local assigned
   at the loop top) is elided by IPA (raw `sw s8,0(sp)` everywhere). With `q` PRE/colouring gives retail's per-path
   `sll v0,s8,24 / sra t8,v0,24 / move v0,t8` and one common `sw v0,0(sp); jal`. (949CC's `s8 player = i` shows the same
   pattern.)
2. The allReady loop reuses `playerIdx` and the cleanup loop's inner index is `p` (retail's dead `move s8,zero`, the
   spilled walking pointer, the s1/s0 colours); `player` is block-scoped in both loops.
3. Frame: seven unused s32 locals (u0..u6) plus `q` = 32 bytes (frame 160 -> 192, first spill home sp+116, s8 home
   sp+188). Not an array; disclosed in the header.

Residual of func_8039842C (122/451 words, size and frame equal): (a) retail loads `gNumPlayers` for the main-loop
preheader test into a1 (`lui a1 / lh a1 / ... / blez a1`), we get the ring temp t9, which shifts the temp ring one slot
for the rest of the function; no single-web force (all 37 phase-1 webs x colours v1/a0/a1/a2 tried with `force.sh`)
changes the strict count, so the retail load is a coloured expression-web piece that ours does not form; (b) the second
spill home is sp+124 (retail) vs sp+120: retail has one more coloured expression web between the walking pointer
(sp+116) and the v0 spill. Neither responds to loop-form, declaration-order, guard, `n`-local or compiled-out-read
variants (about 40 variants tried in the lane, 7 declaration orders).

## What generalises

- Frame residuals of 2/3/4 words next to inlined statics are the statics' PARAMETERS: look for caller-less
  `jr ra; nop` stubs and define the helper (non-static, after the caller, not on keep). 968C0 and 97FFC went from
  "unused locals" / invented locals to genuine helpers; 96FA0's 32 bytes = 4 inlined claimPlayer calls.
- An int argument passed to an IPA-internal callee with a `signed char` formal is elided by IPA; if retail still
  shows `sll/sra/move vN,tM` per path, the source has a signed-char variable assigned in each arm.
- A float `0.0f * x` is folded by uopt; retail keeps `mul.s f,fzero,x` when the zero is spelled as an int `0`
  (and then every other zero of that web must be `0` too).
- Constants spelled the same share one uopt web (hoisted into an FP callee-saved register); `255.f` vs `255.0f`
  splits them. Same for `13` etc: they cannot be split by a different spelling of an int.
- `!=`/`==` operand order between two memory operands sets the temp-ring phase (`item->player != pak`).
- as1 breaks scheduling ties on the lower source line: order helper definitions/statements so the intended
  instruction comes later (97FFC delay slot).
- MAT3's two-line repeated "find next/previous profile on this pak" blocks are inlined statics in the N64 as well;
  hand-expanding them matched (95E8C).
