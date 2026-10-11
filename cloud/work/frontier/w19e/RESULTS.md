# w19e RESULTS (wave 19, fresh game singles)

Picks (frontier next, single recipe, fewest prior attempts, not in §0 table, not claimed):
func_800CF06C (548, arcade road.c road), assign_drones (1236, arcade drones), race_position_update (484),
world_effect_update (456).

Builder scratch: watchman2:~/rush2049/scratch/frontier/w19e. Nothing committed, spliced or pushed.

| Function | Bytes | State | Flags | Scorer output |
|---|---:|---|---|---|
| func_800CF06C | 548 | **MATCH** standalone + unit EQUAL | -g0 -O3 -mips2 -G 0 -non_shared | `score.py fn` -> `func_800CF06C:\n  MATCH`; unit `EQUAL func_800CF06C: 137 words (kept, c_func_800CF06C.c)` / `locked bodies that differ in this unit: 0` |
| assign_drones | 1236 | near-miss, 29 aligned words / 7 ops rows, frame 136 ok (was B62 288/309) | -g0 -O3 -mips2 -G 0 -non_shared | `FAIL assign_drones: 295 of 309 words differ; compiled body is 307 words, target 309` (words 29 ops 7 norm 7) |
| race_position_update | 484 | near-miss, 27 ops rows (frame 120/96) | -g0 -O3 -mips2 -G 0 -non_shared | `FAIL race_position_update: 102 of 121 words differ; compiled body is 108 words, target 121` (words 76 ops 27 norm 27) |
| world_effect_update | 456 | near-miss, 12 words (7 with a volatile decl), frame ok | -g0 -O3 -mips2 -G 0 -non_shared | `FAIL world_effect_update: 106 of 114 words differ; compiled body is 112 words, target 114` (words 12 ops 11 norm 11) |

## func_800CF06C (548 B) -- MATCH, `cloud/matches/func_800CF06C.c`

Arcade ancestor: game/road.c `road(MODELDAT *m)` (B112 had identified it; its best was 131/137).
Callees all locked: camera_victory (N64 pre-pass), entity_update = tiresurf, menu_audio_settings = collision.
Unlocks func_800D11BC (sole blocker).

```
$ score.py fn cand_func_800CF06C.c func_800CF06C --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_800CF06C:
  MATCH
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w19e score func_800CF06C --with cloud/matches/func_800CF06C.c --neighbours
  EQUAL func_800CF06C: 137 words (kept, c_func_800CF06C.c)
  locked bodies that differ in this unit: 0
```

Integration: plain single (`splice_singles.py func_800CF06C`); no own rodata (all constants via lui), no
unit_overrides, no groups superseded, no data-symbol changes.

Path (3 rounds, ~25 min): (1) arcade road() with float literals -> 129/137 words (double arithmetic);
(2) f32 literals with the wave-17 spelling rule: retail holds 3.0 twice ($f26 compare, $f28 store), keeps
0 twice ($f0 for the force clear, $f20 in the loop) and rematerialises 40.0 for the store -> `0.f` clear,
`3.0f`/`3.f`, `40.0f`/`40.f`: 0 ops rows, 4 words (frame 176 vs 192, igpos sp+164 vs sp+168);
(3) frame: 12 bytes above igpos + 4 more below -> one more 12-byte local declared before igpos.
Equivalent EQUAL forms: `temp` moved before igpos plus a 12-byte local after it; `temp` first plus four
f32 locals; two empty inlined statics (no stubs near, rejected). Chosen: arcade order plus BODYRWV
(an arcade road.c global) as a local before igpos. DISCLOSED in the header: BODYRWV[3] and temp[3]
are unused; retail frame 192, without both 168, temp only 176; 24 bytes = 6 slots.

