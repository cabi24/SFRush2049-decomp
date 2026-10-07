# Runtime A cleanup: 276-byte local group candidate

`func_8039B00C`, `[0x8039B00C,0x8039B120)`: **strict local MATCH,
69/69 words, 276 bytes**, normal whole-program O3 plus mandatory `r4300_mul`.
Zero differing, unresolved, unverified or extra words.

The first complete natural reconstruction matched in genuine C140 parent
context. A minimal two-function group reproduces it too: B00C and its complete
real caller C140. The caller remains an explicit 488/496-word nonmatch. All
other routines use real external declarations; there are no stand-in bodies.

B00C traverses twelve 64-byte records, conditionally releases live handles,
clears the slot state, releases the UI object and grouped widget, releases two
additional handles, clears ambient state, and resets the initialization byte.
Its data views state only observed offsets/strides. Callee contracts follow the
locked release/state helpers. No unused locals, dummy reads, artificial
volatility, private flags or assembly are used. There is no owned float data
in the claimed cleanup body.

The C140 context comes from the family explored in #190, #199 and #208, but this
minimal group contains none of their claimed helper bodies. Only B00C is a new
claim; no earlier bytes are counted again. The parent's native float anchor
and incomplete private ABI remain unclaimed. Do not treat the parent as a match
or infer that the complete runtime unit is safe to replace.

Reproduce from repository root with documented IDO:

```
python3 tools/cloud/score.py group cloud/work/frontier/dot_runtime_a_cleanup_group_20261006 --claims --targets asm/us/ovl_a
```

Source, context and observed local score only. No full tests, behavior proof,
image/compression/ROM gates or accepted coverage. Keep this group-only candidate
out of standalone cloud/matches. Fixed base:
`f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`. Independent checker owns validation,
integration and merging.
