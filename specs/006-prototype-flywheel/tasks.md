# Tasks: Track B Prototype Layer & Seed Flywheel

**Input**: Design documents from `/specs/006-prototype-flywheel/`
**Prerequisites**: plan.md, spec.md, research.md (D1–D6), data-model.md,
contracts/declaration-layer.md, contracts/hygiene-and-flywheel.md,
quickstart.md. Baseline: 2026-07-19 histogram (42/597/49/0/197; 289
`func_`-shaped blockers).

**Tests**: Included — both contracts define regression obligations
(no-overreach cases, priority-ladder assert, byte-stability).

**Organization**: US2 (hygiene/buckets) executes FIRST despite being P2 —
the plan's phase note: generation pass 1 should run against final hygiene
so the layer isn't generated twice. Story phases below are in priority
order per the template; the Dependencies section gives true execution
order.

## Format: `[ID] [P?] [Story] Description`

---

## Phase 1: Setup

- [ ] T001 Preflight (no code): verify the 2026-07-19 baseline reproduces —
      run the full histogram once, confirm buckets
      `42/597/49/0/197` and `func_`-shaped blocker count 289; copy
      `build/m2c_histogram.json` to
      `specs/006-prototype-flywheel/research/baseline.json` (committed) as
      the diff anchor for FR-010/SC-001. Abort the feature on mismatch.

---

## Phase 2: User Story 2 — Honest seed hygiene & buckets (P2, executes first)

**Goal**: artifact-class rules, `partial_decomp`, protected instrument.
**Independent Test**: quickstart §1.

- [ ] T002 [P] [US2] Tests first in
      `tests/conveyor/test_autodecomp_population.py`: saved-register rule
      (fires on undeclared use; not when declared; not on near-miss
      names), scalar-cursor generalization (s32-local case; 005 u8* cases
      still pass; no-overreach on typed struct pointers),
      `partial_decomp` precedence (M2C_ERROR in raw output ⇒ bucket wins
      over compiled/blocked; checked pre-hygiene), probe routing (scoped
      run writes `build/m2c_probe.json`, never the population artifact),
      six-bucket exclusivity/coverage/determinism per
      contracts/hygiene-and-flywheel.md §1–5.
- [ ] T003 [US2] Implement in `tools/conveyor/pipeline/autodecomp.py`:
      `_clean_m2c` saved-register injection + scalar-cursor
      generalization (contract §1–2); `partial_decomp` classification on
      raw m2c output before hygiene (§3–4); artifact routing with
      `run.population_complete` marker (§5). Make T002 pass.
- [ ] T004 [US2] Implement `clusters diff` view (contract §6): per-target
      bucket movements, bucket-count deltas, blocker-class deltas,
      deterministic ordering; unit test with two fixture JSONs.
- [ ] T005 [US2] Full-population run ×2 on the Pi (quickstart §1): verify
      six-bucket determinism, counts sum to 885, `partial_decomp`
      absorbs the M2C_ERROR class, scoped probe leaves the population
      artifact untouched; record actuals in quickstart.md §1 and diff
      against `research/baseline.json`.

**Checkpoint**: instrument hardened; hygiene final — generation may begin.

---

## Phase 3: User Story 1 — Generated declaration layer (P1) 🎯 MVP

**Goal**: `func_`/named-callee blockers collapse; compiled ≥ 200.
**Independent Test**: quickstart §2.

- [ ] T006 [P] [US1] Tests first in `tests/conveyor/test_protos.py`
      against contracts/declaration-layer.md: signature capture from a
      fixture m2c definition line; fallback `s32 name();` for
      underivable callees; precedence filter omits hand-context and
      static-target names with reasons recorded; sorted, byte-stable
      emission (stamp excluded from content stability); coverage
      invariant (every referenced known-target callee in exactly one of
      declarations/omitted).