Recovered MODELDAT layout (N64, offsets): BODYFORCE[4][3] 0x0C4; TIRERWR[4][3] 0x244; roaduvs[4]
(stride 36, fpuvs[3][3] first) 0x310; suscomp[4] 0x5CC; tpcomp[4] 0x5DC; airdist[4] 0x5EC; airvel[4]
0x5FC; roadcode[4] (s32) 0x60C; sviscode[4] (u16) 0x61C; idt 0x638; s32 at 0x648 cleared each call.
AIR roadcode = 8.

## race_position_update (484 B) -- near-miss, 27 ops rows (structure right, two extra webs missing)

Not a race function: it is the N64 text helper "fit string into width with an ellipsis" (callers: music_fade
passes `(buf, str, s16 width)`). Callees all locked: object_manager_update = text width `(u8 *, s16 maxlen)`,
func_800BE4F0 = strcat, func_800BE6A4 = strcpy, func_800BE744 = strlen (all wide-string aware: a leading 0xFF
byte marks a 16-bit string). D_80121DE8 is the ellipsis string, D_80149B70 (s8) the letter spacing.
Reconstruction (`race_position_update/best.c`): width check -> strcpy and return; wide = (*dst == 0xFF) read
before the copy; len < 2 -> return; n = len - 2; tail = "..." + the last two characters (wide: hand-copied 4
bytes into tail[7..10]); then shorten dst one character at a time until width + tail fits; strcat(dst, tail).

