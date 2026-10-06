# Runtime-B genuine closure: proposed assembly map

Status: the map and single combined-context diagnostic were approved by the
coordinator. The approved baseline has now been assembled and compiled once;
see README.md and source-bound receipts for its result. The map below records
the evidence and declared uncertainties that preceded that decision.
No active author packet, production source, tool, target, lock, header, or
current portability matrix was edited. This is NEXT-batch research.

Base: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`.
Target: image B. Eight full native bodies total **11,376 function bytes**.
`inventory.json` records complete target identities, source snapshots and a
fresh authenticated direct-call census. Source receipts are inspected historical
evidence; their semantic fixtures were not rerun for this map.
The three older child bodies have an ordinary O2 receipt; it must not be
relabeled as O3 or treated as a directly comparable private-context score.

## 1. Real roots, private boundaries and signatures

| Member | Native bytes / frame | Native live input | Meaningful output | Proposed visibility |
| --- | --- | --- | --- | --- |
| FCE0 | 3056 / 208 | No consumed formal/home; globals | All side effects; return ignored | Genuine external root |
| E114 | 5196 / 288 | No consumed formal/home; global list | All side effects; return ignored | Private static hypothesis |
| F938 | 928 / 56 | s0 status, s2 player, s3 vehicle, s5 signed owner | Status, alpha/transition, effect updates | Private static hypothesis |
| DA78 | 868 / 144 | s1 record, s2 previous-position pointer | v0 player pointer, clipped position/flags | Private static hypothesis |
| D498 | 768 / 104 | s2 record | Signed hit-mask byte; damage and vehicle impulses | Private static hypothesis |
| D200 | 296 / 24 | s1 record, a0 copy mode | Primary object's position/matrix | Private static hypothesis |
| D328 | 124 / 24 | s1 resource, s2 parent, s3 flags, s4 omit transform | v0 allocated 60-byte object pointer | Private static hypothesis |
| E088 | 140 / 24 | s1 record | Scene destruction and object-pool releases | Private static hypothesis |

FCE0 saves/restores s0..s8 and paired f20..f31. Every private member saves ra
only. E114 writes unsaved s0..s8/f20..f30. F938 writes s1/s3/s4/s6/f20.
DA78 writes s0/s3..s8 and f20/f22/f24/f26/f28/f30. D498 writes s0/s1/s3..s8
and those six float lanes but preserves incoming s2. D200 writes s0/s2;
D328 and E088 write s0. These are observed body contracts, not manually
encoded compiler clobber constraints.

Fresh main-image/B direct J/JAL census:

- main `render_viewport_init` calls FCE0 at `800FAA14`.
- FCE0 calls F938 at `8038FE8C` and E114 at `80390878`.
- E114 calls DA78 twice, D498 once, D200 seven times, D328 eight times and
  E088 twice. There are no other direct callers in those scanned populations.
- No literal pointer word equal to these entries was found inside scanned
  function text. This does **not** exclude HI/LO-built addresses, data tables,
  indirect calls, other images or other source-visible exports.

These facts justify evaluating a private-boundary hypothesis with the real
FCE0 root. They do not establish original C `static`, source-file boundaries,
formal order, or the exact original export list. Especially, F938's four live
inputs are genuine, but status aliases `&player->status`, vehicle is selected
by player owner, and owner itself is available in the player. The four-formal
semantic interface is not proof of the original declaration or parameter order.
The same distinction applies to DA78/D200/D328 parameter order. Do not remove,
add, permute or retype them for register allocation.

DDDC and F568 are conventional boundaries and stay external. D3A4/D798 have
main-image callers; they are not private context to pull in for pressure.
CB20 and D054 also stay real external services. D054's third pointer is homed
at entry SP+8, so FCE0's actual duplicate position argument must be retained.

## 2. Concrete shared-schema proposal

`shared_schema.proposed.h` is the proposed C89 schema, declarations and private
prototypes. It contains no implementation, keeper, stub or test caller.
Before an approved compile, a separate layout-only check must establish all
sizes/offsets under the target compiler, including 4-byte pointers. No host
64-bit `sizeof` result is target-layout evidence.

### Record/object/effect views

- BRecord is 104 bytes: next +0, signed owner +4, signed hit_mask +5,
  signed kind +6, signed flags +7, lifetime +8, timer +0xC,
  collision_extent +0x10, velocity +0x14, position +0x20, previous +0x2C,
  matrix +0x38, attached_effect +0x5C, primary +0x60, secondary +0x64.
- `unknown05` in FCE0/E114/children is D498's signed `hit_mask`. FCE0 writes
  zero, so naming/type reconciliation preserves that store. D498 must retain
  sign extension before its 32-bit hit test, masked shift count, and byte
  truncation when storing the mask. Do not silently use u8 semantics there.
- `elapsed` (FCE0/children), `collision_extent` (E114/D498) and `radius`
  (DA78) name the same float at +0x10. Unify as collision_extent, with no
  arithmetic or initialization changes.
- BObject is the common 60-byte object: scene index +4, matrix +8,
  position +0x2C. BDebris is a distinct 72-byte record, despite a similar
  prefix; lifetime +0x38 and velocity +0x3C remain intact.
- **Incompatible homonymous types:** F938's BEffect is a 52-byte status-effect
  record (scene +0, matrix +4, position +0x28). E114's BEffect is a 28-byte
  consumed attached-effect prefix (flags +8, position +0x10). Rename to
  BStatusEffect and BAttachedEffect. Never merge these layouts. The latter's
  28-byte view is not its actual allocation size.

### Player/vehicle views

- BPlayer retains stride 952 (0x3B8). Merge only evidenced fields:
  position +8, velocity +0x14, matrix +0x2C, active signed +0x308,
  color word +0x34C, blocked signed +0x359, owner signed +0x35B,
  input +0x380, kind/ammo signed +0x384/+0x385, status +0x38C,
  action signed +0x3A0, alpha unsigned +0x3A1, transition signed +0x3A2,
  latched word +0x3A4, transition_time +0x3A8, cooldown +0x3AC,
  pitch/yaw +0x3B0/+0x3B4.
- BStatus is 20 bytes. Replace `values[4]` with F938's four evidenced floats
  transition_timer/scale_timer/scale/pitch, at +4/+8/+0xC/+0x10.
- BVehicle retains stride 2056 (0x808): model unsigned +8,
  extent_fc +0xFC, extent_108 +0x108, impulse +0x124,
  horizontal_impulse +0x13C, blocked signed +0x640, radius +0x654,
  state signed16 +0x6C4. Fill only previous unknown ranges; do not add
  fields or padding beyond the native stride.
- Scene stride 68 and scale +0xC agree across E114/D498. BInput, texture and
  texture-bank layouts are retained unchanged. BColor preserves big-endian
  byte-3 alpha semantics; bytewise host tests must not assume host endianness.

### Quad release is an actual pointer lifecycle

E114 stores the A78BC result at BRelease+4. At the pinned base, accepted
`src/blob/groups/frontier_skid_marks/func_800A78BC.c` returns Record88*.
Accepted `src/blob/func_8008D0C0.c:3798` takes s16*, clears that record's
active halfword, then trims the tail by 88-byte records.

The proposed BRelease field is opaque BQuad* and the existing FCE0 logical
`handle` read becomes `quad`; the call is `func_8008D0C0((s16 *)release->quad)`.
This preserves the entire pointer word and known helper contract. It does not
narrow to a scene index, fabricate a return, or claim original pointee spelling.
The map keeps BQuad opaque rather than importing extra quad internals.

## 3. Globals, byte aliases and object-boundary limits

Shared globals with identical scalar/access contracts:

- D_8002EB94 f32 tick, D_80142764 f32 gravity, D_801543CA signed16 count.
- Players D_80152818 (stride952), vehicles D_8014A250 (stride2056),
  scenes D_8012E700 (stride68), texture banks D_80151AE8 (stride8).
- D_803943A4 uses kind*156 + model*12 + component*4. D_80394B08 uses
  kind*12 + component*4; D_803942C0 uses kind*4. Keep the existing finite,
  valid-index domains and table slices. Original declarations are unproven.

Exact physical aliases that a combined linker and behavioral memory model
must preserve, without inventing a shared original C aggregate:

- D_8039A530 equals record pool D_8039A520 +0x10 (active head).
- D_8039AE90 equals debris pool D_8039AE80 +0x10 (active head).
- Release pool D_8039A5F8 +0x10 is the head at 8039A608.
- D_80399B54 is 15 words beyond D_80399B18. F938 reads the former scalar;
  retain it as a separately spelled symbol and bind its address faithfully.

The initializer writes ten contiguous resource words and then seven more.
D328's native indexed-base access and actual E114 callsites establish indices
1,2,3,4,5,7,8,9; FCE0 additionally reads slot0. They do **not** establish one
original 17-element C array. Standalone tests across indices0..16 are synthetic
coverage of a logical storage view, not additional caller evidence. The proposed
extern resource declaration is unsized; no `sizeof` depends on the bound.
Keep all existing reads and preserve separate B54 relocation identity.

Pool declarations are consumed prefixes only. Root reads head+0x10; real pool
helpers additionally access free+0x14. Do not allocate only sizeof(BPool) for
external fixtures. F938's status-effect count4 is supported by its initializer;
a whole-closure case that invokes F938 must use owner0..3. FCE0-alone tests of
larger owners do not automatically extend that domain.

## 4. External helper contracts and discrepancies

### Consumed-return discrepancy: E398

FCE0, F938 and D328 consume full v0 from 8008E398 as a scene-index word.
The accepted `src/blob/groups/dot_sign_extend/group.c` declares its wrapper
void, although it calls 8008E26C, whose accepted source returns signed16 index
extended to s32. The native wrapper leaves v0 intact. Use the return-bearing
research declaration already proved in these packets; leave accepted source
and generated headers untouched. This is an integration blocker, not permission
to alter protected declarations here. The parent argument is full-word at the
wrapper boundary and narrowed inside it, not before the wrapper call.

### Genuine returns presently ignored by caller adapters

- 800B61A8's accepted source returns s32 and takes u8 as its fourth input;
  root/update currently declare void and s32. All actual closure calls pass1
  and ignore its result. Proposed declaration restores s32/u8 without adding
  a use or changing argument value.
- 800B24EC's accepted source returns NameEntry*, accepts char*, s16* output,
  signed8 low/high, s32 error flag. E114's current semantic declaration uses
  void return, u16* output and s32 low (always0). Proposed opaque BNameEntry*
  preserves the unused result and exact byte input contract; use explicit
  `(char *)D_80394D08` and `(s16 *)&D_80394F88` at the existing call. The
  output remains the same 16-bit resource representation, consumed unsigned
  by A78BC. No new output dependency is introduced.

### Coherent logical views, not consumed-return bugs

- Allocation returns void* from its pool. D328's BObject* return view and
  root/update void* declarations must share the void* allocator declaration.
  Free takes pool plus node pointer; use void* node across record types.
- `sound_call_minimal` and `func_80090254` are the same address; normalize to
  one address-spelled declaration and retain signed16 narrowing at every call.
- Matrix pointers spelled f32* in accepted source and f32[3][3] in research
  describe the same row-major address and ordered accesses. Color s32*/u32*
  views describe the same four-byte word. Texture integer/pointer spelling in
  accepted 8008D870 is an address adapter. Record these differences; no need
  to import mismatching generated global headers into the research TU.
- A78BC returns an opaque record pointer, never a scene integer. DA78 and
  D328 also have consumed pointer returns; all must survive natural linking.
- D054 returns an attached-effect pointer and homes its third argument.
- ADD58's collision result is consumed. 8008B2E4/8008C768 return f0. Other root/
  update callback returns are unused; exact unimplemented internal service
  bodies and all hidden side effects remain outside standalone proofs.

## 5. Proposed one-shot assembly and verification, after approval

1. Verify the frozen D498 packet and all six source snapshot hashes. Keep all
   source packets unchanged. Assemble one new research source by taking each
   complete body, normalizing only the documented names/types/call spellings,
   and adding the approved schema. Preserve every statement and expression
   order, every meaningful return, and the lazy kind7 radius behavior.
2. Use native-address definition order D200,D328,D498,DA78,E088,E114,F938,FCE0.
   This is a declared natural baseline choice, not claimed recovered source
   order. Seven real helper definitions are static hypotheses. FCE0 is the
   only external/kept root. No fake E114 root, keeper caller, dummy parameter,
   noinline attribute, output discard, register constraint or padding local.
3. Use the canonical `score.compile_group` with one real combined C input,
   keep list `[func_8038FCE0]`, flags exactly `-g0 -O3 -mips2 -G 0 -non_shared`.
   Record the route's mandatory as1 `-r4300_mul`; do not patch or bypass tools.
   A separate diagnostic layout object is not optimizer context. No alternate
   per-file route or recipe is silently substituted.
4. Inventory the complete ELF: every defined/undefined symbol, emitted body,
   zero/nonzero padding, allocated section, relocation, local constant and
   jump-table byte. Let natural inlining/elimination happen and report it;
   do not invent kept roots to recover missing native boundaries. Compare all
   eight full targets and disclose absent/inlined members, short/excess bodies,
   unresolved and owned-data results. No favorable subsequence/suffix score.
5. GNU-link the entire unmodified ELF at an explicit research placement and
   resolve every external and owned-data relocation. This gives a semantic
   executable, not native-placement equality. Whole-target strict scoring and
   own-data relocation proof remain separately required for any match claim.
6. A closure semantic test must execute real native and candidate child
   bodies reached from FCE0. Existing standalone verifiers intercept private
   calls with contract hooks; their fixture totals do not compose into a
   closure proof. Hook only genuinely external services. Preserve real
   caller/callee stack/return state and root nonvolatile restoration.
7. Combined fixtures must reconcile global byte aliases, all four pools and
   list lifetimes, record allocation before E114, status-effect owner bound,
   full scene-pointer/index distinction, signed hit masks, callback reloads,
   bounded successful D328/quad-node allocations, and D498's nonzero distance
   domain. Prior standalone large-owner or adversarial mutation cases may
   violate another body's domain; explicitly intersect/revalidate them.
8. Stop after this baseline. Structural nonmatch is a result, not authority
   for source-order/signature/type sweeps. Report original TU/visibility and
   source-order uncertainties without a matching or accepted-byte claim.

Temporary compiler work must use a unique TMPDIR and default deletion. Retain
only sources, metadata, scores and necessary verification summaries. No full
clone, native bytes, raw assembly, compiled-object publication, production
edits, PR publication or CI watcher is part of this assignment. Future
publication requires the coordinator's current-master full suites with and
without IDO and independent review, in a later batch.

## 6. Approval questions / remaining ambiguities

- Approved: the evidence-derived shared schema and explicit BQuad pointer
  lifecycle, while leaving original object/array identity unclaimed?
- Approved: restoring unused external return declarations for B61A8/B24EC
  from pinned accepted-source evidence and retaining E398's research-only
  return-bearing contract?
- Approved: the seven-static/one-kept-root visibility as a **diagnostic
  hypothesis**, not original-TU proof, with the existing logical argument
  order unchanged. Approval is for one baseline, with no subsequent shaping
  or signature/order/pressure sweeps without new evidence.
- D498 author freeze arrived during this map: visual.c SHA-256
  f97e37fdbb701210bda355121ce988bd146eb274475645c827c3431e64a2bcea,
  1,658 paired cases, 18 layout facts, 14 wrong-source and three fail-closed
  controls; source is 920 bytes under canonical O3 versus 768 native.
  Independent review remains owned by its reviewer. Recheck at assembly time. Standalone receipts are evidence inputs, not a freshly
  composed semantic guarantee.

Reproduce the read-only inventory:

    PYTHONDONTWRITEBYTECODE=1 python3 runtime-b-closure-map/audit.py \
      --reference-root rush-sixth-baseline

This command performs no compiler invocation and emits only JSON metadata.
