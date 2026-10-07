# Runtime-A object initializer: 220/234 words

Frozen base `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Complete `func_8039B560`, 936 bytes, compiled with stock IDO 5.3 group pipeline
and actual `-g0 -O3 -mips2 -G 0 -non_shared`.

Observed local result: **14/234 words differ**, no extra words, unresolved
symbols, unverified own-section relocations or relocation errors for B560.
The first complete reconstruction differed at 18 words. Direct indexed slots
and removing a once-used scalar fixed the real 72-byte frame and local offsets.
The remaining differences are the f20/f24 allocation of 100.0f versus the Y
snapshot, plus associated load scheduling. Workbench identifies the same
234-instruction geometry, identical integer allocation, and these two FP webs.
No forced-register declarations, unused frame filler, fake calls or extra
volatile qualifiers were added.

The function initializes missing handles for 12 64-byte slots, copies identity
matrices, allocates native models, applies a color snapshot, binds four textures,
sets the six special slot transforms, and signals the initialized/dirty state.
These are descriptive names, not recovered original source names.

## Genuine context and boundaries

The minimal group retains the complete real BE48 caller as its kept root. B560
has no incoming non-ABI parameters; the original does not preserve S/F registers
locally, so it is not mislabeled as an ordinary standalone match. The same
14-word result is observed in the full genuine eight-function runtime-A family.

BE48 is context only here: keeping it as a root produces a larger save frame,
188/190 differing canonical words, 26 nonzero extra words and a comparison-window
HI16 diagnostic at its tail. This minimal context is not a claim that BE48
matches or that this is the exact original whole-image compilation boundary.

Main-blob `sign_extend_call` stays external. Its corrected returning definition
is supplied as `wrapper_return.c` in `external_sources`, not compile `files`.
No cross-image inlining occurs. The existing allocator return is consumed by
both the native B560 and BE48 callers.

Native float anchors `D_803B9594` through `D_803B95B4` and external color
`D_803B3458` have unresolved values/original ownership in the available artifact
set. They remain true address-resolved external reads; no invented literals or
owned-data acceptance claim is made. Names/identity matrix are external too.
`claims` is empty: this is a local NONMATCH improvement, zero accepted bytes.

This minimal research alternative overlaps BE48 context from #208/#224. Do not
install duplicated function definitions or count those earlier functions again.

## Reproduce

Configure stock IDO and GNU MIPS tools, then:

```
python tools/cloud/score.py group \
  cloud/work/frontier/dot_runtime_a_object_initializer_20261006 --targets asm/us/ovl_a
```

Only matching compilation/scoring and residual diagnosis were done. No proof
packet, acceptance suite, CI wait, production edit, lock or promotion is included.
