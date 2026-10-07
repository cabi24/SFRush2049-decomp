# Rush hypothesis batch

Status: completed. 20 variants recorded. No source was adopted; strict exact candidates require independent checker and image/ROM gates.

Ranking uses differing + extra words, with unresolved/unverified/error results blocked. Equal scores do not establish identical programs.

| Candidate | Status | Differing | Extra | ELF group |
|---|---|---:|---:|---|
| Baseline | scored_mismatch | 533 | 0 | control |
| M02 | scored_mismatch | 534 | 0 | 2 |
| M13 | scored_mismatch | 534 | 0 | 13 |
| M09 | scored_mismatch | 536 | 34 | 9 |
| M01 | scored_mismatch | 538 | 0 | 1 |

Expected-effect observations are diagnostic only. Missing measurements remain UNKNOWN. LLM token/active-time accounting and useful-finding counts require human review; no speedup claim.

## Timing

UTC boundaries and monotonic durations are in events.jsonl. Durations below are summed span/process wall time, not CPU time; nested and concurrent stages overlap and must not be added together.
Total run wall time: 21.595 seconds; 2026-10-07T21:44:31.000473+00:00 to 2026-10-07T21:44:52.595205+00:00.
- preflight_validation: 0.064 s across 1 completed span(s)
- freeze: 0.468 s across 1 completed span(s)
- target_preparation: 0.401 s across 1 completed span(s)
- compiler_process: 0.379 s across 22 completed span(s)
- baseline_compile: 0.497 s across 2 completed span(s)
- strict_score: 8.884 s across 23 completed span(s)
- baseline_controls: 2.978 s across 1 completed span(s)
- variant_compile_and_workbench: 6.126 s across 20 completed span(s)
- campaign: 4.415 s across 1 completed span(s)
- elf_dedup: 0.000 s across 1 completed span(s)
- diagnose: 3.936 s across 5 completed span(s)
- final_integrity_verification: 0.117 s across 1 completed span(s)
- report: 0.001 s across 1 completed span(s)
- batch: 21.594 s across 1 completed span(s)
