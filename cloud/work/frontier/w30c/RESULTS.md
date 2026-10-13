# w30c — save chain (wave 30)

Status: final. Branch wave30 (main checkout, base 1b34f797); nothing committed, spliced or pushed. No permission
denials. Builder scratch watchman2:~/rush2049/scratch/frontier/w30c (toolkit reused from wtk), deleted at the end.
Flags `-g0 -O3 -mips2 -G 0 -non_shared` (IDO 5.3); unit = `blob_unit --tag w30c`. Starting drafts: w29d
func_800C813C/best.c (the whole save-unit file), w28b AdjustSpeed/best.c.

## Table

| Function (GC name) | Bytes | Before | After | Group compile | Residual / state |
|---|---:|---|---|---|---|
| draw_speedometer (_CheckAndRepairPlayer) | 1,704 | 99/426 | **EQUAL** unit | **MATCH** | member, claimed |
| func_800C7EC8 (_ReadPlayer) | 628 | unit-EQUAL, blocked | **EQUAL** unit | **MATCH** | member, claimed |
| func_800C7EC0 (_NewPlayer stub) | 8 | locked stub | EQUAL | MATCH | member (stub given body) |
| draw_ui_element (_CheckAndRepairGhost) | 240 | unit-EQUAL, blocked | **EQUAL** unit | **MATCH** | member, claimed |
| func_800CBF2C (_ReadGhostData) | 276 | never drafted (slot_release context only) | **EQUAL** unit | **MATCH** | member, claimed |
| func_800C7300 (_FreeGhost stub) | 8 | locked stub (was thought _AlreadyReadGhost) | EQUAL | MATCH | member |
| func_800CD0FC (_Pow stub) | 8 | locked stub | EQUAL | MATCH | member |
| func_800A361C / func_800A1A3C / func_80094FC4 (pak getters) | 36 each | locked singles | EQUAL (re-typed) | MATCH | members, supersede singles |
| SaveServer | 900 | 133/225, frame 136/160 | **EQUAL** unit, frame 160/160 | 176/225 | context: group-compile only (see below) |
| CreatePlayer | 1,088 | 241/272, 142 rows | unchanged (best of ~12 variants) | context | name-loop constant colouring + structure |
| pak_delete_file_retry | 784 | 166/196, 99 rows | unchanged | — | layout clue only (see below) |

New coverage if installed: draw_speedometer 1,704 + func_800C7EC8 628 + draw_ui_element 240 + func_800CBF2C 276 +
three stubs 24 = **2,872 B** (the getters are already locked; re-typed only).

## The packet: `groups/frontier_menu_save_options/`

Supersedes the locked `frontier_menu_save_options` (same name; all six members kept: func_800CD058, func_800CCEFC,
func_800CCCCC, menu_save_options, func_800A44E8, func_800A370C; `save.c` byte-identical; `group.c` = this lane's
save-unit file). Files: group.c, save.c, ctx_hash.c, ctx_slot.c, ctx_newghost.c.
- members/claims (17): the six old members + draw_speedometer, func_800C7EC0, func_800C7EC8, draw_ui_element,
  func_800CBF2C, func_800C7300, func_800CD0FC, func_800A361C, func_800A1A3C, func_80094FC4.
- context: SaveServer, CreatePlayer, draw_number, format_string_parse, func_800CC040, menu_item_value_get, menu_back,
  menu_transition, AdjustSteer, audio_reverb_update, audio_buffer_sync, func_800C7200, func_800CC50C.
- keep: CreatePlayer, func_800CD058, func_800CCEFC, menu_save_options, func_800A44E8, func_800A370C, SaveServer,
  func_800A361C, func_800A1A3C, func_80094FC4, func_800CC040, menu_item_value_get, menu_transition, AdjustSteer,
  audio_buffer_sync, func_800CC50C. (The keep list mirrors the shadow unit: format_string_parse, menu_back,
  audio_reverb_update and func_800C7200 are INTERNAL in the unit and must not be kept here — keeping
  format_string_parse alone cost draw_speedometer 417 words in the group compile.)
- Non-member helper `_ReadGhostHeader` (non-static, not keep; umerge inlines it into SaveServer; no retail stub).

