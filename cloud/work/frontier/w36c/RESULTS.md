# w36c results (runtime image A: MAT3 playerscr.c / pakerr.c / save.c)

Builder scratch: `watchman2:~/rush2049/scratch/frontier/w36c` (copied from base, master e68aea6e1).
Only this lane directory (and cloud/matches/ for standalone strict matches) is written.

## Status

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| func_803A9088 | drawDeletePlayer@playerscr.c (GC 0x8007d8f0), confirmed (sole caller animText A9EEC case 4) | 3668 | 15/917 (installed context, spill homes only; traced; a lever reaches 13) | installed body (src/ovl_a/groups/runtime_a_profile_menu_20261006) |
| func_803915E8 | updateSelectButtons@recordscr.c (GC 0x80084f38), confirmed | 3528 | **MATCH in group** (own data hand-verified) | groups/dot_runtime_a_callers_deep_20261011/ |
| func_80396FA0 | controlSelect@playerscr.c (GC 0x8007a854), confirmed | 2872 | **MATCH in group** (jump table hand-verified) | groups/dot_runtime_a_availability_refresh_20261007/ |
| func_8039338C | controlSelect@recordscr.c (GC 0x80080874), confirmed | 1960 | **MATCH in group** (no own data) | groups/dot_runtime_a_callers_deep_20261011/ |
| func_8039842C | updatePlayerScreen@playerscr.c (GC 0x80079f80), confirmed | 1804 | **2/451** (was 122): one spill home (sp+120 vs 124) | context in groups/dot_runtime_a_availability_refresh_20261007/ |
| func_803A9EEC | animText@playerscr.c (GC 0x8007ed3c), confirmed; drawControls (stub A8B6C), drawPakFull (stub A9EDC), drawPakDamaged (stub A9EE4) inlined | 1260 | 102/315 strict, structurally 2 words (was 281): frame/locals now exact (str1[44]); residual = default-block placement, which shifts case 6 by 8 bytes | A9EEC/best_group.c |

Matched bytes this lane: 3528 + 1960 + 2872 = **8,360 B** of image A (three functions, two group revisions).

## Deliverables

1. `groups/dot_runtime_a_callers_deep_20261011/` supersedes the installed group of that name (recordscr unit, kept root
   func_80394058). Six old members byte-identical and MATCH; adds members+claims func_803915E8 and func_8039338C.
   `allow_unverified: true` (own .rodata/.data hand-verified, see 915E8 section). Context edit: D_80140BDC/D_8002EB94
   declared volatile (C needs one qualifier per unit; MAT3 has both volatile). w36f works on context func_803923B0 in
   the same group: merge by taking their 923B0 body and keeping these two members and the volatile declarations.
2. `groups/dot_runtime_a_availability_refresh_20261007/` supersedes the w35d revision installed under that name
   (playerscr unit, kept root func_8039842C). Eight old members byte-identical and MATCH; func_80396FA0 moved from context
   to members+claims; two disclosed compiled-out `do {} while (0);` lines (one in 96FA0, one in context 9842C).
   `allow_unverified: true` (inherited). Note: func_80396FA0 is also listed as context/keep in the installed
   runtime_a_profile_menu_20261006 group (an inherited stand-in there; that group is unchanged by this lane).
3. Nothing in cloud/matches/ (all three matches are IPA-internal and only match inside their unit).

## Permission denials

none

## Identities proven

- func_803915E8 = updateSelectButtons@recordscr.c (GC 0x80084f38): strings, constants, structure; MATCH.
- func_8039338C = controlSelect@recordscr.c (GC 0x80080874): MATCH. (The correlation lead "SaveServer" was wrong.)
- func_803923B0 = updateButtons@recordscr.c with updateViewButtons inlined (case 1); func_80394058 = updateRecordScreen
  (controlView inlined); func_80393B3C = initRecordScreen (initButtons inlined); func_803931C0 = setRecordState;
  func_803932E4 = uninitRecordScreen (by call structure, MAT3 recordscr.c order and globals).
