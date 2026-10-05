# Heap-chain unlink: four-word NONMATCH

Base: `e0e734babdac3c6a79d2f87f7f895e34aa170148`, freshly fetched 2026-10-05.
Target: `func_800E7A98`, **0x800E7A98–0x800E7B44, 172 bytes / 43 words**.
This is a complete research candidate, **not a match**. Claims and accepted-byte
gain are zero. No production source, lock, protected target, compiler flags,
keep policy, scoring tool, or shared declaration was changed.

## New evidence and bounded result

The old [A29 packet](../../ipa-groups/codex_heap_unlink_a29/STATUS.md) predates
the accepted heap-compactor context. Its source freshly reproduces **31/43**
differing words and an actual **164-byte ELF function**, eight bytes short.

The now-accepted
`src/blob/groups/codex_heap_release_a25/car_damage_visual.c` supplies the genuine
substantive two-return selector `func_800A51D8`, and the rest of that accepted
six-file group supplies the real heap release with its internal argument ABI.
The candidate is appended to an unchanged copy of the compactor translation
unit. All accepted prefixes and all existing keep decisions are preserved;
only the new public entry is added to the research build's member/keep lists.
The verifier constructs this composition in temporary storage instead of
committing duplicate accepted source.

The retained source selects through that existing helper, then reuses the
incoming heap pointer as the live successor traversal value. It reaches
**4/43 differing words at exactly 172 bytes**, with no excess, unresolved
symbols, unverified references or relocation errors. The four offsets are
`0x5C`, `0x60`, `0x74`, `0x78`: the compiler uses **a3** where native uses **a0**
for one successor-pointer web. Every other bit of all 43 instructions agrees.
The verifier proves this limited register-only residual without modifying it.

Two fixed causal controls are reproduced:
- Replacing the accepted selector call with a direct null/default assignment:
  **32/43**, actual 160-byte function
- Giving the successor its own local instead of reusing the incoming pointer:
  **37/43**, actual 168-byte function

A few initial meaningful pointer-selection/traversal spellings also failed to
close the residual. No broad declaration, layout, allocator or line-number
search was run. Workbench diagnosis preceded the final narrowing; its temporary
absolute target fixture had no relocation records, so its relocation warnings
are not matching evidence. The final complete relocation proofs below are
what establish the four-word result. No instrumented allocator trace was run.

Pinned arcade source `historicalsource/rushtherock` at
`845329d7b36f5a384c5625ed9a0aef584ab46139` was checked first, including the
allocator marker and library/utility source. No original arcade donor for this
N64 heap routine is established. The accepted selector is reconstructed source;
its original name and historical stub identity are **not newly proven here**.
This packet neither invents another deleted helper nor propagates padding,
dead reads, extra formals, unsupported volatile, inline assembly or stand-ins.

## Operation and important edge behavior

The routine receives the heap queue lock, selects its explicit argument or the
current default after acquisition, and walks the global heap chain. If any
node's successor is the selected heap, that predecessor is linked to the
selected heap's successor. It then calls the real release with the selected
address in a1 and tag 1 in a2, and jams the lock queue.

Selecting the chain head does **not** replace the global head. Selecting a heap
outside the chain still calls release. Those behaviors are preserved rather
than repaired. A direct-call scan finds no JAL callers inside the protected
main game target population; it does not rule out indirect or overlay calls.

## Verification

Run from the repository root with the pinned IDO and GNU MIPS tools:

```sh
python3 cloud/work/frontier/dot_heap_unlink_20261005/verify.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest -q tests/cloud/test_dot_heap_unlink.py -o addopts=''
```

- Complete ELF STT_FUNC extent and no symbol overlap; all 43 words inspected.
- Independent GNU assembler/linker replay of the exact compiler function slice
  agrees with the project relocator on all 172 bytes and **11 relocations**.
  Original instruction/addend words are preserved. The sole section-relative
  call is mapped from the compiler's exact function offset to its named callee;
  it is not filled from native instructions. Four zero alignment bytes in this
  temporary GNU fixture are excluded. The candidate has no own-data references.
- All **20 existing accepted context bodies** retain strict matching results,
  exact ELF extents, and identical before/after receipts. The context's own
  overlay-loader literals are verified by the unmodified canonical scorer.
- **1,153 cases**, each executed as native code, project-relocated candidate
  code and independently GNU-linked code: **3,459 bounded MIPS executions**.
  Every one of the **43 instruction offsets** executes in each stream.
- Receive hooks change the default and links before selection. Explicit/default
  selection, head/middle/tail/absent selection, empty chains with explicit
  selection, shuffled chains up to 32 nodes, pre-release complete memory,
  queue/release argument order, delay slots, stack initialization/canaries and
  callee-saved restoration are checked. The release hook also clobbers s0/s1,
  matching the internal callee's established non-O32 clobber contract.
- The unchanged candidate body passes the same cases as host C89 with UBSan
  and warnings-as-errors. Four wrong-contract source mutants are rejected:
  wrong tag, missing unlink, wrong successor, and selection before acquisition.
- Seven focused tests pass, including a fresh full replay and a fail-closed
  unsupported-instruction test. The focused/scorer/guard/submission suite passes
  **713 tests**; own-data/group-own-data suites pass **66 tests**. These
  **779 scoped tests** are not the full repository suite or GitHub CI.

The MIPS behavior tests **hook** the three external calls. They prove this
wrapper's bounded unlink/call contract; they do not execute the allocator's
release internals or certify that every synthetic selected heap is a valid
allocation. Its real C body is present and byte-checked in compiler context.
The host uses host-width pointers and an unsigned-long pointer-transport typedef
for the source's `u32` cast; this is explicitly not native struct/ABI proof.
No arbitrary invalid pointers, cyclic chains, concurrency, hardware timing,
whole-program shadow, image, compression, ROM hash or gameplay claim is made.

## Stop point

Keep this source as an honest four-word lead. Reopen for a measured original
parameter/local lifetime or real source/TU declaration that predicts the one
remaining register web. Do not use arbitrary register pressure, dead state,
flag changes or a variant sweep to erase it. Merging and source admission stay
with the independent checker.

Only C, tests, metadata and notes are retained. Native words, raw disassembly,
objects and proprietary data remain temporary or ignored build artifacts.
