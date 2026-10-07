# Replay-row cleanup: native byte-span traversal

**Match candidate: render_replay_ui, 0x800D7D40, 132 bytes.**
Canonical result: **0/33 differing words**, exactly 132 emitted bytes, native
24-byte frame, zero extra words, unresolved/unverified relocations, or errors.
Only the row helper is claimed. The independent checker owns acceptance and
ROM integration.

## Exact baseline and source change

Branch/tool base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
The stronger current baseline is [PR #281](https://github.com/cabi24/SFRush2049-decomp/pull/281),
commit `cb58780a682ffabc5a0e7e5fa57a3b4ec707e035`, using the complete real
D91A0 caller and the preserved A118 cleanup source. Its row helper scores
2/33; its results entry already matches.

The only C-body change is the existing loop's unit:

- Baseline: count 17 elements, incrementing the counter by 1.
- Candidate: traverse `sizeof(D_80111998[0])` bytes, incrementing by
  `sizeof(Record)` while advancing the same record pointer.

The actual row is 17 records of 64 bytes. The two observed residuals were
precisely the native limit 1088 versus 17, and step 64 versus 1. The changed
loop therefore retains the same 17 iterations and complete cleanup semantics,
while expressing the native byte-span traversal. No local or record capacity
was enlarged, and no padding, pressure, extra call or qualifier was added.

The signed-byte row, full-word object-ID check/reset, signed-half callback
ID, and final row-state clear are unchanged. The original A118 source and
its record declaration remain the provenance for these accesses.

## Real caller context and scope

All of the actual PR #281 caller context is included: D91A0, its D816C callee,
and four existing matrix/predicate bodies from [PR #262](https://github.com/cabi24/SFRush2049-decomp/pull/262),
commit `9873c196b3047e6c48df306fb4c09a556c8d6153`. There is no dependency on
those drafts being merged and no stand-in caller.

`render_results_screen` remains 0/49 with its exact 196-byte extent and
64-byte frame; that is existing #281 credit, not a new claim here. The four
previously accepted helper bodies also retain MATCH without new credit.
D816C remains 654/699 with ten unverified literal relocations; D91A0 remains
905/965 with 72 extra nonzero words and six unverified literals. The context
has no unresolved relocations or score errors in this composition, but is
not an original-TU or whole-group-match proof.

The large reconstruction's private interfaces, remaining external helpers,
literal placement and broad behavior remain unresolved, as documented in
#262. Do not substitute unclaimed context for accepted production recipes.

## Actual recipe

`-g0 -O3 -mips2 -G 0 -non_shared`, canonical `compile_group`, with its normal
as1 `-r4300_mul` workaround. `group.json` preserves the real roots and lists
only `render_replay_ui` under claims.

```sh
python3 cloud/work/frontier/dot_replay_row_match_20261007/repro.py
```

The script compiles the element-count baseline and byte-span candidate in the
same real group, then reports canonical scores, exact extents and frames.
No additional behavior harness, independent review, full tests, CI wait, or
ROM acceptance gate was added before this draft. Only source and lean notes
are published.
