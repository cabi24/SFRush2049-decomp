# w36g (wave 36) — circuitscr + cheatscr, runtime image A

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| func_803A5948 | animText@circuitscr.c (+ inlined animButtonText, getFinishPlaceStr) — CONFIRMED | 4320 | **group MATCH** (code identical; own .rodata/.data verified locally vs build/ovl/a/image.bin: 10 sites, 0 failures) | groups/dot_runtime_a_text_helpers_20261006/ |
| func_803A5940 | animButtonText@circuitscr.c deleted-inline stub | 8 | group MATCH | same group |
| func_803A5938 | getFinishPlaceStr@circuitscr.c deleted-inline stub | 8 | group MATCH | same group |
| func_8039A4D4 | updateButtons@circuitscr.c — CONFIRMED (called last by C140 = updateCircuitScreen; private ABI, saves only ra) | 2864 | **group MATCH** (code identical; own .rodata/.bss verified locally: 30 sites, 0 failures); 13 disclosed unused homes = exact frame residual | groups/dot_runtime_a_text_helpers_20261006/ |
| func_8039C910 | updateCheatScreen@cheatscr.c (initScreen/uninitScreen/InitEgg inlined) — CONFIRMED (NOT updateCircuitScreen: that is the locked C140) | 1844 | **group MATCH** (code identical; `score.py fn` standalone also MATCH with the switch table unverified; table verified locally, base 0x803B95BC) | groups/w36g_cheat_screen/ |
| func_8039C900 / func_8039C908 | uninitScreen / initScreen@cheatscr.c deleted-inline stubs | 8+8 | group MATCH | groups/w36g_cheat_screen/ |
| func_803A5358 | animText@cheatscr.c (printOption inlined) — CONFIRMED | 768 | **group MATCH** (no own data) | groups/w36g_cheat_screen/ |
| func_803A5350 | printOption@cheatscr.c deleted-inline stub | 8 | group MATCH | groups/w36g_cheat_screen/ |

Totals: 9,836 new image-A bytes in two group packets (text_helpers revision 7,200 B: A4D4 2,864 + A5948 4,320 + 2 stubs; new cheat
group 2,636 B: C910 1,844 + A5358 768 + 3 stubs), all code-identical; own data verified locally against build/ovl/a/image.bin. Flags `-g0 -O3 -mips2 -G 0
-non_shared` throughout. No permission denial. Nothing outside this lane dir was written (no cloud/matches file: the
overlay scorer cannot verify own data, so a single with a switch table exits 1 under CI).
Builder scratch: watchman2:~/rush2049/scratch/frontier/w36g (from base, master e68aea6e1).
Transcripts: groups/<packet>.score.txt (all members) and groups/<packet>.claims.txt (`--claims`, both EXIT=0 with
"allow_unverified": true; the text_helpers packet now carries that flag too).

## Integration: superseding `dot_runtime_a_text_helpers_20261006` (same name)
- Packet: `cloud/work/frontier/w36g/groups/dot_runtime_a_text_helpers_20261006/` (group.c + group.json).
- members = the 13 installed members (unchanged text, all still MATCH) + func_8039A4D4, func_803A5948, func_803A5938,
  func_803A5940; claims = those four (7,200 B new); context = [] (A4D4 was the only remaining context);
  keep = func_8039C140, func_803A5948 (unchanged).
- func_8039A4D4 line verbatim: `MATCH (30 section-relative relocations unverified: .rodata+0xc at +0x104, ... .bss+0x4 at +0xb14)`
  with `not verified: own .bss+0x0 ... retail address 0x803BA228 (+4 bytes) is not in a known game .bss range` (expected
  on the builder; image-A .bss). Local owndata.verify vs build/ovl/a/image.bin: `func_8039A4D4 sites 30 failures []
  unverified [] bases {'.rodata': '0x803b9570'}` (B560 shares that base: `func_8039B560 sites 16 failures [] unverified []`).
