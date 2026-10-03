# Packet 2: target/toolchain preflight

PASS on 2026-10-03. This packet adds zero matching bodies/bytes.

- Working branch: `dot/boot-tail-p2`.
- Explicit stacked base: draft PR #59 at `21e104a22575cf4d639261d2f6d9535913074e6a`, tree `1018b58fb08e0c7abe921095dfac01e70322b4d3`.
- Fresh master: `b16dd93ba51ac3e04e8332b1f710337b5b2fbd8f`.
- Prerequisite #52 merged 2026-10-03 19:16:24 UTC, then #54 at 19:16:32 UTC.
- Fresh setup downloaded IDO 5.3 static-recomp v1.2 and verified pinned archive SHA-256 `ab5c741561f80913d58c8b074771f23941a3edd312505a8ebed6d1dfeb65e506` before extraction. `setup.log` records that first run; the reproducible checker uses the same setup afterward and records every installed compiler-file hash.
- All three `asm/us/boot_tail/SHA256SUMS` members passed.
- All 439 unique starts and sizes exactly equal the read-only spec inventory: 99,120 B.
- Existing `func_80010A00` recompiled with `-g0 -O2 -mips2 -G 0 -non_shared`, with mandatory `-Wab,-r4300_mul`, and strictly matched 3/3 words (12 B), no extra words, unresolved symbols, unverified relocations or errors.

Reproduce from repository root:

```sh
python3 cloud/work/boot_tail/scripts/preflight.py
```

`preflight.json` contains numerical/hash evidence only, not native instruction words, compiled objects or ROM data. The script fails immediately on setup, manifest, census or getter mismatch. It does not repair protected inputs. Packet 2 was released before the five-leaf C11 Packet 3 claim. The eight census-boundary blockers in Packet 1 do not affect these five leaf functions; their inventories contain no callees and none is listed as blocked. PR #59 remains unmerged; later drafts must explicitly state that dependency. No production gate, layout, lock, target, scorer or symbol edit was performed.
