# Image B HUD scene closure: lean research

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Three reconstructed image-B bodies, 3,580 bytes total:

- `func_80391650`: 532 bytes, **19/133 words differ**, no extra words,
  unresolved/unverified sites or errors with authenticated own-data context.
- `func_80391864`: 668 bytes, 130/167 differ, one extra word and six
  unverified section-relative sites. The misplaced switch-table reference
  prevents a complete own-data score; both float values check by content.
- `func_80391B00`: 2,380 bytes, 564/595 differ, no extra words, fourteen
  unverified section-relative sites and one own-data comparison failure
  caused by the broad instruction-layout mismatch.

The useful improvement is the first helper: compiling its genuine caller
and sibling together changes the standalone 133/133 differences plus 24
extra words to 19/133, reproducing its IPA save/restore convention. The
other two bodies provide real call context and remain broad NONMATCHes.
No accepted matching/coverage claim is made for any member.

## Reproduce

```
python3 cloud/work/lean/runtime_b_hud_scenes_20261006/reproduce.py
```

Actual recipe is IDO 5.3 whole-program O3 with `-g0 -O3 -mips2 -G 0
-non_shared`, the canonical `uld`/`usplit`/`umerge`/`uopt`/`ugen`/`as1`
sequence, and `as1 -r4300_mul`. Only the real outer caller is kept;
`group.json` deliberately has no matching claims. The helper authenticates
the frozen native asset and image B in memory and supplies it to the unchanged
canonical own-data scorer. No raw bytes, assembly dump or object is included.

## Context and uncertainty

The two internal functions update each player's kind/status scene object,
recreate changed handles, and scale/rotate/project their transforms. The
caller dispatches those helpers in mode 6, updates another marker per player,
and manages the ten-entry per-player stunt-effect lists in mode 4. Sources
use native 52-byte status records, 72-byte effect records, 120-byte score
records, 152-byte cameras and 952-byte players. External helpers keep their
real existing contracts; no invented helpers or stand-in callers are present.

The 91B00 native body writes a local RGB/alpha color before loading a name
from its four-name table. The corresponding source assignments are retained
as observed operations even though no consumer has been recovered; IDO
currently removes them. The separate mode-4 color really is passed to the
scene-color helper. This is disclosed source uncertainty, not added padding
or speculative pressure. No volatile or inline assembly is used.

Remaining gaps include frame/local layout, initial address scheduling and
allocation, the kind switch's reused value, and broad parent topology. Original
names, local declarations and whole-function arcade ancestry are unknown.
Native float constants were decoded from authenticated image B, including
0.025/0.069/0.175 scales and the 2.69-second fade. The kind-switch mapping is
confirmed as kinds 0..7 to resource IDs 216..223.

Valid inputs require player indices 0..3, backed scene handles, table selector
1..4, and valid model/name pointers. The status helper's first flag value must
be zero or include bit 1/2: other nonzero flags can read an uninitialized local
on the original native path. The kind helper requires kinds 0..8; its resource
local is otherwise uninitialized. Float-to-integer conversions require finite
representable values for portable C. These assumptions are not gameplay proofs.
Only compile/scoring was performed. Independent checker owns acceptance,
further validation and integration.
