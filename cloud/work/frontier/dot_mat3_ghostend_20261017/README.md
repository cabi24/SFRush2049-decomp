# GhostEnd: native type and source-boundary research

**Research only. Claims are empty and new accepted bytes are zero.**

`ghost_race_setup` at `0x800D510C` is the native GhostEnd finalizer. Its exact
native extent is 440 bytes / 110 words, frame64. This packet preserves a complete
later-donor transfer and a historical native reconstruction in the same current
canonical context. Neither is a match.

| Source form | Strict words differing | ELF bytes | Extra nonzero words |
| --- | --- | --- | --- |
| Full donor, genuine native free boundary | 54/110 | 448 | 2 |
| Same donor, expanded free operations | 54/110 | 444 | 1 |
| Typed historical native reconstruction | 16/110 | 440 | 0 |

All33 established context bodies remain strict exact in every build, including
the complete24-member canonical save group, wheel_params_set, its three real
free/lock/unlock stubs, and five other locked context functions. CreatePlayer
remains246/272 and menu_back remains3/33, both unclaimed. The group manifest
retains the complete original member and context lists; there are no hidden
failed members. This does not establish whole-program integration.

## Source evidence

The later original GhostEnd is `8001A1E0`,260bytes,void(void), ghost.c. Original
DWARF DIE5AC5C records exactly the header pointer g followed by int i, with no
parameters or nested scope. The available reconstruction additionally has a
meaningful block-local ghost handle, absent from the original DWARF. That local
is disclosed, not treated as authenticated historical N64 source.

The original direct calls are PurgeGhostData at+0x84 and DeallocateMemory at+0xB8.
The donor translation unit is configured Matching, but the read-only lookup did
not freshly compile it or establish a separate strict function acceptance.
Source SHA256:
`78458a41a1f8837aee2d6cb2d311e6dbb53db22b0af00377a29c18095abf6440`.
Original debug ELF SHA256:
`d4a06e4516ab9a75e6de780beee10ea07b8358016e4964a8baf9e8eaa282908c`.

The native Ghost44/Header88 definitions come directly from the unchanged
canonical GhostStart source. They replace the old oversized synthetic GhostData
view and preserve the actual double-pointer handle chain. The native model-run
field is a signed halfword at+1820, with model stride2056. Car stride is952,
place-locked is+239 and score is+240. A data-only IDO probe reads16 emitted
layout/type values and rejects three deliberately wrong layouts.

The current canonical save and heap helper bodies are preserved unchanged.
The native free boundary is the existing func800960CC with its genuine
func80095CF4/95CFC lock/unlock layers. The later GC allocator implementation is
not substituted into the N64 backend. The existing volatile ending-ghost flag
is a disclosed inherited N64 declaration hypothesis; this donor does not prove
its original N64 qualifier. Existing context-only source devices remain unchanged
and receive no new credit here.

The donor's real g/i and direct-car-field form restores the native branch-likely
allocation load missing from the older free-helper candidate. The remaining
source/control-flow difference includes a duplicated player-count reload and
early address setup. Observation-only optimizer tracing reproduces the complete
448-byte-candidate object, SHA256:
`1a84fa4ac87252adb9b493a90a63ed58ce9b262c3e4db9c1acdbe07cf4572f41`.
This is a diagnostic observation, not permission for padding or forced registers.
No unused locals, empty conditional holders, custom instruction edits or forced
allocator colors were added to the new root.

## Bounded behavior proof

An independently expressed fixture-field/event model is compared with native
and relocated candidate MIPS execution over192 fixtures per source form. The
model does not walk the source's pointer chain. Cases cover non-ghost modes,
counts-1..4 (negative counts safely perform no iterations), null/present handles,
negative/zero/positive recording flags, zero/nonzero place-locked bytes, finite
copied times, and valid aligned storage for four records. Sizes are bounded to
0..65535. This is not an invalid-pointer, arbitrary-aliasing or concurrent-state
proof.

All108 reachable native instructions and16 branch outcomes execute. Native
words at offsets0xf8 and0x124 are dead duplicate prefixes generated around
branch-likely scheduling. A separate conservative control-flow traversal confirms
that these two words are unreachable; they are not counted as executed. The full
donor reaches110/112 candidate instructions, with two dead duplicates. The typed
historical control reaches109/110. All three bodies preserve saved GPRs/SP and
FPRs in these executions.

Purge, heap-free and OS queue callees are explicit argument/event stubs with
caller-register clobbers, including the real internal heap free's s0/s1 clobbers.
The busy flag must be1 at every call. Callee allocator/ghost mutations, timing,
thread behavior, actual heap operations and full runtime semantics are outside
this proof. The test does not claim their side effects are executed or modeled.

Three separately compiled semantic negatives change the model-run store to a
byte, erase the recorded size, or free the wrong pointer. Each fails the model
for a behavioral reason. These are proof controls, not matching candidates.

## Reproduction and integrity

`verify.py` is a protocol-v2 proof: seven fresh full-group builds, every member
and context score, complete root extents, exact native direct-call sequence,
all33 strict preservation checks, exact repeat identity, layouts, behavior and
negative controls. Source, script, model, target-row and baseline scorer/lock
hashes are sealed. A second receipt binds full stock/traced object identity.
Focused tests include eight optimized-Python refusal cases, including false
claims, source/model/target mutation, unverified relocation and trace corruption.

Use the existing configured IDO/MIPS toolchain:

```
python cloud/work/frontier/dot_mat3_ghostend_20261017/verify.py
python -O cloud/work/frontier/dot_mat3_ghostend_20261017/verify.py
python -m pytest -q tests/cloud/test_mat3_ghostend_20261017.py
```

For an isolated source packet, pass `--repo /path/to/complete/unchanged/checkout`
to the verifier and set `N64_RESEARCH_REPO` to that path for pytest. Target and
own-data artifacts are never modified. Baseline is current master58425072.
Raw machine code, trace dumps and objects remain outside this packet.
