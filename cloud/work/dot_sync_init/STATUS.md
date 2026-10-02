# Complete sync_init_conditional match

Source: `cloud/matches/sync_init_conditional.c`.
Native extent: **0x800EE7BC–0x800EE820**, 25 instructions / 100 bytes.
Flags: `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.

## Purpose and reconstruction

The zero-input function reads the signed-byte initialization guard. If zero, it
sets the guard to one, creates a one-message libultra queue using the real global
message buffer, and jams a null message with flag zero. It ignores the jam
return value. Every path then sets the separate state byte to -1. A nonzero
guard skips both calls, including when the byte is negative.

The named `const s8 initialized = 1` is precisely the actual guard value, with a
local source lifetime before the branch. Ordinary IDO O2 then reproduces the
native address/constant scheduling. No helper, extra argument, artificial stack
object, volatile workaround, or invented operation is present. Historical
`tiny_A24` research remains unchanged. This is an N64-specific libultra queue
adapter; no direct arcade equivalent is established because the Rush The Rock
reference checkout is unavailable.

The protected entry at 0x800EE7BC includes the guard load. Historical source
naming at 0x800EE7C4 describes a suffix and must not establish an input parameter
or substitute for this full extent. The complete native function has a
`void(void)` interface and a 24-byte frame. Queue layout is independently checked
under IDO (24 bytes, message-count offset 16, message pointer offset 20).

## Verification

- Canonical strict scorer: 25/25 full resolved words exact.
- Clean-directory direct IDO replay; independent explicit HI16/LO16 and JAL
  arithmetic verifies all 14 relocation records and every complete instruction.
- A separate reviewer independently compiles the frozen source and resolves it
  with GNU ld; all 100 native bytes match. Single defined function, 100-byte ELF
  extent, three trailing zero alignment words, no nonzero excess instructions.
- Zero masks, unresolved or unverified relocations, or relocation errors.
- Complete native body and zero-input ABI reviewed; no direct transfer found in
  the 1,216-function / 140,252-word protected inventory. Indirect/computed callers
  are not excluded.
- 131,072 host guard/state/jam-result cases plus 131,072 repeated calls pass
  ASan/UBSan. Exact call arguments/order, guard-before-calls, reset-after-calls,
  ignored failure return, no calls/queue changes for nonzero guard, and unchanged
  message buffer are checked. C89 syntax/warnings pass.
- Cloud/scorer suite: 621 passed; repository suite: 1,299 passed, 41 skipped,
  9 deselected (both exit 0).
- Existing 160 static locks and all blob/group source hashes remain intact;
  all 23 protected manifest entries verify.
- Fresh master 2f1f30c508495db08d61170c4a9b2409461ae326 and open matching
  PR #10–18 were checked independently: no accepted lock/claim or PR overlap.

Reproduce from the repository root:

```sh
python3 tools/cloud/score.py fn cloud/matches/sync_init_conditional.c sync_init_conditional --flags '-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
```

## Scope and limits

Draft source contribution only: **no accepted cartridge coverage is added**.
Accepted source, locks, protected target bytes, scorer, generated context, build
wiring and coverage totals are unchanged. The guard update is not atomic;
thread-safety and real libultra queue scheduling are not established by host
mocks. Host tests supplement exact native bytes, not N64 execution. Leak
sanitization is disabled under sandbox ptrace; the harness allocates no heap.

The ROM, complete extracted image, and derived blob layout are unavailable.
Source-image splicing, compressed-stream identity, full-ROM SHA-1 and `make test`
have not run. Integrators must recheck live LAN ownership and perform normal
image/ROM/lock gates before promotion. See `verification.json` for hashes and
machine-readable proof.
