# Genuine text pipeline research

Research only: empty claims, zero new candidate bytes, zero accepted image bytes.
This is not an installable group. The complete object contains 39 native members,
34 strict observations and five explicit failures, plus two deleted eight-byte
semaphore helper residues. No ROM or canonical integration was performed.

## Source recovery and native adaptations

The supplied MAT3 bodies identify three previously misleading native labels:

- world_velocity_integrate at 800ED804 is HAL_FinishText, the buffered command reader.
- audio_doppler_calc at 800B6788 is the private _DrawText glyph renderer.
- menu_input_process at 800B6BEC is HAL_DrawText, the immediate/buffered text writer.
- func_800B66B0 is _TextLen, the original bounded byte-count helper.

HAL_FinishText retains its original three meaningful locals (i, length, aLong),
800-byte buffer protocol, and opcodes 0 through 6. The four-byte bit packing uses
an unsigned high-byte shift. Native queue calls and unscaled N64 positions remain.
The native switch-table references are still unverified because the partial code
no longer aligns with the scorer's target-address inference. They are not waived.
No native/candidate execution claim is made for this buffered reader.

_DrawText retains the recovered eleven meaningful locals and the real font
setup, glyph, kerning and page contracts. N64 scissor, attribute, color and texture
backend calls replace the different MAT3 backend. The original MAT3 draw's +1
coordinate offsets are not transferred to native rendering. The source follows
the original expression ordering rather than adopting a one-word scheduling
improvement. Its two genuine callers are present. Removing HAL_DrawText in the
causal control inlines the renderer into HAL_FinishText and leaves a deleted
renderer stub; no synthetic keeper was inserted to prevent that.

HAL_DrawText retains its original size/length/str[256] locals, including the
original finished-copy path. Native coordinate and spacing serialization are
unscaled. The first native adaptation removes preliminary half-scaled cache
stores; the selected version also follows native cache-before-record writes for
font, leading, spacing, alpha and foreground color. Position cache writes remain
after the current-coordinate record. The genuine two-local _TextLen short-circuit
loop reproduces all 152 native bytes in this context. It receives no new credit
in this partial packet. Function-static donor caches retain named native globals;
the tested static-linkage variants introduce scattered BSS uncertainty and are
excluded. The original donor and intermediate coordinate adaptation passed the
same bounded storage model before the selected source was frozen.

Source status is deliberately distinct from native matching: the supplied MAT3
source was preservation-audited, but no fresh standalone strict donor receipt was
retrieved for these functions. This packet independently verifies its own native
build and target bindings. Original DWARF declaration order is local evidence,
not a guarantee of source-text declaration order.

## Native types and inherited context

The font header is 16 bytes, with an unsigned byte non-proportional flag at +9.
Glyphs are 12 bytes; texture records are 36 bytes with width/height at +16/+18 and
image pointer at +24. Leading and spacing are signed bytes. Every shared Bank and
ResSlot declaration agrees across the group. The character-width return is
unsigned, and the two-getter height wrapper has its original no-local form.
The final coherence cleanup also makes the graphics color-cache global a byte
pointer everywhere and gives the attribute setter its unsigned-mask prototype.
Only .mdebug changes; all other ELF sections and all 39 comparison rows are
identical to the physics handoff. storage_handoff.json preserves the pre-cleanup
provenance; verification.json seals the final source hashes.

Native BlitColors reads eight bytes in the actual font modes. The current color
therefore has an eight-byte foreground/background view, while the last-foreground
cache is only four bytes. Comparisons and copies to that cache remain one word.
The primitive-color helper's view is likewise eight bytes. These are minimum
native access views, not claims that an original N64 typedef or background color
initialization was recovered. IDO-emitted layout facts and four wrong-layout
controls verify the relevant offsets, bounds and widths; compiler exit status
alone is not used as a layout proof.

All real font/reset and graphics callers remain in their original translation-unit
boundaries. Inherited resource context still includes documented validation-hook
dead blocks, a byte-setter inline blocker, macro-shaped stores and pointer reuse.
They were neither introduced nor counted as new source discoveries here. The
actual queue helpers inline and leave two eight-byte nonnative residues, both
inventoried. The matched graphics bodies do not establish a complete runnable
closure: the receipt records every external direct boundary and register jump.
In particular, reserved func_80087110 stays external; its body was not changed.

