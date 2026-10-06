# w12a results (wave 12, 2026-10-06): residuals that were a single colouring tie

All scores were run from the repo root on the Pi with `python3 -m tools.conveyor.pipeline.blob_unit --tag w12a ... --neighbours`
on branch `wave11` (`ea15236c`).
- Builder scratch: `watchman2:~/rush2049/scratch/frontier/w12a`. It was copied from `base`, then src/blob, include,
  tools/cloud, asm/us/blob and the lock were synced, and the trace toolkit was installed with `--reuse wtk`.
- There were **no permission denials**.
- Nothing was committed, spliced or pushed. The only file written outside this lane dir is `cloud/matches/entity_update.c`.

| Function | Bytes | State | Flags | Scorer output (exact) |
|---|---:|---|---|---|
| `entity_update` | 1,564 | **MATCH** (was 10/391). Whole-program unit, no stand-ins, no extra `--with` parts | `-g0 -O3 -mips2 -G 0 -non_shared` (unit) | `EQUAL entity_update: 391 words (kept, c_entity_update.c)` / `locked bodies that differ in this unit: 0` / `blob_unit score: 1/1 equal` |
| `func_800DFBA0` | 1,192 | **provisional**, code-identical (was 21/298). Proven in the w11f component harness; its caller `func_800E05F0` and its callee `mode_select_input` are unmatched or provisional drafts. **Lane w12c lands the cluster** | `-O3` (unit) | `EQUAL func_800DFBA0: 298 words (internal, c_mode.c)` / `EQUAL mode_select_input: 38 words (internal, c_mode.c)` / `locked bodies that differ in this unit: 0` / `blob_unit score: 4/7 equal` |
| `input_deadzone_apply` | 3,580 | still **122/895** (no change). New diagnosis below | `-O3` (unit) | `FAIL input_deadzone_apply: 122 of 895 words differ` (`tools/ida.sh ida/best.c`) |

## Commands (replay against the current tree)

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w12a score entity_update --with cloud/matches/entity_update.c --neighbours
  EQUAL entity_update: 391 words (kept, c_entity_update.c)
  locked bodies that differ in this unit: 0
blob_unit score: 1/1 equal; object build/blob_unit/w12a/unit.o (5.6s)

TAG=w12a sh cloud/work/frontier/w12a/comp/run.sh        # w11f's component harness; comp/mode.c now has the w12a func_800DFBA0
  FAIL func_800D5E64: 10 of 146 words differ
  EQUAL best_times_display: 56 words (internal, c_d5e64.c)
  FAIL mode_select_handler: 684 of 744 words differ; compiled body is 763 words, target 744; ...
  EQUAL func_800DED78: 122 words (internal, c_ded78.c)
  EQUAL func_800DFBA0: 298 words (internal, c_mode.c)
  EQUAL mode_select_input: 38 words (internal, c_mode.c)
  FAIL func_800E05F0: 295 of 332 words differ; compiled body is 338 words, target 332
  locked bodies that differ in this unit: 0
