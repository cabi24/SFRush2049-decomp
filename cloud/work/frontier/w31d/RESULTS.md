# w31d — control settings group / LoadTrack / SetupWaveModel / func_800E543C / track_texture_load (wave 31)

Status: FINAL. Branch wave31 (main checkout, 880e729e); nothing committed, spliced or pushed. No permission denials.
Builder scratch watchman2:~/rush2049/scratch/frontier/w31d (toolkit reused from wtk) deleted at the end.
Flags `-g0 -O3 -mips2 -G 0 -non_shared` (IDO 5.3); unit = `blob_unit --tag w31d` (trace toolkit us.sh/ctrace/ugt/pretrace).

**Landable: track_texture_load (1,348 B)** = GC `_InitScreen@highscorescr.c`, via the superseding packet
`groups/frontier_billboard_render/` (unit EQUAL, `score.py group` MATCH with and without `--claims`; disclosed devices).

| Function | Bytes | Start | Now | draft_check | Residual class |
|---|---:|---|---|---|---|
| **track_texture_load** (GC _InitScreen) | 1,348 | never drafted | **group MATCH** (unit EQUAL 337 words, frame 120/120) | **EQUIVALENT 6 calls / 6 scripts** | — (devices: 1 empty inline helper, 1 unused local, 1 cast; 3 helper hypotheses) |
| control_settings | 2,552 | 629/638, rows 283, ops 135, frame 1872/1896 | 634/638 body, **rows 280, ops 82, frame 1896/1896** (3 holder devices) | **EQUIVALENT 90 calls / ghost_delete_fail** (fault FIXED) | font_set inline shape (3 sites lack `move s0,v0`), temp ring, case 10/5/7/8 colour |
| func_800DD0C0 | 908 | never drafted (asm) | **first draft: rows 22, ops 10, frame 440/440** (in the group TU) | not reachable (image A) | colour/ring |
| control_cell_text / text2 / text3 | 264/692/820 | EQUAL / 17 / 35 rows | unchanged; text **MATCH in the group compile** | text2 reached via control_settings (90 calls, unit draft) | as w28d |
| func_800DCD58 / func_800DCCE0 | 156/112 | locked members | EQUAL in the unit and MATCH in the group compile (verbatim) | — | — |
| LoadTrack | 2,104 | 39/526, rows 40 | unchanged (lever hour: 40+ variants) | w30d: EQUIVALENT 41 calls | 1 code-free ring pop + WorldBase address form |
| SetupWaveModel | 1,204 | 290/301, rows 233 | unchanged (k1: body length exact, rows 249) | w29d: EQUIVALENT 282 calls | colour (idx needs depth-2 weight) |
| func_80097164 | 344 | 7 rows (w27e) | **4 rows with the SetupWaveModel oracle** (3 = CA8 colours, 1 as1) | — | SetupWaveModel's convention |
| func_800E543C | 700 | rows 180 | unchanged (pretrace) | w28d: EQUIVALENT 122 calls | code-free index occurrence past the stunt call |
| props_render | 2,180 | not drafted | not drafted (time; analysis below) | — | — |

### Integration notes (exact)
1. `groups/frontier_billboard_render/` — supersedes the locked group IN PLACE: same name, all 6 old members, all 9 old
   files byte-identical (cmp vs src/blob/groups/frontier_billboard_render at 880e729e); members += track_texture_load;
   files += track_texture_load.c, ctx_vsync_a145.c (byte-identical copy of src/blob/groups/codex_vsync_a145/group.c);
   context += speed_set, speed_mode0_wrapper, speed_mode1_wrapper, continue_prompt, vsync_wait; keep unchanged;
   claims [track_texture_load]. unit_overrides force_internal += track_texture_load, AudKillSong_TTL,
   AudSetMusicVol_TTL, AudPlaySong_TTL, SaveFlush_TTL. No data-symbol changes (own .rodata 0.05f verified by score.py).
   NOTE for the integrator: w31c also edits groups of the save chain; this packet touches only frontier_billboard_render.
