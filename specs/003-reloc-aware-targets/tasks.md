# Tasks: Relocation-Aware Target Assembly

**Input**: Design documents from `/specs/003-reloc-aware-targets/`
**Prerequisites**: plan.md, spec.md, research.md, data-model.md, contracts/, quickstart.md

**Tests**: included (project convention; 99 local tests today must stay green).

**Organization**: US1 (reloc-aware assembly + gate) and US2 (supersession) are
implemented in that order but verified together; US3 is the end-to-end payoff run.
The toolkit rebuild happens once, inside the acceptance phase.

**Ground rules for the implementer** (in addition to 002's, which all still apply):

- Reuse `jobs/scoring.py`'s `_parse_text_words` / `_parse_relocs` / `_masked_diff`
  for the gate — importing node-side pure helpers on the Pi is established
  practice (`test_reloc_blind.py` does it). Do NOT reimplement the mask logic.
- `matrix_entry` is derived state; result blobs and `work_unit` are the audit
  trail. The purge deletes only `matrix_entry` rows. If you find yourself
  deleting blobs or work_units, stop.
- Never special-case the 19 flagged targets or the 12 locked functions anywhere —
  they upgrade/verify through the ordinary paths or the feature has failed.
- The live DB rules from 002 hold: additive idempotent migrations only; rehearse
  against a copy.

## Phase 1: Setup

- [ ] T001 Add idempotent migrations to `tools/conveyor/coordinator/db.py`:
  `n64_target.tier TEXT NOT NULL DEFAULT 'raw_word'`, `n64_target.gate_reason
  TEXT`, `matrix_entry.target_o_sha TEXT` — same `PRAGMA table_info`-guarded
  pattern as 002's `_COLUMN_MIGRATIONS`. Acceptance: fresh DB and a copy of the
  live DB both connect twice cleanly and expose the columns.

---

## Phase 2: User Story 1 — Targets carry real relocations (P1) 🎯 MVP

**Goal**: static targets assembled from splat asm with relocations, behind the
round-trip gate, with tier/reason recorded and raw-word fallback.

**Independent test**: unit fixtures + the real-assembler integration test, then a
live `matrix extract` showing ≥150 reloc_aware and a relocation-bearing
osCreateMesgQueue object (quickstart §2).

- [ ] T002 [US1] Implement `index_asm_regions()` in
  `tools/conveyor/pipeline/targets.py` per `contracts/target-assembly.md`:
  parse `asm/us/*.s` glabel/endlabel regions keyed by first-instruction vaddr,
  carrying region text lines and ROM words.
- [ ] T003 [US1] Implement `assemble_region(region, target_id, out_o)` in
  `tools/conveyor/pipeline/targets.py`: emit the contract's exact preamble +
  comment-stripped instruction lines (interior labels kept verbatim), assemble
  with the existing `mips-linux-gnu-as` invocation; raise on assembler failure
  with the first stderr line preserved for `gate_reason`.
- [ ] T004 [US1] Implement `gate_target(rom_words, new_o)` in
  `tools/conveyor/pipeline/targets.py`: objdump the new object, mask both word
  lists at the object's own reloc sites (imported scoring helpers), fail with
  `length_mismatch`/`word_mismatch@<i>` reasons per the contract.
- [ ] T005 [US1] Wire the tiered flow into `populate()` in
  `tools/conveyor/pipeline/targets.py`: static targets attempt region→assemble→
  gate with raw-word fallback and `tier`/`gate_reason` recording; dynamic
  targets untouched (tier `raw_word`); print the tier/fallback summary lines
  from data-model.md's coverage report shape. (Supersession purge lands in T008;
  keep the sha-compare hook point ready.)
- [ ] T006 [P] [US1] Write `tests/conveyor/unit/test_target_assembly.py` per the
  contract's fixture list: region parsing (synthetic two-region file + the real
  `asm/us/7600.s` excerpt), gate pass/word_mismatch/length_mismatch on fixture
  words, and the skipif-guarded real-assembler integration test asserting the
  osCreateMesgQueue region assembles to 12 words with HI16 relocs at indices
  0,1 and LO16 at 2,3 and passes the gate against ROM words.
- [ ] T007 [US1] Determinism check in the same test file: assembling the same
  region twice yields byte-identical objects; `populate()` run twice against a
  tmp store changes nothing on the second pass (fabricate a one-target inventory
  in tmp dirs — do not touch `~/.conveyor`).

**Checkpoint**: local suite green; extraction logic proven on fixtures + one real
function; live run deferred to Phase 4 (needs the DB migration applied and the
purge in place first).

---

## Phase 3: User Story 2 — Stale evidence superseded, never mixed (P2)

**Goal**: target-object attribution end to end and purge-on-change, per
`contracts/evidence-supersession.md`.

**Independent test**: unit tests around purge/reschedule/attribution; live proof
happens in Phase 4's re-score.

