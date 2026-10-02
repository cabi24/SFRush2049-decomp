# Complete signed-index call wrapper: strict O3 match

Base: `cabi24/SFRush2049-decomp` master
`2f1f30c508495db08d61170c4a9b2409461ae326` (2026-10-02).

## Result

`sign_extend_call`, `0x8008E398..0x8008E3C0`, is a complete **10-word / 40-byte**
strict match. The standard whole-program IDO O3 pipeline compiles one genuine
wrapper, with four real inputs, one signed-halfword cast, and one real external
callee declaration. There are no helper implementations or caller contexts.

The canonical scorer and a separate clean-directory compiler replay both match
all full resolved words. A further direct JAL relocation calculation independently
checks the call field and compares all ten words without using the scorer's
relocation routine. There are zero masks, unresolved or unverified relocations,
relocation errors, or nonzero excess words. The ELF function is 40 bytes; the two
zero alignment words in its 48-byte text section receive no matching credit.

## Source and ABI evidence

The historical `cloud/work/tiny_A23/sign_extend_call.c` is an unclaimed 4/10-word
O2 nonmatch. That exact source produces a 2/10-word nonmatch through normal O3:
only the adjacent return-address store and arithmetic-right-shift order differs.
Moving its call onto its own readable source line makes the original bitshift
expression match. The final `(s16) index` is the ordinary C representation of the
same signed-low-half conversion and also matches. Thus both the compiler recipe
and normal statement boundaries matter. No historical candidate or research
archive is changed.

The entire ten-word wrapper and 75-word real `func_8008E26C` callee were reviewed:

- The wrapper passes the first, second and fourth inputs through unchanged
- It sign-extends the third input's low 16 bits into the third call argument
- It uses a 24-byte frame solely for the ordinary call convention and saves the
  return address at offset 20; there are no local buffers or invented formals
- The callee consumes all four inputs: it writes the low half of the first to
  record offset `0x14`, the second full word to offset 8, and the fourth to
  offset 0 in a 68-byte resource record
- The callee itself narrows the third input to signed 16 bits and forwards it
  to `func_8008E19C`; it returns a signed-halfword record index in `v0`

The callee is declared with its genuine signed third parameter and `s32` return.
The wrapper keeps the historical and generated-context `void` return. No direct
caller of the wrapper was found across all 1,216 protected functions / 140,252
words, including the direct branch scan. Indirect references are not ruled out,
and original typedef spelling and source-level return intent are not recovered.
The native wrapper leaves the callee's `v0` undisturbed but does not itself write
it. This contribution does not assert that callers use that value.

The three pass-through inputs are retained as raw 32-bit values, without extra
narrowing. Naming remains conservative. The primary arcade reference tree is
unavailable; no direct arcade equivalent is claimed. This is N64 resource-record
code. There are no volatile operations, fabricated helpers, extra parameters,
stack-padding objects, assembly insertion, target edits, or scorer changes.

## Reproduce

```sh
python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_sign_extend --claims
```

Expected output:

```text
Members:
sign_extend_call:
  MATCH
```

`group.json` records the ordinary recipe:
`-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
The already-approved, project-pinned IDO 5.3 static-recomp v1.2 installation is
reused. `verification.json` records source/spec/compiler/scorer hashes, complete
word-count proof, independent relocation arithmetic, and semantic review.

## Integration limits

This is a draft source contribution, **not accepted cartridge coverage**. Only
four files under this new group directory are added. The `claims` field requests
strict CI rescoring; accepted C, locks, protected targets, generated context,
scorer, build wiring, and coverage totals are unchanged. The target is absent
from current accepted locks and other current cloud claims. Draft PRs #10–14
cover different functions.

The original ROM, full extracted image, and derived `build/blob_layout.json`
are unavailable. Source-image splicing, compressed-stream identity, full-ROM
SHA-1, and `make test` have not run. Read-only source-hash checks do not replace
those gates. Live LAN coordinator ownership must be rechecked by the integrator
before the normal image/ROM/lock promotion gates.

## Validation

- Canonical scorer, clean-directory replay, and direct relocation arithmetic: pass
- C89 syntax and warning check: pass
- Host functional check with undefined-behavior sanitizer: **196,608 cases pass**,
  covering every low-half pattern in three high-half/sign forms, one real call,
  and unchanged first/second/fourth inputs; test-only callee stub is not submitted
- All **160 static locks** intact; blob/group source-hash checks: **zero problems**
- All **23 protected manifest files** verify
- Repository test suite: **1,299 passed, 41 skipped, 9 deselected**, exit 0
- Cloud/scorer suite: **840 passed, 27 subtests passed**, exit 0

Aggregate commands:

```sh
python -m pytest tests/cloud tests/conveyor/test_cloud_score.py -rs
python -m pytest tests/conveyor -q -o addopts='' -m 'not node_required'
make check-matched
```

Independent peer review in a separate worktree also rebuilt the exact final source
to strict MATCH and reviewed all 85 wrapper/callee words. All four input roles and
the signed-low-half conversion were corroborated, with the return-intent limit
retained. Changed-submission strict scorer and protected-path guard both pass.