- `score.py group <packet> --targets asm/us/ovl_a` (builder): every member MATCH; func_803A5948 line verbatim:
  `MATCH (10 section-relative relocations unverified: .data+0x0 at +0x328, .data+0x0 at +0x32c, .data+0x0 at +0x348, .data+0x0 at +0x34c, .rodata+0x0 at +0x4e8, .rodata+0x0 at +0x4ec, .rodata+0x4 at +0x4f4, .rodata+0x4 at +0x508, .rodata+0x8 at +0x58c, .rodata+0x8 at +0x5b0)`
  with `not verified: ... no retail data available` (expected on the builder scratch). B560 keeps its inherited 16 unverified rodata relocations.
- Own data proved on the Pi with tools/cloud/owndata.verify against build/ovl/a/image.bin (addresses = score.address_named):
  `func_803A5948 sites 10 failures [] unverified []`, bases .rodata 0x803B9814, .data 0x803B3454. The .data word is the
  block static `del` (retail 0x803B3454 = 0x803B8758 -> "DEL"); it carries an R_MIPS_32 to D_803B8758 (resolves by name).
- New own data in this group: .data (4 B, del) and three float literals. If the group splice refuses an R_MIPS_32 inside
  owned .data, fallback = `extern u8 *D_803B3454` (6 words differ: uopt keeps &D_803B3454 in s0).

## Identities proven
- func_803A5948 = animText@circuitscr.c (GC 0x800695FC); func_803A5940 = animButtonText (GC 0x80068850, inlined);
  func_803A5938 = getFinishPlaceStr (GC 0x8006A000, inlined). Evidence: byte-identical code from the MAT3 port.
- func_8039C910 = updateCheatScreen@cheatscr.c (21-entry jump table of cheat toggles, InitEgg/CheckForEgg calls,
  setGameState 4/16); func_803A5358 = animText@cheatscr.c (13 rows, "%d" for gFrameScale, 21 options).
- Text-layer names (from this match): render_helper = HAL_SetTextDepth, func_800B669C = TB_SetJust,
  brake_light_update = ZOID_ProjectWindow, object_create = HAL_SetFont, dispatch_handler = TB_SetTextColor,
  sound_pitch_diff_halved = TB_CenterText, state_utility = TB_DrawText, camera_auto_follow = TB_DrawTextScrollBox,
  object_bytes_sum_global = TB_GetFontLineHeight, func_800ED66C = HAL_SetTextTranslucency, func_800BEA3C =
  TB_FlashTextColors (tRGBA by value), fcvt_wrapper = TB_Sprintf, func_800BE6A4 = TB_Strcpy, func_800BE4F0 =
  TB_Strcat, object_manager_update = HAL_GetTextWidth (returns unsigned int), crowd_cheer_play = ChangePaneSize,
  func_8039B908 getPointsStr, func_8039B988 getPlaceStr, func_8039BA28 getToGoStr, func_8039BAE0 getFinishedStr,
  func_803A5658 drawReview, func_8039A448 menuItemValid, func_8039C140 updateCircuitScreen.
- Data: see the header of func_803A5948 in the packet (buttonInfo D_803B30C0, gCircuit D_803BAD90, gText D_8017A4E0, ...).

## What generalised (A5948)
- The installed context stand-in was semantically right; it went from 157 normalized rows to 0 by: (1) restoring the
  MAT3 static structure (animText calling animButtonText, which calls getFinishPlaceStr) so umerge inlines and the
  frame grows by the inline reservations; (2) MAT3's unsigned HAL_GetTextWidth return (re-sign-extension of width);
  (3) the label strings through `str` (as1 load order); (4) `del` as a block static (an extern's address is CSE'd
  into s0 across the call); (5) defining getFinishPlaceStr AFTER its caller (MAT3 order) — defined before, umerge
  emits its parameter copy one instruction early.
- Frame-layout sweeps: a batch with `slots-want/got` per variant (tools/vb.py) found the declaration order in one run.

