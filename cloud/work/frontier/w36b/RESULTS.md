# w36b RESULTS (wave 36, image A: carscr rest + recordscr initScreen)

Lane dir `cloud/work/frontier/w36b/`, builder scratch `watchman2:~/rush2049/scratch/frontier/w36b` (from base, master
e68aea6e1; the carscr deliverable is rebased on the w36a revision installed by 2bc8a8558). Nothing outside this directory was
edited. No permission check was denied.

## Status table

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| func_803A44FC | animText@carscr.c (GC 0x800920EC) + inlined animButtonText (GC 0x800921BC) + empty drawOptions; CONFIRMED | 3668 | **2/917 words differ** standalone (as1 cross-block move, see residual) | func_803A44FC/best.c |
| func_803A0498 | setGhosts@carscr.c (GC 0x80090638); CONFIRMED | 2880 | **MATCH** in superseding group dot_cleanup_colortrace_20261011 (no own data) | groups/dot_cleanup_colortrace_20261011/ (body func_803A0498/best.c) |
| func_8039E444 | updateCarComponents@carscr.c (GC 0x800910F8); CONFIRMED | 2268 | **MATCH** in the same group (own .data/.rodata/jump tables checked vs image A) | groups/dot_cleanup_colortrace_20261011/ (body func_8039E444/best.c) |
| func_80393B3C | initRecordScreen@recordscr.c (GC 0x8008077C) + inlined initButtons@recordscr.c (GC 0x80084A5C); NOT carscr/playerscr | 1308 | **MATCH** in superseding group dot_runtime_a_callers_deep_20261011 (own .data/.rodata checked) | groups/dot_runtime_a_callers_deep_20261011/ (body func_80393B3C/best.c) |
| func_80393B34 | deleted procedure of the inlined recordscr initButtons (jr ra; nop) | 8 | **MATCH** (same group, claimed so the stub carries a symbol) | same |

Matched: 2880 + 2268 + 1308 + 8 = 6464 bytes (image A). Near-miss: 3668 bytes at 2 words.

## Scorer output (builder, IDO_DIR = toolkit 796ae99a...)

```
$ python3 tools/cloud/score.py group cand/dot_cleanup_colortrace_20261011 --targets asm/us/ovl_a --claims
Members:
func_803A0498:
  MATCH
func_8039E444:
  MATCH (24 section-relative relocations unverified: .data+0x5c at +0x4, .data+0x5c at +0xc, .data+0x34 at +0x2a0, ...)
Context (informational; excluded from exit status):  [all 12 old members: MATCH]
exit=0
(plain mode: all 14 members MATCH, exit=1 only because of unverified own data, as for the installed group)
$ python3 tools/cloud/score.py group cand/dot_runtime_a_callers_deep_20261011 --targets asm/us/ovl_a --claims
Members:
func_80393B3C:
  MATCH (16 section-relative relocations unverified: .data+0x0 at +0x28, .data+0x0 at +0x2c, .rodata+0x14 at +0x1cc, ...)
func_80393B34:
  MATCH
exit=0
(plain mode: the 6 old members + both new MATCH; context 8039338C 475/490, 803923B0 662/734 as installed)
$ python3 tools/cloud/score.py fn cand/func_803A44FC.c func_803A44FC --targets asm/us/ovl_a --flags "-g0 -O3 -mips2 -G 0 -non_shared"
    +0x094  want 15c10355 bne t6,at,0xd58               got 00137880 sll t7,s3,0x2
    +0x098  want 00137880 sll t7,s3,0x2                 got 15c10354 bne t6,at,0xd54
  2/917 words differ (18 section-relative relocations unverified: ...)
```
Full transcripts: score_carscr_plain.txt, score_carscr_claims.txt, score_recordscr_plain.txt, score_recordscr_claims.txt.
Overlay own data: the builder scorer prints "no retail data available" (expected); `tools/owncheck2.py` (decompresses nothing:
reads the private build/ovl/a/image.bin) compared every own-data reference of the claims with image A: E444 12 refs ok + jump
tables 0x803B962C (5) / 0x803B9640 (11) all entries equal; 93B3C 8 refs ok (white template 0x803B65A8, 7 floats
0x803B9454..0x803B946C); A44FC 8 refs ok + jump table 0x803B97C8 19/19; A0498 has none. The old members' own data still verifies
in the new group (FF04 15, D6A4 7, EF08 33, A0FD8 7, A1EAC 5 + table 19/19).

## Integration notes

1. `groups/dot_cleanup_colortrace_20261011/` supersedes the installed w36a revision (2bc8a8558): same name, keep
   [func_803A1EAC], the 12 old members byte-identical and MATCH, plus members/claims func_803A0498 and func_8039E444; context now
   empty; allow_unverified true. func_8039E444 DEFINES compLight `D_803B2858[2]` (80 bytes, image-A initialiser) besides its
   function-local lightVec template (0x803B303C).
