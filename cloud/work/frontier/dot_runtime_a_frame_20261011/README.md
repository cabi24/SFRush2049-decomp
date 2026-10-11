# Availability root: strict 1,292-byte image-A match

## Deliverable

`groups/availability_root/`: the complete 323-word `func_80394058` root is a new
strict stock-IDO group claim. Its extent is **0x80394058..0x80394564, 1,292 bytes**.
The five earlier exact helpers `80392F28`, `80393004`, `803930B8`, `803931C0` and
`803932E4` are preservation members with **no new credit**. `80393004` is already
locked on master. PRs #309/#310's earlier results are not new work here.

The complete genuine `803923B0`, `8039338C` and `80393B3C` bodies remain explicitly
nonmatching context, with their actual source and root-only keep list. The old
PR #311 packet was research with no claims. No previously claimed failed body is
hidden, and none of the three context functions is claimed or spliced.

Base: master `eae0c6466`. All source needed to replay is included. PR #311 is
historical source provenance, never a runtime checkout dependency. The runtime
image landing path now exists. This candidate has not been accepted/spliced,
and no new image, compressed-stream, full-ROM, hardware or gameplay gate is claimed.

## One traced source lever

The inherited root was 2/323 words off, solely its 120-byte frame versus retail's
128. Workbench `diagnose` reports the 8-byte stack-home gap and identical register
lanes; its unresolved-object relocation warnings are superseded by the strict
scorer and independent GNU full-word relocation checks.

Stock UGEN diagnostics locate the missing area: **T=16, P=4, M=2** before, and
**T=16, P=4, M=10** afterward. `umerge -v` shows no inlined procedures in either
version, so no nested-inline home was missing. Diagnostic and ordinary complete
ELF objects are byte-identical for both builds.

Two unused `s32` scalar declarations placed before the real signed-byte locals
supply exactly **2 × 4 = 8 bytes**. The save area at sp+24..111 and the byte homes
at sp+118/119 do not move. Deleting only these two declarations restores exactly
the old two-word discrepancy. Putting unused declarations after the byte locals
has no effect because that tail of the named area is removed. No buffer, operand,
statement, prototype, helper, call, keep entry, or live local was changed.

This is the disclosed exact-frame-residual technique permitted in
`cloud/work/frontier/WAVE_BRIEF.md`, Wave 16 owner rules. It is **not proof of
original declaration names, types, count or purpose**. There are no padding
arrays, artificial accesses, false returns, forced registers or modified compilers.
The mode-read carrier and three genuine dependencies remain the prior reconstruction.

## Strict result and routine verification

Compiler: stock IDO 5.3. Flags: `-g0 -O3 -mips2 -G 0 -non_shared`.
The canonical group pipeline uses the repository's `-r4300_mul` assembly setting.

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_runtime_a_frame_20261011/groups/availability_root --targets asm/us/ovl_a
```

Exact member output (command exits zero without `--claims` or `--allow-unverified`):

```text
Members:
func_80392F28:
  MATCH
func_80393004:
  MATCH
func_803930B8:
  MATCH
func_803931C0:
  MATCH
func_803932E4:
  MATCH
func_80394058:
  MATCH
```

The scorer then reports all three context functions as nonmatching, including
unverified FP owned-data comparisons. They are unchanged research, not acceptance
claims. Their raw instruction differences are intentionally not republished.
The newly claimed root has no owned-data references.

```sh
python3 cloud/work/frontier/dot_runtime_a_frame_20261011/verify.py
python3 -O cloud/work/frontier/dot_runtime_a_frame_20261011/verify.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_a_root_frame.py -o addopts='' -q
```

One routine test: **1 passed in 0.17 seconds** with IDO, including full-member
strict comparison and the frame-only negative control. Without IDO: **1 skipped
in 0.06 seconds** after checking input bindings; the shared conftest converts
missing-tool skips to failures under `REQUIRE_TOOLCHAIN=1`. No scoring failures
are treated as missing tools. Optimized-Python replay also passes.

`verification.json` binds submitted sources, the verifier and diagnostic helper,
image-qualified parsed target-word hashes and extents, and only the relevant
`extents.json`/`symbols.json` checksum rows. Compiler identity is recorded as
metadata. Mutable tools, locks, Makefile and assembly-text digests are not pins.

Optional stock diagnostic replay, outside the default test suite:

```sh
python3 cloud/work/frontier/dot_runtime_a_frame_20261011/trace_frame.py
```

Only parsed safe metadata is retained in `frame_trace.json`; listings and IR
remain temporary. Native words, ROM data, compiler outputs and assembly dumps
are not deliverables.

## Independent review and remaining limits

Independent review on the frozen C hash `f08e35dc2b03a2a404e22b20b6f52bd9a49a59a5b771755d623989a7d784787c`
relinked all six complete STT_FUNC extents through GNU: **168 relocations**,
including **84 in the root**, every word exact. The root is 1,292 bytes followed
by 12 zero alignment bytes. None of the six members owns data. The independent
removal control also regresses only byte offsets 0 and 0x508.

The review replayed 1,024 native/GNU boundary fixtures, including stack memory,
callback traces and saved GP/FP registers: equality, 307 executed root instructions
and 75 branch outcomes. These retain the inherited valid-index/nonempty-search-cycle
assumptions and stubbed FP services. They do not prove arbitrary-memory C safety,
unbounded search termination, complete dependency semantics or actual rendering.
Private-ABI helpers must remain grouped with the genuine kept root.
