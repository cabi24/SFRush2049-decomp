# w41b RESULTS (wave 41, runtime overlay images A and B)

Branch wave37 at d8ce634e1. Builder scratch `watchman2:~/rush2049/scratch/frontier/w41b` (copied from `base`;
src/ovl_a and src/ovl_b sha256-identical to this checkout). Lane dir only; no git operations; no permission denials.
Coverage effect if the one packet is installed: image B 43 -> 44/49 (+8 B, the AnimateCase stub); image A unchanged.

## Table

| function | state | scorer (exact) | best file | tried this lane | next hypothesis |
|---|---|---|---|---|---|
| B func_8038FCD8 AnimateCase stub + B func_8038FCE0 ShotUpdate | **MATCH** (group packet, all 11 members) | `python3 tools/cloud/score.py group cand/groups__w36h_shots --targets asm/us/ovl_b --claims`: every member `MATCH`, context all `MATCH`, `exit=0` (`scores/w36h_shots.txt`) | `groups/w36h_shots/` | reproduced Astra's form; frame traced; 15 local-set variants (sh/v1..v5) | - |
| A func_8039842C updatePlayerScreen | NONMATCH 2/451 (installed form, unchanged); helper form 22/451 (frame 184) or 10/451 (with `q`) | installed group: `+0x5ec want afa2007c sw v0,124(sp) got afa20078 sw v0,120(sp)` / `+0x5f8 ... lw v0,124(sp) ... lw v0,120(sp)` `2/451 words differ` | installed group (unchanged); helper base `9842C/h_none` | ~680 variants in 11 rounds (b1-b11) + spill/pretrace/ctrace traces + allocator replay | one extra coloured expression web (see below) |
| B func_80391B00 UpdateHUDExtraPostCamera (+ stubs func_80391640/48) | NONMATCH: best0 407/595, holder0 83/595 (artificial `if (e) {}`), Astra 112/595 | `func_80391B00: 407/595` / `83/595` / `112/595 words differ` (`score.py group ... --targets asm/us/ovl_b`) | `91B00/holder0` (w38b/w36j), `91B00/astra` | reproduced all three; frame/home census; allocator replay; 18 islda-root variants (c1, no effect) | same class as 9842C: one more expression web between p and the (u8)alpha conversion |
| A func_803A44FC animText@carscr (+ stubs func_803A44EC/F4) | NONMATCH 2/917 (unchanged) | `score.py fn ... --targets asm/us/ovl_a`: `+0x094 want bne t6,at / got sll t7,s3,2`, `+0x098 want sll / got bne`, `2/917 words differ` | w36b/func_803A44FC/best.c | reproduced; read the ugen listing (A44FC/, snapshot st_a44) | w36x's as1 xbb analysis stands (header weight / bb6 dominance); no new lever |

## 1. ShotUpdate + AnimateCase (MATCH)

- Astra's dot_ovlb_20261022 result reproduced from `w38b/v/sh/k1beforec0` (`sh/a1`: all eleven members text MATCH).
- Frame cause (spill.sh on a multi-file staging, `tools/mstage.sh`): cfe locals area installed 88, helper as-is 96,
  retail 88. umerge's AnimateCase inline area (8 bytes + casing/next/idx rounded to 8) replaces the root's 12 bytes
  of casing/next/idx, so retail's ROOT has exactly 13 words of named locals: i (sp+204), StartStreak, streaktype
  (sp+196), seven 4-byte locals, then off[3] (sp+156..164) last. MAT3 has nine candidates for the seven (shot, h,
  hp, nexth, defense, car, m, sizep, z), so two are not separate N64 locals.
- Probes (`sh/v1`..`v5`): no `sizep` (expression form) 168 words; block-scoped sizep/z do not shrink the area;
  AnimateCase with MAT3's two locals (no idx) gets the frame but `lh a0,6(s0)` replaces retail's `lw; sll; sra;
  move` (729 positional); `defense` removed alone is code-identical (frame stays 216). Exact (0 words): (a) Astra:
  h/shot carry hp/nexth, defense kept; (b) CHOSEN: no `defense` local + the cleanup loop keeps its successor in the
  dead `shot`: one reuse instead of two. Rejected: reuse car/m (10), current node in shot (7) or h (88), successor
  in h (81).
