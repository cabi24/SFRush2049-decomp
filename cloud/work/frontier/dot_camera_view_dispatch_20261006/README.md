# EB028 camera-view dispatcher: complete donor reconstruction

**COMPLETE-NONMATCH. No new matching bytes, acceptance, image or ROM claim.**

This packet reconstructs the complete `func_800EB028` interval,
`0x800EB028–0x800EB690` (1,640 bytes / 410 words), from its genuine arcade
ancestor, [`game/camera.c::setcamview`](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/camera.c#L672).
The N64 port supplies per-car/per-view records, a Y-up force filter, two additional
view distinctions and collision-endpoint corrections. The original-source search
at base `dea99f09ab19b1d3b324ed7097162f7b378e7096` found declarations and callers,
but no complete EB028 definition or prior frozen matching campaign.

## Honest compiler result

| Build | Complete ELF bytes | Native bytes | Differing full-body positions |
|---|---:|---:|---:|
| Exactly `-g0 -O3 -mips2 -G 0 -non_shared` | 1,636 | 1,640 | **285**: 284 common differences + one missing word |
| Existing scorer's `-Wab,-r4300_mul` control | 1,640 | 1,640 | **156** |

Both have the native 96-byte frame. The canonical control reproduces the complete
44-byte case table and 16 literal bytes at `[0x8012451C,0x80124558)`; its four final
zero section-padding bytes are excluded. The next native value belongs elsewhere.
The exact-flags build omits a multiply-delay nop, so its table entries shift and
are **not** claimed equal. Its four floating literals remain equal. No source is
registered as a match and neither build is labelled an exact-O3 match.

The canonical residual is mainly floating-point allocation/temp-register order.
Native loads of `pos_out[1]` use `f0`; the candidate does not promote that array
component. The saved camera-position pointer also uses stack offset 44 rather
than native 48. The proof uses separate explicit stack-write allowlists for these
complete bodies; matching frame size is not described as matching frame layout.

## Source and ABI

The entry has one ordinary O32 pointer input, no private-register arguments and
no unsaved callee-saved writes. All 28 direct-call sites were inspected. Only the
suspension helper's `f0` result is consumed as a return; other live caller values
are saved or reloaded around calls. The unmatched `entity_iterate` remains an
explicit two-vector external boundary that may modify its first endpoint and
returns zero. It does not require a fabricated caller or matching its internals
to reconstruct this outer body.

`candidate.c` is standalone natural C89 source. It needs no in-unit caller,
inlined helper or deleted-static stub. The real 952-byte car, 2,056-byte model
and 152-byte camera strides are expressed as typed records; opaque bytes describe
observed record offsets and are never local frame filler. Matrix storage is nine
floats. All eleven table cases and the unsigned out-of-range path are present.

Distinctive donor/native evidence includes the worms-eye offset, one-sided
±100000 force clamps, `.00002f` accumulation, per-axis elastic filtering, and the
same camera-family dispatch. N64 changes include X/Z components 0 and 2,
separate Y damping, signed car/slot bytes, unsigned view selection, view 2/3's
post-callback collision predicate, view 10's captured endpoint and view 4's live
car endpoint. `osPfsChecker_full` is a historical misname: base
`src/rom/lib_c990.c` establishes this call as `osStopThread`.

`provenance.json` records exact pinned donor blobs. The `veccopy` and `AddVector`
macros preserve substantive definitions from the donor headers. This establishes
source ancestry, not recovery of original N64 spelling or translation-unit
membership. The initial plain donor addition loops stayed looped; the genuine
AddVector control recovered the full 1,640-byte shape. An integer-zero diagnostic
was rejected as a source claim; final source uses the donor's `0.0` spelling.

## Verification

`verify.py` compiles both disclosed recipes with pinned, unmodified IDO and
independently links every relocation using GNU MIPS ld. It checks full ELF symbol
extents, missing/excess positions and meaningful data separately from padding.
It never slices a candidate down to native length to claim equality.

- 768 finite-domain cases compare the unchanged source under host C89+UBSan,
  protected native code, and both complete GNU-linked compiled bodies.
- 768 protected-native executions and 1,536 compiled executions cover all
  410 native instructions; the candidate coverage is 409 / 410 respectively.
- Every byte-valued mode is tested, plus repeated real-view grids with all valid
  model/view indices, nonsymmetric matrices, one-sided force boundary cases,
  signed zero and callback mutations.
- Ordered call arguments and input snapshots, all mapped nonstack bytes, stack
  write allowlists/canaries, integer/FP saved registers and return restoration
  are checked. Caller-save GPRs/FP registers are poisoned at every external call;
  only genuine return values are restored.
- Data-dependent branch outcomes execute. Two guards from the fixed two-step
  elastic loop have only their reachable outcome; unconditional branches are
  recorded separately in the detailed outcome list.
- Four compiled wrong-source controls are rejected: wrong initial lift, wrong
  force axes, reversed post-callback collision test and lost endpoint snapshot.
- Unknown instructions, wrong call targets and redirected saves are refused.

Matrix-copy, transform and suspension arithmetic are explicit, independently
expressed hook models, **not execution of those native leaf bodies**. The other
camera helpers are mutation-aware bounded contracts; they do not simulate their
actual camera algorithms. The host and MIPS hooks use different implementations
of the same contracts. This verifies the outer reconstruction within that domain,
not the correctness of the entire camera subsystem.

The native interpreter fails closed on unmapped/unaligned accesses, unknown
instructions, invalid/misaligned control flow and transfers in delay slots.
NaNs, infinities, FCSR status/exceptions, invalid indices/pointers, partial pointer
aliasing, concurrency, actual thread scheduling, full callee execution, real
scene collision and gameplay are outside this proof. No shadow/source-image,
compression, ROM, hardware, full-suite or hosted CI pass is asserted here.

## Diagnostics and stopping condition

Exploratory comparisons used the genuine accepted transform and related
matrix/object-update context, original donor loops, and a donor brace macro.
They did not identify an improved final source. These context and instrumented
compiler observations are not included in this packet's reproducible receipt,
are non-gating, and are not accepted-context verification claims. The recorded
end-to-end proof above applies only to the two disclosed standalone builds.

No forced compiler object, invented keeper, padding, extra ABI input,
unsupported volatile or source-order sweep was admitted.
The packet stops here. Reopen for a concrete original-source/alias or variable
lifetime explanation that predicts the missing FP web and 44/48 pointer home.

## Portable reproduction

From a repository containing this packet:

```
python3 cloud/work/frontier/dot_camera_view_dispatch_20261006/verify.py
python3 -m pytest tests/conveyor/test_dot_camera_view_dispatch.py -q -o addopts=''
```

`--root PATH` supports a separate complete protected-input/tooling root.
`--record` deliberately replaces the local receipt after reviewed changes.
Ordinary replay compares only stable packet/native, executable, extent,
relocation, data and behavior facts. Human-readable scorer diagnostics are
retained but excluded from equality. No live production-file hashes, live lock
state or test-file hashes are pinned. Compiler-dependent tests skip cleanly when
pinned IDO or GNU MIPS ld is absent. Full current-master test matrices remain a
separate publication gate owned by the coordinator.
