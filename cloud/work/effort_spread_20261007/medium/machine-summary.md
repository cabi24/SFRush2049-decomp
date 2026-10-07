# Rush hypothesis batch

Status: completed. 20 variants recorded. No source was adopted; strict exact candidates require independent checker and image/ROM gates.

Ranking uses differing + extra words, with unresolved/unverified/error results blocked. Equal scores do not establish identical programs.

| Candidate | Status | Differing | Extra | ELF group |
|---|---|---:|---:|---|
| Baseline | scored_mismatch | 4 | 0 | control |
| M07 | scored_mismatch | 8 | 0 | 7 |
| M03 | scored_mismatch | 200 | 2 | 3 |
| M06 | scored_mismatch | 441 | 6 | 6 |
| M01 | scored_mismatch | 380 | 0 | 1 |

Expected-effect observations are diagnostic only. Missing measurements remain UNKNOWN. LLM token/active-time accounting and useful-finding counts require human review; no speedup claim.

## Timing

UTC boundaries and monotonic durations are in events.jsonl. Durations below are summed span/process wall time, not CPU time; nested and concurrent stages overlap and must not be added together.
Total run wall time: 18.917 seconds; 2026-10-07T19:45:57.221066+00:00 to 2026-10-07T19:46:16.137972+00:00.
- preflight_validation: 0.049 s across 1 completed span(s)
- freeze: 0.443 s across 1 completed span(s)
- target_preparation: 0.365 s across 1 completed span(s)
- compiler_process: 0.326 s across 22 completed span(s)
- baseline_compile: 0.402 s across 2 completed span(s)
- strict_score: 8.161 s across 23 completed span(s)
- baseline_controls: 2.617 s across 1 completed span(s)
- variant_compile_and_workbench: 4.929 s across 20 completed span(s)
- campaign: 3.522 s across 1 completed span(s)
- elf_dedup: 0.000 s across 1 completed span(s)
- diagnose: 3.458 s across 5 completed span(s)
- final_integrity_verification: 0.112 s across 1 completed span(s)
- report: 0.001 s across 1 completed span(s)
- batch: 18.916 s across 1 completed span(s)
