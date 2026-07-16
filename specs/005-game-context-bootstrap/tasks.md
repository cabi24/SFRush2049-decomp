# Tasks: Game-Code Context Bootstrap (Track B Walking Skeleton)

**Input**: Design documents from `/specs/005-game-context-bootstrap/`
**Prerequisites**: plan.md, spec.md, research.md, data-model.md,
contracts/extent-repair.md, contracts/seed-derivation.md, quickstart.md.
Codex ground truth in `research/` (cluster-data-refs.md, arcade-structs.md,
m2c-dryrun.md) — implementation tasks cite it; read it before coding.

**Tests**: Included — the plan names three new test modules and the contracts
define oracles; extent repair especially must be test-first (it rewrites
evidence).

**Organization**: Grouped by user story. Note the deliberate cross-story
dependency the spec declares: US3 (game-type context) is the enabling content
for US1's final acceptance number — the pipe (US1) and the instrument (US2)
are independent after Foundational, but SC-001 is expected to need US3
iterations. See Dependencies & Execution Strategy.

## Format: `[ID] [P?] [Story] Description`

---

## Phase 1: Setup

**Purpose**: Confirm the environment matches the plan's assumptions before
touching evidence.

- [x] T001 Preflight check (no code): verify `mips-linux-gnu-objdump`,
      `mips-linux-gnu-as`, `mips-linux-gnu-gcc`, and `cpp` run on the Pi;
      verify `build/game_code.bin` exists and record its sha256; verify
      `~/.conveyor/conveyor.db` has 885 `population='extracted'` rows with
      `target_o_sha` set and the 10 cluster target_ids from
      `specs/005-game-context-bootstrap/data-model.md` all resolve. Emit the
      numbers into the task log; abort the feature if any check fails.

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Honest extents and a working blob→m2c derivation path. Both user
stories US1 and US2 score/classify against target objects; until extents are
repaired those objects are wrong (research.md D1). Nothing else may start
first.

**⚠️ CRITICAL**: T002–T005 rewrite evidence (target objects + supersession).
Test-first, and verify the oracle before proceeding.

- [x] T002 [P] Write `tests/conveyor/test_extent_scan.py` against the
      normative algorithm in `contracts/extent-repair.md`: early `jr $ra`
      inside a loop does not terminate (furthest-forward-branch rule);
      `jr $t9` jump-table dispatch does not terminate; terminating delay slot
      is included; backward branches do not extend the extent; 16 KiB
      `scan_overrun` cap; scan is a pure function (same bytes+address ⇒ same
      extent). Use small hand-assembled word lists, not the real blob.
- [x] T003 Implement `scan_extent(image_bytes, address)` in
      `tools/conveyor/pipeline/targets.py` per `contracts/extent-repair.md`
      (decode big-endian words; track furthest forward branch target across
      all `b*`/`bc1*` forms; end at first `jr $ra`+delay-slot past
      `furthest`; `j`/`jal` never extend; bounds → `scan_overrun`). Make
      T002 pass.
