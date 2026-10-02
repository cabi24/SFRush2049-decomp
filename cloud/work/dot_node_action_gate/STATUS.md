# Complete strict match: func_800CC848

## Scope and behavior

The new single-function source at `cloud/matches/func_800CC848.c` matches all
**32 words / 128 bytes** at `0x800CC848..0x800CC8C8` under normal IDO 5.3 O2.
The native body has a 24-byte stack frame and two consumed O32 inputs.

- Follow the first input through its object pointer and the node at object+8.
- Return 1 when that nested node is null.
- Otherwise follow node+0, load an unsigned index byte at owner+16, and inspect
  the signed enabled byte at +1 of the corresponding 772-byte table record.
- Return 0 when disabled, or 1 when enabled but the action input is zero.
- For an enabled record and a nonzero action, pass the real node to
  `func_800A1A60` and forward its return value.

The source's `Owner`, `Node`, `Object`, and `Car` types are minimal offset views,
not recovered original type names or asserted full object sizes. A valid outer
pointer chain is required; non-null nodes require valid owners and table entries.
The logical table capacity and indirect callers' contracts are not established.
No new null checks or bounds checks are introduced. No direct transfer to this
function was found in 1,216 protected functions / 140,252 words; that does not
exclude indirect callers or computed transfers.

## Matching improvement

The historical `tiny_A40` candidate remains unchanged. Its 8/32-word O2
register mismatch reproduces. A typed, naturally returned status variable
closes the difference: it is initialized to 1 for the real missing-node return,
then reused for the loaded enabled byte. Both uses are semantically meaningful.
This is ordinary standalone O2 C, with no helper context, fabricated operations,
unused pressure local, extra input, forced register, volatile access, artificial
padding, or scorer modification. Source line layout is retained for IDO replay.

## Verification

- Canonical strict scorer and clean-directory direct compiler replay: all 32
  full resolved words exact, ELF function size 128, and no alignment/excess words.
- Independent manual HI16/LO16 arithmetic resolves `D_80144030 + 1` to
  `0x80144031`; manual JAL arithmetic resolves the real `func_800A1A60`.
  Zero masks, unresolved/unverified relocations, or relocation errors.
- A separate reviewer independently recompiles the exact source, checks every
  word, and audits the complete wrapper and its 85-word real callee.
- Independent focused native-interpreter checks: **131,149 cases pass**.
- Repository CI-style suite: **1,299 passed, 41 skipped, 9 deselected**.
- IDO compile-time assertions verify pointer width and all accessed field offsets
  and the 772-byte table stride.
- **1,638,405 host ASan/UBSan semantic cases pass**: every unsigned index and
  enabled-byte pattern, null-node paths, boundary actions and forwarded values,
  delegate call count and node identity, and unchanged input/table memory.
  The test-only stub is never included in the matching compilation. The host
  fixture's 256 entries exercise the byte domain; they do not assert game capacity.
- C89 syntax and warnings pass; all 160 static locks remain intact and existing
  blob/group source-hash checks report no drift. All 23 protected manifest
  entries verify. Approved pinned IDO 5.3 static-recomp v1.2 is reused.

`verification.json` records exact hashes, compiler identity, proof methods,
semantics, checks and limitations without embedding native instruction words.

Reproduce:

```sh
python3 tools/cloud/score.py fn cloud/matches/func_800CC848.c func_800CC848 \
  --flags '-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
```

## Integration limits

This is a draft source contribution; **no accepted cartridge coverage is added**.
Accepted source, locks, native targets, scorer, build wiring and coverage remain
unchanged. Existing accepted claims and PRs #10–18 cover different functions.

The original ROM, complete extracted image and derived `build/blob_layout.json`
are unavailable. Source-image splicing, compressed-stream identity, full-ROM
SHA-1 and `make test` have **not run**. Live LAN ownership and normal image/ROM/lock
gates remain integrator requirements. Arcade source is unavailable; no direct
arcade equivalent is asserted. Host tests are not N64 execution.
