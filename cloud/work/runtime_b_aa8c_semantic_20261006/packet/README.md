# Complete AA8C physical-address semantic model

This reconstructs the complete battle-attachment callback and its two genuine
private cleanup functions as executable semantic C. It is **behavioral research,
not a MIPS matching candidate or a recovered original translation unit**.

Base context: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`.
The target closure is A95C (184 bytes), AA14 (120 bytes) and AA8C (7,812 bytes),
8,116 bytes / 2,029 words total. All full targets are authenticated through the
canonical `score.targets()` loader and compared to image B inflated in memory
from the recorded base-commit asset. No image, ROM bytes or disassembly is saved.

## Deliverable and boundaries

`semantic.c` contains the complete root and the two actual static cleanup
helpers, with one s16 player source formal apiece. It has no fabricated keeper,
extra original helper, suffix, export or hidden ABI argument. The original root's
descriptor pointer is represented by a native address in this verification model.
`semantic_bus.h` and `host_bus.c` are expressly host-test adapters, never proposed
original functions or members of an IDO closure.

The first-line O3 header records the required future matching recipe. It is not
an assertion that this bus-based research model has been compiled by IDO or
matched. The host verification build uses the separately reported host compiler
and flags. No matching compilation or compiler tuning has occurred.

The physical bus deliberately avoids declaring a fictional offset array or a
merged model-ID object. Reads use addresses and exact big-endian widths, not C
pointer arithmetic across unrelated objects. This defines the *test model's*
semantics; it does not resolve the original C object identities/effective types.

Unresolved gates before a natural matching candidate:
- Real registration allows owners 0..5. The two known attachment record regions
  have four-record extents. No battle-only 0..3 invariant has been established.
- Mode 8 is real and reads offsets overlapping independently used color/vertex
  data beginning at 80394884. Neither an eight-row nor invented nine-row float
  array is justified. Native unused loads may also be speculative compiler work.
- Model configuration provenance supports 0..12 but is not a whole-program bound
  proof. Modes outside 0..8 can reach a native uninitialized resource index.
- General scene-resource validity, external-service alias envelopes, FCSR and
  whole-program input invariants still need their own proof. This model's
  invalid-mode branch explicitly fails closed rather than defining native UB.

## Full reconstructed behavior

The callback captures the initial player address and source color, handles the
update/inhibit cleanup exit, calculates color/alpha and viewport visibility,
obtains the live parent transform, cleans the previous cached kind, creates the
five kind-0 or kind-1 effects (plus kind-0 extra effect), selects u16 resources
216..223, and performs both complete animation paths.

Important preserved distinctions:
- Cache is a separate signed-byte address; player.mode is a different field.
- Resource 801427C0 is u16. Effect model IDs are s32 in a separate seven-entry
  table. The neighboring ten-entry table is separately initialized in fixtures.
- Group extra creation uses effect slot 6, never slot 5.
- Texture IDs at 80399B08 are packed u16. Bank stride is 8; texture stride is 36.
  A trigger randomizes state139 but assigns texture slot 0 immediately. Later
  ticks increment/wrap state139 and select that indexed texture.
- The two trailing group bytes 138/139 and halfword 13A remain distinct.
- Full-word sentinel comparisons precede removal-argument s16 narrowing. Cleanup
  sentinel stores occur after removal. Only a live extra removal clears 139.
- Descriptor owner is freshly read for transformed destinations. The initial
  player pointer and source-transform pointer stay captured. External calls may
  change what later owner/cache reads see; mutation controls test that ordering.
- Matrix copy comes first; all x-contribution position stores, then all y adds,
  then all z adds follow. Each float multiply/add rounds independently, and each
  intermediate store is retained. In-place source/destination aliases are tested.
- Kind-1 completion is followed by another live cache test for kind 0, not a
  source-level else which silently caches the earlier value.
- Hide/show take the complete s32 handle while color/model/texture/getter/removal
  call boundaries narrow handles. The allocator's transform pointer is retained.

## Verification scope

`native.py` executes the authenticated complete closure with delay slots,
branch-likely annulment, private call ABIs, integer operations, single-precision
FP operations and caller-save poisoning. Unknown/unmapped operations fail closed.
The independent algorithm is the *host-compiled semantic C itself*, not a second
Python translation of its control flow. Both engines share only fixture memory
and explicit external-service effect contracts.

Comparison includes the full mapped nonstack memory, scene-object state, ordered
service calls and relevant input values, RNG consumption, and ordered physical
color/offset reads. Recorded pre-removal state distinguishes before/after stores.
All 61 AA8C call sites plus the actual cleanup-service sites are exercised.
The complete reachable closure is covered. AB8C and C488 are unreachable duplicate
loads: each immediately follows the delay slot of an unconditional branch and
has no direct/indirect-switch incoming edge in this closure.

Fixtures cover modes 0..8, changed/same/previous kinds, owner 0..3 attachment
paths, guarded owner 4/5 paths, model samples 0/6/12, signed fields, alpha clamping,
all cleanup masks, thresholds and neighboring f32 values, both effect timers,
texture wrap, random selections, source-transform aliases, owner changes after
services, and cache changes between animation paths. This is orthogonal coverage,
not exhaustive gameplay or a full Cartesian-product claim.

Scene services are explicit effect models. Matrix copying/scaling are sequential;
position changes use the allocator-retained pointer; RollUV has the native small
angle guard. Allocation, visibility encodings and random generation are modeled
boundaries, and host trigonometry is not claimed bit-identical to N64 sinf/cosf.
This is full callback/native control-flow verification under those contracts,
not execution of all actual game services or proof of complete gameplay.

## Reproduction and portability

From any working directory:

    TMPDIR=/writable/workspace/tmp python3 /path/to/packet/verify.py \
        --reference-root /path/to/SFRush2049-decomp --output /path/to/fresh.json

A host C compiler is required. IDO and a MIPS linker are neither used nor needed.
The focused pytest cleanly skips if the host compiler is missing. The test file
is not hashed into the receipt. Packet hashes bind only owned source/verifier
files; native target words and the recorded base commit bind native/context
identity. No mutable manifest, scorer, accepted source, context or lock state is
pinned. No production/context/lock/tool edits, publication or CI watch is included.

The original entry packet remains unchanged. Independent review and any eventual
natural-C reconstruction are separate gates. The receipt states the actual
fixture count, coverage, negative controls and host compiler provenance.
