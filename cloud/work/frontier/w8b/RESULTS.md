# w8b results (frontier wave 8, layer 1)

Builder scratch `watchman2:~/rush2049/scratch/frontier/w8b` (copied from `base`); unit tag `w8b`.
Tools: `tools/` (copies of w7d/w7a scripts retargeted to w8b; `full.py` default flags changed to `-O3`; note the
original `full.py` defaults to `-O2`). Sweeps are in each function's `sweep/` directory.

| Function | Bytes | State | Flags | Deliverable |
|---|---:|---|---|---|
| audio_occlusion | 276 | **strict MATCH** (also MATCH at -O2) | -O3 | `cloud/matches/audio_occlusion.c` |
| func_800A3640 | 204 | **provisional** (EQUAL in the unit with a stand-in caller) | -O3 | `w8b/func_800A3640/best.c` |
| ghost_race_setup | 440 | 16 of 110 words in the unit (14 aligned rows; was 91/110 in A119) | -O3 | `w8b/ghost_race_setup/best.c` |
| func_800BB02C | 276 | **strict MATCH** (also -O2) | -O3 | `cloud/matches/func_800BB02C.c` |
| func_800A150C | 312 | **strict MATCH** (-O3 only) | -O3 | `cloud/matches/func_800A150C.c` |
| func_800DC628 | 248 | 13/62 words (was 20/62); function-local static flag verified | -O3 | `w8b/func_800DC628/best.c` |
| func_800A3424 | 228 | **strict MATCH** (also -O2) | -O3 | `cloud/matches/func_800A3424.c` |
| func_800A43FC | 212 | **strict MATCH** (also -O2) | -O3 | `cloud/matches/func_800A43FC.c` |

There are five strict matches (1,304 bytes), one provisional (204 bytes) and two near-misses.
The five matches were scored together in the whole-program unit:
`python3 -m tools.conveyor.pipeline.blob_unit --tag w8b score audio_occlusion func_800BB02C func_800A150C func_800A3424 func_800A43FC --with cloud/matches/<each>.c … --neighbours`
gave
`5/5 equal`, `locked bodies that differ in this unit: 0`.

---

## audio_occlusion — strict MATCH

The historical label is wrong: this is the **controller-state reset**. It takes the pad lock
(`osRecvMesg(&D_801497A8, 0, 1)`), clears the three combined pad masks and, for each of the four pads,
the stick vector and four per-pad masks, then releases the lock with `osJamMesg(..., 0, 0)`. It is the same block
`controller_poll` runs in its skip-frames branch. No arcade ancestor.

Command: `tools/sc.sh cloud/matches/audio_occlusion.c audio_occlusion --flags "-g0 -O3 -mips2 -G 0 -non_shared"` →
```
audio_occlusion:
  MATCH
```
Unit: `EQUAL audio_occlusion: 69 words (kept, c_audio_occlusion.c)`.

The source is shaped by two quirks, both visible in the code:
1. The scalars are written as **`D_8015694C = D_80149784 = D_80156944 = 0;`**. The chained assignment is what puts
   `&D_80156944` and `&D_80149784` into v0/v1 (`lui/addiu; sw zero,0(v0)`). Separate statements give `lui at` (the
   B46 near-miss stopped at 46/69 for this reason).
2. **`f32 D_80156958[4][2];` is defined (tentative/common) in this file.** With a definition, as1 shares one
   `lui at` between the two `swc1` stores of each pad. With an `extern` declaration (array or struct), every store
   gets its own `lui`. The scorer resolves the common symbol normally (strict MATCH). **For the integrator:** the file
   defines a common symbol `D_80156958` (32 bytes). Check that `blob_splice.link_function` resolves it to the
   image address rather than allocating it. The PLAYBOOK precedent is `D_801551E8` for `func_800B7438`.

Same-shape sibling: `controller_poll` (w1g near-miss, unassigned here). It has the same clear block, and its
`swc1` pairs also share one `lui`. Probe: defining `D_80156958` in w1g's group.c moved it from 124 to 120 aligned rows,
and its body from 234 to 230 words (retail 232). See `w8b/controller_poll_probe/`.

## func_800A3640 — provisional (stand-in caller)