2. `groups/dot_runtime_a_callers_deep_20261011/` supersedes the installed callers_deep group: the 6 old members unchanged and
   MATCH; members/claims func_80393B3C and func_80393B34; context 8039338C/923B0 unchanged (installed stand-ins).
   w36c and w36f also deliver revisions of this group (915E8/9338C, 923B0): my change is confined to the 93B3C block
   (function replaced, `extern volatile u8 D_80140BDC;` there, same edit as w36c). Checked: dropping my 93B3C block into w36c's
   revision gives all of w36c's members + 93B3C/93B34 MATCH. The stub MUST stay a named non-static function
   (w17j): as a `static initButtons` its deleted stub has no symbol and w36c's member func_8039338C fails with
   "1 extra words (nonzero beyond target length)".
3. func_803A44FC: not deliverable as a match (2 words). If integrated later it is a standalone-ABI function (scores with
   `score.py fn`), not a group member.

## Shared-declaration changes (for the merge with w36a / w36c / w36f)
- carscr group: `extern s32 string_copy_format(char *,s8,s8,s32);` (was `...,s8,s8,s8`): 4th parameter int, as MAT3
  MBOX_FindObject_WORK(const char *, signed char, signed char, int). E444's retail passes the shared constant-1 register
  unnarrowed; all 12 old members still MATCH (their 4th arguments are literals).
- carscr group: every new E444 declaration (CompRec/CompLight/ObjInstance/ZMatrix typedefs, the gCarComponents cast macro over
  D_803B2618, the compLight definition, D_803B2600/D_803B25EC/D_803B85CC/D_8012E708 externs, callee prototypes) sits just before
  func_8039E444, replacing w35b's stand-in preamble. func_803A0498 needs nothing new.
- recordscr group: `extern volatile u8 D_80140BDC;` (93B3C block); new non-static func_80393B34 defined with 93B3C.

## Identities proven
- A:func_803A0498 = setGhosts@carscr.c (GC 0x80090638): MATCH.
- A:func_8039E444 = updateCarComponents@carscr.c (GC 0x800910F8): MATCH.
- A:func_80393B3C = initRecordScreen@recordscr.c (GC 0x8008077C) with initButtons@recordscr.c (GC 0x80084A5C) inlined;
  A:func_80393B34 = that initButtons' deleted procedure: MATCH.
- A:func_803A44FC = animText@carscr.c (GC 0x800920EC) with animButtonText (GC 0x800921BC) and drawOptions inlined;
  A:func_803A44EC / A:func_803A44F4 = their deleted procedures: 2/917 (schedule only).
- Data: compLight D_803B2858 (2 x 40 B, Lights2), componentToName D_803B2600 (s16[11]), carComponentNames D_803B25EC,
  gCarPartFrame D_803B25C8, gCarComponents D_803B2618 ([4][11] x 52 B), ObjInstanceList D_8012E708 (68 B records),
  recordscr: gSelectMenuSelect D_803BA808, gStatPlayer D_803BA800, viewPane D_803B6530, blit D_803B652C, mblit D_803B6504,
  gDeleteConfirm/gDeleteNotify/gYes/gSelectMenuClear D_803BA84D/854/850/80C, inited D_803B6528; carscr animButtonText strings
  0x803B85F8 "GHOST %d", 0x803B8604 "TIME %01d:%02d.%03d", 0x803B8618 "GET READY TO RACE", msgPos .data 0x803B3048.

## Per function: what closed it

