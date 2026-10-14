# w32c — save + ghost (wave 32)

Status: FINAL. Branch wave32 (main checkout, base d1c53ab2); nothing committed, spliced or pushed. No permission
denials. Builder scratch watchman2:~/rush2049/scratch/frontier/w32c (toolkit reused from wtk), deleted at the end
(one stray: the first `--claims` run wrote /tmp/w32c_claims.log on the builder; removed immediately).
Flags `-g0 -O3 -mips2 -G 0 -non_shared` (IDO 5.3); unit = `blob_unit --tag w32c`.

## Table

| Function (GC name) | Bytes | Before | After (unit) | Group compile | draft_check | State / residual |
|---|---:|---|---|---|---|---|
| func_800CC50C (CompressGhost) | 760 | 9/190 words, 12 rows | **EQUAL** 190 words | **MATCH** | n/a (match) | **member** |
| props_render (GhostStart) | 2,180 | never drafted | **EQUAL** 545 words | **MATCH** | n/a (match) | **member** |
| pak_delete_file_retry | 784 | 165/196, 98 rows | unchanged (best.c = w31c/w28b) | — | EQUIVALENT (w31c) | callee-saved colour; lead `lead_q3.c` |
| CreatePlayer (CreatePlayer) | 1,088 | 241/272, 142 rows | unchanged best; lead 256/272, **120 rows**, ops 43 | context | lead: **EQUIVALENT** 2 calls / 2 scripts | structure (i colour, name/constants) |

New coverage if the packet installs: 760 + 2,180 = **2,940 B**, 2 functions (game 1056 -> 1058).

## The packet: `groups/menu_save_options/` (group name `frontier_menu_save_options`)

Supersedes the locked `menu_save_options` in place (same name; all 22 current members kept).
- **members/claims (24)**: the 22 locked members (func_800CD058, func_800CCEFC, func_800CCCCC, menu_save_options,
  func_800A44E8, func_800A370C, draw_speedometer, func_800C7EC0, func_800C7EC8, draw_ui_element, func_800CBF2C,
  func_800C7300, func_800CD0FC, func_800A361C, func_800A1A3C, func_80094FC4, menu_item_value_get, menu_transition,
  AdjustSteer, func_800C7200, SaveServer, func_800CC040) + **func_800CC50C** + **props_render**.
- **context**: CreatePlayer, draw_number, format_string_parse, menu_back, audio_reverb_update, audio_buffer_sync,
  func_800950AC (func_800CC50C moved to members).
- **keep**: the locked list (already had func_800CC50C) + **props_render**.
- **files**: group.c, save.c, ctx_hash.c, ctx_slot.c, ctx_strncmp.c byte-identical to src/blob/groups/menu_save_options
  (checked with cmp); **ctx_newghost.c** changed; **props.c** new.
  - ctx_newghost.c: the m2c context body of func_800CC50C replaced by the CompressGhost source (end of file); the
    old-style declarations of its callees replaced by prototypes (func_800A47C0 `void *(void *, const void *, u32)`,
    sound_play_menu, car_angular_velocity_clamp, audio_task_complete, audio_buffer_sync, format_string_parse,
    audio_reverb_update(u32, s32)); `extern M2C_UNK D_80111754` -> `extern f32 D_80111754[]`; `struct GhostHeader;`
    forward declaration for the prototype; header comment line. New non-static internal helpers (not keep, names
    unused elsewhere in src/blob): `ghost_blocks_from_size` (GC PS_GetBlocks), `ghost_free_start` (GC
    DeallocateMemory(header->start)).
  - props.c: props_render + non-static internal helper `ReadGhostData` (GC save.c ReadGhostData).

Builder, `score.py group cand/menu_save_options --claims` (IDO 5.3 796ae99a): rc=0, every member MATCH incl.
`func_800CC50C: MATCH`, `props_render: MATCH`, SaveServer's three `own .data verified` lines as before, no NOT
VERIFIED line (log: `claims_score.log`). Context: CreatePlayer 246/272, menu_back 3/33 (unchanged), rest MATCH.

Whole-program unit (`unit_packet.log`; the two changed files `--with`, the other five are already the locked files):
```
blob_unit --tag w32c score <24 members> func_800950AC --with P/ctx_newghost.c --with P/props.c
  --internal ghost_free_start --internal ghost_blocks_from_size --internal ReadGhostData --neighbours
  EQUAL func_800C7200: 64 words (internal, c_ctx_newghost.c)
  EQUAL func_800CC50C: 190 words (kept, c_ctx_newghost.c)
  EQUAL props_render: 545 words (kept, c_props.c)
  (+ the 21 other members and func_800950AC EQUAL, status unchanged)
  locked bodies that differ in this unit: 0
blob_unit score: 25/25 equal
```

### Integration notes (coordinator / w32i)
1. `install_group.py cloud/work/frontier/w32c/groups/menu_save_options menu_save_options` (supersede in place). No
   `--revert-single`/`--revert-group`: props_render and func_800CC50C were never locked anywhere.