The function frees every object on list `index`. The list headers are `D_80144D60[]` (16-byte `List`, as in the locked
`func_80091FBC`/`func_8009211C`), and an object is reached through `Handle->object`. For each object it calls the
object's callback (+8) if set. If it holds an allocation (+72), it frees it under the heap lock
(`osRecvMesg(&D_80152770)` / `audio_reverb_update(allocation, 0)` / `osJamMesg`, the same as `synced_model_render`)
and clears the field. It then removes the handle from list `object->list` (+16, u8) and pushes it onto the free list
`D_801460E0` (insert before head). No arcade ancestor was found.

Classification: it uses s0 and s2-s7 without saving them. It is an IPA callee of the unmatched
`track_render_process`, which has three call sites, so it cannot be landed alone.

Command: `python3 -m tools.conveyor.pipeline.blob_unit --tag w8b score func_800A3640 --with cloud/work/frontier/w8b/func_800A3640/best.c --internal func_800A3640 --keep w8b_standin`
→ `EQUAL func_800A3640: 51 words (internal, c_best.c)`.
`best.c` contains the stand-in `w8b_standin`, which makes three calls and keeps nine s-registers live. It **cannot be
spliced**. The one shaping fact: the allocation is a **block-scoped local read inside the `if`**
(`if (o->allocation) { u32 allocation = o->allocation; …`). Retail's `lw v0,72; …; move s0,v0` comes from that
local. Without it, the load goes straight to s0 (5 words off).
Calling `synced_model_render()` instead is not inlined, so the source is not a kept wrapper.

## ghost_race_setup — 16 of 110 words (unit), 14 aligned rows

The logic comes from A119 (`cloud/work/ipa-groups/codex_ghost_a119`). `menu_transition` is now locked (w7c), so the
function scores in the real unit with no stand-ins. It must be scored in the unit:
`audio_reverb_update` takes register parameters, and the standalone score is 89/110.

Command: `python3 -m tools.conveyor.pipeline.blob_unit --tag w8b score ghost_race_setup --with cloud/work/frontier/w8b/ghost_race_setup/best.c`
→ `FAIL ghost_race_setup: 16 of 110 words differ`.

What moved it (91 → 47 → 26 → 16):
- `D_80114738` is `volatile s8` (single `la; sb` stores).
- The test reads the slot twice: **`if (D_80152698[i] != 0) { handle = D_80152698[i]; D_80152698[i] = 0; …`**.
  This gives retail's `lw v0` test followed by `move a0,v0`, and it computes the player address before the state test.
  Array indexing rather than a `cursor` pointer also lets the count `D_8014A108` stay in a1 on the `state == 0` path,
  because a store through `*cursor` kills the count.
- There is no named `allocation` local in the release path. The local version is 21 rows; see below.

Residual (lane: as1 scheduling plus one PRE placement):
1. Retail has `move a0,v0; sw zero,0(s4)` and reads `handle->data` through a0. Ours hoists the store and reads through
   v0. ugen's listing is the same whatever the order (tested with listing edits in `asmfn.sh`), so this is an as1
   decision.
2. On the ready path, retail branches to the release path's trailing count reload (`b 0x158`). Ours has its own reload.
   The listing edit (`ghost_race_setup/listing_edits/e2.py` on `t1_fn.s`; one shared reload label) reproduces the retail branch shape. A
   compiled-out `if` after the ready/release `if-else` made it much worse (50+ rows).
3. Retail loads `allocation` into s0 in the `beqzl` delay slot. That needs the local, but the local makes as1 hoist the
   argument moves into the branch.

Tried without success:
- inlined-helper splits for the stub `func_800D52C4` (`sweep/s*.c`); a helper that stores `D_80140800` gets that
  address hoisted into s6;
- `>0`/`<0` chain orders (`sweep/r*.c`), line layouts (`sweep/l*.c`) and statement orders (`sweep/w*.c`, `q*.c`).

Next hypothesis: the traced as1 (`w6a/as1`) on `ul_t1` to find why the `sw` passes the `move`. Also, a release-path
spelling whose join block is real, e.g. `goto`, or the ready arm ending in a shared statement.

## func_800BB02C — strict MATCH

`func_800BB02C(index, kind, data)` initialises the 24-byte slot `D_8013FEE0[index]`: flag and active cleared, marker
255, value 0x8000. It then gives the slot a 512-byte buffer:
- `data` if the caller supplied one;
- otherwise, if the shared heap has less than 512 bytes free (`audio_output_setup(0)`), it shares the buffer of the
  first earlier node whose gLink entry has the same kind byte and copies that node's three payload bytes;
