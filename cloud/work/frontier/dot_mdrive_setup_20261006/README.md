# Init_MDrive / multiinit setup: NONMATCH reconstruction

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_800E543C`, `[0x800E543C, 0x800E56F8)`, 700 bytes / 175 words.
Research only. No claims, accepted coverage, or production changes.

## Reproduce and compare

With the pinned IDO 5.3 toolchain in `IDO_DIR`, from the repository root:

    python3 tools/cloud/score.py group cloud/work/frontier/dot_mdrive_setup_20261006

Flags: `-g0 -O3 -mips2 -G 0 -non_shared`, with the canonical scorer's
`as1 -r4300_mul` group stage. The default command reports NONMATCH and exits
nonzero; empty claims are not used to turn that into a success verdict.

Current best complete corrected B93 baseline:
`cloud/work/near_miss_B93/func_800E543C_branches.c`, compiled at O3 either as a
single or a kept one-file group: 171/175 differing words, 166 emitted words,
56-byte frame. The older `_native.c` has an incorrect final drift-clear scope
and is not the semantic baseline.

This candidate: 132/175 differing words, exactly 175 emitted words, native
72-byte frame, zero extra nonzero words, zero unresolved symbols, zero
unverified sites, and zero comparison errors. This is substantial structural
progress, not a match or a complete-game behavior proof.

## Source changes

The donor is [game/mdrive.c:Init_MDrive and multiinit](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/mdrive.c),
with N64-specific state/camera branches and resets preserved from the corrected
native-led B93 reconstruction.

- Represent the actual four 92-byte tire records and four trailing wheel values
  as arrays. Their offsets/stride correspond to the twelve native float stores.
  A descending four-iteration reset naturally unrolls; no raw instruction order
  or assembly is embedded.
- Restore the genuine two-signed-halfword `multiinit` operation boundary, adapted
  to the N64 actions, while keeping the custom early activation path in the root.
  It naturally inlines in this compile. Its exact original N64 boundary and any
  deleted-stub identity remain hypotheses.
- Reuse the accepted volatile-s16 store contract for D_801525F0 from
  `src/blob/func_800D60AC.c` and `players_frame_update.c`. This is qualification
  provenance, not proof of asynchronous mutation.
- The final source has no unused donor locals or invented frame storage. The
  helper boundary itself produces the native-sized frame. Unknown structure
  gaps describe actual record offsets and are not stack padding.

The scalar-field donor-local control reached only 170/175. A forward array loop
reached 158; the native-evidenced descending loop 139. The genuine helper boundary
and live-only locals reach 132, exact length and frame. Direct-root address
controls and incomplete internal-caller experiments were worse and are not
included.

## Context and remaining assumptions

The group keeps E543C as its observed two-argument ABI root and includes its real
inlined helper. Calls to car_setup_confirm, C4F68, stunt_combo_display, and D5E64
use the existing native-led interfaces. Some of those callees remain unmatched;
their original whole-program clobber/inlining context is not reconstructed here.
The other real caller is the broader state-setup routine, so a one-caller internal
compile was not used as a claim. No artificial keeper/caller is present.

The residual is still broad in register allocation/scheduling despite correct
length/frame. The candidate preserves the corrected action-zero drift clear.
No helper stub, parent, or callee is claimed. There is no dependency on another
new draft PR, no shared header or production-source change, and no ROM data.

Source/compile iteration only. Independent review, behavior harnesses, full
tests, CI waits, and image/ROM gates remain with the independent checker.
