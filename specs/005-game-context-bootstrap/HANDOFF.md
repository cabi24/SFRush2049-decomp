# Handoff: 005-game-context-bootstrap implementation

Model routing (2026-07-16). Principle: Codex for anything with a normative
contract + mechanical gate (oracle table, byte-identity, bucket sums);
Claude (Sonnet) only for the judgment-shaped typing loop; Opus only on
escalation. Fable reviews at the two gates.

| Prompt | Tasks | Model | Gate |
|---|---|---|---|
| A1 — extent repair | T001–T005 | Codex | oracle table exact + `repaired 0` on re-run |
| A2 — disasm + plumbing | T006–T009 | Codex | test_disasm green + SC-005 byte-identity |
| B — histogram | T015–T017 | Codex | buckets sum to 885, two runs identical |
| C — cluster pipe + scoring | T010–T013 | Codex | scores in evidence store (needs watchman) |
| D — game types iteration | T018, T019, T014 | **Sonnet** (→Opus if SC-001 unreached after ~3 iterations) | ≥8/10 compile incl ≥4 large; SC-005 stays green |
| E — polish | T020–T022 | Codex | full pytest green, quickstart actuals filled |

Order: A1 → **Fable review gate** → A2 → (B ∥ C) → D → E → **Fable review gate**.
C's T012 and D's T014 scoring need the coordinator + watchman builder window;
everything else is Pi-local.

Rules for all executors, restated from the artifacts:
- Branch `005-game-context-bootstrap`. Read `specs/005-game-context-bootstrap/`
  (spec, plan, research, data-model, contracts/, quickstart, research/) before
  coding. Tick tasks.md checkboxes as tasks complete; commit per task or
  coherent group.
- Python 3.9 stdlib only in `tools/conveyor/`. Do not touch the static-path
  behavior (SC-005 test enforces), the toolkit, node agent, or job types.
- **Never submit seeds against pre-repair target objects** (T005 gate first).
- The pre-commit hook verifies the 22 locked functions — if it fails, the
  change broke evidence; stop and investigate, don't bypass.

The exact prompts live in the conversation log / are reproducible from
tasks.md; each prompt = "implement tasks TXXX–TYYY per specs/005…/tasks.md"
plus the gate to self-verify.
