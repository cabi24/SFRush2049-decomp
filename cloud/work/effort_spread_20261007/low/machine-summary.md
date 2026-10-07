# Rush hypothesis batch

Status: completed. 25 variants recorded. No source was adopted; strict exact candidates require independent checker and image/ROM gates.

Ranking uses differing + extra words, with unresolved/unverified/error results blocked. Equal scores do not establish identical programs.

| Candidate | Status | Differing | Extra | ELF group |
|---|---|---:|---:|---|
| Baseline | scored_mismatch | 4 | 0 | control |
| L16 | scored_mismatch | 4 | 0 | 16 |
| L18 | scored_mismatch | 4 | 0 | 18 |
| L04 | scored_mismatch | 437 | 0 | 4 |
| L01 | scored_mismatch | 380 | 0 | 1 |

Expected-effect observations are diagnostic only. Missing measurements remain UNKNOWN. LLM token/active-time accounting and useful-finding counts require human review; no speedup claim.

## Timing

UTC boundaries and monotonic durations are in events.jsonl. Durations below are summed span/process wall time, not CPU time; nested and concurrent stages overlap and must not be added together.
Total run wall time: 20.796 seconds; 2026-10-07T19:44:13.809764+00:00 to 2026-10-07T19:44:34.606169+00:00.
- preflight_validation: 0.048 s across 1 completed span(s)
- freeze: 0.436 s across 1 completed span(s)
- target_preparation: 0.365 s across 1 completed span(s)
- compiler_process: 0.449 s across 27 completed span(s)
- baseline_compile: 0.481 s across 2 completed span(s)
- strict_score: 9.848 s across 28 completed span(s)
- baseline_controls: 2.628 s across 1 completed span(s)
- variant_compile_and_workbench: 6.587 s across 25 completed span(s)
- campaign: 4.576 s across 1 completed span(s)
- elf_dedup: 0.000 s across 1 completed span(s)
- diagnose: 3.646 s across 5 completed span(s)
- final_integrity_verification: 0.115 s across 1 completed span(s)
- report: 0.001 s across 1 completed span(s)
- batch: 20.795 s across 1 completed span(s)
