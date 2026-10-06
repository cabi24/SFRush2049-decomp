# Runtime image B player-state reset

**Strict MATCH: B:func_8038CA24, 0x8038CA24–0x8038CB10, 236 bytes / 59 words.**
This is a matching candidate, with **zero accepted-byte or ROM-coverage gain**.
No runtime-image splice, image/recompression gate, ROM gate or gameplay test was run.
Merging and production integration remain with the independent checker.

The first natural typed reconstruction matches ordinary IDO 5.3
`-g0 -O2 -mips2 -G 0 -non_shared`, plus the existing mandatory
`-Wab,-r4300_mul`. No source-variant search or compiler-setting change was needed.

## Identity and source admission

Base: `cd22879d40b3de443cfde047b86e75e159b6cec6`. This is **runtime image B**
(ROM stream `0xB6FEC4`, image SHA-256
`b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd`).
Image A shares its load address but has different code at this address.
Protected target manifests, complete extent metadata and symbol addresses are
checked by the replay. Current matches/research/claims and open PRs were checked
before this exact target was reserved. No shared context or accepted body changes.

[Complete C](../../matches/ovl_b/func_8038CA24.c) resets fields in the selected
0x3B8-byte player record, a separate byte status, the selected 0x148-byte group
record, and the selected 0x10C-byte five-handle record. Values are 0, 8, 9 and -1;
both float fields receive positive zero. Other bytes remain untouched.

There is no known arcade donor for this N64-specific service. Field names are
conservative descriptive hypotheses, not recovered original declarations.
Widths, offsets and table strides come directly from this body and its adjacent
consumers. Unknown struct regions describe observed persistent object layout,
not artificial stack padding. No fake formals, helpers, locals, volatile shaping,
assembly or protected compiler/scorer/context/lock edits are present.

The native ABI is `void func_8038CA24(s16 player)`. It consumes a0's low signed
halfword and homes the original a0 word to the caller's argument-save area.
Three current game-image direct call sites pass signed halfwords and execute only
in mode 6:

- `func_800C54F0`, site `0x800C554C`, from its incoming signed slot
- `func_800D348C`, site `0x800D34BC`, from the car/model player field at +0x7C6
- `display_list_flush`, site `0x800FBA8C`, from its sign-extended loop index

This does not prove a universal caller range or original source return declaration.
No caller consumes a return value before replacing v0. This leaf has no callees,
owned literals, tables or storage. The fixed globals are external array views.

## Verification

`verification.json` binds portable results to the exact source, native inputs,
verifier and host harness by SHA-256. Tool executable hashes are emitted separately
when requested, so host/compiler installations do not pollute portable equality.

- Full ELF function size 236; object `.text` is 240 bytes with four zero alignment
  bytes outside the function. No omitted instruction or nonzero excess word.
- Exactly eight HI16/LO16 relocations at pinned offsets to four named globals.
  All are resolved by both the project scorer and independent GNU ld. GNU ELF
  `.text` and function addresses are required to be `0x8038CA24`.
- All nonempty allocated object sections are checked; only `.text` and ABI
  `.reginfo` are permitted. Owned data/storage size is zero.
- **131,072** native address-characterization cases span every signed-low-halfword
  index and two upper-halfword patterns. The full write map, argument home, stack,
  saved general registers and saved floating registers are checked.
- **1,024** additional three-way cases execute protected native, project-relocated
  and GNU-linked words (**3,072 executions**), covering all 59 instruction offsets.
- **16,384** unchanged C89+UBSan host cases cover fixture players 0..3 with arbitrary
  prior object bytes. Complete arrays, adjacent elements and untouched bytes are
  compared with an independent byte-offset oracle.
- Five compiled source mutants are rejected by host behavior and strict native
  scoring: changed mode, missing selection reset, omitted last slot, nonzero
  group timer and clearing an extra byte. Unknown interpreter opcodes are rejected.

The C-valid behavioral domain is four accessible nonoverlapping elements in each
named table, indices 0..3, and arbitrary prior object representations. This is a
fixture domain, not proof of complete caller bounds. Outside it, exhaustive native
index testing characterizes machine address arithmetic only. Negative or oversized
C indexing, invalid storage, callback timing/concurrency and complete caller/game
execution are not claimed. Finite testing is not a universal equivalence proof.

## Reproduction

From the repository root with IDO 5.3, MIPS GNU binutils, GCC and pytest available:

```sh
python3 tools/cloud/score.py fn cloud/matches/ovl_b/func_8038CA24.c func_8038CA24 --targets asm/us/ovl_b
python3 cloud/work/runtime_b_reset_20261006/verify.py /tmp/runtime-b-reset-replay.json --tool-provenance /tmp/runtime-b-reset-tools.json
cmp /tmp/runtime-b-reset-replay.json cloud/work/runtime_b_reset_20261006/verification.json
python3 -m pytest -q tests/conveyor/test_runtime_b_reset_match.py
```

Raw native instructions, generated objects and linked binaries remain temporary
local inputs/outputs and are not published. The packet contains source, tests,
portable proof metadata and research notes only. No CI watcher is installed.

## GNU linker portability and failure isolation

The original [PR #142 verification run](https://github.com/cabi24/SFRush2049-decomp/actions/runs/37397230526)
failed the independent linked-address assertion. Reproduction with Ubuntu's
`binutils-mips-linux-gnu 2.42-2ubuntu1cross5` confirms that the original implicit
location-counter script placed this object's `.text` and function at
`0x8038CA30`, twelve bytes above the required `0x8038CA24`. The input section has
16-byte alignment; `SUBALIGN(4)` did not prevent this output-section rounding.
Debian GNU ld 2.44 preserved the intended address with the original script.

The output `.text` section now has an explicit `0x8038CA24` address. Both versions
place the complete 236-byte function correctly. The native address assertion is
retained, with expected/actual diagnostics; the symbol address/extent, exact
relocations, zero-only alignment tail and complete native-word checks remain.
The fresh GNU 2.42 receipt differs from the original only in the verifier's own
SHA-256 binding. Source, host harness, flags, native inputs, linked-body hash and
all behavior results are unchanged.

The original assertion failure also left the shared scorer targeting image B,
causing the later game-image `func_8010C02C` lookup to raise `KeyError`. `prove()`
now restores the caller's exact prior target directory in `finally`, including
early overlay failures, late game-census failures and successful completion.
Regression tests cover each path from an arbitrary prior directory and inject
a twelve-byte linked-address shift that must still fail closed. They also verify
the game-image seed target is available after a failed link check.

All seven reset tests and five unchanged seed-vector tests pass together with
Ubuntu GNU 2.42. With Debian GNU 2.44, those tests plus the selected scorer,
protected-path guard and submission tests pass: **80 passed, 648 unrelated
locked-source cases deselected** in the sparse checkout. No full-suite or new CI
result is claimed, and no CI rerun or watcher was requested.