2. unit_overrides: none expected — props_render and func_800CC50C kept (both on keep, external callers); the three
   new helpers are non-registered and not on keep (internal by the -kp derivation, inlined, no retail stubs).
   func_800950AC stays a context-listed locked single on keep (wave-31 trap: `force_keep` only if the gate empties it).
3. Data: no own .data/.rodata in the two new members (props_render's 10.0/100.0f/0.75f are `lui/mtc1` immediates).
   Data symbols used by props_render are all inventory names (gameplay_mode, active_player_count, input_rec0,
   D_8011103C… — see props.c header); no symbols.json/blob.ld change expected beyond the splice.
4. After the splice re-check that the generated `asm/us/blob/symbols.json` keeps func_800CD0F4/func_800CD0FC stubs
   (unchanged here).

## func_800CC50C = GC save.c CompressGhost — 9 words -> EQUAL (named lever closed)
Lever 1, the first free's parameter load (as1 placement): as1 breaks ready-list ties on source line (as1t trace,
`Picking node`): ours had the helper (lock on line 61) BEFORE the function, so the lock's `lui/addiu/move/li` (line
61) beat `lw s2,76(s3)` (call line 89) and the load fell into osRecvMesg's delay slot. Defining the helper AFTER the
function (v/a1, 6 words) let the load win too early; retail needs the load ON THE LOCK'S LINE: a one-line helper
that reads the pointer itself, `void ghost_free_start(GhostHeader *h) { u8 *p = h->start; lock; free(p); unlock; }`,
defined after CompressGhost (v/a5: 4 words, ops 0, frame unchanged 104). Lever 2, `gh` in a3 (colour): assign
the named `gh = ghost->header` TWICE — before the track/backward/name copy and again after it for
playerId/finish/GetPtrSize (v/b1: EQUAL). In ctx_newghost.c the callee prototypes mattered: with the file's
old-style `void func_800A47C0();` maxsize took v0 (5 words); with memmove's real `void *` prototype EQUAL.
No unused locals, no compiled-out reads; disclosed: ghost_free_start's identity (GC calls DeallocateMemory(
header->start) directly) and its placement after the caller.

