# BT03 emitter parameter dispatch: COMPLETE-NONMATCH

`func_8001CCDC` is fully reconstructed but remains **COMPLETE-NONMATCH**, with
**zero matched-function or byte credit**. The selected IDO 5.3 O2 body differs at
181/234 positional words and has a 952-byte ELF function versus the exact
936-byte native extent. The raw difference count is dominated by four inserted
narrowing instructions and resulting positional/register shifts. No match is
submitted under `cloud/matches/boot_tail/`.

- Exact native range: `[0x8001CCDC,0x8001D084)`, 234 words / 936 bytes.
- Reused clean worktree base: `0251b819` (prior math review retained).
- Exclusive activation: `57a23793`, recorded before source editing.
- Branch: `dot/boot-tail-bt03-chain-dispatch`.
- Scope: this packet only. Central ledger/publication, protected targets, scorer,
  symbols, layout, locks, shared headers and other targets are unchanged.
- Independent review PASS binds frozen source commit
  `75df2d9881d9df6b57b571dfe9d0c9b74e9e464e` and tree
  `76827e8975539129416f58dbb5b189eeccc1be9f`; see `independent_review.json`.
  Aggregate-head CI is a separate central gate. This packet makes no
  cartridge-coverage claim.

## Source provenance and whole native body

The SOURCE-LEAD is `SetFXParameters` in pinned
[AxioDL/musyx snd3d.c](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/snd3d.c),
revision `78d2e16e4905fc675952162d331c24d5198b2687`, under its pinned
[CC0-1.0 license](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE).
Both source and license were read for this packet. `preflight.json` records the
source hash. This identifies a close public definition, not exact original N64
translation-unit provenance.

The whole native body performs four ordered controller updates using an emitter's
identifier captured once at offset 52:

1. Volume: if flags at offset 8 contain `0x100000`, multiply the float fade at
   offset 64 by volume, then multiply by 127. Otherwise multiply volume by 127.
   Convert to unsigned, narrow to a byte, call `1CC9C` to clamp that byte to 127,
   then send through `1B7C0` (controller 7).
2. X pan: add 1 to xPan, multiply by 64, convert/narrow/clamp, send through
   `1B29C` (controller 10).
3. Z pan: subtract zPan from 1, multiply by 64, convert/narrow/clamp, send through
   `1B3A0` (controller 131).
4. Doppler: multiply by 8192, convert to u32, call `1CCC0` to clamp to 16383,
   then send through `1B4A4` (the 14-bit controller pair 132/133).

All six distinct direct callees were inspected. `1CC9C` accepts a genuine byte;
its home store, byte mask and bound compare corroborate the public `clip127(u8)`
source. `1CCC0` accepts u32 and returns u16. The sender helpers take u32 identifiers
and genuine byte/halfword values; their results are ignored here. The public
source's generic controller APIs are specialized native helpers. Its extra
`paraInfo` loop and later-version filter logic are absent from the entire native
body and are not imported. The target contains no callback, jump table, data-pool
load or indirect control flow other than the final return. All float constants
are immediate native IEEE-754 values.

## Genuine six-input ABI, including unused yPan

The signature is `void(Emitter *, float volume, float xPan, float yPan,
float zPan, float doppler)`. This is supported by three actual native callers,
not a guessed extra formal to match argument homes:

- `1D1F4+0x198`: a0 is its emitter; a1/a2/a3 load float words from stack offsets
  52/64/60; outgoing stack slots 16/20 hold the values at offsets 56/48.
- `1DC08+0x154`: a0 is the emitter; a1/a2/a3 load start-record offsets 4/8/12;
  outgoing slots 16/20 receive record offsets 16/20. These are the public
  START_LIST volume/xPan/yPan/zPan/pitch fields in the same order.
- `1DDE0+0x224`: on the call-reaching path a0/a1/a2 are set at +0x1BC/+0x1C0/
  +0x1C8, a3 loads stack offset 92 at +0x21C, and outgoing slots 16/20 carry
  floats previously loaded from stack offsets 88/84. The intervening branches
  that perform other calls bypass this dispatch call.

The callee moves a1 into f12, homes a2 and a3, later reloads xPan from its a2 home,
reads zPan/doppler from original sp+16/sp+20, and never uses yPan. The public
six-parameter definition corroborates the same intentionally unused yPan slot.
Removing it would move live arguments and change the ABI. No invented parameters
or widened helper prototypes were tried. `Emitter` is only a 68-byte observed
prefix with opaque bytes for untouched fields; IDO assertions prove fields at
8/52/64. The opaque ranges represent real native record offsets, not stack
padding or allocator gadgets.

## Compiler controls and diagnosis

All controls use `-g0 -O2` or `-g0 -O1`, followed by
`-mips2 -G 0 -non_shared`; the unchanged scorer adds `-Wab,-r4300_mul`.

| Control | Differing / target words | Nonzero excess | ELF function bytes |
|---|---:|---:|---:|
| Initial source-led O2 | 181 / 234 | 3 | 952 |
| Initial O1 | 225 / 234 | 14 | 1004 |
| Selected explicit-u32 O2 | 181 / 234 | 3 | 952 |
| Selected explicit-u32 O1 | 225 / 234 | 14 | 1004 |

