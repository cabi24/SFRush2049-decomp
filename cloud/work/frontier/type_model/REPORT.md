# Type and data model survey — game image (2026-10-04)

Inputs: `build/game_code.bin` (647,072 bytes, base 0x80086A50), `build/blob_layout.json` (1,216 functions), `blob_matched.lock.json` (676 matched), `src/blob/**`, `reference/repos/rushtherock/game/*.h`. Every number is produced by a script in this directory; the `*.out` file next to each script is its captured output.

Claim marking: **[T]** tested (script + output named), **[I]** inferred from tested facts.

Population: 540 unmatched functions = 457,224 bytes; 676 matched = 103,784 bytes.

## 0. Method and its limits

`scan_refs.py` decodes every function linearly (no CFG) and tracks per register: `lui` halves, full addresses (`lui+addiu/ori`), address-plus-scaled-index (stride recovered from `sll/addu/subu` chains), and pointers loaded from an absolute address. Outputs:

- `refs.json`: 18,304 absolute references
- `ptr_refs.json`: 1,367 field accesses through a pointer global
- `regoff.json`: 14,919 register+offset accesses with an unknown base

Known under-counting **[T by example]**: `base + (idx*S + const)` is not folded (`func_800E4B58` forms `&game_car[n] + 0x314` this way), register state is lost across calls, and anything reached only through arguments is invisible to the absolute scan. Offset sets for arrays are lower bounds.

Reference totals **[T: scan_refs.out]**:

- 13,523 refs to addresses after the image (BSS), 2,013 distinct addresses
- 4,047 refs into image data, 1,684 distinct addresses
- 489 refs into the static segment
- 859 of 1,216 functions make at least one absolute reference; 469 of 540 unmatched functions reference BSS

## 1. Data segment 0x8010FD7C..0x801249F0 (85,108 bytes)

### 1.1 It is two sections **[T: classify_data.out, rodata_order.out, data_map.out]**

| range | bytes | what | ordering |
|---|---|---|---|
| 0x8010FD7C..0x80123870 | 80,628 | `.data` (initialised data and all string literals) | per-TU blocks, TU order differs from `.text` |
| 0x80123870..0x801249F0 | 4,480 | `.rodata` (float literals + switch jump tables only) | strictly `.text` function order |

`.rodata` starts 16-aligned right after the last `.data` strings (`"Start"`, `"Extra"` at 0x80123850/60). Its first word is `func_80086A50`'s jump table; its last words are floats of `func_8010EA08`, then 8 zero bytes.

### 1.2 Byte breakdown

`.rodata` (4,480) **[T: rodata_owners.out]**

| class | bytes |
|---|---|
| jump tables (words pointing mid-function) | 1,896 |
| float literals referenced by `lwc1` | 2,520 |
| float literals with no detected reference (14 words) | 56 |
| zero padding | 8 |
| doubles | 0 |

`.data` (80,628) **[T: data_map.out]**

| class | bytes | % |
|---|---|---|
| zero | 24,156 | 30.0 |
| ASCII strings | 15,540 | 19.3 |
| small integers | 15,068 | 18.7 |
| other (packed shorts/bytes etc.) | 10,116 | 12.5 |
| float-like (unreferenced) | 6,620 | 8.2 |
| pointers to image data | 6,312 | 7.8 |
| function pointers | 2,040 | 2.5 |
| floats referenced by `lwc1/swc1` | 764 | 0.9 |
| pointers to BSS | 12 | 0.0 |
| jump tables | 0 | 0 |

"float-like", "smallint" and "other" are heuristic **[I]**; pointer, string, jump-table, zero and referenced-float classes are exact.

Side finding **[T: one-off inline dump of the opaque runs]**: the other 10 "opaque" runs in `blob_layout.json` (956 B) are code, not data. Nine are `jr ra; nop` stubs. The 860-byte run at 0x801046FC is a stub followed by a real prologue (`addiu sp,-0x88; lui s4,0x8014`).

### 1.3 Address-ordered map of `.data` **[T: data_map.out]**

Objects are delimited by referenced addresses and data-pointer targets: 2,353 objects, of which 1,007 are directly code-referenced and 1,346 are reached only via pointers (22,888 B). Median object is 12 B; 39 objects of 256 B or more hold 41,422 B.

