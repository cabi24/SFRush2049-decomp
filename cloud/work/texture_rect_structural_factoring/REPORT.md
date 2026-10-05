# Five-site stretched packet-tail join

## Result

A single bounded source-factoring experiment rejects one concrete explanation
for the native eight-emitter layout: under the stock recorded IDO recipe, the
four stretched-mode branch arms **do not duplicate a shared packet tail** back
into four native packet regions.

The experiment is deliberately not a near-match search. Both controls are
rejected by full ELF/link verification as matches. No production C, protected
target, compiler, flags, gate, lock, splice or cartridge artifact was changed.

- Protected native: 1,780 bytes / 445 words; 104-byte frame; eight copies of
  each packet-tag materialization.
- Eight-site unsigned control: 1,776 bytes / 444 words; 112-byte frame; eight
  copies of each packet-tag materialization.
- Five-site joined control: 1,320 bytes / 330 words; 32-byte frame; five copies
  of each packet-tag materialization.

The unsigned control is not byte-equivalent to the older 1,780-byte source and
is not presented as a scheduling improvement. Its declaration/unsigned-arithmetic
choices change register allocation. It retains the eight-region topology and
serves only as the direct control for the factoring change. Between these two
controls, only three genuine consumed packet values and the common stretched
macro tail distinguish the sources; clipping, all mode-zero code, stretched
height setup and branch order stay identical.

## Useful distinction: topology versus external access order

Both controls agree with protected native execution, the independent packet
oracle, and the host-UBSan execution of unchanged C on **all 6,926 cases**.
All 16 mode/flip/stretch combinations emit. Every instruction offset in both
compiled controls is exercised. Full-word stress is included in host testing,
not merely in native modulo-arithmetic replay.

More importantly, the complete external memory event sequence is identical to
native in **every case**, for both controls. The verifier separately compares
read order, write order, and their interleaving. Event comparison records access
kind and address; packet values, final pointer advance, publication count,
guards, unchanged globals, stack restoration and callee-saved registers are
checked independently by the existing bounded machine verifier.

Therefore, matching the native external access order does not establish that
all eight SDK expansion sites, branch-private packet pointer lifetimes, or the
native 104-byte frame are required. A genuine common packet tail preserves that
order while substantially reducing the instruction population and frame.
The earlier fresh single-emitter source differed in external order in 6,578
cases; factoring alone need not create that difference. It also changed where
flags were read and how mode and axis decisions were ordered.

This distinction matters to the alias-boundary investigation: packet-pointer
no-alias metadata is compiler-internal information about scopes and lifetimes.
It is not implied solely by the sequence of externally visible memory accesses.
The experiment does not change or contradict the established metadata cause
of the historical four-word scheduling residual.

## Arithmetic and path-dependent values

The joined source retains `s + right - x` ordering and pre-stretch texture
height. Its shared tail consumes branch-defined `ds`, `dt` and fixed-point T
origin. These values correspond directly to command fields; none exists merely
to perturb register allocation. All six parameters remain full-word `int`.
Coordinate comparisons stay signed. Potentially overflowing arithmetic and
left shifts use unsigned 32-bit operands. Stretch converts the modular command
bottom back to signed for the existing local; the intended two's-complement
conversion is tested on the host and IDO and is not asserted portable to every
C implementation.

The behavior suite includes zero-area emission, clipping before stretch,
rejection, no post-stretch clipping, phase only on vertical flip, all flip/mode
paths, and the independently added y-narrowing cases. Two additional executable
wrong-join controls prove the tests observe path-dependent values across the
join: removing the joined half-texel phase, and removing the vertical derivative
sign, are both rejected. Their hashes, counts and first counterexamples are
recorded in `verification.json` (162 and 339 rejected cases, respectively).

## What has and has not been established

The test eliminates this one natural shared-tail spelling under the exact stock
recipe as an explanation for eight native packet regions. It does not prove
that the original C had eight macro sites, that IDO can never duplicate a tail,
or that other compile contexts cannot matter. Native opcode/materialization
counts are evidence about the output, not recovered source syntax.

The strongest new fact is orthogonal: an authentic intermediate factoring with
five macro sites preserves the exact tested native access order. Memory order,
source macro multiplicity, packet-pointer lifetimes and compiler graph shape
must be treated as separate hypotheses. Another local rewrite should be tied
to new source/compiler evidence rather than selected from size or score.

## Reproduction and gates

Use the same pinned SDK headers and tool environment as
`../texture_rect_fresh_behavior/REPORT.md`. Place `libreultra_gbi.h` and `mbi.h`
under ignored `build/87110_structural_factoring/`; exact Git blob hashes are
checked before using the unmodified SDK macros and full Gfx type.

    python cloud/work/texture_rect_structural_factoring/verify.py
    python -m pytest -q tests/cloud/test_texture_rect_structural_factoring.py

The script checks host semantics before compiling each of the two predeclared
sources, then uses the unchanged standalone full-link verifier. GNU linking
agrees with the project relocator over the complete function extent; both have
no unresolved or unverified symbols, relocation errors, extra functions or own
data sections. All 23 protected manifest entries are revalidated. Binary
objects, SDK headers and diagnostic output remain under ignored `build/`.
The inherited host harness normalizes the LP64 SDK Gfx object count to native
eight-byte packets; compiled MIPS replay verifies the actual O32 storage stride.

Workbench diagnosis of the joined object confirms a structural mismatch and
115 fewer instructions than native. Its relocation-masked/aligned diagnostic
is not used for exact acceptance. Complete linked-body proof rejects both
controls, irrespective of positional similarity.

This is bounded integer instruction-model and host-C evidence, not N64 hardware
rendering or proof that every parameterized case is reachable in gameplay. The
domain assumes disjoint ordinary globals, valid aligned command storage and no
concurrent mutation. No publication or merge was performed by this worker.

Focused validation passes: 33 rectangle-packet tests and 656 cloud-scorer tests.
Python compilation and `git diff --check` also pass. These are focused checks,
not whole-repository CI or cartridge gates.
