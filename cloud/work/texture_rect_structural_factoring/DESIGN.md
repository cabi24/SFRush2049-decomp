# Bounded packet-tail factoring test

Recorded before compiling these two controls, 2026-10-05.

## Question and evidence

The protected native leaf contains eight independent materializations of each
of the E4, E1 and F1 packet tags. The historical near-match writes eight source
`gSPTextureRectangle` invocations; the independent fresh source writes one and
compiles to 540 bytes. Neither observation establishes that eight source macro
sites are necessary: a compiler can duplicate a shared source suffix.

Previously recorded `stretch_nested.c` and `stretch_vertical_first.c` change
flip dispatch nesting but retain all eight source macro sites. The allocator
lifetime/block probes also retain all eight. This test asks a different, narrow
question: will stock IDO duplicate a single genuine stretched-mode packet tail
back into four native emission regions after the original four-arm dispatch?

## Predeclared controls

1. `unsigned_eight.c` retains the historical clipping and four-arm dispatch in
   each mode, plus all eight macro sites. It uses the pinned authentic SDK
   context and unsigned 32-bit arithmetic for additions, subtractions and
   left shifts; signed coordinate comparisons and all six `int` parameters
   remain. Its purpose is to isolate arithmetic-domain changes from factoring.
2. `stretched_tail_join.c` is identical through the complete mode-zero arm and
   stretched-height setup. Only the four stretched emitters are factored into
   one shared SDK macro invocation. Each existing arm computes the horizontal
   and vertical derivatives and the already-fixed-point T origin consumed by
   that shared invocation. Those are real packet fields, not keep-alive values.
   The four texture-origin updates retain their historical arithmetic order.

The joined form has five total macro sites. There is no loop, helper call,
artificial pressure, dummy operation, volatile qualifier, inline assembly,
listing edit, custom macro, or compiler/target/flag modification.

## Predictions and stopping rule

If the eight-site unsigned control keeps the native-sized, eight-region shape,
then the joined source directly tests whether this one source join can explain
the target duplication under the stock recipe. Four stretched packet regions
in the joined object support compiler duplication; one rejects it for this
specific source/recipe. Other outcomes must be reported without extrapolating
that a unique original source has been recovered.

Compile each control once with the existing recipe, validate complete ELF
extents and GNU/project relocation equality, and replay all 6,926 cases against
protected native execution and the independent packet oracle. Run the unchanged
source under host UBSan for all cases before stock compilation. Report static
packet-tag counts, frame size, instruction classes, and external read/write
order separately from output equivalence. Do not iterate by score or promote
any result. The baseline and fresh sources/receipts remain untouched.

## Domain

Disjoint, readable ordinary globals; aligned, sufficiently large packet storage;
no concurrent mutation. Zero-area emission, clip-before-stretch, no second clip,
full-word arguments, and the vertical-only half-texel phase remain mandatory.
Unsigned-to-signed conversion of a stretched command bottom is implementation
defined; the host and IDO tests explicitly check the intended two's-complement
mapping. This is integer packet construction, not floating-point arithmetic.
