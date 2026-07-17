# Quickstart: Game-Code Context Bootstrap (005)

End-to-end bring-up and acceptance walkthrough. Everything except §4 runs on
the Pi alone; §4 needs the coordinator + watchman builder (same bring-up as
`specs/001-matching-pipeline/quickstart.md` §1–4).

## 1. Extent repair (SC prerequisite — run first)

```bash
python3 -m tools.conveyor.pipeline.matrix extract
```

Expect the new repair report: `extents: N agree, N repaired, N conflict`,
plus the 10 cluster before→after lines. Verify against the oracle table in
`contracts/extent-repair.md`:

```bash
python3 - <<'EOF'
import sqlite3, os
db = sqlite3.connect(os.path.expanduser("~/.conveyor/conveyor.db")); db.row_factory = sqlite3.Row
for r in db.execute("SELECT target_id, address, insn_count, gate_reason FROM n64_target"
                    " WHERE target_id IN ('game_loop','countdown','countdown_handler')"):
    print(r["target_id"], hex(r["address"]), r["insn_count"], r["gate_reason"])
EOF
```

`countdown` must now read ~668 instructions (was 2). Re-run extract:
second pass must report `repaired 0` (idempotency / SC-003 determinism).

Actuals (2026-07-16, game-code SHA-256
`bf7da3fa6283428a97372250cd4076d15e9eae10f9d5709c0387fe0742d43a1d`):
the first pass reported `243 agree, 423 repaired, 219 conflict`; all 10
exclusive end addresses matched `contracts/extent-repair.md`, including
`countdown` at `0x800FC9F8` (668 instructions). The immediate second pass
reported `666 agree, 0 repaired, 219 conflict` and `0 targets, 0 evidence
rows purged`.

Amendment (2026-07-16, A1 review gate): the contract's termination
condition was corrected from `pc > furthest` to `pc >= furthest`
(shared-return leaves branch directly to their `jr $ra`; see
contracts/extent-repair.md §3). Re-extract repaired 26 further extents and
cleared 22 false conflicts: now `688 agree, 0 repaired, 197 conflict`
(idempotent), cluster oracle unchanged. Proof case: `sound_stop`
0x800B358C = 40 insns (was 94, overrunning into the next function).

## 2. Histogram — the measurement instrument (Pi-only)

```bash
python3 -m tools.conveyor.pipeline.autodecomp clusters --population extracted --limit 0
```

Outputs `build/m2c_histogram.md` (ranked blockers) and
`build/m2c_histogram.json`. Check SC-003:

```bash
python3 - <<'EOF'
import json
d = json.load(open("build/m2c_histogram.json"))
assert sum(d["buckets"].values()) == d["run"]["targets"], d["buckets"]
print(d["buckets"])
EOF
```

Run twice; bucket counts must be identical. SC-004: read the top-10
blockers in the md — they should cover a majority of `blocked`.

Actuals (2026-07-16, game-code SHA-256
`bf7da3fa6283428a97372250cd4076d15e9eae10f9d5709c0387fe0742d43a1d`):
two consecutive full 885-target runs produced identical JSON after removing
only `run.timestamp`. Both runs reported `15 compiled, 624 blocked, 49
decompiler_failure, 0 no_disasm, 197 extent_conflict` (sum 885); the second
run took 3m26s, comfortably within the 30-minute goal. SC-004 is **not met**:
the top 10 ranked blockers cover 258 distinct blocked functions (41.3% of
624), not a majority. The next-highest-value context investment is a bounded
shared-function declaration/prototype layer, starting with
`slot_state_setup` (27 functions) and `math_utility` (26), then the common
dispatch/audio helpers and libc `memset`/`memcpy`; `M2C_ERROR` and
`saved_reg_s0` are decompiler/inference artifacts rather than game-type
context and should be handled separately.

Re-run after T018/T019 populated `include/game_types.h` (2026-07-16, same
game-code SHA): `29 compiled, 610 blocked, 49 decompiler_failure, 0
no_disasm, 197 extent_conflict` (sum 885, 3m37s) — +14 compiled / -14
blocked population-wide from the ~75 extern function prototypes + typed
symbol-table globals added for the cluster (those declarations are shared
context, so other extracted targets calling the same helpers benefited too).
None of the game-loop cluster's own 10 targets crossed into `compiled`
despite this — see research/t019-stall.md for why (a disasm.py
symbolization gap unrelated to header content, not a shortfall in
`game_types.h` itself).

