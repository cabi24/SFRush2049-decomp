# Rush hypothesis batch

Status: completed. 10 variants recorded. No source was adopted; strict exact candidates require independent checker and image/ROM gates.

Ranking uses differing + extra words, with unresolved/unverified/error results blocked. Equal scores do not establish identical programs.

| Candidate | Status | Differing | Extra | ELF group |
|---|---|---:|---:|---|
| Baseline | scored_mismatch | 4 | 0 | control |
| X06 | scored_mismatch | 8 | 0 | 6 |
| X07 | scored_mismatch | 18 | 0 | 7 |
| X05 | scored_mismatch | 395 | 1 | 5 |
| X01 | scored_mismatch | 90 | 0 | 1 |

Expected-effect observations are diagnostic only. Missing measurements remain UNKNOWN. LLM token/active-time accounting and useful-finding counts require human review; no speedup claim.

## Timing

UTC boundaries and monotonic durations are in events.jsonl. Durations below are summed span/process wall time, not CPU time; nested and concurrent stages overlap and must not be added together.
Total run wall time: 13.244 seconds; 2026-10-07T19:48:05.200980+00:00 to 2026-10-07T19:48:18.444480+00:00.
- preflight_validation: 0.043 s across 1 completed span(s)
- freeze: 0.415 s across 1 completed span(s)
- target_preparation: 0.365 s across 1 completed span(s)
- compiler_process: 0.183 s across 12 completed span(s)
- baseline_compile: 0.412 s across 2 completed span(s)
- strict_score: 4.566 s across 13 completed span(s)
- baseline_controls: 2.520 s across 1 completed span(s)
- variant_compile_and_workbench: 2.524 s across 10 completed span(s)
- campaign: 1.867 s across 1 completed span(s)
- elf_dedup: 0.000 s across 1 completed span(s)
- diagnose: 3.535 s across 5 completed span(s)
- final_integrity_verification: 0.108 s across 1 completed span(s)
- report: 0.000 s across 1 completed span(s)
- batch: 13.243 s across 1 completed span(s)
