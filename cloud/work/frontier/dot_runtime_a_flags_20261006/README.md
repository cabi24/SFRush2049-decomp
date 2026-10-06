# Image A availability-row initializer

`A:func_80393004`, `[0x80393004, 0x803930B8)`: **180 bytes / 45 words MATCH**.
This is a source submission, not accepted cartridge coverage. No image,
compression, ROM or gameplay gate ran.

## Identity and scope

The input is runtime **image A**, ROM stream `0xB5C534`, loaded at
`0x8038A400`, decompressed length 194,128 bytes, image SHA-256
`0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667`.
The target extent is manifest-verified and has an internal-JAL start witness.
The same address in image B lies inside a different function,
`B:func_80392FE4`; image B is not a target or context for this submission.

Current master preflight was `cd22879d40b3de443cfde047b86e75e159b6cec6`.
No image-A matching source already existed for this function. The repository
handoff, archived source, matching paths and open PRs #110–135 were checked;
the parent confirmed there was no other active runtime-image owner. No
production source, protected target, compiler, scorer, lock, context or
compiler-flag policy changed.

Three direct image-A callers were found at `0x803931F4`, `0x8039358C` and
`0x803940B8`. The leaf reads no arguments and preserves all O32 callee-saved
registers, stack pointer and return address. Return-value semantics are not
claimed. Computed callers and original translation-unit membership are unknown.

## Source and count contract

The function initializes two availability rows at `0x803BA7E0` and
`0x803BA7F0`. It sets their first two entries, gives the third entries different
rules, then writes eight count-dependent flags. It reads a signed-byte selector
at `0x803B65A4` and one signed word from the table at `0x803BA830`.

The adjacent, independently target-bound producer `A:803930B8` is 264 bytes.
Read-only native inspection shows four slots populated with `-4`, `-3`, `-2`,
`-1`, or a zero-based linked-list count. The nonnegative count interpretation
assumes a valid terminating traversal without integer overflow. This packet
does not execute the producer or prove an exhaustive census of every possible
writer. The source contract requires a valid initialized selector in `0..3`
and stable, disjoint ordinary storage.

The archived source has a signed induction variable. IDO transforms its loop
into pointer-relative comparisons and emits six excess nonzero words: freshly
reproduced **45/45 differences, six excess words**. An unsigned bounded index
prevents that rewrite. Retaining the signed value comparison with `(s32)i`
then produces the exact native integer comparisons and four-way unroll.
The index is always `0..7`, so the cast is defined. The source is ordinary C89
and contains no assembly, volatile qualifier, padding, dummy parameters,
stand-ins or unused pressure variables. This is a matching source shape, not
proof of the original declaration. Omitting the cast gives **24/45 differences**
and wrong behavior for the genuine negative sentinels.

### Important integer-limit exception

The native compiler output hoists `count-1`, `count-2`, and `count-3` into
wrapping MIPS additions. At `INT_MIN`, `INT_MIN+1` and `INT_MIN+2`, it therefore
differs from the unchanged host C's signed comparisons:

- `INT_MIN`: both rows incorrectly set indices 4, 5, 6, 8, 9 and 10 to one
- `INT_MIN+1`: indices 5, 6, 9 and 10
- `INT_MIN+2`: indices 6 and 10

These three values are outside the observed producer contract. They are
explicitly executed and retained as discrepancies, never counted as successful
host/native equivalence. The compiled candidate and native target agree even
there. The matching source contains no signed arithmetic overflow itself.

## Verification

`verify.py` binds its receipt to the source, native interpreter, host harness,
protected target/metadata/manifest, production scorer and IDO executables.
It checks the complete ELF function symbol: **180 bytes**, with the remaining
**12 zero alignment bytes** outside the function. All **12 HI/LO relocations**
are resolved, with no masks, unresolved symbols, owned data or literals.

An independent ELF parser checks `st_size` rather than using target-length
truncation. GNU ld links the entire unmodified object with `SUBALIGN(4)` and
asserts exact function and section placement at `0x80393004`, then compares
all 180 bytes. The full `.text` remains 192 bytes including its zero tail.
The ordinary production reader independently produces the same 192 bytes.

The proof executes **8,952 cases**, each through protected native instructions,
GNU-linked instructions, the unchanged C89 source with UBSan, and a separately
written mathematical oracle. There are 1,119 distinct directed/seeded counts,
four slots and two memory/register seeds. Tested counts span
`INT_MIN+3..INT_MAX`, including every value `-16..64` and the genuine negative
sentinels. Both native copies execute all 45 instruction offsets and both
outcomes of the loop branch. Exact reads, 22 writes, output/canary bytes,
table/selector preservation and callee-saved registers are checked. Three
separate underflow cases add six native executions and three host executions.
Native execution here means a fail-closed instruction interpreter, not N64
hardware or a whole-game emulator.

Five compiled semantic mutants are rejected. Seven malformed-native controls
reject incomplete/extra extent, unknown opcodes, bad byte/count reads, redirected
stores and a bad return register. Two malformed-ELF controls reject function
address and size drift. Finite tests do not prove unrestricted input behavior,
concurrent mutation, invalid pointers or hardware exception behavior.

## Reproduction

From any current directory, with `IDO_DIR` configured and MIPS GNU binutils in
`PATH`:

```sh
python3 /path/to/repo/cloud/work/frontier/dot_runtime_a_flags_20261006/verify.py --output /tmp/runtime-a-verification.json
```

From the repository root:

```sh
python3 tools/cloud/score.py fn cloud/matches/ovl_a/func_80393004.c func_80393004 --targets asm/us/ovl_a
python3 -m pytest tests/cloud/test_runtime_a_flags.py
```

Native words, raw listings, objects and shared libraries stay under ignored
`build/`. The published packet contains source, hashes, verification code and
non-byte evidence only. Independent review and runtime-image integration remain
separate gates; merging belongs to the independent checker.