- [ ] T007 [US1] Implement `tools/conveyor/pipeline/protos.py`
      (`generate` command): reuse the histogram's per-target m2c pass to
      capture own-definition signatures (research D1), two-pass
      generation exactly (D2), precedence-by-omission filter over the
      preprocessed hand context (D3), emit `build/m2c_protos.h` +
      `build/m2c_protos.json` per data-model.md. Make T006 pass.
- [ ] T008 [US1] Wire consumption in `autodecomp.py`: `_context()`
      includes `build/m2c_protos.h` last when present; its content sha
      joins the context-sha cache keys (layer change ⇒ downstream
      regeneration). Extend the SC-005 body-identity test to run with a
      populated layer present (contract §6).
- [ ] T009 [US1] Acceptance run (quickstart §2): double `generate`
      byte-stability; full histogram → `func_<addr>` blockers for known
      targets = 0, compiled ≥ 200 (SC-001), zero redefinition-class
      errors (SC-002); `clusters diff` against `research/baseline.json`
      attributes the movement; record actuals in quickstart.md §2. If
      compiled lands < 200, STOP and append the residual blocker-class
      analysis to a new `research/t009-shortfall.md` instead of forcing
      context growth (hand-typing is out of scope).

**Checkpoint**: SC-001/SC-002 decided on evidence.

---

## Phase 4: User Story 3 — Unattended flywheel (P3)

**Goal**: every compiling seed gains score evidence without operator
action; Track A unharmed.
**Independent Test**: quickstart §4.

- [ ] T010 [P] [US3] Tests first: flywheel selection logic (compiled ∧ no
      score evidence ⇒ submit; any score evidence ⇒ never resubmit),
      refusal of non-`population_complete` histogram input, and the
      priority-ladder assert (flywheel constant > every static-path
      constant) per contracts/hygiene-and-flywheel.md §7–8, with a
      DB-fixture test in `tests/conveyor/test_autodecomp_population.py`
      or a new `test_flywheel.py`.
- [ ] T011 [US3] Implement the flywheel cycle step in
      `tools/conveyor/pipeline/farm.py` (priority 60, standard budget,
      existing `submit_one`/harvest paths) and the
      `extracted: compiled N, scored M, in_search K` line in the standard
      report (`cli report`/`status.py`). Make T010 pass.
- [ ] T012 [US3] Unattended window (quickstart §4, needs coordinator +
      watchman): start `farm run`, walk away for one window, then verify
      SC-004 (scored == compiled), SC-005 (no static job displaced —
      queue lease timestamps), SC-006 (zero extracted rows in
      `promotion_record`); record actuals in quickstart.md §4.

---

## Phase 5: Polish & Cross-Cutting

- [ ] T013 [P] Ops docs: 006 section in `tools/conveyor/README.md`
      (generate command, six buckets, probe vs instrument paths, diff
      view, flywheel + priority ladder) and a CLAUDE.md conveyor note.
- [ ] T014 Full local suite green
      (`pytest tests/conveyor -m "not node_required"`) and quickstart
      §1–§3 walkthrough with every actuals placeholder filled; §4 filled
      after T012's window.

---

## Dependencies & Execution Strategy

```
T001 → US2: T002 → T003 → T004 → T005      (hygiene first — plan phase note)
        └→ US1: T006 → T007 → T008 → T009   (generation against final hygiene)
             └→ US3: T010 → T011 → T012     (flywheel needs a compiled set)
T013 anytime after T005; T014 last (T012's §4 actuals may trail).
```

- **MVP scope**: T001–T009 (hardened instrument + declaration layer =
  SC-001/SC-002/SC-003). The flywheel converts it into standing evidence.
- **Parallel opportunities**: T002∥T006∥T010 are test-authoring in
  different files; T004∥T003 after T002; T013 anytime late.
- **Builder dependency**: only T012. Everything else Pi-local.
- **Stop rules**: T009 has an explicit shortfall stop (evidence over
  forcing); all executors: never bypass the pre-commit hook.

## Task counts

- Total: 14 (Setup 1, US2 4, US1 4, US3 3, Polish 2)
- Independent test criteria: US2 quickstart §1; US1 §2; US3 §4.
