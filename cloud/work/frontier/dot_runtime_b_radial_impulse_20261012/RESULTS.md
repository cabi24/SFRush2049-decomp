# Image-B radial impulse: complete private-ABI semantic prerequisite

## Result and scope

New complete C reconstruction of **func_8038D498**, native image B interval
**[0x8038D498,0x8038D798)**, **768 bytes / 192 words**. This missing genuine
E114 dependency previously had no complete C source in the repository.

**NONMATCH semantic research, empty claims, zero new strict-matched,
installable, accepted-image or ROM bytes.** The natural ordinary-ABI diagnostic
source is deliberately not represented as an ABI-correct replacement. It emits
888 bytes with a 184-byte frame; native uses a 104-byte private frame. No fake
parent, optimizer-visible stand-in, inline assembly or forced compiler is used.
The next matching step requires real E114/FCE0 context, as established by the
existing `runtime_b_e114_parent_20261006` closure research.

Base master: `46780d3e85fb79001d64cceef25c868e187265d7`.
Deliverable: `candidate.c`. `group.json` is an empty-claims diagnostic recipe;
its kept root is explicitly the ordinary-ABI view used for analysis, not a
claim about the original linkage. Its complete code and data equal standalone
O3 output. The actual `ovl_splice.packet_spec` entrypoint refuses the recipe
as research only. No installer replacement, ownership migration, production
source, accepted lock, splice or ROM change is proposed.

## Native contract and behavior

The sole direct native caller across game, A and B is **E114 +0x111C**.
At +0x1110 it copies its real record pointer from s5 to **s2**, then saves its
live s5, s6 and f22 around the call. D498 consumes s2 without an O32 a0 formal
and intentionally uses otherwise callee-saved GPRs/FPRs without saving them.
The packet binds the complete D498/E114/FCE0 bodies and the exact argument
move. The behavioral runner sets native s2 and ordinary-C a0 separately;
these explicit entry adapters do not prove native calling-convention equality.

Names such as radius, impulse, source and velocity are descriptive hypotheses.
The proven accesses are:

- Snapshot the source player pointer from signed effect byte +4. Snapshot a
  float at +12 in the 68-byte scene selected by primary object's scene index.
  Values <=8 select magnitude 800 and scale 1; values <=20 select 400 and 0.6;
  larger values select 200 and 0.5. The exact native 0.6/66000/330000 literals
  are authenticated. No whole-function arcade ancestor or original names are
  established.
- Snapshot the primary object's three position fields at +44/+48/+52. Walk
  signed-count players in 952-byte strides. The attachment test uses the
  **loop position** to index the 2,056-byte vehicle array; the hit bit and
  eventual velocity writes use the player's signed **index field**. These
  are kept distinct, including when a helper changes the index.
- Skip the effect owner, attached or inactive players, and already-set hit
  bits. Within the inclusive squared threshold `(field*4)^2`, set the hit bit
  before calling the real damage boundary D3A4. The distance expression's
  addition order is kept explicit.
- Normalize the pre-call delta, apply 330000 and the selected scale, add the
  66000 vertical term, and pass the real three-float impulse to A61B0 with
  the current player's matrix. Reload player index after that helper. Add
  the returned vector to vehicle +0x124/+0x128/+0x12C. Set vector y to zero
  before adding it to +0x13C/+0x140/+0x144; even the zero-add is retained.
- Observe D_801543CA after synchronous calls. Its qualifier is reused from
  five current image-gate-accepted source declarations and independent EB90C
  native read evidence, with source/lock-identity checks. This does not invent
  an asynchronous writer or claim recovery of original qualification.

All three float[3] locals have actual vector uses. No buffer capacity change,
unused pressure local, arbitrary volatility, fake source-level prototype or
fabricated helper/caller is used. Struct opaque bytes represent observed storage.
The native-specific layout assertions check seventeen widths/sizes/offsets.

## Verification

- **2,400 finite valid fixture cases / 4,800 machine executions** compare
  protected native and the complete independently GNU-linked stock C against
  an indexed field oracle. All mapped nonstack bytes and helper-entry state
  snapshots agree. Caller-save GPR/FPR poisoning, ordinary-C callee saves,
  the native retained s2 input, stack pointer and red zones are checked.
- All **190 reachable native instructions** and **221 reachable C instructions**
  execute. Native +0x88/+0xB0 and candidate +0x100 are duplicate constant-load
  instructions bypassed by branch-likely scheduling. They remain included in
  the complete-byte comparisons; they are not ignored or removed from extents.