## New group `w36g_cheat_screen` (cheatscr.c, image A)
- Packet: `groups/w36g_cheat_screen/` = eggs.c (InitEgg, genuine MAT3 body, inlined; its retail stub is one of
  func_8038F734/F73C and is NOT claimed; it is an extra named function in the object, not a member) + cheatscr.c;
  members = claims = func_8039C910, func_8039C900, func_8039C908, func_803A5358, func_803A5350 (2,636 B);
  keep = func_8039C910, func_803A5358 (both reached from data: the screen table / the multiblit callback);
  "allow_unverified": true (the only own data is C910's switch table). `score.py group ... --claims` (builder):
  `func_8039C910: MATCH (2 section-relative relocations unverified: .rodata+0x0 at +0x324, .rodata+0x0 at +0x32c)`,
  func_8039C900 MATCH, func_8039C908 MATCH, func_803A5358 MATCH, func_803A5350 MATCH; exit 0.
- A5358: MAT3 port 16 normalized rows / 190 words; the settings_scroll packet's unsigned label offset gives the
  192-word geometry (code identical); the frame (136, str1 +112) needs the Dreamcast unit's used `int x = 154` plus a
  used `name` local (no unused homes). The packet's "carried text pointer" residual does not occur with the MAT3
  `str = str1` form.
- Standalone `score.py fn` of the single-file form (`func_8039C910/v3.c`, statics) also prints
  `MATCH (2 section-relative relocations unverified: .rodata+0x0 at +0x324, .rodata+0x0 at +0x32c)` but exits 1
  (no image-A data tree), so it is NOT placed in cloud/matches (CI would fail); the group form is the deliverable.
- Local owndata: `func_8039C910 sites 2 failures [] unverified [] bases {'.rodata': '0x803b95bc'}`.
- Levers: InitEgg store order (N64 stores gNextTimeOut first); gActiveEdges `s32` (MAT3 unsigned rotates a0/a1/a2);
  define initScreen/uninitScreen after updateCheatScreen (MAT3 order) — the same definition-order lever as A5948.

## A4D4 notes
- MAT3 port v1: 126 normalized rows; function-local statics cursorSpeed/scrollSpeed (v2, uopt keeps scrollSpeed in
  $f16 and sinks the stores): 56; the carscr sibling's idioms (fabsf on the right, int 255, volatile gNumModels view):
  0 normalized rows, registers identical, only stack offsets differ.
- Frame: retail 152 = our 96 + 52 B of untouched homes ABOVE the first-declared local (+4 B shift of the locals by
  local-area alignment). Probes: an inlined helper's locals land below the caller's (not an inline); 11/12 unused
  homes give 144/152 with wrong offsets; exactly 13 unused s32 declared first reproduce every slot (n13).
  A 52-byte unused array also works but pad arrays are not allowed, so thirteen scalars (disclosed in the header).

## What generalises (all four)
- **Definition order of inlined statics is a lever** (twice): umerge-inlined helpers defined AFTER their caller (MAT3
  file order) fixed the last words of A5948 (getFinishPlaceStr param copy) and C910 (as1 order at the join after
  the inlined initScreen). Keep MAT3's function order inside a file.
- **Deleted-inline stubs are claimable**: write the inlined static as a non-static function named after the caller-less
  `jr ra; nop` stub, off the keep list; the stub keeps its symbol (5 stubs claimed here). In this unit the emission
  order is callee-first; name stubs in that order (A5938 getFinishPlaceStr < A5940 animButtonText; C900 uninitScreen <
  C908 initScreen). A static helper's unnamed stub lands inside the previous member (C908 got "1 extra word" from
  InitEgg's stub until InitEgg moved to its own file, eggs.c, ahead of cheatscr.c).
- **Frame layout from real locals first**: A5358's 8-byte gap was the Dreamcast unit's `int x = 154` plus a `name`
  local (both used); A5948's was declaration order + a 192-byte buffer; only A4D4 needed disclosed unused homes (13).
  `tools/vb.py` prints `slots-want/got` per variant, which finds these in one batch.
- **Sibling idioms transfer**: the carscr updateButtons lessons (w35a D6A4: fabsf on the right, int 255, volatile
  gNumModels, function-local static speeds) took circuitscr updateButtons from 126 to 0 normalized rows directly.
- **N64 types differ from MAT3**: gActiveEdges is s32 in image A (MAT3 unsigned), HAL_GetTextWidth keeps MAT3's
  unsigned return, tRGBA is passed by value to TB_FlashTextColors.
- The A5358 research packet's residual ("carried text pointer") disappears with the plain MAT3 `str = str1` form; its
  unsigned label-offset finding was right and needed.

## Log
- 2026-10-20/21: scratch created; A5948 matched (group), A4D4 matched (group, 13 disclosed homes), C910 and A5358
  matched (new cheat group). All four assigned functions done.
