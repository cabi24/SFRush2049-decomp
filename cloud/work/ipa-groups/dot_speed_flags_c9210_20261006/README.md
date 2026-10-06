# Complete command-writer match from full-word flag inputs

## Current receipt schema (2026-10-06)

Current receipts omit whole-file manifest, scorer/own-data tool, lock and redundant
production-context digests. The recorded BASE and `git show BASE:path` source reads
remain, as do native target authentication, own packet/verifier/C-harness bindings,
compiler executable identities, complete compiled extents, relocations, owned data
and behavioral evidence. Historical compatibility normalizers accept only their
explicitly listed legacy fields; unknown proof fields and changed invariants still
fail comparison. Descriptions of the earlier receipt schema below are historical.
This schema correction adds no matching or accepted bytes; fresh replay and the
aggregate test matrix are separate required checks.

## Integration portability update (2026-10-06)

The receipt no longer hashes its pytest wrapper. Base-commit source ancestry and recipe checks remain intact. Replay comparison excludes only the historical accepted-source digest; packet sources, verifier, recipe, native words/addresses, relocations, owned data and behavior remain strict.

Focused packet tests passed with IDO available and with IDO absent. Compiler-dependent tests skip without IDO or the MIPS GNU linker. These scoped results do not claim the required aggregate repository-suite pass; that matrix is recorded separately before any publication. No protected production files, native assets, accepted locks or compiler policy are changed.


**Sole new matching claim:** `speed_set`, `[0x800C9210, 0x800C92DC)`,
204 bytes / 51 words. **Accepted-byte and ROM-coverage gain: zero.**
Base: `cd22879d40b3de443cfde047b86e75e159b6cec6`.

## Source and discovery

The complete source is the existing accepted
`src/blob/groups/codex_vsync_a145/group.c`, with exactly two formal types changed:
`speed_set`'s `first` and `second` parameters are `s32` instead of `s8`.
The compiler flags, real four-root keep list, five complete function bodies,
control flow and byte-store casts are otherwise unchanged. The proof enforces
this exact difference and the original recipe. There are no new helpers,
pressure locals, padding, volatile qualifiers, extra arguments or inline assembly.

The old narrow interface emitted six extra instructions to store and sign-extend
the inputs. The new full-word carriers preserve the native interface, which
receives the flags in `s1` and `s2` and truncates only at the byte stores.
This was found while examining the genuine caller `func_800D6160`; that caller
remains nonmatching and is not claimed or added to this unit.

The historical names `speed_set` and `vsync_wait` do not establish their semantic
names. The proven routine allocates and queues command `0x0A`, clamps one float
to `[0,1]`, clamps another below at zero, and stores two byte flags. No direct
arcade ancestor or original signedness/typedef is claimed for this routine.
The audited callers all supply Boolean word values, so they cannot distinguish
original `s8` and `s32` source semantics. The native code supports this source
reconstruction and its wider bounded bit-carrier tests, not original text recovery.

## Complete compiler evidence

- IDO 5.3, original `-g0 -O3 -mips2 -G 0 -non_shared` whole-program recipe;
  the unchanged scorer also supplies its mandatory `-r4300_mul` assembler flag.
- `speed_set`: complete 204-byte ELF extent, 51/51 relocated words exact,
  all eight target relocations resolved.
- Already accepted `vsync_wait`: complete 148 bytes / 37 words remain exact,
  with no duplicate credit.
- The other real compiled context remains explicitly nonmatching:
  `speed_mode0_wrapper` 6/22, `speed_mode1_wrapper` 6/22, and
  `continue_prompt` 10/23. Their full ELF extents are 88, 88 and 92 bytes.
- GNU `readelf` independently measures every function. GNU `ld` links the
  **unmodified full ELF object**, resolving all 20 unit relocations. Every
  complete function is compared; no native-length truncation or masks are used.
  Four zero alignment bytes are outside all functions. No owned data or literals.
- The original narrow source has a **228-byte** ELF body: 47/51 native positions
  differ, plus six excess positions (five nonzero and one zero), for **53
  complete-extent differences**. The old scorer's five-extra-words summary is
  not the ELF extent. The frozen control records both quantities.

The full GNU link anchors the first function at C9210. Other functions are
compared at their resulting object offsets; byte equality does not establish
original contiguous translation-unit placement. Production image integration
is a separate gate.

## Native callers and behavior

`callers.json` preserves an independently reviewed census of 21 direct JAL sites
in 13 registered game functions. Every site supplies `s1/s2` as words 0 or 1.
The countdown site uses `s4 = 1`; the replay checks the defining instruction
and intervening direct writes. Full caller hashes and entry addresses, all
selected native extents, external relocation bindings and source hashes are
receipt-bound. The authenticated target address is explicitly checked, including
an address-only corruption regression. Computed, static-image and overlay
callers are outside this direct-JAL census.

The submitted tests execute unchanged host C89 with ASan/UBSan, protected native
code, independently GNU-linked code and a scalar byte oracle:

- 8,000 fixtures / 16,000 native executions cover finite normal values, signed
  zero, both clamp boundaries, full-word flags and every low byte.
- All 49 reachable instruction offsets execute. Two compiler-emitted duplicate
  fallthrough instructions at offsets `0x78` and `0x98` are unreachable, but are
  included in complete byte equality. Both outcomes of all three conditional
  branches execute.
- Checks include command contents, unchanged record bytes/canaries, exact call
  sequence and arguments, release-time record mutation, all four caller argument
  homes, saved return, stack restoration and the native register contract.
  `s0` and `s3` are explicitly documented IPA clobbers. The flag registers and
  floating argument registers remain preserved by this body.
- Four compiled source mutants and five malformed-native controls are rejected.
- Eleven focused tests require no optional compiler skip. Full compiler replay
  is explicit below.

The allocation, receive and two enqueue calls are bounded contract hooks;
actual allocator/queue internals do not execute. A valid aligned writable record
of at least 14 bytes is required (tests use 24 bytes). Null allocation, actual
queue storage, scheduler behavior, concurrency, hardware floating exceptions,
FCSR effects, NaNs/subnormals/infinities, traps and gameplay are outside this
packet's proof. Finite tests are not universal equivalence.

## Reproduce

From the repository root with pinned IDO and MIPS GNU binutils available:

```sh
python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_speed_flags_c9210_20261006 --claims
python3 cloud/work/ipa-groups/dot_speed_flags_c9210_20261006/verify.py --compiler
python3 -m pytest -q tests/conveyor/test_speed_flags_contract.py -o addopts=''
```

`verify.py` alone checks bindings. `--compiler` freshly rebuilds and compares the
entire source-bound receipt. Only explicit `--record` replaces the receipt.
Compiler/tool hashes, full extents and resolved-body digests are in
`verification.json`; raw instruction data, ROM bytes and objects are not saved
in the packet.

## Checker handoff

This is a candidate submission. Existing production source, locks, flags,
protected targets and scoring tools are unchanged. No source-image, compression,
full-ROM SHA-1, hardware or gameplay gate has run. The independent checker owns
production integration, acceptance and merging. No CI monitoring is requested.
