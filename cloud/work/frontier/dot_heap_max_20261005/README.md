# Largest free heap block: complete matching source

Base checked: `e0e734babdac3c6a79d2f87f7f895e34aa170148` (2026-10-05).
Target: `func_800E79F8`, **0x800E79F8–0x800E7A98, 160 bytes / 40 words**.
The preceding `func_800E79F0` is an **already accepted 8-byte empty stub**.
Only the 160-byte query is a new matching candidate. Accepted-byte and ROM
coverage gain claimed here: **zero**.

The query takes the heap lock, selects the supplied heap (or the current default
when its argument is null), follows its block list, finds the largest unsigned
size among blocks whose signed `used` byte is zero, unlocks, and returns the
snapshot. It returns zero for an empty list or no free blocks. The historical
address-region/audio labels do not describe these semantics.

## The source evidence that changed the result

The archived A40 direct source reproduced **21/40 differing words** at O3.
The now-accepted `src/blob/groups/frontier_heap_free_total/group.c` supplies
an actual sibling algorithm and a two-return heap selector. Its types agree
with `src/blob/groups/audio_heap/group.c`: 32-byte blocks, `next` at +4,
unsigned `size` at +12, signed `used` at +20, and heap head at +8.

The fixed sequence of experiments was:

1. Reuse the existing meaningful `heap_or_default` selector, with the two actual
   scan locals declared as largest then block. Exact 160-byte body, **2/40**;
   only the loop accumulator and outgoing unlock-argument zero moves swap.
2. Follow free-total's wrapper/worker structure, assigning the immediately
   preceding callerless E79F0 stub to this largest-free-block worker. Define the
   worker after the wrapper, as in the accepted sibling. Both E79F8 and E79F0
   become strict **MATCH** through the ordinary O3 group pipeline.
3. Move that same meaningful worker before the wrapper: **2/40** again.
   This is a reproducible source-order scheduling control, not a padding,
   register-pressure or line-number sweep.

`verify.py` reconstructs these three controls from the submitted source and
unchanged accepted baseline. Workbench diagnosis was run on the two-word lead;
its raw relocation warnings came from the private absolute native fixture.
The authoritative proofs resolve every relocation and compare complete words.

### Source-boundary inference, explicitly limited

The worker performs the entire meaningful maximum scan; it is not a stand-in.
The selector is an unchanged substantive accepted helper. The mirrored adjacent
stub/wrapper topology and sibling source explain the source structure, but do
**not prove the original name or original declaration of E79F0**. No original
arcade heap donor is claimed. This is a source-admission question for the checker.

No padding locals, extra formals, dead reads, empty predicates, unsupported
volatile, inline assembly, register bindings, scoring changes or protected
compiler/keep-list changes are used. The group keep list contains only the actual
query entry. The existing production stub and all accepted sources are untouched.

## Full object and context verification

- All **40 complete E79F8 words**, exact ELF STT_FUNC extent, and all **8
  relocations** pass the canonical strict scorer. E79F0 also matches its exact
  8-byte stub. Independent GNU ld/objcopy places both named functions at their
  native addresses and reproduces every claimed byte.
- The object emits an additional 8-byte compiler-owned selector stub before
  E79F0. It is outside both named claims. No own `.data`, `.rodata`, `.rdata`
  or `.bss` exists; there is no unresolved literal/table obligation. No final
  text alignment bytes are claimed.
- IDO compile-time checks establish the native block size and every accessed
  offset. Host pointers use the host ABI and are not presented as native layout.
- In a four-body free-total unit, its two accepted bodies plus the candidate
  and existing stub are strict matches.
- In the real heap unit, all five accepted bodies plus the candidate and
  existing stub are strict matches. Each accepted source is an unchanged
  byte-for-byte prefix; only the candidate's two unaltered bodies are appended,
  sharing the existing identical selector and compatible declarations.
