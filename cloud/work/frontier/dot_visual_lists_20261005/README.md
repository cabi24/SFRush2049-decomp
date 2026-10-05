# Four-list traversal: complete natural-C match

Base: `cc4d5fdd0bbc42dbf6be49f00d9454af8cb9c4f5`.

`visual_objects_update`, **[0x800B55FC, 0x800B5688)**, **140 bytes / 35 words**,
now passes the unchanged canonical scorer under ordinary IDO 5.3
`-g0 -O3 -mips2 -G 0 -non_shared`; O2 is also a strict match.

The source is `cloud/matches/visual_objects_update.c`. It walks exactly four
16-byte indirect lists. For each node, it tests the word at body+72 and calls
the actual existing `MaxPathZeroControls(node, action)` if that word is nonzero.
After the call it reloads the node's body pointer and that body's next link.
The callback may change either; prefetching the next link changes behavior.
Later list heads are loaded when their turn is reached. The operation's return
value is ignored. Its concrete subsystem semantics are not claimed.

This is an N64-specific reconstruction. A search of pinned Rush The Rock
`845329d7b36f5a384c5625ed9a0aef584ab46139`, including `LIB/blit.c`, `LIB/font.c`,
`LIB/anim.c`, `game/hud.c` and `game/maxpath.c`, found no direct donor. Arcade
`MaxPathZeroControls(MODELDAT *)` has a different interface and is not the donor
for the historical N64 label. The List fields are corroborated by the already
accepted indirect-list primitives `func_80091FBC` and `func_8009211C`.
`VisualNode` and `VisualBody` are observed prefixes; unobserved fields are not
invented as gameplay state. The record gap locates the observed word at +72;
it is not local-stack padding.

## What actually closes the match

The previous raw-offset reconstruction is preserved unchanged at
`cloud/work/tiny_A42/visual_objects_update.c`. Its O2-only packet reported
22/35 differing words. A fresh O3 control reproduces that result.

A bounded 2-by-2 comparison isolates the counted loop:

| Source representation | Pointer-to-end outer loop | Counted four-element loop |
|---|---:|---:|
| Previous raw-offset fields | 22/35 differ | strict MATCH |
| Typed fields | 22/35 differ | strict MATCH |

Every control has the complete **140-byte ELF function size**. No shorter
prefix is compared. The counted loop fixes the outer iteration/end-pointer
allocation; typed fields make the accesses explicit but are not the cause of
matching. The control sources are derived deterministically by `verify.py`
from the two bound source files, rather than preserved as many redundant files.

No dummy callers, extra parameters, padding locals, dead reads/conditions,
register bindings, unsupported volatile qualifiers, compiler modifications or
protected recipe changes are used. This was one semantic counted-loop change,
with the four fixed causal controls, not an allocation or source-layout sweep.

## Verification

`verification.json` binds the candidate, previous source, verifier, executor and
host harness to their SHA-256 values and records the compiler and protected
native fingerprints.

- Full ELF STT_FUNC size: 140 bytes. All 35 words are strict-equal after relocation.
- Independent GNU linking resolves all **five text relocations**, including the
  actual operation call. Its complete body equals the protected target.
- Four trailing zero section-alignment bytes are verified and excluded.
- No owned `.rodata`, `.data` or `.bss` bytes occur.
- Separate IDO compile-time assertions check O32 List size 16, head+8,
  node body+0, body next+0 and body predicate+72. The function stays exact.
- Genuine O3 compilation with unchanged accepted list insert/remove source
  retains all three exact symbols: the new target (140 bytes), `func_80091FBC`
  (352 bytes) and `func_8009211C` (348 bytes).
- **8,064 cases** compare native execution, GNU-linked execution, a Python
  logical oracle and unchanged C compiled with host ASan+UBSan. These include
  empty/nonempty lists, zero/high-bit/all-one predicates, signed action boundary
  bit patterns, replacement next links, replacement body pointers, terminating
  links, future-node predicate changes and later-list head changes.
- All **35 instruction offsets** execute. The native model clobbers ordinary
  caller-save registers and outgoing O32 argument homes at each call, checks
  callee-save/stack restoration, rejects unmapped or unaligned memory, and
  compares all external memory (including untouched sentinel fields).
- The call snapshots include node, action, pre-call body/next/predicate and all
  four current list heads. Callback writes are modeled explicitly; the target
  itself is checked to make no external stores.
