# Countdown/results digits HUD: lean research

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_801084D4`, `[0x801084D4,0x80108AB0)`, **1500 bytes / 375 words**.
Observed canonical O3 comparison improves **326/375 to 273/375 differing words**.
Candidate extent 1508 bytes, **two nonzero excess words**, zero unresolved
symbols, unverified relocations or errors. This is NONMATCH research.

## Prior body and native-backed corrections

The existing handwritten `cloud/work/ipa-groups/catchup_logic/cu.c` contains the
full general loop but is not a semantic reference: it explicitly declares twelve
unused padding locals and omits the native per-player opacity reset. These
limitations are retained only in the pinned baseline, not in this candidate.
This is a corrected existing body, not a claim of a newly discovered function.

Changes justified by the native accesses/calls and accepted helper contracts:
- Remove all d0_/f0_..f10_ padding locals and unused adv/s locals.
- Restore func_800ED66C(-1.0f) after every completed eligible player's draw loop.
- Load that player's full-word y coordinate before the opacity callback, then
  reload the count-dependent x coordinate after it; cast y only at draw calls.
- Use the actual s32 camera_shake_update(u16) width return, rather than narrowing
  each width to u8, and a signed-byte saved object mode.
- Normalize slot/queue return declarations; use the existing returning font_set
  wrapper for font 1 or 2. It naturally retains native return copies after IDO
  inlining without dummy operations, pressure locals or new volatile qualifiers.

The ten-byte digit buffer and two-byte single-character string are unchanged
meaningful storage from the prior body. Nine digit/punctuation bytes are written
and walked; the draw helper receives the two-byte terminated string. Capacities
were not tuned for matching. The ordinary float frame-time declaration is kept;
no asynchronous-mutation hypothesis is added.

The old catchup group includes explicit stand-ins and artificial dead conditions.
It is not reused. The packet uses the compact genuine slot_state_setup and real
corrected PR220 countdown callback instead, preserving the slot helper's
out-of-line boundary. Countdown source SHA256:
`d41ecea76cddbccd58af80410a26666fa283e45beb5bd0a6d21cd712b1b43837`.
The returning wrapper derives from frozen
`cloud/work/s20261004/E/src/helper.h`.

## Observed controls

All comparisons use the same genuine slot/countdown context:
- Frozen prior cu.c: 326/375, 1452 bytes, no nonzero excess. Its missing reset and
  padding declarations mean it is only a compiler control, not a behavior oracle.
- Complete corrected body with direct lock/slot/unlock calls: 312/375, 1500 bytes,
  no nonzero excess.
- Complete corrected body with returning wrappers: 273/375, 1508 bytes, two
  nonzero excess words. All three have no relocation diagnostics.

Context slot_state_setup remains 18/58, own extent 232 bytes, two canonical
next-symbol excess words. The prior PR220 countdown body stays 105/150, own
extent 604 bytes, four canonical next-symbol excess words in the candidate
(one in direct-call controls). Neither context is claimed or newly matched.

## Reproduce and assumptions

```sh
python3 tools/cloud/score.py group cloud/work/lean/game_countdown_digits_hud_20261007
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`, plus mandatory canonical assembler
`-r4300_mul`. Recover the prior control by substituting frozen cu.c for callback.c
while retaining the real group context. Recover the corrected direct-call control
by replacing the two font_set calls with the equivalent queue/slot/queue sequences.
The scorer, protected targets and accepted production files are unchanged.

Native O32, valid racer/layout, text and queue contracts are assumed. This is not
proof of original declarations or translation-unit ownership. Frame/allocation
and scheduling differences remain, and no arbitrary-state, concurrency, aliasing
or gameplay safety claim is made. Empty group claims remain authoritative.
Only C, required real context, recipe and observed notes are included. No
independent review, behavior harness, full suite, CI wait, acceptance, integration
or merge was performed. The independent checker owns verification, acceptance
and ROM integration. No ROM bytes, raw assembly dumps, binaries or credentials.