## Complete partial results

- HAL_FinishText: 151/178 words different, 768 emitted versus 712 native bytes,
  96-byte frame, 14 excess words and two unverified references.
- _DrawText: 46/281 words different, full 1,124-byte extent, 112-byte frame.
- HAL_DrawText: 258/320 words different, 1,276 emitted versus 1,280 native bytes,
  376-byte frame. A short function is not an exact match.
- object_bytes23_sum: 4/18 words different, 72-byte extent, 24-byte frame.
- object_bytes_sum_global: 4/21 words different, 84-byte extent, 24-byte frame.

The other 34 native bodies have strict complete extents, including the genuine
font/resource and graphics bodies. Their full rows are sealed in verification.json.
There are also 12 zero alignment bytes outside the 41 emitted function symbols.
No partial, residue, modeled behavior or observed exact helper is counted as a
new installed match.

## Bounded behavior checks

The renderer's separately expressed field/event model is checked against both
protected native MIPS and freshly compiled candidate MIPS for 2,560 fixtures.
It checks narrow/wide strings, empty strings, unknown characters, newline/space,
proportional/non-proportional rendering, positive and negative centering widths,
previous glyph zero versus positive indices, distinct per-glyph kerning rows,
two texture pages, signed spacing, two starting positions, both font modes and
four alpha states. All visible state and ordered call arguments are compared.
Memory accesses are range/alignment checked; stack, return and preserved FPR
boundaries are enforced. Each captured callee poisons ordinary caller-clobbered
registers. The renderer uses its observed private s2/s0 entry convention.

The tests reach 259 of 281 native words. A conservative decoded control-flow check
proves five duplicate scheduling words unreachable, including one in the float
conversion sequence. The remaining 17 uncovered words are the unsigned-float
conversion fallback. Alpha fixtures stay in the valid 0 through 254.9 range.
Fonts are preinitialized with four glyphs and two pages; setup, graphics and queue
services are deterministic captures. The model does not execute the renderer
backend, reserved rectangle function, resource refresh or concurrent mutations.
The wide-string terminator behavior follows the native length convention.

Six independently compiled wrong-behavior controls fail for precise model
differences: zero-index kerning, draw endpoint, newline leading, spacing direction,
initial page and alpha flag. No VM failure is accepted as a semantic rejection.

The separate storage model checks 737,280 fixtures against the real host-compiled
writer, _TextLen and SetDefaults bodies. It compares all 800 buffer bytes, caches,
initialization state, direct-render arguments and ordered queue/copy events. Cases
span every changed-field mask, first/later initialization, depth modes, finished
copies, narrow/wide text, signed character limits and near-capacity positions.
Five wrong-behavior controls have exact fixture/message/exit refusals. Fatal ASan
and UBSan are enabled. The native float bit-pun is retained under explicit
-fno-strict-aliasing; the native memcpy name is adapted to a host-size wrapper.
Input storage and finished copies are bounded (at most 255 copied bytes); settings
are stable during each call. Real queue, graphics and arbitrary-length behavior
are outside this model. This is a host/source model, not native writer execution.

## Reproduction and integrity

With stock IDO and MIPS binutils configured:

    python3 cloud/work/frontier/mat3_text_pipeline_research_20261017/verify.py
    python3 -O cloud/work/frontier/mat3_text_pipeline_research_20261017/verify.py
    python3 -m pytest tests/cloud/test_mat3_text_pipeline_research.py -q

The verifier binds all source/model/test files, compiler stages, scorer/own-data
checker, locked-match manifest and protected target manifests/hashes. It validates
all extents, all failures and empty claims before requiring complete fresh receipt
equality. Explicit exceptions stay active under Python -O. Optimized refusal tests
and a relocated packet replay exercise those guards. Stock and observation-only
traced uopt produce the same complete object. Raw traces, target bytes, binaries
and disassembly remain only in ignored local build scratch.
