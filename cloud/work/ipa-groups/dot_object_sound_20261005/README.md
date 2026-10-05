# Object effect half-turn and positional sound

**Strict matching candidate: 324 bytes / 81 words**, `func_8010DBB8`,
`[0x8010DBB8, 0x8010DCFC)`. Accepted-byte and ROM-coverage gain: **zero**.

The normal IDO O3 group reproduces the entire ELF function, both owned float
literals, and every GNU-linked target word. It is for independent review;
no splice, source-built image, compression, ROM or gameplay gate is claimed.

## Source evidence and selection

The work starts from master `e0e734babdac3c6a79d2f87f7f895e34aa170148`.
Final replay is rebased on `31b2799e`, which integrates PRs 107 and 108.
The target and all selected accepted context sources are unchanged.
Current locks, tracked wave-6 results, earlier B10/B77 attempts and open PRs
107–109 were checked. The parent was told the exact DBB8 range before editing.
This target remains unlocked. The earlier `heads_B10` complete candidate was
14/81 words off; it is reproduced as a fixed control. This is a source-backed
reopening, not a never-attempted function.

New evidence:

1. The accepted `stat_lap_split` reconstruction now provides the actual
   positional-sound callee and its genuine call-group context.
2. [Pinned arcade `game/vecmath.c`, lines 108–110](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/vecmath.c#L108-L110)
   contains the substantive two-input, three-component `dotprod` function.
   Its O3 inlining reproduces the complete native floating-point load,
   multiply, add, result-register and branch shape. Expanding the same
   expression directly in this caller does not.

Donor revision: `845329d7b36f5a384c5625ed9a0aef584ab46139`.
Whole `game/vecmath.c` SHA-256:
`7909033d4e95a86d94dc14b96539b272dbd9e8fd7f7b7e67f6b8cbbc0ca2ee31`.
The donor was verified in the existing pinned checkout; no source fetch or
repository was changed. No direct whole-function arcade donor is established.
The original N64 name or separate address of the inlined helper is unknown.

## Reconstructed contract

The callback receives a valid object record, whose signed definition index
and signed player slot must select valid arrays. It computes the definition
pointer, sets state 7, and attempts to allocate a real 24-byte list node.
The state change still occurs on allocation failure.

On success it initializes the node with the definition's word statistic,
object owner and `0.0333333f` lifetime. It takes the dot product of the selected
player's three-component velocity and the object's third matrix row. A strictly
positive result rotates the object by `3.1415927f` using the accepted yaw
helper. It then clears object flag bits 1 and 2, prepends the node to the active
list, and requests the definition's sound at the object's position with mode 2.

The source retains distinct allocator-result and working-node carriers, both
consumed by the real control/data flow. The archived B10 source also distinguishes
the allocator return from its surviving list-node carrier. This is not proof of
original local names. Collapsing those roles gives exactly the same 324-byte
instruction extent but a 32-byte frame instead of native 40, leaving ten actual
stack/frame mismatches. No unused local, padding array, dead condition, invented
formal, volatile, keeper, stand-in or protected recipe change is used.

The opaque spans are observed record layouts, not artificial local storage:
actor index at 16, matrix at 20, position at 56, state at 90 and player at 92;
player stride 952 with velocity at 20; definition stride 48 with statistic at
12 and sound at 28. The node declaration preserves the accepted allocator's
field names, widths and pointer types, including its otherwise opaque
halfword fields. Sixteen O32 compile-time layout assertions pass.

## Compiler controls and complete-object proof

All final builds use ordinary `-g0 -O3 -mips2 -G 0 -non_shared`, plus the
existing stock scorer's `-Wab,-r4300_mul` behavior. The group retains only the
actual callback; the real substantive `dotprod` helper is inlined normally.
The donor's external declaration is preserved, as in its original source.

| Fixed source control | Differences | ELF callback bytes |
|---|---:|---:|
| Archived B10 best | 14/81 | 324 |
| Typed source with collapsed node carriers | 10/81 | 324 |
| Direct expanded dot expression | 48/81, own references unverified | 328 |
| Final natural donor function group | **0/81, strict MATCH** | **324** |
| Changed lifetime literal | zero instruction differences, owned-data refusal | 324 |

Workbench diagnosis was run on the archival near-match before source changes.
The final object contains an explicitly named eight-byte deleted donor-helper
stub before the callback and four zero section-alignment bytes afterward.
Neither is counted as callback bytes or assigned an original N64 identity.
The GNU proof links the callback symbol itself at 0x8010DBB8 and checks all
324 bytes; it does not claim the unspliced helper stub's image placement.

All 15 callback text relocations resolve, including its three true calls.
The complete eight-byte owned literal extent at 0x801249C4 is checked against
the protected artifact and independently linked with GNU ld. Eight additional
zero bytes are rodata alignment only. No data or BSS allocation occurs.
The wrong-literal control is refused despite zero masked instruction differences.

## Genuine context and behavior

A ten-body O3 regression includes the new callback plus unchanged accepted
sources for its actual allocator, yaw rotation and positional-sound callees,
the sound's matrix transform, and all five accepted sound-list group bodies.
Every complete body remains strict MATCH. Existing sound-group keep decisions
are unchanged. This checks separate accepted type views; it does not establish
a unified shared-type model or the entire game caller closure.

The semantic proof has **4,424 cases**:

- Protected native and independently GNU-linked callback executions agree on
  every memory read/write, call and complete fixture-memory snapshot.
- The actual native allocator and yaw helper execute, rather than replacement
  native bodies. Their unchanged accepted C sources are linked with the
  unchanged candidate in the host UBSan check.
- An independent binary32 oracle agrees on complete callback results and call
  arguments, including allocation failure, flag preservation, every valid
  fixture array endpoint, signed zero, cancellation and strict sign boundaries.
- All 81 callback instructions and all 33 allocator instructions execute.
  Forty-one of the yaw helper's 42 instructions execute; its unused epsilon
  early-exit delay path is outside the fixed positive-pi call domain.
- Trigonometric calls and the final positional-sound API are explicit boundary
  contracts with aggressive caller-save clobbering. Callee-saved registers,
  stack canaries, untouched actor bytes, player/definition arrays and list
  memory are checked.
- Six meaningfully wrong source variants are rejected: inclusive direction
  test, wrong vector axis, state, flag mask, sound mode and lifetime literal.

Tests use finite binary32 arithmetic, eight valid player slots, sixteen valid
fixture definitions and disjoint storage. FCSR flags, signaling-NaN payloads,
arbitrary pointer aliases, invalid indices and gameplay are not certified.

## Reproduce

With the repository-pinned IDO and GNU MIPS environment:

```sh
python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_object_sound_20261005 --claims
python3 cloud/work/ipa-groups/dot_object_sound_20261005/verify.py
python3 -m pytest tests/cloud/test_object_sound_match.py -q -o addopts=''
```

`verify.py --output PATH` saves a fresh metadata-only receipt without modifying
sources. `--no-behavior` runs the compiler/link/context checks only. Native words,
assembly, objects and fixture memory remain temporary or ignored local build
artifacts. Only C, Python, JSON metadata and this research note are proposed.
Merging and final cartridge acceptance remain with the independent checker.

## Local checks

- Eight focused packet tests pass, including fresh compiler and 4,424-case replay.
- The packet plus scorer, owned-data, group, whole-unit, protected-path and
  submission-tool selection passes **816 tests**, with no failures or skips.
- All **402 static locked-function guards** pass.

These are local checks, not hosted CI or cartridge integration.
