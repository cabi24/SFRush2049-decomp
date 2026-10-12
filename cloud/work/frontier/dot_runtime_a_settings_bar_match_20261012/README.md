# Image-A settings bar: 772 newly strict candidate bytes

## Deliverable

`groups/dot_runtime_a_settings_bar_20261012/` is a complete one-member kept
callback group. Its member and claim are `func_803AE63C`, native interval
`[0x803AE63C,0x803AE940)`: **193/193 words MATCH, 772 bytes**. The genuine
callback remains externally kept, with no invented caller or extra argument.
No prior matching body is counted again; no existing group/lock is migrated.

Baseline: current master `3bec9ce30`, whose older complete source in
`cloud/work/runtime_a_settings_bar_20261006/candidate.c` differs at four words.
New matching candidate bytes: **772**. Accepted-image and ROM bytes: **0**.
Installation, cartridge verification and merging belong to the independent checker.
Keep the pull request draft.

## Trace-first change

The old pointer declaration is optimized away, but reserves four bytes of CFE
local storage. The expression derived from it still requires a spill home.
Read-only stock-fidelity `uopt` instrumentation establishes:

- CFE local area: 20 → 16 bytes
- Derived-address expression home: −24 → −20
- Final local area: 32 → 28 bytes
- Aligned frame: 64 bytes on both sides
- Actual saves/reloads: stack+40 → native stack+44

The only semantic-source rewrite removes `MenuSlot *slot` and accesses the
same entries directly through `D_803B6B14[marker+21]`. All four prior differing
instructions at +0x8C, +0x9C, +0xD0 and +0xE4 become exact. No unused local,
padding, artificial read, volatile, compiler patch, buffer resizing or ABI
change is used. Both diagnostic objects are byte-for-byte identical to the
corresponding ordinary stock compiler objects. `trace.json` records the
compact facts; instrumented output is diagnostic only, never match evidence.

The three existing external angle literals are also replaced with their
natural float spellings. The resulting twelve owned bytes independently
verify at `0x803B99D4..0x803B99E0`; the same complete code remains exact.
The original inferred record views, real two-halfword output buffer, full
sixteen-bit marker and signed-byte Hidden return are preserved. The earlier
packet documents the native NewMultiBlit/frame-dispatch callback consumers
and the actual projection helper's two-halfword output. Their current native
bodies are bound in the new receipt. No exact arcade donor is claimed.

## Verification

```
python3 cloud/work/frontier/dot_runtime_a_settings_bar_match_20261012/verify.py
python3 -O cloud/work/frontier/dot_runtime_a_settings_bar_match_20261012/verify.py
python3 cloud/work/frontier/dot_runtime_a_settings_bar_match_20261012/verify.py --score
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_a_settings_bar_match.py -o addopts='' -q
```

Canonical output, without `--claims` or `--allow-unverified`:

```
Members:
func_803AE63C:
  MATCH
    own .rodata verified at 0x803B99D4..0x803B99E0
```

The `--score` wrapper authenticates the current committed asset, loader
pointer and complete image-A digest before supplying owned-data evidence to
the unchanged canonical scorer. Bare cloud scoring without this image data
reports unverified references; missing data is never treated as a match.
The source is packaged as a group rather than a standalone `cloud/matches`
entry because overlay owned-data artifacts are not committed.

Independent GNU linking checks the original complete object: all 21 original
relocations, 772-byte STT_FUNC, twelve zero text-padding bytes, all literal
bytes and four zero data-padding bytes. Explicit section addresses preserve
the native non-16-aligned text and rodata placements. Every linked word and
owned-data byte must equal authenticated native inputs, with no retained
relocation or unexpected allocated section. Absent owned data and a corrupted
literal are both rejected. The old source still reproduces exactly 4/193.

The v2 receipt pins source, verifier, trace summary and consumed native target
rows, not shared tooling, whole locks or assembly-file text. The single test
checks the input receipt before its toolchain portion and uses the shared
missing-tool policy. No ROM bytes, raw assembly, extracted images, compiler
objects, credentials or unrelated data are published.

These are complete code/data/extent matching proofs. No new gameplay,
hardware, compression, full-image or ROM behavior result is claimed.