- Four source mutants are rejected: omit the fourth list (3,828 cases), force
  action zero (6,265), invert the predicate (8,022), and prefetch the next link
  before the operation (2,334).
- Eight focused regression tests pass, including source-binding failure and
  live native mutation checks.

The C host uses actual host pointers and normalized node identities; it does
not pretend that 64-bit host struct offsets are O32. O32 layout is established
separately by the IDO assertions and complete native/linked proof. ASan leak
checking is disabled because the execution environment's tracing is incompatible
with LeakSanitizer; ASan memory access checks and UBSan remain enabled.

## Reproduce

From the repository root with the pinned IDO and MIPS GNU binutils environment:

```sh
python3 tools/cloud/score.py fn cloud/matches/visual_objects_update.c visual_objects_update --flags '-g0 -O3 -mips2 -G 0 -non_shared'
python3 cloud/work/frontier/dot_visual_lists_20261005/verify.py
python3 cloud/work/frontier/dot_visual_lists_20261005/verify.py --compiler
python3 -m pytest tests/conveyor/test_dot_visual_lists.py -q -o addopts=''
```

Ordinary verification never updates the receipt. `--record` is an explicit
reviewed evidence-generation option. Current protected manifests are always
validated; an unrelated manifest comment change is not treated as changing
these 35 target words.


## Portable ELF receipt and final local checks

The original full-object SHA-256 is retained as provenance. A fresh compiler
replay in a different worktree initially failed only that hash: IDO embeds the
absolute source filename in nonallocated `.mdebug`. A controlled experiment
compiles identical source from two different-length paths. Their full-object
and `.mdebug` hashes differ, while every other section is byte-identical.
Physical section/file offsets also move when the debug pathname changes length.

The portable receipt checks all ABI/header facts and section attributes, and
hashes every complete non-debug section, including `.text`, `.rel.text`,
`.symtab`, `.strtab`, `.shstrtab`, `.options` and `.reginfo`. It excludes only
physical file offsets and the nonallocated ECOFF `.mdebug` payload/size.
The latter must retain the expected nonallocated type and flags and is hashed
separately for provenance. Exact path presence is checked in each controlled
build. No code, symbol, relocation, ABI flag or allocated data byte is masked.
Tampering with text, relocations, symbols, register metadata, options or ABI
flags is rejected. Format-only tests additionally reject an allocated-debug
section. All existing full-body, GNU-link and behavior checks remain in force.

The initial frozen commit `7e36ee7e` on `cc4d5fdd` passed the full local
Cloud/Conveyor suite: **2,227 passed, 60 skipped, nine deselected, zero failures**.
The first attempt stopped at collection because the new worktree's pinned
submodule directories were empty. Exporting the exact existing Git commits
`ec2efeebb33e2b1de81e39fbf7cbe3cd97350472` (decomp-permuter) and
`7fda1c6821148c65cb6aa52d3a356048554ca1cf` (mips_to_c) into this worktree cleared
that local setup issue; no tracked setup or CI repair is included.

The branch was then rebased onto wave-6 master
`264381e98f9f53c41cff41fb0518d85c2b35cfbd`. The candidate, target region and both
accepted context sources remain unchanged; fresh master's manifest verifies
that exact target region. The target is still absent from master's accepted
lock and published matching sources. All eight focused tests pass after the
portable receipt fix. The unchanged candidate still passes strict O2/O3,
GNU-linked and three-body context proof plus the full 8,064-case behavior replay.
Protected-path/changed-submission checks and all 402 static locks pass.
Exact-head remote CI has not been run or monitored by this worker.

## Limits and integration

This is a new matching **candidate**, not accepted source or ROM coverage.
Accepted-byte gain: **zero**. The context is a three-body list-primitive
regression, not a real caller/callee closure or full shared-type model.
The actual `MaxPathZeroControls` body and the calling `func_800B5688` remain
unmatched. Behavioral callbacks test the traversal contract; they are not
compiled as stand-in context for the matching claim. The bounded acyclic
fixtures do not prove termination or memory safety for corrupted/cyclic lists,
concurrent mutation, arbitrary pointers or the real operation's gameplay.

No splice, complete shadow unit, linked-game-image, compressed-stream identity,
ROM hash or gameplay gate was run. Merging and final integration remain with
the independent checker. No ROM bytes, raw instruction dumps, objects,
credentials or unrelated private data belong to this packet.
