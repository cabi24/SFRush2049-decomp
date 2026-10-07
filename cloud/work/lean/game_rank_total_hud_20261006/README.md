# LEAN RESEARCH: rank/total HUD callback (race_finish)

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`. Native address 80107AF4, 1000 bytes / 250 words, frame 280.

This is a first complete native-backed C reconstruction of the callback historically named `race_finish`. The native body draws a player's position, a slash and the total as three separately styled glyphs; the speculative race-completion implementation in `src/game/game.c` is not used. Both the frozen definition index and PR163 supplement were checked for existing native-backed C before this work.

## Observed code generation

IDO 5.3 `-g0 -O3 -mips2 -G 0 -non_shared`; canonical automatic `-Wab,-r4300_mul`:

- Initial complete direct-call C (`direct_baseline.c`): **221/250 differing words**, 964 bytes/frame 200, zero excess.
- Same reconstruction using the genuine returning font abstraction, standalone: 221/250, 996 bytes/frame 248, zero excess.
- With the actual font helper and another real caller: **214/250**, **1008 bytes/frame 248**, **two excess words**.

The retained group improves differing words but trades zero excess for two and remains 32 frame bytes short. No unresolved/unverified sites or scorer errors are reported. It is nonmatching research, not acceptance. The baseline here is valid newly reconstructed C, not an empty or placeholder seed.

## Complete behavior and real storage

- Preserves state, total-count, gameplay-mode and enable gates; rendering/alignment setup and teardown.
- Selects the actual fonts and snapshots the width of glyph 56. Eligibility uses the real `s32 func_800CF604(s16)` predicate, followed by the player's signed mode byte.
- Uses 952-byte player records with position/mode bytes at +0xEE/+0xEF and the actual 32-byte-per-player-count coordinate rows.
- Snapshots total count into the third glyph, writes the current player's rank into the first, and renders each character with its shadow and proper font choice. The first glyph uses the actual player's RGB color and alpha 255; the remaining glyphs use the real color-mode helper.
- Reloads player count at the native helper boundaries and keeps the initial glyph advance width across the rendering calls.

Only two tiny meaningful arrays are needed: three unterminated glyph bytes and a two-byte single-character string including its terminator. Their complete native reads/writes establish their logical storage. No large capacity is invented to fill the native 280-byte frame, and no unused locals, artificial callers, volatile pressure or assembly are added.

The unused first ABI word is represented as `u32 callback_argument`; its original signedness/pointer type is unresolved. Record names describe observed fields rather than recovered original declarations. Valid local-player/table indices, sufficient records, valid renderer/font state and intended representable coordinate arithmetic are assumptions. Rank/total bytes retain native low-byte conversion; no extra digit formatting or range validation is introduced.

## Genuine context and minimal replay

The gfx-lock/font-select/unlock wrapper is the real source abstraction in frozen `cloud/work/s20261004/E/src/helper.h`. `slot_context.c` is the actual credits-scroll font body normalized as in PR #215. `countdown_caller.c` is the unchanged real callback from corrected PR #220. These are genuine call sites, not stand-ins. Context remains nonmatching and unclaimed; full sound-update IPA and the larger native frame remain unresolved.

With project IDO and MIPS tools configured:

```
python3 cloud/work/lean/game_rank_total_hud_20261006/replay.py --repo .
```

The replay reads frozen tools and protected target/manifests into temporary storage, builds the included initial/candidate C and real context, and prints score metadata and source identities. No behavior/acceptance checks, live-source changes, locks, splice claims or ROM integration are added. Independent verification, acceptance and merging belong to the checker.
