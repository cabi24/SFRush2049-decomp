# Rush hypothesis batch

Status: completed. 15 variants recorded. No source was adopted; strict exact candidates require independent checker and image/ROM gates.

Ranking uses differing + extra words, with unresolved/unverified/error results blocked. Equal scores do not establish identical programs.

| Candidate | Status | Differing | Extra | ELF group |
|---|---|---:|---:|---|
| Baseline | scored_mismatch | 533 | 0 | control |
| H08 | scored_mismatch | 511 | 0 | 8 |
| H12 | scored_mismatch | 531 | 0 | 12 |
| H09 | scored_mismatch | 541 | 0 | 9 |
| H01 | scored_mismatch | 539 | 0 | 1 |

Expected-effect observations are diagnostic only. Missing measurements remain UNKNOWN. LLM token/active-time accounting and useful-finding counts require human review; no speedup claim.

## Timing

UTC boundaries and monotonic durations are in events.jsonl. Durations below are summed span/process wall time, not CPU time; nested and concurrent stages overlap and must not be added together.
Total run wall time: 18.734 seconds; 2026-10-07T21:44:54.886382+00:00 to 2026-10-07T21:45:13.620771+00:00.
- preflight_validation: 0.053 s across 1 completed span(s)
- freeze: 0.481 s across 1 completed span(s)
- target_preparation: 0.401 s across 1 completed span(s)
- compiler_process: 0.316 s across 17 completed span(s)
- baseline_compile: 0.449 s across 2 completed span(s)
- strict_score: 7.393 s across 18 completed span(s)
- baseline_controls: 2.781 s across 1 completed span(s)
- variant_compile_and_workbench: 4.629 s across 15 completed span(s)
- campaign: 3.471 s across 1 completed span(s)
- elf_dedup: 0.000 s across 1 completed span(s)
- diagnose: 4.138 s across 5 completed span(s)
- final_integrity_verification: 0.113 s across 1 completed span(s)
- report: 0.001 s across 1 completed span(s)
- batch: 18.734 s across 1 completed span(s)