| range | approx | content | referencing functions |
|---|---|---|---|
| 8010FD80..80110290 | 1.3 K | 576 B record table, 524 B pointer table at 80110030, HUD scalars | `func_800D5E64`; 800C84FC..800D7634 |
| 80110290..801108C8 | 1.6 K | small tables | `func_800D6914`..`func_800D7634`; `func_800E8CB8`..`func_800EB90C` |
| 8011196C..80113E80 | 9.5 K | 0x440-stride tables, 3,272 B table, two fn-pointer tables | `func_800D816C`..`func_800D91A0` only |
| 80113E80..80114D08 | 3.7 K | many small TUs; game-loop state 801146C4..F8; 880 B float table | 800B61FC..800FD464 |
| 80114D08..801158DC | 3.0 K | one 3,028 B table with fn pointers | `func_8008BEA4` (matched) |
| 801158DC..80116198 | 2.2 K | effects tables | 800B7A40..80108154 |
| 801164BE..80116DBC | 2.3 K | menu tables | `func_800DA2C0`..`func_800DE45C` |
| 80116E06..80117314 | 1.3 K | audio tables | 800B3B4C..800B4FB0 |
| 80117498..80117518 | 0.1 K | state scalars (0x801174B4 `gstate`) | 99 functions |
| 8011755C..80118C10 | 5.8 K | 1,488 B camera table; 4,324 B table | 3 camera funcs; `entity_tick_main` |
| 80118C10..80118E37 | 0.5 K | 500 B curve tables; dispatch scalars | `engine_torque_calc`; `dispatch_handler` |
| 80118E38..8011A924 | 6.9 K | pointer-linked records, mostly not code-referenced | few |
| 8011A924..8011B5BA | 3.2 K | particle/audio/entity tables | 800B59F0..800BB9B0; 800B15B4..800B338C |
| 8011B898..8011E75C | 12.0 K | six tables of about 1.9 KB | `physics_float_calc` only |
| 8011E76C..8011EAAC | 0.8 K | car tables | 800A5908..800AB18C |
| 8011EAEC..8011F0B0 | 1.5 K | two u16 tables (512 B, 516 B), pad state | 800A150C..800A1644; `Input_ProcessGameplayPad` |
| 8011F0B0..80123564 | 17.6 K | string-dominated, small tables between | 92 functions, TU-clustered, not text order |
| 80123564..80123870 | 0.8 K | 732 B int table; "Start"/"Extra" | `func_80097164`; maxpath/display |

The 67 TU-like clusters and every object of 512 B or more are in `data_map.out`.

### 1.4 Is the layout per-function?

`.rodata`: **yes** **[T: rodata_owners.out, rodata_order.out]**.

- Of 1,104 owned words, the owner's address is non-decreasing with 1 exception (0x801243CC, a stale-`lui` artefact **[I]**).
- 1 of 679 referenced rodata addresses has more than one referrer (the same artefact).
- 214 functions own rodata: 53 matched (372 B tables + 416 B floats) and 161 unmatched (1,524 B tables + 2,104 B floats).
- The 161 unmatched owners are **208,328 of 457,224 unmatched bytes (45.6%)**: 29 own jump tables (45,276 B) and 132 own floats only (163,052 B).

`.data`: **no** **[T: data_map.out]**.

- Order violations against text order: 147/149 in 0x8010FD7C..0x8011EC00 and 78/84 in the string region.
- It is TU-contiguous: of 710 single-owner objects, 72.5% of address-adjacent pairs have owners within 15 functions (median distance 0), and 569 fall into 67 clusters.
- `.data` was linked in a different object order from `.text` **[I]**.
- 317 objects have two or more referrers, so functions cannot own `.data`.

### 1.5 How matched functions get away with it

**[T: code read of `blob_splice.link_function`, `blob_group._local_data_bases` / `_jump_table_windows`, `tools/cloud/score.py`; counts from rodata_owners.out]**

- **Single path (`link_function`).** Links only `.text.<target>` and resolves names via `PROVIDE(D_XXXXXXXX = addr)`. The object's own `.rodata` is never placed, so literals are spelled as fake externs: `extern f32 D_80123884; … if (value < D_80123884)` (`src/blob/func_8008B3F4.c`).
- **Group path (`blob_group`).** `_local_data_bases` derives one image base for the object's `.rodata/.data` from the retail HI16/LO16 words and requires byte equality with the image. If sites disagree, `_jump_table_windows` places each table independently and checks every entry. `.bss` is refused.
- **`score.py`.** Section-relative relocations are "unverified" and need `--allow-unverified`; named `D_` symbols resolve via `asm/us/blob/symbols.json`.

