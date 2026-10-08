# Image-A projected gauge callback: complete standalone MATCH

`A:func_803A3DA4`, `[0x803A3DA4,0x803A4134)`, **912 bytes / 228 words**.
The strict matching source is `cloud/matches/ovl_a/func_803A3DA4.c`.
Its first line is the required bare `-g0 -O3 -mips2 -G 0 -non_shared` header.
The unchanged canonical scorer adds mandatory backend flag `-Wab,-r4300_mul`.
No caller, inlined helper, deleted-static stub, own data or group context is needed.
This packet does not modify locks, splice code, or claim a ROM coverage change.

## Evidence-driven resolution

This extends, rather than overwrites, the frozen NONMATCH research packet
`cloud/work/frontier/dot_runtime_a_projected_gauge_20261006`.
Base commit: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`.

The original reconstruction matched every instruction, register and non-stack
operand but reserved a 72-byte frame instead of 64 bytes. Native code passes
one addressed two-halfword projection output to an ordinary five-argument
helper. It preserves a derived slot address across visibility and projection
calls. Native code does not establish a separate declared pointer cache or
quarter-height cache. The family callback A3A6C uses the same projection
contract; its independently replayed exact reconstruction has a 72-byte frame,
so its declarations cannot establish this callback's declaration list.

A diagnostic debug build identified the original declared pointer home directly
above the output buffer, while the actual derived address spill occupied a
separate compiler home. This motivated replacing the named pointer with direct
indexed array expressions. That bounded change gives the native 64-byte frame
and reduces the residual to four loads/stores of the derived address at
`sp+36` instead of `sp+40`.

The original inline-quarter control independently shows that eliminating the
quarter-height cache moves that compiler home by four bytes without changing
any instruction or register. Combining the two source simplifications gives
complete equality. No declaration permutation, padding, dummy argument, unused
read, volatile, artificial pressure, fabricated helper or compiler-flag change
is used. The ordinary optimizer preserves both common subexpressions.

Bounded controls retained here:

- Named slot and height: 23/228 differing words, 72-byte frame
- Direct slot indexing only: 4/228 differing words, 64-byte frame
- Inline quarter-height only: 27/228 differing words, 72-byte frame
- Final direct indexing plus inline quarter-height: 0/228, 64-byte frame

These are typed reconstructions, not recovered original source spellings.
Read-only family context can be inspected at the base commit:

    git show 6b2e9e506fe3d2267a710e41c85af5364ccd00c7:cloud/matches/ovl_a/func_803A3A6C.c
    git show 6b2e9e506fe3d2267a710e41c85af5364ccd00c7:cloud/matches/ovl_a/func_803A4134.c
    git show 6b2e9e506fe3d2267a710e41c85af5364ccd00c7:cloud/work/near_miss_B69/brake_light_update_native.c

## Complete verification

- Complete ELF `STT_FUNC` and `.text` are both 912 bytes, with no omitted tail
  or alignment bytes. No allocated own data/storage is emitted.
- All 31 relocations and 15 external anchors are checked. The canonical scorer
  reports zero differences, extra words, unresolved symbols, unverified
  relocations or errors. Independent GNU placement of the unmodified whole
  object equals the native target words byte-for-byte.
- Fourteen O32 layout/stride assertions pass. The unchanged final source runs
  under C89 UBSan, bounds and float-cast-overflow instrumentation.
- All 68,306 C-oracle cases pass, including every signed-halfword height,
  geometry boundaries, marker/player/segment paths, signed player flags and
  side-effecting helper hooks. Three wrong-source mutants are rejected.
- Focused tests cover foreign-working-directory replay, first/interior/last
  native-word and extent mutations, final-source/packet binding, and early
  missing-IDO/missing-linker skips.

Run from the repository root:

    python3 cloud/work/frontier/dot_runtime_a_projected_gauge_match_20261006/verify.py --check
    python3 -m pytest tests/cloud/test_runtime_a_projected_gauge_match.py -q

The receipt binds only this packet's owned source, verifier, support, controls,
base commit and native target words. Tests, mutable production context,
manifest/symbol-map serialization and live lock state are not pinned.
The original NONMATCH packet remains historical evidence. No raw native words,
ROM bytes or assembly dumps are distributed here.

## Limits

The host oracle is **not native-emulator evidence**. Actual renderer/projection
code is not executed; helpers are bounded side-effecting hooks and external
float constants are synthetic model values. Strict compiled-word equality is
verified separately. Original source spelling, translation-unit identity,
unrestricted aliases, invalid pointers, concurrency, hardware, gameplay,
image construction, compression and ROM identity remain unproved. Float
fixtures are finite and signed16-representable; integer narrowing uses the
checked host/target low-halfword convention rather than universal ISO C.
