# Rectangle scheduling: separate source-shape constraints

This is a rejected, bounded source experiment for `func_80087110`, not a match
or a claim about the original source. It was conducted on PR #98 research
checkpoint `493e7835` after reading the causal report, exact allocator membership,
authentic SDK controls, and prior negative source matrices.

## Test specified before compilation

The existing positive scheduling control wraps the stretched both-flags body
in a one-shot block. That adds a default-live entry for height and offset,
crossing only height's normalized divisor threshold and exchanging their
registers. The new hypothesis was that replacing the **conditional itself**
with a guarded one-shot exit could merge the genuine predicate with the
structured exit, closing the base-alias relation without that extra lifetime.

The sole new source is `guarded_exit.c`: a while guard uses the original
short-circuit both-flags predicate, performs the original drawing operations,
and immediately returns. Its false path reaches the original x/y/no-flip
fallback. No dead read, padding, keeper, fake parameter, added call, qualifier,
compiler option, global declaration, or target change is involved.

**Rejected.** Stock IDO emits 1,780 function bytes, with twelve differences and
no extra words. Both the original four scheduling offsets and the eight
height/offset exchange offsets are present. GNU linking agrees with the project
relocator over the complete function, all 32 relocations resolve, and only the
expected twelve zero alignment bytes lie beyond the ELF function extent.

The fully relocated output is byte-identical to the earlier rejected
`fallback_dispatch` control, despite the distinct source test. Its SHA-256 is
`ca9ec43290346368916e16d7cdec1ee967dde09b4cd8e00e2f59bcd2b0ca60c4`.
That equivalence is the stopping condition for this source-shape family.

## What this distinguishes

For the frozen topology and toolchain, the allocator effect and scheduling
effect are separable. A structured guard alone is insufficient. In the new
source the alias close still appears **after** the unconditional both-flags
exit. Moving the guard has therefore retained the troublesome assembler
predecessor while still increasing the allocator lifetime denominator.

Read-only diagnostics, bound to the source in
`allocation_diagnostic_receipt.json`, confirm:

- Height: four occurrence blocks plus six default-live blocks, X=10, divisor
  four; net saving four, priority 1, phase-one register t4.
- Offset: four occurrence blocks plus five default-live blocks, X=9, divisor
  three; net saving four, priority 4/3, phase-one register t3.
- Both retain interference count 20. Their exchange is the same normalized
  threshold effect as in the body-wrapper control, not a phase-two transition.
- The native diagnostic listing and print-only allocator trace each emitted
  optimized Ucode byte-identical to the stock run. No force controls were set.
  Diagnostic executables are explanatory only; the numerical proof above uses
  the stock cc/uopt/ugen/as1 pipeline.

The height occurrence graph IDs are 30, 31, 36, 40, with default-live IDs
32, 33, 34, 35, 37, 39. Offset occurrence IDs are 31, 36, 40, 32, with
default-live IDs 33, 34, 35, 37, 39. Graph IDs are source-specific, not ROM
addresses. Normalization is X when X<3 and 2+((X-2)>>2) otherwise.

## Backward constraints for the next independent hypothesis

An ordinary-C solution retaining this instruction population must satisfy
both axes together:

1. Reach an assembler graph in which the post-exit metadata does not block the
   existing edge-add hoist. Merely changing a local branch-fill tie is not the
   known cause; XBB runs before final branch filling.
2. Preserve compatible register priority **and** interference. Reducing
   height's denominator alone was previously insufficient when the operation
   crossed into phase-two coloring. Adding blocks elsewhere can fix one tie
   while introducing the already-measured s/step exchange.

These are a search filter for this topology, not an impossibility theorem for
other authentic source contexts. There is no evidence here for a missing
actual v1 read on the taken path, nor for an original loop in the application.

A stock-stage reinspection of the prior `function_first_two_macro` Gfx** holder
also found that it does **not** relocate the alias close. Its eight differences
are the original four scheduling offsets plus four stack-slot offsets. Its
near score must not be confused with the body wrapper's eight register-only
differences, which do fix the original schedule.

## Reproduction and scope

Run `python cloud/work/texture_rect_schedule_constraints/verify.py` with the
pinned stock IDO and GNU MIPS tools configured. It recompiles the frozen
baseline and the complete new source, checks the observed metadata without
editing it, and fully links and inspects both ELF bodies. The checked-in receipt
contains only source/tool hashes, scalar verification, and offsets.

This packet does not claim new semantic coverage: the control was already
rejected by the complete binary comparison. All raw listings, compiler
intermediates, original bytes, and diagnostic traces remain in ignored build
storage. No protected source, lock, scorer, compiler, or promotion state was
changed. No further equivalent loop spellings were tried.
