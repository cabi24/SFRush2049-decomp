# w28b — save / controller-pak / track loading (wave 28, IDO 5.3)

Status: done. One installable group (pak write cluster, 7 members, 2,440 B, all MATCH in `score.py group`), three
complete non-matching drafts. Nothing committed, spliced or pushed. No permission denials. Builder scratch
watchman2:~/rush2049/scratch/frontier/w28b deleted at the end.

| Function | Bytes | Before -> after | Verdicts | Residual class |
|---|---:|---|---|---|
| pak write cluster (7 fns) | 2,440 | 5/7 EQUAL -> **7/7 EQUAL**, `score.py group` MATCH | n/a (match) | none |
| func_800A1BB4 | 184 | 9 rows -> **EQUAL** | | |
| func_800A1DD4 | 192 | 21 words -> **EQUAL** | | |
| AdjustSpeed (0x800A2680) | 784 | w2d 164/196 (pre-rename names; 125/196 + unresolved osPfsRename today) -> 166/196 words, **99 rows, frame 232/232** | no script reaches it (census) | colour: callee-saved priority (callback constant vs pfs) |
| func_800CD104 | 1,088 | 244/272 (156 rows, frame 184) -> 243/272 (163 rows, frame 216/224, named-local homes exact) | draft_check saves: DIVERGES, one extra own-stack word only (`--callee 'void func_800CCCCC(s32 p0)=s5/...'`) | frame (8 B) + colour/constant hoisting |
| func_800C813C | 900 | m2c context 222/225 -> complete GC-structured draft 211/225 (alone, callees o32) | race DIVERGES (one stack word, call #1); saves/attract_pak ERROR host -11 | blocked: IPA unit with func_800C7EC8 (s0) + draw_ui_element (t0-t2) |
| func_800CC040, func_800C7EC8, func_800BB9B0, track_texture_load | | not attempted (time went to the cluster) | | |

Pool: func_800CD104, func_800C813C, func_800CC040, AdjustSpeed (0x800A2680, pak), pak write cluster
(func_800A1DD4, func_800A1BB4), func_800C7EC8, func_800BB9B0, track_texture_load.

## 1. Controller-pak write cluster — 7/7 EQUAL in the unit, `score.py group` MATCH (installable group)

Group dir: `groups/pak_write_cluster/` (group.c = `pak_write_cluster/cluster.c`, group.json with claims = members).
Bytes: func_800A1BB4 184, func_800A1C6C 360, func_800A1DD4 192, track_data_decompress 652, track_collision_setup 468,
MaxPathZeroControls 396, slot_state_lookup 188 = **2,440 B**.

Unit (Pi, repo root), output verbatim (filtered lines are the other four EQUAL rows, unchanged from w27e):
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w28b score func_800A1BB4 func_800A1C6C func_800A1DD4 \
  track_data_decompress track_collision_setup MaxPathZeroControls slot_state_lookup \
  --with cloud/work/frontier/w28b/pak_write_cluster/cluster.c --internal func_800A1BB4 --internal func_800A1C6C \
  --internal func_800A1DD4 --internal track_data_decompress --internal pfs_queue_init --internal pfs_lock \
  --internal pfs_unlock --neighbours
  EQUAL func_800A1BB4: 46 words (internal, c_cluster.c)
  EQUAL func_800A1C6C: 90 words (internal, c_cluster.c)
  EQUAL func_800A1DD4: 48 words (internal, c_cluster.c)
  EQUAL track_data_decompress: 163 words (internal, c_cluster.c)
  EQUAL track_collision_setup: 117 words (kept, c_cluster.c)
  EQUAL MaxPathZeroControls: 99 words (kept, c_cluster.c)
  EQUAL slot_state_lookup: 47 words (kept, c_cluster.c)
  locked bodies that differ in this unit: 0
blob_unit score: 7/7 equal
```
Builder, `score.py group cand/pak_write_cluster` (no --claims): every member `MATCH` (func_800A1BB4, func_800A1C6C,
func_800A1DD4, track_data_decompress, track_collision_setup, MaxPathZeroControls, slot_state_lookup).

### func_800A1BB4 (9 rows -> EQUAL)
Traced cause: retail's two odd blocks (the early `jr ra; nop` and the found path's `beqz; nop; b exit; nop`) are
both blocks that ugen emitted with ONE instruction that as1 then deleted as dead. A non-empty early-return block ends
in `b exit`, which as1 tail-duplicates into `jr ra` (cf. locked Input_SetPadEnabledFlag); the empty one becomes a
conditional branch to the shared exit (ours). How to get such an instruction past uopt: a dead store whose variable
is "read" by a compiled-out holder at the exit. Final form:
```
if (D_80144030[index].modified == 0) { next = p = 0; goto out; }
next = p = D_80144D68[index].head;
while (p != 0) { data = p->file; next = data->next;
    if (data->data != 0) { if (D_80144030[index].slots[data->slot].modified != 0) { next = 0; break; } }
    p = next; }
if (p == 0) D_80144030[index].modified = 0;
out:
if (next) {}
```
Steps measured (rows): base 9; found block `{ next = p; break; }` + test of next 6 (jump-around appears);
pointer return of the record (`return &D_80144030[index]`, gives the early `jr ra`) 14 (v0 forbidden to the record
web: forbidden0=0x50000000, the value return); void + `goto out` + `next = 0` holder 9 (structure exact, p/next
colours swapped because next appeared first); `next = p = 0` in the early block 3 (address temp); `next = p = head`
order -> EQUAL (also EQUAL: `p = head; next = p;`). Rejected: s32/pointer returns (false-prototype risk, and v0 forbid).

### func_800A1DD4 (21 words -> EQUAL)
w27e showed retail needs both the `bytes >>= 5` temp pop (dest == operand) and a codeless web in a3. The a3 web is a
copy of the byte offset that nothing reads except a compiled-out holder after the loop:
```
end = address + bytes;
bytes = (u32)(address - *file->data);
first = bytes;          /* only read by the holder below */
bytes >>= 5;
last = (u32)(end - *file->data - 1) >> 5;
map = *file->data + file->data_size;
for (i = bytes; i <= last; i++) ...
if (first) {}
```
uopt colours `first` (a3, live across the loop), as1 deletes its dead move. Also EQUAL: `first = bytes;` placed
before the reassignment (original length). A constant (`first = 0`) is propagated and does not work; `(u32)address`
does not either (16 words). The `(u32)end` copy breaks the frame.

### Integration notes (coordinator)
- New group `pak_write_cluster` (no group superseded; no member was locked). Install with
  `install_group.py cloud/work/frontier/w28b/groups/pak_write_cluster pak_write_cluster`.
- unit_overrides `force_internal`: func_800A1BB4, func_800A1C6C, func_800A1DD4, track_data_decompress (IPA
  register/stack conventions: A1DD4 = a0,a1,a2,t3; A1C6C = handle a0, address t2, bytes a1, offset t1; track_data_decompress
  handle a2, mode in the caller's 4(sp), s0-s8 unsaved). pfs_queue_init/pfs_lock/pfs_unlock are non-static helpers,
  not members, not keep (umerge inlines them; no stubs in retail).
- keep: track_collision_setup, MaxPathZeroControls, slot_state_lookup. MaxPathZeroControls was provisional
  (w15f): it is now real with its real body, and func_800CC040/func_800CD104 (its callers) remain unmatched.
- `volatile` D_80144030 is local to group.c; no data-symbol changes.
- Devices disclosed in the group.c header: volatile pak records, slot_state_lookup's flag2 test, empty range check
  in A1DD4, the A1BB4 dead stores + `goto out` + holder, the A1DD4 holder web.

## 2. AdjustSpeed (0x800A2680) — draft `AdjustSpeed/best.c`, 99 rows, frame exact

Not a speed function: a controller-pak delete with retry (calls osPfsDeleteFile 0x8000A700, osPfsFreeBlocks). No GC
counterpart in the correlation (the GC pak layer is PS_*/paksysdc.c, a different platform layer); the libultra
osPfs* API was the anchor. Started from w2d best.c with the wave-27 renames (osPfsRename -> osPfsDeleteFile,
osJamMesg -> osSendMesg). Changes that mattered: the lazy-init lock is the locked func_8008A704 (inlined), unlock
sync_release_video; func_800A2670 (osPfsFreeBlocks wrapper) and func_800A2678 (list move) are the two retail stubs
in front of it; the heap-locked buffer free must be a helper with NESTED lock/unlock helpers (pak_heap_lock/unlock,
all non-static `--internal`): that alone takes the frame from 216 to retail's 232.
```
blob_unit --tag w28b score AdjustSpeed --with cloud/work/frontier/w28b/AdjustSpeed/best.c --internal func_800A2670 \
  --internal func_800A2678 --internal pak_buffer_free --internal pak_heap_lock --internal pak_heap_unlock --neighbours
  FAIL AdjustSpeed: 166 of 196 words differ   locked bodies that differ in this unit: 0
  udiff: want 196 words, got 196; differing rows 99 (words); frame 232/232
```
Residual (ctrace): 10 callee-saved candidates for 9 registers. Ours colours the `&func_8008A6A4` callback constant
(p1 w44, save 10, tot 20 > callee cost 10.25) and spills pfs (retail: callback t0 -> 96(sp), pfs s8). Probes that did
not help: no pfs local (104 rows), pfs declared first (102), pfs laundered `(char *)(u32)` (pfs becomes a variable
but still spilled, 102), pfs from the global `D_80144030[port].pfs` (99), `goto` retry loop instead of `for(;;)`
(99; block weights unchanged). Next lever: something that makes the callback web single-block (tot 10 < 10.25), or a
second use of pfs; `force.sh` with pfs in s8 to confirm the rest is colour-only.

## 3. func_800CD104 = GC save.c CreatePlayer — draft `func_800CD104/best.c`

GC evidence (functions.json + `reference/gc/disasm/save.c/CreatePlayer.s`): `CreatePlayer(int pak, unsigned char
*name)` returns the player handle; locals player, before, p, data, fileh. The N64 retail homes 220/216/212/208/204
are exactly those five in GC order. Call-for-call: _InitPlayer = func_800CCCCC (IPA, player in s5), strcpy =
func_800A473C, _CalcChecksum = format_string_parse, memcpy x2, PS_WriteFile = slot_state_lookup (data, 0x810),
PS_FlushFile = MaxPathZeroControls(fileh, 0), PS_FlushPak = track_collision_setup(pak, 1), InsertLLE =
func_80091FBC into gPlayerFiles = D_8012E6D8 (ordered by strcmp > 0), _RemovePlayerFile/_MovePlayerFile =
audio_volume_pan/func_800949D4 stored at (*fileh)+8/+12, PS_CreateFile = track_process_main(pak, file, "P", 0x810,
0x3544, 'NRUE'). GC `_Pow(num, exp)` is inlined in N64 (num 10 in s1, exp 4-i, `while (exp--)`), and is
probably the deleted static at the caller-less stub 0x800CD0FC. N64-only: the file name is "RUSH2049" + 6 filtered
name characters + a "%0*d" suffix built inline (GC: _GetUniqueExtension). The GC reference did not give the N64
name-filter or the frame of the inline area.
- Frame: declaring player, before, p, data, fileh, file[17], i, attempt, src, c, digits[32] puts file at 184 and
  digits at 136 as in retail (frame 216; retail 224: 8 B more spill/inline area at the bottom).
- Score: `FAIL func_800CD104: 243 of 272 words differ; compiled body is 265 words, target 272`, rows 163,
  frame 224/216 (`--internal func_800CD0FC`, 0 locked bodies differ). The research packet's body (r0) is 244 words /
  156 rows / frame 184 in today's unit; mine is not a strict row improvement, but its frame and homes are right.
- Left: retail hoists '=' (61) and '*' (42) into a1/a2 and `attempt` into a3 across the name loop, reads the name
  through a walking pointer, and keeps two copies of the char (v1 load, v0 compares): suggests an inlined
  valid-character helper with a parameter copy (a non-static `_ValidNameChar` helper was worse: 166-172 rows).
- draft_check (`dc.c` = the lean file, saves, `--callee 'void func_800CCCCC(s32 p0)=s5/s0,s1,s2,s3,s4,s6,f20,f22'`):
  `VERDICT: DIVERGES` — the only difference in call #1 is one stack word the draft writes in its own frame
  (`0x80031BEC gGameThreadStack+0x113C`, words written retail 131 / draft 132); everything else identical. Treat as
  frame artefact, not logic, but it is not EQUIVALENT.

## 4. func_800C813C = GC save.c SaveServer — draft `func_800C813C/best.c`

`save_settings` (matched) calls it as `func_800C813C(0, 0)`, the GC signature is `SaveServer(int forceUpdate, int
rumbleOnly)`; retail reads neither argument. Structure taken from GC SaveServer (PS_PakServer, sInitCountdown,
per-pak sReadingPak flag, "RUSH2049" prefix test, "P" extension test) plus the N64 one-pass player/ghost split
(player -> func_800C7EC8; ghost -> inlined _AlreadyReadGhost/_ReadGhostHeader with the 44-byte ghost entry sorted
by time into D_80152020). The m2c context body in codex_func_800C7200_2 is logically equivalent but unstructured.
Score alone: `FAIL func_800C813C: 211 of 225 words differ; compiled body is 214 words, target 225`, frame 72/160 —
meaningless until func_800C7EC8 (s0 param, clobbers s0/s3-s5) and draw_ui_element (t0-t2) exist as C in the unit.
draft_check with `--callee 's32 draw_ui_element(s32 p0, s32 p1, s32 p2)=t0,t1,t2' --callee 's32 func_800C7EC8(s32
p0)=s0/s3,s4,s5,s6,s7,f20,f22'`: race `DIVERGES` (call #1, one extra stack word 0x80031CD8), saves/attract_pak
`ERROR host exited -11`. Not diagnosed.

