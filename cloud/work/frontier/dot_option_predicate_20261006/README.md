# D8078 option predicate: genuine-caller match candidate

**Claim: func_800D8078, 0x800D8078, 220 bytes.** Canonical result is **0/55
words differ**, exact 220-byte ELF extent, zero extra nonzero words,
unresolved/unverified relocations, or score errors.

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
The predicate is unchanged genuine source from
`cloud/work/frontier/w8a/func_800D8078/best.c`. All `D_zz`, `zz_caller`, and
`zz_caller2` stand-ins are removed. The old wave-8 full score was explicitly
provisional because those callers were invented. This packet restores that
instruction match with the **actual complete D91A0 caller**, not a new
predicate discovery or a reuse of the stand-in claim.

## Actual baseline and source context

The honest kept predicate alone scores 25/55 differing words. The current
real group scores 0/55 without changing its body or flags. It includes:

- The complete D91A0 caller, D816C callee, and four accepted math/predicate
  bodies from [PR #262](https://github.com/cabi24/SFRush2049-decomp/pull/262),
  commit `9873c196b3047e6c48df306fb4c09a556c8d6153`.
- The genuine A118 row-cleanup pair from
  `cloud/work/ipa-groups/codex_result_rows_a118/group.c`, retained with the
  actual caller as in [PR #281](https://github.com/cabi24/SFRush2049-decomp/pull/281).

All source is included. There is no dependency on either draft being merged.
No synthetic use, pressure local, dead blocker, qualifier, padding, or
compiler/target modification was added.

The predicate uses a signed player byte, a 76-byte input record's row byte,
a signed-half selected index, 10-byte signed option rows, and the actual
option-validity table. Its source ordering and repeated reads are the same
as the preserved wave-8 real body.

## Scope and residuals

Only D8078 is claimed. The results-cleanup body remains MATCH, with no duplicate
credit over #281; the internal row helper remains 2/33. All four previously
accepted math/predicate helpers remain MATCH, again with no new credit.

The large reconstructed context is **not** accepted production source:
D816C remains 654/699 with 10 unverified literal relocations. D91A0 remains
903/965 with 68 extra nonzero words, six unverified literal relocations,
and one canonical context error: an unpaired HI16 for D_80113EDC in its emitted
text. This error is disclosed rather than hidden or weakened; it does not
occur in the claimed predicate. The full group is not a ROM-integration or
whole-unit-match claim. Do not replace unclaimed production recipes with it.
Original TU/private interfaces and broad behavior remain unproven. The
independent checker owns acceptance and integration.

## Reproduce

Actual flags: `-g0 -O3 -mips2 -G 0 -non_shared`, canonical `compile_group`,
including as1 `-r4300_mul`. `group.json` retains only real roots and declares
only D8078 as a claim.

```sh
python3 cloud/work/frontier/dot_option_predicate_20261006/repro.py
```

The script compiles the honest kept baseline and the real caller group,
reporting full extents, all scores and context errors. No behavioral harness,
extra independent review, full tests, CI wait, or ROM acceptance is required
before this draft. Published content contains source and lean notes only.
