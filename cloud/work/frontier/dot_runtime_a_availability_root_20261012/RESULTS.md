# Image-A model-slot initialization: strict callee, private-ABI research

## Source deliverable and accounting

`group/` now reproduces **func_80397AD8, all 327 words / 1,308 bytes**, interval
`[0x80397AD8,0x80397FFC)`, with unmodified stock IDO 5.3. Its complete 40-byte
literal range and four-byte white color initializer also match. The sole real
caller 97FFC remains **15/268 words different**, full 1,072-byte extent and
72-byte frame. Outer 9842C remains NONMATCH.

This is **empty-claims, non-installable research**. New individually strict
source bytes: **1,308**. New installable, accepted-image and ROM bytes: **0**.
The private callee cannot be installed independently of the unresolved caller
and the existing availability/cleanup group ownership. This packet proposes
no installer replacement or group-identity migration. `packet_spec` refuses
its empty claims, and the verifier requires that refusal.

All four prior exact bodies remain diagnostic members and strict matches:
94564 (720 B), 94834 (400 B), 949CC (308 B), and 94B00 (728 B). The first three
are **1,428 already accepted bytes**; 94B00 is **728 inherited #359 candidate
bytes**, still unaccepted at base master `46780d3e85fb79001d64cceef25c868e187265d7`.
None is new credit or proposed for reinstallation. No accepted identity loses
members, and no protected source or lock changes.

## New causal result

The inherited complete 97AD8 reconstruction in `context_baseline.c` (PR #365)
scored **200/327**, with a true **1,280-byte** function and 28 missing native
bytes. Its partial own-data failure and 22 unverified references remain in the
receipt rather than being excused as an accepted baseline.

Native code repeatedly multiplies position components by a retained `1.0f`.
The inherited source spelled these as literal multiplications, which the
front end simplified before register allocation. A genuinely consumed
`f32 scale = 1.0f` local restores those operations and their native floating
register allocation. Declaring it after the real four-byte color local and
before the signed-halfword output local also restores both native homes:
color at `sp+76`, output at `sp+70`. The frame remains **80 bytes**.

This one coherent source change reaches strict **0/327**, full 1,308 bytes,
zero extra words, unresolved symbols, unverified references or data errors.
It introduces no unused local, buffer reduction, pad array, invented formal,
fake caller, new volatility or compiler override. Original variable names
and exact declaration spellings remain unknown.

Two causal controls isolate the declaration placement while retaining the
same operations and complete 1,308-byte extent:

- Scale before color: exactly one differing word, color home `sp+72` instead
  of `sp+76`, at +0x14.
- Scale after output: exactly one differing word, output home `sp+74` instead
  of `sp+70`, at +0x15c.

The real signed-byte input still arrives at incoming stack+3, from 97FFC's
stack+0 word store. The full native call census is 9842C+0x16c to 97FFC and
97FFC+0x188 to 97AD8. All 327 callee words, including entry/exit and calls,
are now byte-exact. The already-matched texture lookup's actual volatile
count declaration is retained and bound as an input; no new volatility is
used to steer compilation.

## Root history and measured stopping point

All #365 stages remain self-contained and replayable:

- Imported #359 kept-root baseline: canonical 260/268; complete GNU linkage
  gives **262/268 plus 12 extra nonzero words**, 1,120 B, frame 104. The
  canonical truncated-slice HI/LO and own-data diagnostics remain recorded.
- Genuine 9842C parent and 949CC cleanup: canonical 176/268, GNU 177/268,
  1,032 B, frame 64. The absent 40-byte native tail is counted.
- Complete real 97AD8 context: 31/268, full 1,072 B, frame 64.
- Consumed screen-count local: 17/268, frame 72 and exact spill homes.
- Final loop-line separation: 15/268. The newly exact callee preserves this
  root result and every prior exact member.

This round re-established stock UGEN/assembler tracing and independently
confirmed complete object identity for unforced traced uopt, ugen and as1.
The legal three-color diagnostic still gives 6/268; it is not matching
or installation evidence. At the 97AD8 call, as1's ready-list trace selects
the completed byte store before five independent stack stores; the native
order leaves that byte store in the call delay slot. Joining statement lines,
using a comma argument, and reusing consumed quotient/divisor locals did not
close it. The other nine words remain the screen-address/value allocation
swap. Bounded pointer, local-use and block-boundary probes were also flat or
worse; none is retained. This is the measured stop for the root, rather than
an unsupported claim that all source solutions are exhausted.

The exact callee changes outer 9842C's private-clobber environment: it now
scores 440/451, emitted 1,712 B versus native 1,804 B, frame 152 versus 192.
Its jump-table references remain unverified. It is not part of the exact
body/owned-data proof or an installer claim.

## Verification

- Protocol-v2 receipt binds all source, baseline, recipe, verifier/trace/test
  inputs, selected native target words/extents, the complete real caller
  census, relevant symbols and owned-data identities. Eight stock compiler
  passes are identified. No shared-tool, lock, Makefile or assembly-text pins;
  no static ROM TU is rebuilt; no non-master Git object is needed for replay.
- Independent ELF/full GNU linkage covers **305 original selected-body REL
  records**, all six complete selected function extents and exact symbol
  bindings, every instruction word, and **52 owned bytes**: callee rodata
  `0x803B94BC..0x803B94E4` (40 B), root literals
  `0x803B94E4..0x803B94EC` (8 B), color at `0x803B5EE8` (4 B).
  The complete 16-byte `.data` allocation is classified as the selected 4-byte
  color plus 12 zero alignment bytes; nonzero suffixes and expansion fail.
  Parent-owned context data remain explicitly outside this proof.
- Missing/corrupt owned data, corrupted root/callee instructions and overlong
  function extents fail closed. Canonical source and GNU proofs independently
  require the new callee to be exact and all prior bodies preserved.
- Normal and Python `-O` deep replay pass. The focused test takes about 0.75 s
  with IDO; without IDO it checks input identities and cleanly skips.
- The unchanged real root bodies agree over **20,480 logical-host cases**,
  also under ASan/UBSan. This uses deterministic helper doubles and stable
  memory; it is finite root baseline equivalence, not an executed callee,
  gameplay, unrestricted aliasing/concurrency, pointer-width or exceptional
  division proof.

The four-byte color and all existing object/array capacities are unchanged.
No raw assembly, ROM payloads, extracted images, compiled binaries,
credentials or unrelated private data are part of the publication.

## Reproduce

```
python3 cloud/work/frontier/dot_runtime_a_availability_root_20261012/verify.py --deep --score
python3 -O cloud/work/frontier/dot_runtime_a_availability_root_20261012/verify.py --deep
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_a_availability_root_research.py -q -o addopts=''
python3 cloud/work/frontier/dot_runtime_a_availability_root_20261012/trace.py
```

Optional `trace.py --tracer PATH` independently checks the instrumented uopt's
complete object fidelity and force-only diagnostic. All acceptance evidence
comes from stock IDO. The independent checker owns further ownership
reconciliation, image composition/recompression, the full-ROM gate and any
merging. This remains a draft.
