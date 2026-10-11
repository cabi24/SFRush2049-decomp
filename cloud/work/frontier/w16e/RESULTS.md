# Wave 16, lane w16e: func_80096130, func_8010FBE0, entity_process_main

Status: final. No matches. All three are stopped under the stop rule, with the cause traced further.

| Function | Bytes | State | Flags | Scorer output (whole-program unit, `--neighbours`) |
|---|---:|---|---|---|
| func_80096130 | 264 | 3 of 66 words (spill home of index*8 only) | -O3 unit | `FAIL func_80096130: 3 of 66 words differ` / `locked bodies that differ in this unit: 0` (`f130/best.c`) |
| func_8010FBE0 | 128 | 6 aligned rows (unchanged) | -O3 unit | `FAIL func_8010FBE0: 30 of 32 words differ \| words 6 ops 6 norm 6 \| frame 24/24` (`fbe0/best.c` = w15d) |
| entity_process_main | 1424 | 2 of 356 words (unchanged) | -O3 unit | `FAIL entity_process_main: 2 of 356 words differ \| words 4 ops 0 norm 0 \| frame 280/280` (`epm/best.c` = w11a) |

Permission denials: none. Variants: `<fn>/v/`, about 110 in total.

## func_80096130 (264 B): 3 words; what retail's split has to look like is now pinned down

**Correction to w15c.** In the current unit, the plain source with no shaping (`w12e/f130_base.c`, `f130/v/base.c`)
scores **21** words, not 3. w15c's `f130/s1..s4,c1..c5` files have a duplicated `void func_80096130(s32 index)`
line, so they do not compile. The 3-word level needs the `if (slot) {}` read.
`f130/best.c` is the smallest shaping that reaches it: one `if (slot) {}` right after the store, with no `off` local
and no `if (0) {}`. It scores the same as w12e's best.c.

Trace (ctrace, force, spill on the snapshots `base`, `b7`, `b8`):
- **Plain base is already retail's split.** Web 5 (the slot address) has net 3 and bestcost 3 (v1 is caller-saved
  across osRecvMesg, audio_reverb_update and osJamMesg), so it splits. After the split, alloc (w7) takes v1 and the
  head remnant of w5 takes a3, and the tail after osJamMesg gets no piece, so it reloads into a ring temp (`lw t7,36(sp)`).
  The homes are already retail's (idx*8 at 32).
  `force.sh base func_80096130 "p1:w7=c6"` fixes the whole head. **16 rows remain, all of them the tail.** Retail
  reloads into **v1**, so in retail the tail piece after the split is a coloured web, and the ring temps after it
  shift by one.
- **Why the tail gets no piece.** A piece holding only the store (one use) has nothing to gain. A second code-free use
  in the tail (`if (slot) {}`) makes the tail profitable, but it also raises the whole web's net to 4 > 3, so the web
  is no longer split (best.c: v1, unsplit, wrong `.alias` and home). If the extra use comes after the first memset
  (b8), the web crosses a 4th call (bestcost 4) and splits again. The tail fragment (w37) then also crosses that
  memset and is split to memory.
- **Spill homes (spill.sh b7).** Retail's 32(sp) for idx*8 is temp 1 (the alloc web's home). So in retail the idx*8
  web shares a block with the slot web but not with the alloc web: there is a block boundary after alloc's last
  occurrence (the inlined `address` store at 56(sp)), and slot is live after it.
- Exhausted this wave (all measured in the unit):
  - 40-file matrix (`v/m/`): `if (slot) {}` × {`do {} while (0);`, `if (0) {}`} at 4 positions each. Results: 3, 11 or 21
    words. The compiled-out boundaries never changed a result.
  - Holders at 8 positions (`b*`) and idx*8 occurrences in the store block (`c*`, `if (index * 8)` folds to
    `if (index)` and drops `move v0,t7`).
  - A second definition of slot (`a*`, `h*`): recompute, `D_80156D38 + index`, `slot += index`, a `(u32)` launder.
    Every recompute or launder is CSE'd back into web 5. `slot += index` makes a real variable web: 24–41 words.
  - Calling the real non-static `audio_effect_process` instead of the stub helper: not inlined across files (52 words).
- **Best next hypothesis.** Read `f_split`'s region growth (`uopt.c` 155990–156090, `f_is_cup_affecting_regs`). The
  question is when a tail block that holds calls *after* the last use still yields a coloured piece. That would show
  whether retail's tail piece comes from a different block shape (for example memsets in a block of their own after
  a branch, which needs real control flow) or from a different weight. A `p1:wN=s1` "split once" force mode would
  also settle it. Source-level levers available without a new tool are exhausted.

