# Remaining boot-tail source-contract repairs

Base: `cf10b3392d7f00ae42d75c008b79fdc2541aab6b`.

This source-only packet prepares the 41 remaining refused candidates (8,192
target bytes) outside
[PR #82](https://github.com/cabi24/SFRush2049-decomp/pull/82). It does not promote
functions, migrate locks, establish new matches, or claim additional cartridge
coverage. The production assembly slots and all 383 current source-body locks
remain intact. The maintainer and independent checker retain the actual
promotion, full-ROM SHA-1 and merge gates.

## What is prepared

- [Audio records](../audio_record_contracts/README.md): 12 candidates / 1,500
  target bytes. Eleven work with the shared production declarations. The twelfth
  requires a separately proved cleanup-body adaptation and maintainer relock.
  Its two accepted names at +0x20 share one genuine unsigned halfword through
  IDO's anonymous-union extension (with the expected compiler warning), adding
  no field storage; independent native offset and 0x68-size checks pass.
- [Sequence contexts](../sequence_context_contract/README.md): 7 candidates /
  1,596 bytes. Consolidating the real 0xFF8 record requires source-text changes
  to six accepted bodies, so the canonical refactor exists only as a temporary
  actual-TU adapter. Its production TU and locks are untouched.
- [Voice and sequence lists](../voice_lists/README.md): 8 candidates / 1,900
  bytes. Genuine fields are exposed in accepted voice records, preserving
  their native strides and every accepted production function body.
- [Samples, buffers and emitters](../sample_buffer_contract/README.md): 6
  candidates / 1,356 bytes. Shared native records replace incompatible partial
  declarations. The emitter's established defined-path qualification remains.
- [Macro state and streams](../macro_stream_contracts/README.md): 8 candidates /
  1,840 bytes, with genuine accepted macro/helper, stream-record and SDK contracts.

Only source declarations, prepared candidates, explicit tests, research and
verification notes are changed. Protected targets, symbol addresses, pinned
context records, static locks, compiler flags and scoring code are unchanged.
No remote builder or promotion driver was used. Existing locked candidate bodies are
preserved; new adapted paths still require refreshed maintainer evidence.

## Combined-source proof and its limits

Each packet compiles real ROM translation units through the existing
asm-processor, retaining live assembly passthroughs. Every proposed C body and
all previously accepted C bodies in scope must have their exact ELF function
extent and complete native relocated bytes. There are no masked differences,
unverified relocations or unresolved symbols in those C proofs. Native layout
assertions and deliberate wrong-code/declaration/extent controls reject bad
inputs. Together, the final candidate overlays verify 144 C bodies across eight
actual TUs: all 41 candidates plus all 103 previously accepted C bodies in scope.
Promotion-lifecycle regressions keep these tests useful after a valid
maintainer transaction.

The per-packet receipts distinguish C-body proofs from whole-TU claims. In the
voice/list packet, two untouched assembly bodies, `8001B9F8` and `8001BE14`,
reference historical jump-table aliases absent from the protected scorer symbol
manifest. Their bytes and symbolic relocations are preserved against baseline;
a new completely relocated proof of those two bodies is not claimed. The
protected symbols are deliberately not patched to make this limitation vanish.
The macro/stream packet likewise preserves untouched assembly/padding against
baseline without claiming a new relocated-target proof for unrelated assembly
jump-table bodies. Its stream overlay reports a .reginfo register-mask change outside text;
its size/GP and allocated C data payload are unchanged.

Fresh audio, sample, sequence and macro receipts reproduce exactly in the
combined checkout. The voice receipt has six run-specific whole-object hashes
because .mdebug retains absolute temporary source filenames. All stable source,
target, exact-extent and relocated-body evidence matches; the differing raw
object hashes are retained as artifact identities rather than normalized away.

PR #82 remains separate. An optional read-only replay against its verified
published tree `13eb95cbe593611c823dfa3e05a9226cc9c0d1bd` combines its eight
macro and three queue wrappers with these eight macro/stream handlers. All 39
C bodies, including the 20 prior accepted bodies in those two TUs, pass together.
No PR #82 source file is copied into this packet or required by normal tests.

The emitter replay retains genuinely uninitialized float outputs. It covers
612 safe executions, rejects 172 unwritten-output cases and two helper-capacity
cases, and preserves prior-iteration values where the native behavior does.
Producer/listener invariants that exclude unsafe paths are still unproved.
This contract repair does not change those semantics or establish hardware
safety outside the qualified domain.

## CI and reproducibility

All five packet regressions are included in the existing normal `tests/cloud`
or `tests/conveyor` suites. This explicit coverage matters because the standard
changed-submission selector does not discover the adapted promotion directories.
A result of zero selected cloud submissions is therefore not a match claim.
`REQUIRE_TOOLCHAIN=1` is used for the focused regressions so missing toolchain
skips cannot be reported as a successful proof.

Final focused run: **64 passed, zero skipped**. The broader run reports
**1,984 passed, 17 failures (including two failed subtests), 41 skipped, nine
deselected, and 25 subtests passed**. Every failure identity and message was
reproduced on the untouched base: 15 existing blob section-relative relocation
checks, plus two builder subtests whose inline source-comment prose is parsed
as compiler flags. None was repaired. The skips are nine missing cross-GCC
checks and 32 missing SDK/ROM/generated-input checks; they are not passes.

See `verification.json` for exact commands, receipt comparisons, source hashes
and baseline controls. Passing source-contract tests does not make master CI
green.
No raw objects, native bytes, raw assembly dumps or private inputs are included.

Reproduce from the repository root with pinned IDO, MIPS binutils and the pinned
submodules available:

```sh
REQUIRE_TOOLCHAIN=1 python3 -m pytest -o addopts='' -q \
  tests/cloud/test_audio_record_contracts.py \
  tests/cloud/test_sequence_context_contract.py \
  tests/cloud/test_sample_buffer_contract.py \
  tests/conveyor/test_voice_list_promotion.py \
  tests/cloud/test_macro_stream_contracts.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest -o addopts='' tests/conveyor tests/cloud -q -rs -m 'not node_required'
python3 -m tools.cloud.guard_paths --base cf10b339 --head HEAD --lock-revision cf10b339
python3 tools/cloud/check_submissions.py --base cf10b339 --head HEAD
make check-matched
git diff --check
```

## Maintainer transaction

1. Adopt the reviewed canonical declarations and adapted source paths, then
   regenerate their context/source evidence using the approved process.
2. Re-prove and relock the accepted audio cleanup `func_8001144C` if adopting
   `func_80013964`, and the six sequence bodies `func_8001729C`, `func_8001734C`,
   `func_800173B4`, `func_80017410`, `func_80017470`, `func_800174D0` when adopting
   the canonical sequence-context adapter. These seven production bodies have
   not been changed or relocked here.
3. Use the normal narrowly scoped promotion transaction, fresh objects and the
   complete cartridge SHA-1 gate. Preserve the emitter qualifications and
   report any remaining proof limitations. Merging stays with the independent
   checker.
