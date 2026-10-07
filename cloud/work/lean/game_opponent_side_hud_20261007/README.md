# Opponent-side HUD callback: lean research

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_80107EDC`, `[0x80107EDC,0x80108154)`, **632 bytes / 158 words**.
Observed canonical O3 comparison improves **109/158 to 21/158 differing words**.
Candidate extent is 636 bytes, with **one nonzero excess word** and no unresolved
symbols, unverified relocations or errors. This remains NONMATCH research.

## Grounded change and prior work

The complete body already exists at frozen
`cloud/work/ipa-groups/dynamic_difficulty/dd.c`; this is not a newly reconstructed
function. That directory's STATUS describes the same two missing slot-return
copies. The old dynamic_difficulty symbol is a tail label, not this function's
entry. Current protected manifests supply the registered complete head/extent.

Both direct queue/slot/queue sequences are replaced by the established returning
font_set wrapper from `cloud/work/s20261004/E/src/helper.h`. The wrapper returns
the prior font after releasing the queue; its inlining preserves the two native
return copies naturally. No extra input, pressure local, padding, new volatile,
dead condition or fake call was added.

The former four-context-body group contains explicit stand-ins and artificial
dead conditions, so it is not reused. This packet instead uses the genuine
compact slot_state_setup body and corrected PR220 countdown callback. The latter
is an actual font helper caller and preserves its native out-of-line boundary.
The callback SHA256 is
`d41ecea76cddbccd58af80410a26666fa283e45beb5bd0a6d21cd712b1b43837`.

This target renders a formatted, shadowed opponent-side label for eligible
players; historical sound/music names describe text functions here. The formatter
music_tempo_adjust has a real variadic interface in frozen
`src/blob/music_tempo_adjust.c`. Its format word is modeled as a pointer, and
queue returns / slot return types are normalized to the real helper contracts.
These declaration corrections alone are score-neutral.

## Observed controls

- Prior complete dd.c in the genuine two-caller context: 109/158, 628 bytes,
  zero nonzero excess, no relocation diagnostics.
- Same body with corrected declarations/text pointer: identical 109/158.
- Same context plus returning wrappers: 21/158, 636 bytes, one nonzero excess,
  no relocation diagnostics.
- Context slot_state_setup: 18/58, own extent 232 bytes, two canonical excess
  words in the next-symbol span. It is not claimed.
- Prior PR220 countdown context: 105/150, own extent 604 bytes, four canonical
  excess words in the candidate's next-symbol span (one in paired controls).
  It is not claimed and is not a new research result here.

## Reproduce and assumptions

```sh
python3 tools/cloud/score.py group cloud/work/lean/game_opponent_side_hud_20261007
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`, plus canonical mandatory assembler
`-r4300_mul`. For the paired baseline replace callback.c with the pinned frozen
dd.c while retaining the packet's real slot/countdown files and group recipe.
The scorer, protected target manifests and accepted production owners are untouched.

Native O32 pointer, text-format, car/racer layout and queue contracts are assumed.
The complete body retains the prior native-derived 2056-byte car and 952-byte
racer strides. This is an inferred real compilation context, not a recovered
original translation unit or proof of arbitrary input/aliasing/concurrency safety.
Empty group claims remain authoritative. Only C, required real context, recipe
and observed notes are included. No independent review, behavior harness, full
suite, CI wait, acceptance, integration or merge was performed. The independent
checker owns verification/acceptance and ROM integration. No ROM bytes, raw
assembly dumps, binaries or credentials are included.
