# Queue-wrapper declaration repair

Status: strict local verification passed; **not ROM-promoted**.
Base: `abc0f256e8c6b5e2004662af09aca7c5bf915204`.

## Repair and scope

The three existing sources in `cloud/matches/boot_tail/` now forward-declare
`struct OSMesgQueue_s`, the genuine SDK tag in `include/PR/os_message.h`,
instead of inventing a separate `struct MessageQueue` type. They only pass a
queue pointer, so no local layout definition or replacement ABI is needed.
Their extern queue and SDK function declarations use this same type.

- `func_800250F0`: blocking receive, then return zero; 48 target bytes.
- `func_80025120`: nonblocking jam of a null message; 48 target bytes. The
  existing unused `int token` parameter and void return remain unchanged.
- `func_80025150`: blocking receive with void return; 44 target bytes.

These are N64 SDK queue wrappers. No arcade counterpart is established.
All function bodies, symbols, flags and qualifiers are unchanged. The repair
only makes their declarations compatible with the queue already declared by
`func_800250AC` in `src/rom/lib_25bb0.c`. The stream-state object `D_80056230`
and every remaining stream-state candidate are untouched.

The normal changed-submission CI selector covers all three source paths.
There are no adapted-source copies outside that selector. Existing locks
remain unchanged and all 383 locked bodies pass the local hash guard.
The ten repair/lifecycle regression cases plus the existing cloud scorer,
integrity, submission-selector and path-guard tests pass (641 tests); the existing
promotion and lock unit tests also pass (18 tests). Repository-wide aggregate
validation is left to integration.

## Actual verification

`verify.py` compiles temporary copies of the actual production TU using
`tools/asm-processor/build.py`, the real headers and all remaining assembly
passthroughs. It does not replace the production file, update locks, run a
remote command, or invoke either mutating batch/diagnosis driver.

It checks the fixed BASE TU, the actual current production TU, and a combined
TU with only still-unpromoted queue candidates overlaid. All 21 TU functions,
totaling 3,836 protected target bytes, strictly match after full relocation
resolution in all three objects. This
includes all five previously locked C bodies: `func_80024FB0`,
`func_8002506C`, `func_800250AC`, `func_80025264`, and `func_80025594`.
The 3,836-byte regression surface is not new C coverage: only 140 bytes are
candidates for future promotion.

Every target section and extent is checked against the existing protected
manifest. The unchanged cloud scorer compares every relocated word, refuses
unresolved/unverified relocations and nonzero excess words, and verifies
function boundaries. The final TU has only four bytes of zero alignment
padding after its last 236-byte function. The standalone 44-byte wrapper has
four bytes of zero alignment padding; in the combined TU its span is exactly
44 bytes. All function offsets and the complete 3,840-byte text/padding extent
stay fixed; every compiled C function has an exact protected-target STT_FUNC
size. None of the three TUs introduces allocated data sections.

The verifier also establishes two negative controls:

1. Restoring the historical `MessageQueue` declarations passes the isolated
   `rom_tu.h` fitting check but makes the real combined TU fail on
   `D_800586A8` redeclaration. Header-only checks are demonstrably insufficient.
2. Changing the first wrapper's blocking flag causes one full-word mismatch.
   Source hashes and relocation-blind comparisons cannot stand in for the
   actual result.

`evidence.json` records source/header/target/toolchain hashes, exact flags,
all comparison results and both failure controls. It contains no ROM bytes
or assembly dumps. The compiler's 24 installed-file hashes were independently
checked against the recovered pinned IDO 5.3 v1.2 toolchain record before use;
the standard single-function and three-member group sanity checks also passed.

## Reproduce locally

With the pinned `tools/cloud/setup.sh` compiler and MIPS binutils available,
run from the repository root. `IDO_DIR` may select an already verified pinned
installation; it changes the executable location, not compilation flags.

```sh
python3 cloud/work/boot_tail_promotion/queue_wrappers/verify.py --output /tmp/queue-evidence.json
REQUIRE_TOOLCHAIN=1 python3 -m pytest -q tests/conveyor/test_queue_wrapper_promotion.py
python3 -m tools.conveyor.pipeline.lock check --quiet
```

Standalone locked flags remain `-g0 -O2 -mips2 -G 0 -non_shared`; the scorer
adds its existing `-Wab,-r4300_mul` erratum option. Full-TU compilation uses the
actual Makefile default:

```text
-G 0 -mips2 -O2 -non_shared -Iinclude -Iinclude/PR -D_LANGUAGE_C
-Wab,-r4300_mul -Xcpluscomm -Isrc/rom
```

`-Isrc/rom` only locates the unchanged real header for temporary TU copies.
GNU assembler flags are `-march=vr4300 -mabi=32 -Iinclude`.

## Promotion-safe regression lifecycle

The verifier reads the historical TU, locks and incompatible source declarations
from the fixed BASE commit. The historical type-conflict control therefore
survives real promotion. It requires the original five C bodies and their
current TU locks to remain intact. Candidate source bodies are checked against
BASE source locks because legitimate promotion migrates those locks away.

For the current production state, every C body must match its current TU lock.
Only remaining candidate passthroughs are overlaid; already-promoted candidates
must have current TU locks and are compiled and compared in place. Both partial
and complete promotion remain valid. All current and overlaid functions still
undergo the same strict native comparison, exact C symbol-size checks and
complete TU offset/extent checks.

Three temporary-state regressions exercise one, two and all three migrated
candidate locks, without changing the production TU or real lock file.
Additional controls reject a promoted body without its TU lock, removal of an
original lock, and a wrong promoted blocking flag even when its temporary lock
hash is updated. The last case ensures current lock consistency cannot replace
canonical machine-code verification.

## Maintainer boundary

Production `GLOBAL_ASM` slots, `matched.lock.json`, pinned context records,
symbol addresses, protected targets, scoring code and builder state have not
changed. The existing context JSON still describes the old preambles and
must not be mistaken for fresh evidence. Refit only these three source paths
if using the batch promotion workflow, and keep its candidate set limited to
these three wrappers rather than retrying every candidate in the segment.

The normal target-scoped promotion transaction can use the existing source
paths and locked body flags. The maintainer must run the real ROM build and
SHA-1 gate and migrate locks through the authorized promotion workflow. No
full-ROM gate was run here; no cartridge identity or new coverage is claimed.