- [ ] T008 [US2] Implement the supersession purge in
  `tools/conveyor/pipeline/targets.py` `populate()`: when a target's computed
  `target_o_sha` differs from the stored value, `DELETE FROM matrix_entry WHERE
  target_id=?` in the same transaction as the row update; count and print
  `superseded: <n> targets, <m> evidence rows purged`; first-build (NULL→sha)
  goes through the same path.
- [ ] T009 [US2] Add the attribution echo, submit side: manifest cell target
  entries gain `"target_o_sha"` in `tools/conveyor/pipeline/matrix.py`
  (cmd_submit), `tools/conveyor/pipeline/corpus.py` (cmd_submit), and
  `tools/conveyor/pipeline/lock.py` (verify_entry) — the sha of the exact bundled
  .o bytes.
- [ ] T010 [US2] Add the echo node side + ingest side: `tools/conveyor/jobs/
  compile_score.py` copies `target.get("target_o_sha")` into every result cell
  when present (ok and fail cells alike); `tools/conveyor/pipeline/matrix.py`
  cmd_ingest stores `cell.get("target_o_sha")` into the new column (NULL when
  absent, old blobs never rejected).
- [ ] T011 [US2] Add the SC-006 attribution check line to
  `tools/conveyor/pipeline/corpus.py` cmd_report:
  `attribution: <n> cells checked, <k> mismatched (expect 0)` comparing surviving
  `matrix_entry.target_o_sha` against current `n64_target.target_o_sha`
  (NULLs excluded from the mismatch count, counted separately as `legacy=<j>`).
- [ ] T012 [P] [US2] Write `tests/conveyor/unit/test_supersession.py` per the
  contract's post-conditions: purge deletes only the changed target's rows (all
  toolkits) and never touches work_unit; dry-run submit counts purged targets as
  new work while unchanged targets contribute none; ingest stores the echoed sha
  and NULL for legacy cells; flag interplay — a `reloc_only_diff` target's flag
  clears after purge (002 recompute) and follows the normal path after a true-0
  re-ingest.

**Checkpoint**: full local suite green; every piece in place for the live run.

---

## Phase 4: User Story 3 — The flagged nineteen upgrade automatically (P3)

**Goal**: end-to-end acceptance on real hardware (quickstart, measured numbers
recorded in place).

- [ ] T013 [US3] **Toolkit rebuild + smoke (node_required)**: quickstart §§0–1 —
  rebuild on watchman (ships the T010 echo), publish, pin; `cli smoke` must PASS
  (SC-005). Record the toolkit sha in quickstart.md.
- [ ] T014 [US3] **Live re-extraction (node not needed, DB is live)**: quickstart
  §2 — `matrix extract`; record tier counts (SC-001: reloc_aware ≥ 150),
  fallback reasons, supersede counts; run extract again and record the
  all-zeros/identical second pass (SC-007); spot-check osCreateMesgQueue's
  object shows 2×HI16 + 2×LO16 relocations.
- [ ] T015 [US3] **Re-score + upgrade run (node_required)**: quickstart §3 —
  corpus submit / ingest-results / report. Required: osCreateMesgQueue
  **true=0** (SC-002); record flagged 19 → <after> and the true-0 count
  (SC-003); attribution line reports 0 mismatched (SC-006); immediate resubmit
  is 100% cache. If osCreateMesgQueue does NOT reach 0, STOP per research D6 —
  debug the permuter's reloc handling on that single pairing on watchman before
  re-running anything at scale.
- [ ] T016 [US3] **Lock regression gate (node_required)**: quickstart §4 —
  `lock verify` re-proves all 12 locked functions at score 0 against reloc-aware
  targets (SC-004); `lock check` green. Any lock failure is a feature bug, not a
  lock bug — do not re-pin.

---

## Phase 5: Polish

- [ ] T017 [P] Document the tier system, gate, and supersession semantics in
  `tools/conveyor/README.md` (operating note: what `gate_reason` values mean,
  why `superseded` purges are normal after re-extraction) and add the feature
  summary to CLAUDE.md's conveyor section.
- [ ] T018 Full local suite green; commit on `003-reloc-aware-targets` with
  project conventions, staging only your files. Leave wiki/status updates to the
  reviewer.

---

## Dependencies

```text
T001 ─→ T005, T008, T010(ingest), T011
T002 ─→ T003 ─→ T004 ─→ T005 ─→ T007, T008
T006 alongside T002–T004   T012 alongside T008–T011
T008–T012 ─→ T013 ─→ T014 ─→ T015 ─→ T016 ─→ T017, T018
US1 ⊃ T002–T007   US2 ⊃ T008–T012   US3 ⊃ T013–T016
```

Parallel: T006 with T002–T004; T012 with T009–T011; T017 anytime after T015.

## Implementation strategy

MVP = Phases 1–2 (assembly + gate proven on fixtures and one real function).
Phase 3 is mandatory before any live extraction — running T014 without T008
would leave stale evidence mixed with new objects, the exact corruption this
feature exists to prevent. Phase 4 is one sitting with the pool up (~30 min of
wall time, mostly the re-score); its measured numbers are the acceptance
evidence. The arcade matrix refill is explicitly out of scope (rides with the
shim iteration).
