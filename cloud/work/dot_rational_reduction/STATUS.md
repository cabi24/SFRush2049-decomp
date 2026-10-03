# func_800A557C: tested range-reduction research (NONMATCH)

Natural C89 reconstruction of the complete 114-word / 456-byte game function.
The primary IDO 5.3 O2 candidate is **71/114 full resolved words different**:
452-byte function, 464-byte aligned text section (12 trailing zero bytes).
No unresolved/unverified relocations or nonzero excess words. `claims: []`.
No accepted-source, target, lock, scorer, build wiring, ROM or coverage changes.

## New contribution and provenance

The inherited `near_miss_B29` sources were inspected rather than presented as
new research. Its natural source scores 86/114, polynomial source 74/114, and
intrinsic source 70/114. The last is not a valid improvement: it reads `sp30`
in an expression that also writes it through `modff`, and reads `temp_f2` and
`temp_f6` before initialization. It is expressly rejected, not retained as a
candidate. The new named-local source separates both modff calls, uses one
semantic sign variable for rounding, and evaluates each Horner polynomial
with its real dependencies. It reaches 71/114 without that undefined behavior.

This contribution adds the first complete native/linked-candidate/host
semantic proof for this target, including poisoned call-clobbered registers,
checked stack memory, branch-likely annulment, every conditional branch outcome,
both zero/two-modff-call paths, exact half-way rounding and neighbors, negative
arguments, odd/even reciprocal paths, and near-zero direct reduction. Native
coefficients are external symbols, not substituted constants or guessed ROM
values. Test fixtures vary those coefficients independently.

Native assembly suggests a tangent-like range-reduced rational approximation;
that identity and its numerical error bound remain unproved because coefficient
values are unavailable. The arcade reference submodule is absent locally.

## Experiments and boundaries

- O2 sign-local reconstruction: 71/114, 452-byte function.
- O3 same source: 71/114; no improvement.
- O1 same source: 111/114 plus 14 nonzero excess words; rejected.
- Workbench `diagnose` confirms structure/register scheduling residual and
  114 versus 113 true instructions. Its reloc-masked numbers are not evidence;
  the canonical strict scorer and independent GNU link compare all raw words.
- Existing natural B29 form: 86/114; polynomial B29: 74/114. No blind continued
  register shuffling, casts, padding or fake helpers were used.

Native conversion of the rounded reduction index to signed 32-bit integer is
only represented by C for finite in-range values. NaN input and overflowing
range reductions are explicitly outside the tested C domain; early rejected
positive/negative infinities are covered. Coefficients used in tests are injected
fixtures, not extracted retail values. IEEE single rounding is modeled; actual
N64 FCSR exceptions, traps, denormal and NaN-payload behavior are not established.

## Reproduction

With pinned `tools/cloud/ido` and GNU MIPS binutils on PATH:

    python3 cloud/work/dot_rational_reduction/verify.py /tmp/rational-proof.json
    python3 -m pytest tests/conveyor/test_rational_reduction_research.py -q

The replay performs 10,139 three-way differential cases and 100,000 ASan/UBSan
host cases. It never changes evidence unless an explicit output path is passed.
A missing compiler/linker makes the corresponding pytest test skip; repository
CI success alone is not a matching or ROM claim.
