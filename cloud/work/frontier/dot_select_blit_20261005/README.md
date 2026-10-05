# SelectBlit: natural C strict match

Target: `stat_race_update`, `0x800FE5B0–0x800FE73C`, 396 bytes / 99 words.
Base: `cc4d5fdd0bbc42dbf6be49f00d9454af8cb9c4f5`.
Status: **strict matching candidate, pending independent review and integration**.
Accepted-byte gain claimed here: **zero**.

## Why this target was reopened

The latest master accepted `func_800EF5B0` (N64 `RenameBlit`) in wave 5. Its
real texture/Blit contracts and the authentic arcade `SelectBlit` implementation
provide a source-based reason to revisit A71, rather than repeat allocation
variants. The standalone target does **not** require this callee's body to
compile: acceptance makes an unchanged, genuine context regression possible.

The current lock, visible branches/handoff, latest wave-5 results and open PR
list were checked on 2026-10-05 at approximately 19:50 UTC. This target was
unlocked and absent from the current wave assignments; the only open PR was
#98 on `func_80087110`. The latter and all its working directories were left
untouched. This establishes no visible overlap, not certainty about unpublished
Claude work. A packet-specific claim is recorded in `claim.json`.

Source lead: [arcade LIB/blit.c SelectBlit, lines 196–242](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/LIB/blit.c#L196-L242).
Reference commit: `845329d7b36f5a384c5625ed9a0aef584ab46139`.
Reference file SHA-256: `34652da79c592dfd0c77e93ba717bd0d3e52e9ae869c01d69b01d650da93a080`.
The arcade source stays outside the submitted packet.

## Result and source explanation

`cloud/matches/stat_race_update.c` reproduces all 99 native words at
`-g0 -O3 -mips2 -G 0 -non_shared`, with the scorer's normal
`-Wab,-r4300_mul` handling. The first complete donor-based candidate matched.
The O2 control is **98/99 NONMATCH**. Recompiling the archived A71 source at O3
reproduces its **45/99 NONMATCH**, exact extent, with no unresolved relocations.

The natural arcade `row, col, secPerRow` flow removes A71's explicit buffer
carrier and its repurposed row-count arithmetic. The Info pointer, section
count, quotient and remainder receive the native lifetimes/register choices.
No filler locals, dead checks, artificial reads, unused arguments, helper
stand-ins, keepers or compiler changes are introduced.

N64-specific changes are recovered directly from this complete native body:

- A zero sections-per-row quotient becomes one, rather than returning.
- Destination width and height are unchanged.
- Texture Top is `row * vSize`; Bot increases downward from the truncated
  signed-halfword Top.
- Flip reverses the selected column and preserves the partial-row remainder.
- Texture lookup failure returns without calling UpdateBlit.

The declared Blit is an observed prefix, not a claim that the complete record is
40 bytes. No `sizeof(Blit)`, array indexing by that size, or aggregate copy is
used. TexDef's width is unsigned 16-bit at byte 16. Opaque record fields are
layout evidence; they do not create stack/register pressure.

## ABI and complete-object proof

Four consumed arguments use the standard `a0–a3` calling convention. The native
24-byte frame saves/restores `ra`; it does not write unsaved callee-save GPRs or
require hidden incoming registers. It calls RenameBlit with the Blit, its Name,
and preserve=1, then reloads Info. The only other call is the one-pointer
UpdateBlit (`Input_ApplyPadConfig`).

`verify.py` checks:

- ELF function symbol extent exactly 396 bytes, with all 99 relocated words
  equal, no extra words, unresolved symbols, masks, or unverified data.
- Both `R_MIPS_26` relocations: offset 64 to `func_800EF5B0`, offset 372 to
  `Input_ApplyPadConfig`.
- An independent GNU link at `0x800FE5B0` resolves those symbols to their
  authenticated native addresses and reproduces the entire function.
- The `.text` section's four trailing bytes are zero alignment outside the
  function symbol. The candidate has no own rodata, data, BSS, or local tables.
- A genuine six-body O3 context leaves the target plus five unchanged accepted
  functions fully exact: RenameBlit, InitBlit (`collision_sound_play`), the
  texture lookup (`func_800B24EC`), UpdateBlit and `Input_InitPadHandlers`.
  The accepted sources are copied read-only; no production keep/root settings
  are edited.

The archived workbench diagnosis is retained only in ignored local scratch. It
reports the first old-source divergence in the colored register pool with an
equal frame, then mixed structure/register residuals. The raw target-object
relocation warning is expected because its call addresses are already resolved;
the strict scorer and GNU linker, not that heuristic, establish equality.

## Behavioral evidence

`verify_semantics.py` runs **4,304** deterministic cases through four independent
routes: the protected native instruction stream, GNU-linked candidate code, an
arithmetic oracle, and the actual C compiled for the host with UBSan.

Cases cover null owners, null/zero-width texture recovery, failed recovery,
zero quotient fallback, signed index and horizontal step, forward/reversed
partial rows, signed-halfword coordinate wrap, and unchanged destination
geometry. Synthetic RenameBlit replaces Info and clobbers caller-save GPRs, so
reloading and ABI preservation are tested. UpdateBlit records its call.
The interpreter verifies preserved GPRs, stack restoration, and writes confined
to Info/texture-coordinate fields.

Two native-only cases additionally exercise division-by-zero and signed-division
overflow traps. Host undefined division/overflow cases are excluded, explicitly.
The corpus executes 95 of 99 instruction offsets. Four division-guard/trap
instructions are not executed; complete branch coverage is not claimed.
This is not a gameplay-domain claim or a test of either actual callee's logic.

All five deliberately wrong contract variants are detected: reversed column
formula, zero-quotient fallback value, unsigned index division, wrong flip arm,
and omitted final update. Exact discriminator inputs are in `verification.json`.

## Reproduction

```sh
python3 tools/cloud/score.py fn cloud/matches/stat_race_update.c stat_race_update \
  --flags '-g0 -O3 -mips2 -G 0 -non_shared'
python3 cloud/work/frontier/dot_select_blit_20261005/verify.py
python3 -m pytest tests/cloud/test_select_blit_contract.py -q
```

The eight focused tests pass. Source, packet, target-manifest, accepted-context
and compiler hashes are in `verification.json`; no native byte streams,
assembly dumps, objects, compiler binaries or private inputs are submitted.

The complete game shadow unit, source-built image, compression and ROM gates
require the independent integration environment and were **not run here**.
The six-body context check is narrower and is reported as such. Merging,
splicing and coverage changes remain with the independent checker.