## func_8010FBE0 (128 B): 6 rows; the integration side is clear, the source form is not

**What integration would have to change (nothing in tools).**
- Define the task in the function's file: `OSScTask D_80155238;` (0x58 bytes, 0x80155238–0x8015528F, zero-initialised
  `.bss`/COMMON). `blob_splice.splice` already handles this. `defined_data_symbols` picks up COMMON objects and
  binds them with an absolute assignment (`D_80155238 = 0x80155238;`) instead of PROVIDE. `blob_unit` verifies the
  extent as own `.bss` (inside `owndata.GAME_BSS`, no overlap).
  The precedent is locked func_800B2BDC.c (`Node D_80138880[100];`).
- No `unit_overrides` entry (prefer_definition is for functions). Nothing else defines these symbols: the 417 other
  mentions are `extern s32 D_80155238;` lines in m2c prototype headers. The blob.ld `PROVIDE(D_80155238…D_8015528C)`
  lines do not need to change: a PROVIDE gives way to a definition.
  The members at 0x80155240/48/88/8C keep their PROVIDE names for other references.
- **But no source form with the definition scores better than 6 rows.** The address web has net 3 > bestcost 2.1, so
  it is coloured a3 (s7: 15 words). w15d's oracle `force.sh s7 … "p1:w6=s"` gives 0 rows.
- Tried this wave:
  - Every store through the struct, with or without a pointer local (f2/f3: 31-word body, `addiu` base register).
  - `&D_80155238.next` as the jam argument (f4: same as s7).
  - next through an alias symbol (f1). The web splits, but the remaining piece in the first block holds msgQ and msg
    (two uses, net 1 > 0) and is coloured v0 (10 rows).
  - msgQ/msg as separately *defined* scalars (g1). as1 does **not** share `lui at` across two different defined
    symbols, so this gives the same 6 rows as best.c. The sharing needs one symbol plus an offset.
  - Compiled-out block ends (`do {} while (0);`, `if (task) {}`, `if (D_80155238.next) {}`) at five positions (`k*`).
    None of them changed the address web (nocs 2, tot 3, best 2.1). This uopt block boundary is not movable from source.
- **The contradiction.** Sharing needs msgQ and msg to be two uses of one defined symbol in the same block. Two such
  uses always make a profitable piece (net 1 > 0) unless that piece's bestcost is at least 1, which means it would
  have to be live across the memcpy call.
- **Best next hypothesis.** Retail may have reached the jam argument through a value that is not this web, for
  example an inlined static that returns `&D_80155238`. That would leave next/msgQ/msg as the only uses, which pins
  bestcost at 0, so even that needs a reason for the first block's piece to cost more. The source form is still unknown.

## entity_process_main (1,424 B): 2 words (one round, nothing new)

Tried (all in the unit; w11a best.c is still the best):

| Form | Result |
|---|---|
| `(u16)~(1<<i)`, `~(u16)(1<<i)`, `&= (u16)~…` | 17–19 |
| `(s16)poly->flags` sign extension | 2 words, `lh` instead of `lhu`, same shl,NOT,load order |
| `1u << i` | 2, unchanged |
| `\| 0` wrapper | 2, unchanged |
| `& 0xFFFF` | 17 |
| `~(u32)` | 18 |
| `poly[0].flags` | 18 |
| `(i & 0xFF)` shift count | 42 |
| Holders: mask through `xlu`/`pad1`, load through `body`/`xlu`, with `if (x) {}` before or after (e11–e16) | 22–126 |

The order in ugen is fixed by the ucode postfix order. Retail's order (shl, load, NOT) still needs a `Uswp` or some
other non-tree re-emission in uopt. No C expression tried reaches it. Bitfields cannot express a variable bit index.

## What generalises

- **Re-check earlier lanes' "plain source" claims after a unit change, and check that the file compiles.** w15c's
  variant files had a duplicated declarator line, so its claim that plain source gives 3 words was never measured.
- **Spill-home reuse tells you the block structure.** A home that reuses temp N (not temp 0) means the web shares a
  block with the owners of temps 0..N-1 but not with the owner of temp N. That pins where retail's block boundaries fall.
- **as1 shares `lui at` only within one defined symbol (sym+off)**, not across two adjacent defined symbols.
- Compiled-out `do {} while (0);` / `if (0) {}` did not move uopt's colouring blocks in either function here. The
  `if (x) {}` reads matter as *occurrences* (they change nocs and net), not as block boundaries.
