# Image-A availability setup: genuine context, 15-word residual

## Deliverable and accounting

`group/` is a **research-only, empty-claims** source group for `func_80397FFC`,
complete native interval `[0x80397FFC,0x8039842C)`, **1,072 bytes**. It is not a
replacement for either accepted availability or cleanup group. The current
selective installer explicitly refuses this packet with “no claims (research
only), nothing to splice”; the verifier checks that refusal before compiling.

**Zero new strict-matched, accepted-image, or ROM bytes.** The four diagnostic
preservation members remain strict matches: 94564 (720 B), 94834 (400 B),
949CC (308 B), and 94B00 (728 B). The first three total **1,428 accepted bytes**;
94B00 is **728 inherited candidate bytes from PR #359**, not new credit and
not yet accepted at base master `84d8a4827e7fed06365846e013351b580593d921`.
Every preserved body is listed in the diagnostic manifest and independently
verified; none is proposed for reinstallation. No accepted identity is given a
reduced replacement claim set, and no lock/source is changed.

## Measured causal chain

- Imported #359 kept-97FFC context: canonical **260/268** differing words,
  full GNU **262/268 plus 12 extra nonzero words**, full extent 1,120 B, frame
  104. The canonical target slice truncates a final HI/LO pair and masks two
  shifted literal sites. Its three reported relocation/data errors are saved
  honestly in the receipt; full original-REL GNU reconstruction resolves the
  complete function and establishes the stricter count.
- Add the complete existing genuine **9842C parent** and its already-exact
  949CC cleanup: canonical **176/268**, full GNU **177/268**, 1,032 B, frame 64.
  The absent 40-byte native tail is counted; nothing is borrowed from its
  neighbor. This makes 97FFC internal and removes false o32 callee-save work.
- Add a newly reconstructed **complete real 97AD8 callee**: **31/268** in
  both checks, full 1,072 B, frame 64. Native caller census is exactly
  9842C+0x16c → 97FFC and 97FFC+0x188 → 97AD8. The latter naturally receives
  its single player input at stack+0; native 97AD8 reads the low signed byte
  at incoming stack+3. No fictitious formal, synthetic caller, or empty stand-in.
- Keep the consumed screen-count local used in the three-to-four-screen
  override: **17/268**, frame **72**, all native spill homes recovered.
  Stock UGEN M24/T32/P4 becomes M28/T32/P4, then normal frame alignment adds
  four bytes. There are **no unused locals**, capacity reductions, pad arrays,
  forced registers, invented helpers, or compiler changes.
- Put the final ambient-sound loop body on its own source line: **15/268**,
  full native extent/frame and all **8 owned literal bytes** verified.

Source spellings and variable names are reconstructions. The named count is
not evidence of the original declarations. Reconstructed 97AD8 remains
NONMATCH (200/327 canonical words), as does outer 9842C (443/451); their owned
context data and gameplay semantics are not promoted by the root result.
The existing matched texture lookup declares table-count byte D_80140BDC
volatile. That actual declaration, pinned as a consumed source input, is
preserved in the callee; arbitrary new volatility was not introduced.

## Residual diagnosis and stop

Workbench diagnosis and stock UGEN layout distinguish the home, color and
schedule effects. Instrumented uopt with no force is byte-identical to the
complete stock object. In root procedure 5, the relevant phase-one choices
are screen-value web 46 → v0, consumed local web 44 → v1, and address web
79 → a0. The legal three-color diagnostic
`p1:w46=c3,p1:w44=c2,p1:w79=c1` reaches **6/268** in independent GNU linkage.
Those remaining offsets are +0x174/+0x178/+0x17c/+0x180/+0x184/+0x18c:
the same byte store and stack spills in a different order before/inside the
97AD8 call. **The forced object is not a match or accepted deliverable.**

Bounded natural experiments included consumed count width/reuse, a genuine
layout pointer, direct-global vs local conditions, switch form, source line
boundaries, named call results, and equivalent narrow parameter spellings.
None closed the color/schedule pair. The final source retains only the useful
count and loop-line changes. No broad random sweep or ineffective shaping is
published. A next useful step is stock scheduler/priority tracing at the
97AD8 call boundary, not another frame-capacity search.

## Proof and limits

`verify.py` binds source/recipe/baseline/own verifier/trace/host/test hashes,
selected complete target words and extents, exact caller bodies and consumed
symbol addresses, both owned root literals, and all eight stock IDO passes.
It does not pin shared scoring/tools, lock files, Makefile, or assembly text.
No static ROM TU is rebuilt. The imported #359 baseline is self-contained;
replay never reads a non-master-ancestor Git object.

For all five causal stages, an independent ELF reader checks every defined
function's full STT_FUNC extent, next compiled symbol and zero suffix. GNU
rebuilds the complete selected bodies and **every original REL record in those
bodies**, preserving exact function-entry call destinations and rebasing the
root's whole compiler-owned 8-byte literal slice. Final proof checks **213
text relocations**, zero unresolved GNU state, exact allocation set, every
instruction word and both whole literals. Unclaimed context data are not
included in that GNU acceptance comparison. The receipt records full missing
and excess extents for controls rather than padding/cropping them.

Instruction corruption, corrupt/missing root literals and an overlong function
extent are rejected. Normal and Python `-O` proof pass. The focused test takes
about 0.6 seconds with IDO and cleanly skips when IDO is absent after verifying
all bound inputs. Canonical group scoring correctly returns NONMATCH.

Both exact C root bodies, unchanged by the host generator, run over **20,480
logical-host cases**: counts 0–4, all 16 owner-presence masks, 16 table/height
seeds, and 16 positive divisors. Complete helper-event traces and root state
agree. The same matrix passes ASan/UBSan. Helper implementations are explicit
deterministic doubles and memory is stable: this does not execute real retail
helpers, prove arbitrary aliasing/concurrent mutation, pointer-width layout,
exceptional division, model-slot semantics or gameplay. The real 97AD8 context
and 9842C parent are independently unverified reconstructions; no hardware,
image composition/compression or full-ROM gate was run.

## Reproduce

From the repository with IDO 5.3 and GNU MIPS binutils configured:

```
python3 cloud/work/frontier/dot_runtime_a_availability_root_20261012/verify.py --deep --score
python3 -O cloud/work/frontier/dot_runtime_a_availability_root_20261012/verify.py --deep
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_a_availability_root_research.py -q -o addopts=''
python3 cloud/work/frontier/dot_runtime_a_availability_root_20261012/trace.py
```

Optional `trace.py --tracer PATH` checks entire-object fidelity, the exact
three-color diagnostic, and the six remaining GNU differences. Nothing from
the diagnostic compiler is used for the stock result or installation.
