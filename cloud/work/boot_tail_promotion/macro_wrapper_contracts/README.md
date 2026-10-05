# Shared macro-wrapper pointer contract repair

**Eight repair candidates, 352 bytes, pass the actual combined-TU check.**
All 15 existing locked C bodies (808 bytes) also pass, with no byte or extent
regression. This is prepared source and local verification only: no production
promotion, new matching credit, cartridge coverage, or ROM SHA-1 claim.

## Scope and provenance

Base and freshly fetched master: `abc0f256e8c6b5e2004662af09aca7c5bf915204`.
The owned production TU is `src/rom/lib_22300.c`; it remains unchanged.
`../gate_refusal_diagnosis.jsonl` records the eight `func_80023754` redeclaration
failures after the accepted `func_80023818` declaration. `../final_status.json`
records those refused candidates and the accepted population.

The existing candidate files in `../sources/` have score-zero source locks.
They are preserved, as are every lock, pinned context, target, symbol table,
compiler flag, generated layout, and other production source. The `sources/`
files here are new repair inputs derived from those exact legacy sources.
Fast tests prove their non-comment tokens differ only in the three type names.

The original adaptation made a new opaque state/control struct tag and command
struct tag for each function. Those are distinct C types, so declaring the one
external `func_80023754` with all of them in the same TU is invalid. These repairs
reuse the exact `MacroState_80023818`, `MacroControl_80023818`, and
`MacroCommand_80023818` declarations already accepted in this TU. Their suffix
anchors the existing declaration; it does not imply separate runtime objects.
The state and control stay opaque. There are no aliases between incompatible
struct types, old-style declarations, `void *` erasure, fake ABI formals,
volatile qualifiers, padded layouts, inline assembly, or scoring changes.

## Native contract and retained values

The complete dispatcher `func_80023E9C` passes the same state/command roles in
a0/a1 at its calls +0x7A8 through +0x888 and consumes byte v0. The complete
196-byte `func_80023754` takes state, embedded control, command, and mask in
a0–a3. It accesses packed state flags at +0x24, four 4-byte control entries
and their count at +0x10, and command words +0/+4; its sole callee is
`func_80021548`. The wrapper does not interpret or dereference the state/control
record, so opaque pointers and byte-offset selection preserve the real contract.
The command remains the known two-word object. `func_80023754` itself is left
as assembly and no new callee implementation is supplied.

| Wrapper | Control offset | Mask |
|---|---:|---:|
| `func_80023818` (already accepted) | `0xC4` | `0x00200000` |
| `func_80023844` | `0xD6` | `0x00400000` |
| `func_80023870` | `0xFA` | `0x00800000` |
| `func_8002389C` | `0x11E` | `0x01000000` |
| `func_800238C8` | `0x130` | `0x08000000` |
| `func_800238F4` | `0x142` | `0x04000000` |
| `func_80023920` | `0x154` | `0x02000000` |
| `func_8002394C` | `0xE8` | `0x10000000` |
| `func_80023978` | `0x10C` | `0x20000000` |

Supporting repository research: `../../boot_tail/BT05-controls/README.md`.
Canonical protected targets: `asm/us/boot_tail/`; native TU passthrough sources:
`asm/us/nonmatchings/rom/lib_22300/`. Original opcode names and complete state
layout remain unknown. The authenticated arcade checkout is absent; no arcade
counterpart is asserted or copied.

## Verification

`verify.py` is read-only apart from its temporary directory and stdout. It does
not import or execute the mutating promotion/diagnosis driver. It mirrors the
existing promotion driver's declaration deduplication as a pure text operation,
placing only these eight candidate bodies into temporary copies of the real TU.
It retains all other C bodies, pragmas, and assembler passthroughs, then invokes
the vendored asm-processor, pinned IDO, and MIPS assembler with the actual ROM
TU flags. This is not a concatenation of isolated function snippets.

`verification.json` records the final replay:

- All 24 installed IDO files equal the pinned Packet 2 compiler hashes. The
  standard single and three-member group smoke checks also passed locally.
- All protected boot-tail manifest entries pass integrity verification.
- Each of the 23 functions is compiled standalone under its exact source flags,
  standalone under Makefile flags, and with the unchanged `rom_tu.h` checker.
  All have identical full relocated bytes and no data/rodata allocations.
