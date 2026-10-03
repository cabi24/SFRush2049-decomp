# D10 Packet 3: C13 input-controller wrappers

State: CLAIMED on 2026-10-03. Owner: independent C13 matching worker.
Branch: `dot/boot-tail-p3-c13`. Immutable base:
`76780b3a1b3e26c54b86e1f153344e92dac15b20` (Packet 2), stacked on
unmerged PR #59 at `21e104a22575cf4d639261d2f6d9535913074e6a`.
Packet 2 preflight was independently replayed with identical receipt JSON.

Exclusive matching interval: `[0x80021428, 0x80021548)`, nine native
32-byte functions, 288 bytes total. Edit scope: this directory plus those nine
`cloud/matches/boot_tail/func_80021*.c` files. The central packet owner alone
writes D10, STATUS.csv, research.json and generated central outputs.

Stop on strict-score failure after the directed -O2/-O1 comparison unless
native evidence justifies a bounded variant; diagnose near matches first.
Stop on extent, target integrity, ABI, or shared-header conflict. Targets,
scorer, boundaries, locks, shared headers and production gates stay unchanged.
No farm/runtime-image work, ROM/image inputs, promotion or merging.

## Local result

All nine natural C89 bodies match strictly at
`-g0 -O2 -mips2 -G 0 -non_shared`; the scorer additionally supplies
`-Wab,-r4300_mul`. Each native and compiled body is exactly 32 bytes, with
no alignment padding or excess words. Each has one `R_MIPS_26` relocation
at offset 8, resolved to the genuine external helper `func_80021150`.
All 72 words compare in full after relocation; no masking, unresolved symbols,
local-data assumptions or relocation errors. New locally matching bodies:
9 / 288 B. This is not cartridge coverage or maintainer acceptance.

| Function | Interior control pointer offset | O2 strict result | O1 control |
|---|---:|---|---|
| `func_80021428` | 196 | MATCH, 8/8 words | 7/8 differ, 2 extra words |
| `func_80021448` | 214 | MATCH, 8/8 words | 7/8 differ, 2 extra words |
| `func_80021468` | 232 | MATCH, 8/8 words | 7/8 differ, 2 extra words |
| `func_80021488` | 250 | MATCH, 8/8 words | 7/8 differ, 2 extra words |
| `func_800214A8` | 268 | MATCH, 8/8 words | 7/8 differ, 2 extra words |
| `func_800214C8` | 286 | MATCH, 8/8 words | 7/8 differ, 2 extra words |
| `func_800214E8` | 304 | MATCH, 8/8 words | 7/8 differ, 2 extra words |
| `func_80021508` | 322 | MATCH, 8/8 words | 7/8 differ, 2 extra words |
| `func_80021528` | 340 | MATCH, 8/8 words | 7/8 differ, 2 extra words |

One directed source per function was enough. Every function was compiled at
O2 first, then O1 as an evidence control. Native evidence for O2 is identical
per member: a 24-byte nonleaf frame, unchanged first pointer argument, interior
pointer formed in the call delay slot, and no argument spill. The O1 control
inserts a spill/reload of the first argument and a register copy; its aligned
text is 48 bytes. No tuning or near-match iteration was performed, so the
near-match diagnosis gate was not needed.

## ABI and source-family evidence

The wrappers consume one voice-object pointer and forward it unchanged as the
helper's first argument. Their second argument is a byte-wise interior pointer,
with the nine offsets shown above. Read-only inspection of the existing
`func_80021150` target proves use of both pointer arguments: the first is retained
for voice state; the second supplies a source count at offset 16 and four-byte
controller records. The helper masks its return to 16 bits at offset `0x2A8`.
Each wrapper returns that helper result unchanged. No extra formal, fabricated
callee body, struct-padding local, keeper, dummy call or assembly is used.

The byte pointer expression avoids inventing an unproved complete voice layout.
The true external helper is only declared. Its reconstruction is outside this
packet. Headers and symbol names remain unchanged.

Pinned [AxioDL/musyx snd_midictrl.c](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/snd_midictrl.c)
contains analogous `inpGetVolume`, `inpGetPanning` and other controller wrappers.
Its [CC0-1.0 license](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE)
is pinned to the same revision. This is a source-family lead only: that public
version forwards a third dirty-mask argument absent here and has a different
layout. Exact per-wrapper source names, original translation unit and N64
library version remain unproved. No third-party source is vendored.

## Reproduction and review

- `python3 cloud/work/boot_tail/C13-small/verify.py --check`
- `python3 -m unittest discover -s cloud/work/boot_tail/C13-small -p 'test_*.py' -v`
- Standard scorer, for each named source: `python3 tools/cloud/score.py fn
  cloud/matches/boot_tail/func_80021428.c func_80021428 --targets asm/us/boot_tail`

`scores.json` records target-body/source/compiler/tool hashes, full lengths,
relocations and both flag controls without copying target bytes or objects.
The three focused tests validate the inventory and receipts, compile all nine
sources under host C89/pedantic/Werror, and exercise all wrappers across 144
pointer/return-value scenarios with a test-only external-helper stub. This does
not reconstruct or validate the helper's behavior.

Independent C11-peer source/ABI review and strict replay passed at
`a50d5fa7b09b75c359dc3330c3d21df3a64c5145`; all nine source hashes remain unchanged.
The reviewer independently reproduced the receipt, three focused tests and nine
changed-submission MATCH results. In addition, the packet worker ran 630 existing
cloud setup/guard/submission/integrity/scorer regressions and 21 inherited metadata
tests, all passing with no skips. Aggregate exact-head CI remains pending.

Publication uses the single first-wave aggregate draft, targeting master to
trigger repository CI and explicitly depending on unmerged PR #59 plus completed
Packet 2. The central lead reconciles the conditional `status_delta.json`; no
source or verification logic was changed during aggregation. No production image
or ROM gates run. Merging remains with the owner's independent checker.
