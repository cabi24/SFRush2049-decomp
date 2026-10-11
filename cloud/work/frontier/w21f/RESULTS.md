# w21f RESULTS: entity_spawn_init (0x8008EA10, 5544 B, 1386 words, -O3)

| function | bytes | state | flags | scorer output |
|---|---:|---|---|---|
| entity_spawn_init | 5544 | **EQUAL in the whole-program unit; group MATCH** (was 7 words) | -g0 -O3 -mips2 -G 0 -non_shared | Pi: `python3 -m tools.conveyor.pipeline.blob_unit --tag w21f score entity_spawn_init --with cloud/work/frontier/w21f/groups/frontier_entity_spawn_init/entity_spawn_init.c --neighbours` → `EQUAL entity_spawn_init: 1386 words (kept, c_entity_spawn_init.c)` / `locked bodies that differ in this unit: 0`. Builder: `python3 tools/cloud/score.py group cand/frontier_entity_spawn_init` (no `--claims`) → `entity_spawn_init: MATCH` / `own .rodata verified at 0x80123960..0x801239A8`. Context `func_8008B2E4: MISMATCH (2 extra words (nonzero beyond target length))` is informational; the locked frontier_func_8008E408 group prints the same kind of line (`1 extra words`). |

Deliverable: `groups/frontier_entity_spawn_init/`, containing:
- `entity_spawn_init.c`: the member. Its header discloses every device.
- `func_8008B2E4.c`: a byte-identical copy of `src/blob/func_8008B2E4.c`, used as context.
- `group.json`: members `[entity_spawn_init]`, context `[func_8008B2E4]`, keep both, claims `[entity_spawn_init]`.

There is no `cloud/matches` file. The function inlines func_8008B2E4 19 times and two file statics, so it can only match in the unit or in a group. Time used: about 1.5 h.

## Integration notes
- **Install:** it installs like the locked `frontier_func_8008E408` group, which follows the same pattern (a locked context file plus a static inlined helper that has one call site).
- **Overrides and supersessions:** none. No unit_overrides, no data-symbol changes, no group superseded (entity_spawn_init has never been locked).
- **Unlocks:** this is the sole blocker for buffer_swap, cpak_read and func_80090308 (`frontier show`: 3 functions, 3,652 B).
- **Extra stubs in the shadow unit:** the two statics `get_car` and `get_mph` leave two unsymbolled `jr ra; nop` stubs between func_8008E408 and entity_spawn_init. Retail has no stubs there.
  - attach() in func_8008E408's group does the same thing: its stub sits before func_8008E408 in the unit, and that group was spliced.
  - The member body is symbol-bounded and the stubs do not follow a member, so the splice should not see "extra words". Still, run `blob_splice` / `blob_rom rom` to confirm.