Of 53 matched rodata owners: 22 single-path with fake externs, 18 group-path with fake externs, 12 group-path with native jump tables, 1 group-path with native floats. No single-path match has a switch. 678 rodata addresses are already declared as `D_` externs.

## 2. Global base addresses

### 2.1 Arrays and formed bases **[T: cluster_bases.out, array_extent.out, bases.json]**

"unm. bytes" is the total size of unmatched functions touching the entity; rows overlap. N for rows 1, 2 and 7 is evidenced by a formed end pointer plus the next element's offsets ceasing to match. Other N are guesses.

| # | base | kind | stride | N | funcs | unm. | unm. bytes | offsets | max off | current name / hand types |
|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 80152818 | array | 0x3B8 | 6 | 135 | 101 | 139,288 | 142 | 0x3B4 | `gPlayerCarState1`/`game_car`/`player_array`; `GameCar` (4 fields); hand `CarRecord[]`, `B952[]` |
| 2 | 8014A250 | array | 0x808 | 6 | 100 | 76 | 104,608 | 161 | 0x804 | `gTrackDataB`/`track_data` (wrong, see 3.2); hand `Car[]`, `Model2056[]`, `CarState[]` |
| 3 | 8012E700 | array | 0x44 | ? | 69 | 44 | 64,848 | 23 | 0x40 | none; hand `Ent[]`, `Object68[]`, `IndexedRecord44[]`, `ModelSlot[]`, `TrackNode[]`, `Model68[]`, `Record[]`, `RecordSlot[]` |
| 4 | 8014A114 | array | 0x4C | ≥2 | 68 | 52 | 69,428 | 16 | 0x48 | `input_rec0/1`, `InputRecord`; same stride at 8014A0C8 |
| 5 | 8017A4E0 | pointer block | - | - | 47 | 46 | 57,628 | 12 | 0x24 | `countdown_state` |
| 6 | 801526A8 | array | 0xC | ? | 40 | 30 | 47,744 | 3 | 0x8 | hand `V3[]` |
| 7 | 80144030 | array | 0x304 | 4 | 39 | 29 | 24,320 | 28 | 0x104 | `player_slots`; hand `CarSlot[]`, `Car[]` |
| 8 | 80151FC8 | array | 0x78 | ? | 39 | 29 | 43,236 | 43 | 0x74 | none (same stride at 80140808) |
| 9 | 80150B70 | header | - | - | 29 | 23 | 37,532 | - | - | `gEffectEmittersArray`; hand `Control152[]`, `u8[][0x98]`, `Fx[]` |
| 10 | 801407F0 | struct | - | 1 | 28 | 17 | 21,004 | 4 | 0xC | hand `PathGraph`, `Header`, `Graph`, `PGraph` |
| 11 | 80151CE8 | struct | - | 1 | 26 | 18 | 21,984 | 5 | 0x8 | hand `TrackRecord[]`, `Section[]`, `Track812`, `Path` |
| 12 | 80151CF4 | array | 0x50 | ? | 25 | 16 | 20,188 | 15 | 0x4C | none |
| 13 | 80146108 | struct | - | 1 | 24 | 20 | 26,432 | 41 | 0x29 | `D_80146108_Record` |
| 14 | 80153E88 | struct | - | 1 | 24 | 24 | 32,780 | 8 | 0x7 | hand `Slot[]` |
| 15 | 80150B7C | array | 0x98 | ? | 22 | 17 | 29,520 | 22 | 0x88 | `gSoundIndexCurrent` |
| 16 | 80156D38 | array | 0x14 | 64 | 20 | 17 | 7,816 | 8 | 0xC | hand `Voice[64]`, `ResSlot[64]` |
| 17 | 8011753C | array | 0x30 | ? | 18 | 12 | 12,672 | 12 | 0x20 | (.data) camera table |
| 18 | 80139320 | array | 0x40 | 52? | 16 | 12 | 17,952 | 24 | 0x3C | hand `PlayerRec[]` |
| 19 | 80156CF0 | array | 0x10 | 4 | 15 | 15 | 17,048 | 8 | 0xD | `gPlayerStatusArray`; hand `Slot16[]`, `PadState[]` |
| 20 | 8017A510 | array | 0x48 | 4 | 15 | 11 | 14,172 | 22 | 0x47 | hand `Record[]` |
| 21 | 80149B68 | struct | - | 1 | 13 | 11 | 10,452 | 5 | 0x8 | none |
| 22 | 80140BF0 | array | 0x20 | 4? | 12 | 5 | 4,580 | 15 | 0x1E | `input_buffer`/`pad_config`, `PadConfig` |
| 23 | 80118E28 | struct | - | 1 | 11 | 9 | 12,356 | 9 | 0x8 | (.data) |
| 24 | 801569B8 | array | 0x7C | 4 | 10 | 6 | 7,192 | 22 | 0x54 | `gObjectSlotTable`; hand `Ply[]` |
| 25 | 80156BE0 | array | 0x80 | 2 | 5 | 5 | 4,176 | 23 | 0x7C | in game_types.h |

