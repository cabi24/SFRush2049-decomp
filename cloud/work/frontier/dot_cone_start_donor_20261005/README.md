# Cone-hit initializer: genuine matrix contract correction

**NONMATCH. No matching claim, accepted-byte gain or production change.**
`func_8010DCFC`, **0x8010DCFC–0x8010DF90**, 660 bytes / 165 words.

The new contribution is a substantive buffer/interface repair. The archived
`heads_B14/func_8010DCFC_singleformal.c` declares only three floats for the output
of `vector_normalize_length`, then supplies the same array to `math_utility`.
The unchanged accepted helpers write and read **nine floats / 36 bytes**.
The old reconstructed source is undersized. **The retail routine is safe in
this respect:** its 136-byte frame puts the matrix at SP+84, inside the frame.
This is a defect in an archived research seed, not a discovered retail-game bug.

## Source and native evidence

The pinned [Rush The Rock ancestor](https://github.com/historicalsource/rushtherock/tree/845329d7b36f5a384c5625ed9a0aef584ab46139)
provides `targets.c:1496–1533`, StartCone; `LIB/fmath.h:41–73`, MATRIX; and
`visuals.c:2442–2463`, PointInDir. File hashes and the complete pin are in
`provenance.json`. The inherited copyright notice is retained in the candidate.

MATRIX is a 48-byte union containing a 3×3 orientation and a three-float position.
StartCone genuinely passes this object to PointInDir. Retail's matrix begins at
SP+84, with the next live Visual pointer at SP+132, exactly 48 bytes later.
The candidate uses the same 48-byte object; its positions are SP+60/SP+108.
Only the 36-byte orientation is consumed. This is a supported ancestral layout
hypothesis, **not recovery of the original N64 typedef**. A 36-byte orientation
object is also a valid sufficient-capacity control, and remains nonmatching.

The N64 specialization adds two mode branches, obtains its signed player index
from target byte +92, uses descriptor callbacks, stores a five-second lifetime,
chooses vertical launch velocity by target type, and emits the existing
positional-sound call. The candidate models those native operations rather than
copying unrelated arcade operations or unused donor locals. PointInDir itself
has N64 algorithm/threshold differences; no byte-identical arcade helper claim
is made.

The public helper declarations use the existing accepted interfaces, including
`f32 [3][3]` for the basis output, unsigned visibility mask, and signed sound
result/byte mode. The source's Visual and the accepted allocator's Node are
separate reconstructed O32 views of the same 24-byte record. Their offsets are
checked by IDO; this does not establish a shared original C tag or whole-game
compatible type model. Host-width pointer fields are initialized by name and
compared to separately encoded O32 memory, not mistaken for ABI proof.

## Complete compiler results

- Typed donor-backed candidate, O3 and O2: **148/165 target-sized differences**;
  complete ELF function **664 bytes**; **149 complete-body differing positions**,
  counting its one excess zero instruction. Frame **112**, versus retail 136.
  The eight trailing zero section-alignment bytes are outside the function.
- Genuine 36-byte orientation control: same 664-byte body and 148/165 differences;
  frame 104. It is not a match or an original-declaration claim.
- Archived single-formal seed, pinned O2: **652-byte** complete function,
  148/165 differences including missing positions; frame 96. Its helper output
  starts SP+80 and runs through SP+115, beyond both its declared array and frame.
- Genuine four-body O3 context preserves the candidate and all three unchanged
  accepted helpers: allocator `func_80090284`, basis builder
  `vector_normalize_length`, and matrix copier `math_utility`.

All complete extents are independently checked with GNU readelf. GNU ld resolves
every relocation and every word is compared without relocation masks. The
candidate has **21 relocations and no owned data**. The context's four-byte
basis-builder literal is verified against protected data at 0x8012388C; twelve
zero alignment bytes are excluded. Per-body context links establish complete
streams at native entry addresses, not original contiguous unit placement.

The authentic ScaleVector macro control and genuine context leave the complete
candidate stream unchanged. No subsequent
allocator, local-order or spelling sweep was run. Workbench diagnosis finds a
structural/register residual and 24 missing frame bytes; these are not repaired
by adding unconsumed locals, extra formals, padding, dead reads, volatile,
stand-ins, keepers, assembly or protected compiler/scorer changes.

## Behavioral and bounds verification

`verify.py` runs **2,048** deterministic cases through unchanged host C89+UBSan,
an independent state oracle, protected native code and GNU-linked candidate code
(**4,096 MIPS executions**). It covers all **165 native instructions** and both
outcomes of all **eight conditional branches**. It checks all mapped native
memory, nonzero gap canaries, exact permitted stack writes, stack canaries,
return address, saved registers/FPRs, call ordering and argument contracts.

Fixtures cover both special modes, rejected mode/type combinations, allocator
failure, all launch branches, signed-byte flags and selected callback changes.
Allocator and matrix callbacks can change owner/descriptor/type, replace the
motion pointer, and change the active-list head; cached and reloaded state is
checked against the native behavior. Four compiled wrong-contract mutants are
rejected: velocity scale, cleared flags, launch axis and cached sound descriptor.
Unknown instructions, truncated bodies, wrong calls and redirected stack saves
are rejected by regression tests.

Runtime external calls are **explicit O32 contract hooks**. The matrix hook
writes all nine output components; it is not a numeric PointInDir simulation.
A separate ASan+UBSan process executes the **unchanged accepted basis-builder
source**: the genuine 48-byte matrix passes with its unused position canary
intact, while the legacy three-float output triggers stack-buffer-overflow.
Its magnitude/normalization dependencies are mathematical host contracts.

No invalid pointers, arbitrary negative indices, NaN/FCSR behavior, concurrency,
actual dispatch/gameplay, full helper runtime, complete-game shadow build,
source-image/compression/ROM gate or coverage increase is claimed. The record
fixtures use player indices 0/1 and descriptor indices 1/2. Retrying matching
requires real additional source or compiler-boundary evidence explaining the
remaining native frame and pointer webs.

The protected native scan finds no direct calls and four aligned data references
at 0x80117808, 0x80117958, 0x80117F88 and 0x80117FB8. These support callback
registration, without proving a runtime dispatcher or original C signature.

## Reproduce

With the repository's pinned IDO and GNU MIPS tools available:

    python cloud/work/frontier/dot_cone_start_donor_20261005/verify.py /tmp/cone-start-proof.json
    python -m pytest tests/conveyor/test_cone_start_donor.py -q

The saved receipt contains hashes, counts and offsets, not ROM words or data
dumps. Seven regression tests are discovered by the existing Conveyor suite.
The changed-submission scanner schedules zero matching jobs because claims are
empty and this is a research path. No production source, lock, protected target,
shared header, compiler recipe or scorer is edited. Acceptance and any future
integration remain with the independent checker.