Builder, `score.py group --claims cand/frontier_menu_save_options`: exit 0, every claim `MATCH` (log
`claims_score.log`). Without --claims the same 17 members MATCH; context: SaveServer 176/225, CreatePlayer 246/272,
func_800CC040 29/224, menu_back 3/33, func_800CC50C 117/190 (unmatched drafts), the rest MATCH.

Whole-program unit with all packet files (`unit_packet.log`):
```
blob_unit --tag w30c score draw_speedometer func_800C7EC0 func_800C7EC8 draw_ui_element func_800CBF2C func_800C7300
  SaveServer func_800CD0FC func_800A361C func_800A1A3C func_80094FC4 func_800CD058 func_800CCEFC func_800CCCCC
  menu_save_options func_800A44E8 func_800A370C menu_item_value_get func_800CC040 func_800C7200 format_string_parse
  --with G/group.c --with G/ctx_hash.c --with G/ctx_slot.c --with G/ctx_newghost.c --internal draw_speedometer
  --internal func_800C7EC0 --internal func_800C7EC8 --internal draw_ui_element --internal func_800CBF2C
  --internal func_800C7300 --internal func_800CD0FC --internal _ReadGhostHeader --neighbours
  EQUAL draw_speedometer: 426 words (internal, c_group.c)
  EQUAL func_800C7EC0: 2 words (internal, c_group.c)
  EQUAL func_800C7EC8: 157 words (internal, c_group.c)
  EQUAL draw_ui_element: 60 words (internal, c_group.c)
  EQUAL func_800CBF2C: 69 words (internal, c_group.c)
  EQUAL func_800C7300: 2 words (internal, c_group.c)
  EQUAL SaveServer: 225 words (kept, c_group.c)
  EQUAL func_800CD0FC: 2 words (internal, c_group.c)
  EQUAL func_800A361C: 9 words (kept, c_group.c)   (also func_800A1A3C, func_80094FC4: 9 words, kept)
  EQUAL func_800CD058 / func_800CCEFC / func_800CCCCC / menu_save_options / func_800A44E8 / func_800A370C
  EQUAL menu_item_value_get: 17 words (kept, c_ctx_slot.c)
  FAIL func_800CC040: ... (unmatched context draft, as before)
  EQUAL func_800C7200: 64 words (internal, c_ctx_newghost.c)
  EQUAL format_string_parse: 45 words (internal, c_ctx_hash.c)
  locked bodies that differ in this unit: 0
```

