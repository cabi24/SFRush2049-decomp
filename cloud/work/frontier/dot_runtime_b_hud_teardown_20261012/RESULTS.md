# Image-B HUD teardown: four-word NONMATCH research

## Deliverable and accounting

`candidate.c` reconstructs the complete **func_8039244C**, image B, native
interval **[0x8039244C, 0x803925C8)**, **380 bytes / 95 words**. Stock IDO 5.3
now differs in **4/95** words, versus the current-master baseline's **34/95**.
Both have the full 380-byte extent and native 64-byte stack frame. There are
no excess words, unknown references, unverified sites or owned-data bytes.

The source-first research recipe has **empty members and claims**. Current
`ovl_splice.packet_spec` rejects it as research-only; that refusal is exercised.
No source is installable from this packet. Zero new strict-match, accepted-image,
gameplay-oracle or ROM bytes. The already accepted 48-byte `sound_call_minimal`
wrapper is recompiled unchanged and remains exact, with no repeated credit.

Base master: `46780d3e85fb79001d64cceef25c868e187265d7`. The predecessor is the
current-master `cloud/work/runtime_b_teardown_20261006/candidate.c`, not a
historical checkout or unmerged dependency. All target words are read from
protected, hash-checked population inputs. Source, tests and research only;
no production source, accepted lock, native target or shared tool changes.

## New causal evidence

The prior attempt stopped at a mixed scheduling/register residual. Workbench
was run before reopening. Its raw relocation-display constants are not real
constant errors: complete native-address linkage resolves those independently.
The meaningful differences were early state/handle ordering, the live count's
register class, an inner-loop initializer order and first/second pointer colors.

The native body loads the next record's handle after the preceding helper call
but before the preceding state store. A consumed signed-halfword `handle`
carrier states that ordering directly without imposing a false helper prototype.
The record arrays are distinct; no intervening helper can change the next handle
between the new load and its call. The explicit signed `count` carrier records
the observed post-helper player-count reload. Counter initialization precedes
the inner effect-pointer assignment as in native. Both added carriers are used.

The decisive new lever was the first pointer's **guarded initialization**.
The inherited source initialized it before the signed-positive player-count
guard. Instrumented, stock-identical uopt shows that web at priority **8.5**
across six live blocks, versus the second pointer at **10.2** across five.
Moving the assignment into the genuine guarded region gives both priority
**10.2**, five blocks. Earlier first use now wins s5; the second pointer gets
s6. This naturally reproduces the native allocation. A prior force-only
swap confirmed the color component before the source change; the forced
object was never retained or used as acceptance evidence.

Three independent initializers share one source line. This makes stock as1
schedule their address halves in the native order. That is explicitly disclosed
statement-line shaping. All other effective changes describe real values and
lifetimes. No unused local, changed array capacity, arbitrary padding, invented
caller/formal/helper, extra volatile, artificial read, inline assembly or
modified acceptance compiler is retained. Original source names and spellings
remain unknown; no complete arcade donor was identified.

Replayed causal controls:

- Original current-master source: **34/95**, 380 bytes, frame 64.
- Final source with first-pointer initialization moved outside the guard:
  **17/95**, same extent/frame. The pointer colors reverse.
- Final source with the three initializers on separate lines: **6/95**, same
  extent/frame. The two address-scheduling differences return.
- Final source: **4/95**, same extent/frame. An ordinary whole-program O3
  recipe emits exactly the same complete body as the standalone O2 build.

## Exact stopping point

Canonical current scorer, without `--claims` or `--allow-unverified`:

```
python3 tools/cloud/score.py fn \
  cloud/work/frontier/dot_runtime_b_hud_teardown_20261012/candidate.c \
  func_8039244C --targets asm/us/ovl_b

func_8039244C:
  4/95 words differ
```

The scorer also prints its four detailed differences. They are summarized here
without publishing a native assembly dump:

- **+0xa4 and +0xc0:** equal/not-equal branch operand order is reversed. The
  complete tested semantics agree, but instruction words do not.
- **+0x134 and +0x138:** the computed loop endpoint uses t0 rather than native
  v1, affecting its addition and the pointer comparison.

