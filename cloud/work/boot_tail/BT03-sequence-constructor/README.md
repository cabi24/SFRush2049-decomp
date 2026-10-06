# Sequence constructor 800178B0: complete reconstruction

**COMPLETE-NONMATCH. Zero new matching bytes or cartridge coverage.**

This packet adds the first complete C reconstruction for boot-tail
`func_800178B0`, native interval **0x800178B0–0x80017D38, 1,160 bytes**.
It was open and unclaimed in the current D10 ledger at base
`cd22879d40b3de443cfde047b86e75e159b6cec6`; the 25 open PRs checked on
2026-10-06 did not cover this function. Work is isolated on
`dot/boot-tail-fresh-178b0`. No central ledger, target, shared header,
production source or lock is changed.

## Result and why it is useful

The complete retained source compiles to a **1,160-byte ELF function** under
pinned IDO 5.3/O2. Its whole relocated body differs in **206/290 words**,
with zero unresolved symbols, unverified relocations, relocation errors or
nonzero excess words. The 1,168-byte text section ends with eight zero padding
bytes. There are no owned data sections. This is a broad nonmatch, not an
execution-ready matching claim.

The new reconstruction supplies the sequence constructor's five real inputs,
complete 4,088-byte context layout, 132-byte program view, and initialization
behavior. It replaces the old role-only metadata with an executable source
baseline and bounded native validation. Naming describes behavior; no exact
original middleware version, original source file or donor identity is claimed.

The main residual is structural: the candidate frame is 120 bytes versus
152 native bytes, with different pointer spill/reload, packed-bank load and
loop scheduling geometry. The first 26 instructions after the frame allocation
are otherwise identical. Artificial locals, padding, unused parameters,
volatile accesses, keepers, inline assembly and compiler/target changes are
not acceptable ways to fill the frame difference. Stop here until authentic
source/compiler context explains that geometry.

## Whole-body behavior and contracts

Signature: a word-valued result from five pointers: first bank, second bank,
nullable program, sequence data, nullable options.

- Scan eight contexts and select the first nonzero available byte at +4033.
  If none is available, return all ones without changing state or calling a helper.
- Store the two banks and sequence pointer, clear pending state, initialize two
  128-byte bank maps, and fill 64 track-group bytes with slot+23.
- Apply nullable option flags: bit 0 supplies two enabled masks, bit 1 supplies
  the unsigned speed, bit 3 supplies track/group assignments and group resets,
  bit 2 applies one default-group volume fade and optional additional fades.
- Store tempo and pass it with the byte slot to the tempo helper. Initialize
  the optional master stream. If absent, only its current pointer is cleared;
  its other three fields retain their previous contents.
- Reset all 64 stream time pairs and all three event/pitch/modulation pointers
  per track, resolve present offsets relative to the sequence allocation, and
  set all 64 external level bytes to 127. Unwritten track fields are retained.
- Clear the note heads, reset all 16 MIDI channels, set all 16 program words to
  all ones, then process both 128-entry maps in order. A map index of 255 or a
  bank channel of 255 is skipped. A duplicate channel is overwritten by the
  later entry, including entries from the second bank. Four packed identifier
  bytes are assembled with unsigned arithmetic.
- Apply each of the optional program's 16 channel presets and four controller
  values (7, 10, 91, 93), retaining live reads across the external calls.
- Clear the counter. Set active unless option bit 4 is present; that bit leaves
  the previous active byte untouched. Allocate the returned identifier, then
  clear the selected context's availability.

Direct native boot-tail caller `8001558C` has two call sites, `80015660` and
`8001568C`. Both pass the two derived bank pointers, the current packed 132-byte
program, the sequence pointer, and options at outgoing stack+16; both preserve
and return the result. The guarded path saves that result around release.
This agrees with the archived caller in `BT03-low-registers`. It does not imply
that unobserved indirect callers have been exhaustively identified.

The constructor reads all five inputs. Program header bytes are represented
as bytes, avoiding an unwitnessed alignment requirement on the caller's packed
record. Banks use eight-byte records with byte fields and a sentinel key at +5.
The context's unknown ranges correspond to real fields outside this function's
writes, not frame padding. IDO compile-time assertions verify every used native
field offset and all nine modeled type sizes. Caller and eight callee full-body
hashes are retained in `evidence.json`.