### func_803A0498 setGhosts (154/720 stand-in -> MATCH)
MAT3 port (153) -> player-first id compares (145) -> one `continue` guard in the best-branch limbo loop (32: uopt then inserts
the limboGhosts+3 reload only after the body's calls, the latch is no longer a critical-edge join) -> shift-loop bodies on their
own source line (0: as1 breaks scheduling ties by line; on one line the unrolled copies come out 0,1,2,3, retail 1,2,3,0).

### func_8039E444 updateCarComponents (556/567 stand-in -> MATCH)
1. N64 body from the words: compLight is Lights2 (40 B, light padded to 16), ambient col[0] flash (u8), dir bytes =
   (s8)(lightVec[k]*10) from lightVec itself (BodyVector's output vector is never read), no SetColor/printf.
2. Early-out as one condition `locked == 1 || option not in 1..5` (the MAT3 goto chain: i coloured, gOpts/gActiveOption row
   pointers spilled; one condition: retail's i-in-memory, s7/s8 rows) -- 538 -> 474.
3. string_copy_format 4th parameter int (474 -> 324, length exact).
4. Zero for case 0/1/10 spelled `0.f` (own $f30 web), YawUV literals `3.1415927f` / `3.14159265f` (324 -> 22).
5. compLight defined in the unit: the six dir-byte stores share one `lui at` only then (w15d rule).
6. Loop top order currentLevel, whichLight, compScale (as1 tie) and the named local `mask = 1 << p;` just before
   MBOX_ShowObject: the retail frame (144) has one 4-byte local beyond the MAT3 list (136 without; ctn/mask homes 4 low).

### func_80393B3C initRecordScreen (288/327 stand-in -> MATCH)
1. Identity from w36n/w36f lead + MAT3 recordscr; port with 55.2f (GC 59.2f) and no gSelectPakIdx.
2. The positions of records 17/21/22 are multiplied by a constant 1.0 (GC's "no-op copies"): `x *= (1.0f / 1.0f)`.
   Toy-proven (t/toy*.c): literal `1.0f` is deleted outright; a propagated `f32 scale = 1.0f` keeps the mul but stores the
   products through absolute `lui at` addresses (9 extra words); a cfe-folded constant expression (`1.0f/1.0f`, `(float)1.0`,
   `1.0f*1.0f`) keeps the mul AND the s0-based stores, as retail. (202 -> 3)
3. initRecordScreen defined before initButtons (MAT3 order): the &buttonColor address (caller line) wins as1's tie (3 -> 0).
4. Packaging: inlined helper non-static as func_80393B34 (see integration note 2).

### func_803A44FC animText (new port -> 2/917)
Levers: one `||` continue condition (shared latch copy), the ?: text pointer through `str`, `drawOptions(i)` in the else arm
(its inlined parameter slot is the last retail local word; frame 312 once the continue condition removed the extra spills),
`45.0f * gTextPosScale * 300.0f` order, `!D_803AF980 ? [225] : [224]`, `p != pIdx`, animText before animButtonText (line tie
for the inlined parameter's home store).
Residual (2 words): retail `lw t6; li at,1; bne t6,at; sll t7,s3,2` (load stall kept, sll in the delay slot); ours puts sll
in the stall and addu in the delay slot. Debug as1 (w12b build_as1dbg.sh, AS1DBG=8): the xbb pass processes the loop header
bb 5 (weight 1, stalls 2) and MOVEs two instructions of the inlined body's first block (bb 6) into it. Listing oracles
(tools/asmtry.sh): `.loc` values, labels, alias directives after the branch, the parameter store position -- no change; moving
the IV increment into the header block reproduces retail's delay slot, so the difference is in what as1 sees in the header.
Oracle that does reproduce retail's header (`lw; li at,1; bne; sll`): a predecessor-less directive block before the header
label (`b $32; .alias $21,$sp; $32:` in the listing, w12b mechanism: the header gets xbb weight 65534 and nothing is hoisted into
it) -- but that adds a `b`, so retail must give the header that weight some other way. Directives after the guard `ble` or
before the label without a `b` do nothing. Retail image A has 16 such kept-stall sites; in EF08's (matched by w36a) the load sits in the preceding block (PRE placed it
before the label), which hides the stall from the xbb pass. Next hypothesis: a source form that lets PRE place the
`buttonsInited[i]` load in the preheader/latch; or a structural feature giving bb 5 another weight. Tried without effect:
volatile buttonsInited, switch/continue forms, blank-line distances, splitting the call lines.

## Tools (tools/)
`rg.sh` (group score), `fn.sh` (standalone score), `dd.py` (aligned diff, groups or files, `--obj`), `mkg.py`/`ge.sh`/`gc.sh`
(compose groups), `owncheck2.py` + builder `gw3.py` (own .data/.rodata/jump-table check against image A/B), `rocheck.py`,
`odis.py`, `rd.py` (image words), `force.sh` (colour oracle wrapper), `asmtry.sh` (edited ugen listing -> diff), `dbg.sh`
(debug as1 xbb log), `toy.sh` (single-file IDO toy compile + objdump).

## What generalises
1. A constant that must survive as a multiply (x * 1.0) can be a cfe-folded constant EXPRESSION, not a literal: `1.0f` is
   deleted, a propagated variable changes the store addressing, `(1.0f / 1.0f)` keeps both mul and base-register stores.
2. Function definition ORDER is a lever for inlined helpers: the caller defined first (as in the MAT3 file) gives the
   inlined body higher line numbers than the call site, which decides as1 ties (A44FC, 93B3C).
3. A deleted inlined helper that is a separate procedure in retail (caller-less `jr ra; nop`) should be a named non-static
   function off the keep list, especially in groups with neighbouring members (w17j).
4. MAT3 prototypes fix argument narrowing: MBOX_FindObject_WORK's 4th parameter is int.
5. A file-scope static that the code stores to at several offsets (compLight) must be DEFINED in the group for the shared
   `lui at`; its initialiser comes from the image.
6. An early-out written as one condition vs a goto chain changes colouring priorities of the webs it touches.
