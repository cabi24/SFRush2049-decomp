# Image-A statistics: the stored-name boundary is not closed

## Result

**Empty-claims research. No source-match, accepted-image or ROM coverage.**
Fresh baseline: `46780d3e85fb79001d64cceef25c868e187265d7`.

A reproducible **checksum-valid malformed save** defeats the proposed
18-byte source-name bound for `func_803AA3D8`:

1. The complete native `func_800C7EC8` loader accepts the synthetic save through
   the complete native `draw_speedometer` validator. All 44 checked checksum
   spans have valid values, computed independently and checked by the native
   `format_string_parse`. The loader returns the actual profile handle.
2. The loader copies 13 saved name bytes into profile+20, then 11 saved
   configuration bytes into profile+33. The synthetic name field deliberately
   has no NUL; six following configuration bytes are nonzero, then a NUL.
   The resulting contiguous logical string is 19 bytes. These are generated
   fixture bytes, never user save data.
3. The complete native `func_8039338C` input handler copies that string with
   the actual native narrow `func_800BE6A4` path into `D_803BA840`. It sets the
   confirmation flag at scratch+13 and writes zero to the one-byte choice at
   scratch+16. The initial confirmation string is therefore 16 bytes.
4. A second complete input-handler call toggles that actual choice byte to
   one. This exposes 19 bytes again while the confirmation flag remains one
   and completion remains zero. The NUL is at scratch+19. This detail means
   the initial zero choice is not a stable termination invariant.
5. Across the six authenticated languages, confirmation format slot 143 in
   languages 4 and 5 has 45 fixed bytes. Adding the exposed 19-byte name and NUL
   requires **65 bytes**. An 18-byte bound would be needed for a 64-byte array.
   The independent host `snprintf` check reproduces all 12 format lengths.

This is **not evidence that ordinary name entry can create the fixture**.
The save violates the apparent 13-byte terminated-name convention and places
non-normal values in configuration fields. Its version and checksums pass the
shown native loader path; physical controller-pak I/O, ordinary creation,
whole-game reachability and hardware behavior are unproved. The fixture is a
counterexample to deriving a universal bound from this loader/validator alone.

[Draft #363](https://github.com/cabi24/SFRush2049-decomp/pull/363) remains the
unchanged 2/717-word AA3D8 research result: frame 192 versus 176, text[80]. No
smaller AA3D8 array is retained. All production sources and accepted locks are
untouched. This packet adds evidence, not a duplicate matching packet.

## Reproduction

    python3 cloud/work/frontier/dot_runtime_a_name_boundary_20261012/verify.py --host
    python3 -O cloud/work/frontier/dot_runtime_a_name_boundary_20261012/verify.py --host
    python3 cloud/work/frontier/dot_runtime_a_name_boundary_20261012/verify.py --sanitize
    python3 -m pytest tests/cloud/test_runtime_a_name_boundary.py -q -o addopts=''

The one focused test passes in about 0.8 seconds. It needs no IDO, GNU MIPS
assembler/linker, private ROM, LAN or emulator. The host option uses the local
C compiler; the sanitizer option enables ASan/UBSan/bounds with recovery off
and only LeakSanitizer disabled. All C writes stay within generously backed
arrays; it checks required lengths without deliberately overflowing C.

## Native experiment and proof scope

The fail-closed logical MIPS-II interpreter executes current-tree authenticated
instruction words directly, including delay slots and branch-likely annulment.
Every consumed function's **complete** target byte sequence is matched to the
unchanged authenticated game or image-A asset and bound by its word hash.
The 24-byte native checksum jump table is separately bound. No copied native
instructions, images, payload strings, objects or assembly are published.

Eight functions execute: C7EC8, C7818, B466C, 91FBC, C734C, C7308, BE6A4 and
9338C. AA3D8 is authenticated as downstream context but is not executed. The
receipt reports executed versus total instruction counts, and does not claim
full branch coverage. Twenty fixtures cover logical lengths 0 through 19;
lengths 0 through 12 are ordinary terminated-name controls and remain unchanged
through both input-handler calls. Lengths 13 through 19 are deliberately
malformed name fields.

Four external service categories remain bounded contracts:
- File read: supplies the already backed 2,064-byte synthetic save
- `memcpy`: exactly the three native 13/8/11-byte copy triples; any other triple
  is rejected
- File release: checks the actual file handle; native C7308 itself clears the
  profile's data pointer
- Menu sound calls: record arguments without memory effects

Native list insertion and statistics propagation execute, rather than being
replaced with success constants. All unused statistics payloads are zero.
No external helper is allowed to mutate the profile name or confirmation
storage. Those real service implementations and asynchronous behavior are
outside this fixture.

The independent Python checksum model agrees with native code on 289 cases:
all 256 one-byte values, the empty value and 32 deterministic multibyte values.
It executes 44/45 instructions; only the unreachable divide-by-zero break is
excluded because that arm's divisor has low bits 5. The complete instruction
bytes and table remain verified even for that excluded instruction.

Negative controls reject a wrong stored checksum, wrong version, changed
native name-copy count, unsupported instruction and unmapped memory. Unknown
calls/control transfers and writes to selected code or checksum-table storage
fail closed. Python optimized mode retains all gates.

The source/own-verifier/test/target/format-resource bindings follow protocol v2.
No shared-tool, mutable lock, Makefile or assembly-text hashes are pinned. No
source match is being installed, so no GNU-relocation, selective-installer or
compiled-extent match claim is made. Claims are explicitly empty.

## Bounded fallback investigation

The remaining `func_803AD57C` statistics root was checked in the full genuine
statistics group, preserving the ten accepted members plus the exact #360
ABEC8 candidate and unchanged #363 AA3D8 context. Its baseline is 348/523
canonical differing words, full 2,092-byte extent, frame 320 versus 312, with
eight explicit unresolved own-data sites. Nothing was waived.

The workbench diagnosed structural/register/stack differences. Direct native
slot indexing, separate real title and counter lifetimes, source branch
structure, declaration order and the adjacent AD574 inline-stub hypothesis
were checked using the unmodified stock compiler. The bounded natural probes
found no strict result. A temporary syntactic improvement to 333/523 was still
structurally wrong; it is not a deliverable. The adjacent-stub identity remains
unproved. No helper or rewritten root is published and no numeric capacity
change is retained. This avoids turning a weak probe into a new accepted
semantic or caller assumption.

## Limits and next useful step

A safe AA3D8 capacity change now needs an explicit valid-save convention that
excludes this malformed path, enforcement of termination earlier in the real
load flow, or a different native local-lifetime explanation for its 16-byte
frame gap. This audit does not establish that every possible writer was found;
one authenticated accepted-load fixture is enough to invalidate the proposed
universal derivation. Do not treat it as proof of an exploit or a normal-game
buffer overflow.

No ordinary-user save, actual renderer/native formatter run, original C array
capacity, native gameplay oracle, image composition/recompression, ROM hash or
hardware test is claimed. Leave integration and merging to the independent
checker. Publication contains source, tests and derived verification metadata
only, with no protected edits or unrelated private information.
