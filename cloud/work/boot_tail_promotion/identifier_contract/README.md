# Identifier-wrapper declaration repair

Verified locally on 2026-10-04 against master
`abc0f256e8c6b5e2004662af09aca7c5bf915204`.

## Result and boundary

The only production-source change is the return type of the existing
`func_8001EDF4(u32)` declaration in `src/rom/lib_207b0.c`: `u32` becomes `int`.
It now agrees with the accepted definition in `src/rom/lib_1f5b0.c` and the
unchanged, already locked 48-byte `func_800201D0` candidate. No function body,
argument, ABI, flag, protected target, symbol address, pinned context, or lock
was changed.

The candidate remains a `GLOBAL_ASM` passthrough in the committed TU. Its C
body was substituted only in a temporary verification overlay. This is a
verified source-contract repair, not a promotion or a cartridge-coverage claim.
The full ROM build, ROM SHA-1 gate, lock migration, and promotion record were
not run. They remain the maintainer's integration transaction.

## Contract evidence

- Accepted callee: `src/rom/lib_1f5b0.c:func_8001EDF4` takes one unsigned word
  key and returns `int`. A key equal to the all-ones sentinel or a missing
  `SequenceNode` returns `-1`; a found node returns its signed word at +12.
- Native callee: `asm/us/nonmatchings/rom/lib_1f5b0/func_8001EDF4.s` has the
  sentinel test, a call to `func_8001EAA0`, packed node-word loads, and the
  all-ones failure result. This confirms the bit-level contract; the native
  instructions alone do not distinguish an original `int` from `u32` typedef.
  The signed declaration is selected for consistency with the accepted C
  definition and its explicit failure result, not an invented original name.
- Wrapper: `cloud/matches/boot_tail/func_800201D0.c` compares the callee result
  with `-1`, then returns the original unsigned input on success or
  `0xFFFFFFFF` on failure. Native `lib_207b0/func_800201D0.s` confirms a single
  word input, preserved input return, and all-ones failure. Its unsigned return
  type and body are unchanged.
- Existing same-TU callers `func_8001FD2C` and `func_80020048` assign the result
  into their unsigned identifier and compare it with `0xFFFFFFFFU`. Conversion
  of `-1` to the 32-bit unsigned identifier preserves all bits; both variants
  return one word through the same ABI register. No signed ordering test is
  introduced. Fresh actual-TU codegen proves that neither caller changes.
- Other native callers support only the same word/sentinel contract:
  `lib_17dc0/func_80017540.s` removes an entry on all-ones failure;
  `lib_1cf90/func_8001DDE0.s` stores the returned word as the emitter identifier
  and tests for all-ones failure. Their TUs were not edited. Future list/voice
  repairs should retain this evidence rather than adding conflicting local
  prototypes.
- Original middleware names and any arcade equivalent remain unknown. This
  is a native N64 audio/middleware reconstruction; the arcade reference repo
  is not available in this cloud checkout. Existing source-led corroboration:
  `cloud/work/boot_tail/BT03-high-medium/README.md` and
  `cloud/work/boot_tail/BT03-high-next/README.md`.

The original refusal is retained as historical evidence in
`../gate_refusal_diagnosis.jsonl`: conflicting redeclaration of
`func_8001EDF4` in the combined TU. `verify.py` reproduces this failure from the
original base plus the unchanged candidate before testing the repair.

## Fresh verification

`verify.py` builds the actual complete TU with the checked-in asm-processor,
IDO 5.3, the Makefile's ROM-TU `-O2` options, and every remaining assembly
passthrough. It does not strip peer C bodies or use a standalone-header-only
check as combined-TU evidence.

Three local builds all pass:

1. Original base: all 17 accepted C bodies, 1,820 bytes.
2. Declaration repair: the same 17 accepted C bodies, unchanged bytes.
3. Repaired TU plus candidate overlay: 18 C bodies, 1,868 bytes; the new
   wrapper's exact `STT_FUNC` extent is 48 bytes.

For each C body the verifier requires the exact protected-target extent and
complete relocated byte equality with no masked fields, unresolved symbols,
unverified references, or relocation errors. Short bodies, zero words added
inside a symbol's extent, and overlapping neighbor extents fail. It validates
the protected target manifest before reading target words or symbol addresses.

GNU `ld` independently relocates each entire real-TU object at its native base.
All 24 TU slots match their protected targets in all three builds. The complete
linked `.text` has 2,656 bytes, including 12 terminal zero-alignment bytes outside
the final target slot; those bytes are not counted as any C function's extent.
Its SHA-256 is identical for baseline, repair, and candidate overlay:
`174d946435b5cee3bec2df4be71ec64b4ef19599eea79ea5a21460515ae44d8c`.
The unchanged standalone candidate also freshly matches all 48 bytes.

`verification.json` contains per-function hashes, sizes, compiler hashes,
protected-input hashes, flags, the negative control, and explicit ROM-gate
status. It contains no object files, ROM bytes, raw assembly dumps, or secrets.

## Reproduce

From the repository root, with the pinned IDO toolchain and MIPS binutils
available (use `IDO_DIR` if the compiler is installed outside `tools/cloud/ido`):

```sh
python3 cloud/work/boot_tail_promotion/identifier_contract/verify.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_identifier_contract.py tests/conveyor/test_cloud_score.py
make check-matched
git diff --check
```

Recorded results: 594 tests passed, including 11 new regression cases; all 383
static locked bodies remain intact; `git diff --check` passed. The normal scorer
sanity checks also passed for `sound_handles_clear` and the three-member
`resource_slot_clear` group. Repository-wide aggregate tests were not run as
part of this isolated tranche.

The changed-submission selector does not select `src/rom/*.c` or adapted
`cloud/work/boot_tail_promotion/sources/*.c`. The new actual-TU regression test
lives in `tests/cloud`, so the existing CI Cloud tooling tests execute it
explicitly with `REQUIRE_TOOLCHAIN=1`. Missing compiler/binutils cannot silently
pass in that mode. The test can continue verifying later same-TU promotions;
it does not require the whole source file to remain byte-frozen.

## Maintainer integration

After review, retain the signed declaration and run the existing promotion
transaction scoped to `func_800201D0` with its unchanged locked source and
flagset. Recheck the final combined TU, force a fresh object rebuild, and run
the full-ROM build/SHA-1 gate before migrating the lock or claiming promotion.
Inspect any batch plan first: the historical diagnosis/promotion drivers can
mutate source and sync a remote builder. This repair did not run those drivers
or contact the builder. Merge remains with the independent checker.