- **Line-number dependence:** the helpers must stay defined *after* entity_spawn_init, with prototypes before it. as1 breaks an aftercycles tie between `addu s1` (from get_car's body) and `sw zero,644(sp)` (`i = 0`) by the lower source line. If get_car is defined first, its addu is scheduled before the sw, which is 2 rows wrong (v/k1.c).
  - Do not reorder the file, and do not run a tool that moves definitions.

## How the last 7 words closed (traced)
w20g left two spill homes 4 B off (model address 96 vs 92, switch type copy 88 vs 84). It had two explanations: (A) an extra early web, (B) `car` kept as a variable plus 8 B more area. B is right.

1. **`car = get_car(slot)`** with `static Car *get_car(s16 slot) { return &D_80152818[slot]; }`, written as a statement after the first early return (v/g3.c).
   - The inlined return arrives in v0 (`str.A(car, varR(2))` in the uopt listing). uopt keeps `car` as a variable (s1) and no longer substitutes the address expression, so car has no spill home.
   - The helper adds an 8-byte inline area, so the area is 632.
   - `spill.sh` then gives retail's homes exactly: model address 92, type copy 84, area 632 → 648, frame 728.
   - The w20g homesim prediction for B is confirmed.
2. **mph in v0** (v/k1.c).
   - With only get_car, uopt copy-propagates the v0 return into `car->mph` (`ilod@248(varR(2))`), so v0 is in use in mph's block. I added a block-mask dump, `tools/maskpatch.py`, which shows that mph's block already carries the colour-occupancy mask 0x40010800 (v0, s1, s6) when mph is decided (v/g12.c, block 7). mph gets v1. A separator (`do {} while (0)`, `if (x) {}`, a label) does not stop the propagation.
   - A second helper, `static s16 get_mph(Car *c) { return c->mph >> 2; }`, makes mph the next v0 return, as in retail. Its 8-byte area replaces two filler words (pD44, pD45).
3. **as1 order** of `sw zero,644(sp)` vs `addu s1` (v/q1.c, q2.c).
   - The two nodes tie on aftercycles (16), and as1 picks the lower line number (`as1t.sh`).
   - Defining get_car after the function gives the inlined addu a higher line than `i = 0`. After that, ops = 0.
4. **type in s4** (v/t1.c).
   - Each inlined return label adds one uopt block to type's range: card 154 → 156, nocs 46 → 47, save 192/47 = 4.085. type then loses to the constants 0 and 68 (150/36 = 4.1667), and s4/s5/s6 are permuted (71 words).
   - Oracle check: `PRIO=9:1.03` on the k1 snapshot leaves only item 3.
   - Fix: `if (type) {}` at the top of the loop body adds 10 to type's total (202/47 = 4.30). Result: EQUAL.
   - This is a disclosed compiled-out read.
   - Natural alternatives that did not change type's block count: removing the three empty join blocks in type's range (22 = the `type < 4 || type >= 6` landing, 161 = the inner `type == 6 / type == 7` join under `if (first)`, 182 = the colour switch end), several `||` spellings, and combining the two helper calls into one statement.
   - Not found: a form that removes two of those blocks without changing the code layout.

## Findings that generalise
- **An inlined helper's return value keeps a variable alive.** `x = helper(...)` assigns x from varR(2), so uopt does not substitute the expression into x's uses. This is a clean way to get "a pointer kept as a variable" when hand-written forms are always copy-propagated. It complements w15d (an inlined static's return value is a separate register value).
- **Side effect of that v0.** uopt copy-propagates the v0 into later uses until v0 is redefined, which forbids v0 for every web live in those blocks. Retail values in v0 right after such an assignment point to a second inlined return.
- **Inlined code carries the helper's source line numbers,** and as1 breaks aftercycles ties by line. So *where* a helper is defined (before or after its caller) changes instruction order.
- **Each inlined return label is one uopt block** in the live ranges of everything live there. Re-check the nocs (blocks counted in groups of 4) of tied candidates after adding a helper.
- **Frame residual (owner rule):** pB0..pB2, pC1..pC3 and pD00..pD43 are exactly the residual.
  - They are 50 slots: 49 s32 plus one u16.
  - Without them the frame is 528 (retail 728). With one fewer it is 720 (159 words). One more still gives 728 (8-byte granularity).
  - So the w15e set is now minimal for this structure. Its identity is not known: an inlined func_8008E06C at the 6 colour stores would explain 12 of the words but does not fit umerge's budget in the shadow unit. v/w1.c: body 1353, `-space_time` as in w20g.

## Not done / next
- If the owner wants the `if (type) {}` device removed, the next lever is two fewer uopt blocks in type's range with identical code (candidates: blocks 22/161/182 above), or an extra natural type use in the loop. `tools/tc.sh` prints type's card/nocs per variant.
- Helper identity: retail has no kept function for either helper (scan of every ≤40-word function for `&D_80152818[x]` / `lh 248(a0)`). Being file statics with one call site matches the attach() precedent.

## Tools (lane `tools/`, diagnostic only)
- **Retagged from w20g:** `sv.sh`, `oracle.sh`, `hl.sh`, `ct.sh`, `t.sh`, `bmap.py`, `homesim.py`, `olim.sh`, `mdis.sh`. The oracle binary `bin/uopt_or` in builder `~/rush2049/scratch/frontier/w21f` is w16d patch_spl + w16b splitpatch + w17f prio_patch + `maskpatch.py`.
- `tc.sh FILE LABEL`: unit score, spill homes, and the SAVELOG card/nocs/tot of slot, type, model address and constant 5.
- `blk.py OR_SPL B…`: the live ranges containing block B, plus its occurrences. Use it to find empty uopt blocks.
- `maskpatch.py`: with `LBB=1`, prints each live range's blocks and their colour-occupancy masks at its p1 decision.

## Notes
- Variants are in `entity_spawn_init/v/` (ignored).
  - g*: the getter.
  - k*, p*, q*: the mph helper, statement order and helper placement.
  - t1: EQUAL.
  - u*: filler count.
  - w*: e06c.
  - x*, r*: block spellings.
- No permission denials. Nothing was written outside the lane dir, the builder scratch and the session scratchpad.