- Disclosed shaping (shots.c header): (1) `defense` is not a local (code-identical direct field use); (2) hit-poly
  successor kept in the dead `shot` pointer; `z` and `sizep` function-scope before off[3]. No unused locals: frame
  arithmetic retail 208 = installed 208; the helper adds umerge's inline area, the two dropped locals return 8 bytes.

## 2. func_8039842C: what was established (no match)

Base = w38b helper form `9842C/h_none` (four MAT3 control helpers inlined, no unused locals, no `q`): `22/451`.
- **The frame counts only the spill homes actually used**: frame = round8(112 + A + 4 x (highest USED temp + 1)),
  112 = end of the register save area, A = cfe locals area. Helper form: A = 64 (root locals 36 B at -4..-36,
  four inlined calls x 8 B at -37..-64), used temps t0 = reset-loop pointer (web 169), t1 = &D_803BACE0[playerIdx]
  (web 53) -> 184. Retail: A = 64, reset pointer 124 = t0, web 53 at 116 = t2 -> 188 -> 192. Installed form:
  A = 68 (seven unused + q), t0 = 120, t1 = 116 -> 192. So retail needs NO extra frame; it needs web 53 one home
  higher (t2). All code words already equal retail.
- f_spilltemps is first-fit in bit order over block conflicts; `9842C/sim.py` replays it exactly from the
  spill.sh CONFL log (`9842C/spill_hnone.txt`). Search over the replay: retail = h_none + ONE extra coloured
  expression web X numbered before 53 that conflicts with 53 and with web 10 (gNumPlayers, a1) or web 0
  (gSkipToGameState load), and not with web 16 (first-loop &gPlayers[i]); equivalently 53 conflicting with 16, or
  the allReady LFTR bound web 114 numbered before 53. Renumbering existing webs alone never works.
- Tried, all frame 184 / >= 22 words: loop-index sharing in every loop (b1, 144), compiled-out holders in
  controlWait / loop head (b2, 343), locals in controlWait/controlControls (b3), natural block-boundary forms for the
  installed `do {} while (0);` (b4: `if (D_8014A108) {}`, `if (0) {}`, `if (D_8014A108 > 0) {}` are equivalent to it,
  others lose the a1 colouring), declaration/test forms (b5, 128), s8 helper parameters (b6), MAT3 file order (b7),
  block-local q / switch-value local / cast argument (b8), one function-scope i shared by the loops (b9), static and
  K&R helpers (b10), index spellings of the D_803BACE0 accesses (b11). Stopped per Discipline (>600 variants).

## 3. func_80391B00: frame census (no match)

- Retail stack census (`tools/spuse.py`): best0/holder0 put every named local where retail does (battle screenPos
  324/326, coinColor 300..303, stunt slot 244, stunt coinColor 208..211, name buffer sp+172). Astra's 112 variant
  removes tex/texIdx and adds MAT3's dead `e` assignments: its coinColor/name buffer sit 8 bytes high (216.., 180),
  so that is not retail's local set; the e-lines cost two extra spill temps that the removal hides.
- holder0 homes (spill.sh + sim.py, `91B00/spill_holder0.txt`, `91B00/pre_holder0.txt`): p (web 101,
  &gStuntPlayer[i], split) t1 = 144 = retail; the (u8)alpha conversion (web 119) t2 = 140, retail 136 = t3; the
  counts pointer (297) t6 = 124 = retail. Replay search: one extra expression web numbered between 101 and 119
  that conflicts with 119, 101 and a t0 occupant (9 gNumPlayers or 57) puts alpha at 136 with everything else
  fixed: the same "one missing expression web" class as 9842C (and, per w38b, 90DCC/A9088). The expressions of
  the alpha computation are numbered late (bits 323-331, created when p is copy-propagated), so any web built from
  them cannot be that X; islda-root spellings of p->lock/started/active/working change nothing (c1, 18 variants).