- otherwise `audio_dma_sync(0, 512)`.

The gLink entries are `D_80153E88`, 8-byte `Link`, as in `func_800EC914`. The caller is `audio_interrupt_handler`.

Command → `func_800BB02C:\n  MATCH`. Unit: `EQUAL func_800BB02C: 69 words`.

Shaping:
- **No `slot` pointer local.** Every access is `D_8013FEE0[index].field`, so the slot address is a spilled temp
  (frame 32). A named local adds 8 frame bytes (frame 40).
- The shared data pointer is copied **before** the payload bytes.
- **`if (i == index) {}` after the loop is compiled-out code.** It is retail's `bnel v1,a3 / b` pair at the loop exit.
  This is a direct sighting of the w3a "compiled-out debug code" rule.

## func_800A150C — strict MATCH (-O3)

This is the encoded-string-to-character-map converter, with logic from `cloud/work/encoded_string_mapping`
(1 word off). The remaining word was the operand order of `bnel t5,a1` in the byte-mode table search. uopt
canonicalises `x == y` so that a variable operand comes first, whatever the source order; variable types, casts and
`!(a != b)` were tried in `sweep/v*.c` and `w*.c`. **`if (D_8011EAEC[index] - code == 0)` keeps the table load
first.** This is a quirk spelling and is disclosed in the header. Command → `MATCH`. At -O2 it is 78/78 words off.

## func_800DC628 — 13/62 words (was 20/62)

This is the bit-buffer setup (the TU of `func_800DC1AC` (bit writer) and `func_800DC57C` (bit reader)). Once only, it
builds the inverse byte permutation `D_80116FE8[D_80116FE4[k]] = k` for k < 32. It then sets
`D_801170E8 = (bits0 + bits1 + 7) >> 3`, and returns 0 if that is at least 33 bytes or `bits1` is at least 33.
Otherwise it zeroes the buffer `D_8012E618`, stores the bit counts and position, and returns 1.

New facts:
- **The once-flag `D_801170F8` is a function-local `static s32 … = 1`.** The scorer reports
  `own .data verified at 0x801170F8`. The static removes the address register that an `extern` (or volatile) flag gets
  (`la a2; lw 0(a2); sw 0(a2)`; traced uopt: `ldaS` save 0.5 → 0).
- `i = 0` is assigned inside the `if`, after the flag clear.

Command: `tools/sc.sh func_800DC628/best.c func_800DC628 --flags "-g0 -O3 …"` → `13/62 words differ`.

Residual: 11 words are one colouring permutation, plus 2 words of as1 order (`addiu a2` / `lw v1`). Retail colours
size v1, `&D_801170E8` v0 and the clear pointer v0. The traced uopt (`runs/dc2`, `runs/dc3`) colours in web-number
order: size (web 29) takes v0 before the address and the pointer. In retail something already in v0 must overlap size
but not the address store, and `i` cannot do that. An `if (i) {}` after the size computation (`lead_if_i.c`, 7/62)
gives the size/pointer colours but moves the address to a2.

Tried without movement:
- all 24 declaration orders;
- size spellings (`sweep/s*`, `h*`);
- a pointer to the count;
- a struct view of `D_801170E8..F8`;
- volatile globals.

Next hypothesis: a result variable or a second value in v0 whose live range begins at the size store; or a
compiled-out use that ends between the chain's store and `andi`.

## func_800A3424 — strict MATCH

This is the Controller Pak file "in use" byte. `handle->object` gives the port (+16) and file (+17), as in
`func_800A1A60`. When the byte changes, a set marks the port as having active files (+10). A clear scans the 16
40-byte slots (+132), and if none is active it clears +10 and, unless +9 is set, the port's enabled byte (+1). The
caller is `playgame_state_change`. The logic comes from `cloud/work/game_C91` (46/57).

Command → `MATCH`, both -O3 and -O2. Unit EQUAL.

Shaping:
- **`extern volatile Pak772 D_80144030[]`.** Volatile gives retail's `beqzl …; b exit` scan loop and keeps the +10
  store before the +9 load. Making the three fields volatile (`alt_field_volatile.c`) also matches.
- No local for `handle->object`; one swaps v0 and v1.

## func_800A43FC — strict MATCH (first compile)

