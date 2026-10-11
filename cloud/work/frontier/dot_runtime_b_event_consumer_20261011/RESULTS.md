# Image B event consumer: strict MATCH

Deliverable: `cloud/matches/ovl_b/func_803936A8.c`.
Baseline master: `c340dfcacc1a3051930f984c9c05ad16548c01f2`.
Population B, `[0x803936A8, 0x80393B34)`, **1,164 bytes / 291 words**.
This is newly strict-verified source; no new image-landed or ROM-verified bytes
are claimed. The pre-existing lean candidate was a complete 2-word NONMATCH.

## Directed diagnosis and source change

Baseline `cloud/work/lean/runtime_b_event_consumer_20261006/candidate.c`
reproduced exactly 2/291 words different under stock IDO 5.3 O3: buffer-base
instructions at offsets +0x74 and +0x130 used frame+96 rather than frame+112.
Both bodies had the same 144-byte frame, instruction count, register sequence
and no extra words. Workbench diagnosis was run first; its unlinked comparison
also reported 62 unresolved relocation-layout sites, which are separately
resolved in the strict scorer and GNU linker checks. These are not semantic
source discrepancies. The full-word scorer establishes the two-word residual.

The source-reachable lever is declaration order (workbench IDO law L63).
Moving the existing `char text[32]` declaration above `i, x, y, event` moves its
home up exactly 16 bytes without changing frame size or generated code elsewhere.
One directed candidate was tried, and all 291 words then matched. Four actual
4-byte scalar/pointer locals explain the offset. No added pressure state, pad
array, fake prototype, removed logic, sweep or optimizer patch was used.
The verifier restores the old declaration order as a negative control and
requires precisely those two mismatches, with both frames still 144 bytes.

## Genuine semantics and ABI

The function takes one opaque callback argument, homes it at entry-sp+0, and
returns 1. Image B's ten-entry 36-byte HUD descriptor table at 0x80393DE8
registers this callback in entry 9 (descriptor 0x80393F2C; callback slot
0x80393F48). Its texture sentinel is -1. The native B:803925D0 setup passes
that table and count 10 to `sound_control` / NewMultiBlit (0x800B37E8).
This establishes registration identity, not universal frame-dispatch reachability.
The input argument is not fabricated frame pressure. No extra formal is used.

The first loop draws each signed-byte player counter, shadow then foreground.
The second visits exactly four 24-byte events, independently established by the
already-locked B:803914B4 producer. Events with signed age >40 deactivate and
apply a signed-byte delta to the row player's counter; positive deltas also
update the row/column matrix through `func_800F7E30`. Younger events interpolate
position and draw same-player or other-player labels. Age advances only when
both pause flags are zero. The final alignment and render-state calls remain.

Record offsets, signedness and one-pointer callback ABI follow native code;
original names, whole-function arcade ancestry and original local declarations
are unknown. A real 32-byte text buffer is retained, not an arbitrary pad.
Its original capacity is inferred. The native behavior assumes valid row/column
indices, player counts, selector 1..4, valid string pointers, and formatted text
that fits the buffer, including the terminator. No bounds-hardening change is
claimed. All helpers remain external, with existing return/parameter contracts;
`func_800F7E30` is the already matched signed-byte matrix add helper.

## Verification

Frozen receipt and independent peer review pass. The input-bound verifier pins source,
itself, target words, target metadata and the callback-registration evidence.
It must not pin mutable tools, lock files or assembly text. It checks the complete
ELF function, full GNU relocation resolution at exact VMA, zero-only alignment
padding, and absence of owned data. No raw assembly, ROM bytes or objects belong
in this packet. One test invokes the complete replay under the shared conftest
missing-tool policy. No protected source, target or lock is changed.


Reproduce from the repository root with stock IDO 5.3 and MIPS GNU binutils:

```sh
python3 tools/cloud/score.py fn cloud/matches/ovl_b/func_803936A8.c func_803936A8 --targets asm/us/ovl_b --flags "-g0 -O3 -mips2 -G 0 -non_shared"
python3 cloud/work/frontier/dot_runtime_b_event_consumer_20261011/verify.py
python3 -m pytest -q tests/cloud/test_dot_runtime_b_event_consumer.py
```

Exact scorer output:

```text
func_803936A8:
  MATCH
```

Verifier output:

```text
PASS: 291/291 words; 1164-byte exact-VMA GNU proof; 62 relocations; 2-word control
```

The complete ELF function is 1,164 bytes; `.text` is 1,168 bytes with exactly
four zero alignment bytes after the body. GNU ld independently resolves all
62 text relocations at VMA 0x803936A8, without masks or unresolved symbols.
There is no owned data, literal pool or jump table. The packet test completes
well under one second with the local toolchain. No host-behavior emulation,
whole-image splice, recompression or cartridge build was performed.


Independent boot-neighbor lane reviewed frozen source/verifier commit
`0570223f7c4dcb93114f973057b3400da34ad534`, independently replayed the verifier
and REQUIRE_TOOLCHAIN packet test, compared the source against the prior
candidate and locked signed-byte helper ABI, and reported no blocking findings.
Protected-path guard, changed-submission rescoring, and `make check-matched`
also pass (571 static locks and 56 runtime-image entries intact at this base).
Keep the PR in draft state for the user's independent checker; no merge or
ready-for-review action is authorized by this packet.