## 4. func_803A44FC

Reproduced 2/917 standalone. The ugen listing (`st_a44`) confirms w36x's picture: the header block is
`.noalias $2,$sp; lw $14,0($2); bne $14,1,$87` and bb6 starts with the inlined parameter store `sw $19,304($sp)`
followed by the &gCamera[p] precomputation whose `sll/addu` as1's xbb hoists. No new lever found; not reworked
beyond reproduction (w36b/w36x/f3: ~180 variants and an as1 weight trace).

## What generalises

1. **Frame = cfe area + homes actually used.** uopt's f_spilltemps may allocate more temp homes than ugen places in
   the frame; the frame is round8(save-area end + A + 4 x (highest used temp + 1)). A residual "home one slot
   off + frame 8 short" can need a different home order, not more temps (9842C).
2. **Replay the home allocator** (`9842C/sim.py`): f_spilltemps is first-fit in bit order over block conflicts and
   the spill.sh CONFL lines give the full lower-triangular conflict matrix; a short search tells exactly which extra
   web (position + conflicts) the retail layout needs before writing any C.
3. **Inlined helpers shrink the root's local set.** When a helper is inlined, umerge adds 8 bytes plus the helper's
   locals; to keep the retail frame, the root must lose the same bytes (ShotUpdate: 13 root words, two of MAT3's
   nine candidate pointers are not N64 locals). Check `spill.sh` AREA f_readnxtinst and the retail slot of the
   last array before adding helpers.
4. Expressions created by copy propagation (inline parameters, `p = &G[i]` pointer locals) get LATE itable
   numbers (9842C 193-198, 91B00 323-331); this decides home order and colouring ties.
5. Tools: `tools/mstage.sh` stages multi-file overlay groups for the trace toolkit; `tools/obatch2.py`/`ob2.sh`
   batch-score with frame and sp-offset remaps (`--gdir`/`--file` for multi-file groups); `tools/spuse.py` stack
   census retail vs ours; `tools/ownwin.py` content-checks every own-data window against the local image;
   `tools/reorder.py` reorders group functions.

## Integration note: `groups/w36h_shots/` (image B)

- Install: `python3 -m tools.conveyor.pipeline.ovl_splice group --targets asm/us/ovl_b --claims all
  cloud/work/frontier/w41b/groups/w36h_shots`, then `ovl_layout build`, `ovl_splice check`, `ovl_rom rom`.
- Supersedes the installed group `src/ovl_b/groups/w36h_shots` (same name). Members: the ten installed members
  (func_8038D200, D328, E088, D498, F938, DA78, FCE0, F560, F568, E114, all still MATCH) + func_8038FCD8 (new stub,
  not locked anywhere). claims = all eleven. context, keep, flags unchanged; adds `allow_unverified: true`.
  ctx_*.c byte-identical to the installed copies; only shots.c and group.json change
  (sha256 f3fb737c... / 72992bad...).
- Stub claimed: func_8038FCD8 (helper non-static, not on keep); object order F938, FCD8 STUB (8), FCE0 = retail.
- Own data: local `ownv.sh` gives 8/11 MATCH with own data verified; FCE0, F568, E114 `NOT VERIFIED` only by
  owndata's one-base rule (literals in two retail .rodata regions, 0x80394AFC and 0x80394CF4) - exactly the same
  three on the installed copy. `ownwin.py`: content_failures=0 for all eleven members (every window equals the
  image bytes). The builder's plain run exits 1 only on "no retail data available", as the installed copy does.
- Receipts/tests: no test or receipt reads production shots.c or group.json (grep of tests/ and cloud/work for the
  group name and file hashes). The five dot_runtime_b_* verification.json pin only the ctx_*.c hashes, unchanged.
  ovl_matched.lock.json entries are rewritten by the installer.
- Caveats: two disclosed shaping changes (see section 1); Astra's equally exact variant is in `sh/v4/v_a_astra.c`.