Stock uopt's final trace has first/second pointers in s5/s6 and the consumed
count in v0. The remaining endpoint expression is rematerialized rather than a
colored local. Merely naming/splitting it does not create the missing native
live range. A compiled-out endpoint read changed unrelated allocation and was
rejected. The ordinary signed/unsigned/type and condition-spelling controls
did not close the residual. No forced endpoint or arbitrary pressure is kept.
A useful next step needs authentic endpoint-carrier/loop-expression evidence;
resume from this source rather than repeating the old pointer-lifetime search.

## Contracts and verification

The function releases a non-null resource, clears its pointer after the helper,
then releases scene handles across three 52-byte records and ten 72-byte effects
per player. It writes status values 8, 0 and 11 and clears released handles to
-1. The signed player count is reread after every complete player group.
Historical sound names do not establish an audio interpretation.

The complete native direct-caller census across game/A/B is exactly
**records_screen +0x110**. The complete caller and both complete helper bodies
are input-bound. The real `sound_stop` boundary is a resource pointer;
`sound_call_minimal` takes one sign-extended s16 O32 argument. There are no
fabricated private-register arguments. Native and candidate preserve the O32
callee-saved integer registers and stack pointer under adversarial caller-save
integer clobbers. No floating-point instructions occur in these bodies.

The protocol-v2 receipt checks:

- Complete STT_FUNC set, exact native entry/next boundary, every 380-byte body,
  384-byte text section and the final four zero alignment bytes. Unknown
  allocated data and unexpected relocations fail closed. No tail is borrowed.
- Independent GNU linkage of the **entire unmodified object**, consuming all
  **21 original REL records**, checking all bindings, exact native placement,
  full final text and every unmasked instruction word. There is no owned data.
- **1,344 field-oracle fixtures / 4,032 machine executions**: protected native,
  final candidate (also independently GNU-identical), and inherited baseline.
  Every one of all three bodies' 95 instructions and both outcomes of every
  conditional branch execute. Complete mapped nonstack bytes and helper-entry
  event/state snapshots agree. Hooks exercise live count changes, later handle
  changes, resource replacement and opaque record mutations.
- **1,344 actual-source C89 host fixtures**, both ordinary and
  ASan/UBSan/bounds builds, with sanitizer recovery disabled. Only LeakSanitizer
  is disabled. A compiled wrong-state mutation fails in both builds. Twelve
  native-only compile-time width/offset/size checks pass and leave the candidate
  body unchanged.
- Unsupported machine instructions, wrong state and an overlong STT_FUNC extent
  are rejected. Empty claims are refused by the actual installer entrypoint.
- Retained stock intermediates and diagnostic uopt with tracing enabled produce
  the same **complete object** as ordinary stock IDO. Diagnostics are optional;
  no tracer is required by the receipt or CI test.
- Normal and Python `-O` deep replay pass. The one focused CI test passes in
  **0.46 s**. With IDO absent it cleanly skips in **0.39 s**, after validating
  source/native input identities. Strict shared skip handling is retained.

Receipts pin packet source/proof/harness/test files, the inherited source,
selected complete target/helper/caller words and bindings, the parsed-word
inventory used for the caller census, and eight stock compiler executables.
They do not pin shared tools, locks, Makefile or raw assembly text. No static
ROM translation unit is rebuilt.

## Replay

```
python3 cloud/work/frontier/dot_runtime_b_hud_teardown_20261012/verify.py --deep --score
python3 -O cloud/work/frontier/dot_runtime_b_hud_teardown_20261012/verify.py --deep
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_b_hud_teardown_research.py -q -o addopts=''
python3 cloud/work/frontier/dot_runtime_b_hud_teardown_20261012/trace.py
```

Optional diagnostic allocator: add `--uopt /path/to/instrumented/uopt` to
`trace.py`. Its retained artifacts stay in the ignored build directory.
Independent review, publication and exact-head hosted CI are pending.

## Limits

The finite C-valid fixture domain has four accessible, nonoverlapping player
groups; initial count is -32768, -1, 0 or 1..4, and helper-modified count stays
0..4. Negative initial counts exercise only the entry guard. No invalid pointers,
counts beyond storage, malformed linked resources, arbitrary aliases, thread
interleaving, original source identity or full gameplay behavior is proved.
The helpers are synchronous state-mutating doubles in behavioral tests, not their
real implementations. The bounded MIPS interpreter is not N64 hardware. Full
image composition/recompression, emulator/gameplay and ROM checksum gates remain
unrun. Keep draft; the independent checker owns integration and merging.
