# Additive TB_DrawText source-boundary research

This extends the unchanged 39-member text pipeline from draft PR415, head
50e4f1e3d34f606be683ca4de91fde58ea426f73. It contains only the added text
placement/width sources, replay, bounded model and evidence. The inherited
22 C files are hashed and materialized from that packet rather than duplicated.
Claims and candidate/image credit remain empty.

## Source and result

The saved MAT3 TB_text.c donor identifies TB_DrawText at 0x8004FB78, 208 bytes,
original DWARF DIE 0x107B41: void(signed short x,signed short y,void *text),
with no automatic locals. Genuine TB_CenterText and TB_RightText return signed
short; horizontal centering shifts the unsigned text width, while vertical
centering divides the signed font height. HAL_DrawText receives text and -1.
Donor source SHA256 cbb9c9b255213d2d6479bd495ef564165e7f3d4978b70be8ee641f6ec77a83eb;
HAL source 01edf586c784fbb0ed8da894559957b058e4da80de3af1c6f564731891a3a4fb.
The donor was retrieved read-only; no fresh MAT3 compile was performed.

The native root is state_utility/TB_DrawText at 0x800B71D4, 396 bytes. Retaining
these real Center/Right boundaries leaves 14/99 differing words at the full
396-byte extent and 24-byte frame. The fourteen words are the existing font
pointer materialization differences in two inlined font-height calls. The
flattened horizontal control is separately reproduced. No padding, register
forcing, object edits or synthetic caller is introduced.

Native SetTextPos stores unscaled coordinates and preserves the -32768 sentinel;
the MAT3 donor scales both coordinates by two. The native behavior is retained.
The genuine Just object is eight bytes, enum-sized h@0/v@4, initialized {0,3}.
The additive graph retains PR415's externally visible reconstruction boundary.
A separate authentic file-static/co-located caller control remains partial;
its section-addressing changes are diagnostic, not a recovered original N64
translation-unit assertion. All eight initialized bytes are checked independently
against native data, including the trailing nonzero vertical value. The scorer's
setter note narrows from eight bytes to four after adding another reader; this
is a window-selection note, not missing data evidence.

The added width body is the already-locked native reconstruction imported from
the AI text caller graph. Its 16-byte header is completed without changing code,
data or relocations; the TB declaration agrees with its unsigned-byte pointer
formal. These are coherent native views, not a new donor reconstruction of width.
That inherited width body retains null-pointer counter arithmetic, a monospace
read before ch is initialized, and source scheduling choices; none is claimed
as portable ISO C or covered by the placement model. The body is strictly scored
only as compiler context. Existing pipeline source-shaping, aliasing, volatile,
font/storage/queue and external graphics limitations in PR415 still apply.

## Complete accounting

The compact additive graph has 43 native bodies: 37 strict and six explicit
failures. The three added strict helpers (width, CenterText, RightText) are already
locked and receive no credit. All 39 inherited native outcomes and extents are
preserved. Two 8-byte deleted semaphore residues remain explicit nonnative code.
The inherited failures are B3F50 4/21, B7128 4/18, B6788 46/281, B6BEC 258/320
with 1276 emitted/native 1280 bytes, and ED804 151/178 plus 14 excess words and two
own-data gaps. The new root's 14/99 failure is retained. Reserved 87110 remains
external. No canonical integration, whole-image or ROM verification is claimed.

## Bounded behavior and reproduction

The host model executes the actual root, Center/Right helpers and native
SetTextPos. Width, height and final DrawText are deterministic captured services.
37,748,736 fixtures cover every signed-short x with paired y=-1-x, four
horizontal/four vertical modes, six unsigned-width boundaries and six stable
font heights. An independent explicit two's-complement narrowing model checks
coordinates, sentinel preservation, service counts, pointer and -1 draw bound.
This is not the Cartesian product of all x/y values, arbitrary service mutation,
width evaluation, rendering or whole-game equivalence. Four compiled negatives
must fail at the exact differential fixture and exit3; fatal UBSan failures do
not substitute for that expected failure.

Run replay.py from a complete checkout containing the unchanged PR415 packet.
Four focused tests include 1,142 precise optimized refusals, relocated replay, native
layout/data negatives and standard absent-tool behavior. Inputs bind all nine
active IDO stages, scorer/ownership tools, protected manifests, source and target
rows. Every emitted function, full extent, bounded frame, complete comparison
and non-debug section is retained. Repeated complete objects must agree.

The final complete stock object also equals an observation-only instrumented
ugen build byte for byte. trace.json binds the final added/inherited source and
all nine stock stages; the instrumented executable is fingerprinted separately.
Raw trace logs and objects remain outside the packet. Reproduce with trace.py
--ugen /path/to/instrumented/ugen; the normal replay seals the source-bound
trace receipt against its freshly built complete object.