Stride census: 0x3B8 in 79 functions, 0x808 in 64, 0x44 in 41, 0x4C in 34, 0x304 in 30, 0x98 in 15, 0x78 in 9.

### 2.2 Scalars dominate reach **[T: cluster_bases.out, cover_all.out]**

| address | funcs | unmatched | name today | note |
|---|---|---|---|---|
| 801174B4 | 86 | 77 | `gstate` | bitmask word |
| 8014A108 | 80 | 63 | `active_player_count` | s16 |
| 8014A110 | 79 | 65 | `gTrackDataA`/`gameplay_mode` | s32; "track data" name is wrong |
| 801461D0 | 68 | 66 | `gMainGameStruct` | declared `OSMesgQueue` by 4 matched sources |
| 80151AD0 | 56 | 51 | (game_types.h) | |
| 8014978C | 52 | 40 | none | |
| 80142728 | 43 | 27 | `gRaceControlData` | |
| 80152770 | 38 | 29 | `gPlayerCarState2` | used as `OSMesgQueue` in matched lock wrappers |

Dense scalar blocks, by unmatched functions touched:

- 8014A0F8..8014A110: 112 functions, 161,816 B
- 80117498..801174E4: 86 functions, 120,336 B
- 80151ABC..EC: 68
- 80154390..D4: 54
- 80156940..B0: 54
- 80149770..A8: 53

### 2.3 Pointer globals **[T: ptr_refs.json; summary in the scan run]**

| pointer | funcs | unmatched | unm. bytes | offsets | max off | reading |
|---|---|---|---|---|---|---|
| 8017A4E4 | 40 | 39 | 50,864 | 87 | 0x3B0 | all word accesses; a word/pointer table of about 0x3B4 bytes, not the car |
| 8017A4EC | 27 | 27 | 37,112 | 25 | 0x46C0 | 24 of 25 are `lhu`; large u16 record |
| 801497F0 | 14 | 7 | 4,156 | 11 | 0xC | small node |
| 80149438 | 11 | 9 | 16,452 | 2 | 0x4 | `gDisplayListPtr` |

## 3. Arcade correspondence

Arcade layouts were computed: `arcade_layout.c` (25 arcade headers) compiled with `mips-linux-gnu-gcc -mabi=32 -g` and flattened by `arcade_layout_gdb.py` into `arcade_layouts.json` (37 types). Sizes: `MODELDAT` 0xA6C, `CAR_DATA` 0x40C, `Car` 0x278, `RECKON` 0x11C, `MPCTL` 0x30, `Visual` 0x1C, `MPATH` 0x18.

### 3.1 Existing function names are not evidence **[T]**

`MP_TargetSpeed` and `assign_default_paths` in `src/blob/groups/codex_heap_release_a25/group.c` are matched bodies of the form `if (flag) { osRecvMesg(&D_80152770,…); audio_reverb_update(…); osJamMesg(…); flag = 0; }`. They are unrelated to arcade `maxpath.c`.

Anchor method **[T: float_fingerprint.out]**: 51 non-`lui` float literals are common to N64 rodata (183 distinct) and arcade `game/*.c` (307 distinct); 19 are rare on both sides.