## props_render = GC ghost.c GhostStart — fresh draft -> EQUAL (draft path, `props_render/v/`)
GC transcription p0: 296 rows, body 572/545 -> `(s32)` size store (no u32 trunc sequence) p2: 21 ops -> D_801543CA /
D_80153FD2 `volatile` (retail `la; sh 0(reg)`, as the locked func_80108DA8/func_800EC190) p3: length exact, 8 ops ->
named locals `ghosth`, `ghost`, `bk`, `size` and the order g, i, j, np, car, n, … (retail's named slots n 160 /
np 168, frame 184) q0: 230 rows / ops 8 -> the car-settings source as `((CarCfg *)(data + 1860))[car].s` (ugen
evaluates the data chain before car*16; retail's temp ring) q4: 8 rows -> GC ReadGhostData as an inlined helper
(its parameter is retail's v0 copy before `move s1,v0`; removes the `ghosth` local, frame unchanged) r1: 5 rows ->
the tail as `D_80153FD2 = active_player_count = np;` t2: **EQUAL**. No devices, no unused locals.
N64 vs GC: n = D_80156994 ? 4 : 2 slots (GC 4); best time from the player data TrackRec table (+0xE0, 96 B entries,
+6 when backwards); gLink carries three more header bytes; 12 car-setting bytes from data + 0x744 + car*16.

## pak_delete_file_retry — one lane-hour, no strict improvement
Re-traced on w31c's p5 (no `pfs` local) and best (with it): 9 s-register candidates; ours colours the
&func_8008A6A4 callback (w41/w44, save 10, nocs 2, tot 20 > callee cost 10.5) and splits pfs (tot 10). A compiled-out
`if (pfs) {}` before `if (retry == 0)` (`lead_q3.c`) colours pfs (s7) and makes the body length exact (196) with
frame 232, but 105 rows (callback still s0, &D_8011194C loses its register); forcing the callback split on q3 gives
75 rows — the rest is an s-register permutation (retail: s0 &D_80144D60[port], s1 port, s2 node, s3 queue,
s4 controller, s5 file, s6 &D_8011EAE8, s7 &D_8011194C, s8 pfs; callback in t0 across func_800A1E94 (IPA preserves
t0) and spilled to 96(sp) across osSendMesg). Not a deliverable (device + more rows). Next: a source form that
lowers the callback's priority (a longer live range: e.g. the error-report call in an inlined helper taking the
callback as a parameter, or the constant assigned to a local before the retry loop), then force-oracle the
permutation. best.c = w31c's (unchanged), dc.args as w31c.

## CreatePlayer — one lane-hour, lead only
Retail facts (from the words): `ext` = sp+193 (= &file[9]) hoisted in s5 for the digit store while the test reads
`file[9+i]` through s8; the name loop's i and the digit loop's i are both s0 and `i = 0` sits at the attempt loop's
back edge (before memset); name in s7 walked by a0; '=' and '*' hoisted into a1/a2 only; a3 = attempt copy for
sprintf set before the name loop. GC: _GetUniqueExtension (stub 0x800CD0F4 by GC file order) is a different
algorithm on GC (strstr/strcmp over the file list), so the stub's N64 body is still unknown.
Lead `lead_f2.c` (EQUIVALENT): `char *ext` replacing `c` (named slot count unchanged; `ext = &file[9]` before the
attempt loop) gives retail's s5 store base and no strength reduction; `for (attempt = 0, i = 0; ; attempt++, i = 0)`
gives the back-edge init. Rows 142 -> 120, ops 44 -> 43, frame 224/224, but words 241 -> 256 and the body is one
word short — not strict. Remaining: i takes a3 (spilled around memset, cost 10 < s0 18.6) where retail has s0; name
spilled (228(sp)) instead of s7; '.'/':' hoisted into s5/s6. draft_check (`dc.args`, the func_800CCCCC s5 shim
with its clobbers — without them the standalone file faults after func_800CCCCC):
`EQUIVALENT` over 2 calls / 2 scripts (saves 1, player_finish 1). `diagnose one CreatePlayer` fails (the
workbench target still defines func_800CD104 — rename not propagated to that cache).

## GC names / layouts recorded (source function)
- CompressGhost (save.c): PS_GetBlocks = inlined `ghost_blocks_from_size`; DeallocateMemory(header->start) =
  inlined `ghost_free_start`; memmove = func_800A47C0 (returns void *).
- GhostStart (ghost.c) -> props_render; gRace = gameplay_mode; gInited = D_80114734; gRestartGame = D_80114650;
  gNumCars = D_801543CA; gNumPlayers = active_player_count; num_humans = D_80153FD2; gGhostPlayers split on N64 into
  D_801409F0[4] -> D_80140A18 (player records, 0x30: playerId 0x0C, name 0x14, datah 0x2C), D_80142500[4] ->
  D_80142530 (ghost headers 0x58), D_80140AF8[4] -> D_80140B28 (Ghost 0x2C); gGhosts = D_80152698; gPlayers =
  input_rec0 (0x4C: node 0, slot 1, carTrans 0x42, handle 0x48); gLink = D_80153E88 (8 B: track 0, car 1, three
  header bytes 2–4, np 5, 176, 6); model = D_8014A250 (0x808: frameTime 0x718, state 0x7CC); gTrackNum = D_8014978C;
  gBackwards = D_80152570; gRetraceTime = D_8002AFB8; gRetraceRate = D_8002AFB4; ghostRaceTimes = D_80111754;
  gCarHandling/Engines/Tires/Frame/Wings/TireRims/Horn = D_8011103C/D_80111080/D_8011119C/D_80111230/D_8011128C/
  D_8011157C/D_801115C0 (s8 [5][13]); gFrontTireScale/gRearTireScale = D_801112DC/D_801113E0 (f32 [5][13]);
  GetPlayerCarTrans = func_800CDDE8; AllocateMemoryHeap = audio_dma_sync; ReadGhostData (save.c) = inlined helper
  `ReadGhostData` -> func_800CBF2C(ghosth, 1, 1) (GC passes the same 1, 1).
- N64 ghost header (from GhostStart): car 0x08, carTrans 0x09, settings[12] 0x0A–0x15, bestTime 0x30,
  frameTime 0x34 (GC 0x30), start/offset/size 0x4C/0x50/0x54 (size s32).
- Proposed rename for the batch: props_render -> GhostStart, func_800CC50C -> CompressGhost (already pending).

## What generalises
- **as1 ties go to the lower source line, so a helper's DEFINITION LINE matters:** an inlined helper defined above
  its caller wins every tie against the caller's own code; defined below, it loses them. To put a parameter load
  between the helper's argument set-up instructions, the load must be on the helper's line: let the one-line
  helper read the value itself (`u8 *p = h->start;`).
- **A named local assigned twice** gives two webs whose colours follow retail when the single-assignment form lands
  in v1 (CompressGhost `gh`).
- **Old-style callee declarations change colouring** (a `void` vs `void *` return): use real prototypes in group
  context files when a body becomes a member.
- **GC wrapper functions inline as helpers:** GC `ReadGhostData(g) { return _ReadGhostData(g, 1, 1); }` is exactly
  the copy retail makes before an IPA call (`lw v0; beqz v0; move s1,v0`).
- **ugen evaluation order follows the expression shape:** `((T *)(base + K))[idx]` evaluates the base chain first,
  `base + K + idx*16` the index first — the whole temp ring after it shifts.
- draft_check of a standalone draft calling an IPA member needs that member's clobbers in `--callee …=regs/clobbers`.