Re-run after the 2026-07-17 contract §5 symbolizer amendment (same game-code
SHA): two consecutive full 885-target runs reported `33 compiled, 606
blocked, 49 decompiler_failure, 0 no_disasm, 197 extent_conflict` (sum 885).
After removing only `run.timestamp`, both JSON files had SHA-256
`0e8573b5925bd1a039cc100053256d62b340d9ab23ca4f9d23967cf732a2da10`.
This is +4 compiled / -4 blocked versus the T018/T019 run.

Re-run after the fourth §5 amendment (indexed-`addu` idiom (c) +
`D_8011ED0C` table addition + byte-cursor `_clean_m2c` rule, 2026-07-17,
same game-code SHA): two consecutive full runs reported `42 compiled, 597
blocked, 49 decompiler_failure, 0 no_disasm, 197 extent_conflict` (sum
885), JSON identical after removing `run.timestamp`. Population-wide
`compiled` progression across the feature: 15 → 29 → 33 → 42.

T019 residue full-run record (2026-07-17, same game-code SHA): `34 compiled,
605 blocked, 49 decompiler_failure, 0 no_disasm, 197 extent_conflict` (sum
885). This is +1 compiled / -1 blocked versus the contract §5 amendment and
corresponds to the newly typed `sound_control` allocation record.

## 3. Cluster seeds compile (SC-001, Pi-only)

```bash
python3 -m tools.conveyor.pipeline.autodecomp clusters --population extracted \
    --targets @tools/conveyor/clusters/game_loop.txt
```

Expect ≥8/10 in `compiled`, including ≥4 of the 60+-instruction members
(game_loop, playgame_state_change, RaceStateMachine_Update,
attract_or_transition, countdown, …). If blockers name game types, grow
`include/game_types.h` (provenance comment per definition — FR-003) and
re-run; the delta between histogram runs is the progress record.

Static no-regression (SC-005): before merging context changes,

```bash
python3 -m tools.conveyor.pipeline.autodecomp one viGetTimeToDeadline --dry-run
```

