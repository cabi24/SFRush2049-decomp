# Rush hypothesis batch

Status: completed. 15 variants recorded. No source was adopted; strict exact candidates require independent checker and image/ROM gates.

Ranking uses differing + extra words, with unresolved/unverified/error results blocked. Equal scores do not establish identical programs.

| Candidate | Status | Differing | Extra | ELF group |
|---|---|---:|---:|---|
| Baseline | scored_mismatch | 4 | 0 | control |
| H05 | scored_mismatch | 4 | 0 | 5 |
| H07 | scored_mismatch | 69 | 0 | 7 |
| H12 | scored_mismatch | 440 | 0 | 12 |
| H01 | scored_mismatch | 355 | 1 | 1 |

Expected-effect observations are diagnostic only. Missing measurements remain UNKNOWN. LLM token/active-time accounting and useful-finding counts require human review; no speedup claim.

## Timing

UTC boundaries and monotonic durations are in events.jsonl. Durations below are summed span/process wall time, not CPU time; nested and concurrent stages overlap and must not be added together.
Total run wall time: 16.051 seconds; 2026-10-07T19:51:02.743514+00:00 to 2026-10-07T19:51:18.794048+00:00.
- preflight_validation: 0.043 s across 1 completed span(s)
- freeze: 0.439 s across 1 completed span(s)
- target_preparation: 0.404 s across 1 completed span(s)
- compiler_process: 0.245 s across 17 completed span(s)
- baseline_compile: 0.424 s across 2 completed span(s)
- strict_score: 6.584 s across 18 completed span(s)
- baseline_controls: 2.645 s across 1 completed span(s)
- variant_compile_and_workbench: 3.925 s across 15 completed span(s)
- campaign: 2.905 s across 1 completed span(s)
- elf_dedup: 0.000 s across 1 completed span(s)
- diagnose: 3.414 s across 5 completed span(s)
- final_integrity_verification: 0.106 s across 1 completed span(s)
- report: 0.001 s across 1 completed span(s)
- batch: 16.050 s across 1 completed span(s)
