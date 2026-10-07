# Rush hypothesis batch

Status: completed. 20 variants recorded. No source was adopted; strict exact candidates require independent checker and image/ROM gates.

Ranking uses differing + extra words, with unresolved/unverified/error results blocked. Equal scores do not establish identical programs.

| Candidate | Status | Differing | Extra | ELF group |
|---|---|---:|---:|---|
| Baseline | scored_mismatch | 533 | 0 | control |
| M03 | scored_mismatch | 533 | 0 | 3 |
| M05 | scored_mismatch | 533 | 0 | 5 |
| M02 | scored_mismatch | 546 | 0 | 2 |
| M01 | scored_mismatch | 546 | 0 | 1 |

Expected-effect observations are diagnostic only. Missing measurements remain UNKNOWN. LLM token/active-time accounting and useful-finding counts require human review; no speedup claim.

## Timing

UTC boundaries and monotonic durations are in events.jsonl. Durations below are summed span/process wall time, not CPU time; nested and concurrent stages overlap and must not be added together.
Total run wall time: 19.026 seconds; 2026-10-07T20:41:48.310303+00:00 to 2026-10-07T20:42:07.336121+00:00.
- preflight_validation: 0.057 s across 1 completed span(s)
- freeze: 0.444 s across 1 completed span(s)
- target_preparation: 0.401 s across 1 completed span(s)
- compiler_process: 0.345 s across 22 completed span(s)
- baseline_compile: 0.449 s across 2 completed span(s)
- strict_score: 8.253 s across 23 completed span(s)
- baseline_controls: 2.829 s across 1 completed span(s)
- variant_compile_and_workbench: 5.234 s across 20 completed span(s)
- campaign: 3.870 s across 1 completed span(s)
- elf_dedup: 0.000 s across 1 completed span(s)
- diagnose: 3.826 s across 5 completed span(s)
- final_integrity_verification: 0.114 s across 1 completed span(s)
- report: 0.001 s across 1 completed span(s)
- batch: 19.025 s across 1 completed span(s)
