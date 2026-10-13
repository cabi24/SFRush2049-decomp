# w28d — model/resource parser units, camera, attract, race end (wave 28)

Status: final. Branch wave28 (main checkout), nothing committed/spliced. Builder scratch
watchman2:~/rush2049/scratch/frontier/w28d (toolkit reused from wtk) deleted at the end. No permission denials.
Flags everywhere: `-g0 -O3 -mips2 -G 0 -non_shared` (IDO 5.3). Unit = `blob_unit --tag w28d` via `us.sh`.

## Table

| Function | Bytes | Before | After | State | Residual class |
|---|---:|---|---|---|---|
| **attract_or_transition** | 2,076 | B136 492/519 (rows 669) | **MATCH** | `cloud/matches/attract_or_transition.c` | — |
| func_800E56F8 (IPA s4) | 1,436 | ipa-groups 115/359 (ops 2) | 4/359, rows 3 (1 disclosed device) | best.c, PROVISIONAL until E6AF8 | as1 placement of one `lui` |
| func_800E6AF8 | 1,336 | 173 rows WITH pad arrays | 117/334, rows 106, no pads, frame 128/160 | best.c (same file) | 7 unknown named-local words + schedule |
| attract_mode_handler (IPA s5,s6,s7) | 264 | not started | **EQUAL with stand-in caller** | best.c, PROVISIONAL | needs caller control_settings |
| attract_video_handler (IPA s5,s6,s7,+s8) | 692 | not started | 18/173, rows 17, frame 48/48 | best.c, provisional draft | inlined-return copy (`move v0,s0`) |
| attract_demo_handler (IPA s5–s8 + stack) | 820 | not started | rows 35, ops 5, frame 56/56 | best.c, provisional draft | parameter colours s5/s6 |
| race_finish | 1,000 | not drafted | 186/250, rows 100, ops 26, frame 280/280 | best.c | colour: const-1 web vs `i` for s8 |
| func_800E543C | 700 | B93 162/175, rows 223, frame 56 | 152/175, rows 180, ops 24, frame 72/72 (3 devices) | best.c | index reg t0 vs a2 |
| func_80096CA8 (IPA s8,t0) | 1,204 | w27e 265 rows (c1), frame 128 | 289/301, rows 134, ops 41, frame 112/112 | best.c | IPA param colour (idx vs first) |
| func_80097164 | 344 | w27e 7 rows | unchanged (blocked on CA8's convention) | w27e piece | — |
| func_800EB028 | 1,640 | w27b 175 rows | lever found (163 rows) but costs 8 B frame; best stays w27b's | w27b/func_800EB028/best.c | register/schedule |
| lap_complete | 1,232 | w27b 111 rows (108 re-measured) | no gain (pair_set inline does not move its frame) | w27b best.c | frame 176/184 |
| props_render | 2,180 | not drafted | not attempted (needs func_800CBF2C's register contract) | — | — |

## attract_or_transition — MATCH (2,076 B), no devices

```
ssh watchman2 '... python3 tools/cloud/score.py fn cand_aot.c attract_or_transition --flags "-g0 -O3 -mips2 -G 0 -non_shared"'
attract_or_transition:
  MATCH
python3 -m tools.conveyor.pipeline.blob_unit --tag w28d score attract_or_transition --with cloud/matches/attract_or_transition.c --neighbours
  EQUAL attract_or_transition: 519 words (kept, c_attract_or_transition.c)
  locked bodies that differ in this unit: 0
blob_unit score: 1/1 equal
```
The per-frame display-list prologue (frame record D_80156BE0[D_8015F72D], command buffers, segment, two static DLs,
depth image, FILL-cycle depth/colour clears, split-screen divider bars from wheel_render_full(0), texture filter and
colour dither options, VI dither-filter/divot). Starting from near_miss_B136's command list, two levers closed it:
1. **SDK GBI macros** (gSPSegment, gSPDisplayList, gDPSetDepthImage, gDPPipeSync, gDPSetCycleType, gDPSetColorImage,
   gDPSetFillColor, gDPFillRectangle, gSPSetGeometryMode, gDPSetTextureFilter, gDPSetColorDither) with the
   `Gfx *_g = (Gfx *)(pkt);` block form copied from the matched sibling func_800F7448 (whose local GBI definitions I
   extended with the F3DEX2 forms). B136's hand-written `COMMAND()` do-while macros were 669 rows off.
2. **`extern volatile s8 D_8015F72D`** (as in func_800F7448): retail forms the index address `addiu; lb 0(reg)`.
Integration: plain single (splice_singles). No unit_overrides, no data-symbol changes. Not draft_checked (not needed).

## func_800E56F8 + func_800E6AF8 — IPA unit (`func_800E56F8/best.c`)

Command: `blob_unit --tag w28d score func_800E56F8 func_800E6AF8 --with w28d/func_800E56F8/best.c --internal func_800E56F8 --neighbours`
```
  FAIL func_800E56F8: 4 of 359 words differ        (rows words 3, ops 2)
  FAIL func_800E6AF8: 117 of 334 words differ      (rows words 106, ops 62, frame 160/128)
  locked bodies that differ in this unit: 0
```
- Start: cloud/work/ipa-groups/func_800E56F8/group.c. It still carried stale copies of func_800E4B58/E398C/E451C/E4300,
  which are locked now: with them `locked bodies that differ: 4`. Removed.
- E56F8 levers: the state byte 0x26D written through an s8 lvalue (`*(s8 *)&...->pad0EC[0x26D] = 2`) — a u8 store
  made its own constant web; with s8 the 2 joins the s32 web retail hoists to `li s6,2` (115 → 23 words); the slot byte
  for D_801440AD read into a named local (`temp_a0`, retail v0) (→ 46 alone, 5 with the next); `var_a0 +=
  (s16field * 100000) / 800;` then the clear (temp_lo form kept the load in a coloured web) → 4 rows.
- **Device (disclosed):** `if ((temp_v0 ^ 0) == 1)` — no-code identity (w16a) puts the constant first (`bnel s5,v0`);
  without it 4 rows (`g10`).
- Last 2 rows: retail as1 hoists `lui t7,%hi(D_8013FECB)` into the `lb` delay before `bnezl` (fall-through block's
  first instruction); ours has `lh t8` first. Tried arm swap, one-line layout, s8-typed global: no change.
- E6AF8: `extern volatile` on D_801525F0 (render-thread flag; render_thread_entry tests it) and D_80153FD2 (173 → 136),
  typed `D_80120E74[D_80153E84].v[D_80153F24]` (shift-form *6, no reload after the store; → 104), pad arrays removed and
  `... i, j, r, t` declared last so their homes have retail's relative order (retail 124/120/116/112, ours 96/92/88/84):
  retail has 7 more words of named locals before `i` (unknown; no arcade/GC counterpart found) → frame 128 vs 160.
- draft_check: E56F8 alone (`--params ipa_s4=s4`): `VERDICT: EQUIVALENT over 190585 calls / 6 scripts`. Unit mode
  (`--unit func_800E56F8,func_800E6AF8 dcunit.c --params func_800E56F8:ipa_s4=s4`): `VERDICT: EQUIVALENT over 68802
  calls / 6 scripts`. E6AF8 alone with `--callee 'void func_800E56F8(s32 x)=s4'` crashes the host (-11): E56F8 clobbers
  s0–s3,s5,s6 unsaved; use --unit.
- Integration when E6AF8 matches: group {func_800E56F8, func_800E6AF8}, force_internal func_800E56F8 (already internal
  in the unit), E6AF8 kept. func_800E543C (callee) stays separate.

## attract_mode / video / demo handlers (labels WRONG; callers in control_settings, overlay-A only)

GC correlation has no candidates; no arcade counterpart. They draw one menu cell `D_80153FD8[row][col]` (44-byte
cells, 2 per row; s16 x/y/w/h at +24..30) under the D_801461D0 font lock, centring text with camera_auto_follow
(x + w/2, y + h/2, w, h, -1, 0, text), pair D_80118E20 {1,1} … {0,3}. regmap_verify --derive: mode (s5,s6; s7 read
without a definition = third parameter hypothesis), video (s5,s6,s7 set in order s6,s7,s5; s8 read as the text
pointer), demo (s6,s7,s5,s8 set in that order; stack parameters at 64..76 = o32 positions 2..5). All three are
`NEVER CALLED` by every script (draft_check on mode_handler).
- **attract_mode_handler: `EQUAL attract_mode_handler: 66 words (internal, c_a4.c)`** with `--internal
  attract_mode_handler --keep __standin_attract_mode_handler` (stand-in caller appended in best.c) = PROVISIONAL.
  Lever: font_set as ONE inlined level with osRecvMesg/osSendMesg written directly (the nested gfx_lock/gfx_unlock
  form moves `move s0,v0` into the osSendMesg delay slot: 3 rows).
- attract_video_handler: 17 rows, frame 48/48. Three font_set sites need the nested form (single-level is not
  inlined at 3 sites). Lever: the x position and the font height reuse the dead parameters (text → s8, col → s7);
  retail has only two named homes (c, y at 40). Left: retail's first inlined font_set return copy `move v0,s0` and
  the cell pointer built straight into s0.
- attract_demo_handler: 35 rows, ops 5, frame 56/56; x reuses `row`; y declared before fh (homes 48/44). Left: ours
  puts row/x in s5 (retail s6, sel1 s5) — prototype order does not move it; retail shares `li a2,1` between the pair
  stores and osRecvMesg.
- Integration: all three need the real caller control_settings (2,552 B, unmatched, many deps) in a group;
  `text` typed s32 in video_handler's draft (it is reused for x).

## race_finish — first draft (`race_finish/best.c`)
Per-player race position "P/N" HUD (`place + 1`, '/', car count), shadow pass at (x+1, y+1), place digit in player
colour D_80116168[i]. `186 of 250 words differ | words 100 ops 26 norm 26 | frame 280/280`.
draft_check: `VERDICT: EQUIVALENT over 17029 calls / 6 scripts`.
Levers: six inlined font_set sites + the locked func_800B669C body as inlined `pair_set` size the frame exactly;
y is an s32 whose `(s16)` conversions are spill temps (72/76); the glyph loop in index form.
Residual (ctrace): the int-1 constant web (save 14.07) takes s8 and `i` (11.8) splits to memory; retail has i in s8.
`if (i) {}` in the inner loop (device) gives ops 13 but words 113 — not a strict improvement. `continue` shapes: no gain.

## func_800E543C (`func_800E543C/best.c`)
`152 of 175 words differ | words 180 ops 24 norm 24 | frame 72/72` (B93: 223/30, frame 72/56).
draft_check: `VERDICT: EQUIVALENT over 122 calls / 6 scripts`.
Levers: expression form (retail spills model/vehicle/config to spill-temp slots 28/32/36 → coloured expression webs,
not named locals); camera stores through a 4-site inlined static `set_cam(v, cam)` (frame 72 exactly; hypothesis for
the caller-less stub func_800E5434); `extern volatile s16 D_801525F0`.
**Devices (disclosed):** three compiled-out `do {} while (0);` raise the callee-saved cost (7.55 → above the 8.3
caller cost) so &D_8014A250[index] stays out of s0 as in retail (without them 211 rows, s0 used).
Left: index in a2 (retail t0, spilled to its home 78(sp) across func_800C4F68/stunt_combo_display, i.e. retail keeps
`index` live past those calls — use not identified).

## func_80096CA8 (`func_80096CA8/best.c`, unit head + draft) — GC `_SetupWaveModel`
`289 of 301 words differ | words 134 ops 41 norm 41 | frame 112/112` (w27e c1: 265 rows, frame 128).
GC-derived (recorded for name reconciliation, all from GC `_SetupWaveModel`/`MBSetupWad`, waves.c / mb_wad.c):
- names: `_SetupWaveModel(idx, first)` = func_80096CA8; `_ReadWave` = func_80097164 (weak, call anchor);
  `MBSetupWad` = func_80096C28, `MBOffsetWad` = func_80096BBC, `MBGetFromWad` = lookup_with_output (call-order
  correspondence, not scored by the correlation); gWaveInstance = D_80156D38 (20-byte records, +3 valid, +5 indirect,
  +6 waveNum, +0xC data ptr, +0x10 load offset); gRomObjDefPtrs = D_801161F4, gTexDefPtrs = D_80151AE8,
  gPalDefPtrs = D_80138670 ({ptr, count}); gTargetPaths = D_801392D0/D4 (waveNum 101..119 on N64, 0xE9..0xFB on GC).
- layout: the 12-byte Wad context {base, TOCentries, TOC} (`__anon_87656`) at sp+100; locals in GC order wad, WADptr,
  TXLDptr, TXLDcnt (sp+88), IMAGptr, offset (sp+80) reproduce every retail home; N64 RomObjDef = 0x58 bytes
  (name[16], radius, unused, lodCount @0x16, lod[4] of 16 bytes @0x18 with displayList @+8 and a second pointer @+12).
- **`offset` is uninitialised on the `first != 0` path, as in GC** — retail's `lw s5,80(sp)` is that read.
- Residual: IPA parameter colouring. Ours: first (save 17.9) takes s8, idx (0.84) splits; retail the reverse. Forcing
  first to split is declined; `volatile D_80140A04` and `!= 0` spellings are worse. draft_check (`--params
  idx=s8,first=t0`) DIVERGES only at the `sw t0,116(sp)` caller-home spill (same as w27e's c1: 1646/1647 words equal).
- func_80097164: GC `_ReadWave` (locals i, size, winst, waveNum, name[32], geoloaded) is a file loader, not the ROM
  DMA/LZSS/inflate path; it gives names (winst, waveNum = `type`) but not the three unused frame locals. Still 7 rows,
  blocked on CA8's convention.

## func_800EB028 (no new best.c; w27b's stands)
GC `update_camera` and arcade setcamview add `gCamPos[i] += pos_out[i]` loops; GC has locals i, j, pos_in, pos_out,
m, camera, rear, view. A block-scoped `f32 y = pos_out[1];` in case 0 reproduces retail's $f0 promotion (175 → 163 rows)
but adds a frame slot (96/104); the `pos_out[i] + cam->pos[i]` spelling and a loop do not help. Next: find a
function-level f32 retail already has (frame is exact at 96 without it) or the same promotion in case 1.

## lap_complete (no gain)
Inlining func_800B669C as `pair_set` and declaring a local before `text` do not change the frame (176 vs 184; retail
also saves one s-register fewer). race_finish's levers (index loop, s32 y) are the next thing to port.

## What generalises
- **Hand-written display-list macros are not equivalent to the SDK GBI block macros**: the `Gfx *_g = (Gfx *)(pkt);`
  form closed a 669-row function to MATCH. Use func_800F7448's local GBI definitions.
- **`extern volatile` explains retail's `lui; addiu; lX 0(reg)` address form** for single accesses of thread-shared
  flags (D_8015F72D, D_801525F0, D_80153FD2). Same lever in three functions.
- **Dead parameters are reused for new values** in IPA register-parameter functions (attract handlers): retail has
  fewer named homes than values, and the values sit in the parameters' s-registers.
- **One inlined font_set level when called once; nested lock/unlock helpers when called 3 times** (umerge only
  inlines the single-level form at one site).
- **A store through a u8 lvalue keeps its constant out of the s32 constant web** (E56F8 `li s6,2`).
- **GC types give frame layouts even for divergent code**: _SetupWaveModel's local list reproduced every home.

## Notes for the coordinator
- `cloud/matches/attract_or_transition.c` is a plain single; nothing else is installable.
- All provisional bodies' stand-in callers are appended to their best.c (`__standin_*`); not deliverables.
- draft_check needs `--unit` for E6AF8 (E56F8 clobbers s-registers unsaved).
