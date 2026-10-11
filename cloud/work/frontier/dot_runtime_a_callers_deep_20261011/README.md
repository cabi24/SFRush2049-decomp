# Authentic overlay-A callers recover four complete helper bodies

**928 distinct new candidate bytes**, all complete, relocation-resolved stock
IDO 5.3 matches. This is source evidence, **zero accepted-image or cartridge
coverage gain**. Independent checking and merging remain separate.

| Image-A target | Complete bytes | Prior result | This group |
|---|---:|---|---|
| 80392F28 | 220 | 2/55 | 0/55 |
| 803930B8 | 264 | 65/66 + 17 excess | 0/66 |
| 803931C0 | 276 | 17/69 standalone; over-inlined group | 0/69 |
| 803932E4 | 168 | 40/42 + 6 excess | 0/42 |
| 80393004 | 180 | previously matched | preserved 0/45; no new credit |

The first four entries are the only claims. The native alignment gap after
931C0 is not claimed. Prior E3BC, D494, ED20 and the PR163 address set
93004/A3DA4/A4134/A6A28/AF744 receive no duplicate credit. Only image A is used.

## What provided the missing context

The prior packet supplied complete helper source, but lacked the genuine
large callers. This packet reconstructs both **80394058** and **8039338C** in
full from their protected image-A extents, ordinary control flow and witnessed
data accesses. The only retained external root is **80394058**. There are no
synthetic callers, extra keep roots, unused pressure variables, dummy formals,
assembly, volatile qualifiers, patched tools or instruction substitutions.

Adding the complete 94058 root immediately makes 92F28, 930B8 and 931C0 exact.
Their bodies are verbatim the archived source. Adding complete 9338C supplies
cleanup's second real caller, retaining its native private-register body.
The cleanup's ordinary index loop over 23 records then matches exactly.
No speculative root is needed merely to prevent inlining.

All 29 static calls in native 9338C and all 18 in native 94058 appear in the
same target order in the compiled reconstructions. The author verifier checks
this independently of match claims. Fourteen generated callsites reach the
five helper bodies. The protected image has a fifteenth, from 93B3C, which
remains external to this reconstructed source group. Ordered static calls are
an authenticity cross-check, not proof of complete caller behavior.

## Source and compiler experiments

The initial genuine-94058-only source preserves prior helpers and gives three
new complete matches plus unchanged 93004. Cleanup is still inlined, leaving
an eight-byte retained stub. Full 9338C retains cleanup at 15/42 differences.

Six grounded cleanup/typing configurations were compiled after that:

- Consistent signed availability-byte rows, original pointer loop: 15/42
- Signed bounded record index: 0/42, retained in the deliverable
- Unsigned bounded record index: 0/42
- Pointer less-than end test: 27/42, one excess word
- Separately initialized end pointer: 2/42
- Index bounded by the real array's sizeof ratio: 0/42

Each is an ordinary spelling of the same 23-record traversal. The original
pointer spelling recalculates its end from a retained base each iteration;
the index form lets stock IDO materialize the native fixed end. No separate
end symbol was invented. Workbench diagnosis preceded these changes; its
raw-relocation caveat was retained and is not used as matching evidence.

Both availability rows are signed-byte views because the genuine callers use
signed reads. Mixing the row types introduces a second constant-one register
and regresses the existing initializer to 44/45; consistent row types preserve
it. Producer ownership is clarified to a doubly linked list, with object/owner
links following exactly the native accesses. The selector, initializer,
producer and mode **function bodies remain byte-for-byte the archived C text**.

The only compiler recipe is stock `-g0 -O3 -mips2 -G 0 -non_shared`, canonical
`-Wab,-r4300_mul`, and the repository's unchanged whole-group pipeline.

## ABI and callsite boundary

- 92F28 has no source arguments, uses no frame and preserves O32 callee-saved
  registers. It writes the selected category, optional category-relative byte
  and both two-entry flag rows. Authentic context resolves the two-site
  constant-five register web without editing this body.
- 930B8 has no source arguments. Its 24-byte frame saves its return address;
  it deliberately clobbers s0-s7 and fp without preserving them. Three external
  helpers receive the slot in a0 and return a predicate in v0. It is not a
  standalone O32 replacement.
- 931C0 receives mode in a0, stores it, dispatches complete mode-zero and
  mode-one behavior, and sets the dirty word. Its 24-byte frame saves ra.
  It inherits the producer's private saved-register clobbers and is also not
  a standalone O32 replacement.
