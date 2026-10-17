# Audio-options initialization: exact-function research closure

Base `58425072b49e95f0fbe5c4c239988fb4885f7c5c`, 2026-10-17.
Research only. Empty claims, zero aggregate candidate/image credit, and no
complete-group acceptance. This packet extends the source boundaries of
audio-cleanup research PR #405; it does not replace a locked group.

## Native result and complete boundary

The native `drone_pathfind_main` at 0x800D71D0 is `audioscr.c::initScreen`,
with `initButtons` inlined. Its full 1,124-byte native body strictly matches:
281/281 words, frame 168, no extra words, unresolved references, ownership
refusals, or errors. The actual caller `func_800D7634`, teardown
`credits_screen`, renderer allocator/wrapper, SaveGlobals, HAL font setter,
font-slot setter, and genuine reset helpers all retain their exact results.

The native `drone_collision_avoid` at 0x800D6E7C is `calcMenuRect`.
It remains 30/213 differing words, frame 120 versus native 144, with the full
852-byte extent and no extra words. Every difference is a stack/frame
immediate: opcodes, registers, control flow, and nonstack constants agree.
Its two owned `"10"` literals pass strict content verification. The fixed
residual checker does not accept arbitrary frame changes or masked data.

All 48 known emitted native bodies are explicit members, with no hidden
context. Forty-two pass strict comparisons. Six failures prevent installation:

- `drone_collision_avoid`: the 30 stack/frame-immediate differences above.
- `func_800D6914`: 182/315 words differ, twelve unverified references, and an
  owned-rodata content refusal. Removing the inherited volatile model-count
  qualifier worsens this already-refused member; the new initializer stays exact.
- `func_800D6160`: 65/76 words differ.
- `object_bytes23_sum`: 4/18 words differ.
- `object_bytes_sum_global`: 4/21 words differ.
- `world_velocity_integrate`: 158/178 words differ, three extra nonzero words,
  and two unverified section references.

Every negative-control receipt preserves all 48 outcomes. Eight other emitted
source-helper symbols are eight-byte inlining stubs, without identified
separate native targets: `audio_initButtons`, `setButtonColor`,
`ui_uninitButtons`, `ui_ProcessCommand`, `du_gfx_lock`, `du_gfx_unlock`,
`post_msg_a`, and `post_msg_b`. Their extents are recorded in `fidelity.json`;
the verifier rejects an omitted body that has a known native target.

The exact initializer is a function-level research result only. No bytes are
credited to an accepted image or the aggregate candidate count.

## Source evidence and N64 adaptations

The retrieved MAT3 `audioscr.c` is historically recorded Matching, 12/12
functions and 6,772 code bytes, with source SHA-256
`decd753ae39fa5fbdcbf9a0c41f8a07cd79bba921528c998c5d2c7762e0f0ae5`.
That status is not a fresh donor build or proof of N64 inlining.

Original DWARF records three parameterless static void functions:

- `initScreen`, MAT3 0x8006600C, 316 bytes, DIE 0x13E3EA: no locals.
- `calcMenuRect`, MAT3 0x80066AFC, 732 bytes, DIE 0x13EE0C: int menuIdx,
  menuCount, optionWidth, valueWidth; unsigned char str[40]; unsigned char
  pointer charPtr; int songIdx, in that order.
- `initButtons`, MAT3 0x80066DD8, 700 bytes, DIE 0x13F055: four-byte color
  union buttonColor, texture pointer tex, unsigned-short texIdx, int buttonIdx.

The saved donor adds a block-local flags variable absent from original
DWARF. This reconstruction uses the conditional flags expression directly;
it adds no root locals or extra storage. The authentic button, renderer,
color, texture, and matrix contracts reproduce the initializer naturally.
The matrix argument is an opaque pointer to the actual matrix storage.
Stored button identifiers and the wrapper's parent argument are signed int;
the allocator and wrapper return signed short consistently. The button's
64-byte record and renderer object's color at +0x3C agree across the copied
translation units. The real renderer allocation body is retained and checked.

