# Rush hypothesis batch

Status: completed. 10 variants recorded. No source was adopted; strict exact candidates require independent checker and image/ROM gates.

Ranking uses differing + extra words, with unresolved/unverified/error results blocked. Equal scores do not establish identical programs.

| Candidate | Status | Differing | Extra | ELF group |
|---|---|---:|---:|---|
| Baseline | scored_mismatch | 533 | 0 | control |
| X02 | scored_mismatch | 533 | 0 | 2 |
| X10 | scored_mismatch | 533 | 0 | 10 |
| X07 | scored_mismatch | 543 | 0 | 7 |
| X01 | scored_mismatch | 540 | 0 | 1 |

Expected-effect observations are diagnostic only. Missing measurements remain UNKNOWN. LLM token/active-time accounting and useful-finding counts require human review; no speedup claim.

## Timing

UTC boundaries and monotonic durations are in events.jsonl. Durations below are summed span/process wall time, not CPU time; nested and concurrent stages overlap and must not be added together.
Total run wall time: 14.185 seconds; 2026-10-07T21:45:15.395593+00:00 to 2026-10-07T21:45:29.580214+00:00.
- preflight_validation: 0.050 s across 1 completed span(s)
- freeze: 0.437 s across 1 completed span(s)
- target_preparation: 0.365 s across 1 completed span(s)
- compiler_process: 0.207 s across 12 completed span(s)
- baseline_compile: 0.465 s across 2 completed span(s)
- strict_score: 4.688 s across 13 completed span(s)
- baseline_controls: 2.778 s across 1 completed span(s)
- variant_compile_and_workbench: 2.791 s across 10 completed span(s)
- campaign: 2.204 s across 1 completed span(s)
- elf_dedup: 0.000 s across 1 completed span(s)
- diagnose: 3.731 s across 5 completed span(s)
- final_integrity_verification: 0.115 s across 1 completed span(s)
- report: 0.001 s across 1 completed span(s)
- batch: 14.184 s across 1 completed span(s)