blob_unit score: 4/7 equal; object build/blob_unit/w12a/unit.o (5.3s)
```

entity_update is also EQUAL together with w1g's provisional `func_800C3AD0` / `func_800AD4C8` /
`input_process_controller` part:
`... score entity_update func_800C3AD0 func_800AD4C8 input_process_controller --with eu/match_full.c --with w1g_part2.c`
gives `4/4 equal` and `locked bodies that differ in this unit: 0`.

## entity_update: 10 → 0 (`cloud/matches/entity_update.c`)

- **Residual (w11b):** a tie in uopt p1 colouring. The `*surf` reload web w177 (tot 6 / nocs 4) and the
  `D_80152818[car->id]` slot-address PRE web w188 (tot 3 / nocs 2) both had save 1.5. Ties go to the lower web
  number, so `*surf` took v0; retail has the slot in v0.
- **Lever:** a compiled-out read of an otherwise-unloaded field of the same game_car record,
  `if (D_80152818[car->id].place) {}`, placed as the first statement inside the state test.
  - It emits no code.
  - It adds one use to the slot-address web, so that web's save becomes greater than 1.5. The slot is now coloured
    first and takes v0, and `*surf` takes v1.
- **Variants that also give EQUAL:**
  - `dr_pos[0]` read in place of `place` (offset 0, arcade CAR_DATA's first field);
  - pad bytes at +0 or +858;
  - the read placed before the state test, or in the `*surf == 4` arm.
- **Variants that fail:**
  - a read in the `*surf == 7` arm stays at 10;
  - reads of `b856`/`b857` CSE with the real loads and move the colours (35–37 words);
  - a bare address test `if (&D_80152818[car->id]) {}` gives 38;
  - copying `*surf` into a local gives 73–147.
- **Type recovered for the header:** `EUSlot` (952 bytes) is CAR_DATA, with `dr_pos` at 0, b856, b857 (`state`,
  as in battle_mode_setup), and `place` at 0x35B (as in func_800EC914).
- **Both shaping quirks are disclosed in the file header:**
  - the `eu_nop()` frame-alignment helper (w11b);
  - the compiled-out read.
- **Integration notes:**
  - It is a plain kept function: no group and no unit_overrides.
  - `eu_nop` is static and is inlined away (w11b checked that no stub is left).
  - Own literals: 250.0, -5.0, 1.0, 0.25, -200.0. The scorer reported no unverified relocations.
  - entity_update is a real caller of the provisional `func_800C3AD0` (w1g group `func_800AD4C8`). That group
    still also needs `camera_victory` and `camera_trigger_check`.

## func_800DFBA0: 21 → 0 in the component harness (`dfba0/best.c` = `comp/mode.c`)

- **Residual (w11f):** the shared constant-1 and constant-2 webs (40/15 = 2.667) outranked the three counts and
  `i` (41/16 = 2.5625).
- **Lever:** `if (count0|count1|count2|i) {}` as the first statement of the loop body.
  - It is one compiled-out read.
  - It adds an in-loop use to all four variable webs, which lifts them above the constants. The counts then take
    v1/a0/a1, `i` takes a2 and the constants take a3/t0, as in retail.
- **Variants that do not work:**
  - four separate `if (countN) {}` statements: 14 words;
  - `||` in place of `|`: changes the code (267).
- **Integration:**
  - This is provisional, because the caller `func_800E05F0` is unmatched and `mode_select_input` is provisional.
  - **Lane w12c lands the cluster.** Use `comp/mode.c` (func_800DFBA0 body, with its disclosed header comment).
  - The component layout and the member list are unchanged from w11f's RESULTS: kept `func_800D5E64` and
    `func_800E05F0`; internal `best_times_display`, `mode_select_handler`, `func_800DED78`, `mode_select_input`,
    `func_800DFBA0` and `func_800E0050`; plus the `__inline entity_hierarchy_update` override.

## input_deadzone_apply: 122 (unchanged), new mechanism for the FP "ring phase"

I traced ugen (`ugt.sh`, `SCHED=1`) on the w11b best:

1. **ugen generates this procedure twice.** After the first pass, `f_check_no_used` →
   `f_get_temp_area_size() != 0` (ugen needed stack spill temps: the `136/140(sp)` swc1/lwc1 pairs). If flag
   `0x10019d3c` is clear, ugen sets it, calls `f_restore_i_ptrs`, resizes the temp area (`f_set_temps_offset`,
   `f_init_temps`) and **regenerates**. There are at most two passes (the flag). In this unit 4 procedures
   regenerate. Source: recompiled `ugen.trace.c` around line 57400.
2. **The FP temp free list is not reset between the passes.** Every procedure's first pass starts at the same
   head. Our second pass starts with the list left by pass 1's last FP statement, the `moved:` copies
   `cur[k] = np[k]` (pass 1 allocates 36,42,38 there, so pass 2 starts 40,36,42,38 = f8,f4,f10,f6). That is
   why our entry loads start[0] into f8.
   - Retail's entry order is f6,f10,f4,f8 (internal 38,42,36,40). This is exactly the pass permutation applied
     twice to the reset state, i.e. what a third pass would give. A third pass is impossible with this flag, so
     retail's first pass must have had a different FP event sequence.
   - Our two passes emit identical op sequences, so the retail difference is in something pass 1 does
     differently from the final pass.
3. **Listing oracle** (`asm.sh` on the ugen listing, `ida/diag/ugen_listing_best.s`):
   - renaming f4↔f10, f6↔f8 over the whole listing: 273 rows;
   - renaming only the first 500 listing lines (up to the first face section): **51 rows**.

   So the permutation is local to the entry and the first face section. The rest of the function already
   matches, and the 12 "load placement" rows are as1 consequences of the register names.

**Next hypothesis:** find the source shape that makes ugen's *first* pass end with free-list order 38,42,36,40.
- It must leave the final pass identical to the current one.
- Example: a spill or temp-area-dependent decision that pass 1 makes differently, such as an expression that
  needs a stack temp only while the temp area is still 0.
- Two other checks:
  - whether retail had fewer or more ugen spill temps (if the temp area were 0, ugen would not regenerate and
    would start at f4, which retail does not);
  - the FP colours w36/w66 (forcing both still leaves 115 rows).

## Generalisable

- **The compiled-out read lever applies to address and PRE webs too.** `if (rec[i].other_field) {}` adds a use
  to the shared address web without emitting code.
  - The field must not be loaded by real code nearby. Otherwise the dead load CSEs with the real load and takes
    a register.
  - A bare address test (`if (&rec[i]) {}`) does *not* work: it changes the code.
- **One compiled-out read can raise several webs at once.** `if (a|b|c|i) {}` in a loop gave each variable one
  in-loop use (+10 tot). Separate statements were worse (14 words) and `||` adds code.
- **ugen can regenerate a procedure, and the FP temp free list carries across the regeneration.** If a
  function's FP temps look like a renamed ring from the entry onward, check `ugt.sh` for a second pass (emit
  counter restarting after `f_check_no_used`, at most one regeneration). The final pass's start state is the
  FP free-list order after pass 1's last FP statement, not the reset order.

## Files

| Path | Contents |
|---|---|
| `cloud/matches/entity_update.c` | The match. Header comment, CAR_DATA-typed header (`hdr3.h`), body |
| `eu/match_body.c`, `eu/match_full.c` | The same match, as body only and with the header |
| `eu/w11b_best_*.c` | w11b baseline |
| `eu/{a,b,c,d}_*.c` | Probes quoted above |
| `eu/gen*.py` | Variant generators |
| `comp/` | w11f harness copy; `mode.c` holds the w12a func_800DFBA0. `run.sh` uses TAG w12a |
| `dfba0/best.c` | Same as `comp/mode.c` |
| `dfba0/e_*.c` | Probes |
| `ida/` | w11b best + ipcorder header/part; `diag/ugen_listing_best.s` |
| `tools/eu.sh` | entity_update unit scorer (`HDR=` header) |
| `tools/df.sh` | Component scorer for mode.c variants |
| `tools/ida.sh` | input_deadzone_apply scorer |
| `tools/udiff.py` | Toolkit copy |

No script pins the lock, the manifest or the asm text.
