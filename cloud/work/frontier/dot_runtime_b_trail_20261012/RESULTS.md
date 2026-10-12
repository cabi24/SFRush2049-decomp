# Image-B trail callback: 1,332 newly strict-matched candidate bytes

Source-first deliverable: `group.c` and `group.json`. Sole member, claim and
kept function: **func_8038CB20**, complete `[0x8038CB20,0x8038D054)` extent,
333 words / 1,332 bytes. No context function or private ABI is needed. This is
fresh candidate yield, distinct from inherited D054 and the prior DDDC result.
Zero new accepted-image, compressed-image or ROM-verified bytes are claimed.

## Trace-first source reconstruction

Current-master lean baseline reproduces 54/333 differences with no extra words,
unresolved symbols, unverified data or errors. Workbench diagnoses 29 register
and 25 constant/home differences, identical instruction topology and a native
224 versus candidate 208 frame. It is not a source-line scheduler problem.

1. Native origin loads occupy the colored f0/f2/f12 lanes. Three genuinely used
   x/y/z coordinate locals, followed by observed lower-then-upper stores, recover
   those lanes and the native store order. No helper or signature is changed.
2. Declaration order recovers the corner-array home at sp+140. Its genuine
   four-by-three capacity remains unchanged.
3. A diagnostic forced-color oracle identifies exactly 20 address-register
   differences. The stock-coloring trace gives pool/mode/timer priorities
   30/17, 20/11, 29/16. A disclosed compiled-out pool-active read at the fade-loop
   head changes these to 40/17, 20/11, 29/17, restoring s4/s5/s6. No load from that
   read is emitted. Instrumented binaries are diagnostic only; every accepted
   result below comes from recognized stock IDO 5.3.
4. The resulting ordinary used-local body has only two frame words different:
   native 224 versus candidate 216. Exactly two unused s32 locals are declared
   before corners, under the live Wave 16 rule allowing the exact frame residual.
   Their original identities remain unknown and are not invented. Stock UGEN
   gives M100/T28/P4 instead of M92/T28/P4; each local contributes four bytes.
   Every other instruction, including the corner home, remains byte-identical.

Both shaping devices are conspicuous in the source header. There is no pad
array, capacity change, inline assembly, volatile qualifier, `#line`, fake
return type, stand-in, modified production compiler or source-boundary shortcut.
No whole-function arcade donor has been established.

## Canonical whole-group score

From the repository root:

```
python3 cloud/work/frontier/dot_runtime_b_trail_20261012/verify.py --score
```

The verifier authenticates current-tree image B in memory and invokes unchanged
`score.py group`, without `--claims` or `--allow-unverified`:

```
Members:
func_8038CB20:
  MATCH
    own .rodata verified at 0x80394D90..0x80394DAC
```

A standalone stock build also matches under that same authenticated owned-data
context. The one-member group is used because there is no public
`asm/us/ovl_b_data` artifact; a bare standalone CLI has no owned-data image.
The focused packet test supplies full-group CI coverage. No image/data payload
is submitted, and ordinary changed-single discovery is not this group's gate.

## Full input-bound verification

- Pins only packet source/group/verifier, selected native extent and words,
  inherited lean source, test, five complete native helper targets, authenticated
  image and owned literals. No tool,
  generated-assembly text, lock, Makefile or nonancestor commit pins.
- Independent GNU linking resolves all **35 text relocations**, every external
  binding and seven owned float values. Full 1,332-byte function, next native
  boundary, 1,344-byte text section and 12 zero alignment bytes check. The full
  owned interval is 28 bytes, with four zero section-alignment bytes. ELF machine,
  byte order, ABI, MIPS2 flags and allocated section set are checked; unresolved
  symbols or linked relocations are rejected. GNU `--hash-style=sysv` keeps its
  ABI identity deterministic across the known GNU 2.42/2.44 default difference.
- The unchanged baseline independently GNU-links to exactly 54 differing words.
  Removing the two unused locals independently leaves only offsets 0 and 0x530,
  the frame allocate/restore pair. Removing the pool read leaves 20 differences.
- Missing owned data fails closed. Corrupting each of all seven individual
  literal entries also fails closed, including the two repeated 0.0333333 values.
- Stock UGEN diagnostics on/off produce identical complete objects for both
  frame variants. Stock as1 diagnostic on/off objects are also identical.
- Native C89 layout compilation checks 32-bit scalar/pointer widths; 64-byte
  Trail and 28-byte Fade sizes; every used node/coordinate/color/pool offset.
  No static ROM TU is rebuilt; the applicable compiler is IDO 5.3.
- The genuine accepted helper sources were inspected: `func_8008D0C0`,
  `func_800AFA84`, `func_8008E3C0`, `func_800A78BC`, and
  `groups/codex_vertices_b88/group.c` (`func_8008C074`). Their observed void,
  pointer-return, ordinary integer/pointer and stack-argument contracts agree.
  Historical giant context headers' implicit s32 declaration of 8C074 is not
  used: its real accepted definition is void. Helpers are external and unchanged.
- Normal and optimized-Python deep replay and canonical whole-group scoring pass.

The function ages/removes fade nodes and emits or updates four-corner trail
quads. Valid well-backed acyclic pool lists, live pointers, and finite timer/
coordinate values are assumptions. Full helper execution, gameplay, hardware,
unrestricted memory safety, original local names and private replacement oracle
are not established. Installation, compression, ROM SHA-1 and merging remain
with the independent checker; keep this PR draft.

## Independent review

The paired image-A lane independently reran normal and optimized replay,
canonical whole-group scoring and the focused test, and audited the complete
source, helper contracts, frame disclosure, GNU extent and owned literals.
It found no blocking source/geometry issue. Its requested selected-native-helper
binding and focused-test source binding are included; no shared tool is pinned.
The focused proof passes in 1.08 seconds locally; missing IDO cleanly skips
through the shared conftest while still checking the input bindings.
