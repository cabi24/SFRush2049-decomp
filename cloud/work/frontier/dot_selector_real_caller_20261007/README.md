# D9058 with the actual D91A0 caller: NONMATCH research

`func_800D9058`, `0x800D9058`, 328 native bytes / 82 words.
Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
The complete existing selector body is unchanged; only real caller context is
added. No new function discovery, accepted-byte, or ROM-coverage claim.

## Actual comparison

| Canonical O3 build | Differing words | ELF bytes | Frame | Extra nonzero |
| --- | ---: | ---: | ---: | ---: |
| Existing clean C104 group, D9058 kept | 77 / 82 | 364 | 48 | 9 |
| Actual D91A0 caller, D9058 internal | 36 / 82 | 328 | 24 | 0 |

Both have zero unresolved/unverified relocations and score errors. The new
body has the exact native extent and 24-byte frame but remains NONMATCH.

The baseline is the actual frozen O3 recipe at
`cloud/work/dot_selector_d9058_20261006/group.json`, not its separate O2
standalone diagnostic. That packet explicitly kept D9058 only for visibility
because the real caller was missing; it did not claim an authentic private
ABI. `selector.c` and `font.c` preserve its complete C sources. The selector
header records this packet's actual O3 flag use.

## New genuine context

The missing D91A0 caller and real D816C/math closure come from [PR #262](https://github.com/cabi24/SFRush2049-decomp/pull/262),
commit `9873c196b3047e6c48df306fb4c09a556c8d6153`. The unchanged real row-cleanup
and option-predicate sources are the context established by #281 and #285.
All source is included here; no prior draft needs to be merged to reproduce.
D9058 is no longer retained as an external entry: its actual parent is retained.
There are no synthetic callers, dead-return keepers, pressure variables,
inlining blockers, invented qualifiers, or protected-tool changes.

The source snapshots the actual height field before locking, selects font 11,
computes the signed/narrowed count, and caps at 13. That count is two fixed
entries plus twelve candidates minus index 8. It is not the earlier mistaken
count of 10. The existing clean font context and word-view record assumptions
are inherited from the frozen packet, including its 96-byte field stride and
unproven original enclosing record type/count.

## Remaining limits

The clean selector still has a different private argument allocation from
native. Its own `slot_state_setup` result is 30/58 with three extra nonzero
words. No artificial historical slot_sound inline blocker is imported.
Results cleanup and D8078 retain their existing MATCH results without new
credit. The row helper remains 2/33.

D816C remains broad NONMATCH with ten unverified literal relocations. D91A0
is 898/965 with 68 extra nonzero words, six unverified literals, and one
unpaired HI16 context error for D_80113EDC. That context error is reported,
not masked. Neither the complete group nor its large callers are claimed as
accepted source. Original TU/private interfaces and behavior equivalence
remain unproven. The independent checker owns acceptance and integration.

## Reproduce

Actual flags: `-g0 -O3 -mips2 -G 0 -non_shared`, standard `compile_group`,
including the canonical as1 `-r4300_mul` workaround. Claims remain empty.

```sh
python3 cloud/work/frontier/dot_selector_real_caller_20261007/repro.py
```

This recompiles the frozen kept-context baseline and the actual caller group,
printing complete extents, frames, residuals and context errors. No extra
independent review, behavior harness, full tests, CI wait, or ROM gate was
added before this research draft. Published content is source and lean notes.