- **2,400 actual-C LP64 logical-host cases** agree with the same field oracle
  on consumed event arguments and all selected mutable fields. The same
  output matrix passes ASan/UBSan/bounds with recovery disabled and only
  LeakSanitizer disabled. Host pointer layout is not confused with native O32.
- Mutation hooks change the owner, live count, player index, matrix and
  velocity state. These are explicit deterministic helper doubles, not retail
  helper implementations. The damage target's complete native body is bound
  as call-contract evidence but is not executed by this packet.
- Independent GNU linkage consumes all **18 original REL records** from the
  complete unmodified object, verifies every external binding, MIPS2/BE ELF,
  exact 888-byte STT_FUNC, 896-byte text, eight zero suffix bytes, twelve
  literal bytes and four zero literal suffix bytes. Unknown symbols/storage
  and unsupported relocation types are refused. GNU sees **222 full-word
  differences** including the 30 extra nonzero words; no borrowed native tail.
- Candidate literal storage is deliberately placed at a separate diagnostic
  address, 0x80400000. The literal values are verified; no native owned-data
  address/layout match is claimed. Canonical scorer's six unverified data
  references remain visible and are never waived.
- Missing/corrupted literal data, a freshly compiled wrong-magnitude source
  and an unsupported machine instruction all fail closed. The empty-claims
  installer refusal is exercised. No already accepted body is defined or
  duplicated, so there are no preservation-member or coverage additions.
- Stock UGEN diagnostics leave their whole group object byte-identical;
  standalone/group complete text and data are identical. Non-code object
  metadata differs between the two compile routes and is not called identical.
- Protocol-v2 receipts bind owned sources, recipe, verifier, interpreter, host,
  trace and test; selected full targets/helper/callers; relevant symbol rows;
  parsed-word caller census; accepted count-contract evidence and eight stock
  compiler passes. No shared-tool, whole-lock, Makefile or assembly-text pins.
  No static ROM translation unit is rebuilt.

Canonical command:

    python3 tools/cloud/score.py fn cloud/work/frontier/dot_runtime_b_radial_impulse_20261012/candidate.c func_8038D498 --targets asm/us/ovl_b --flags '-g0 -O3 -mips2 -G 0 -non_shared'

Canonical summary (with current-tree authenticated image data in the verifier):

    192/192 words differ (30 extra words (nonzero beyond target length); 6 section-relative relocations unverified: .rodata+0x0 at +0xf4, .rodata+0x0 at +0xfc, .rodata+0x4 at +0x128, .rodata+0x4 at +0x148, .rodata+0x8 at +0x154, .rodata+0x8 at +0x15c)

Replay:

    python3 cloud/work/frontier/dot_runtime_b_radial_impulse_20261012/verify.py --deep
    python3 -O cloud/work/frontier/dot_runtime_b_radial_impulse_20261012/verify.py --deep
    python3 cloud/work/frontier/dot_runtime_b_radial_impulse_20261012/verify.py --sanitize
    python3 cloud/work/frontier/dot_runtime_b_radial_impulse_20261012/trace.py
    REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_b_radial_impulse.py -q -o addopts=''

## Limits and next useful work

Valid backed arrays/records, count 0..4, owner/index 0..3, finite ordinary
geometry and nonzero distance at each hit are the fixture domain. Radius zero
is exercised only without hits. Negative/out-of-range indices, excessive counts,
zero-distance division, exceptional FP/FCSR/NaN payloads, arbitrary aliasing,
concurrent mutation and original source identity remain unproved. Python FP
models round each tested operation to binary32; this is a finite differential
check, not a full hardware arithmetic theorem. Actual gameplay, parent execution,
real helpers, composition/recompression and full-ROM identity remain unrun.

Keep draft. The independent checker owns acceptance, installation and merging.
Independent review, publication and final-head CI are pending.

The initial F568 scout found no new closure lever: its accepted shared-count
qualifier leaves the inherited object identical; correcting the old double
comparison reproduces the known broad structural residual. Stock-identical
tracing showed end-address globals taking s7/s8 and a depth range taking f20,
displacing semantic table/angle values. C910's genuine narrowed creator formal
control was 14/69 versus the inherited ABI-facing 13/69. Neither stalled target
is republished or given credit here. D498 is the new usable context prerequisite.