(or the tasks' chosen known-good static sample) — seed text must be
byte-identical before/after `game_types.h` lands.

**Actuals (2026-07-16, T018/T019)**: `include/game_types.h` populated
(GState enum, the full 9-entry symbol-table's types, `Track_Data`, a
fully-typed `SoundClearRecord` for `sound_control`'s own parameter, and
~75 extern function prototypes covering every named-symbol blocker in the
T011 run). Scoped cluster probe after full iteration:
`compiled=0 blocked=9 decompiler_failure=1` (game_loop.txt, 10 targets) —
**SC-001 not met**. This is not a header-content shortfall: the T011
blocker list shrank from 90 distinct symbols to 12 (every `?`-typed
prototype blocker is gone; `sound_control` itself has zero self-contained
errors left), but every remaining blocker is a raw numeric address
(`*(void *)0xADDR`) with no attachable symbol name, caused by a confirmed
`disasm.py` symbolization gap (register-formed pointers via `lui`+`addiu`
aren't tracked) plus a few addresses never in the 9-entry symbol table.
Full analysis, evidence, and unblocking path: research/t019-stall.md.
`RaceStateMachine_Update` separately fails at the m2c level (unsupported
jump-table dispatch, `jr $t9` — a decompiler limitation, not a type-context
gap).

The 2026-07-17 SC-005 amendment tests the emitted static function body rather
than the necessarily growing shared context prelude. The body-identity test
(`test_game_types_keeps_known_good_static_function_body_byte_identical`) is
green with the populated header; research/t019-stall.md retains the original
whole-seed incompatibility analysis that motivated this correction.

**Actuals (2026-07-17, contract §5 amendment)**: the scoped probe reported
`compiled=0 blocked=9 decompiler_failure=1 no_disasm=0 extent_conflict=0`.
The repaired state machine now emits symbols for `frame_counter`, `msgq_ptr`,
`input_rec0`, `input_rec1`, and `player_array` in the cluster assembly, but
no cluster member crossed into `compiled`; remaining failures are unrelated
raw addresses, inferred-structure/prototype blockers, or empty-tokenizer
compile failures. The adjacent `countdown_state`/`countdown_object` consumers
at `0x8017A4E0/0x8017A4E4` share one `lui`, so both correctly remain numeric
under the normative mismatched-pair refusal. The amended SC-005 function-body
identity test passes.

**Actuals (2026-07-17, T019 residue)**: four header/probe iterations moved the
scoped result to `compiled=1 blocked=8 decompiler_failure=1`; `sound_control`
is the compiling member and `RaceStateMachine_Update` is the accepted m2c
jump-table failure. SC-001 remains unmet (1/10, including 1 large member).
SC-005's function-body identity test stayed green after every valid header
change. The remaining compiler diagnostics include literal-address void
expressions and malformed m2c output outside the four authorized mechanical
classes, so the stop rule fired; exact source lines are recorded in
`research/t019-stall.md`.

**Actuals (2026-07-17, second gate)**: per-consumer LUI rebinding, complete
cluster-survey symbol/type coverage, and unknown function-pointer cast cleanup
moved the scoped probe to `compiled=4 blocked=5 decompiler_failure=1
no_disasm=0 extent_conflict=0`. The accepted decompiler failure remains
`RaceStateMachine_Update`. The remaining failures include unsurveyed numeric
bases and inferred local/call member chains outside the amendment, so the
below-8/10 stop rule fired. The two full histograms were therefore not run;
exact evidence is appended to `research/t019-stall.md`.

**Actuals (2026-07-17, fourth gate — SC-001 MET)**: scoped probe
`compiled=8 blocked=1 decompiler_failure=1`. Compiling: game_loop (225
insns), playgame_state_change (636), attract_or_transition (519),
Input_ProcessGameplayPad (207), countdown_handler (124), sound_control
(117), process_inputs (89), game_mode_handler (64) — seven of 60+
instructions (needed 4). Accepted failures: RaceStateMachine_Update (m2c
jump-table) and countdown (m2c expression merging; see t019-stall.md).
Closed by: contract §5 idiom (c) (lui+addu runtime-index symbolization,
implemented with a derivation-version cache key), the derived-asm-cited
`D_8011ED0C` u16 array, and a `_clean_m2c` rule rewriting m2c's invalid
`u8*`-cursor member reads to explicit offset loads.

## 4. Cluster seeds score (SC-002 — needs coordinator + builder)

```bash
# coordinator on Pi, agent on watchman (001 quickstart §1–4), then:
python3 -m tools.conveyor.pipeline.autodecomp seed --population extracted \
    --targets @tools/conveyor/clusters/game_loop.txt --budget 3600
python3 -m tools.conveyor.cli report            # after jobs drain
```

Every compiling cluster seed gets a recorded numeric score attributable to
its target (population-tagged in the report). Score 0 = matched (evidence
only — see §5).

**Actuals (2026-07-17, T019 residue)**: scoring was not started because the
header-only convergence stopped at 1/10, below T014's required 8/10. No claim
of per-target score coverage is made for this residue run.

**Actuals (2026-07-17, fourth gate — SC-002 MET)**: 9 seeds submitted
(`no_seed=1`: RaceStateMachine_Update, the accepted m2c failure); all
searches ran to terminal state on Watchman24 (16 cores, 3600s budget).
Every compiling cluster seed has a numeric best score recorded with a
retained best-source artifact, attributable per target: process_inputs
755, sound_control 925, game_mode_handler 1295, countdown_handler 4480,
game_loop 6495, attract_or_transition 9115, playgame_state_change 12970,
Input_ProcessGameplayPad 40055. `countdown` ran but recorded no score
(its seed does not compile — the second accepted failure). The permuter
demonstrably refines these seeds (process_inputs 8305→755,
sound_control 10615→925 within the budget), validating the seeds as
search starting points. No score-0 yet — expected for a walking
skeleton; search-pool refinement at scale is explicitly out of scope
(spec, Out of Scope).

## 5. Firewall check (SC-006)

```bash
python3 -m tools.conveyor.pipeline.lock add work/whatever/game_loop.c:game_loop \
    --flags=-g0 --skip-verify  # MUST refuse before reading the source
python3 -m tools.conveyor.pipeline.promote batch --locked --via-builder          # touches no extracted fn
```

The lock/promote guards must reject extracted-population functions with the
FR-010 error; `promotion_record` must show no extracted entries.

**Actuals (2026-07-17)**: `lock add` refused `game_loop` with
`error: game_loop is extracted-population — evidence-only (005/FR-010)` and
exit status 1. A `promotion_record` → `n64_target` population join returned
zero extracted entries. The live-pool walkthrough intentionally did not invoke
`promote batch`; its complete-plan preflight is covered by the local guard.

## 6. Tests

```bash
pytest tests/conveyor -m "not node_required"
```

New: `test_extent_scan.py`, `test_disasm.py`,
`test_autodecomp_population.py` — plus the full existing suite green.

**Actuals (2026-07-17)**: `162 passed, 5 deselected in 12.07s`.
Quickstart §1–§3 and §5–§6 reproduced; §1's second extract reported
`688 agree, 0 repaired, 197 conflict`, two §2 runs both reported
`42/597/49/0/197` with identical timestamp-normalized JSON hashes, and §3
reported `compiled=8 blocked=1 decompiler_failure=1`. Section 4 was not run
because a live scoring harvest was in progress and its actuals are recorded
separately.
