# D6E7C menu measurement: fixed-buffer research hypothesis

Observed local research candidate, explicitly NONMATCH.
Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `drone_collision_avoid` at **0x800D6E7C, 852 bytes / 213 words**.
The historical label masks menu label/value measurement, not driving AI.
Neither the master definition index nor PR163 supplement had native-backed C.

Canonical local comparison improves **211/213 to 139/213 differing words** when
the new complete body is compiled with its genuine D71D0 setup caller and D7634
parent. The standalone control has15 extra nonzero words; the published context
has none, and no unresolved/unverified references or relocation errors for this
target. The ELF body is **840 bytes**, twelve short, and frame136 versus native144.
This is a substantial nonmatch, not verified coverage.

## Explicit unproved capacity assumption

The real encoded-string copy receives a local scratch address at native sp+84;
the next observed live scalar is at sp+128. This packet makes one fixed
**44-byte buffer hypothesis** from those boundaries. The original declaration
and maximum source-string length are not independently established. No buffer
size tuning was performed, and this is not a proven safe runtime string bound.
The source comment preserves the same limitation. It must be resolved before
accepting a final source/layout claim.

The routine retains four label rows, two fixed numeric text references, fourteen
candidate names through the real encoded-string copier, the separate bytewise
uppercase pass, width comparisons and final left/width/height/value-column
stores. Strings remain external resources. The native unsigned width comparisons
and signed centering division are retained.

The real graphics lock/font/unlock wrapper organization comes from
`cloud/work/s20261004/E/src/helper.h` (also used by recent controller-menu work).
`setup.c` and corrected `caller.c` are the genuine D71D0/D7634 context published
in PR267; `speed.c` is the accepted `codex_vsync_a145` closure. These are context,
not additional claims. Making only the real slot setter visible caused unwanted
inlining and regressed the score, so it is not part of this packet. Partial
compiler visibility and recovered field meanings remain assumptions.

## Reproduce

With IDO5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_menu_measure_d6e7c_research_20261006
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`, canonical `-Olimit 5000`
and `as1 -r4300_mul`. No artificial caller, extra formal, capacity sweep,
assembly or production/lock changes. Independent checker owns acceptance and
ROM integration.