The native 32-byte frame, s0 identifier snapshot, argument homes, and FPU sequence
support O2. O1 has a different 40-byte frame and broad scheduling differences.
The O2 function's `.text` section is 960 bytes, including eight bytes of section
alignment beyond its 952-byte ELF symbol. The 16-byte function excess over the
native body is real, not legitimate alignment slack. Three of the four words beyond the
native extent are nonzero, which explains why the scorer's excess count is three.

`tools/workbench.py diagnose` was run on the first O2 object before refinement,
with the existing GNU MIPS objdump. It reported structure mismatch, cfe-spelling,
234 versus 238 real instructions, identical 32-byte frames, and the first drift
at +0xC8. At each of the four byte conversions, native code masks a0 directly in
the helper-call delay slot. The candidate masks to a temporary and then moves that
temporary into a0 in the delay slot. Later temporary-register shifts follow those
extra allocations. Its word-only temporary target object lacks symbolic
relocations, so workbench's relocation-symbol warnings are expected; the strict
scorer independently resolves all calls without masks, unverified data or errors.

Nine directed controls were bounded to actual quantization data flow: explicit
byte, word-then-byte and word casts; genuine byte/word quantization locals;
separately stored clamp results; an explicit low-byte integer mask; and an
unsigned-long cast. Eight retained the same residual. A word local reduced the
positional count to 155/234 and the extent to 948 bytes, but changed the first
conversion register to v0 and retained three inserted operations. This is not a
semantic discovery or an exact match, and was not selected just for its lower
positional score. `verify_controls.py` reproduces all eleven initial/directed
rows with source hashes. No flags beyond O2/O1, declaration permutations,
register forcing, unused locals, dummy calls or weakened prototypes were used.

The selected explicit-u32 spelling keeps conversion-to-word followed by normal
integer byte narrowing visible, making the native modulo-256 behavior defined
for the documented finite unsigned input domain. It emits the same O2 code as
the initial source. Next useful evidence is original N64 helper declaration style
or compiler-front-end analysis of this direct narrowing shape. Widening the
proven byte helper just to remove a move is not an accepted remedy.

## Reproduction and semantic verification

Use the already-approved, pinned IDO installation; no tools were installed or
copied. `preflight.json` pins all 24 compiler files, scorer/setup, getter, target
manifest and census. All 439 extents / 99,120 bytes reconcile; the existing getter
strictly matches. `verify.py` checks every relocation, both optimization controls,
exact ELF symbol extents, native layouts and complete direct-call/caller census.

```
python3 cloud/work/boot_tail/BT03-chain-dispatch/verify.py --ido-dir "$IDO_DIR"
python3 cloud/work/boot_tail/BT03-chain-dispatch/verify_controls.py --ido-dir "$IDO_DIR"
python3 cloud/work/boot_tail/BT03-chain-dispatch/test_host.py
python3 cloud/work/boot_tail/BT03-chain-dispatch/test_host.py --sanitize
python3 cloud/work/boot_tail/BT03-chain-dispatch/test_native.py
```

The actual candidate is compiled as C89 and tested in 6,040 calls / 48,320 modeled
callee events per host mode. Normal and ASan/UBSan/float-cast-overflow modes pass.
Tests cover both fade paths, unrelated flag bits, signed zero, 127/255/256 and
16383/16384 boundaries, high-u32 values and byte wrap, eight-call order, ignored
callee failures, emitter mutation by every helper, and preservation of the
identifier snapshot. yPan includes NaN and varying values and remains unused.
Float contraction/fast math are disabled. Leak detection alone is disabled for
the traced sandbox; the harness allocates no dynamic memory.

The independent host oracle stages arithmetic into float values and quantizes
through double only after that rounding; it does not call the candidate's helper
expressions. Every evaluated float-to-u32 operand is finite, nonnegative and
below 2^32. This is an explicit test/C semantic domain, not an inferred global
invariant. Negative integer-valued operands, NaN, infinities and values at/above
2^32 have no portable C conversion claim here. Native lowering has extra fallback
behavior for them; host tests deliberately do not invoke undefined conversions.

`test_native.py` reads the protected words only at runtime and executes the full
body with a purpose-limited MIPS/FPU instruction interpreter and audited helper
models. It passes 3,084 calls / 24,672 external-call events, exercising all live
O32 slots, both branches, high-bit conversion paths, invalid-conversion fallback,
full caller-saved clobbers, global emitter mutation and preservation of sp/s0/ra/
FCSR. The interpreter rejects unimplemented instructions and documents its limits.
It is useful native semantic evidence, not hardware/emulator validation or an
alternative to strict matching. No ROM bytes, target dumps, disassembly dumps,
objects, credentials or unrelated private data are included in this packet.

## Independent review receipt

The reciprocal reviewer independently inspected the full native body, all three
caller paths and all six callee ABIs, fetched the exact public source and CC0
license, and replayed the pins, strict scores/extents, all eleven controls, both
host modes and native instruction-model tests. Review PASS is for honest
COMPLETE-NONMATCH research only. No source correction was required.

An additional optimized O2 host run passed. Three intentional defects were
rejected: wrong volume scale, lost identifier snapshot and wrong fade-flag mask.
`test_review_mutations.py` reproduces those reviewer checks; only its original
temporary worktree path was made packet-relative. `review_mutations.json` is the
reviewer result. This review-only supplement leaves the frozen C source and
verification harnesses unchanged.
