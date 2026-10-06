# Runtime A real parent: two new local group matches

This extends the context explored in [PR #190](https://github.com/cabi24/SFRush2049-decomp/pull/190).
New local whole-program O3 candidates:

- `func_8039B214`: strict MATCH, 209/209 words, 836 bytes, `[0x8039B214,0x8039B558)`
- `func_8039A448`: strict MATCH, 35/35 words, 140 bytes, `[0x8039A448,0x8039A4D4)`

Both have zero differing, unresolved, unverified or extra words. Normal O3
flags and mandatory `r4300_mul` are used. The new candidate total is 976 bytes;
this is not accepted cartridge coverage.

## Actual context and progress

The complete native-derived `func_8039C140` body is the real outer caller of
both B214 and BE48, with calls at +0x1FC and +0xD0. Its complete control flow
includes initialization, the four-option selector, modal/code editing,
configuration selection, player checks and final state transitions.

Adding this genuine parent improves B214 from 126/209 differences to five
register-only differences. Its signed input-word contract closes those final
five. The real A448 option predicate improves from its old standalone eight-word
residual to one load-width difference; the actual signed-byte access to
D_803B3428 closes it. No dummy caller, fabricated pressure, helper padding,
forced register, private compiler flag or assembly is used.

All included bodies are explicit in group.json. Only B214 and A448 are new
claims. B120 remains a strict 61/61 match but was already reported in #190 and
is matching context here. BE48 remains 129/190 words off, three extra words and
an unpaired HI16 relocation in its own unclaimed extent. C140 remains 484/496
off with incomplete enclosing/private ABI context. Neither is claimed or
eligible to replace native code from this result.

The actual local returning sign_extend_call research definition from #190 is
retained because BE48 consumes its allocator result. Production sources and
locks are unchanged. The BE48 float storage at D_803B95B4 and C140 float storage
at D_803B95B8 remain external native anchors in unclaimed context; their values
and original ownership are not invented. Record views encode observed offsets
and strides, not recovered original type names. The genuine byte-copy helper's
pointer return and matrix helper's array-pointer parameter are also retained.

## Integration boundary

This context supersedes #190's narrower context if the checker elects to use
it; do not install the two groups independently or count B120 twice. Both PRs
remain available for the checker's decision. The real parent itself and its
remaining callees are not a fully matched or verified module. Source and local
matching evidence only are supplied; no test suite, behavior proof, own-data
acceptance, image/ROM gate or safe-splice claim is made.

Reproduce from repository root with documented IDO:

```
python3 tools/cloud/score.py group cloud/work/frontier/dot_runtime_a_config_parent_group_20261006 --claims --targets asm/us/ovl_a
```

Omit --claims to view all runtime member scores. The returning wrapper is
main-blob context and is not an image-A target; its informational target lookup
does not gate the two declared runtime claims. Neither new candidate belongs
in standalone cloud/matches. Fixed base is
`f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`. Independent validation, integration
and merging remain with the checker.
