# AA8C service effects and alias contracts

Read-only research against `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`.
This packet executes actual native scene-service bodies in a bounded interpreter
and records source-compatible interfaces. It is not a full AA8C execution,
compiled matching candidate, original declaration recovery, or gameplay proof.
No production source, context, locks, protected tools, or native assets changed.

## Most consequential findings

1. Scene creation retains its transform pointer; it does not copy 48 bytes.
   Later position/matrix updates write through that pointer. Effects therefore
   cannot all be modeled as isolated changes to a scene-object dictionary.
2. The object-creation wrapper has a real integer result: its allocator returns
   a sign-extended 16-bit handle in a 32-bit register. Generated `void` declarations
   are not valid evidence for the AA8C uses of that result.
3. Hide/show have a wider handle contract than the other scene setters: their
   native flag read sometimes sign-narrows the handle while the paired write
   uses the full input. AA8C intentionally supplies full-word handles to these
   functions and signed-low-half handles to color/texture setters.
4. AA8C's transform writes are a three-pass, single-precision sequence with
   intermediate destination stores and repeated owner reads. A mathematical
   dot-product rewrite is not an alias- or rounding-preserving substitute.
5. The five packed texture IDs are unsigned halfwords. A randomized initial
   texture-frame byte does not select the initial texture: that assignment
   explicitly reads texture slot zero.
6. Mode-8 native addressability does not prove a ninth float source row. A
   verification-only physical-address bus avoids manufacturing such an object;
   it does not recover the original external C object/alias declarations.

## Native scene records and service interfaces

The service record address is `0x8012E700 + handle * 0x44`. Relevant fields are
flags +0, auxiliary word +4, retained transform pointer +8, two scalar words
+0xC/+0x10, unsigned model ID +0x14, child/sibling/link halfwords
+0x16/+0x18/+0x1A, four texture words +0x20..+0x2C, and color +0x3C.
Names describe observed uses, not recovered original struct identity.

### Creation and deletion

- `sign_extend_call` at `0x8008E398`: source-compatible interface
  `s32 (s32 model, Transform *transform, s16 parent, u32 flags)`.
  The wrapper sign-narrows only a2 and calls `func_8008E26C`. a0/a1/a3 are
  otherwise forwarded unchanged. It returns the allocator's v0 unchanged.
- `func_8008E26C` at `0x8008E26C`: scans live count `0x80156990` for a model-ID
  halfword equal to `0xFFFF`, otherwise extends the count. It updates peak count
  `0x801569A8`, stores flags/full transform pointer/model low16, initializes the
  two scales to 1.0f, sets the three links to -1, zeros words +0x1C..+0x3C,
  and calls `render_mode_select(index,parent)`. It does not read transform
  contents or write through that pointer. Native evidence includes pointer
  store `0x8008E330`, halfword model store `E334`, and the sign-extended return
  saved at `E36C`, restored at `E388`.
- `render_mode_select` at `0x8008E19C` appends a signed-16 handle either to the
  root chain headed at `0x8015B254` (negative parent) or a parent's child chain.
  `func_8008E144` walks sibling halfwords to find the end. There is no callback.
- `sound_call_minimal` at `0x80090254`: `void (s16 handle)`. It calls
  `entity_spawn_callback(handle,0,0)` at `0x8009026C`; the historical names are
  not audio or callback semantics. This entry skips child/sibling recursive
  deletion, unlinks the node using the real graph scanners, stores model-ID
  `0xFFFF`, resets child/sibling links, and reduces the live count when removing
  the trailing free extent. It does not free, clear, copy, or mutate the retained
  transform. It does not invoke a user callback. This is why private AA8C cleanup
  may preserve all transform bytes while setting handle sentinels after returns.

These routines have no general capacity, invalid-index, broken-chain, or
arbitrary-pointer safety guarantee. Reconstructing their actual side effects
does not establish that owners 4/5 or truncated invalid handles are legal.

### Pointer getter and setters

