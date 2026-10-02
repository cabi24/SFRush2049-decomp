# Four-byte cached-state update: func_800B7360

## Result

Complete 132-byte (33-word) natural C function match at
`0x800B7360..0x800B73E4`, compiled alone with the exact source-header flags.
All words match after resolving both real data-address relocation pairs;
there are no masks, unresolved symbols, unverified references or extra body words.
The ELF function symbol is exactly 132 bytes. Three following alignment words
are zero and are outside the function.

The function compares four unsigned-byte arguments against a four-byte cache.
When any differs it stores all four bytes, then clears the associated full word.
When all four match, neither cache nor word is changed. The cache/invalidation
interpretation is descriptive; no specific arcade equivalent is established.
Reference arcade checkout is unavailable in this environment.

## Source and ABI review

The C uses four unsigned-byte parameters, one natural short-circuit condition,
four byte assignments and one word assignment. It has no helper functions,
assembly, artificial locals, padding, volatile qualifiers or ABI stand-ins.
Its argument narrowing matches the target. It is a leaf with no stack frame;
the four writes at 0/4/8/12(sp) are ordinary incoming argument home slots.
No callee-saved register is touched. It returns void and does not expose the
target's scratch register contents as a manufactured return value.

A scan of all 1,216 canonical game functions finds four direct JAL sites in
`dispatch_handler` (two), `world_velocity_integrate`, and `race_finish`.
The inspected sites provide byte arguments in a0-a3, with the last race-finish
argument equal to 255. This does not rule out indirect/computed calls.

## Verification

- Canonical scorer: strict MATCH, 33/33 full words.
- Independent peer review: fresh pinned IDO compile and GNU linker replay
  verify all 33 words and four HI/LO relocations; 36,736 independent host cases
  cover all 16 combinations of changed components and byte-edge/random cases.
- Fresh direct IDO 5.3 compile: separate explicit HI16/LO16 carry arithmetic
  resolves both cache and associated-word addresses; full words equal target.
- Semantic host tests: 17,680 cases passed, covering all 256 equal uniform
  byte values, every individual mismatching position, six preserved-word
  sentinels, randomized inputs and unsigned argument truncation.
- Repository suite: **1,307 passed, 41 skipped, 9 deselected**, exit 0.
- All 161 static locks intact; accepted blob and group source hashes have no
  problems. Protected target manifest passes all 23 entries.
- Eligibility checked against fresh master 2989bf5, accepted locks, existing
  cloud singles, group claims, and PR #1-23 before probing. Rechecked on
  a12daa63 after the maintainer integrated earlier submissions; still unowned.

Reproduce:

```sh
python3 tools/cloud/score.py fn cloud/matches/func_800B7360.c func_800B7360 --flags '-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
```

## Scope

Draft source contribution only; no accepted coverage or ROM claim. Accepted
source, locks, scorer, protected native targets and build wiring are unchanged.
Original ROM, complete extracted image and derived blob layout are unavailable;
image splicing, compressed-stream identity, full-ROM SHA-1 and `make test` have
not run. Integration still requires live ownership and normal image/ROM gates.
