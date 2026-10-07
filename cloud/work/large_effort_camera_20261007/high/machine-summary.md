# Rush hypothesis batch

Status: completed. 15 variants recorded. No source was adopted; strict exact candidates require independent checker and image/ROM gates.

Ranking uses differing + extra words, with unresolved/unverified/error results blocked. Equal scores do not establish identical programs.

| Candidate | Status | Differing | Extra | ELF group |
|---|---|---:|---:|---|
| Baseline | scored_mismatch | 533 | 0 | control |
| H02 | scored_mismatch | 533 | 0 | 2 |
| H03 | scored_mismatch | 533 | 0 | 3 |
| H01 | scored_mismatch | 546 | 0 | 1 |

Expected-effect observations are diagnostic only. Missing measurements remain UNKNOWN. LLM token/active-time accounting and useful-finding counts require human review; no speedup claim.

## Timing

UTC boundaries and monotonic durations are in events.jsonl. Durations below are summed span/process wall time, not CPU time; nested and concurrent stages overlap and must not be added together.
Total run wall time: 16.103 seconds; 2026-10-07T21:02:31.122546+00:00 to 2026-10-07T21:02:47.225635+00:00.
- preflight_validation: 0.051 s across 1 completed span(s)
- freeze: 0.455 s across 1 completed span(s)
- target_preparation: 0.402 s across 1 completed span(s)
- compiler_process: 0.298 s across 17 completed span(s)
- baseline_compile: 0.405 s across 2 completed span(s)
- strict_score: 6.759 s across 18 completed span(s)
- baseline_controls: 2.728 s across 1 completed span(s)
- variant_compile_and_workbench: 4.316 s across 15 completed span(s)
- campaign: 3.221 s across 1 completed span(s)
- elf_dedup: 0.000 s across 1 completed span(s)
- diagnose: 3.065 s across 4 completed span(s)
- final_integrity_verification: 0.119 s across 1 completed span(s)
- report: 0.001 s across 1 completed span(s)
- batch: 16.102 s across 1 completed span(s)