- `func_8008B3A0` at `0x8008B3A0`: `Transform *(s16 handle)` is source-compatible.
  Native sign-narrows a0 and returns the word at record+8 in the return delay
  slot at `0x8008B3C4`. There are no writes apart from argument homing to stack.
  Treat the returned value as the actual live pointer, not a copied transform.
- `func_8008D6FC` at `0x8008D6FC`: source-compatible
  `void (s16 handle, const f32 *position, const f32 matrix[3][3])`.
  It loads the retained pointer at `D728`. If position is non-null, three
  sequential load/store pairs write destination +0x24,+0x28,+0x2C at
  `D72C..D740`. Only afterward, if matrix is non-null, it calls `math_utility`
  at `D74C`. A position write can therefore change a subsequently read matrix
  source if the supplied ranges overlap. Pointer identity is retained across
  the whole service call.
- `func_8008E06C` at `0x8008E06C`: `void (s16 handle,const u32 *color)`.
  One full-word source read at `E080` precedes one full-word scene+0x3C write
  at `E094`. AA8C passes pointers to local color words; a verification service
  taking a color by value must adapt this ABI explicitly.
- `func_80090770` at `0x80090770`: `void (s16 handle,u16 model)`.
  It sign-narrows the handle and stores only the low model halfword at record
  +0x14 (`0x80090798`). AA8C's resource table is independently read by lhu.
- `func_8008D870` at `0x8008D870`: `void (s16 handle,void *texture,s32 slot)`
  is source-compatible, with texture an opaque 32-bit pointer value. A negative
  signed full-word slot assigns all four texture words at +0x20,+0x24,+0x28,
  +0x2C (`D89C..D8AC`). A nonnegative slot writes only +0x20+slot*4 (`D8CC`).
  No range clamp exists. AA8C uses exactly slot -1 at its two texture calls.

### Visibility

`model_data_load`/hide at `0x8008AE8C` and
`model_transform_setup`/show at `0x8008B0D8` are source-compatible with
`void (s32 handle,s32 mode,u32 viewport_mask)`.

For the AA8C call modes, hide mode 0 ORs bit 31 into the root flags; hide mode 1
ORs `viewport_mask<<8`. Show mode 0 clears both bit 31 and the selected viewport
bits. They do not write transforms or call arbitrary code. Other documented
modes traverse child/sibling graph links, but AA8C's used root-only modes require
no traversal. Do not replace full-word native handles at AA8C animation calls
with an unconditional signed-halfword argument conversion. On valid allocator
outputs the full word already is a signed-16 value; this is a domain fact, not
permission to change wider-input behavior.

## Math, mutation ordering, and floating limits

- `math_utility` at `0x8008D6B0` reads and immediately writes offsets 0,4,...,32
  in order. `src==dst` is valid. A partially overlapping destination sees the
  earlier writes on later reads. It is neither an all-at-once snapshot nor
  memmove. The runtime checks include forward and backward overlap.
- `func_8008B32C` takes `(matrix_src,matrix_dst,f32 scale)`, with the third float
  arriving in a2 bits. It sequentially multiplies each of the nine floats into
  its destination and never writes xyz. AA8C explicitly uses it in-place at
  `BDE4`, `BF00`, `C830`, and `C8DC`. The final call scales the extra matrix using
  live group+0x13C after all other animation work.
- `func_8009EA68` takes `(f32 angle,matrix)`. It returns without calling trig or
  writing unless angle is strictly less than -0.0001f or greater than 0.0001f.
  Quiet NaN fails both comparisons. It calls sinf first, cosf second. For each
  column, it computes from the old row0/row1 pair, writes
  `old0*cos - old1*sin` to row0, then `old0*sin + old1*cos` to row1. The third
  row and xyz are unchanged. The actual stores are `0x8009EAF0` and `0x8009EAFC`; the
  load/call range is `0x8009EA68..0x8009EB10`. Do not run the effect unconditionally
  at zero/tiny angles in a boundary fixture.
