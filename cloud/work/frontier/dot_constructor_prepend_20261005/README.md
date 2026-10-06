# Constructor prepend contract and accepted-context check

**NONMATCH: 24/77 differing words, unchanged from C41. Zero matching claims,
accepted bytes, or ROM coverage gain.** Target `car_stats_display` is the
historical label of a variadic record constructor at
`[0x800D197C, 0x800D1AB0)`, 308 bytes. No arcade donor or exact original typedef
has been recovered.

## Useful finding

C41 called the global list's word at +8 `tail`. Both complete accepted list
helpers establish a 16-byte list header with **head at +8 and tail at +12**.
The constructor passes the head as `before` to `func_80091FBC`: it requests
insertion before the current head. Its existing machine behavior was already
right; the old source name and explanatory text were wrong. `candidate.c`
corrects that structural view and names the member `head`.

The native caller constructs a record with a narrowed kind byte, two float-bit
fields, two fixed unsigned payloads, up to four variadic words, and -1 fillers.
It publishes through the list helper, sets the active byte, captures the handle
**after insertion**, releases the queue, and returns the saved handle. Word
payloads and list links are represented by unsigned 32-bit carriers; this is
bit-preserving evidence, not proof of original source typedefs. A manifest-backed
direct JAL census finds one caller: `race_countdown_display` at `0x800D2910`.
Indirect callers and registration are not established.

## Bounded compiler result

Four fixed builds were checked:

- Archived C41 source, ordinary O3
- Corrected list view, ordinary O3
- Corrected source under its recorded O2 header
- Corrected source plus the complete unchanged accepted allocator, insertion,
  and removal functions, under the existing ordinary O3 group pipeline

Every constructor emits the complete 308-byte body and retains **24/77**
differences. The corrected O3 and genuine group caller bodies are byte-identical
to C41. All three accepted context bodies remain exact: `func_800D18D8` 164 bytes,
`func_80091FBC` 352, and `func_8009211C` 348. The context's existing externally
visible roots remain kept. No invented caller, dead read, extra argument,
unused buffer, volatile qualifier, or protected build change is introduced.

The candidate includes a compiler-conditional stdarg compatibility header:
IDO uses C41's recorded SDK macros; the host uses its own standard `stdarg.h`.
The function body itself is unchanged between native and host compilation.
All already accepted source files are read without alteration.

Stop this tested context hypothesis here. Reopening needs independent evidence
for the original variadic interface, original source-unit boundary, or a
compiler mechanism that predicts the remaining register/scheduling differences.
No spelling sweep was run.

## Verification

`verify.py` freshly compiles the sources and verifies complete ELF STT_FUNC
extents, every instruction position and every relocation. GNU ld independently
resolves all references, agreeing with the unchanged canonical relocation path.
For the concatenated group, a temporary ELF witness changes defined function
symbols to unresolved symbols and supplies their proven addresses in the linker
script. No instruction bytes or relocation records are modified. Original ELF
symbol extents supply extraction bounds; there is no target-length truncation.
No owned data, literals, jump tables, or BSS exist. Zero text-alignment bytes are
reported separately and receive no credit. Shortened ELF extents and unsupported
relocations are rejected.

The behavioral proof covers 1,792 cases (256 deterministic arbitrary-bit seeds
by counts -2 through 4), 3,584 protected-native/GNU-linked caller executions,
and 1,792 unchanged-host C89/UBSan executions. All 77 instruction offsets and
both outcomes of every conditional execute in both bodies. It checks the full
60-byte output record, queue argument/order, before-head insertion argument,
post-insertion handle reload, saved return across release-time mutation, mapped
memory canaries, saved registers, and stack restoration. Source mutants for
before-tail insertion, wrong fillers, inactive publication, and wrong returned
handle, premature time read, and premature handle capture are rejected, as
are unknown instructions and a broken stack restore.

**External allocation, insertion, receive and release are bounded contract
hooks. Their native internals are not executed by these behavioral tests.**
Their accepted source bodies are separately byte-verified in compiler context.
The hooks deliberately change the handle at insertion and release to test the
caller's ordering, and publish the start-time bits only after queue acquisition.
This does not model a native scheduler or concurrent list mutation.

Counts above four, invalid/null allocation results, invalid pointers, hardware
FCSR effects, original C typedefs, real scheduler behavior, complete callers,
full-game shadow/image/compression/ROM gates and gameplay are outside scope.
Negative counts demonstrate the complete native fallback behavior; they are not
asserted reachable in gameplay. Tests are finite evidence, not universal proof.

## Reproduce

With the repository's pinned IDO and GNU MIPS tools available:

```sh
python3 cloud/work/frontier/dot_constructor_prepend_20261005/verify.py
python3 -m pytest -q tests/conveyor/test_dot_constructor_prepend.py
```

`--write` regenerates the frozen receipt; the default command compares a fresh
proof against its portable evidence. IDO embeds the absolute source path in
nonallocated ECOFF `.mdebug`, even with `-g0`. Controlled builds of each
single-source variant at two differently sized paths show that only `.mdebug`
payload/size and physical file offsets vary. Every other ELF section is equal.
The original-path builds reproduce all three saved full-object hashes.

The receipt keeps complete object/debug hashes and host/linker identities as
run provenance. Portable replay excludes only the three single-object/debug
hashes and the GNU linker/host compiler version strings. The relative-input
group's full object/debug hashes remain equality requirements. All four objects
also bind every non-debug section's bytes, size and attributes plus ELF identity
and ABI metadata. In particular, code, relocation records, symbols, `.options`,
`.reginfo`, data/BSS and target/source/context hashes remain checked. Physical
file offsets and the verified nonallocated `.mdebug` payload/size are the only
ELF fingerprint exclusions; allocated or executable debug sections are rejected.

Actual IDO source-path controls and synthetic mutations reject changes to code,
relocations, symbols, data, ABI and section attributes. Native/host behavioral
checks, malformed-object controls and independent GNU relocation are unchanged.
Tool identity remains visible, while portable replay requires the same verified
native results. Base and ownership
are recorded in `claim.json`. The target is unlocked on base `cd22879d`.
Only additive source/research/test files are included. No ROM bytes, assembly
dumps, binaries, protected-source edits, or matching submission are added.

## Local test scope

The original packet's six focused tests and selected cloud scorer, submission,
protected-path and integrity regressions passed: **738 tests, no failures or
skips**. The portability follow-up passes **32 focused packet tests**. A broader
local attempt passed **764 tests** but encountered 27 setup errors from a missing
sparse-checkout hook file and exhausted scratch inodes. No broad all-green result,
full repository suite or remote CI pass is claimed for this follow-up.
