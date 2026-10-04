# BT05 mapped macro arithmetic: complete NONMATCH

**One complete 404-byte reconstruction, zero matching functions and zero
verified-body bytes.** The authorized selector mapping closes the behavioral
proof gap. It does not close the source/compiler residual or assign production
ownership of the original jump table.

- Base: `cf4b9c619c72e83aa5da2e8b5c110765f9b03693`.
- Function: `func_80023BDC`, `[0x80023BDC, 0x80023D70)`.
- Exclusive claim acknowledged by central commit
  `690e879b18a9e4dd2919e6138942ff19a94a6131`.
- Final source: `nonmatch/func_80023BDC.c`. Nothing is submitted to
  `cloud/matches/`; central ledgers and other workers' packets are unchanged.

## Native contract and source-family evidence

The full canonical body takes a voice pointer, an aligned pointer to a two-word
macro command, and a byte operation. It returns byte zero. The voice is passed
through; the source deliberately leaves its structure opaque rather than
inventing fields or borrowing a newer version's state layout.

The five actual call sites in `80023E9C` at `80024760`, `80024780`, `800247A0`,
`800247C0`, and `800247E0` supply operations 0, 1, 2, 3, and 4, respectively,
and consume the low-byte result. Inspecting these calls does not implement or
claim the larger dispatcher, which remains outside this packet.

`80023AD4(voice, controller-byte, index-byte)` returns a signed halfword.
Its whole native body either forwards the index to the controller getter and
narrows the actual result, or masks the index to five bits and reads a signed
halfword from the local/global variable bank. `80023B50` takes those same three
inputs plus a signed-halfword value. Its whole native body writes the selected
bank or forwards the value to its controller setter; this caller ignores its
result. Both helpers remain declared-only in every scored source. The packet
does not implement or require equality of their descendants.

The first operand comes from selector word0 bits24..31 and index word1 bits0..7.
For operation4, the second operand is signed16(word1 >> 8). Otherwise it comes
from selector word1 bits8..15 and index word1 bits16..23. The authenticated table
maps 0/4 to addition, 1 to subtraction, 2 to multiplication, and 3 to signed
integer division, with explicit result0 on denominator0. The result is clamped
to [-32768,32767] and passed to the setter using word0 bits8..15 and16..23.
The command is read again after the helpers; it is not silently snapshotted.

Signed16 operands make addition, subtraction, multiplication and division safe
in signed32, including -32768 / -1. The source preserves that input domain and
uses word-sized operand carriers with real signed16 values. Conversion of the
immediate to signed16 relies on the measured N64/IDO two's-complement conversion
convention, also checked by all65,536 immediate encodings on the host.

The pinned CC0
[MusyX mcmdVarCalculation family](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synthmacros.c)
independently agrees with this calculation and command encoding.
`references.json` records the source/license blobs and exact revision. It is a
source-family lead, not the original N64 release. The later source's void
handler return and widened controller wrappers are not substituted for the
native byte-zero return and actual helper contracts. No arcade equivalent was
found in the available repository; no unavailable arcade source is cited.

### Default-path limit

Operations5..255 still call the two getters, then the unsigned guard bypasses
the table and loads an uninitialized native stack result before clamping. The
source retains an uninitialized result for this out-of-contract path. No fake
default, new guard or invented result is added. Defined C and semantic equality
are claimed only for the actual caller domain0..4. Native-only invalid-selector
fixtures observe stack dependence; they are explicitly not host-C executions
or a defined-behavior claim.

## Complete table and relocation accounting

`mapping_proof.json` is a source-free extract of the independently authenticated
2026-10-04 receipt. It records the approved20-byte table at `8002D950` and all
five absolute targets, along with canonical function hash and base provenance.
This packet opens no asset data and performs no wider read. The receipt's full
68-byte original inspection concerned three separately assigned tables; only
this function's mapping is reproduced here.

Every candidate has three external call relocations, two `.text` HI16/LO16
relocations to its compiler-generated `.rodata`, and five `.rodata` R_MIPS_32
relocations to its own `.text`. `verify.py` inventories and independently resolves
all ten. It then checks those complete bytes against a separate GNU linker
result and compares every unmasked text word and all20 table bytes against the
canonical target and authenticated mapping. No candidate words are replaced
with target words, and no comparison mask is used by this separate proof.

The temporary proof placement uses text80023BDC and rodata8002D950 with4-byte
input subalignment. This respects the actual native function start despite
IDO's16-byte input-section alignment. It is an evaluation link only, not a
production linker modification, storage assignment, or promotion. The compiler's
12 trailing alignment bytes in `.rodata` are verified zero but are not claimed
as authenticated native table data.

