# Rush hypothesis batch

Status: completed. 10 variants recorded. No source was adopted; strict exact candidates require independent checker and image/ROM gates.

Ranking uses differing + extra words, with unresolved/unverified/error results blocked. Equal scores do not establish identical programs.

| Candidate | Status | Differing | Extra | ELF group |
|---|---|---:|---:|---|
| Baseline | scored_mismatch | 533 | 0 | control |
| X02 | scored_mismatch | 533 | 0 | 2 |
| X09 | scored_mismatch | 533 | 0 | 9 |
| X04 | scored_mismatch | 546 | 0 | 4 |
| X01 | scored_mismatch | 538 | 0 | 1 |

Expected-effect observations are diagnostic only. Missing measurements remain UNKNOWN. LLM token/active-time accounting and useful-finding counts require human review; no speedup claim.

## Timing

UTC boundaries and monotonic durations are in events.jsonl. Durations below are summed span/process wall time, not CPU time; nested and concurrent stages overlap and must not be added together.
Total run wall time: 14.244 seconds; 2026-10-07T21:06:24.690878+00:00 to 2026-10-07T21:06:38.934929+00:00.
- preflight_validation: 0.045 s across 1 completed span(s)
- freeze: 0.466 s across 1 completed span(s)
- target_preparation: 0.365 s across 1 completed span(s)
- compiler_process: 0.206 s across 12 completed span(s)
- baseline_compile: 0.434 s across 2 completed span(s)
- strict_score: 4.752 s across 13 completed span(s)
- baseline_controls: 2.711 s across 1 completed span(s)
- variant_compile_and_workbench: 2.975 s across 10 completed span(s)
- campaign: 2.320 s across 1 completed span(s)
- elf_dedup: 0.000 s across 1 completed span(s)
- diagnose: 4.243 s across 5 completed span(s)
- final_integrity_verification: 0.119 s across 1 completed span(s)
- report: 0.001 s across 1 completed span(s)
- batch: 14.243 s across 1 completed span(s)