## What the GC reference did and did not buy
- Bought: function identities and names for the save code (CreatePlayer, SaveServer, _Pow, and the PS_* layer
  mapped onto N64 labels track_process_main/slot_state_lookup/MaxPathZeroControls/track_collision_setup/
  drone_set_catchup), the CreatePlayer local declaration order that fixes every named home, and the ghost-list
  semantics. Also confirms the pak write cluster's MaxPathZeroControls = PS_FlushFile and track_collision_setup =
  PS_FlushPak (GC CreatePlayer calls both in that order).
- Did not buy: anything for the pak write cluster's two matches (pure compiler tracing), AdjustSpeed (no GC
  counterpart: GC has no libultra pak layer), and the N64-only name filter / inline frame of CreatePlayer.

## Names recorded (GC function they came from)
CreatePlayer (func_800CD104), SaveServer (func_800C813C), _Pow (func_800CD0FC stub, inlined into CreatePlayer),
_InitPlayer (func_800CCCCC, from CreatePlayer's call), PS_CreateFile (track_process_main), PS_WriteFile
(slot_state_lookup), PS_FlushFile (MaxPathZeroControls), PS_FlushPak (track_collision_setup), PS_PakServer
(track_render_process), PS_ReadFile (drone_set_catchup), InsertLLE (func_80091FBC), _CalcChecksum
(format_string_parse), strcpy (func_800A473C), strcmp (func_8008AD04), strncmp (func_800950AC), gPlayerFiles
(D_8012E6D8), gPS_Files (D_80144D68), sInitCountdown (D_8011742C), sReadingPak (D_80117434),
_RemovePlayerFile/_MovePlayerFile (audio_volume_pan/func_800949D4). All from save.c except the PS_* layer
(GC ps2/ngc pak system), whose N64 bodies are the pak write cluster and its neighbours.

## Next levers
- AdjustSpeed: make the callback constant a one-block web or give pfs a second use; force-oracle first.
- func_800CD104: the valid-char helper shape (two copies of the char, '=' and '*' hoisted), 8 B of bottom frame.
- func_800C813C: needs func_800C7EC8 (628 B, IPA s0) and draw_ui_element (240 B, t0-t2) drafts as one unit; then
  `draft_check --unit draw_ui_element,func_800C7EC8,func_800C813C`.
- Not attempted: func_800CC040 (GhostFileAvailable lead), func_800BB9B0 (LoadTrack, confident), track_texture_load.
