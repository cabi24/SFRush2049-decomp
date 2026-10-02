# Complete func_8008B32C matrix scale match

Source: `cloud/matches/func_8008B32C.c`.
Native extent: **0x8008B32C–0x8008B3A0**, 29 instructions / 116 bytes.
Flags: `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.

## Reconstruction

A natural pair of three-iteration loops scales each element of a 3-by-3 matrix.
The function consumes source and destination pointers in a0/a1 and the third
float argument's bits in a2 (normal O32 mixed-argument ABI). The native entry
moves a2 to f12. Ordinary IDO O2 produces the original peeled inner loop with
row-major loads/stores. No helpers, extra arguments, artificial locals, volatile
accesses, register controls, padding operations or scorer changes are used.
In-place use is supported; no `restrict` or bulk-copy assumption is added.

## Verification

- Canonical strict scorer: **29/29 full words exact**.
- Clean direct pinned-IDO replay independently compares every raw instruction.
  No relocations, masks, unresolved/unverified symbols, or nonzero excess words.
  ELF function extent is 116 bytes; three zero alignment words are excluded.
- A separate reviewer independently compiles and uses GNU objcopy to compare
  all 116 native bytes, checks the full ABI and 16 direct call sites, including
  in-place use and float constants carried in a2.
- **500,000 ASan/UBSan host cases** pass: disjoint, in-place, row-aligned forward
  and backward overlap; all surrounding elements preserved; random float bit
  patterns include zeros, infinities, NaNs and subnormals. NaNs are compared by
  class, not payload. C89 syntax/warnings pass.
- Repository suite: **1,299 passed, 41 skipped, 9 deselected**, exit 0.
- Existing blob/group source hashes: zero problems.
- All **160 static locks** remain intact; protected native manifest verifies
  all 23 entries. Existing accepted source/locks are unchanged.
- Fresh master and PR #1–20 are checked for overlap. This symbol is absent from
  the 834-symbol conservative exclusion union of accepted locks, cloud singles,
  group claims, earlier PR targets/mentions and prior task exclusions.

Reproduce from repository root:

```sh
python3 tools/cloud/score.py fn cloud/matches/func_8008B32C.c func_8008B32C --flags '-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
```

## Scope and limits

Draft source contribution only: **no accepted cartridge coverage is added**.
Accepted source, locks, protected targets, scorer, generated context, build
wiring and coverage totals are unchanged. The 3-by-3 matrices must refer to
valid accessible float rows. No direct arcade equivalent is established because
the reference checkout is unavailable. Host tests do not establish N64 FCSR
rounding/exception flags, NaN payloads or subnormal behavior on console.

Original ROM, complete extracted image and derived blob layout are unavailable.
Source-image splicing, compressed-stream identity, full-ROM SHA-1 and `make test`
have not run. Integrators must recheck live LAN ownership and normal image/ROM/
lock gates before promotion. See `verification.json` for hashes and evidence.
