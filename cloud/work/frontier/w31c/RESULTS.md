# w31c — save chain (wave 31)

Status: FINAL. Branch wave31 (main checkout, base 880e729e); nothing committed, spliced or pushed. No permission
denials. Builder scratch watchman2:~/rush2049/scratch/frontier/w31c (toolkit reused from wtk), deleted at the end.
Flags `-g0 -O3 -mips2 -G 0 -non_shared` (IDO 5.3); unit = `blob_unit --tag w31c`.

## Table

| Function (GC name) | Bytes | Before | After | Group compile | draft_check | State / residual |
|---|---:|---|---|---|---|---|
| SaveServer (SaveServer) | 900 | unit EQUAL, group 176/225 | **EQUAL** unit | **MATCH** | EQUIVALENT (w30r, 3,490 calls) | **member** |
| func_800CC040 (SaveGhost) | 896 | 29/224 (frame 136/184) | **EQUAL** unit | **MATCH** | **EQUIVALENT** 4 calls / 4 scripts | **member** |
| func_800CC50C (CompressGhost) | 760 | m2c 117/190 | 9/190 words, 12 rows, frame 104/104 | context (old draft kept) | **EQUIVALENT** 2 calls / 2 scripts | as1 placement + 1 colour |
| pak_delete_file_retry (PS_DeleteFile role) | 784 | 166/196, 98 rows | unchanged (cause re-traced) | — | **EQUIVALENT** 2 calls / 2 scripts (harness fix) | callee-saved colour |
| CreatePlayer (CreatePlayer) | 1,088 | 241/272, 142 rows | unchanged | context | EQUIVALENT (w29d) | structure (name loop) |

New coverage if the packet installs: SaveServer 900 + func_800CC040 896 = **1,796 B**, 2 functions.

## The packet: `groups/menu_save_options/` (group name `frontier_menu_save_options`)

Supersedes the locked `menu_save_options` in place (same name; all 20 current members kept). Files: group.c,
save.c, ctx_hash.c, ctx_slot.c, ctx_newghost.c (all from the locked group) + **ctx_strncmp.c** (new).
- **members/claims (22)**: the 20 locked members (func_800CD058, func_800CCEFC, func_800CCCCC, menu_save_options,
  func_800A44E8, func_800A370C, draw_speedometer, func_800C7EC0, func_800C7EC8, draw_ui_element, func_800CBF2C,
  func_800C7300, func_800CD0FC, func_800A361C, func_800A1A3C, func_80094FC4, menu_item_value_get, menu_transition,
  AdjustSteer, func_800C7200) + **SaveServer** + **func_800CC040**.
- **context**: CreatePlayer, draw_number, format_string_parse, menu_back, audio_reverb_update, audio_buffer_sync,
  func_800CC50C, **func_800950AC** (SaveServer and func_800CC040 moved to members).
- **keep**: the locked list (still has SaveServer, func_800CC040) + **func_800950AC**.
- File changes against src/blob/groups/menu_save_options:
  - group.c: SaveServer defined `(s32 forceUpdate, s32 rumbleOnly)` + compiled-out `if (forceUpdate | rumbleOnly) {}`
    (DISCLOSED DEVICE; the `(void)` form is also MATCH with no device, see `SaveServer/v/groups/g1`); header comment.
  - ctx_newghost.c: one line, `void SaveServer(void);` -> `void SaveServer(s32 forceUpdate, s32 rumbleOnly);`.
  - ctx_slot.c: func_800CC040 rewritten on GC SaveGhost's helpers; cart_free now calls nested helpers; new
    non-static, non-keep (internal) helpers `ghost_heap_lock`, `ghost_heap_unlock`, `cart_free_direct`,
    `DeleteGhost`, `GhostDirName` (names checked unused in src/blob). No retail stubs for them (inlined
    non-static helpers leave none).
  - ctx_strncmp.c: byte-identical copy of `src/blob/func_800950AC.c` (locked single, -O2 standalone; here only context).