For the final O2 source, the function symbol is404 bytes; the padded `.text`
section is416 bytes with no nonzero excess. All relocations resolve, and the
independent calculation equals GNU ld byte for byte. Nevertheless82/101 native
words differ and every case destination is four bytes early:
`80023C8C, 80023C98, 80023CA4, 80023CB8, 80023C8C` rather than the authenticated
`80023C90, 80023C9C, 80023CA8, 80023CBC, 80023C90`.
Thus neither complete text nor complete table matches.

The unchanged stock scorer also reports its two local-rodata relocations as
unverified. That is preserved as an independent admission blocker. The separate
link proof does not change the scorer's result or permit matching credit. No
source-table transplant, linker production ownership, symbol fallback edit,
relocation masking, or protected scoring/target edit was used.

## Bounded compiler controls

The initial source-family-shaped signed16 locals scored93/101 at O2 with six
extra nonzero words. O1 scored100/101 with four extras. The unchanged workbench
was run before refinement; it identified a structural/narrowing/register
residual with matching48-byte frames. The initial diagnosis used temporary
objects only; its raw report is not published. The final equal-address
no-relocation ELF diagnosis is summarized without native instructions in
`diagnosis.json` and agrees with the82-word residual.

Fourteen natural authored forms are preserved under `controls/`; the selected
word-operands form is also copied to `nonmatch/`. `verification.json` binds all
15 file paths to30 O2/O1 compilations and complete link proofs. Controls cover
explicit argument casts, raw two-word command access, result casts, byte masks,
real register storage hints, SDK word typedefs, C89 non-prototype declarations
with explicit promotions, word-valued call-contract spelling, operand carriers,
and explicit immediate masking. The word-call form was only a physically
compatible promoted-value declaration control, not adopted as a newly proved
original prototype. No extra formal, fake call, padding local, keeper, volatile
barrier, hand-written assembly or flag sweep was introduced.

The byte-mask expression prevents a narrowed byte load; word-sized carriers
remove redundant signed16 assignment chains. This yields the final404-byte
body and82/101 words, with no excess. O1 for that same source is99/101 with18
extra nonzero words. Native halfword spills and narrow-result lowering still
have no exact source form in this bounded set. All forms remain NONMATCH.
Further work needs authentic N64 translation-unit/compiler context or a newly
supported narrowing/lowering mechanism. Repeating these controls is not a new
hypothesis. Production table ownership remains separately unassigned.

## Reproduction and semantic checks

With pinned IDO5.3 in `IDO_DIR` and GNU MIPS binutils on PATH, from repository
root:

```
python3 cloud/work/boot_tail/BT05-mapped-macro-dispatch/verify.py --check
python3 cloud/work/boot_tail/BT05-mapped-macro-dispatch/test_layout.py
python3 cloud/work/boot_tail/BT05-mapped-macro-dispatch/test_semantics.py --check
```

`verify.py` checks the pinned compiler file hashes, protected target manifests,
all census extents, the canonical404-byte hash and the existing getter's strict
match. The IDO layout check proves32-bit pointers/words,16-bit halfwords,
8-byte commands and word1 offset4 without changing candidate text.

The semantic suite runs72,144 cases each through the actual retained host C
with AddressSanitizer/UndefinedBehaviorSanitizer, the complete canonical MIPS
body and the complete independently linked O2 candidate. Cases cover all five
operations, signed edge pairs, clamp boundaries, divide-by-zero handling,
all65,536 immediate encodings, all256 controller/index bytes, helper mutation
of both command words after each read, randomized valid inputs and incoming
high-register garbage. Native/candidate calls aggressively clobber ordinary
caller-saved registers, check stack/callee saves, and require exactly the selected
table access and no uninitialized stack reads on all valid paths.

A further1,004 canonical-native cases cover all251 default selectors with four
stack patterns: they prove no table read and the expected uninitialized result
load. Across valid/default tests97/101 native PCs execute; the two compiler
arithmetic-trap instructions and two unreachable scheduler-duplicated
instructions do not. The replay fails closed on unsupported operations, invalid
memory, unexpected calls, misalignment or step-bound exhaustion.

These tests use synthetic, aligned, live storage and explicit helper contracts.
They prove no original variable contents, malformed bytecode safety, transitive
helper equality, whole-engine behavior, production link or ROM coverage.
`semantic_verification.json` binds source and test hashes. No ROM bytes, raw
native disassembly, object files, credentials or unrelated private data are
included. D1248/helper work and T050 implementations remain untouched.

Independent review of an immutable source commit/tree is required before central
intake. No merge is performed by this packet.