- [x] T004 Integrate the repair pass into the extract flow in
      `tools/conveyor/pipeline/targets.py`: for each `extracted` target
      classify agree/repair/conflict; on repair update `insn_count`,
      re-carve via `function_words`, re-assemble via `assemble_words`, store
      the new object, set `target_o_sha` and `gate_reason='extent_repaired'`,
      and purge that target's `matrix_entry` rows in the same transaction
      (reuse 003's supersession helper — do not reimplement); after all
      scans, mark targets whose `address` falls strictly inside another
      repaired extent `gate_reason='extent_conflict:<container_id>'`; print
      the repair report line (`extents: N agree, N repaired, N conflict`)
      plus before→after for the 10 cluster targets. Extend
      `tests/conveyor/test_extent_scan.py` with a DB-fixture test proving
      supersession fires and conflict rows are excluded flags-wise.
- [x] T005 Run `python3 -m tools.conveyor.pipeline.matrix extract` on the Pi
      and verify against the oracle table in `contracts/extent-repair.md`
      (all 10 cluster end addresses exact; `countdown` ≈668 insns); re-run
      and verify `repaired 0` (idempotency, SC-003 precondition). Record the
      agree/repair/conflict counts in
      `specs/005-game-context-bootstrap/quickstart.md`'s §1 as actuals.
- [x] T006 [P] Write `tests/conveyor/test_disasm.py` against
      `contracts/seed-derivation.md`: the three objdump normalizations
      (column strip, `$`-prefix, `.L<vaddr>:` labels with branch-operand
      rewrite incl. `bc1f/bc1t[l]`); `jal` rewrite to `target_id` when the
      vaddr is a known target else `func_<vaddr>`; `%hi/%lo` symbolization
      only for game-symbol-table addresses; byte-identical output across two
      runs; cache invalidation on (extent, image sha, symbol-table sha)
      change. Feed it fixed word lists via a fake image.
- [x] T007 Implement `tools/conveyor/pipeline/disasm.py`: carve
      `[address, address+4*insn_count)` from `build/game_code.bin`,
      disassemble via `mips-linux-gnu-objdump -D -b binary -m mips:4300 -EB
      --adjust-vma=0x80086A50 --start-address --stop-address`, apply the
      normalizations, symbolize per the committed game symbol table (module
      constant, 9 entries from `data-model.md` with the
      research/cluster-data-refs.md evidence cited per entry), emit
      `glabel <target_id>` files cached at `build/m2c_asm/<target_id>.s`.
      Make T006 pass.
- [x] T008 Population plumbing in `tools/conveyor/pipeline/autodecomp.py`:
      add `--population {static,extracted}` (default `static`) and
      `--targets id1,id2,…|@file` to `seed`, `one`, and `clusters`; asm
      resolver becomes two-branch (static → `_asm_index()` unchanged;
      extracted → `disasm.py` cache); refuse targets whose `gate_reason`
      starts `extent_conflict`; flagset for extracted defaults to
      `-g0 -O2 -mips2 -G 0 -non_shared` with `-O1` recorded alternate
      (extend `farm._flagset_for` fallback); capture m2c stderr per target
      into diagnostics instead of discarding it. Add
      `tests/conveyor/test_autodecomp_population.py` covering: default
      static behavior unchanged (assert same SQL predicates / same seed for
      a fixture), `@file` target resolution aborts on unknown names,
      extent-conflict refusal.
- [x] T009 Wire the (initially empty) `include/game_types.h` into
      `autodecomp._context()`'s header chain (after `m2c_types.h`) and
      capture the SC-005 baseline: generate the m2c seed for one known-good
      static function (pick a farm-matched one, e.g. from `src/rom_auto/`)
      before and after the wiring and assert byte-identity; keep that
      comparison as a test in `tests/conveyor/test_autodecomp_population.py`
      so later game_types.h growth re-runs it.

**Checkpoint**: extents honest, derivation deterministic, population axis
exists, context chain extended — US1 and US2 can proceed in parallel.

---

## Phase 3: User Story 1 — Cluster seeds compile and score (P1) 🎯 MVP

**Goal**: The 10 game-loop-cluster functions flow m2c seed → compile → score
with evidence recorded (SC-001, SC-002).

**Independent Test**: quickstart.md §3 (compile, Pi-only) and §4 (score,
builder required); evidence store shows population-tagged scores for cluster
targets.

- [ ] T010 [P] [US1] Create `tools/conveyor/clusters/game_loop.txt` with the
      10 target_ids and address comments from `data-model.md`; implement
      `@file` list parsing (strip `#` comments) where T008 left the hook;
      add a resolution test to
      `tests/conveyor/test_autodecomp_population.py`.
- [ ] T011 [US1] Compile-probe run:
      `python3 -m tools.conveyor.pipeline.autodecomp clusters
      --population extracted --targets @tools/conveyor/clusters/game_loop.txt`
      on the Pi. Expected per research/m2c-dryrun.md: all 10 derive and m2c
      cleanly (the dry run's failures were extent artifacts removed in
      Phase 2); record which compile as-is (numeric-global seeds are
      self-contained, research.md D3) and the named blockers for the rest —
      this blocker list is US3's worklist. Persist the output in the task
      log verbatim.
- [ ] T012 [US1] Scoring path: submit compiling cluster seeds via
      `autodecomp seed --population extracted --targets @…` against the live
      coordinator+builder (001 quickstart §1–4 bring-up); confirm
      `function_status.seed_kind='m2c'` and `in_search` transitions, then
      after harvest confirm numeric scores in the evidence store
      attributable to each target (`matrix_entry`/report), artifacts
      retained. This is SC-002.
- [ ] T013 [US1] Population visibility in reporting: ensure
      `python3 -m tools.conveyor.cli report` (and `matrix report` if it
      lists targets) distinguishes `extracted` rows from `static` (spec
      US1/AC3) — add the population column/tag where missing in
      `tools/conveyor/pipeline/status.py` or the CLI report path; verify by
      eyeball on the T012 results.
- [ ] T014 [US1] SC-001 acceptance run (after US3 iterations land): ≥8/10
      cluster seeds compile including ≥4 members of 60+ instructions
      (game_loop, playgame_state_change, RaceStateMachine_Update,
      attract_or_transition, countdown, Input_ProcessGameplayPad,
      sound_control qualify); every compiling seed has a recorded score.
      Record final numbers in quickstart.md §3/§4 as actuals.

**Checkpoint**: walking skeleton proven end-to-end.

---

## Phase 4: User Story 2 — Failure histogram across all 885 (P2)

**Goal**: Every extracted target in exactly one outcome bucket; ranked
blocker report as durable artifacts (SC-003, SC-004).

**Independent Test**: quickstart.md §2 — full run on the Pi, bucket counts
sum to population, two consecutive runs identical.

- [ ] T015 [US2] Rework `cmd_clusters` in
      `tools/conveyor/pipeline/autodecomp.py` into the bucketed instrument
      per `contracts/seed-derivation.md`: with `--population extracted
      --limit 0`, classify every extracted target with a target object into
      exactly one of `compiled` / `blocked` (named symbols via the existing
      tokenizer) / `decompiler_failure` (m2c nonzero/timeout; first stderr
      line kept) / `no_disasm` (derivation failure reason, e.g.
      `scan_overrun`) / `extent_conflict` (from `gate_reason`); one target's
      crash must not abort the batch (FR-009).
- [ ] T016 [US2] Emit the two durable artifacts from the same run data:
      `build/m2c_histogram.json` matching the schema in `data-model.md`
      (run header with image/context shas, bucket counts, per-target map,
      ranked blockers with arcade hints via existing `_arcade_hint`) with
      deterministic ordering, and `build/m2c_histogram.md` (human ranking,
      successor of `build/m2c_clusters.md`). Add a bucket-exclusivity +
      counts-sum + determinism (two runs, timestamp field excluded) test to
      `tests/conveyor/test_autodecomp_population.py` using a small fixture
      population.
- [ ] T017 [US2] Full 885-target run on the Pi
      (`autodecomp clusters --population extracted --limit 0`), twice;
      verify SC-003 (identical bucket counts) and the ~30-min performance
      goal; evaluate SC-004 from the md (do the top-10 blockers cover a
      majority of `blocked`?) and write the answer + the named
      next-highest-value context investment into quickstart.md §2 actuals.

**Checkpoint**: the rest of Track B is priced.

---

## Phase 5: User Story 3 — Minimal game-structure context (P3, feeds US1)

**Goal**: Provenance-stamped game types sufficient for the cluster, no static
regression (SC-005).

**Independent Test**: US1's blocker list (T011) shrinks after each addition;
T009's byte-identity test stays green.

- [ ] T018 [P] [US3] Populate `include/game_types.h` — bounded strictly by
      the T011 blocker list (FR-004), content per `data-model.md` and
      `research/arcade-structs.md`: `GState` enum
      (`/* rushtherock: game/game.h:104-117 */`, N64 note: stored as `u8`);
      input-record struct(s) shaped to the observed 0x18-byte layouts at
      0x8014A118/0x8014A164 (cite globals.h:143-147,213-215 +
      modeldat.h:472-493 as shape references); partial player/car struct
      with declared fields only where the cluster reads them (0x380/0x3A3
      offsets; cite modeldat.h:403-467 CAR_DATA as reference-not-layout);
      `Track_Data` verbatim (checkpoint.h:101-109); `extern` decls matching
      the game symbol table. Exclude arcade-hardware fields (coin/cabinet
      switches, ZOID objnum, Visual, gnState) with a header comment saying
      why.
- [ ] T019 [US3] Iterate: re-run the T011 cluster probe after each
      game_types.h addition; grow definitions until SC-001's threshold is
      reachable, keeping every definition provenance-stamped and
      cluster-justified; re-run the T009/SC-005 static byte-identity test
      and the T017 histogram once at the end to demonstrate run-over-run
      progress (spec US2/AC3). Hand the final compile list to T014.

**Checkpoint**: context bootstrap complete; SC-001/SC-005 satisfied.

---

## Phase 6: Polish & Cross-Cutting

- [ ] T020 [P] Promotion firewall (FR-010/SC-006): in
      `tools/conveyor/pipeline/lock.py` (`add`) and
      `tools/conveyor/pipeline/promote.py` (`run`/`batch` entry points),
      resolve the function's `n64_target.population` and reject `extracted`
      with `error: <target_id> is extracted-population — evidence-only
      (005/FR-010)`; unit-test the guard in
      `tests/conveyor/test_autodecomp_population.py`; verify
      `promotion_record` contains no extracted entries after the feature's
      runs.
- [ ] T021 [P] Ops documentation: add the 005 section to
      `tools/conveyor/README.md` (extent repair semantics + report line,
      `--population`/`--targets` usage, histogram artifacts, firewall), and
      note in `CLAUDE.md`'s conveyor section that extracted extents are now
      scan-derived (info.txt sizes no longer trusted for this population).
- [ ] T022 Full local suite green: `pytest tests/conveyor -m "not
      node_required"` (existing + `test_extent_scan.py`, `test_disasm.py`,
      `test_autodecomp_population.py`); then walk quickstart.md §1–§6
      end-to-end and fill in every "actuals" placeholder.

---

## Dependencies & Execution Strategy

```
T001 → T002..T009 (Foundational; T002∥T006 test-first, T003→T004→T005,
                    T006→T007→T008→T009)
        ├─ US1: T010 → T011 → T012 → T013   (pipe works; scores recorded)
        ├─ US2: T015 → T016 → T017          (independent of US1/US3)
        └─ US3: T018 → T019 (driven by T011 blocker list)
US3 T019 ──→ US1 T014 (SC-001 final acceptance)
T020, T021 anytime after Foundational; T022 last.
```

- **MVP scope**: Foundational + US1 + US3 (the spec declares US3 the
  enabling content for US1's acceptance number). US2 is one command reusing
  US1 machinery and can land before or after the US3 iteration loop.
- **Parallel opportunities**: T002∥T006 (different test files); after T009:
  US1's T010 ∥ US2's T015 ∥ US3's T018 skeleton; T020∥T021 during any wait
  on builder scoring (T012).
- **Builder dependency**: only T012 and T014's scoring half need
  coordinator+watchman; everything else is Pi-local — sequence builder work
  into one window if watchman is flaky (see memory: it times out under
  container load).
- **Evidence-safety ordering**: T005 (oracle-verified extents) MUST precede
  any seeding/scoring; do not submit cluster seeds against pre-repair
  objects.

## Task counts

- Total: 22 (Setup 1, Foundational 8, US1 5, US2 3, US3 2, Polish 3)
- Independent test criteria: US1 quickstart §3–4; US2 quickstart §2; US3
  blocker-list shrinkage + SC-005 byte-identity.
