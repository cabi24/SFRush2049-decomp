# Results cleanup: matching with the actual D91A0 caller

**Match candidate:** `render_results_screen`, `0x800D7DC4`, **196 bytes**.
Canonical result: **0/49 differing words**, exactly 196 emitted bytes, native
64-byte frame, zero extra words, unresolved/unverified relocations, or errors.
Only this body is claimed. Acceptance and integration remain with the checker.

## Source and baseline

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
`rows.c` is unchanged complete source from
`cloud/work/ipa-groups/codex_result_rows_a118/group.c`. Its preserved status
explicitly records the missing second genuine caller, `func_800D91A0`.

The A118 baseline retains only the real results entry. IDO inlines its sole
row-helper call, leaving no native row-helper body; the results entry scores
47/49 differing words with 25 extra nonzero words. The new group supplies the
complete actual D91A0 caller from [PR #262](https://github.com/cabi24/SFRush2049-decomp/pull/262),
commit `9873c196b3047e6c48df306fb4c09a556c8d6153`, together with its real D816C
callee and four existing matrix/predicate bodies. All source is included here;
there is no runtime dependency on that branch being merged.

No row-cleanup body, fake caller, parameter carrier, inlining-threshold block,
compiler flag, protected target, or scorer was altered. The new real call
prevents the sole-use inlining and exposes the intended private
helper interface naturally.

## Complete observed result

| Body | Different words | ELF bytes | Extra nonzero | Unverified relocs |
| --- | ---: | ---: | ---: | ---: |
| render_results_screen, claimed | 0 / 49 | 196 | 0 | 0 |
| render_replay_ui, unclaimed | 2 / 33 | 132 | 0 | 0 |
| func_800D816C, context | 654 / 699 | 2688 | 0 | 10 |
| func_800D91A0, context | 905 / 965 | 4156 | 72 | 6 |

All have zero unresolved relocations and score errors. The row helper now has
its exact 132-byte extent and native 24-byte frame, but two words still differ.
It is not a matching claim. All four reused matrix/predicate helpers retain
their prior MATCH results; no duplicate matching credit is claimed.

The results body clears four rows, invokes the real 17-record cleanup for
each, releases the results sprite if present, and clears the final state flag.
The row helper retains the signed-byte row, 64-byte record stride,
signed-half object callback ID, and original full-word ID reset to -1.

The large new caller/callee sources remain research reconstructions. Original
TU composition, their private-register interfaces, remaining external helpers,
and their literal relocations are unresolved. Their detailed provenance and
seed-correction assumptions are retained in PR #262; the corrected advancing
76-byte input-record pointer is present here. This group must not replace
unclaimed helpers' accepted production recipes.

## Actual recipe

`-g0 -O3 -mips2 -G 0 -non_shared`, standard canonical `compile_group`, including
as1 `-r4300_mul`. `group.json` preserves the real D91A0 and results roots and
lists only `render_results_screen` under claims.

```sh
python3 cloud/work/frontier/dot_results_cleanup_20261006/repro.py
```

The reproduction compiles the actual A118 baseline and the complete real
caller group, printing canonical scores, ELF extents and frames. This is
compile/scoring evidence only; no behavior harness, extra independent review,
full tests, CI wait, or ROM integration precedes the draft publication.
