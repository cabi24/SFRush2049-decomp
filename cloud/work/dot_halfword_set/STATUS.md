# Complete indexed unsigned-halfword setter: strict O2 match

Base: `cabi24/SFRush2049-decomp` master
`2f1f30c508495db08d61170c4a9b2409461ae326` (2026-10-02).

## Result

`func_80090770`, `0x80090770..0x8009079C`, is a complete **11-word / 44-byte**
strict match through the unchanged canonical cloud scorer. The ELF function
symbol is exactly 44 bytes. The entire emitted text is 48 bytes, with one trailing
zero alignment word excluded from the claim. There is no helper or caller context.

The source is `cloud/matches/func_80090770.c`, compiled with
`-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul` under pinned IDO 5.3.
No masks, unresolved or unverified relocations, relocation errors, or nonzero
excess instructions are allowed.

## Source and type evidence

The older `cloud/work/tiny_A23/func_80090770.c` describes the right indexed
halfword write but gives both inputs signed-short types and puts its body on one
line. It reproduces a strict 7/11-word nonmatch. Keeping that signed value type
and using normal statement lines still leaves four differing register operands.
The corrected unsigned value type plus normal statement lines matches all words.
The historical source and its research archive remain unchanged.

The native index is sign-extended from the low 16 bits of `a0`. The address is
`0x8012E714 + index * 0x44`. The low halfword of `a1` is written there, and both
incoming arguments have the ordinary home stores at `sp+0` and `sp+4`.
No third input is consumed and no result register is defined. The source therefore
uses `void func_80090770(s16 index, u16 value)`.

The store instruction alone cannot establish value signedness. The existing
`camera_aspect_ratio` group declares `D_8012E714` as `u16` and writes the same
field with explicit unsigned-halfword lvalues at this exact stride. The matching
original-IDO output provides further evidence for the unsigned parameter.
The neighboring accepted setter `func_800A785C` independently uses the same signed
index and 0x44 stride for another word in these records.

`IndexedRecord44` is a minimal view of the native offsets: the field is at 0x14
within each 0x44-byte record based at `D_8012E700`. Opaque bytes describe the
existing record layout; they are not stack padding or additional memory accesses.
The setter preserves native behavior without introducing range checks. No array
capacity or valid negative-index domain is inferred from this function.

The full protected inventory was scanned for direct jumps/calls and PC-relative
branches to the function; none were found across 1,216 named functions and
140,252 protected words. This does not rule out indirect callers or computed
transfers. The primary arcade-source checkout is absent, so no original arcade
function name or direct equivalent is claimed. This is an N64-side state-field
setter; no portable arcade ancestry is established.

## Reproduce

```sh
python3 tools/cloud/score.py fn cloud/matches/func_80090770.c func_80090770 \
  --flags '-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
```

Expected output:

```text
func_80090770:
  MATCH
```

The approved compiler archive is pinned by `tools/cloud/setup.sh` to SHA-256
`ab5c741561f80913d58c8b074771f23941a3edd312505a8ebed6d1dfeb65e506`.
The existing approved installation was reused; its executable hashes are recorded
in `verification.json`.

An independent clean-directory replay copied the final source and called the
pinned IDO driver directly. It resolved the HI16/LO16 pair by separate, explicit
full-word arithmetic: `D_8012E700 + 0x14 = D_8012E714`. All 11 resulting words
are exactly equal to the full protected target. Canonical relocation and comparison
also pass. The verification record includes source, compiler, object, target,
resolved-body, scorer, and protected-manifest hashes, without publishing target
words or compiled objects.

## Additional validation

- Host C89 syntax and warnings: pass with `-std=c89 -pedantic -Wall -Wextra -Werror`
- Host layout/behavior checks: 15 cases, at indices 0/3/7 with values
  0/1/0x7FFF/0x8000/0xFFFF; only the selected two-byte field changes
- Host assertions confirm record size 0x44 and field offset 0x14
- Cloud/scorer suite: 840 passed, 27 subtests passed, exit 0 (244.39 seconds)
- CI-style repository suite: 1,299 passed, 41 skipped, 9 deselected, exit 0
  (59.06 seconds)
- All 160 static locked functions intact
- Existing blob/group source-hash checks: zero problems
- All 23 protected manifest entries verify

The host cases supplement native byte identity; they are not execution of an N64
binary. Changed-submission scoring and protected-path checks run against the final
submitted revision; exact-head CI status is reported on the draft pull request.

## Limits and integration

This draft source contribution adds **no accepted cartridge coverage**. Accepted
C, locks, coverage totals, build wiring, protected native targets and the scorer
are unchanged. Only the new cloud submission and its two evidence documents are
added. The target is absent from accepted locks and other current matching PRs;
this lane is separate from the parallel `func_800BEA3C` contribution.

The original ROM, full extracted game image, and derived `build/blob_layout.json`
are unavailable here. Source-image splicing, compressed-stream identity, full-ROM
SHA-1, and `make test` have not run. Source-hash checks do not replace those gates.
Live LAN coordinator ownership must be rechecked by the integrator before normal
image/ROM/lock promotion gates. No merge or promotion is performed by this work.