Builder (`score.py group cand/menu_save_options`, IDO 5.3 796ae99a): rc=0, every member MATCH; with `--claims`: rc=0,
all 22 claims MATCH, `SaveServer: MATCH` + `own .data verified at 0x8011742C (4 bytes …)`, `0x80117430`, `0x80117434
(4 bytes, all zero …)`. Context: CreatePlayer 246/272, menu_back 3/33, func_800CC50C 117/190 (old draft), rest MATCH.

Whole-program unit (`unit_packet.log`; the six packet files `--with`, as renamed copies `SaveServer/v/unitcopy/`
— `_ReadGhostHeader`/`cart_free`/`slot_forget` renamed because the LOCKED group still defines those non-registered
helpers in the current unit; after the supersede they are the same functions):
```
blob_unit --tag w31c score <22 members> func_800950AC --with U/group.c --with U/save.c --with U/ctx_hash.c
  --with U/ctx_slot.c --with U/ctx_newghost.c --with U/ctx_strncmp.c --internal RGH31 --internal cart_free_u
  --internal slot_forget_u --internal cart_free_direct --internal ghost_heap_lock --internal ghost_heap_unlock
  --internal DeleteGhost --internal GhostDirName --neighbours
  EQUAL SaveServer: 225 words (kept, c_group.c)
  EQUAL func_800CC040: 224 words (kept, c_ctx_slot.c)
  EQUAL func_800950AC: 29 words (kept, c_ctx_strncmp.c)
  (+ the 20 old members EQUAL, internal/kept status unchanged)
  locked bodies that differ in this unit: 0
blob_unit score: 23/23 equal
```

### Integration notes (coordinator / w31i)
1. `install_group.py cloud/work/frontier/w31c/groups/menu_save_options menu_save_options` (supersede in place; no
   `--revert-single`/`--revert-group` needed: func_800950AC stays a locked single, it is only context here).
2. Data: SaveServer's function-local statics become this TU's own .data: sInitCountdown 0x8011742C (= 3), sLastPak
   0x80117430 (= -1), sReadingPak[4] 0x80117434 (zero) — verified by `--claims`. The inventory symbols
   D_8011742C/D_80117430/D_80117434 are declared only in include/game_globals.h (use 0/0); no other user.
3. Trap check (wave 28): the context list names a locked single, func_800950AC. It is on this group's keep list, so
   the shadow unit should keep it; if the gate reports it internal/emptied, add `force_keep` for it in
   unit_overrides (func_8008B2B4 precedent). It will appear in the manifest's duplicates (single + context copy,
   byte-identical source), like draw_number.
4. unit_overrides: none expected (SaveServer, func_800CC040 kept as before; new helpers are non-registered, not on
   keep -> internal by the -kp derivation).
5. `--claims` printed no NOT VERIFIED line. No symbols.json/blob.ld change expected beyond the splice.

## SaveServer — the group-compile cause (named lever, closed)
Not a recursion chain: `tools/cg.py` (retail call graph from asm/us/blob: jal edges, address-taken functions from
lui/addiu pairs and from .data words, jalr = may call any taken function) finds NO path from SaveServer's callees
back to SaveServer; save_settings has no caller in the game image at all. Bisection in the unit (`SaveServer/v/`,
`tools/ucheck.sh` counting `lb …,128(…)` loads of sInitCountdown): removing func_800C7EC8 (vA), the ghost helper
(vB), the getter break (vD), track_render_process (vE), all of those (vF) or the sReadingPak stores (vG) keeps the
in-loop reload; removing only the two strncmp calls (vI) removes it. func_800950AC is a defined, kept locked single
in the unit and an undefined external in the group program: uopt treats a call to an undefined external as unable
to touch the TU's function-local statics, a call to a defined procedure as possibly modifying them. With
ctx_strncmp.c as context (+keep) SaveServer is MATCH in the group compile. Prototype: GC's two parameters plus the
disclosed `if (forceUpdate | rumbleOnly) {}` (without it 210/225: ugen stores a0/a1 to their homes); billboard_render
calls SaveServer(1, 0), so the parameters are real at the call sites.

