# Runtime A color smoothing: natural-source improvement

`func_8038A634`, `[0x8038A634,0x8038A820)`, 492 bytes / 123 words.
**NONMATCH: 41/123 words differ**, no unresolved or unverified relocations,
no extra words. Fixed research base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

The first complete natural array-loop reconstruction scored 113/123. Explicit
three-byte RGB table fields plus a genuine three-byte snapshot buffer improve
this to 41/123. No source-shaping devices, fabricated formals, stand-in callers,
volatile fields, compiler patches or assembly are used. O2 and O3 give the same
residual. Eighteen follow-on expression/layout/denominator trials did not improve
the 41-word candidate.

The first 38 words match. The workbench finds all 18 pool-lane assignments equal;
first divergence is the temporary after the first signed division. Native uses
a restricted t6–t9 cycle, whereas the standalone candidate continues into t3–t5.
The changed temporary choices alter division scheduling. This is consistent
with missing whole-program context; it is a hypothesis, not a proved closure.
The actual callers are `func_8038DFEC` (1,520 bytes, call at +0x540) and
`func_8038E5DC` (3,704 bytes, call at +0xE18). Neither was fabricated or compiled
as context here.

The source snapshots the selected RGB bytes, either snaps or takes one tenth of
the signed component delta, derives alpha from a signed byte, and conditionally
updates an existing BLIT. It preserves direct global reloads and unsigned-byte
narrowing. Its BLIT record describes only the accessed prefix; original names
and exact arcade ancestry are unclaimed.

Reproduce from repository root with documented IDO:

```
python3 tools/cloud/score.py fn cloud/work/frontier/dot_runtime_a_color_smoothing_20261006/candidate.c func_8038A634 --targets asm/us/ovl_a --flags "-g0 -O3 -mips2 -G 0 -non_shared"
```

This is a research improvement only. It is not eligible for standalone
`cloud/matches`, splicing, acceptance, or cartridge-coverage credit.
