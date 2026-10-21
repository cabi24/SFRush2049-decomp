# w36x2 RESULTS (wave 36 second pass, runtime image A: trackscr near-misses + UpdateSelectCar + updateWarningScreen)

Builder scratch: watchman2:~/rush2049/scratch/frontier/w36x2 (copied from base, master e68aea6e1).
Base packet: cloud/work/frontier/w36e/groups/dot_runtime_a_c93c_closure_20261012/ (12 claims; read-only).

## Deliverable

**groups/dot_runtime_a_c93c_closure_20261012/** = w36e's 12-claim packet, byte-identical except func_8038A820
(animTrackMaps, context): MAT3-style `if (D_803B7CE4[trackIdx].handle == -1) continue;` + header comment. Same name,
same members/claims/context/keep (group.json copied unchanged). Supersedes w36e's copy only if the integrator wants
the better A820 context; it adds NO new claims (no new matched bytes). Builder, exit 0:
`score.py group lane/groups/dot_runtime_a_c93c_closure_20261012 --targets asm/us/ovl_a --claims` -> C93C, BBB4, DFEC,
ACB0, B834, A634, AE50, B388, B940, AEB0, B3EC, C5EC `MATCH`; context E5DC `412/926 words differ`, CD14
`1085/1206 words differ`, A820 `249/292 words differ` (positional: A820 is one word short; aligned it is the single
mapPos `lui at`, see below). Own data unchanged from w36e's packet (A820's literals/static identical).
No standalone matches this lane. work/ holds scratch copies (not packets).

## Status

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| func_8038E5DC | updateTrackScreen (kept root) | 3704 | 1 insn off (case 14 store); genuine spelling NOT found: retail stored through a second symbol MAT3 cannot show (see section) | packet body unchanged |
| func_8038CD14 | buildTrackMaps (+inlined race/stunt/battle/obstacle, load/unloadPathData) | 4824 | NOT matched; lead: `s32 size[2]` array reproduces retail's kept size stores (dl half exact); 1005 -> 404/1206 positional, 21 -> 14 normalised hunks | func_8038CD14/best_unit.c (+README.md) |
| func_8038A820 | animTrackMaps | 1168 | SOURCE-CORRECT: `if (handle == -1) continue;` removes the extra trackIdx spill; the only remaining word is one `lui at` that retail emits because its assembler saw mapPos at a section offset == 0x7FF8 (mod 64K) (%hi carry between mapPos+4 and +8). Proven: the revision's ugen listing + `.space 0x7FF8` before mapPos assembles to retail 292/292; the original layout itself is not reconstructed | groups/dot_runtime_a_c93c_closure_20261012 (context) |
| func_80390DCC | UpdateSelectCar@selectcarload.c | 1700 | 33 words (all spill homes -4), NOT matched; frame/home law pinned down (see section) | w36e/func_80390DCC/group_best (unchanged) |
| func_8038A400 | updateWarningScreen@warningscr.c | 556 | 3 words short (was 2 layouts): pak-loop residual CLOSED by MAT3 function-local `static Blit *sBlit`; controller-loop layout remains | func_8038A400/best.c |

## func_8038A820 (animTrackMaps)

1. Extra `sw v0,92(sp)` at the loop head: the handle==-1 edge entered the block holding the trackIdx reload. MAT3-style
   `if (D_803B7CE4[trackIdx].handle == -1) continue;` (or any ucode statement after the inner `if (trackIdx < 6 ||
   trackIdx == 19) {...}`) gives uopt a separate join block for the inner exits; the reload goes there (retail 0xac24)
   and the loop-head store disappears. (w36e's 'continue form' attempt looked worse only because the positional count
   went 61 -> 249 when the function became one word short; the aligned diff goes 15 -> 11 rows, all from point 2.)
2. Remaining: retail `lui at; lwc1 mapPos+8; lui at; ...; lwc1 mapPos; ...; lwc1 mapPos+4` (two luis), ours shares one.
   as1 shares `lui at` between offsets of a symbol defined in the unit only when the %hi16 ADDENDS (section-relative
   offsets, REL relocations against .data) are equal; for an extern it never shares (tested: 3 luis). Builder test
   (astest/, tools/padcheck.sh): with `.space 0x7FF8` before mapPos in .data, as1 emits exactly retail's split, and
   the revision's A820 ugen listing assembled that way equals retail 292/292 words (relocation fields masked; with
   no pad: one word short). So the retail assembler saw a %hi carry between mapPos+4 and mapPos+8, i.e. mapPos
   (0x803B8388) at a section offset == 0x7FF8 (mod 64 KB). The final image does not show such a section start
   (image A .data appears to begin at 0x803AF980 and 0x803B0390 falls inside a 64-byte tMenuButton array), so the
   original object/section layout that produced it is NOT reconstructed - only the effect is proven. Function-local
   statics sit in .data in MAT3 source order (B3EC cityPos/mapPos/rotAngle 0x803B8338.., race x2 0x803B8358/64,
   stunt 0x803B8370, battle 0x803B837C, A820 mapPos 0x803B8388), followed by warningscr's statics (gPakScr 0x803B839C
   .. sBlit 0x803B83C8) and string data (0x803B83D0) although warningscr's code is first in .text. A per-TU group
   object places mapPos at a small offset and as1 shares the lui: A820 cannot be claimed from a per-TU group; it
   needs the original data layout (integrator/tooling decision). Only .data objects straddling such a boundary are
   affected (mapPos is the only one seen).

## func_8038E5DC (updateTrackScreen) - genuine spelling of the case-14 store NOT found

Evidence gathered (all negative for a natural source form):
- GC: rush_d.elf disassembly of updateTrackScreen (0x80087ABC) case 14 is plain and identical in shape to case 15
  (`lis/addi r3,gOptions; ... stb r0,0x1d(r3)` for every store); MAT3 case 14 = `gOptions[29] = 3`.
- MAT3/GC data: nothing but gOptions[41] covers 0x80168268..+0x29 (rush_d_variables.csv), and trackscr.c's DWARF
  globals list has no other symbol there; MAT3 declares no other option global. w36y map: only gOptions (proven).
- N64 ROM-wide scan (game + images A/B): 0x80146125 is never accessed absolutely anywhere else; no separate symbol.
- uopt trace (pretrace, st_base vs st_alias): the store is `istr.J@29(ldaS(21+0),3)`, a use of gOptions' coloured LDA
  web w429 (v0, 24 blocks). Retail's store is outside that web. Every C spelling of the same symbol is canonicalised
  to ldaS(gOptions+0)@29 and folds: pointer locals (block/function scope, &a[29], a+29, &a[21]+8, a; p[29]), casts
  (u8*, volatile s8*, struct overlay, s8(*)[41]), u32 arithmetic, variable index (i/j/k/add = 29), block-scope extern
  redeclarations (same type; different types are cfe errors). A numeric address `*(s8 *)0x80146125` is a different
  shape (`lui t9` hoisted, 136 rows). Only a second SYMBOL at that address reproduces retail (the w36d alias diagnostic
  `D_8014611D[8] = 3` = strict MATCH). Retail therefore stored through a different symbol name (e.g. a linker-defined
  alias in the N64 overlay link) that MAT3 cannot show. Per the lane brief a synthetic symbol is not acceptable: not
  delivered. Owner decision needed if an alias symbol is ever allowed.

## func_80390DCC (UpdateSelectCar) - frame/home analysis (not matched)

All 33 residual words are spill homes 4 bytes low (ours 88..108, retail 92..112). spill.sh facts (w36e group_best):
uopt Udef = 120 with w36e's 13 unused locals; f_spilltemps hands slot k the home `frame - Udef - 4(k+1)`; 6 slots;
frame = round8(area) + 88.
- With umerge inlining (CreateSelectCar inlined, stub 0x80390BB8) Udef is always a multiple of 8 (padding sweep:
  12 u's -> 120, 11 -> 112, 10 -> 112; padB[46..52] all 120). Retail homes need either
  (A) Udef 112 + ONE MORE coloured expression web that takes spill slot 0 (sp+116, never stored) and conflicts with
      every other home web (first in itable order, before &D_803BA2F0[slot]); or
  (B) no umerge inlining: hand-inlined CreateSelectCar body (`{ s32 c = cam; ... }`) gives an UNROUNDED Udef
      (10/11/12 u's -> 100/104/108). v/u7/inl2_u12: homes then equal retail exactly (frame 224 vs 232; locals 8
      low); Udef 116 (= 2 more 4-byte locals) would give frame 232 and every home/local offset; remaining diff is
      the as1 order of `sb t8,0(s0)`/`sb zero,1(s0)` around the music_seq_load jal (0x2c8..0x2e4), which matches
      only with umerge inlining (callee line numbers). (B) needs 16 unexplained 4-byte locals - not adopted.
- Tried without success: idx s32/u16/late, `slot % 13` at single uses, cam s32, if-chain for the switch, `else if`
  player fold, alphaCol aggregate initializer (union with c[4] first), named LoadState pointer, callees
  LoadSelectCar/UnloadSelectCar in the unit (kept: no change; internal: breaks them), CreateSelectCar parameter
  types (sss/iis/isi/sii/ssu/ssus). The extra early web of (A) is still unidentified.

## func_8038A400 (updateWarningScreen)

`score.py fn lane/func_8038A400/best.c func_8038A400 --targets asm/us/ovl_a`: `118/139 words differ` positional (ours
136 words: every word after the controller loop is shifted by 3; aligned diff = only the controller loop + branch
offsets). Progress over w36e var/n.c (50/139, 2 residuals):
1. MAT3 has `static _tBlit *sBlit;` INSIDE updateWarningScreen. As an extern (D_803B83C8) uopt PRE-inserts the sBlit
   load into the pak-loop goto edge (`lw a0,sBlit; b done`), so the goto becomes `beqz v0,inc; ...; b done`. As a
   function-local static the done block reloads it itself (`lui t5; lw t5; ...; move a0,t5`) and the goto is
   retail's `bnez v0,done`. (gPakScr as a file static: no effect; numPaks static: worse.)
2. Controller scan inline (helper form = same code but leaves a deleted-procedure stub retail does not have).
Remaining (3 words): retail `lb; beqzl t8,inc; b exit; nop; inc: addiu; sltu at,p,end; bnezl` - IDO here gives
`lb; bnez t8,exit; addiu; bnel p,end`. Tried ~40 forms (helper/inline; i/pak; s32/u32/s16/u16/s8/u8; for/while/
do; <, <=, !=, >; continue/else-break; pointer-walk; goto found; dead stores/holders in the break path; `while (i <
4 && !attached)`). u8/u16 `i` gives retail's break-block shape but loses the pointer IV; pointer-walk source gives
the shape but CSEs the post-loop compare into the latch compare.

## func_8038CD14 (buildTrackMaps)

Lead only (func_8038CD14/README.md). In the inlined buildRaceTrackMaps, retail keeps "dead" stores of both
allocation sizes (`sw a2,300(sp)`, `sw v1,296(sp)`) and computes `size + 7` from the register. A local ARRAY
`s32 size[2]` (vtx = size[1] @300, dl = size[0] @296; dl as `(n * 5 + 7) * sizeof(Gfx)`) reproduces it: uopt keeps
stores to constant-indexed local array elements (memory variables) but can still colour them (dl -> v1, exact).
volatile / address-taken / struct forms do not (struct is scalarised). Remaining: the vtx element is not a register
candidate here (retail: a2, pushing trackRomObjDef to a3), end-of-DL `_g` pointer in a0, constant-72 web split
(multu vs shift sequences swap places), temp-ring start. All 12 claims still MATCH with this CD14 (vrun c4).

## Identities proven / refined

- D_803B83C8 (sBlit) is MAT3's FUNCTION-LOCAL `static _tBlit *sBlit` of updateWarningScreen (code-proven: only the
  local-static form gives retail's done-block reload; extern gives a PRE'd load). D_803B839C = gPakScr (file static).
- Image A .data holds trackscr's function-local statics in MAT3 source order: B3EC cityPos 0x803B8338 / mapPos
  0x803B8344 / rotAngle 0x803B8350, buildRaceTrackMaps mapPos x2 0x803B8358/0x803B8364, buildStuntTrackMaps
  0x803B8370, buildBattleTrackMaps 0x803B837C, animTrackMaps mapPos 0x803B8388; warningscr's statics follow
  (0x803B839C..0x803B83C8) though warningscr's code is first in .text.

## What generalises

- **as1 shares `lui at` between offsets of a unit-defined symbol only when the %hi16 addends of their SECTION offsets
  are equal.** A residual of exactly one extra `lui at` between consecutive offsets of a static = a %hi carry in
  the original object's section offsets; a per-TU group cannot reproduce it (A820). Test: prepend `.space N` to
  the ugen listing (tools/padcheck.sh).
- **MAT3 function-local statics also fix branch layout**: an extern lets uopt PRE a load into a goto edge, making
  `if (..) goto L;` a two-branch `beqz; b L` instead of one `bnez L` (A400).
- **Constant-indexed local array elements keep their stores** while being coloured: the source form for "dead"
  stores of a value that is used from a register (CD14 sizes).
- **Positional "words differ" hides wins when a function changes length**: A820's continue form went 61 -> 249
  positional but 15 -> 11 aligned rows (w36e had rejected it). Use an aligned diff (tools/vscore.py prints both).
- Without umerge inlining a procedure's Udef is NOT rounded to 8 (hand-inlined body: 100/104/108); with umerge
  inlining it always is (UpdateSelectCar frame law).

## Notes / permission denials

None. Builder scratch: watchman2:~/rush2049/scratch/frontier/w36x2 (bin/ = trace toolkit reused from wtk;
astest/ = as1 lui-sharing experiments).
