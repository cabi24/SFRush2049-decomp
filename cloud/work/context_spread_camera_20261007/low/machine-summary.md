# Rush hypothesis batch

Status: completed. 25 variants recorded. No source was adopted; strict exact candidates require independent checker and image/ROM gates.

Ranking uses differing + extra words, with unresolved/unverified/error results blocked. Equal scores do not establish identical programs.

| Candidate | Status | Differing | Extra | ELF group |
|---|---|---:|---:|---|
| Baseline | scored_mismatch | 533 | 0 | control |
| L10 | scored_mismatch | 534 | 0 | 10 |
| L12 | scored_mismatch | 534 | 0 | 12 |
| L15 | scored_mismatch | 540 | 0 | 15 |
| L01 | scored_mismatch | 539 | 0 | 1 |

Expected-effect observations are diagnostic only. Missing measurements remain UNKNOWN. LLM token/active-time accounting and useful-finding counts require human review; no speedup claim.

## Timing

UTC boundaries and monotonic durations are in events.jsonl. Durations below are summed span/process wall time, not CPU time; nested and concurrent stages overlap and must not be added together.
Total run wall time: 24.895 seconds; 2026-10-07T21:44:03.181263+00:00 to 2026-10-07T21:44:28.076017+00:00.
- preflight_validation: 0.066 s across 1 completed span(s)
- freeze: 0.492 s across 1 completed span(s)
- target_preparation: 0.502 s across 1 completed span(s)
- compiler_process: 0.473 s across 27 completed span(s)
- baseline_compile: 0.666 s across 2 completed span(s)
- strict_score: 10.875 s across 28 completed span(s)
- baseline_controls: 3.266 s across 1 completed span(s)
- variant_compile_and_workbench: 7.890 s across 25 completed span(s)
- campaign: 5.615 s across 1 completed span(s)
- elf_dedup: 0.000 s across 1 completed span(s)
- diagnose: 3.920 s across 5 completed span(s)
- final_integrity_verification: 0.117 s across 1 completed span(s)
- report: 0.001 s across 1 completed span(s)
- batch: 24.894 s across 1 completed span(s)