- func_803A9088 = drawDeletePlayer, func_803A9EEC = animText@playerscr.c (blit callback), func_803A7B58 = drawSelect,
  func_803A87D4 = drawWait, func_803A8B74 = drawCreatePlayer (animText's switch order).
- Deleted-static stubs (emission position of the inlined helpers in our object vs retail): 0x803A8B6C drawControls,
  0x803A9EDC drawPakFull, 0x803A9EE4 drawPakDamaged, 0x80395E14/0x80395E1C controlPakFull/controlPakDamaged,
  0x80396ED8/0x80396EE0 controlControls/controlWait, 0x803968B8 claimPlayer (w35d's helper is NAMED func_80396EE0 but IDO
  emits it at 968B8), 0x803949C4 probably uninitButtons.
- Globals (recordscr): D_803B5F28 buttonInfo[23], D_803B64E8 gButtonScale, D_803B64EC gButtonSpace, D_803B64F0 buttonX,
  D_803B64F4 maxY, D_803B64F8 minY, D_803B64FC buttonZ, D_803B6500 cursorZ, D_803BA7D0 buttonsInited, D_803BA7D4
  buttonsJustInited, D_803BA7D8 cursorSpeed, D_803BA7DC scrollSpeed, D_803BA7E0 gSelectMenuValid[11], D_803BA7F0
  gSelectMenuVisible[11], D_803BA800 gStatPlayer, D_803BA804 gRecordState, D_803BA808 gSelectMenuSelect, D_803BA80C
  gSelectMenuClear, D_803BA830 gNumPakPlayers[4], D_803BA840 gDeleteName, D_803BA84D gDeleteConfirm, D_803BA850 gYes,
  D_803BA854 gDeleteNotify, D_803B65A4 gSelectPakIdx, D_803B65AC/65A8/65B0 the white buttonColor initialisers.
  Calls: func_800B5F88 PlayLRSound, func_800B5F4C PlayUDSound, func_800B5FC4 PlayStartABSound, func_800CD544
  ClearPlayerRecords, func_800BE6A4 TB_Strcpy, func_800CCEFC ReadPlayerData, func_800A3424 PS_LockFile, func_800C9404
  ClearTotalStats, func_800F769C AddPlayerStats, func_800C734C AddPlayerScores, func_800C7308 PurgePlayerData,
  func_800B5570 setGameState, D_80146150 gJustPlayers[4], D_8012E6D8 gPlayerFiles.

## What generalises

- **A compiled-out block boundary (`do {} while (0);`, or a compiled-out read) is a colouring-priority lever.** Two of
  this lane's residuals that w35d left as "colour-only" (96FA0 262 words, 9842C 122 words) closed to 0 and 2 with one
  such line each. Workflow that found them in minutes: `ctrace.sh` to find the competing webs (here a constant web
  vs an lda web, both split, whose pieces race for a3), `force.sh ... "p1:wN=s"` as the oracle (0 words), then a vgen
  sweep that drops `do {} while (0);` / `if (glob) {}` at 4-8 statement boundaries (256 variants).
- `force.sh`/`pretrace.sh`/`spill.sh`/`ugt.sh`/`as1t.sh` work on overlay groups too: `snap.sh LABEL group.c KEEP` stages
  the group; only `udiff` fails (it looks up blob targets). `w36c/ocmp.py OBJ NAME` scores any fetched object against
  image A locally.
- `default: break;` matters for loop-increment PRE through a jump-table switch: without a separate default block IDO
  does not PRE the increment at all. as1 may hoist a default-block instruction into the dispatch block's load stall unless
  the register is live at `j $24` or the block has two predecessors (hand-edited listing + `asm.sh`).
- Inlined-static frame residuals: the MAT3 helpers are inlined in N64 too, and their deleted stubs sit in MAT3 source
  order (drawControls between drawWait and drawCreatePlayer). Text buffers can be larger than MAT3's (str1[41..44] vs 40).
- MAT3 direct typed ports match first time when the unit context is right: 9338C (no shaping at all) and 915E8 (the
  sibling 94DD8's idioms + one 255 spelling split) both matched within two compiles.

## func_803915E8 = updateSelectButtons@recordscr.c: MATCH (group)

Identity: MAT3 recordscr.c updateSelectButtons (GC 0x80084f38, 2656 B). Caller func_803923B0 (= updateButtons@recordscr
with updateViewButtons inlined; lane w36f) calls it for gRecordState 0. Same structure, strings ("BUTTON_SELECT"/"BUTTON"
twice each, 0x803B8C30/40/48/58), constants (-165, 540, 180, 60, 10, -80, -120, 160, 120), 23 buttons, 11 menu items.
N64 differences from MAT3: no buttonsInited test (it is in the caller), out-of-range alpha is the 255-ramp formula
(MAT3: 80), type-11 x is `buttonX + 0 * gButtonScale`, floats instead of doubles for the 60/10 speed terms.

Deliverable: `groups/dot_runtime_a_callers_deep_20261011/` (superseding revision of the installed group, same name;
the six old members are byte-identical sources and still MATCH; func_803915E8 added to members and claims;
`allow_unverified: true`). Builder: `score.py group lane/groups/dot_runtime_a_callers_deep_20261011 --targets asm/us/ovl_a --claims`, exit 0:

```
Members:
func_803915E8:
  MATCH (20 section-relative relocations unverified: .data+0x0 at +0x8, .data+0x0 at +0xc, .rodata+0x30 at +0x234, .rodata+0x30 at +0x238, .rodata+0x34 at +0x428, .rodata+0x34 at +0x42c, .rodata+0x38 at +0x43c, .rodata+0x38 at +0x440, .rodata+0x3c at +0x46c, .rodata+0x3c at +0x474, .rodata+0x40 at +0x488, .rodata+0x40 at +0x4ac, .rodata+0x0 at +0x78c, .rodata+0x0 at +0x7a4, .rodata+0x10 at +0x760, .rodata+0x10 at +0x770, .rodata+0x18 at +0xc4c, .rodata+0x18 at +0xc64, .rodata+0x28 at +0xc20, .rodata+0x28 at +0xc30)
    not verified: ... no retail data available   (x9, expected on the builder)
Context: func_80392F28 MATCH, func_80393004 MATCH, func_803930B8 MATCH, func_803931C0 MATCH, func_803932E4 MATCH,
         func_80394058 MATCH, func_8039338C 475/490, func_80393B3C 203/327, func_803923B0 679/734
```
Own data, hand check (`rocheck.py` on the group object vs build/ovl/a/image.bin): 9/9 .rodata sites OK (f32 0x803B942C
6.2831855, 0x803B9430 1.5707964, 0x803B9434 0.4, 0x803B9438 12.566371, 0x803B943C 3.1415927; str 0x803B8C30, 0x803B8C40,
0x803B8C48, 0x803B8C58); .data+0 = ffffffff = retail 0x803B65AC (the buttonColor initialiser; the group has no other .data).

Levers: the w35d 94DD8 idioms carried over unchanged (int `60`/`10`/`540`/`180`, `0 * gButtonScale`, volatile
gFrameSeconds/gNumModels, `D_803B64E8 * 0.4f`); the only search was the 255 spelling (81 variants, first try
`255.f - (...) * 255.0f` matched). The frame needs the same two unused s32 slots before prevPos as 94DD8 (disclosed).
Context edits (not claimed): the 93B3C and 923B0 context declarations of D_80140BDC / D_8002EB94 became volatile
(C requires one qualifier per declaration in the unit); their scores moved 288->203 and 666->679. w36f owns 923B0:
when merging, keep `extern volatile f32 D_8002EB94` (MAT3 says volatile, and 915E8 needs it).

## func_8039338C = controlSelect@recordscr.c: MATCH (group, first port)

Identity: MAT3 recordscr.c controlSelect (GC 0x80080874, 1928 B): same three-way menu walk over gSelectMenuValid[11]
(D_803BA7E0) with gStatPlayer (D_803BA800) stepping through gPlayerFiles (D_8012E6D8) filtered by pak, the
delete-confirm/yes/notify bytes (D_803BA84D/850/854), the A-button view branch (ReadPlayerData / PS_LockFile /
setRecordState(1), or ClearTotalStats + AddPlayerStats/AddPlayerScores over gJustPlayers[4] (D_80146150) and the
file list) and B = uninitRecordScreen (func_803932E4) + setGameState(16). N64 differences: no MtapConnected test
(the pak index wraps 0..3 always), gText.single[177] (GC 187). Called directly from the kept root func_80394058
(= updateRecordScreen); IPA-internal (keeps addresses in unsaved s0-s8, frame 24).

The installed context body (native-led, 475/490) was replaced by the direct MAT3 port, which matched on the first
compile: statement order, loop forms and `while`/`do`/`break` shapes exactly as MAT3, no shaping at all.
Builder, same group dir, `--claims`, exit 0: `func_8039338C:\n  MATCH` (full output in 9338C/claims_out.txt).

## func_803A9EEC = animText@playerscr.c: 261/315 (in progress)

Identity: MAT3 animText (GC 0x8007ed3c): HAL_SetTextDepth(0) / per-player switch on gPlayerState (D_803BACE0) /
HAL_SetTextDepth(-1) / return 1; the blit callback (data_ref, kept root of runtime_a_profile_menu_20261006). N64: no
updatePlayer call, no FormatMC case (7-entry jump table at 0x803B9890). The three small draw statics are inlined and
their deleted stubs sit in MAT3 source order: func_803A8B6C = drawControls (between drawWait A87D4 and drawCreatePlayer
A8B74), func_803A9EDC = drawPakFull and func_803A9EE4 = drawPakDamaged (between drawDeletePlayer A9088 and animText).
Defining drawControls as a real helper gives the retail frame 280 and playerIdx home sp+276 (281 -> 277).

Open residual: retail PREs the loop increment `playerIdx + 1` into s0 on every path (the drawPak* `p + 1` argument
is the same expression; the out-of-range path has its own block 0x803AA354 holding `addiu s0,s2,1`). Experiments
(t/pre*.c): IDO only does this PRE through a jump-table switch when the switch has a separate default block; with a
plain switch the increment stays at the latch (277). `default: break;` turns PRE on but uopt hoists the default's copy
into the dispatch block (261). `default: break; case 7: break;` reproduces the retail CFG exactly (54 words, all of
them the 4-byte local-area shift below) but makes the table 8 entries, so it is not the source. Second residual: the
local area needs one more 4-byte slot above drawPakFull's str1 (retail str1 at sp+208/144, temps at 128/132; ours
212/148, 132/136). An unused local does not take a slot (tested). Tried and failed (A9EEC/b4..b10): default bodies `;`, `break`, `continue`, `(void)0`, `if (playerIdx) {}`,
a dead block local, `playerIdx = playerIdx`, an inlined empty/non-empty helper called from `default:`, a `state` local,
default first, unused locals (do not take a slot), loop-increment spellings. Best legitimate form: `A9EEC/best_group.c`
(MAT3 shape + drawControls helper + `default: break;`, 261/315). Retail's out-of-range edge has its own block
0x803AA354 (`addiu s0,s2,1`, as1 also steals it into the case 0-4 delay slots); ours always gets that edge merged
into the latch and the PRE copy hoisted into the dispatch block. Unresolved; parked.

## func_8039842C = updatePlayerScreen@playerscr.c: 122/451 (unchanged count, less shaping)

The w35d frame residual (seven unused s32 locals u0..u6, frame 160 -> 192) is exactly the four MAT3 control statics
that the N64 inlines into the root: controlWait (case 1), controlControls (case 2; the N64 moved it from MAT3's second
loop into the switch, with its own updateButtons(p) call), controlPakFull (case 5) and controlPakDamaged (case 6).
Written as non-static helpers defined after the root (`9842C/helper_form_group.c`), the root needs only `q` and no unused
locals, and scores the same 122/451 (size, frame 192, spill homes as w35d). The `q = playerIdx` per arm is still
required: a plain `updateButtons(playerIdx)` after the switch loses retail's per-path (s8) conversion (357/451).
The w35d residual (gNumPlayers preheader load coloured a1 in retail, ring temp in ours) is unchanged.

## func_803A9088 = drawDeletePlayer@playerscr.c: 15/917 (installed context, traced; lever reaches 13, not delivered)

Identity confirmed (MAT3 drawDeletePlayer, GC 0x8007d8f0; sole caller animText case 4; MAT3 locals x, y, cx, str,
str1[128], paneT, menuItem, screenPos[2] map onto the retail homes 268/264/260/.., 128..255, .., 112). The installed
body (research packet dot_runtime_a_delete_profile_20261012) is 15/917; all 15 words are spill-temp homes.
Traced with the toolkit (`snap.sh pm1 ref/pm_installed/group.c func_803A9EEC`, `spill.sh`, `pretrace.sh`):
`f_spilltemps` hands out homes in itable order from sp+72 down: bit 4 = the coloured gNumPlayers load
`ilod.J@0(ldaS(D_8014A108))` -> temp0 sp+72 (never spilled, invisible), bit 17 player*4 -> 68, bit 21 slot base -> 64,
bit 24 camera -> 60, then bit 37 = short-x `cvtl.J(x)`: in ours it shares no block with bit 4 and REUSES temp0
(sp+72), so short-y (bit 43) takes the new temp4 (sp+56) and the loop's slot/gSel pointers come out swapped
(52/48). Retail has short-x at sp+56 = temp4: there the short-x web conflicts with the gNumPlayers web (or with temp0's
owner), so it gets the new slot and short-y reuses temp0. The source construct that makes the gNumPlayers web span a
block where short-x is live is the open question (the research packet reached the same point). Statement-order
variants (A9088/b1, 12 variants) do not move it.

Stub identities found on the way (emission position of our helpers vs retail stubs): 0x80395E14/0x80395E1C =
controlPakFull/controlPakDamaged (inlined into updatePlayerScreen), 0x80396ED8/0x80396EE0 = controlControls/controlWait
(same), 0x803968B8 = claimPlayer (w35d's group names its helper func_80396EE0 but IDO emits it at retail's 968B8
position), 0x803A8B6C = drawControls, 0x803A9EDC/0x803A9EE4 = drawPakFull/drawPakDamaged (inlined into animText);
0x803949C4 probably uninitButtons (inlined into uninitScreen 949CC); 0x80398B38 unknown.

### A9EEC update: the as1 hoist is understood; best now 114/315

The traced pipeline (`ugt.sh`, `as1t.sh`, `asm.sh` on hand-edited listings) shows that with `default: break;` ugen's
output already has retail's shape: `$257: addu $16,$18,1` (the default block) falls into the latch `$258`. as1 then
hoists that `addu` into the dispatch block to fill the `lw $15,132($sp); lw $24,0($15)` load stall and retargets
the range check to the latch. Retail's as1 did not. Hand edits prove what blocks the hoist: a second predecessor of
`$257` (`b $257` from case 6) or a `.livereg` marking `$16` live at `j $24` (either one gives retail's dispatch block).
Source with that effect so far: `default: break;` written BEFORE `case 6:` (A9EEC/best_group.c, 114/315): no hoist,
but the default block is then laid out between cases 5 and 6 (`b latch; addiu s0,s2,1`) instead of retail's fall-
through at 0x803AA354 (2 words). The other ~112 words are the local-area shift: retail's cfe locals area is one
4-byte slot larger (`spill.sh`: ours `AREA f_readnxtinst area=134`, temps from sp+140; retail temps 4 lower; retail
str1 buffers at sp+208/144, ours 212/148). Unused locals, extra used locals in animText scope, local order inside the
helpers and an extra local in drawControls do not change the area (b13, b14); umerge's layout of inlined locals is the
thing to understand next.

### A9EEC update 2: local area fixed (102 strict, one 2-word structural hunk)

The missing 4-byte slot is the two drawPak* text buffers: `u8 str1[N]` with N = 41..44 in BOTH helpers gives the
retail homes (str1 at sp+208/144, temps at 128/132); MAT3 has 40. One helper alone does not (b16). `A9EEC/best_group.c`
uses 44 (disclose: capacity 41..44 are code-identical). Remaining: retail's default block (0x803AA354) falls into the
latch after case 6 and as1 leaves it there; ours with `default: break;` last gets the addu hoisted by as1 into the
dispatch block (hand-edited listing tests: only a `.livereg` marking $16 live at `j $24`, or a second predecessor of
the default label, stops it; an empty `.livereg`, an s2-only `.livereg` or a different `.loc` does not). Writing
`default: break;` before `case 6:` stops the hoist but lays the block out before case 6 (2 words, plus case 6's code
shifted by 8 bytes, hence 102 strict). Default-first placements are worse (b18).

## func_80396FA0 = controlSelect@playerscr.c: MATCH (group), 262 -> 0

Traced with the toolkit (`snap.sh av1 groups/av/group.c func_8039842C`, `ctrace.sh func_80396FA0 av1`, `pretrace.sh`):
the two competing register candidates are w192 = constant 13 (LIVE-RANGE blocks 12, 130: the two `sel = 13` wraps)
and w191 = `lda D_803BADB8` (blocks 11, 96, 109: the first-loop store and the two toggles). Both whole webs split
(13: totalsave 19 = bestcost 19; lda: 11 < 18); because the 13 web's whole-web save (0.5758) is above the lda web's
(0.44), its first-loop piece (save 1.67) is re-decided first and takes a3 (`li a3,13` hoisted). Oracle:
`force.sh av1 func_80396FA0 "p1:w192=s"` -> 0 differing words (the lda piece then takes a3, exactly retail).
Source lever (96FA0/b3, 256 variants, 3 winners): one compiled-out block boundary in the right place changes the
block counts that set those priorities: `do {} while (0);` between the edges chain and the autoFlags chain
(chosen: it is the shape of an empty release-build macro between the two N64-separated chains), or
`do {} while (0);` / `if (D_803BAD80[p]) {}` right after the first while loop. Disclosed as SHAPING in the header.

Deliverable `groups/dot_runtime_a_availability_refresh_20261007/` = the installed w35d group, byte-identical except
that one line, with func_80396FA0 moved from context to members/claims. Builder `--claims`, exit 0:

```
Members:
func_80396FA0:
  MATCH (2 section-relative relocations unverified: .rodata+0x44 at +0x460, .rodata+0x44 at +0x468)
    not verified: .rodata+0x44 at +0x468: no retail data available
Context: func_80394564 MATCH, func_80394834 MATCH, func_80394B00 MATCH, func_80397AD8 MATCH, func_80397FFC MATCH,
         func_803968C0 MATCH, func_80395E8C MATCH, func_80394DD8 MATCH, func_8039842C 122/451, func_803949CC MATCH,
         func_80395E24 MATCH, func_80396EE8 MATCH, func_80396F44 MATCH
```
Own data: the 14-entry jump table at retail 0x803B9484 (object .rodata+0x44): entries as offsets from the function
start are equal (`jtcheck.py`: 0x474, 0x644 x10, 0x568, 0x5a4, 0x5ec); `rocheck.py` over 96FA0/94DD8/97FFC/97AD8: 22
sites, 0 bad (the w35d own-data proof still holds on this object).

## func_8039842C update: 122 -> 2/451

Same lever as 96FA0: a compiled-out `do {} while (0);` between `if (!screenInited) initScreen();` and the main player
loop (9842C/b3, 243 variants; also `if (D_8014A108) {}` there) changes the block counts so the gNumPlayers preheader load
is coloured a1 as in retail, and the whole temp ring falls into place. It works on the w35d body (7 unused locals u0..u6
+ q); on my helper form (controlWait/controlControls/controlPakFull/controlPakDamaged inlined) it gives 10 words (local
area 4-8 bytes larger), so the w35d form stays. The last 2 words: the reset loop's walking pointer (v0 across
setPlayerState) is spilled at sp+120, retail sp+124. `spill.sh av4`: our cfe locals area is 68 (homes from sp+120:
temp0 = bits 0/10/14/59/74/144, temp1 = 116, ...); retail's walking pointer at 116 and v0 spill at 124 imply a cfe area of
64 plus one more coloured expression web before the 116 one, i.e. one fewer local than w35d's u0..u6 and one more
expression web: the real (inlined) locals are still unknown. 6 or 8 unused locals, s16/s8/f32 types, q placement: worse.
Included in the deliverable group as improved CONTEXT (all members still MATCH).

### A9088 lever round (not delivered)

Block-boundary sweeps (A9088/b2, b3, 600 variants): `if (D_8014A108) {}` just before `if (D_803BADCC[player]==1)` makes
the short-x web conflict with the gNumPlayers web, so short-x gets its own home sp+56 as in retail (the research packet's
diagnosis, now reproduced); 13/917 is the best (`A9088/lever_b3_v0005_13words.c`, three compiled-out reads). The
remaining words: constant 1 takes v1 instead of v0 in the confirm/notify tests (the extended gNumPlayers web now holds v0
there) and the loop's slot pointer / gSelectedPlayer homes stay swapped (48/52). Not delivered (more shaping, small gain).
