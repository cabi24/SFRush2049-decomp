# Runtime A list-control callback: 900-byte standalone match

`func_803AE940`, image A `[0x803AE940,0x803AECC4)`: **strict MATCH,
225/225 words**, standalone IDO 5.3 O2. This is a new matching candidate,
not an image/compression/ROM acceptance claim.

## Source and provenance

Base: `7e62ed7b3c3e6f1e788bf013419b89e132b4c65d`. Source is
`cloud/matches/ovl_a/func_803AE940.c`; canonical flags are its O2 header plus
the scorer's mandatory `-Wab,-r4300_mul`. There are no private compilation
flags, invented caller bodies/formals, pressure objects, forced volatility,
assembly, or owned constants. The first full natural reconstruction matched.

The protected image-A extent records both `data_ref` and `prologue` entry
proof. Its complete native SHA-256 is
`06cf3b3f92db2312ea382822691b45d872794700b16d06c31cfd6da3f7d7aef1`.
The image hash is `0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667`.
Before reconstruction the current master was searched by address and symbol
across source, research, claims and locks. Existing AE940 references were
end-of-extent references for the separate AE63C settings callback, not this
body. Read-only arcade comparison at rushtherock
`845329d7b36f5a384c5625ed9a0aef584ab46139`, especially `game/select.c` and
packed animation descriptors in `game/attract.c`, found related BLIT idioms;
no exact whole-function donor or original-name recovery is claimed.

## Behavior and actual context

This ordinary O32 callback receives one BLIT pointer and returns integer 1.
It snapshots four 4-bit descriptor components as signed-halfword locals before
calling any helper. It applies mode/group/scroll visibility gates, selects a
two-state texture, optionally applies a floating fade, centers geometry,
adjusts list text using a resource entry and measured width, applies row
spacing, and sets either horizontal flip or a per-texture flag before updating.
The front-end list-control interpretation is inferred from these operations;
exact screen/option labels are not claimed.

The native BLIT fields agree with the previously established callback layout:
X/Y at 14/16, width/height 20/22, alpha/flip/hide 24/25/26, callback at 40,
descriptor at 44, and texture index at 52. Texture records stride by 32 and
use byte 21. ResourceTables is an accessed prefix consistent with accepted
`src/blob/groups/slot_sound/func_800A4E58.c`: the two relevant pointers are
at +12 and +16, with an unsigned halfword index at offset +2 of the first.
Its two-halfword view describes only accessed storage, not a newly asserted
original type. The equivalent array spelling `indices[1]` was also compiled
once and left 26 schedule/register differences; it was not used to infer
additional semantics or an ownership contract.

Real helpers are the visibility wrapper at 80094F88, texture selection at
800EF5B0, fade getter at 800BEA30, selector at 800B42F0, text measurement at
800B3FA4 and BLIT update at 80094EC8. Historical symbol labels for the last
three are not reliable semantic names. The accepted fade getter returns a
float directly. No private helper context or retained volatile register
contract is required by the complete matching body.

## Fresh verification

`verify.py` binds the source, protected metadata, tool binaries and scorer.
It checks the complete ELF function is exactly 900 bytes, the entire text is
912 bytes with exactly twelve zero alignment bytes, there is only one defined
function and no allocated data, and all **35 relocations** target the expected
16 external anchors inside the function extent. No instruction masking is used.
An independent explicit-address GNU ld link agrees byte-for-byte with both
the protected native body and project relocation result.

The fixture checks **5,824 cases** using unchanged candidate C, a separate
Python state/trace oracle, and GCC undefined/bounds/float-cast-overflow
sanitizers. Cases include all four tested control indices, multiple groups
and texture entries, signed geometry boundaries, non-boolean flag values,
scroll boundary transitions, byte wrap after finite float conversion, and
five side-effecting helper mutation stages. Descriptor snapshots, post-call
geometry/global reloads, and the resource-width callback are covered.
Eight wrong-source controls are rejected by both host behavior and strict
native comparison. Focused tests verify the oracle contracts, source/receipt
binding and sanitized host replay. The focused suite also invokes the complete
compiler/GNU verifier when its tools are available, and explicitly skips only
that case when they are absent. WITH-tool and nested-root runs pass 20/20;
missing IDO passes 19 with one documented native replay skip. Optimized Python
is explicitly refused. Stable receipt comparison retains source, native byte,
geometry, relocation and behavior bindings while allowing separately recorded
live toolchain/scorer/protected-manifest metadata to change.

## Limits and reproduction

Backing tables and helper effects are synthetic. Actual resource contents,
reachable descriptor ranges, renderer execution, invalid floating conversion
and FCSR exception behavior are not proved by the host fixture. The tested
float inputs are finite, nonnegative and convertible to unsigned32. Wider
signed-halfword assignment follows the target/GNU implementation rather than
a portable-C claim. There are no image, compression, ROM or hardware gates.

```
python3 cloud/work/frontier/dot_runtime_a_list_control_20261006/verify.py
python3 cloud/work/frontier/dot_runtime_a_list_control_20261006/verify.py --check
python3 -m pytest -q tests/cloud/test_runtime_a_list_control.py
```

The verifier accepts `--repo` and `--tools-repo` for trusted target/tool inputs.
Use the documented IDO 5.3 and MIPS binutils setup. Temporary products remain
under ignored `build/list_control`; publication includes no ROM/native dumps,
objects, generated binaries, credentials or unrelated private information.
