# Tasks: Track B Population Closure

**Input**: `/specs/007-population-closure/` (spec, plan,
contracts/closure-and-datasyms.md). Baseline: 006 close-out
(60/298/281/49/0/197 over 885; 184 `func_` classes, ≥135 in-blob
unregistered; ~119 `x<addr>` symbols / ~84 targets).
**Prerequisite state**: 006 merged (or this branch carrying it); the 006
flywheel operational.

**Tests**: Included; the contract lists the unit-test obligations
explicitly.

## Format: `[ID] [P?] [Story] Description`

---

## Phase 1: Setup

- [ ] T001 Preflight: confirm the 006 close-out histogram is the current
      population artifact (buckets above) and copy it to
      `specs/007-population-closure/research/baseline.json` (committed
      diff anchor). Verify `build/game_code.bin` sha matches the recorded
      stamp. Abort on mismatch.

---

## Phase 2: User Story 1 — Callee closure (P1) 🎯 MVP

**Independent Test**: closure runs twice — first registers with
provenance, second registers zero; after layer regen no in-blob `func_`
blocker remains.

- [ ] T002 [P] [US1] Tests first in `tests/conveyor/test_closure.py` per
      the contract's unit-test obligations: jal/j decode incl. the
      `pc & 0xF0000000` page rule, candidate filter (in-blob, aligned,
      unknown, static-range exclusion), all four outcome classes,
      fixpoint termination, both caps as explicit outcomes, idempotency
      on a fixture DB, no-supersession-side-effects.
- [ ] T003 [US1] Implement `tools/conveyor/pipeline/closure.py` (`run`
      command) per contract §1–§6: raw-word decode → gate via 005
      `scan_extent` → register through the existing carve/assemble/store
      path with `gate_reason='discovered'` → iterate → emit
      `build/closure_report.json`. Make T002 pass.
- [ ] T004 [US1] Live closure run on the Pi ×2: record iteration/outcome
      counts and idempotency in quickstart-style actuals (create
      `specs/007-population-closure/quickstart.md` §1 with the commands
      and the actuals — mirror 005/006 format); expect ≥135 registered or
      classified with reasons.

**Checkpoint**: population enlarged and gated.

---

## Phase 3: User Story 2 — Generated data symbols (P2)

**Independent Test**: double run byte-stable; `x<addr>` class eliminated
in the next histogram; zero hand-table collisions.

- [ ] T005 [P] [US2] Tests first in `tests/conveyor/test_datasyms.py`:
      width rule (word>half>byte, f32/f64, integer-on-conflict with
      conflict recorded), access citations, byte-stability, hand-table
      omission, merged-lookup precedence in disasm, cache-key coverage
      (changing the generated table regenerates derivations).
- [ ] T006 [US2] Implement `tools/conveyor/pipeline/datasyms.py`
      (`generate` command) per contract §7–§9, and the `disasm.py` merged
      single lookup + `symbol_table_sha()` extension per §10. Make T005
      pass.
- [ ] T007 [US2] Extend `protos generate` with the datasyms externs
      section per §11 (subordinate to hand context; never touches
      `game_types.h`); extend `tests/conveyor/test_protos.py`
      accordingly; re-verify 006's zero-redefinition gate over the
      enlarged population.

---

## Phase 4: User Story 3 — Re-measure & absorb (P3)

**Independent Test**: histogram ×2 deterministic over the enlarged
population; diff attributes movement; flywheel scores new compiles
unattended.

- [ ] T008 [US3] Full regeneration sequence on the Pi: `datasyms
      generate` → `protos generate` (twice, byte-stability) → full
      histogram ×2 → `clusters diff research/baseline.json`. Gates:
      SC-001 (no in-blob `func_` blockers), SC-002 (`x<addr>`
      eliminated), SC-003 (coverage/determinism over enlarged
      population), SC-004 (compiled ≥ 200 — else STOP and write
      `research/t008-shortfall.md` with residual classes per FR-010; do
      not expand scope). Record actuals in quickstart.md.
- [ ] T009 [US3] Flywheel window over the enlarged compiled set
      (coordinator + watchman): verify unattended submission of new
      compiles, evidence dedupe, priority ladder, and SC-005 (100%
      scored coverage; zero extracted promotions; static bodies/
      throughput unchanged). Record §-actuals.

---

## Phase 5: Polish

- [ ] T010 [P] Ops docs: 007 section in `tools/conveyor/README.md`
      (closure run + report, datasyms generate, merged lookup, enlarged
      population semantics) and a CLAUDE.md note (population is now
      closure-derived; work-inventory names are historical).
- [ ] T011 Full local suite green + quickstart walkthrough complete;
      update the wiki status page at the milestone.

---

## Dependencies & Execution Strategy

```
T001 → US1: T002 → T003 → T004
        └→ US2: T005 → T006 → T007      (scan input = enlarged population)
             └→ US3: T008 → T009
T010 anytime after T004; T011 last.
```

- **MVP**: T001–T004 (the population becomes honest). US2 multiplies it;
  US3 measures it.
- **Model routing** (post-Fable): Codex for every task except gate
  reviews — each phase has machine-checkable oracles (idempotency,
  byte-stability, class elimination), so an Opus session reviews at the
  two checkpoints (after T004, after T008) using the contract oracles;
  escalate judgment questions to Opus with the stall-report pattern.
- **Stop rules**: T008's SC-004 stop; caps in closure are outcomes, not
  errors; never bypass the pre-commit hook.

## Task counts

Total: 11 (Setup 1, US1 3, US2 3, US3 2, Polish 2).
