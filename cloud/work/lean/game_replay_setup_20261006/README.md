# LEAN RESEARCH: complete options-menu setup

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `replay_save_prompt`, 800DABDC..800DB1E0, 1540 bytes / 385 words.

## Observed compiler result and limits

The tracked seed does not compile: its slot_state_setup(void) declaration
contradicts four actual selection arguments. After only the documented real
prototype repairs, it scores **377/385**, with **12 nonzero excess words** and a
1588-byte ELF function. This new complete C gives the canonical reported result
**365/385**, **3 excess words**, and **1552 bytes** at the same IDO 5.3 O3 flags.

The new result still has **14 unverified owned-literal relocation sites**, two
owned-data mismatch diagnostics and one unpaired-HI16 diagnostic. They remain
explicit in observed.json; raw diagnostic byte excerpts alone are redacted.
Consequently the differing-position count is not a fully relocated improvement
proof. This is a smaller compiled research body with corrected source/context,
not a match, verified behavior, accepted coverage or ROM result.

Flags: `-g0 -O3 -mips2 -G 0 -non_shared`, canonical added `-Wab,-r4300_mul`.

## Complete native source and genuine context

The source retains both full setup paths: live font-height queries and visible
option count, selected-option scan, all 32 model rows, constructors and color,
three texture selections, the final three matrix/position setups, the 2-D panel
path, actual callback-record registration, final input reset and enabled flag.

The real 64-byte Button model comes from the earlier complete DA2C0 native
reconstruction, extended to the 32 rows actually traversed by this initializer
and its DA0BC cleanup. The final matrices/positions use rows 29, 30 and 31;
this removes the seed's incorrect far-offset field expression. All actual
address-taken locals are meaningful (texture index and matrices), with no
padding, dummy value, fabricated parameter or new volatile qualifier.

The real gfx_lock/gfx_unlock/font_set wrapper definitions are retained from
`cloud/work/s20261004/E/src/helper.h`. They are required source context for the
native repeated queue/selection sequences, not invented helpers. The selected
font formal is supported by that existing wrapper/slot reconstruction. Texture,
constructor and Blit factory signatures use their current source contracts.
`entity_audio_update` is declared void(void), matching the accepted complete
`src/blob/entity_audio_update.c`; no fake a0 is supplied to silence m2c.

The minimal repaired-seed baseline changes exactly:
- slot_state_setup(void) to slot_state_setup(s32 selection)
- the stale entity_audio_update declaration to void(void)
- the unset-a0 M2C_ERROR call to the genuine no-argument call

Other seed problems are not disguised as validated behavior. The new native
source is separately inspectable. Slot setup and other actual external callees
still need fuller compiler context for matching; no stand-in caller is used.

Assumptions: valid nonoverlapping native records and selected indices; at least
one of the 14 options enabled; nonzero queried font height and representable
signed divisions; successful constructor handles indexing real 68-byte render
records; valid texture/model name tables; ordinary finite FP operation; and the
established external callback effects. No malformed-input fallback is invented.

## Minimal replay

Set `IDO_DIR` to the pinned compiler, then:

    python3 replay.py --repo /path/to/SFRush2049-decomp

Named immutable Git inputs are materialized locally, the exact baseline repairs
are applied, and both complete sources are compiled. Canonical scores, errors,
unverified references, source hashes and ELF extents are reported. No scorer,
boundary or protected input is changed. No independent review, behavior harness,
full suite, CI wait or ROM gate was run. Checker/Claude owns verification,
acceptance and ROM integration.
