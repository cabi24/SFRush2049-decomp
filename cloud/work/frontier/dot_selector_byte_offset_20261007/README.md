# D9058 byte-stride field access and in-place height adjustment

Research NONMATCH: `func_800D9058`, 0x800D9058, 328 native bytes.
Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
No new matching-byte or ROM-coverage claim; the independent checker owns
acceptance, integration, and merging.

## Result

The authentic-caller baseline from [PR #289](https://github.com/cabi24/SFRush2049-decomp/pull/289)
is 36/82 differing words. This candidate is **25/82**, with the same exact
328-byte extent and native 24-byte frame. Both have zero extra nonzero words,
unresolved/unverified relocations, and score errors for D9058.

Two source-grounded expression changes explain the improvement:

- Read the existing pre-lock height snapshot through a byte offset of 96 per
  row, instead of a 24-word index. This states the established field stride
  without inventing an enclosing record layout or count.
- Assign the unsigned 32-bit subtraction by 24 back to the height snapshot
  before signed division by 16. The unsigned subtraction and signed cast
  remain explicit, preserving wrapping behavior and truncation toward zero.

The initial field read still precedes all three callbacks. Count remains two
fixed entries plus twelve candidates minus index 8, or 13. The signed-halfword
narrowing still happens before the cap, and flags are read after unlocking.
No additional local, synthetic caller, padding, pressure, qualifier, inline
assembly, keep-root, or compiler change is used.

Workbench diagnosis before edits showed an allocation mismatch at equal
instruction count and frame. The candidate now reproduces the native height
address and subtraction temporaries. Remaining differences are selector input
s2 versus s1 and loop/global-address coloring. Source-order and equivalent
loop variants did not resolve those differences; the broad caller/context
private allocation remains unproven.

## Source/context provenance and limits

All real caller/context sources and group roots are unchanged from #289:
D91A0/D816C and math helpers from #262, row cleanup from #281, predicate from
#285, and the frozen clean selector/font C104 source. The packet includes them
so it does not depend on unmerged drafts. This is not original-TU proof.

D8078 remains 0/55 and results cleanup 0/49, without new credit. The row helper
stays 2/33 here, intentionally preserving this comparison baseline rather
than importing the separate #298 match. D816C remains 654/699 with ten
unverified literals. D91A0 remains 898/965 with 68 extra words, six unverified
literals, and its existing unpaired HI16 for D_80113EDC. The group is not an
accepted whole-group recipe; claims remain empty. Other unclaimed context
scores, extents, and frames are printed by the script.

## Actual recipe

`-g0 -O3 -mips2 -G 0 -non_shared`, canonical `compile_group`, including the
normal as1 `-r4300_mul` workaround.

```
python3 cloud/work/frontier/dot_selector_byte_offset_20261007/repro.py
```

The script reconstructs #289's exact baseline through just the two reverse
expression edits, then builds both variants in the identical real context.
Only source and this lean reproduction/verification note are published.
No extra behavior harness, independent review, full tests, CI wait, or ROM
gate was added before this research draft.
