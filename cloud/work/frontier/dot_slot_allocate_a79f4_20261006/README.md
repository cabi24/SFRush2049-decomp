# Display-slot allocator: three-word schedule residual

Observed local research candidate for `func_800A79F4` (240 bytes).
Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Prior source: `cloud/work/frontier/w7c/func_800A79F4/best_4words_storeswap.c`.

The canonical positional comparison improves **4/60 to 3/60 differing words**.
The width store occurs before the height-minus-one store, while the width-minus-one
store occurs after the two state-byte clears. This gives the target's width/height
register allocation. The remaining three differences are only the placement of
the width-minus-one store relative to those two independent state-byte clears.

The complete body remains 240 bytes, with zero unresolved/unverified references,
relocation errors or extra nonzero words. Still a NONMATCH; no accepted coverage
or ROM-integration claim. The function's seven actual parameters, slot structure,
200-slot bound and writes are unchanged. There are no dummy locals, unused reads,
volatile qualifiers, extra helpers, stand-in callers or forced registers.
A bounded set of consumed post-decrement spellings did not improve the result;
the delivered source uses ordinary assignments.

## Reproduce

With IDO 5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py fn cloud/work/frontier/dot_slot_allocate_a79f4_20261006/candidate.c func_800A79F4 --flags '-g0 -O3 -mips2 -G 0 -non_shared'
```

Those are the exact source flags; the canonical single-file scorer adds
`-Wab,-r4300_mul`. No real caller context is required for this local standalone
comparison. No broad tests, independent acceptance replay, image/ROM integration
or CI watching was performed. The independent checker owns acceptance and merging.