- That heap unit's previously nonmatching E7D0C context remains **15/49** with
  identical complete 196-byte relocated body before/after. Its scorer's
  nonzero-extra count changes 1→0 because an anonymous compiler helper sits
  outside the exact ELF extent in the baseline. This is disclosed, not counted
  as a new match or a changed source/body result.

A separate-file exploratory combination duplicated anonymous selector stubs;
its scorer counted those as nonzero excess after two otherwise identical named
bodies. The final shared-selector contexts avoid this artifact while preserving
both accepted source prefixes. No scorer or acceptance rule was changed.

## Behavior and adverse controls

The frozen corpus has **4,588 cases**: extreme unsigned sizes (including
0x80000000 and 0xFFFFFFFF), signed statuses -128/-1/0/1/127, empty lists,
permuted 1–48-block chains, explicit heaps and null/default selection. The
receive hook can replace the default and mutate the selected first block before
the scan. The jam hook mutates it after the scan, checking that the returned
snapshot survives release and caller-save clobbers.

- Canonical native code, relocated candidate code, independent GNU-linked code,
  and an independent maximum-of-free-sizes oracle agree. **13,764 bounded MIPS
  executions** exercise all **40 native instruction offsets**.
- The same unchanged C source passes all 4,588 cases with C89, warnings-as-errors
  and UBSan. Full host block/heap/queue snapshots permit only hook mutations.
- Native execution checks queue address/argument order, one receive then one jam,
  stack initialization and canaries, callee-saved registers, stack restoration,
  and every non-stack byte. Unknown instructions/unmapped memory fail closed.
- Four meaningful source mutants are rejected: signed-size comparison (4,077
  cases), include-used-blocks (3,182), first-block-only (1,858), and default
  cached before lock acquisition (300).

The native OS queue implementation/scheduler is modeled by explicit O32 hooks.
This is bounded ordinary-memory behavior, excluding invalid/cyclic lists,
arbitrary pointers, concurrent mutation while holding the lock, and OS failure
or blocking progress. It is not a gameplay or scheduler integration proof.

## Final local validation

On the submitted source tree based on e0e734ba:

- **12 focused tests passed**, including a fresh complete compiler/GNU/native/
  host replay, exact-extent failures, all relocation uncertainty failures and
  source/receipt binding.
- Complete Cloud + Conveyor suite: **2,375 passed, 42 skipped, nine live-node
  tests deselected, zero failures**. Collection independently confirms 2,417
  selected tests. Missing private ROM/image/layout/SDK inputs and MIPS cross-GCC
  account for the skips; these checks are not represented as having run.
- All **402 static-lock guards** pass; protected paths and diff formatting pass.
- The changed-submission gate schedules zero jobs for this frontier directory.
  The group is therefore explicitly scored and compiled by the focused tests;
  the zero-job gate is not used as matching evidence.

The full suite used the exact pinned mips_to_c/decomp-permuter revisions and
available pinned IDO/binutils. These local tool checkouts are not in the commit.
This is local test validation, not an exact-head remote CI or ROM claim.

## Reproduce

Use the repository-pinned IDO and MIPS binutils environment, then from repo root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_heap_max_20261005/group
python3 cloud/work/frontier/dot_heap_max_20261005/verify.py
python3 -m pytest tests/cloud/test_heap_max_contract.py -q -o addopts=''
```

`--record --output PATH` explicitly records a reviewed new receipt. Ordinary
verification never rewrites it. The live protected manifest is always validated;
an unrelated manifest annotation change is tolerated only if every per-body,
source, context, ELF, relocation, compiler and behavioral receipt field is equal.
A changed target-body hash is explicitly rejected by regression tests.

The packet includes source, tests, counts and hashes only. No ROM bytes, raw
assembly dumps, object files, compiler binaries, credentials or private data
are added. No source splice, production context/lock changes, complete shadow
unit, source image, compression, ROM SHA-1, gameplay or merge is claimed.