## func_800CC040 = GC save.c SaveGhost (N64 form) — 29 words -> EQUAL
The old draft (w2d/w30c context) differed only in frame (136 vs 184) and sp offsets; every slot was right relative to
the top except the inlined heap-free parameter (ours top-72, retail top-104) and 16 B below it. Levers, each measured
(`func_800CC040/v/`): GC helpers as inlined non-static helpers — `DeleteGhost(ghosth)` (GC DeleteGhost: `if fileh
PS_DeleteFile`), `_FreeGhost` = func_800C7300 for `*saveGhosth = NULL`, a heap free in the limbo path (s2 152, s3
160); the main-path free (GC DeallocateMemory) with NESTED heap lock/unlock helpers puts the parameter at retail's
80(sp) and adds retail's 16 B below (s6 176, 27 words); the "BCK"/"FWD" choice as a helper (s9 184 but `backward` in
v1); taking the header pointer instead of the byte (`GhostDirName(m)`: the byte load is then a ring temp) -> s11
EQUAL. Rejected: one free helper for both paths (umerge stops inlining a two-call free, s1; nested in both paths s13
192); a remove-ghost wrapper (s4/s5/s8: frame right but an extra `ghost` web).
Shaping hypotheses to disclose: two free helper shapes (cart_free with nested lock helpers; cart_free_direct plain),
and GhostDirName's identity (GC SaveGhost picks "BCK"/"FWD" inline next to GetGhostExtension). No unused locals,
no compiled-out reads.
draft_check (`func_800CC040/dc_draft.c` = ctx_slot.c draft + func_800C7300's body; `dc.args`):
`VERDICT: EQUIVALENT over 4 calls / 4 scripts (ghost_save 1, ghost_pak 1, ghost_resave 1, ghost_delete_fail 1)`.
Harness facts: func_800CBF2C's convention is h=s1, back=s0, select=s3 and it clobbers s2 (regmap_verify --derive):
`--callee 's32 func_800CBF2C(void *h, s32 back, s32 select)=s1,s0,s3/s2'`; a draft calling the stub func_800C7300
must carry its body (the retail address is the empty `jr ra` stub).

## func_800CC50C = GC save.c CompressGhost — fresh draft, 9 words (`func_800CC50C/best.c`)
GC transcription (memmove = func_800A47C0, _CalcChecksum = format_string_parse, AllocateMemoryHeapHigh =
sound_play_menu, zip = car_angular_velocity_clamp, _NewGhost = func_800C7200, AllocateHandleHeap =
audio_task_complete, GetPtrSize = audio_buffer_sync, ghostRaceTimes = D_80111754, gRetraceRate = D_8002AFB4,
gTrackNum = D_8014978C): 85 rows -> `s32 maxsize` (trunc.w.s) 50 -> GC PS_GetBlocks as an inlined helper 47 -> a
named `gh = ghost->header` local before `unzippedChecksum` (frame 96 -> 104, homes exact) 31 -> GC's memmoves on
`header->start` with `sorted` used only by checksum/zip 12 rows, 9 words. Header field N64 offsets: start 0x4C,
offset 0x50, size 0x54, the s16 cleared at 0x24 (GC 0x22), new header revision 17 (GC 20), data at +0x58.
Left: (1) the first free's parameter load lands in osRecvMesg's delay slot (retail: before the lock's argument
set-up) — one-line/nested helper forms change nothing (z17–z21); (2) `gh` gets v1, retail a3.
draft_check: `VERDICT: EQUIVALENT over 2 calls / 2 scripts (ghost_save 1, ghost_pak 1)` (dc.args: the
audio_reverb_update shim). Not put in the packet (the context copy in ctx_newghost.c is unchanged).

## pak_delete_file_retry — cause re-traced; the "a1 bug" is a harness convention (`pak_delete_file_retry/`)
- One fewer nested helper (p1 pak_buffer_free without nested lock helpers, p2 no func_800A2670 helper, p3 no
  func_800A2678 helper, p4 inline buffer free): the lock message slots move DOWN with the frame (relative offset
  unchanged), so the extra 8 B is above them. It is the named `pfs` local: without it (p5) the lock slots sit 36 B
  below the frame top as in retail and every named home keeps retail's relative offset; the frame is then 224 =
  retail - 8 = retail's spill slot of the &func_8008A6A4 callback (t0 -> 96(sp)). ctrace (p5): callback p1 w41
  tot 20 > callee cost 10.5 -> s0; the pfs expression (controller+12, retail s8) w99 tot 10 < 10.5 -> split. So the
  residual is callee-saved colour only (w28b/w29d's diagnosis), not an inline area. best.c stays w28b's (98 rows).
- draft_check: `audio_reverb_update` is IPA-internal (address a1, tag a2, clobbers s0/s1 unsaved — regmap_verify
  derive). The C call `audio_reverb_update(address, 0)` is right in the unit; standalone o32 needs
  `--callee 'void audio_reverb_update(u32 address, s32 tag)=a1,a2/s0,s1'` (in dc.args). With the a1/a2 shim alone
  the draft still faulted (node in s0/s1 clobbered); with the clobbers: `VERDICT: EQUIVALENT over 2 calls / 2 scripts
  (ghost_resave 1, ghost_delete_fail 1)`. Suggest adding this entry to tools/recomp/ipa_callees.json (not edited).
- Next: a source form giving the pfs expression a second weighted use / the callback a single-block web
  (force.sh with callback=t0, pfs=s8 on p5).

## CreatePlayer — one lane-hour on the _GetUniqueExtension lever, no improvement (best.c = w30c's)
`CreatePlayer/v/`: c1 (stub 0x800CD0F4 as `func_800CD0F4(char *ext, s32 attempt)` = sprintf + digit loop, _Pow
nested) ops 44 -> 40 but words 142 -> 159, frame 224 -> 240 (helper locals digits/src/i + parameter area); c13
(file + ext parameters) 161, frame 248; c3/c4/c5 (helper also holding the name loop) are NOT inlined by umerge (body
196 words); c8/c9 (`s32 func_800CD0F4(s32 c)` = name-character filter) give the exact body length 272 but 153 rows,
frame 232 and a materialised 0/1 result retail does not have. Retail evidence for the next lane: the digit loop
reads `file[9+i]` through the file base (s8) and stores through `ext` = sp+193 (s5) with the caller's `i` in s0; the
name loop's `move v0,v1` (a copy of the loaded char used for the compares while the store uses the load) is the
signature of an inlined helper parameter or a separate `c` variable.

## GC names / layouts recorded (source function)
SaveGhost = func_800CC040 (save.c SaveGhost; N64 adds the player parameter and the remove-old-ghost loop);
DeleteGhost = inlined helper in func_800CC040 (save.c DeleteGhost); SaveGhostInLimbo = func_800CB748; GetPtrSize =
audio_buffer_sync; DeallocateMemory = the heap-locked audio_reverb_update sequence (cart_free); CompressGhost =
func_800CC50C (save.c CompressGhost); memmove = func_800A47C0; AllocateMemoryHeapHigh = sound_play_menu; zip =
car_angular_velocity_clamp; AllocateHandleHeap = audio_task_complete; PS_GetBlocks = `(size + 255) >> 8` inlined;
ghostRaceTimes = D_80111754; gRetraceRate = D_8002AFB4 (save.c CompressGhost); _GetUniqueExtension = stub 0x800CD0F4
still unproven (save.c order); strncmp = func_800950AC.

## What generalises
- **uopt and function-local statics:** a call to an UNDEFINED external is assumed not to modify a TU's
  function-local statics; a call to a DEFINED procedure (even a trivial leaf like strncmp) is. A group compile that
  hoists a static the unit reloads needs the defined callee as context (+keep), not a recursion chain.
- **Frame offsets relative to the top** split a frame miss into "named locals" vs "inline areas" vs "spills": compare
  each home as `frame - offset`. Nested lock/unlock helpers inside a free helper add their areas BELOW its parameter.
- **umerge stops inlining** a small helper once it has two call sites in the caller (cart_free twice) — two shapes
  in one function are two helpers.
- **draft_check for IPA callees needs their clobbers** (`=regs/clobbers`): audio_reverb_update s0,s1;
  func_800CBF2C s2 — derive them with `regmap_verify --derive`.

Tools: `tools/cg.py` (retail call graph / reachability), `tools/ucheck.sh` (unit score + one-function disassembly).
