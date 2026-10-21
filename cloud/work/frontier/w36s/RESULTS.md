# w36s RESULTS (wave 36: resurrect / tracks / targets)

Lane started 2026-10-20 on master e68aea6e1. Builder scratch: watchman2:~/rush2049/scratch/frontier/w36s

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| stunt_combo_display (0x800D3B28) | **Resurrect@resurrect.c (confirmed)** | 4700 | **group MATCH + unit EQUAL** | groups/w36s_resurrect/resurrect.c |
| func_800D348C | **FindHumanResPt@resurrect.c (confirmed)** | 1684 | **group MATCH + unit EQUAL** (internal) | groups/w36s_resurrect/resurrect.c |
| camera_follow_path (0x800C46D0) | FindSurfaceUvsFromResPoint@resurrect.c | 1484 | **group MATCH + unit EQUAL** (internal; Astra #389 body) | groups/w36s_resurrect/camera_pair.c |
| func_800C4F68 | SetResurrectionData@resurrect.c | 1416 | **group MATCH + unit EQUAL** (arcade leftover locals) | groups/w36s_resurrect/camera_pair.c |
| func_800D3B20 (stub) | FindBestQuat (static, inlined) | 8 | group MATCH (locked stub superseded) | groups/w36s_resurrect/resurrect.c |
| func_8010C974 | **_AnimateMine@targets.c (confirmed)** | 2636 | 167/659 words (frame 376/392); 151 with 4 filler slots; 125 rows after forcing j/index -> pure temp-ring rotation | func_8010C974/best.c |
| LoadTrack (0x800BB9B0) | LoadTrack@tracks.c (conf) | 2104 | re-scored only: 39/526 in the current unit (w31d best.c, unchanged residual) | cloud/work/frontier/w31d/LoadTrack/best.c |

### Group w36s_resurrect (9,284 B new: 4700 + 1684 + 1484 + 1416)
`cloud/work/frontier/w36s/groups/w36s_resurrect/` (group.json, resurrect.c, camera_pair.c, particle_context.c =
byte copy of src/blob/groups/frontier_particle_knot/group.c, func_8008B2E4.c = byte copy of src/blob/func_8008B2E4.c).
`score.py group cand/w36s_resurrect` (builder, no --claims): func_800D348C MATCH (own .rodata verified
0x80124180..0x80124184), func_800D3B20 MATCH, stunt_combo_display MATCH (own .rodata verified 0x80124184..0x801241A4),
camera_follow_path MATCH (own .rodata verified 0x80123F84..0x80123F88), func_800C4F68 MATCH; context all MATCH except
func_8009F058 22/1307 (same as in the installed frontier_particle_knot group). Full output: score_group.txt.
blob_unit (`--internal camera_follow_path --internal func_800D348C --internal func_800D3B20 --neighbours`):
5/5 EQUAL, "locked bodies that differ in this unit: 0" (blob_unit.txt).
Integration: new group; claims = the five members; keep stunt_combo_display, func_800C4F68 (+ the context keeps);
unit overrides needed: force_internal camera_follow_path, func_800D348C, func_800D3B20; the locked stub single
func_800D3B20 is superseded by the group's real body (prefer_definition).

## func_8010C974 (_AnimateMine) notes
Start: Astra #403 typed body (498/659 in the unit). Levers that worked (-> 167/659, 8 ops rows):
- one statement per physical line (Astra's packed lines gave as1 line ties: reversed white-colour byte stores,
  mat[1] add order); GC-style mat[0] `pos = pos + uvs * 0.75f` (fs = product) ; team colour written through
  `color_ptr = &color` (retail `addiu v1,sp,368` + sb 0/1/2(v1)); mat[1] adds A0 / A1+A2 (or all three) on one line;
- nested ZOID_RemoveObject -> ZOID_RemoveObjectTree(objnum, 0) -> entity_spawn_callback(objnum, 0, 0) (MAT3 zoid.c
  chain) adds 32 B of inline area (frame 344 -> 376); `int i, j, k;` puts the colour at retail's 368(sp) and
  opos/uvs at 352/316.
Remaining: (1) frame 376 vs 392 (16 B: unknown inline helper / locals; 4 filler slots close it, best_framefill.c,
151 words); (2) colour: j (copy/glow loop counter) and index want a1 (force p1:w62=c4,p1:w24=c4 fixes every
a0/a1 row; GC's single j for all three loops fixes the first loop but hoists the glow loop's j=0 above CopyUV);
(3) ugen temp ring: from the first texture lookup (&gTexDefPtrs[x>>10].texDefs[x&1023]) retail allocates
t7,t8,t9,t6,t5 where ours allocates t6,t5,t7,t8,t9 (same evaluation order; free-list order differs) -> t5/t7
rotation for the rest of the function (125 rows after forcing (2)). Source spellings of the lookup are
canonicalised (index-first / i[p] forms identical at site 1). Not closed.

## Key findings so far (Resurrect cluster)
- N64 Resurrect = MAT3 Resurrect + N64 additions: humans respawn at pole_pos_offset[6+slot] (row 6 in kGhost),
  WorldVector(offset) added to the respawn point; race-type tests are written as explicit == chains (no
  unsigned range tricks); `/ 2` (int) in the demo speed (a `/ 2.0f` becomes `* 0.5f` in IDO); res_last_time reset
  with int `0`; collide_state/collide_count as two statements (no chained reload).
- GC `finish:` label placement (before the offset zeroing) reproduces retail's duplicated reversed stores.
- Retail has an empty block on the !inPlace edge: `} else {}` after the in-place block (PRE inserts the uvs
  address temps there; +blocks make the callee-saved cost exceed t0's -> m stays in t0 with home spills).
- Frame (umerge/cfe law, mmt.py): the only deleted-static stub before Resurrect (func_800D3B20) is FindBestQuat;
  FindSurfaceUvsInPlace and FindDroneResPt are written inline (blocks). Retail anchors (d = 400 - sp):
  node 10, startPath..endIdx 44..56, jumpPts 60, time_scale 100, crash_time 104, vel[3] 116..108, human 124,
  sine 156 .. surf 184, drone y 224/z 228, cfe area 252 -> FindBestQuat area base 264 -> 280.
  => 24 extra declared slots (arcade-era leftovers, cf. SetResurrectionData's arcade xv/yv/zv/last_cp_index/dp/chkpnt).

## LoadTrack
Re-scored w31d/w30d best in the current unit (`blob_unit --tag w36s score LoadTrack --with
cloud/work/frontier/w31d/LoadTrack/best.c --internal w29_FindWorld --internal w29_NewWorld --internal w30_Clear`):
`FAIL LoadTrack: 39 of 526 words differ`. The prior draft is already a GC (MAT3 tracks.c/world.c) typed port; the MAT3
body adds nothing new for the residual (one code-free ugen ring allocation missing between the gNumWindows test and
the trackInited load: retail allocates t6 there, ours t9; the WorldBase store/reload addresses; li -1 scheduling).
Stop rule from w30d-w32d stands; not reattempted further (time went to the Resurrect cluster and _AnimateMine).

## Permission denials
None.

## Identities proven
- stunt_combo_display = Resurrect (resurrect.c): calls FindHumanResPt (func_800D348C), FindSurfaceUvsFromResPoint
  (camera_follow_path), TracePathPtForward (lap_count_select), NumPtsToCheckpoint (difficulty_select),
  FindClosestPathPt (func_800D2C10), PassedCP (race_countdown_display), CountdownTimeOut (viDeadlinePassed),
  MakeQuatFromUvs (func_800D14F4), CopyUV (math_utility), syminit (func_800D11BC), ExtractYaw (func_800C4180),
  RoadPosition (camera_trigger_check), fyaw (func_800C40E8), WorldVector (func_8009E820), SetNewPathIndex
  (func_800C4C9C); constants 146.66667, 1.1/0.8, 0.01/1.5, 58, pole_pos_offset = D_801210E8.
- func_800D3B20 (stub) = FindBestQuat (static, inlined into Resurrect) -- by frame layout.
- func_800C4CF8 = SetDroneResPathIndex, func_800C4078 = SyncMaxpathToLastCheckpoint, func_800D3430 = FindResPt,
  func_800D2FA8 = FindResPtRecurse, func_8009E820 = WorldVector, func_800D14F4 = MakeQuatFromUvs.
- func_8010C974 = _AnimateMine (targets.c): WPR_MINE callback (Astra #403); retail inlines MAT3's ZOID_RemoveObject ->
  ZOID_RemoveObjectTree(objnum, 0) -> _RemoveObjectTree = entity_spawn_callback(objnum, 0, 0) (frame evidence: +8 B per
  site), MBOX_NewObject = func_8008E26C wrapper, ZOID_SetTexture = func_8008D870, MBOX_HideObject = model_data_load,
  MBOX_ShowObject = model_transform_setup, ProximityDamage = func_8038D798 (image B), StartBlast = save_write_data.
- D_80117360 = res_last_time[4], D_80154390 = resurrectTime, D_801407B0 = gResurrectInPlace, D_80142760 = gDeathMode,
  D_80152734 = gNumLaps, D_80153E88 = gLink (8-byte N64 records, owner at +7), D_8014A96C = model[0].fastin.modelrun.

## What generalises
- **N64 retains arcade-era declarations as unused locals.** SetResurrectionData's exact frame needs the arcade
  set_resurrection_data leftovers (dp, xv, yv, zv, last_cp_index, chkpnt); Resurrect needs 24 such slots and
  FindHumanResPt 12. When a MAT3 port is frame-short by whole slots in a file with an arcade ancestor, read the
  arcade function's declaration list before hunting for helpers.
- **Deleted-static stub census + umerge law pins helper vs inline code**: only one stub before Resurrect and the
  cfe/inline-area arithmetic (base = align8(S + parm)) proved FindBestQuat was the static and FindDroneResPt /
  FindSurfaceUvsInPlace were written inline (y/z homes at 224/228 only fit that layout).
- **GC goto/label placement is reproducible**: `finish:` before the offset zeroing makes IDO duplicate the three
  stores (reversed) into both goto paths exactly as retail.
- **A dead/empty block can be the whole difference**: `} else {}` gives PRE an edge block (address temps placed
  there) and +blocks raise the callee-saved cost (m moves from s4 to t0 + home spills).
- **as1 line ties**: PRE-inserted remats take the last line of their block; a dead store after a call gives them a
  later line (Resurrect); separate-line vs packed statements decide byte-store and add order (both functions).
- **Local struct pointers decide coloured address webs**: `tCarData *car = &gGameCar[m->slot]` (block local) made
  retail's v1 address web and the drone path (t1) appear; plain `pathIndex` copies were propagated away.
- `x = x + y` vs `x += y` again decided FP add operand order (WorldVector offset add, mine ring offsets).