Native platform differences are preserved: twelve named songs plus random
and off (14 scan entries), native table indices 39/40/16, random song 6,
and button Y position 55.2. These differ from the MAT3 20-entry song scan,
table indices 40/41/17, random song 19, and Y position 59.2. The repeated
width calls and ASCII uppercase expression remain. An explicit
`*charPtr != 0` loop condition removes one otherwise redundant compiler move;
it does not introduce another local, frame object, or helper. `str[40]`
remains the original extent despite the residual frame gap.

The original old-style signed-char `validOption()` has no parameters or
locals and returns one; its calls may pass the ignored option index.
HAL_GetTextWidth returns unsigned int and takes a signed-short limit;
TB_GetFontLineHeight returns signed short. N64 model and texture lookups
retain their existing trailing flag argument. The original MAT3 DWARF does
not establish that trailing reconstruction argument; N64 calls are checked
against their own native targets instead.

The initializer's owned four-byte color aggregate and floating constants
pass the strict scorer. Texture names use existing symbol-backed external
arrays; this packet does not misclassify those arrays as newly owned data.
The queued N64 audio path, with its native slot allocator and lock/unlock
bodies, is retained. The MAT3 synchronous command backend is not substituted.

## Genuine context and inherited limitations

The font closure includes actual font-slot, font-height, resource access,
reset, and text replay callers. A setter-only context lets the compiler
inline/delete helpers and changes the internal argument ABI. Restoring the
real callers makes the retained setters exact without a fake call graph.
The corresponding partial font-height and text replay bodies stay explicit.

This does not certify the older context as original source. Retained devices
are visible in the source and receipts:

- The compiled-out block end and unused padB in `func_800D7634`, and unused
  frame locals in the already-refused `func_800D6914`.
- The old constant-one `func_800D690C`, duplicate post-message boundaries,
  and volatile audio slot-table scheduling treatment.
- The inherited volatile input view in the older audio group. The model count
  now consistently uses ordinary unsigned char, as all 26 original MAT3 DWARF
  entries do; the generated header is not independent original-header evidence.
  A retained-volatile control reproduces the former D6914 zero-word result
  with its mixed-section ownership refusal, while all other results stay equal.
- The font setter's compiled-out block boundary, dead inline blockers in
  `object_byte9_set` and `func_80096288`, and resource active-store wrappers.
- The renderer allocator's compiled-out range-check source boundary.
- Partial reconstructed resource/font structure views and the unresolved
  font-height/text-replay body results.

No new padding, unused local, forced register, fake caller, or address trick
was added to the initializer or rectangle. Existing helper spelling and
definition ordering are compiler context, not historical syntax proof.

## Replay and controls

Run `python3 cloud/work/frontier/mat3_ui_audio_init_20261017/verify.py` with
the repository's existing IDO toolchain configured. Source and target hashes,
all 48 receipts, the full emitted-function inventory, strict owned-data checks,
and the exact 30-site residual are pinned. Guards use explicit exceptions and
remain active with `python3 -O`; normal and optimized outputs are identical.

Ten current-source mutations must fail: initializer button count, state mask,
random song, button float, and button color; rectangle buffer extent, font,
literal, song count, and uppercase operation. The float, color, and literal
mutations specifically test owned-data rejection even when instruction
relocations would otherwise be masked. Compared byte values are omitted
from published error receipts. A coherent retained-volatile model-count
control preserves the exact initializer and every other result except the
already-refused D6914 member. It is reported as a source-type control, not
evidence that the N64 original used the qualifier.

Run the fast guards with `python3 -m unittest discover -s tests/cloud -p
test_mat3_audio_init_research.py`, also under `python3 -O`. The whole stock
object and instrumented uopt/ugen/as1 object are byte-identical; their hashes
and settings are recorded in `fidelity.json`. Only the stock compiler and
strict comparisons establish the result. No objects, ROM bytes, raw assembly
dumps, or compiler traces are included. No canonical installation, integration
build, ROM gate, or CI watcher is claimed.
