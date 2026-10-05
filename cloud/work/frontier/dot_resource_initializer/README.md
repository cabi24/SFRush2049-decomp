# Resource initializer: fresh callee-aware research

**NONMATCH. No matching claim, production-source edit, or accepted-byte gain.**
Target `func_8010D85C`, `[0x8010D85C, 0x8010D9CC)`, 368 bytes. This packet
improves the complete O3 baseline from **62/92 to 12/92 differing words**.
All remaining differences are stack-relative immediate operands: opcodes,
registers, branch layout and resolved calls/data addresses are otherwise exact.

## Preflight and scope

- Remote master was checked at `cf10b3392d7f00ae42d75c008b79fdc2541aab6b`
  on 2026-10-05. The target was unlocked, absent from the earlier Claude wave-2
  assignments, and newly callee-ready following acceptance of `sound_bank_load`.
- Work is isolated to this research directory and its dedicated regression test.
  Accepted source, symbols, target inputs, locks, flags and scoring machinery
  are unchanged. No remote builder was used.
- The starting source is `cloud/work/dot_resource_init/candidate.c`, SHA-256
  `d28af79147333fec2422797822df56da00944052b05716e5666d203477aa00a7`.
  Its behavioral audit and neutral, address-anchored naming remain relevant.
- No genuine arcade donor is established. The old `ai_obstacle_avoid` label
  and invented implementation in `src/game/game.c` are not source evidence.

## What improved

1. The now-accepted callee establishes that the name table and returned value
   carry pointers, and that the output object is an unsigned halfword. Those
   types replace the historical integer carriers without changing instructions.
2. Its `volatile u8 D_80140BDC` declaration also explains this caller's native
   address-form byte read. This alone improves 62/92 to 29/92 and restores the
   exact 368-byte function extent.
3. Reusing the existing `kind` local for the slot index *after* the directory
   lookup gives 12/92. The index still observes any callback mutation. This
   reproduces the native register allocation without adding a local or altering
   an argument.

The callback sequence, signed-byte suffix conversion, guards, post-lookup
slot/text/flag rereads, callback installation and zero-time store are preserved.
Names such as resource, handle and state remain descriptive hypotheses.

## Two unresolved source questions

**Frame geometry.** Native code uses an 88-byte frame; this candidate uses 64.
The 12 differing operands are frame entry/exit, incoming argument spill slots,
the text spill and the halfword-output address. Saved-register bytes agree.
The candidate has exactly 92 instructions and no text-alignment tail, so this
is no longer an instruction-count or trailing-padding mismatch. Workbench
`diagnose` was run before changing source; its initial structural warning was
superseded by the final full relocated comparison. Four declaration-order
controls only move real local homes and all remain 12/92. No unsupported local,
array, padding, keeper, assembly, parameter or flag change was introduced.

**Callee arity.** Retail explicitly supplies a fifth stack value `1` to
`sound_bank_load`; the accepted callee source declares four parameters and does
not consume that value. The retained five-argument extern documents the observed
caller boundary. It is incompatible with the accepted four-parameter definition
and must not be treated as a settled full-program C contract. This packet does
not add an unused formal to the callee or combine incompatible declarations.
A separate control uses precisely four arguments plus the unchanged accepted
callee in a real two-file O3 group: the callee remains strict MATCH, while the
caller is 59/92 with a 360-byte ELF extent. Merely making the callee visible does
not reproduce the fifth store or solve the frame.

Stop here until authentic declaration/TU or inlined-source evidence explains
the missing 24 frame bytes and resolves the fifth-argument boundary. The new
12-word plateau is not permission for a pressure or padding search.

## Bounded controls

`verify.py` deterministically reproduces all controls from preserved sources.
The `verification.json` receipt includes per-source and object hashes.

| Control | Differing words | ELF function bytes |
|---|---:|---:|
| Prior source, O3 | 62/92 | 364 |
| Volatile count | 29/92 | 368 |
| Pointer / unsigned-halfword contracts | 29/92 | 368 |
| Reused post-call slot index, final | 12/92 | 368 |
| Kind reused for suffix | 25/92 | 368 |
| Both index and suffix reuse | 12/92 | 368 |
| Search pointer reuse | 29/92 | 368 |
| Four declaration-order controls | 12/92 each | 368 each |
| Four-argument caller alone | 59/92 | 360 |
| Four-argument caller plus genuine callee | 59/92 | 360 |
| Final source, O2 control | 73/92 | 356 |

The principal set has 12 source controls (including the baseline); the separate
read-only callee group and final O2 check complete this bounded investigation.

## Verification

- Final IDO flags: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
- ELF symbol size **368**, complete text size **368**, alignment bytes **0**.
- All relocations over the complete text resolve without masks, unresolved
  symbols, unverified sections or diagnostics. An independent GNU link agrees
  byte-for-byte with scorer relocation. All 92 full words are compared.
- 13,840 deterministic native/candidate/host differential cases pass.
  They include guard combinations, high argument bits, signed-byte boundaries,
  call order, arbitrary 32-bit returned tokens and mutation of slot/text/flags.
- The host fixture uses actual object pointers for names and the returned
  record, mapping identity back to the native result token for comparison.
  It does not manufacture test pointers from integers.
- 100,000 host cases pass ASan/UBSan, plus the focused UBSan regression.
  Leak detection is disabled for this execution environment only.
- O32 node/state/table sizes compile-check at 24/112/68 bytes.
- Four focused packet tests pass, along with the four existing initializer
  tests. No unrelated CI was used as evidence.

Reproduce from the repository root with the pinned `IDO_DIR`, GNU MIPS tools
and Python dependencies configured:

    python3 cloud/work/frontier/dot_resource_initializer/verify.py --output /tmp/resource-proof.json
    python3 -m pytest tests/conveyor/test_frontier_resource_initializer.py tests/conveyor/test_resource_init_research.py -o addopts='' -q

The interpreter fails closed and checks mapped/aligned accesses, stack and
callee-saved restoration, call arguments and changed bytes. It models callees;
it does not execute the whole game. Fixtures use eight nonnegative slots.
Original table bounds, negative-slot validity and callback signatures remain
unverified. No ROM/image/compression gates or runtime-game claim are made.
No raw native instructions, object files or ROM data are published here.