### Integration notes (coordinator)
1. `install_group.py cloud/work/frontier/w30c/groups/frontier_menu_save_options frontier_menu_save_options
   --revert-single func_800A361C --revert-single func_800A1A3C --revert-single func_80094FC4` (the three getters move
   from their single files into the group with the same bytes; their single sources use split symbols
   `&D_80144031/&D_80144036 + pak*0x304`, which cannot CSE with SaveServer's `&D_80144030[pak]`).
2. unit_overrides `force_internal` probably not needed (members not on keep become internal by derivation); if the
   gate disagrees: draw_speedometer (IPA player s8), func_800C7EC8 (fileh s0), draw_ui_element (t0/t2/t1),
   func_800CBF2C (decode s0, ghosth s1, select s3), func_800C7EC0, func_800C7300, func_800CD0FC (stubs).
3. Context duplicates: ctx_slot.c = src/blob/groups/menu_cc040_20261004/group.c minus its func_800CBF2C draft;
   ctx_newghost.c = src/blob/groups/codex_func_800C7200_2/group.c minus its SaveServer draft; ctx_hash.c =
   codex_hash_a80/group.c verbatim. Those two locked groups still carry the old func_800CBF2C / SaveServer CONTEXT
   drafts: once this group owns func_800CBF2C as a member, slot_release's context copy should be replaced by a copy
   of this one (or dropped) so the unit has one definition (the unit currently resolves it to ours with --with).
4. Data: no data-symbol changes (SaveServer is context only; its function-local statics would become the TU's own
   .data at 0x8011742C/30/34 when it lands). D_80144031/D_80144036 stay in the inventory (other users).
5. Wave-28 trap check: context names locked kept functions (menu_transition, AdjustSteer, menu_item_value_get,
   audio_buffer_sync, func_800CC50C...) — all on this group's keep list, so the shadow unit should not empty them;
   format_string_parse / audio_reverb_update / func_800C7200 / menu_back are internal there already.

## Per-function notes

### draw_speedometer = GC _CheckAndRepairPlayer(player, repair) — 99 -> EQUAL
Two GC-structure levers: (1) the four option-repair loops `21..41` (gTrackNum = D_8014978C saved/forced) use their
own counter `j` (GC has `i` and `j`): 99 -> 49 words; (2) the stats loops indexed by `i` (`save + 140 + i * 96`, GC
`data->raceStats[i]`) instead of a byte offset `off`: 49 -> 0 (the dead `off` local removed). Frame 32/32.
N64 player-data layout (GC __anon_387c, 0x810 B; N64 lacks GC's raceType byte): nameChecksum 0, id[2] 4, revision 12,
name[13] 13, optChecksum 28, options[21] 32, contChecksum 56, controls[11] 60, rumble 71, raceSettings 72 (20),
battleSettings 92 (16), stuntSettings 108 (20), obstacleSettings 128 (12), raceStats[6] 140 (96 each),
stuntStats[4] 1292 (64), battleStats[8] 1548 (12), obstacleStats[1] 1644 (24), circuitStats[4] 1668 (28),
circuit 1780 (76), carSettings[13] 1856 (16).

### func_800CBF2C = GC _ReadGhostData(ghosth, decode) + N64 flag — first draft -> EQUAL
From slot_release's context copy with the draw_ui_element arguments corrected to (ghosth, 1, 0)/(ghosth, 1, 1):
57 words; `fileh != NULL` arm first (menu_transition, then re-read): 7 words (temp ring); GC's form — test
`(*fileh)->data`, then `ghost->header = (*fileh)->data; (*ghost->header)->start = NULL;` without a local — EQUAL.
GC names: menu_back = _DecodeGhost (IPA s1), menu_transition = ghost-data free, menu_item_value_get = ReadGhostData,
drone_set_catchup = PS_ReadFile.

### SaveServer — 133/225 -> EQUAL in the unit (frame 160/160); NOT installable yet (group compile 176/225)
Levers (each measured):
1. The v0-loaded channel fields are the locked pak-TU getters func_800A361C (byte 0), func_800A1A3C (byte 1),
   func_80094FC4 (byte 6), inlined by umerge (each = a v0 return web + 8 B of frame), re-typed on D_80144030 so the
   inlined copies share s8 = &D_80144030[pak]. Present/state/busy at the top, state again per file.
2. No `lastPak` local: the static sLastPak is used directly (uopt keeps it in v1, narrows `sLastPak = pak` to
   `lb v1,159(sp)` from pak's home and sinks the store into the fileh path): 33 words, frame/body exact.
3. Ghost branch = inline helper GC `_ReadGhostHeader(fileh)` with GC `_AlreadyReadGhost`'s loop inside (one `ghosth`
   for the search and the new ghost = retail s0); `ghost->header = (*fileh)->data; header = *ghost->header;`
   (one fewer coloured web).
4. The stub 0x800C7300 is GC `_FreeGhost(ghosth)` (GC order _NewGhost, _FreeGhost, _AlreadyReadGhost; N64
   0x800C7200 = _NewGhost, then the stub), `*ghosth = NULL`, inlined on the repair-failure path: frame 152 -> 160.
5. GC's `while (fileh != NULL)` instead of `if (fileh) do {} while`: the last 2 words (an as1 line-number tie between
   `sb v1,sLastPak` and the hoisted getter address). -> `EQUAL SaveServer: 225 words (kept, c_ss25.c)`.
Prototype: (void) here. GC's `(s32 forceUpdate, s32 rumbleOnly)`: ugen stores a0/a1 to their homes (210 words);
EQUAL only with a compiled-out `if (forceUpdate | rumbleOnly) {}`. Locked callers (save_settings, codex_input_aux)
declare (s32, s32) and call (0, 0): a parameter-list mismatch across TUs, not a false return type — coordinator call.
Why the group compile differs (traced, not fixed): in the group program uopt treats sInitCountdown as unmodified by
the loop's calls (loads it once into a0 and tests it at the loop top without a reload); in the shadow unit it reloads
it every iteration (retail `lui t9; lb t9,sInitCountdown` at +0x78). Hypothesis: in the unit some known call chain
from SaveServer's callees reaches SaveServer again (recursion => the function-local static may change), the group
program lacks it. Next lever: find that chain (the unit's call graph from func_800C7EC8/draw_text/object_render_cleanup/
AdjustSteer/... to save_settings or codex_input_aux) and add it as context, then move SaveServer to members.
draft_check not re-run (w29d: EQUIVALENT over 5,455 calls with its dc.c; the logic is unchanged apart from the
statics now being used directly).

### CreatePlayer — unchanged (241/272, 142 rows, frame 224/224); best.c = group.c
Tried (rows): name walker pointer forms 146–155; goto / while(1) attempt loops 142 (no change); digit-store address
forms `file[i+9]`/`(file+9)[i]`/`*(file+9+i)` 142–155; an `ext = &file[9]` pointer through the existing local `p`
(`CreatePlayer/v/cp10_ext.c`) gives retail's `addiu s?,sp,193` expression web but 152 rows. ctrace: all eleven char
constants are equal-priority webs (save 7.14, tot 100, 14 blocks) hoisted out of the attempt loop; the two lowest web
numbers ('.', ':') win s5/s6, which pushes `i` to a3 (spilled) and `name` to its home; retail colours none of them
(only '=' and '*' as caller-saved split pieces a1/a2 inside the name loop) and keeps i s0, name s7, ext s5.
Force oracle on cp10 with retail's s-register map: still 118 rows -> not colour-only.
GC lead: the stub 0x800CD0F4 just before _Pow (0x800CD0FC) is a second deleted static of CreatePlayer — by GC file
order (_GetUniqueExtension at line 3511, _Pow at 3619) probably the N64 name/extension builder; the retail digits
store through a separate `ext` web matches GC `_GetUniqueExtension(..., char *ext)`. Next: write the name+digits
builder as that inlined static (with _Pow nested), sized against the 224 frame.

### pak_delete_file_retry — unchanged (166/196, 99 rows); layout clue only
Retail's osRecvMesg message slots of the two inlined lock calls are at 196/172, ours 188/164 with every named home
equal (request 208, retry/status 215/214) and the frame equal (232): the inline-area block sits 8 B higher in retail
and the spill block has one more slot (84, 88, 96 + one). Dropping the `pfs` local moves the named homes, not the
lock slots (frame 224). So one 8 B inline area of ours (the nested pak_heap_lock/unlock in pak_buffer_free, or the
func_800A2670/func_800A2678 helpers) is probably not an inline call in retail, and the callback constant's spill
slot (96) is retail's extra spill web. The named lever (D_8011EAE8 address web) was not reached; next: test the
area hypothesis (one fewer nested helper) before the colour oracle.

## GC names / layouts recorded (source function)
_CheckAndRepairPlayer = draw_speedometer, SavePlayerOption = audio_bus_mix, gOptions = D_80146108, gTrackNum =
D_8014978C (save.c _CheckAndRepairPlayer); _ReadGhostData = func_800CBF2C, _DecodeGhost = menu_back, ReadGhostData =
menu_item_value_get (save.c _ReadGhostData/ReadGhostData); _FreeGhost = stub 0x800C7300, _NewGhost = func_800C7200,
_ReadGhostHeader/_AlreadyReadGhost = inline in SaveServer (save.c order 2541–2614); PS_PakServer = track_render_process
(SaveServer); channel getters func_800A361C/func_800A1A3C/func_80094FC4 = gPakChannels[pak] bytes 0/1/6 (GC
PS_PakInserted family, paksysdc.c; exact GC names not determined); _GetUniqueExtension = probable stub 0x800CD0F4.

## What generalises
- A callee-saved web that "should not exist" in a loop nest can be two loop counters merged into one variable (GC
  `i`/`j`), and `off`-stepped loops are retail `a[i]` loops after IDO's strength reduction.
- v0-loaded struct fields in a caller are umerge-inlined LOCKED getters from other TUs; a locked getter written on a
  split symbol (`&D_80144031`) cannot CSE with the caller's base — re-type it (same bytes) in the group.
- A function-local static used directly (no local copy) shows up as a v1 web plus a narrowed `lb` from a stack home.
- For group packaging, copy the shadow unit's keep/internal status of every context function (blob_unit manifest
  `keep`/`internal`): a context callee kept in the group but internal in the unit breaks IPA register use.