- `3.125e-05` and `1.05`, both unique to `maxpath.c`, are owned by `func_800E4B58` (unmatched, 2,268 B).
- `0.925` (drivetra.c) → `func_800E2F00`
- `90000` (drivsym.c) → `func_800E398C`
- `1.3` (cars.c/drivsym.c) → `func_800D91A0`

### 3.2 MPCTL and MODELDAT in `func_800E4B58` **[T: disassembly]**

```
lh    t6,0x7C6(a0)            ; m->net_node
lui/addiu t9 = 0x80152818     ; game_car
t7 = t6*0x3B8 ; t8 = t7+0x314 ; s6 = t8+t9   ; cp = &game_car[node] + 0x314
lwc1  0(s6), 0xC(s6)          ; xrel, len
lwc1  4(s6)                   ; yrel
swc1  0x10(s6)                ; tgtspd (clamped against 28.0)
swc1  0x14/0x18/0x1C(s6)      ; tgtpos[0..2]
lh    0x24(s6), 0x28(s6)      ; mpi, mpath_index -> jal 0x800B9338
lh    s7,0x7C6(t7) ; jal 0x800E398C          ; avoid_areas(m->net_node)
```

| field | arcade | N64 | verdict |
|---|---|---|---|
| `CAR_DATA.mpath` | +0x374 | +0x314 | present, moved −0x60 |
| `MPCTL.xrel, yrel, cyrel, len, tgtspd, tgtpos[3]` | +0x04..+0x20 F32 | +0x00..+0x1C F32 | same order and types, shifted −4 |
| `MPCTL.mpi` | +0x00 S32 | +0x24 S16 | moved behind the floats, narrowed |
| `MPCTL.mpath_index` | +0x2A S16 | +0x28 S16 | kept |
| N64 +0x20 | (`interval_time` S32 at +0x24) | float store from `D_801543CC` | type changed **[I]** |
| `MODELDAT.net_node` | +0xA36 S16 | +0x7C6 S16 | present near the tail; struct 0x264 smaller |

The 0x808-stride array at 0x8014A250 is the N64 `MODELDAT model[6]`, not track data **[I]**. The car element is the N64 `CAR_DATA` (0x3B8 against 0x40C).

### 3.3 Car array against `CAR_DATA` **[T: arcade_compare_car.out]**

141 typed N64 offsets. Type agreement is 63/141 at shift 0 and 73/141 (0.52) at the best single shift (+0x18), against a chance baseline of 0.18.

- **+0x000..+0x0E4:** an unbroken float run where arcade has `dr_pos … tireW`. Kind preserved; individual vectors are not identified by type alone.
- **+0x0F8..+0x0FE:** four `lh` fields where the arcade S16 quad `crashflag, rpm, engine_type, body_type` sits after a 0x18 shift. Good anchor.
- **+0x124..+0x1B4:** a 0x18-byte period (word at +0, `sh` at +8/+A/+C, six or more repeats) where arcade has `Visual visuals[20]` of 0x1C bytes. Restructured.
- **+0x314..+0x33C:** `MPCTL` per 3.2.
- **+0x350..+0x3B4:** does not line up with the arcade tail at any single shift.

Verdict: same lineage, coarse field order preserved, arcade offsets cannot be transcribed. Each sub-block needs its own anchor.

### 3.4 Others **[T: arcade_matrix.out]**

Type-pattern scoring does not discriminate for the rest. 0x8014A250 against `MODELDAT` scores 0.60 over a 0.48 baseline, because 105 of 161 offsets are floats. 0x80144030, 0x8012E700, 0x80151FC8 and 0x8014A114 have no arcade type of similar size or pattern among the 37; treat them as N64-specific **[I]**.

### 3.5 Pointer-based structs in unmatched code **[T: ptr_structs.out]**

Register+offset accesses with a non-absolute base: 14,919 total, 10,723 in unmatched functions. `build/m2c_histogram.json` is an m2c outcome histogram, not an offset histogram, so this was measured directly.

- `lh 0x7C6(ptr)` appears in 35 functions, 27 unmatched (41,192 B). Recurring fields in that family: floats +0x124/128/12C, +0x220/224/228, +0x3F0, +0x794/798/79C; bytes +0x640, +0x730, +0x7CC; shorts +0x3F8, +0x6C4, +0x7E2/7E4; word +0x7D4.
- By largest pointer offset: 52 unmatched functions (69,488 B) top out in 0x400..0x80F, 37 (42,496 B) in 0x100..0x3B7, and 43 (57,832 B) at 0x810 or beyond.
- `lb 0x35C/0x35D(ptr)` appears in 11 and 8 unmatched functions, and `lw 0x3A4(ptr)` in 8. This is consistent with the car element reached by pointer **[I]**.

