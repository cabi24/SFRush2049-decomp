# Rush hypothesis batch

Status: completed. 25 variants recorded. No source was adopted; strict exact candidates require independent checker and image/ROM gates.

Ranking uses differing + extra words, with unresolved/unverified/error results blocked. Equal scores do not establish identical programs.

| Candidate | Status | Differing | Extra | ELF group |
|---|---|---:|---:|---|
| Baseline | scored_mismatch | 533 | 0 | control |
| L17 | scored_mismatch | 532 | 0 | 17 |
| L01 | scored_mismatch | 533 | 0 | 1 |
| L03 | scored_mismatch | 546 | 0 | 3 |

Expected-effect observations are diagnostic only. Missing measurements remain UNKNOWN. LLM token/active-time accounting and useful-finding counts require human review; no speedup claim.

## Timing

UTC boundaries and monotonic durations are in events.jsonl. Durations below are summed span/process wall time, not CPU time; nested and concurrent stages overlap and must not be added together.
Total run wall time: 21.711 seconds; 2026-10-07T20:41:04.319517+00:00 to 2026-10-07T20:41:26.030181+00:00.
- preflight_validation: 0.050 s across 1 completed span(s)
- freeze: 0.519 s across 1 completed span(s)
- target_preparation: 0.533 s across 1 completed span(s)
- compiler_process: 0.449 s across 27 completed span(s)
- baseline_compile: 0.465 s across 2 completed span(s)
- strict_score: 10.109 s across 28 completed span(s)
- baseline_controls: 2.863 s across 1 completed span(s)
- variant_compile_and_workbench: 7.386 s across 25 completed span(s)
- campaign: 5.283 s across 1 completed span(s)
- elf_dedup: 0.000 s across 1 completed span(s)
- diagnose: 3.135 s across 4 completed span(s)
- final_integrity_verification: 0.112 s across 1 completed span(s)
- report: 0.001 s across 1 completed span(s)
- batch: 21.710 s across 1 completed span(s)