Validity requires finite sentinel-terminated bank records, keys below 128,
non-sentinel mapped channels below 16, valid track/group indices, and aligned
sequence offset words addressing the actual underlying allocation. This code
preserves the native lack of input validation; it does not add arbitrary bounds
checks. Models do not establish behavior for malformed resources, asynchronous
memory mutation, or the complete surrounding middleware.

## Bounded source experiments

Every retained row uses the unchanged common flags plus the scorer's mandatory
`-Wab,-r4300_mul`. Five natural source forms were compiled, with an O1 control
for the final source:

| Source | O2 differing words | ELF bytes | Notes |
|---|---:|---:|---|
| `controls/baseline.c` | 260/290 | 1,144 | Initial complete early-return body |
| `controls/guard.c` | 206/290 | 1,160 | Positive allocation guard and explicit option-dependent activation |
| `controls/store_order.c` | 215/290 | 1,156 | Sequential pointer/zero stores and assignment-valued tempo input |
| `controls/packed.c` | 258/290 | 1,152 | Packed halfword plus two bytes for bank identifiers; speculative field labels |
| `controls/selected_index.c` | 259/290, 3 excess | 1,172 | Final availability indexed by slot, grouped low identifier half |

The retained body is the guard form, with readable indentation and a byte-based
program header. Those final cleanups leave code identity unchanged. The packed
field names in a rejected control are hypotheses, not recovered declarations.
Final O1 produces a 1,752-byte function, 290/290 differing words and 143 nonzero
excess words. No broad allocator/permutation sweep was performed.

Workbench diagnosis ran on the initial and final complete objects before and
after the bounded structural controls. `diagnosis.json` keeps only safe summary
metrics; the authoritative residual is the strict relocated scorer. The
workbench compares an already-linked native object to a relocatable candidate,
so its relocation-symbol warnings do not establish unresolved relocation errors.
The strict replay resolves the entire candidate text section separately.

## Verification and reproduction

With the established IDO 5.3 toolchain and normal host C compiler:

```sh
python3 cloud/work/boot_tail/BT03-sequence-constructor/verify.py --check
python3 cloud/work/boot_tail/BT03-sequence-constructor/test_packet.py
```

The verifier is independent of the current working directory. It does not
install or download anything. `evidence.json` binds source and harness hashes,
canonical native inputs, compiler flags, all control source hashes, complete
ELF extents, resolved text hashes, and native/host behavior receipts.

- **1,782 native-versus-compiled cases** cover all nine allocation outcomes,
  all 32 flag combinations plus null options, optional defaults/master,
  zero/nonzero counts, duplicate map destinations, all-one/high-bit identifiers,
  controlled live option/program changes, and full caller-save clobbering.
  The entire non-stack memory and external-call trace agree. Every one of the
  **288 conservatively reachable native instructions** is executed; the two
  unreachable duplicated stores at +0x114/+0x138 are independently excluded by
  a delay-slot-aware conservative CFG walk.
- **16,896 host C89 cases** cover all 256 availability masks, all flags/null
  options, and absent/present defaults/master. ASan and UBSan pass. Leak
  detection is disabled because it is unavailable under this execution host's
  tracing; the one allocation is explicitly freed. Host LP64 layout is not
  substituted for N64 layout.
- Focused tests cover source/receipt identity, complete ELF accounting,
  relocation/data absence, native layout, caller sites, instruction coverage,
  big-endian memory, branch delay and annulment behavior, fail-closed memory and
  opcode handling, and rejection of a deliberately changed C group default.

The integer interpreter supports only the instructions used here and fails on
unexpected opcodes or callees. Helpers are explicit deterministic boundary
models, with mutations to test required live reloads. Thus this is a proof of
this body's bounded behavior and interfaces, not execution of every callee.

Independent source review and any publication are separate. No production
acceptance, lock credit, source-owned image, compression/ROM gate, full-project
suite or hosted CI result is claimed. No CI watcher is requested.
