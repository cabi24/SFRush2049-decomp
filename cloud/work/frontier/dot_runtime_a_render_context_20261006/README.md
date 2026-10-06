# Runtime-A rendering context: C140 near-match

Research base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
IDO 5.3 stock group pipeline; actual `-g0 -O3 -mips2 -G 0 -non_shared`.

Complete real `func_8039A4D4` (2,864 bytes) restores missing floating-point
clobber context for its real `func_8039C140` caller (1,984 bytes). The included
B120/B214/BE48/A448/B00C family supplies genuine call edges too. No synthetic
caller, pressure function, forced register or unused frame filler is present.

## Observed scores

- C140: **7/496 words differ**, versus 484/496 in the earlier parent context.
  Four frame/home words and three scheduling words remain. Native frame is
  136 bytes; candidate frame is 128 bytes.
- A4D4: **623/716 words differ**, complete reconstructed body, NONMATCH.
- B120/B214/BE48/A448/B00C retain zero code/relocation differences. These are
  previously reported context, not new progress or new acceptance claims.
- Neither new research body reports unresolved symbols, own-section verification
  gaps, relocation errors or nonzero extra words.
- `claims` is empty. These observations do not claim behavioral equivalence,
  original source, cartridge coverage or source-owned data completeness.

Final otherwise-identical plain-clock control: change only
`extern volatile f32 D_8002EB94;` to `extern f32 D_8002EB94;`.
C140 then differs at **129/496** words and A4D4 at **631/716**; all five earlier
helper code scores remain zero. This final one-change control supersedes
intermediate plain-clock scores.

## Contract boundaries

The clock qualifier is an **existing accepted-source contract hypothesis** for
this exact symbol. At the pinned base:

- `cloud/matches/entity_collision_detect.c:20-21,50` declares the same volatile
  float view.
- `cloud/matches/func_8008B3C8.c:111` uses it too.
- `include/game_globals.h:51` retains plain float and records a volatile quirk.

A4D4 reloads the clock on distinct native arithmetic accesses. Preserving those
observations does not prove an asynchronous or interrupt writer; the earlier
writer audit did not find one. The ordinary-clock control allows the checker
to decide that source-contract boundary independently.

A4D4 uses IDO's genuine `fabsf` intrinsic and normal pragma, indexed slot loops,
a real previous-Y snapshot, matrix/texture/color calls and observed animation
branches. C140's switch body order follows native blocks (0, 1, 3, 2). Neither
body has an unrecovered computed jump table. Aggregate views describe accessed
native offsets/strides, not original type names.

Native float anchors `D_803B957C` through `D_803B9590`, `D_803B95B4` and
`D_803B95B8` stay external with unresolved values/original ownership. The fixed
artifacts provide no runtime-A data image to settle them. `D_803B345C` remains
the external native color initializer. A text score cannot settle these data
boundaries.

Main-blob `sign_extend_call` remains external to runtime-A compilation.
`wrapper_return.c` documents its actual returning implementation, listed only
in `external_sources`, never the compile `files`.

This expands the alternative contexts in #190, #199, #208 and #211. Do not
install all copies into one unit or count existing helpers again. Those PRs
remain available for the checker's integration decision.

## Reproduce

With stock IDO 5.3 and GNU MIPS tools configured:

```
python tools/cloud/score.py group \
  cloud/work/frontier/dot_runtime_a_render_context_20261006 --targets asm/us/ovl_a
```

For the plain-clock control, copy the directory to temporary storage and change
only the declaration above. Preserve the same manifest, flags, kept root and
external-image boundary, then run the group scorer against that copy.

Only matching compilation/scoring was performed for this lean publication.
No acceptance suite, ROM reconstruction, CI wait, lock or promotion is included.
