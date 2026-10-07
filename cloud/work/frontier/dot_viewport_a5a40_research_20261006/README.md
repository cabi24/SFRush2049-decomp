# A5A40 default viewport: typed contract and store-order research

Observed local research candidate, explicitly NONMATCH.
Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_800A5A40`, **252 bytes / 63 words**.
Baseline source: `cloud/work/dot_viewport_init/candidate.c`.

Canonical local comparison improves **15/63 to 14/63 differing words**.
The ELF body remains **248 bytes**, four bytes short of the target. There are
no unresolved/unverified references, relocation errors or extra nonzero words.
The baseline gives 15/63 under both its documented O2 flags and the O3 flags
used here. This is a small source-order improvement, not a complete match.

The candidate writes `bounds->top` before `bounds->left`; these independent
zero stores produce the observed one-word reduction. It also repairs a prior
source-contract error: the fourth `arb_rate_set` argument and `D_80154188` are
floating-point horizontal projection values, not an opaque context pointer.
Evidence is the real callee in `cloud/work/dot_viewport_rate_reopen/candidate.c`
and its call to `src/blob/exhaust_smoke_effect.c`. The 72-byte binding-record
view likewise comes from that genuine callee, replacing the prior 8-byte prefix
view. The contract/type repairs alone do not reduce the local mismatch score.
Names and recovered field meanings remain reconstruction hypotheses.

No artificial caller, unused argument, assembly, volatile access or padding is
introduced. No production source, accepted lock or coverage claim is changed.

## Reproduce

With IDO 5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py fn cloud/work/frontier/dot_viewport_a5a40_research_20261006/candidate.c func_800A5A40 --flags '-g0 -O3 -mips2 -G 0 -non_shared'
```

The canonical driver also passes mandatory `-Wab,-r4300_mul`.
Independent acceptance and ROM integration remain with the checker.
