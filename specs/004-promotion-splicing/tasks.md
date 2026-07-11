# Tasks: Promotion Splicing

**Input**: design docs in `/specs/004-promotion-splicing/`. Tests included
(project convention). 002/003 ground rules all apply (stdlib-only Pi tooling,
additive migrations, never `git add -A`, honest task status — blockers ≠ done).

**The one rule above all**: the full-ROM SHA-1 is the only promotion
authority. If a step tempts you to weaken, mock, or skip it, stop.

## Implementation status (2026-07-09, Opus)

**T001–T004 done and green** (119 local tests pass incl. 9 new in
`test_layout.py`). **T005–T014 BLOCKED** on a prerequisite that fails on contact
(HANDOFF rule 1 + "settled research fails → stop and report with evidence").

Done:
- **T001** asm-processor vendored (`tools/asm-processor/`, pinned from sm64's
  copy, sha recorded in its README); toy compile deferred to the builder.
- **T002** `promotion_record` migrations (source/flags/evidence/rom_tu),
  rehearsed on a live-DB copy.
- **T003/T004** `pipeline/layout.py` derive/report/coverage + tests. 87/88
  segments derive cleanly (only the synthetic dynamic entry refuses); 0x8800 =
  strchr/strlen/memcpy @ -O2; padding-aware tiling, deterministic map + hash.

**BLOCKER (T005/T006 go/no-go):** `layout convert` requires `make extract`
(splat re-split) to emit `nonmatchings` asm + regenerate the linker script
(D2/D3); rule 5 forbids hand-editing those generated files. **splat 0.37.1
cannot run against the current `symbol_addrs.us.txt`** — four independent, hard
error classes (multi-colon `arcade:` comment tokens ×158; invalid `type:data`;
duplicate addresses; duplicate names). Sanitizing enough to run would change
splat's disassembly vs the checked-in asm, so re-split cannot reproduce the ROM
baseline the hash-neutral skeleton (SC-001/FR-003) is proven against. Full
evidence in quickstart.md §2.

**Reviewer decision required** (either is a real project, not a T006 wrinkle):
1. Remediate the extraction inputs so splat re-split is idempotent with the
   checked-in asm (dedupe/retype `symbol_addrs`, reconcile `reloc_addrs`,
   confirm the splat/spimdisasm version that produced the current asm) — then
   T005–T014 proceed unchanged.
2. Approve a deviation from D2/D3 + rule 5: a targeted `convert` that reuses the
   known-good checked-in asm (splitting `asm/us/<seg>.s` into per-function
   `nonmatchings/…` files) and patches the one linker-script object reference,
   without re-running splat. Rule 5 explicitly names the linker script, so this
   needs sign-off, not an implementer's unilateral call.

**REVIEWER RESOLUTION (2026-07-10, Fable) — Option 1 executed; T005–T014 UNBLOCKED.**
The bar was reframed: asm-text idempotence with a new splat version is neither
attainable nor required — the ROM hash is the baseline. Done and proven live:

- `tools/sanitize_symbol_addrs.py` (new, idempotent, checked in) normalizes
  `symbol_addrs.us.txt` for splat: valid attrs kept, `type:data` dropped,
  colon-bearing prose de-colonized, 268 duplicate addr/name definitions
  commented out first-wins (1,817 kept).
- splat 0.41.0 installed in `~/.splat-venv` (system pip is PEP-668 managed);
  `make extract` now uses it via `SPLAT_PYTHON`.
- Re-split regenerates 86 asm files + linker script + auto-syms; three stale
  auto-names in the hand-written `asm/us/1000.s` (hasm = source, not
  generated) canonicalized (`func_800020F0`→`main` etc.).
- **`make test`: ROM matches** (SHA-1 exact) on the re-split baseline, and
  extraction is **idempotent** (split N and N+1 outputs hash-identical).
- 003 interplay verified: `matrix extract` superseded 134 targets / 630 rows
  (canonical names changed reloc-aware objects — by design), reloc_aware rose
  178→180, and **all 12 locks re-verified at score 0** on the pool.

Proceed with T005 (`layout convert` + `make extract` now work as D2/D3
assumed). Rule 5 stands unmodified.

Not started: T005–T014.