This tracks Controller Pak insertion. `D_80149440` is the SDK `OSContStatus[4]`; status bit 0 is `CONT_CARD_ON`,
bit 1 `CONT_CARD_PULL`. A pak that is on and not pulled marks `D_80144030[port]` +2 as inserted. A port whose pak is
gone is cleared and flagged changed (+3). With **both arrays volatile**, IDO unrolls the natural four-port loop by two,
which is retail's "pair" loop. The C33 and fresh-small attempts (51/53) used non-volatile data and never unrolled.
Command → `MATCH` (-O3 and -O2).

---

## What generalises

1. **The Controller Pak / SI records are volatile.** `D_80144030` (772-byte per-port pak records) and `D_80149440`
   (`OSContStatus[4]`) are declared volatile, as `dma_wait_complete` already does for its fields. That one fact closed
   func_800A3424 and func_800A43FC.
   - Its effects: volatile fixes `beqzl; b` loop shapes, keeps store/load order, and makes IDO unroll a small loop
     by 2.
   - Other unmatched readers of `D_80144030`: `track_render_process`, `func_800A3724`, `func_800A1BB4`,
     `func_800A1C6C`, `slot_state_lookup`, `track_collision_setup`, `MaxPathZeroControls`, `func_800E73D8` (also
     reads `D_80149440`), `func_800C813C`, `func_800D91A0` and others (`type_model/refs.json`).
2. **A file-scope array defined in the TU changes as1's `lui` sharing.** Consecutive stores into a *defined* (even
   tentative/common) array share one `lui at`; with `extern`, each store gets its own (audio_occlusion; also applies
   to controller_poll).
3. **A once-only flag is a function-local `static … = 1`.** An `extern`/global flag that is read and cleared gets an
   address register, and the static does not. The splice verifies the `.data` byte.
4. **Chained assignment of zeros** (`a = b = c = 0`) puts the inner targets' addresses in v0/v1 (w1g rule 8,
   confirmed). The order of the chain sets which address goes to which register.
5. **uopt canonicalises equality operands** (a variable before a memory load), so the source order of `==` is
   irrelevant. `a - b == 0` keeps `a` first. Look for this when one of two identical loops has swapped `bne` operands.
6. **Reading a global array element twice** (`if (A[i]) { h = A[i]; A[i] = 0; …`) gives "load to v0, then
   `move a0,v0`". A store through a cursor pointer kills unrelated globals for PRE; array indexing does not.
7. **Block-scoped local inside an `if`** for a value live across a call: `lw v0; move s0,v0` (func_800A3640).
   This confirms w7c rule 1.
8. **`if (i == index) {}` after a search loop** is retail's `bnel; b` exit pair (func_800BB02C), another compiled-out
   check.

## Global types recovered

- `D_80144030` `Pak772[4]` (volatile in pak/SI code): enabled +1, inserted +2, changed +3, retained +9,
  files_active +10, OSPfs +12, files[16] x 40 bytes at +132 (in-use byte +0).
- `D_80149440` `OSContStatus[4]`.
- `D_80144D60` `List[]` (16 bytes) object lists; `D_801460E0` the free-handle list; object: next +0, callback +8,
  list index u8 +16, allocation +72.
- `D_8013FEE0` `Slot24[]`: flag +0, active +16, marker +17, value u16 +18, data +20. `D_80153E88` gLink: state +0,
  kind +1, payload[3] +2.
- The controller block: `D_80156944`/`D_80149784`/`D_8015694C` combined masks, `D_80156958` `f32[4][2]` sticks
  (defined in the controller TU), `D_80149B10`/`D_80143A00`/`D_80156978`/`D_80156998` per-pad masks,
  `D_801497A8` the pad lock queue.
- The bit buffer: `D_801170E8` byte size (u16), `D_801170EC`/`F0` bit counts, `F4` position, `F8` static once-flag
  (=1), `D_8012E618` buffer, `D_80116FE4`/`D_80116FE8` permutation and its inverse.

## Integration notes

- The singles `audio_occlusion`, `func_800BB02C`, `func_800A150C`, `func_800A3424` and `func_800A43FC` are spliceable
  as written. For `audio_occlusion`, check how the splice handles its common definition of `D_80156958`.
- `func_800DC628`, if later closed, owns `.data` 0x801170F8 (the static flag).
- `func_800A3640` waits on `track_render_process`. Do not add it to coverage.