```
us.sh race_position_update best.c        -> FAIL race_position_update: 102 of 121 words differ; compiled body is 108 words, target 121 | words 76 ops 27 norm 27 | frame 120/96
us.sh race_position_update best_frame.c  -> FAIL ...: 101 of 121 words differ; compiled body is 108 words | words 71 ops 27 norm 27 | frame 120/120
```
Traced cause (ctrace, v8): retail has two more register webs than any draft. It saves s0-s6 but s5 is never
used, and the loop's width call passes a register `s4 = -1` converted to s16 at every call (`sll a1,s4,16;
sra`) while the two calls outside the loop pass `li a1,-1`. A named `lim = -1` variable, `0xFFFF`, a merge
of two assignments and an inlined `text_width(u8 *, s32)` wrapper all constant-fold to `li a1,-1`. With the
two webs missing, `w` (the measured width) is split to memory (ours) instead of living in a2 with
spill/reload around every call (retail sp+92). `best_frame.c` (three calls through an invented inlined
wrapper, frame 120 exact) is NOT a proposal: there is no caller-less stub to justify it.
Best next hypothesis: whatever produces the s4 web is also the s5 web (an inlined helper whose parameter
is an int, e.g. a "measure up to n characters" wrapper whose deleted stub is elsewhere in the unit); look
for a `jr ra; nop` stub whose callers vanished in the text module (0x800BDxxx-0x800BExxx).

## world_effect_update (456 B) -- near-miss, 12 words (7 with a volatile declaration), frame 24 ok

N64 graphics/heap init, not an effect update: clears four counters, sets D_80154188 = 90.0f (FOV),
`while (osTvType == 0) {}` (osTvType 0x80000300 loaded once), viewport_setup(osTvType, get_tv_offset(),
D_8002AFC0, D_8002AFC4), allocates three 0x22620-byte buffers (audio_dma_sync, sound_play_menu x2; each
passed to audio_loop_control(buf, 0)), aligns all three `(p + 32) & ~63`, fills the two 0x80-byte
SegmentTableEntry records at D_80156BE0 (+0x58 display-list base D_800586D0 / D_8006A090, +0x5C s16 = 2,
+0x7C aligned buffer), then D_8015B250 = seg[0].base, D_8015B260 = seg[0].base + 1728, msgq_ptr (0x801497C8,
really the display-list cursor) = seg[0].base + 0x9CC0, D_801497F4 = msgq_ptr, then calls func_800A5B3C,
func_800A5488, particle_velocity_set. Preceded by the locked caller-less stub func_800EE8AC.

```
us.sh world_effect_update best.c           -> FAIL world_effect_update: 106 of 114 words differ; compiled body is 112 words, target 114 | words 12 ops 11 norm 11 | frame 24/24
us.sh world_effect_update best_volatile.c  -> FAIL world_effect_update: 110 of 114 words differ; compiled body is 113 words, target 114 | words 7 ops 7 norm 7 | frame 24/24
```
Path: 39 words (globals written directly) -> 7/12 words by reading the base back from the record just
stored (`D_8015B250 = D_80156BE0[0].dl_base; ... + 1728; ... + 0x9CC0`): uopt forwards the stored address
and ugen keeps it in the temp t9 as in retail (written as `D_800586D0 + 1728` the offsets fold into
relocations; written as `D_8015B250 + 1728` the base becomes a coloured web in a2/a3).

Remaining:
1. Head (7 words, both files): retail keeps `addiu sp; sw ra` at the top and leaves the `bnez` delay slot
   empty; ours lets as1 sink the prologue and fill the slot with the `swc1`. as1 trace (`as1t.sh v5`) has one
   block from the prologue through the bnez. Every loop form tried gives the same code (while, if + while(1),
   goto, do-while, a local copy of osTvType, a static helper holding the loop with or without the stores,
   `do {} while (0)` before/after the stores). Retail must have a block boundary or an as1 barrier there.
2. msgq_ptr reload (5 words in best.c): the uopt listing for best.c already has `sw $12,0($7); lw $13,0($7)`;
   as1 deletes the reload. Retail keeps it. The only listing difference with `extern u8 *volatile msgq_ptr`
   is `.set volatile` around the two accesses, which stops as1 (best_volatile.c). Not proposed: the other
   users of msgq_ptr are locked with a plain declaration. `do {} while (0)` or `if (msgq_ptr) {}` between the
   two statements does not keep the load.
**Proven (asm.sh oracle): both residuals are as1 block boundaries.** Taking best.c's own ugen listing and
inserting two labels -- one right after the prologue (`.frame` line) and one between `sw $12,0($7)` and
`lw $13,0($7)` -- assembles to the retail function exactly:
`asm.sh v26 e2.s world_effect_update --summary` -> `want 114 words, got 114; differing rows 0 (words)`
(`world_effect_update/asm_proof/e2_two_labels.s`). The first label alone leaves 6 rows (the msgq reload), so
retail's entry block holds only the prologue and a block boundary separates the msgq store from its reload.
Source forms tried that do NOT create those boundaries: `do {} while (0);` (top, after the stores, after the
msgq store: at the msgq point uopt then forwards the value itself), `if (msgq_ptr) {}`, an empty inlined
static at either point (the stub func_800EE8AC hypothesis), a static helper holding the stores/loop.
Best next hypothesis: the retail entry block is empty because the body starts with a construct whose
code moved or vanished (a loop header, or a uopt long-block split counted from a different statement
sequence). Look at uopt's block list (`w17a/tools/patch_lbb.py`) for best.c vs a variant with the stores
reordered, to find what makes uopt open a new block at the function head.

## assign_drones (1236 B) -- near-miss, 29 aligned words / 7 ops rows, frame 136 exact

Not the arcade assign_drones (label is historical; B62's description was right): the N64 end-of-race
records update. For each player (count s16 D_8014A108, 0x4C-byte Player records at D_8014A118): resolve the
profile pointer (cached at +0x48, else &D_80146150[slot]); if the profile has no save block, return. Then for
two record sets -- the profile's own `(*(*prof)->save)->rec[track]` and the global `D_80150F88[track]` -- insert
the car's finish time into the 5-entry ascending `times[]` (shift down; for the global set also shift the
owner bytes D_80151AC0[n+9 -> n+10] (not track-indexed in retail) and the owner pointers
D_80151690[track].who[]), then bump races/u54, add Player.laps (u16 +0x40) to u4E, laps driven
D_80144018[i] to laps, sum this player's lap times D_80149A78[i][0..laps) into total, and add car distance
/ 528.0f to the u32 distance. Finally func_800CD8EC(prof, (u8)track2). track = D_8014978C (s8); with the
D_80152570 flag set, track += 6 and track2 += 19 (mirrored tracks in the records table).

```
us.sh assign_drones best.c -> FAIL assign_drones: 295 of 309 words differ; compiled body is 307 words, target 309 | words 29 ops 7 norm 7 | frame 136/136
```
Path (3 rounds): draft from the disassembly 179 words / 9 ops, frame 144 -> dropping the u8 copy of
track2 (it is a spilled CSE of `(u8)track2`, sp+64, read back with lbu 67) and indexing `D_8014A118[i]`
directly (no `pl` local; retail's s4 is the strength-reduced address) -> frame 136, 175 words ->
`track2 += 19; track += 6;` instead of `= track + N` -> 29 words (ring offset fixed).
Remaining (ops 7): retail loads D_8014978C into v0 and copies it (`move ra,v0`, `sw v0,128(sp)`, and the
+19/+6 read v0); ours loads straight into ra. Same for the car record address: retail computes it in v0
and copies it to s5 (`move s5,v0`, the field loads read v0), ours computes it in s4 -- which also swaps the
pl/car registers (s4/s5). Both look like "a returned value kept as its own web" (w15d), but static getters
for either (tested: get_track, get_car) add 8 bytes of frame each and make it worse (24 / 11 ops rows), and
there are no caller-less stubs near 0x800F4FEC. `track2 = track = D_8014978C` and `track2 = D; track = track2`
are worse. Unused: `time` IS a variable (removing it loses the hoisted $f2).
Trace (snapshot ad9 = best.c): `track` is w0 -> c13 (ra, home sp+132), `car` w44 (save 40.0) -> s4, the
strength-reduced player address w19 (save 36.2) -> s5. Retail has them the other way round.
`force.sh ad9 assign_drones p1:w44=c19,p1:w19=c18` -> `want 309 words, got 307; differing rows 17 (words)`:
the s4/s5 swap is 12 of the 29 words; the other 17 are the two missing v0 copies (structural: retail's
loaded track value and car address are webs of their own, copied into the variables).
Best next: one lever may do both -- in retail `car` is defined by a copy, so its web starts later and has
lower priority than the player address. Look for the source form that makes the car address an expression
web first (e.g. the address computed in the `if (j == 0)` arms, or used before the assignment).

Recovered layouts (offsets proven by the retail loads/stores):
- Record (0x60): times[5] f32 +0x2C, total f32 +0x40, laps u16 +0x44, races u16 +0x46, u16 +0x4E,
  u16 +0x54, dist u32 +0x58. Records start at +0x8C of the save block; global copy D_80150F88[] (stride 0x60).
- Profile: s32 valid +0x08, SaveBlock ** +0x2C. D_80146150[] = Profile * per controller slot.
- Owner table D_80151690[] (stride 0x3C): Profile ** who[5] at +0x28. Owner bytes D_80151AC0[10..14].
- Player (D_8014A118[], stride 0x4C): u8 car +0, u8 slot +1, u16 laps +0x40, Profile ** prof +0x48.
- CarData (D_80152818[], 0x3B8): s8 finished +0xEF, f32 time +0xF0, f32 dist +0x108.
- D_80144018[] u8 laps per player; D_80149A78[][8] f32 lap times per player.

## What generalises
- Two-spelling float constants again decided func_800CF06C (retail held 3.0 in two registers and 0 in two):
  read the retail FP registers per constant before writing literals (wave 17 rule, confirmed).
- `asm.sh` with hand-inserted labels is a cheap proof that a residual is an as1 block boundary
  (world_effect_update: two labels -> 0 rows). The source construct that makes the boundary is still open.
- `x += N` vs `x = y + N` in a two-variable init moved assign_drones' whole temp ring (175 -> 29 words).
- A spilled `(u8)var` CSE read back with `lbu 3(slot)` is not a local; dropping the named u8 copy fixed the frame.