- The immutable baseline, actual current TU, and proposed full TU compile through asm-processor. All 23
  bodies strictly match canonical target bytes with zero differing/extra words,
  unresolved symbols, unverified relocations, errors, or masked relocations.
  All 23 exact `STT_FUNC` sizes equal their target extents and end at their next
  function symbol; no neighbor borrowing or object padding counted as a body.
- All 66 TU function offsets stay identical. All 13,552 raw `.text` bytes,
  including padding, stay identical. Allocated non-text bytes also stay identical
  (only the existing 24-byte `.reginfo` section is present).
- The old independently suffixed sources still pass the header-only checker but
  reproduce the actual combined-TU redeclaration refusal. A deliberately wrong
  control offset is rejected by both standalone and full-TU strict comparison
  (one differing word each).
- A C89 host harness checks all nine wrappers, including accepted `80023818`,
  with 36 cases: one helper call, exact pointers/mask, zero return, and no local
  mutation of state or command. It does not purport to emulate the helper.
- Eight fast tests cover unchanged source tokens, shared declarations,
  offsets/masks, short-body/overflow rejection, exact TU extent enforcement,
  and missing/duplicate passthrough rejection.
- The final 16 packet tests plus the canonical scorer, asm-owned-directive, and
  lock test suites pass: **613 passed**. The separate lock check reports all **383** locked
  functions intact; this hash check supplements the compiled regression above.

Reproduce from the repository root after standard toolchain setup (`IDO_DIR`
may point to an existing pinned compiler installation; MIPS binutils must be on
`PATH`):

```sh
python3 cloud/work/boot_tail_promotion/macro_wrapper_contracts/verify.py > /tmp/macro-wrapper-proof.json
python3 -m pytest -q cloud/work/boot_tail_promotion/macro_wrapper_contracts/test_verify.py
python3 -m tools.conveyor.pipeline.lock check
```

The new sources are outside the normal changed-submission rescore selector, so
that CI job alone is insufficient. Run the explicit verifier above at review
and integration. Numerical proof contains no ROM bytes, raw assembly, or objects.

## CI lifecycle after promotion

The verifier now reads the immutable reviewed TU and lock set from `BASE` for
its baseline and original-refusal controls, then separately checks the actual
current production TU. Every currently accepted C body must match its current
body lock. The original 15 locks must remain, but the population may grow.
Only candidates still represented by assembly passthroughs are inserted into
the temporary overlay; already-promoted candidates require valid current TU
locks and are verified in place. All eight repair sources still receive their
standalone/header/source replay even after promotion.

Every later accepted body also receives exact-extent/full-relocation comparison
against its canonical target in the baseline, current, and overlaid TUs. The
function layout, text size, allocated non-text bytes, and untouched assembly
and padding stay fixed. Raw relocation encodings inside already fully verified
C bodies may differ after a legitimate later promotion; their full relocated
bytes must still match without masks. `all_raw_tu_text_bytes_equal` reports the
additional raw equality result (true for this packet); it is not a permanent
requirement that would reject a different valid relocation representation.
Two unrelated assembly jump-table aliases are not resolvable through the pinned
boot-tail symbol table, so no new whole-TU relocated-byte claim is made for them
and no symbol mapping is invented.

Eight additional tests cover promotion-state detection, missing/stale locks,
unlocked bodies, raw-encoding ownership/padding, and full replay with one or all
eight wrappers treated as promoted. A deliberately incorrect already-promoted
body is rejected even when its temporary fixture lock is recomputed to match. Their production-source and lock fixtures
exist only as in-memory values and temporary compiler inputs. The real lock
file and production TU are never edited. These tests prove the permanent CI
entry can keep validating source after the normal maintainer transaction.
The verifier restores scorer target-selection state when called in-process.

## Maintainer integration boundary

The repair sources are not registered in `matched.lock.json`. Integration still
requires independent review, fresh coordinator/static-pool true-zero evidence
and context evidence for these source paths, correct gated lock replacement,
then the usual promotion transaction and forced full-ROM SHA-1 verification.
Do not blindly add duplicate source locks or execute the legacy batch drivers.
No coordinator or remote builder was contacted, and no ROM gate was attempted.

The other four refused handlers in this TU (`80021BF0`, `800225FC`, `80022678`,
`80023520`) have separate shared-callee contracts and remain outside this repair.
Their assembly passthroughs are retained byte-for-byte. No additional conflict
appeared within this eight-wrapper combined-TU trial.