## 4. Duplication in matched sources **[T: source_dup.out, decl_overrides.json]**

622 C files (543 single, 79 group). 402 single files exceed 50 KB because each embeds the same generated context prelude (median 104 KB; 42.4 MB total).

| measure | value |
|---|---|
| struct/union typedef occurrences | 21,186 |
| distinct typedef names | 233 |
| distinct bodies | 324 |
| names from the shared prelude (≥300 files) | 48 (9 with a variant body) |
| hand-written typedef names | 185, with 261 distinct bodies |
| hand-written names with more than one body | 37 |
| hand-written names used by exactly one file | 120 |
| identical body under different names | 3 |
| `D_` addresses declared anywhere | 2,315 (BSS 892, .data 684, .rodata 678, static 53, code 8) |
| with a generated default declaration | 2,185 |
| hand-declared only | 130 (16 conflicting) |
| hand override of the default | 266 |
| two or more mutually different hand types | 61 |
| struct-typed by at least one hand source | 94 |

Conflicts concentrate on the section-2 bases: `D_8012E700` has 9 hand spellings, `D_80150B70` 7, `D_80151CE8` 4, `D_801407F0` 4, `D_8014A250` 3, `D_80152818` 3.

## 5. Storage outside the image **[T: refs.json; ownership from file reads]**

The image ends at 0x801249F0. Referenced ranges beyond it:

| range | span | addrs | funcs |
|---|---|---|---|
| 801249F0..80124FE8 | 0x5FC | 37 | 24 |
| 8012E5E8..8012E758 | 0x174 | 58 | 128 |
| 80138660..8013A028 | 0x19CC | 68 | 60 |
| 8013C068..8013C378 | 0x314 | 25 | 16 |
| 8013E5D8..80141428 | 0x2E54 | 132 | 143 |
| 801424F0..80146210 | 0x3D24 | 321 | 290 |
| 80149410..8014D280 | 0x3E74 | 351 | 336 |
| 80150B60..801572BC | 0x6760 | 876 | 457 |
| 8015B248..8015B9D4 | 0x790 | 25 | 23 |
| 8015F728.., 80161360..80161498 | small | 48 | 54 total |
| 801749C4.., 8017A498..8017A640 | small | 53 | 80 total |
| 8038A400, 80394030.., 80399AF8.., 803B9A50.. | sparse | 15 | 14 total |

Game BSS spans about 0x801249F0..0x8017A640 (roughly 350 KB) with large unreferenced gaps. The 0x8038A400+ addresses are runtime image B's load area. Static-segment data used by game code: 56 addresses, 489 refs, 97 functions, mostly 0x8002AFA0..0x8002B024 (133 refs) and 0x8002E4A0..0x8002ECF8 (298 refs, scheduler).

Ownership today: none for game storage.

- `rom_owned_data.json`: 5 ROM slots (464 B) and 3 storage blocks (`vi_manager` 0x1220, `timer_services` 0x40, `sdk_initialize` 0x10), all static-segment.
- `rom_owned_storage.ld`: NOLOAD sections for those 3 blocks only.
- `src/blob/blob.ld`: places 1,216 `.text.*` and 11 `.blobdata.op_*` inputs and PROVIDEs 4,466 absolute symbols.
- `blob_group._local_data_bases` refuses `.bss` relocations.

## 6. What this implies

### 6.1 Structs touching the most unmatched bytes **[T: cover_structs.out]**

| pick | entity | unm. funcs | bytes touched | new bytes | cumulative |
|---|---|---|---|---|---|
| 1 | car array 0x80152818 (0x3B8 × 6), embedded MPCTL at +0x314 | 101 | 139,288 | 139,288 | 30.5% |
| 2 | pointer block 0x8017A4E0 and its pointees (0x8017A4E4, 0x8017A4EC) | 46 | 57,628 | 42,560 | 39.8% |
| 3 | 0x8012E700 (0x44 records) | 44 | 64,848 | 32,664 | 46.9% |
| 4 | 0x8014A114 input records (0x4C) | 52 | 69,428 | 26,976 | 52.8% |
| 5 | 0x80151FC8 (0x78 records) | 29 | 43,236 | 14,128 | 55.9% |
| 6 | 0x80144030 player slots (0x304 × 4) | 29 | 24,320 | 11,764 | 58.5% |
| 7 | MODELDAT array 0x8014A250 (0x808 × 6) | 76 | 104,608 | 10,940 | 60.9% |
| 8–10 | 0x80149B68, 0x801407F0 path graph, 0x801526A8 vec3 array | 11/17/30 | - | 26,456 | 66.7% |