- `func_8008B2E4` at `0x8008B2E4` accepts and returns f32. It contains its RNG
  update directly, with no external call: seed at `0x8011735C` becomes
  `seed*0x41C64E6D+12345` modulo 2^32; bits 16..30 produce the nonnegative
  integer. The native order is conversion to f32, multiplication by max, then
  division by 32768.0f. The seed write precedes the floating conversion. A
  shared deterministic random boundary supports AA8C control-flow comparisons,
  but does not validate this actual RNG state or exceptional FP behavior.

AA8C's repeated transform regions copy the matrix, then execute:

1. All three xyz assignments: `offset.x * matrix[0][j] + position[j]`.
2. All three updates: `offset.y * matrix[1][j] + destination_position[j]`.
3. All three updates: `offset.z * matrix[2][j] + destination_position[j]`.

Each multiply and add is single precision and each update is stored before the
next component. Examples are mode-0 creation `0x8038AEB8..0x8038B0C0` and mode-1
refresh `0x8038B9C8..0x8038BBD0`. Fresh signed-halfword owner reads occur between
stores even when there is no external call; their relevance is broader than
callbacks. Never add restrict, immutable-owner assumptions, source/destination
disjointness, or multiply/add reassociation without separate evidence.

The scene-only setters/remover/allocator inspected here do not directly target
descriptor.owner, player mode, physics model, or attachment-cache storage. On
valid disjoint records they do not mutate those fields. However, pointer-based
matrix/position services and AA8C's own transform writes can alias arbitrary
mapped state unless a stronger pointer-provenance/domain invariant is supplied.
The bounded helper callgraph is closed and contains no indirect callback call.
That is a narrower and better-supported statement than whole-program non-aliasing.

## Texture and mode-8 address details

At `0x8038C4E0`, the live texture-frame byte at group+0x139 is read as signed,
used with stride 2, and the ID is loaded unsigned-halfword from `0x80399B08`.
Low 10 bits select a 36-byte texture record; high bits select an 8-byte bank
descriptor at `0x80151AE8`, whose first word is the texture base.

During the trigger path, Random(5) initializes group+0x139 at `C664`, but `C66C`
still reads texture slot 0. A later animation tick increments/wraps that byte and
uses it for the next texture. group+0x138 is a separate lifetime-step byte;
cleanup A95C clears +0x139 only for a live extra handle.

Mode 8 addresses start at `0x80394884`, the independently loaded source-color
word. Both changed-mode and same-mode paths perform their three offset reads.
The separately established selector packet remains the authoritative domain
audit: four-record storage does not prove all registrations are owner 0..3;
normal model producers do not close every possible model-byte producer.

Neither an incomplete float-array declaration nor an unsigned-char cast proves
that all these linked addresses belong to one C source object. A cast/union or
volatile qualifier cannot by itself establish original extent, effective type,
or alias ownership. Likewise, unused native loads might be compiler speculation;
they alone do not recover the original source declarations. A verification bus
can preserve physical memory traffic with its own documented contract while
leaving the original C object boundary explicitly unresolved.

## Reproduction and limits

    python3 verify_native_effects.py --reference-root /path/to/repository
    SFRUSH_REFERENCE_ROOT=/path/to/repository python3 -m pytest test_packet.py -q

The verifier reads base-commit native sections and checks them against canonical
live `score.targets()` words. It also binds the complete 7,812-byte AA8C target
and checks its 61 direct calls / 14 destinations. It does not pin the changing
scorer, manifest, symbols file, live locks, production C, or this pytest file.
It emits metadata only, never native bytes or disassembly.

The 61 passing bounded service calls cover 480 native instructions, including
creation reuse/extension and parent/root linking, actual removal/unlinking,
setter signed-handle and model-halfword widths, position-before-matrix aliasing,
retained transform identity, sequential overlapping copies, and AA8C's used
visibility modes. Unmapped indices and unknown opcodes fail closed. There is
no exhaustive scene-graph, arbitrary-state, FCSR, trig, RNG, or full AA8C proof.

The direct replay passed both normally and from a foreign working directory with
an intentionally absent IDO path, yielding identical receipts. Focused pytest
passed after loading the existing dependency environment (1 test). No compiler
is needed, so no compiler-dependent test is unguarded. Full integration suites,
publication, and CI monitoring were outside this audit's scope.