2. `groups/codex_control_settings_a4/` — PREPARED, NOT installable (control_settings/text2/text3/func_800DD0C0 not
   matching); members/context/overrides listed in its group.json "note" and below.
3. Nothing in cloud/matches (no standalone MATCH: track_texture_load is IPA-internal and needs speed_set's context).

## control_settings fault FIXED (2026-10-14 08:37)
Cause (traced): retail func_800DCD58 (locked, codex_control_settings_a4) writes s0/s1 WITHOUT saving them
(`lw s0,32(s1)` ...; IPA: retail control_settings keeps nothing in s0/s1 across it). Locked functions carry no
frontier `unsaved_callee_regs` facts, so draft_check called the retail body unshimmed and the w29c/w30d drafts
(compiled without DCD58's body, so IDO assumed o32 saves) kept the cell pointer in s0 across it -> s0 = C.buttons
= 0 -> `lb a0,1(s0)` fault. Fix: the group TU defines func_800DCD58/func_800DCCE0 (the locked bodies, verbatim;
both EQUAL in the unit) and func_800DD0C0 (C draft), as the packet must anyway.
`VERDICT: EQUIVALENT over 90 calls / 1 scripts (ghost_delete_fail 90)` (control_settings/v/dc_cs1.c = v/cs1.c), and again
on the final `control_settings/best.c` (dc.args `--scripts ghost_delete_fail`): same verdict, 90 calls.

## control_settings lever hour (done) — `control_settings/best.c` (= `groups/codex_control_settings_a4/group.c`)
Unit (`control_settings/run.sh FILE`): control_settings `want 638 words, got 634; differing rows 280 (words);
frame 1896/1896 | ops 82` (w30d: 629 body, rows 283, ops 135, frame 1872). control_cell_text EQUAL, text2 rows 17,
text3 rows 35, **func_800DD0C0 (first draft) rows 22, ops 10, frame 440/440**, func_800DCD58/func_800DCCE0 EQUAL,
`locked bodies that differ in this unit: 0`.
Steps: (1) func_800DD0C0 as C in the TU (IPA memory params row/col at the caller's 0(sp)/4(sp) come out naturally:
retail `sw s3,0(sp); sw s4,4(sp)`); (2) **frame: each umerge-inlined call costs 8 B of merged area in this frame**
(measured: nested font_set = font_set+gfx_lock+gfx_unlock = 24/site; lock/unlock-only macro 16/site; TB_CenterText
8; static vs non-static no difference; text2's 48-B frame shows none, so the area appears only in frames with a
local area). Retail 1896-1872 = 24 = 3 inlined calls: the first FONT_SET as a nested font_set_DC gives frame 1896
exact (any of the 4 sites: rows 292-293). (3) row needs +2.3 over the s3 cost 37.3 (ctrace p1dec web 8: totalsave
35 with one holder): a second compiled-out `if (row) {}` in the search loop -> row in s3, col in s4 as retail
(ops 127 -> 82, rows 280). DEVICES: 3 holders, disclosed (oracle-grade, not a claim).
Tried/measured: cross-TU `object_create` (HAL_SetFont) for FONT_SET: not inlined (calls stay, body 598);
single-level font_set1_DC at >1 site: not inlined (umerge size rule; inlined only at 1 site, as in control_cell_text);
parameterless fs11 nested: same 24 B/site; inlined `TB_CenterText_DC(num, C.w/2 + C.x)` (GC TB_CenterText, as
func_8010B9C8): ops 82 -> 72 (retail keeps the centre x in s0 across object_manager_update = helper parameter
evaluated before the call) but frame +8 (1904), rows 351.
Residual (traced): (a) three FONT_SET sites lack retail's `move s0,v0` (an inlined return copy), but four inlined
font_set sites cost >= 32 B and retail has only 24 B -> the shape is unresolved (contradiction noted: either a
cheaper inline form exists or the 24 B is TB_CenterText + 2 calls and the copies come from elsewhere);
(b) temp ring one register off in several cases (follows from the missing copies); (c) case 10's constant 1 in s0
(retail t0, s0 = &D_8017A4E0 there); (d) case 5/7 / case 8 statement order of the lang/strings loads.
Next: find a 4-site form whose return copy survives at <= 24 B total (test: does an inlined helper with an
unused *parameter* only cost 8 total when every site is in one basic-block chain?), then the natural row/col
loop weight (candidate: the original search loop indexes `D_80153FD8[row][col]` with a non-SR'd form).

### Packet `groups/codex_control_settings_a4/` (prepared, NOT installable: members not matching)
Supersedes the locked group in place: members func_800DCD58, func_800DCCE0 (old, bodies verbatim) +
control_settings, control_cell_text, control_cell_text2, control_cell_text3, func_800DD0C0; keep control_settings;
context slot_state_setup/object_create/func_800B669C via `ctx_gr3_b.c` (byte-identical copy of
credits_scroll_grp/gr3_b.c, the func_8010B9C8 precedent). `score.py group` on the builder (2026-10-14):
func_800DCD58 MATCH, func_800DCCE0 MATCH, control_cell_text **MATCH**, control_cell_text2 18/173,
control_cell_text3 198/205 (+2 words), func_800DD0C0 117/227, control_settings 535/638 (group compile; unit 616).
Unit install will need force_internal: control_cell_text, control_cell_text2, control_cell_text3, func_800DD0C0,
gfx_lock_DC, gfx_unlock_DC, font_set_DC, font_set1_DC, pair_set_DC. No data-symbol changes.

## LoadTrack (lever hour, no new best; `LoadTrack/best.c` = w30d best unchanged)
`FAIL LoadTrack: 39 of 526 words differ | words 40 ops 9 norm 9 | frame 168/168` (us.sh, UARGS
`--internal w29_FindWorld --internal w29_NewWorld --internal w30_Clear`).
ugt.sh (l0) confirms: ring allocations t6 (line `gNumWindows >= 2`), t7/t8 (second `D_80156994 || D_8014978C >= 6`),
then t9 at `if (D_80143A10.trackInited)` (retail t6): exactly one code-free ring allocation is missing in the
`gRace == 6 / == 4` + `other.{loaded,ready,inited} = 1` stretch (D_8014A110 is a coloured web in v0 in both; the
FREE events of v0/a0/t0/t1 do not touch the t6-t9 ring).
Tried (all 39/526 unless noted): `do {} while (0);` after EVERY statement from line 213 to 240 (28 variants; one
41); chained `loaded = ready = inited = 1` both orders (303, body 530); `trackInited != 0`, `(s8)`/`(s16)`/`(u8)`
casts on the test; `4 == D_8014A110`; `gNumWindows > 1`; store order loaded/inited/ready (41); `switch` (131);
`if (trackInited == 0) {...}` wrapping the tail (95); `i = D_8014A110` reuse (43; race web moves to v1, no pop);
`i = D_8014978C` reuse (44).
WorldBase lever: `(volatile u32 *)&` cast on the load or on the store only (non-volatile extern): unchanged 39; an
inlined getter/setter helper keeps the address coloured (a2) and adds 8/16 B frame. uopt still CSEs &WorldBase
into a register whenever store and reload are both present; retail's separate `lui at`/`lui a0` means the two
accesses are not one address web — untested: a second extern name for the same address (stand-in, not acceptable)
or the reload coming from a different TU's inlined accessor that umerge pulls in after uopt's CSE.
Next: SCHED=1 ugt.sh on a retail-shaped hypothesis for the gRace test (e.g. the compare value first loaded into a
named variable assigned through `op tmp; move var,tmp`, such as `race = race_get()` from an inlined locked getter).

## SetupWaveModel + func_80097164 unit (lever ~40 min, no new best; w29d `func_80096CA8/best.c` stands)
Unit (`--internal SetupWaveModel --internal func_80097164`): w29d best `290 of 301 words differ; compiled body is
294 words, target 301`, rows 233, frame 112/112. ctrace (sw0): first (w11, save 17.94, tot 323) -> s8 at cost 0;
idx param piece w203 save 16.67, **tot 100 vs bestcost 100 -> split (exact tie)**; idx whole web w3 tot 15 vs 110.
Tried: GC's in-loop HOODSHEEN/SHEEN/BRAKE range test `if (gWaveInstance[idx].type >= 64 && ... <= 76) {}` (empty
body) in the j loop (k1: body length EXACT 301, rows 249, ops 40; idx's param piece then colours s8 but the whole
web still splits and idx arrives in memory, first -> s7) and in the i loop (k2: rows 264); one `if (idx) {}` holder
in the i loop (k3: idx web 25 vs 110, split) or in the TXHD loop (k4: rows 311). Retail's idx weight needs ~3
depth-2 loop occurrences (w29d's 5 holders), so a single natural compiled-out test cannot supply it.
**func_80097164 unit with w29d's holders oracle as SetupWaveModel** (`SetupWaveModel/v/unit_oracle.c`, oracle,
NOT a deliverable): `FAIL func_80097164: 4 of 86 words differ` (rows 4: slot in s7 because the oracle swaps
idx/&wad, plus the known `lw s3` (rom) scheduling row); with the w29d best SetupWaveModel 10/86, rows 7.
`size` before `rom` is worse (9/86). So func_80097164 is done modulo SetupWaveModel's convention + 1 as1 row.
Next: find the source of idx's depth-2 weight (three occurrences per j iteration that PRE deletes): candidates
are the three `gRomObjDefPtrs[idx].ptr` loads in the j loop written so their address is not loop-invariant
before PRE (e.g. through `n`'s memory copy reloaded each iteration, as retail's `lw t6,68(sp)` bound shows).

## func_800E543C (~20 min, no new best; w28d `func_800E543C/best.c` stands: 152/175, rows 180, ops 24, frame 72/72)
pretrace (e0): index = bit 1 (varP +6), coloured a2 (save 2.0, nocs 5). Every post-stunt occurrence is PRE-deleted:
&D_8014A250[index] (bit 12) computed only in blocks 3/7 (DELETE 4,11-13,16-18,21,22,24,27-29), &player_array[index]
(bit 4) in 1/5 (DELETE 3,6,21,30), &D_80153E88[index] (bit 38) INSERTed in the three camera arms 11/16/17 (DELETE
25) - the same placement retail shows (t1/a3/v1 computed in the same arms, spilled to 36/32/28 around the calls).
So retail's extra `sh t0,78(sp)` at the stunt call is a code-free occurrence of index that survives to colouring
but emits nothing after the call (no reload): not one of the address expressions. A plain dead read is removed by
uopt (w29c/w30d). Next: a statement whose index operand is consumed by a code-free ugen pop after the call (e.g.
`index = index;`-like self-assignments through an inlined helper's parameter/return copy, w15d), tested with
pretrace for a surviving bit-1 antloc in the post-stunt blocks 21-30.

## track_texture_load = GC `_InitScreen@highscorescr.c` — **group MATCH** (NEW, never attempted before): READY TO INSTALL
First draft from the GC disassembly + retail (no arcade ancestor). `track_texture_load/best.c` = the member file
`groups/frontier_billboard_render/track_texture_load.c`.
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w31d score track_texture_load --with cloud/work/frontier/w31d/track_texture_load/best.c \
  --internal track_texture_load --internal AudKillSong_TTL --internal AudSetMusicVol_TTL --internal AudPlaySong_TTL --internal SaveFlush_TTL --neighbours
  EQUAL track_texture_load: 337 words (internal, c_best.c)
  locked bodies that differ in this unit: 0
blob_unit score: 1/1 equal
ssh watchman2 '... python3 tools/cloud/score.py group cand/frontier_billboard_render'   (no --claims)
  billboard_render / func_800F207C / func_800F1930 / PrintScores / SetHighScoreState / FindGhosts / track_texture_load: MATCH
  ... score.py group --claims: track_texture_load: MATCH (context informational, unchanged: speed_set MATCH)
```
Path (rows): t1 GC transcription 150 (frame 72) -> AudKillSong inlined as an in-file helper (= locked -O2
object_type1_create's body; retail inlines it, msg in s0) + speed_set flags (1, 0) 140 -> **inited/circuitCode
declared u8** (their `= 1` stores share one constant web, retail s3, apart from the s32 1 arguments; s8 shares
everything into one s4 web) 30 -> memcmp size parameter **u32** (size constants separate from the s32 multipliers
in s5-s8) 26 -> `(s8)` test of circuitCode (retail lb) 25 -> separate counter `i` for the ranks loop (retail a0;
IDO unrolls the 6-entry loop 2 + 4) 16 -> frame via inline levels (each inlined call reserves 8 B):
AudSetMusicVol (= locked speed_mode0_wrapper's body) and AudPlaySong wrappers, statement order
`stuffUnlocked = 0; gSavedGhost = 0;`, AudKillSong defined after the caller (as1 .loc order) and its two statements
on one line (`move s0,v0` after the `sb`) -> 2 rows (own .rodata reloc only) frame 112 -> one more 8-B inline
area + one named slot -> EQUAL. Calling the locked `speed_mode0_wrapper` directly is also unit-EQUAL, but in the
group compile as1 then puts the 0.05f `lwc1` in the jal delay slot (4/337; group flags `-Wab,-r4300_mul` or
`-Olimit 5000` do not change it), so the packet uses the byte-equivalent in-file wrapper.
DEVICES / hypotheses (all disclosed in the file header): SaveFlush_TTL(0) = an EMPTY inlined helper (identity
unknown; GC calls SaveServer(1,0); SaveFlush(0) at that point, the N64 has no code there), unused local `j` (exact
frame residual: retail 120, without it 112, one named slot), `(s8)` cast on the circuitCode test, helper identities
AudPlaySong_TTL / AudSetMusicVol_TTL (wrappers) and AudKillSong_TTL (= object_type1_create).
draft_check (t18 form = best.c with the locked speed_mode0_wrapper instead of the byte-identical in-file wrapper;
`--callee 'void SetHighScoreState(s32 state)=s2'`): `VERDICT: EQUIVALENT over 6 calls / 6 scripts (battle_end 1,
ghost_delete_fail 1, ghost_pak 1, ghost_resave 1, ghost_save 1, player_finish 1)`.

### Integration notes (exact) — `groups/frontier_billboard_render/`
Supersedes the locked group IN PLACE: same name, all 6 old members, all 9 old files byte-identical (checked with
cmp against src/blob/groups/frontier_billboard_render at wave31 880e729e); members += track_texture_load; files +=
track_texture_load.c, ctx_vsync_a145.c (= byte-identical copy of src/blob/groups/codex_vsync_a145/group.c, for
speed_set's IPA f20/f22/s1/s2 convention); context += speed_set, speed_mode0_wrapper, speed_mode1_wrapper,
continue_prompt, vsync_wait; keep unchanged ([billboard_render]); claims [track_texture_load]; provenance appended.
unit_overrides force_internal += track_texture_load (IPA-internal, only caller billboard_render), AudKillSong_TTL,
AudSetMusicVol_TTL, AudPlaySong_TTL, SaveFlush_TTL (non-static inlined helpers, not members, not kept, no stubs:
retail has no stubs around 0x800F5F90). Own .rodata: the 0.05f literal (retail 0x80124600 = 0x3D4CCCCD) verified
by score.py group (MATCH). No data-symbol changes.

## props_render (not drafted; lead for the next lane)
correlation.md top candidate GC `GhostStart` (ghost.c; fld x17 + memset/memcpy). Retail outline (props_render/v/
retail.s): `gNumSlots = D_80156994 ? 4 : 2` (sp+160); if gRace == 2 and !D_80114734 (gInited-like): pointer tables
D_801409F0[i] = &D_80140A18[i] (48-B stride) and D_80142500[i] = &D_80142530[i] (88-B stride) for i < slots (IDO
unroll 4), then gNumCars/gNumPlayers = 1 when D_80114650 (gRestartGame), the per-player memcpy of names/records from
gPlayers (D_8014A118 +72 chain), func_800CDDE8 (GetPlayerCarTrans), audio_dma_sync(0, n*12) (AllocateMemoryHeap),
the func_800CBF2C (= _ReadGhostData, now LOCKED in menu_save_options; IPA s0/s1/s3) loop, memset x2 + memcpy.
Recipe: draft from GC GhostStart with the N64 slot count; package with menu_save_options' func_800CBF2C as context.

## GC-derived names / layouts recorded this wave
- track_texture_load = GC highscorescr.c `_InitScreen` (full mapping in its file header): object_type1_create =
  AudKillSong, sync_entry_register = AudPlaySong, speed_mode0_wrapper = AudSetMusicVol(vol, rate) (speed_set(.., 1, 0)),
  track_lighting_setup = RecordGameOver, net_state_validate = CalcUnlockedStuff, func_800A1910 = memcmp (u32 size),
  func_800F45F8 = UpdateCircuit(u8 *ranks, f32 time), func_800F42C8 = GetCircuitCode, func_800F2888 = _RankScores,
  entity_audio_update = ClearRushControls, func_800F1114 = _UninitScreen; data: D_801148A4 = inited (u8),
  D_80143CE8 = circuitCode, D_801543D4 = gCircuitPlayer, D_80154628 = gCircuitTrack, D_80154640 = gCircuitSize,
  D_80142694 = gStuffUnlocked, D_80143F18 = gDrawName, D_80142528 = gThisTrack, D_801461A8 = gSavedGhost,
  D_80146108 = gOptions (+0xC music volume, +0xE song), D_801426A0/F0/50/80, D_80142AD8/D78/DC0 = namePanes+0x90..
  saved unlock tables, D_80150DD8/E30/E88/EB8/ED8/F00/F40 = gUnlockedTracks/Cars/Circuits/Trans/Engines/Tires/Frames,
  player_array = gGameCar (+0xEE place, +0xEF finished, +0xF0 time), D_8014A160 = gPlayers[i] + 0x4C (76-B stride).
- func_800DD0C0: the "big" (state_word_a & 0x7C03FFFE == 0) variant of the two-line pak dialog cell (msg[416/4]
  title, msg[412/4] vs [408/4] suffix, h - 40 vs h - 20), same option rows as control_cell_text2 with sel = cell.sel1.
- GC _SetupWaveModel's in-loop HOODSHEEN/SHEEN/BRAKE range test is `gWaveInstance[idx].type in [0x40, 0x4C]`.

## What generalises
- **A locked callee with unsaved callee-saved registers but no frontier facts breaks draft_check silently**: every
  locked function lacks `unsaved_callee_regs`, so a draft compiled without the callee's body keeps a value in a
  register the retail callee clobbers (control_settings' `lb a0,1(s0)` fault = func_800DCD58 writing s0/s1). Put the
  locked callee's verbatim body in the draft TU (it is a group member anyway).
- **Every umerge-inlined call reserves 8 B of merged frame area in a function that has a local area**, independent of
  the helper's params/locals (nested font_set = 3 calls = 24 B; void lock/unlock 8 each; static or not); frames
  without a local area (control_cell_text2, 48 B) show none. A frame residual in multiples of 8 below the named
  locals = count of inlined calls.
- **Constant-web sharing by store width signedness decided a 337-word function**: declaring two flag bytes u8 kept
  their `= 1` stores in one web apart from the s32 `1` arguments (rows 140 -> 30); a `u32 size` memcmp parameter
  separated the size constants from equal-valued s32 multipliers (30 -> 26).
- **A helper defined AFTER its caller changes as1's .loc tie-breaks** (osRecvMesg delay slot), and two helper
  statements on one line move the inlined return copy after the store (track_texture_load).
- **Unit-EQUAL with a cross-TU locked wrapper can still differ in the group compile** (as1 put a literal lwc1 in a
  jal delay slot; `-Wab,-r4300_mul`/`-Olimit 5000` group flags do not explain it): a byte-equivalent in-file copy of
  the wrapper matched both. Always run `score.py group` before claiming.