`MODELDAT` ranks 7th only by marginal gain. It touches 104.6 KB, nearly all overlapping the car's functions, so the two must be recovered together. With scalars included (`cover_all.out`), the two blocks at 0x8014A0F8 and 0x80117498 touch 46.2% and 15 picks reach 84.2%.

### 6.2 Can functions own their rodata?

Yes for `.rodata`: strict function order, single owner per word, already proven by 13 group-path matches. No for `.data`: it is TU-ordered in a different link order with 317 shared objects, so it must be owned per TU or left opaque behind typed externs.

The gap is the single-function path, which has no rodata placement. 29 unmatched switch functions (45,276 B) need the group path or a fix. 132 float-only functions (163,052 B) can proceed with fake externs.

### 6.3 Recommended order of work

1. **Re-anchor before naming.** Flag `MP_TargetSpeed`, `assign_default_paths`, `gMainGameStruct`, `gPlayerCarState2` and `gTrackDataA/B`. Pair arcade and N64 functions by float fingerprints plus rodata ownership; the 19 rare literals already give about 15 pairs.
2. **Rodata ownership in the single-function path.** Port the `_local_data_bases` / `_jump_table_windows` verification into `link_function`, and have `score.py` verify section-relative relocations against the per-function window in `rodata_owners.json`.
3. **Shared header, scalars first.** Seed from the 2,185 generated defaults plus the 266 hand overrides (`decl_overrides.json`) and resolve the 61 real conflicts by hand.
4. **Car and MODELDAT together**, anchored from `func_800E4B58`. Build sub-blocks from N64 access evidence (`bases.json` has per-offset access widths) and take arcade names only where an anchor exists.
5. **N64-specific records** in cover order: 0x8017A4E0 block, 0x8012E700, input records, 0x80151FC8, player slots. Each has 1–9 rival hand typedefs to merge.
6. **BSS ownership and per-TU `.data`** last. This needs `.bss` support in the group linker and TU boundaries from the 67 data clusters.

## Files

| script | output | purpose |
|---|---|---|
| `common.py` | - | loaders |
| `scan_refs.py` | `scan_refs.out`, `refs.json`, `ptr_refs.json`, `regoff.json` | address materialisation scan |
| `classify_data.py` | `classify_data.out`, `data_classes.json` | word classes for the 85,108-byte run |
| `rodata_order.py` | `rodata_order.out` | function-order test for a range |
| `rodata_owners.py` | `rodata_owners.out`, `rodata_owners.json` | per-function rodata ownership, matched spelling modes |
| `data_map.py` | `data_map.out`, `data_objects.json` | `.data` objects, clusters, TU contiguity |
| `cluster_bases.py` | `cluster_bases.out`, `bases.json`, `loose_scalars.json`, `scalar_blocks.json` | arrays, structs, scalars |
| `array_extent.py` | `array_extent.out` | element-count evidence |
| `ptr_structs.py` | `ptr_structs.out` | pointer-based access profile |
| `arcade_layout.c`, `arcade_layout_gdb.py` | `arcade_layouts.json` | arcade struct layouts (needs empty stub `Pro/*.pro` and `ieeefp.h` on the include path) |
| `arcade_compare.py`, `arcade_matrix.py` | `arcade_compare_car.out`, `arcade_matrix.out` | typed-offset agreement against arcade |
| `float_fingerprint.py` | `float_fingerprint.out` | shared rare float literals |
| `source_dup.py` | `source_dup.out`, `decl_overrides.json` | typedef and declaration duplication |
| `cover.py` (`--structs`) | `cover_all.out`, `cover_structs.out` | greedy cover of unmatched bytes |

Run order: `scan_refs` → `classify_data` → `rodata_owners` → `data_map` → `cluster_bases` → the rest. All are Python 3.9 standard library, run from that directory.
