# Image-B setup initializer: complete six-word NONMATCH

B:func_803908D0, `[0x803908D0,0x80390B00)`, **560 bytes / 140 words**.
The final full ELF function differs at six words. This is research, with **zero
matching, accepted or cartridge-coverage gain**. It is deliberately absent from
`cloud/matches/`. Master base: `cd22879d40b3de443cfde047b86e75e159b6cec6`.

Image B loads from ROM stream `0xB6FEC4`; its SHA-256 is
`b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd`.
The overlapping image-A address is not interchangeable. A fresh master/open-PR
check found no competing 803908D0 submission before work began.

## Reconstruction and remaining blocker

The function resets one signed halfword, initializes four pools, resolves ten
and seven model names, resolves five and five texture names, resets four
player-effect handles, and invokes the existing two-pool setup wrapper.
There are **nine static call sites and 32 dynamic calls**.

The model lookup's historical name `string_copy_format` is retained; its actual
accepted interface is `s32(char *, s8, s8, s8)`. The texture lookup's accepted
interface is `NameEntry *(char *, s16 *, s8, s8, s32)`. Every lookup independently
reads the unsigned table-count byte, subtracts one, and narrows to signed eight
bits: counts 0, 128, 129 and 255 pass -1, 127, -128 and -2, respectively.
Accepted model and texture sources both use the volatile-qualified global view.
Removing this inherited qualifier produces identical object bytes here; it is
not a new volatile shaping device or a claim of asynchronous updates.

`struct_fields_init` is an accepted ordinary-O32 five-argument routine which
writes a 24-byte pool header and calls accepted `pool_linked_list_init`.
Its old m2c integer spelling of the second argument represents the pool address;
the actual linked-list body establishes that field as a pointer. This packet's
pointer view is an explicit native-ABI contract adaptation; accepted sources
are unchanged. B:8038D1A8 is the existing 88-byte two-pool wrapper.

The full 928-byte B:8038F938 consumer establishes four player-effect records,
52-byte stride, signed handle at +0, a matrix at +4 and position floats at
+40/+44/+48. A natural four-record initialization loop reproduces the repeated
native immediate stores. Four unrelated scalar declarations had incorrectly
encouraged constant reuse. Unknown original names and whole-function arcade
ancestry remain unestablished. Accepted `math_utility` at 0x8008D6B0 copies
nine floats into the consumer's record+4, and accepted `func_80090F44` operates
on that 3x3 matrix; together with the position stores, these establish all 12
float slots as real transform state. The reference arcade checkout is absent; the
current accepted lookup-source comments and prior name-lookup research were
searched for the available interface ancestry.

The first natural source had a 556-byte ELF function and 75/140 differences.
Moving genuine name advancement to the loop increment, separating model and
texture name cursors, pairing their initializations, and using the consumer's
record-array loop yield the complete 560-byte six-word candidate. No unused
pressure variables, fake parameters/helpers, assembly, special compiler flags
or protected-input changes were introduced.

The remaining offsets are +0x15c/+0x160/+0x170 and
+0x1ac/+0x1b0/+0x1c0: the same texture-name load is scheduled earlier than the
fifth outgoing-argument store at two call sites. Registers, helper addresses,
arguments and all other words agree. Workbench diagnosis was run before the
bounded structural refinements and again at this residual. Ordinary O3,
removing the inherited qualifier, an explicit last-table local, and an argument
line break did not change the six-word result. Advancing the texture-name
cursor in the call expression worsened it and was rejected. No exhaustive
search or proven compiler impossibility is claimed. Freeze this route until a
genuine source/context or scheduling explanation appears.

## Verification

`verification.json` binds the final source, controls, proof, host test, protected
inputs, helper bodies/entry addresses, consumer, compiler binaries and flags.

- Full ELF function and complete text: 560 bytes. No owned data, literal,
  jump table, alignment padding, unresolved relocation, mask or excess word.
- All 61 relocations and 23 external bindings are enumerated. GNU links the
  entire unmodified object at its native entry and agrees exactly with the
  canonical project relocator. Both preserve the six native differences.
- Four native 32-bit layout assertions cover pointer width and record layout.
- 1,024 fixtures, each through protected native, project-relocated and GNU-linked
  instructions: 3,072 executions, all 140 instruction offsets, all eight outcomes
  of the four loop branches, 27 independent count loads and 32 calls each.
- Every helper entry checks the complete mapped-state snapshot and exact ordered
  arguments. Entire final mapped storage, gaps/canaries, caller-save poisoning,
  preserved registers and stack bounds are checked.
- The unchanged C89 source passes 1,024 UBSan/bounds host cases. All count bytes
  and independent count/name mutation modes are covered. Five compiled
  wrong-contract mutants fail; unknown native instructions fail closed.

These are **abstract helper-boundary proofs**. Pool hooks intentionally fill
backed storage with synthetic patterns to test that the caller preserves it;
they do not execute real pool construction. Lookup hooks model result-domain
and call/observation behavior, with adversarial count/name changes to exercise
legal compiler boundary effects, rather than real string searches. Actual
helper implementations are source/lock/native-body bound, not executed by the
producer. Valid backed arrays and nonaliasing storage are required. Full table
contents, pointer-invalid inputs, aliases, concurrency, hardware, whole-game
behavior, image/compression/ROM gates and hosted CI are outside this evidence.

## Reproduction

With IDO 5.3, GNU MIPS binutils and GCC available:

```
export IDO_DIR=/path/to/ido
python3 cloud/work/runtime_b_initializer_20261006/verify.py --check
python3 -m unittest discover -s cloud/work/runtime_b_initializer_20261006 -p 'test_*.py' -v
```

`RUSH_REPO` may name an exact base checkout for protected inputs and canonical
scorer imports when the packet is in a sparse source-only worktree. `TMPDIR`
may point at an available writable directory. No production, protected target,
lock or scorer file is modified. Tool hashes may legitimately differ on another
installation; the frozen replay intentionally reports such provenance drift.

Source-only draft research handoff. Independent review precedes publication;
merging and any integration remain with the independent checker. No CI watcher.