- 932E4 has no source arguments; it clobbers s0-s2, saves ra in a 24-byte frame,
  narrows each non--1 record handle to signed16 for the external helper, and
  performs the pointer/flag cleanup. It is not standalone O32.
- 93004's complete existing leaf remains exact. Its count domain and the
  previously documented INT_MIN..INT_MIN+2 compiler-underflow exception are
  unchanged; this packet does not reclassify those exceptional values.

The sole root 94058 saves all integer callee-saved registers for its private
callees. No return-value semantics are asserted for void helpers. The linked
list must be valid and terminating; counts must not overflow; selector and
row indices must be in range; search rows must contain a reachable available
entry. Helpers retain their ordinary declared effects. No concurrency or
arbitrary invalid-state safety is claimed.

## Data and storage views

All storage is external, address-bound data. These functions own no compiler
literal, jump table, newly defined storage or data payload. No such data is
claimed or published.

The compiler-derived layout probe checks 32-bit pointer size, a 16-byte slot
record, the owner byte at +16, node payload at +0, payload next/previous/object
at +0/+4/+8, payload text address +20 and flag word +44, root head/tail +8/+12,
64-byte cleanup records, and the resource label pointer at +708. Opaque fields
express intervening observed offsets, not artificial register-allocation
padding or recovered source names. The 24-byte text member describes the gap
before +44; it does not establish the original string's capacity or lifetime.

Cleanup walks exactly 23 records at 803B5F28 through exclusive 803B64E8 and
updates each word at +4. Producer reads 80156CF0, 8012E6D8 and linked objects,
writing only the four-word 803BA830 table. Mode reads the 13-byte-per-category
availability rows and writes documented state. Symbol names such as
`sound_call_minimal` are legacy address bindings; their spelling alone does
not establish subsystem semantics.

## Verification and reproduction

From the repository root with IDO_DIR and GNU MIPS tools configured:

```
python3 cloud/work/frontier/dot_runtime_a_callers_deep_20261011/verify.py
python3 -O cloud/work/frontier/dot_runtime_a_callers_deep_20261011/verify.py
python3 tools/cloud/score.py group cloud/work/frontier/dot_runtime_a_callers_deep_20261011 --claims --targets asm/us/ovl_a
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/conveyor/test_runtime_a_callers_deep.py -q
```

Author verification authenticates the entire protected target manifest,
extents, tracked compressed asset, loader pointer and image-A identity. It
requires each exact ELF function size, every resolved instruction and no
uncertainty or nonzero excess. It binds source, recipe, tool executables,
scorer, metadata and native/compiled body hashes. Seven compiled negative
controls reject missing 9338C, the earlier cleanup spelling, wrong slot/record
bounds, wrong selector/category or dirty constant, and inconsistent row types.
The full source passes a C89 syntax check; failure guards remain active under
optimized Python. Temporary binary and listing artifacts are not committed.

Timeline (UTC, 2026-10-11): isolated branch created 13:35; first complete 94058
compile yielded the three helper matches 13:37; complete 9338C plus indexed
cleanup yielded all four 13:40; author full-extent checks and controls passed
13:42. Per-build and aggregate replay timings are in verification.json. These
are wall-clock observations, not an instrumented performance benchmark.

## Explicit limits and next genuine context

This is an additive alternative call group. Do not link it alongside another
copy of the same helpers. The original whole translation unit has not been
recovered. The two large callers are complete research reconstructions and
remain **nonmatching**: 9338C is 475/490 (1952 generated versus 1960 native
bytes), and 94058 is 317/323 (1244 versus 1292 bytes).

94058's interior, after a 24-byte prologue shift, has only four mode-register
and four stack-byte-offset differences across native offsets +0x50..+0x4C4.
Its frame is 72 rather than 128 bytes: 48 bytes of missing floating-register
saves plus 8 bytes of non-save frame difference. The authentic initializer
93B3C and renderer 923B0 remain external; recovering their actual effects is
the next grounded context avenue. No fake floating locals or frame padding
were added. The caller audit and offset diagnostic do not upgrade its score.

No whole-caller execution, original-TU layout, full-image splice, deflate,
full-ROM SHA-1, hardware or gameplay proof is claimed. Independent review of
complete helper bodies and bounded semantics is provided separately.
