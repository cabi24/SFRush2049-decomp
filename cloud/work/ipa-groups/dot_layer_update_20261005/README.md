# Layer parameter update: verified 152-byte matching candidate

`mode_select_input`, **0x800DFB08–0x800DFBA0**, is a strict full-body
**MATCH: 152 bytes / 38 words**. This is a review-ready matching candidate;
accepted-byte and ROM-coverage gain are **zero**.

This packet finishes verification and registration of an existing source lead.
The helper itself is the unchanged A169/A168 reconstruction. It is **not a newly
discovered algorithm or a new improvement to the helper's old zero score**.
Current accepted allocator source replaces the archive's old 11-word-nonmatching
allocator. All six runtime context bodies now match together.

## Source and ownership

Base is `e24b47d89a0c8ffade1e4c75ad76b9d390a1c232`. Current locks, tracked claims,
open PRs through #115, A168/A169 and frontier w6c were checked; the coordinating
worker confirmed no active DFB08 owner before source edits. Only DFB08 is claimed.

The complete archived A169 source is retained, except that its allocator
definition becomes the genuine `Msg *func_80091B00(void)` declaration. The new
`allocator.c` is copied byte-for-byte from accepted
`src/blob/groups/frontier_slot18_alloc/func_80091B00.c`. The archive's flags and
keep list are unchanged. There are no new padding locals, fake formals, dead
reads, stand-in callers, compiler/scorer changes or target edits.

The accepted allocator's existing volatile slot table is inherited and disclosed.
It was previously introduced to control scheduling; this packet provides no new
source-provenance argument for that qualification. The rest of the archived
context is unchanged. Original N64 type names and a whole-function arcade donor
are not established. The accepted allocator and archived context retain their
separate pointer/type views across translation units. This compile regression
does not prove a unified ISO C shared-type model; the defined host fixture uses
a correctly typed Msg allocator boundary.

The genuine hierarchy wrapper remains marked `__inline`. Native DFB08 contains
its complete receive/transform/jam operations and has no call to the standalone
wrapper. The ordinary exported wrapper still matches. This supports an inline
expansion hypothesis; the original spelling of that annotation is unknown.
Removing the annotation is a bounded control: 28/38 differences and a 96-byte
helper. It is not an accepted alternative.

## Actual behavior and private ABI

The 20-byte layer record starts with an integer handle and two float fields at
+4 and +8. The helper receives that record in s0, level in f12 and style in f28.

1. If level differs, write +4 and call the genuine clamping client wrapper.
2. Reload +8 after that call; if style differs, write +8.
3. Capture the handle before blocking queue acquisition. Use that snapshot for
   the real transform operation with three -2 sentinel fields and the new style.
4. Release the same queue and return.

This is an internal IPA helper, not an ordinary O32 entry. Its 32-byte frame
saves ra; s1/s2 and f20/f22/f24/f26 may be changed. The incoming s0/f28 and other
untouched preserved registers are checked. Real native DFBA0 contains two calls
to the helper; E05F0 contains the actual outer call to DFBA0.

## Complete compilation and relocation proof

- Canonical strict comparison and independently parsed ELF function extent:
  all 38 words, exactly 152 bytes, no excess or uncertain references.
- Six relocations, including all four calls, independently replayed by GNU as/ld.
  Full unmasked bytes equal the authenticated native body. No owned literal,
  data, table, or alignment bytes are included; the whole compiled object has
  no nonempty data sections.
- The same full-body GNU check passes for all six runtime context bodies:
  transform, client wrapper, hierarchy wrapper, lookup, allocator and scheduler.
- Both complete actual callers are retained but remain unclaimed NONMATCHs:
  DFBA0 is 183/298 differences with a 1,184-byte ELF body versus native 1,192;
  E05F0 is 315/332 with 1,312 bytes versus native 1,328. Their incompleteness as
  native matches is not masked by the helper's exact result.
- The GNU proof copies each complete ELF function byte sequence unchanged into
  a temporary object with its original relocation records. Section-relative
  call addends are mapped to verified exact callee entries by absolute linker
  aliases. GNU performs every relocation. The proof does not use the scorer's
  relocation resolver or masks. All temporary instruction data stays local.
- Sixteen compile-time O32 assertions check actual record sizes and field offsets.

The archived A169 control still matches the target while retaining its allocator's
11/42 nonmatch. This explicitly separates new verification/context work from the
already-known matching helper source.

## Behavior and adverse checks

**3,116 cases** compare a transition oracle, unchanged complete candidate C under
host C89/UBSan, protected native instructions and independently GNU-linked code.
There are **6,232 native executions**. Native replay executes the actual helper,
client wrapper, transform, handle lookup and accepted allocator. Only the OS
queue calls use explicit callbacks; they validate arguments and poison ordinary
caller-saved integer/FP registers.

Cases cover unchanged/changed parameters, clamp boundaries, signed zero,
invalid-handle sentinels, valid/mismatched handle generations, every entity slot,
message counts wrapping at 255, varied free-slot prefixes, and callback mutations
at acquisition/release. They check the meaningful distinction between the
post-client style reload and the pre-acquisition handle snapshot. Native and
GNU-linked read/write traces, outputs, queue snapshots, stack canaries and private
ABI preservation agree. All **38 target instructions** and both outcomes of both
target conditional branches execute. Context coverage is reported individually;
full coverage of all context paths is not claimed.

Four compiled wrong contracts are rejected by strict compilation and host/oracle
counterexamples: omitted level store, inverted style predicate, style routed to
the wrong API, and handle read after queue acquisition. Test-level mutations of
real ELF instruction bytes, symbol extents and relocation types are also refused;
unknown native opcodes fail closed.

The host uses its native pointer layout and a fixture allocator with the same
first-free scan contract. The actual accepted allocator instructions execute in
both native replays. The host candidate file is included unchanged; unused large
caller functions are removed by the host linker's ordinary section garbage
collection, not by modifying their source.

The finite domain has accessible records, at least two free message slots and
single-threaded callback mutations. FCSR flags, signaling NaNs, exhausted
allocation, invalid pointers, arbitrary aliases/concurrent writes and gameplay
are outside this proof. No complete shadow unit, splice, source-built image,
compression, ROM hash or remote CI result is claimed.

## Reproduce

With the pinned IDO toolchain and GNU MIPS binutils available:

    python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_layer_update_20261005 --claims
    python3 cloud/work/ipa-groups/dot_layer_update_20261005/verify.py
    REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_layer_update_packet.py -q -o addopts=''

The normal changed-submission scanner discovers this registered IPA group and
strictly verifies its single claim. `verification.json` binds the complete
source, tests' implementation, compiler, recipe, archive and accepted context.
The ordinary verifier does not rewrite the receipt; `--output` is explicit.

No ROM bytes, raw assembly dumps, binaries, credentials, or unrelated private
data are submitted. Acceptance, merging and cartridge gates remain with the
independent checker.
