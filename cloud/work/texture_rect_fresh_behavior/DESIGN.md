# Fresh behavior reconstruction, 2026-10-05

This design was recorded before opening any previous candidate C. Inputs were
only the protected native section, its three direct calls in two containing
native bodies, and `texture_rect_verification/replay.py` plus its native model.
The target is 445 words / 1780 bytes at 0x80087110..0x80087804.

## Machine contracts

- Six O32 word inputs: x, y, right, bottom, s, t. Signed comparisons govern
  coordinates; no callee-side halfword narrowing. Both native callers ignore
  the return value. The historical `s16` y prototype is not an established
  source type: it could describe a restricted calling domain, so absence of a
  narrowing instruction alone does not disprove that original declaration.
- Clip x, y, right, bottom in that order against the four globals. Normal
  texture axes advance their starting texel on left/top clipping. Flipped axes
  advance on right/bottom clipping. Native arithmetic wraps at 32 bits.
- Reject only when right < x or bottom < y AFTER clipping. Equality emits
  three Gfx packets and advances the global head by 24 bytes.
- Bit 4 reverses S and bit 8 reverses T. For reversed axes, start at the
  opposite clipped edge by adding right-x or bottom-y to the input texel.
- Mode zero puts clipped right/bottom directly into the command and uses
  S derivative magnitude 4096 and T magnitude 1024. The 0x8000 bit has no
  effect in mode zero.
- Nonzero mode adds one to right/bottom command coordinates and uses S
  derivative magnitude 1024. With 0x8000 it stretches bottom by the original
  clipped height plus one, halves T derivative to 512, and adds 16 to the
  5-bit-fraction T start only when vertically flipped. There is no reclip
  after stretch, and the texture-height adjustment uses pre-stretch height.
- Native emission is one G_TEXRECT (0xE4), one G_RDPHALF_1 (0xE1), then
  G_RDPHALF_2 (0xF1), tile zero. Coordinates are 12-bit fields after <<2;
  texture positions and derivatives are masked to 16-bit fields.
- All input globals remain unchanged. The ordinary supported domain assumes
  disjoint aligned packet storage and no racing mutation of the globals.

## Source hypotheses deliberately NOT inherited

Names of the callers (Input_ProcessGameplayPad and audio_doppler_calc), UI/HUD
purpose, exact source parameter widths, typedefs, macro revision, eight macro
expansion sites, block scopes, declaration order, alias assumptions, compile
optimization level, and meaningfulness of the native spill slots are not
behavioral facts. A 445-word native body is not a specification for C length.
Native read order is evidence to explain later, not necessary for equivalent
observable behavior under the explicitly disjoint, nonracing domain.

## Clean formulation

Use a single SDK gSPTextureRectangle emission after clipping and adjusting
coordinates/derivatives, rather than eight copied emitters. Keep signed
coordinate comparisons; perform potentially overflowing arithmetic and shifts
through 32-bit unsigned values. Full-word signed reinterpretation is the
compiler's two's-complement implementation-defined conversion, to be tested
on both host and IDO. Preserve three individually published packet pointers
as the authentic SDK macro does. No inline assembly, fake variable keepers,
volatile objects, or compiler-gate changes.

Before any stock compile, validate host C with UBSan against protected-native
execution and the independent scalar packet oracle. Cover all mode/flip/stretch
paths, full-word values, both clipping directions, zero area and rejections.
Then stock-compile once and use extent/CFG/residual only to diagnose which
unproven source/optimization choices affect shape, not as a search fitness.

## Falsifiable tests

1. The source-independent 0x8000 metamorphic test: toggling it in mode zero
   must leave the complete packet unchanged; in nonzero mode it must affect
   derivative and bottom, but the +16 phase applies only when bit 8 is set.
2. A zero-size rectangle exactly on the clipping boundary must still publish
   24 bytes. Replacing strict rejection with <= would fail immediately.
3. A rectangle clipped at bottom and stretched must encode a bottom beyond
   clip_bottom where applicable. A second clamp or post-stretch height in the
   T-origin calculation would fail.
4. Inputs differing by 65536 must not be silently reduced to s16 before
   clipping. This tests the broad native-word contract, not caller reachability.
