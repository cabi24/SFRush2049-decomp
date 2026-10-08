# Rush hypothesis batch

Status: completed. 10 variants recorded. No source was adopted; strict exact candidates require independent checker and image/ROM gates.

Ranking uses differing + extra words, with unresolved/unverified/error results blocked. Equal scores do not establish identical programs.

| Candidate | Status | Differing | Extra | ELF group |
|---|---|---:|---:|---|
| Baseline | scored_mismatch | 4 | 0 | control |
| C01 | scored_mismatch | 8 | 0 | 1 |
| C02 | scored_mismatch | 29 | 0 | 2 |
| S03 | scored_mismatch | 440 | 1 | 10 |

Expected-effect observations are diagnostic only. Missing measurements remain UNKNOWN. LLM token/active-time accounting and useful-finding counts require human review; no speedup claim.

## Timing

UTC boundaries and monotonic durations are in events.jsonl. Durations below are summed span/process wall time, not CPU time; nested and concurrent stages overlap and must not be added together.
Total run wall time: 13.286 seconds; 2026-10-07T17:16:03.496117+00:00 to 2026-10-07T17:16:16.782532+00:00.
- preflight_validation: 0.049 s across 1 completed span(s)
- freeze: 0.519 s across 1 completed span(s)
- target_preparation: 0.365 s across 1 completed span(s)
- compiler_process: 0.225 s across 12 completed span(s)
- baseline_compile: 0.566 s across 2 completed span(s)
- strict_score: 5.233 s across 13 completed span(s)
- baseline_controls: 2.992 s across 1 completed span(s)
- variant_compile_and_workbench: 3.189 s across 10 completed span(s)
- campaign: 2.372 s across 1 completed span(s)
- elf_dedup: 0.000 s across 1 completed span(s)
- diagnose: 3.036 s across 4 completed span(s)
- final_integrity_verification: 0.122 s across 1 completed span(s)
- report: 0.001 s across 1 completed span(s)
- batch: 13.286 s across 1 completed span(s)
