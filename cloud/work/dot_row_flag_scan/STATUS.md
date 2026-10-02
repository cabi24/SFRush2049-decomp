# Complete four-byte-row flag scan: strict O2 match

Base: `cabi24/SFRush2049-decomp` master
`2f1f30c508495db08d61170c4a9b2409461ae326` (2026-10-02).

## Result

`func_800F7564`, `0x800F7564..0x800F75B0`, is a complete **19-word / 76-byte**
strict match through the unchanged canonical cloud scorer. The ELF function
symbol is exactly 76 bytes. The emitted text is 80 bytes, with one trailing zero
alignment word excluded from the claim. No helper or caller context is compiled.

The source is `cloud/matches/func_800F7564.c`, compiled with
`-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul` under pinned IDO 5.3.
There are no masks, unresolved or unverified relocations, relocation errors,
or nonzero excess instructions.

## Reconstruction and semantic evidence

The older `cloud/work/tiny_A24/func_800F7564.c` used a manually constructed
pointer/end-pointer loop. It reproduces a strict 19/19-word nonmatch with two
nonzero excess instructions. The canonical workbench diagnosed a structural
mismatch. The final source expresses the same native scan as a plain indexed
`for` loop; IDO naturally creates the target's pointer induction and bound.
The frozen historical candidate and its research archive remain unchanged.

The function takes one full-word field index in `a0`, reads a signed halfword
row count at `D_8014A108`, and scans selected signed bytes from `D_80150E88` with
a four-byte row stride. A nonpositive count returns zero without reading table
bytes. A positive count visits every row from zero through count minus one;
any nonzero selected byte sets the result to one. It continues scanning after
a hit and returns the result in `v0`. The native body has no stores, calls,
stack frame, or other incoming arguments.

The source's four-byte rows preserve the actual stride. Valid field subscripts
are 0 through 3; no caller-derived promise about wider flat-storage indices is
made. No table capacity or bounds check is invented. The neighboring accepted
`func_800F7644` uses the same signed count and the same natural indexed-scan
pattern on a different table; its source hash remains locked and its independent
fresh scorer replay passes. Its matching bytes receive no additional credit.

All 19 native instructions were reviewed and every instruction address through
the return delay slot was accounted for. The full protected inventory was
scanned for direct jumps/calls and PC-relative transfers to the target: none
were found across 1,216 functions / 140,252 protected words. This does not exclude
indirect callers or computed transfers. A separate reviewer replayed the final
source in an isolated worktree, confirmed strict MATCH, and independently
checked the one-input ABI, signed count, stride, loop termination and return.

No fabricated runtime operation, stand-in, extra argument, volatile access,
uninitialized local, forced register, or artificial stack padding is used.
The primary arcade-source checkout is absent, so no direct arcade equivalent
or original developer identifier is asserted. This is an N64-side table query;
portable arcade ancestry has not been established.

## Reproduce

```sh
python3 tools/cloud/score.py fn cloud/matches/func_800F7564.c func_800F7564 \
  --flags '-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
```

Expected output:

```text
func_800F7564:
  MATCH
```

The approved compiler archive is pinned by `tools/cloud/setup.sh` to SHA-256
`ab5c741561f80913d58c8b074771f23941a3edd312505a8ebed6d1dfeb65e506`.
The existing approved installation was reused; compiler executable hashes are
recorded in `verification.json`.

A clean-directory replay copied the final source and invoked the pinned IDO
driver directly. Independent full-word arithmetic applied the two HI16/LO16
pairs for `D_8014A108` and `D_80150E88`, including the signed-low-half carry.
Both addends are zero. All 19 fully resolved words equal the protected target.
Canonical relocation and comparison also pass. The sanitized proof records
source, compiler, object, resolved-body, target, scorer and manifest hashes,
without committing native instruction streams or compiled objects.

## Additional validation

- Host C89 syntax and warnings: pass with `-std=c89 -pedantic -Wall -Wextra -Werror`
- Host semantics: 5,156 checks pass under AddressSanitizer and UndefinedBehaviorSanitizer
- Cases cover counts -32768/-1/0/1/2/7/8/32767, four field positions, byte values
  -128/-1/1/127, active/inactive rows, untouched other columns, all 256 occupancy
  patterns over eight rows, and the last row at maximum signed-short count
- Table contents remain unchanged; no early bounds checks or short-circuit return
  are introduced. Native full-word identity establishes the exact access sequence
- Leak detection is disabled because it cannot operate under this sandbox's
  ptrace environment; the host test performs no dynamic allocation
- CI-style repository suite: 1,299 passed, 41 skipped, 9 deselected, exit 0 (56.39 seconds)
- Core cloud/scorer suite: 840 passed, 27 subtests passed, exit 0 (187.91 seconds)
- Broad cloud-related test selection also completed with exit 0
- All 160 static locked functions intact
- Existing blob and group source-hash checks: zero problems
- All 23 protected manifest entries verify

The host checks supplement native byte identity; they do not execute an N64
binary. Changed-submission scoring and protected-path checks run against the
final submitted revision. Exact-head CI status is reported on the draft PR.

## Limits and integration

This draft source contribution adds **no accepted cartridge coverage**. Accepted
C, locks, coverage totals, build wiring, protected native targets and scorer are
unchanged. Only the new cloud source and its two evidence documents are added.
The target is absent from accepted locks and current matching PRs; this lane is
separate from the parallel `sign_extend_call` contribution.

The original ROM, full extracted game image, and derived `build/blob_layout.json`
are unavailable. Source-image splicing, compressed-stream identity, full-ROM
SHA-1, and `make test` have not run. Source-hash checks do not replace these gates.
Live LAN coordinator ownership must be rechecked by the integrator before normal
image/ROM/lock promotion. This contribution performs no merge or promotion.