## Phase 1: Setup
- [ ] T001 Vendor asm-processor into `tools/asm-processor/` at a pinned commit
  (record sha in the README); crib the Makefile integration pattern from
  `reference/repos/sm64` or `mk64` (asm-processor wraps the IDO invocation for
  GLOBAL_ASM TUs). Acceptance: a toy GLOBAL_ASM TU compiles on watchman.
- [ ] T002 Additive migrations for `promotion_record` new columns
  (source/flags/evidence/rom_tu) per data-model.md, 002 pattern, rehearsed on
  a DB copy.

## Phase 2: US1 — layout map + hash-neutral scaffolding 🎯 MVP
- [ ] T003 [US1] Implement `layout derive|report` in
  `tools/conveyor/pipeline/layout.py` per contracts/layout-map.md (inputs:
  splat.us.yaml, 003 `index_asm_regions`, symbol_addrs, flag_registry;
  tiling/naming refusal rules; deterministic JSON).
- [ ] T004 [P] [US1] `tests/conveyor/unit/test_layout.py`: derivation on
  fixture inputs (clean tile, gap refusal, unnamed refusal, dynamic-range
  entry), determinism, coverage math.
- [x] T005 [US1] Implement `layout convert [--revert]`: yaml edit, splat
  re-split invocation, TU generation (exact format from data-model.md),
  dirty-tree refusal; Makefile: compile `src/rom/*.c` via asm-processor+IDO
  into O_FILES for converted segments; matching-build guard (research D7).
- [x] T006 [US1] **Walking skeleton (node_required)**: quickstart §§1–2 —
  convert 0x8800, build on watchman, **SHA-1 must match with zero
  promotions**. Record MEASURED. If it fails: STOP, report (this validates
  D2/D4 for real; expected wrinkles: late_rodata, macro.inc includes in
  nonmatchings asm — solve for this one segment before generalizing).

## Phase 3: US2 — the promotion transaction
- [ ] T007 [US2] Implement `promote.py` (library + CLI) exactly per
  contracts/promotion-transaction.md: preconditions with named remedies,
  splice with provenance header, build+gate (local IDO or --via-builder),
  lock migration, promotion_record, atomic commit/rollback.
- [ ] T008 [P] [US2] `tests/conveyor/unit/test_promote.py`: precondition
  refusals, splice text, rollback restores byte-identical TU, lock migration
  add+remove in one step, already-promoted refusal (build mocked; the real
  gate is Phase 4's job).
- [ ] T009 [US2] **strlen promotion + rollback drill (node_required)**:
  quickstart §3 — SC-002 and SC-003 proven live. Record MEASURED.

## Phase 4: US3 — batch, coverage, pipeline closure
- [ ] T010 [US3] `promote batch --locked` + convert the remaining segments
  holding locked functions (layout report names them); run the batch
  (node_required): SC-004 — 12/12 promoted, SHA-1 exact, locks green at new
  paths. Flag-conflict note: research D5 (os_sp mixed evidence) — resolve by
  the segment's single real flagset; record which.
- [ ] T011 [US3] `layout coverage` wired into `make progress` and the conveyor
  report (SC-005): linked-C functions/bytes, derived only.
- [ ] T012 [US3] Upgrade `jobs/verify_promote.py` to call the promote library
  (drop the work/matched.c stopgap); one conveyor-driven promotion end to end
  on the builder (SC-006). Toolkit rebuild required (jobs/ change) — batch it
  with any pending node-side changes.
- [ ] T013 [P] Docs: README operating note (convert → promote → coverage;
  refusal remedies), CLAUDE.md conveyor section update.
- [ ] T014 Full suite green; commit(s) per convention; quickstart MEASURED
  blanks all filled. Wiki/status is the reviewer's.

## Dependencies
T001,T002 → T005,T007. T003 → T004,T005 → T006 → T007 → T008,T009 → T010–T012 → T013,T014.
US1 ⊃ T003–T006; US2 ⊃ T007–T009; US3 ⊃ T010–T012.

## Strategy
MVP = Phases 1–2: the hash-neutral skeleton is the whole risk; everything
after is transactional plumbing. T006 is the go/no-go checkpoint — do not
convert a second segment before it passes.
