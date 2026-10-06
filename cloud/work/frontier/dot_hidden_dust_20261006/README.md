# Hidden boundary does not improve the HUD visibility callback

Target: `dust_cloud_effect`, `[0x800EFEA8, 0x800F0100)`, 600 bytes.
Base: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7` (2026-10-06).
Status: **NONMATCH, source-boundary hypothesis falsified. No claims.**

The genuine arcade `Hidden` helper produces exactly the same complete relocated
callback bytes as the prior expanded logic, both alone and with the complete
resource-selector source. This boundary does not fix the callback's missing
frame or private-call contracts. No additional allocation variants are justified.

## Bounded result

| Source route | Full function size | Frame | Differing positions / 150 | Missing words |
| --- | ---: | ---: | ---: | ---: |
| Standalone, expanded or Hidden, with or without selector body | 560 | 40 | 148 | 10 |
| Kept-root group, expanded or Hidden, external selector | 560 | 40 | 148 | 10 |
| Kept-root group, expanded or Hidden, real internal selector | 588 | 56 | 127 | 3 |
| Native | 600 | 144 | n/a | 0 |

Eight fixed builds use the unchanged canonical single/group routes and the bare
`-g0 -O3 -mips2 -G 0 -non_shared` recipe. Mandatory scorer-injected
`-Wab,-r4300_mul` / `as1 -r4300_mul` are retained and recorded. No special flags,
pressure locals, dead reads, padding, volatile additions, fabricated callers or
edited native targets are used. The 88-byte frame deficit in the best group
is in non-save storage: native save slots and candidate save slots are both 24
bytes. Workbench diagnosis independently reports a structural mismatch and a
three-instruction short body; its relocation-blind score is not acceptance
and is not used for the receipt.

The independently observed source-boundary equality is the useful result.
The A141 historical 127/150 result remains a nonmatch. Its old canonical
"no extra words" statement did not establish an exact 600-byte ELF extent;
this packet explicitly measures the 588-byte STT_FUNC and all three missing
words. No padding or neighboring helper instructions are counted as the body.

## Ancestry and source inputs

The callback is N64-specific. **Only its visibility helper has authenticated
arcade source ancestry**, not the full callback or its resource selector.

- Arcade [game/hud.c Hidden, lines 1270–1280](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/hud.c#L1270-L1280)
  compares requested visibility with the signed Hide byte, calls UpdateBlit on
  change, and returns the reloaded byte. `Input_ApplyPadConfig` is the N64
  updater. Both candidate call sites retain that post-callback read.
- `baseline.c` is the exact existing A141 body from
  `git show 6b2e9e506fe3d2267a710e41c85af5364ccd00c7:cloud/work/tiny_A141/dust_cloud_effect.c`.
- `candidate.c` replaces only those two expanded sequences with the genuine
  helper calls. It preserves the record views and all other operations.
- The `_selector.c` controls append the complete natural selector from
  `cloud/work/dot_menu_options_root_20261005/slot_state_setup.c` at the stated
  base. Its integer types are supplied by the existing callback types; the
  unused return declaration is corrected from signed byte to actual signed
  word. There are no stand-ins or downstream artificial inline blockers.
  The selector is unclaimed context and is not asserted to match.

The helper's return is consumed at both real sites. The native invalid-layout
path disables animation; the active hidden path returns one. Those branches
remain unchanged. This is compiler-boundary research, not a new host/native
semantic differential proof or an assertion of original N64 declaration names.

## Verification and portability

`verify.py` pins only its packet sources, itself, and the selected native words.
It reads current target words with the protected manifest checks intact; it
never pins live target manifests, tools, lock state, production source hashes
or test files. The base identifies historical source/context provenance.
Unrelated target annotations do not invalidate it. Fresh receipts compare only
the actual proof fields, including complete body hashes, extents, relocations,
owned-storage absence and the nonmatch. IDO binary hashes are provenance.

Each full STT_FUNC extent is relocated without masks. All body relocations
resolve; there are no own-data/storage sections. Strict equality is false for
every build. Pairwise baseline/Hidden equality checks the full relocated body,
canonical comparison, real function size, frame, missing/excess positions and
storage state. Source and selected-native mutation controls fail closed.

Reproduce from the repository root:

    python3 cloud/work/frontier/dot_hidden_dust_20261006/verify.py
    python3 cloud/work/frontier/dot_hidden_dust_20261006/verify.py --compiler
    python3 -m pytest -q tests/cloud/test_hidden_dust_boundary.py

Fresh local validation: two consecutive complete compiler replays passed;
six focused tests passed. With IDO intentionally unavailable, five tests passed
and one compiler test skipped cleanly. The compiler test also guards a missing
MIPS GNU linker. No full-suite, independent GNU-link, whole-program shadow,
image, compression, ROM, runtime gameplay or remote-CI pass is claimed.

Ownership preflight at 2026-10-06 05:50 UTC found master at the stated base and
no visible open PRs. The target was unlocked at that base, and the parent was
notified before editing. No live lock assertions are enforced by this packet.
No shared checkout, accepted context, lock, production path or protected tool
was changed. Publication and aggregate current-master validation remain with
the parent; merging and acceptance remain with the independent checker.

Stop here. Reopen only when a genuine selector/callee-summary or original
wrapper source accounts for the missing 88 bytes of non-save frame and the
actual return-home/register behavior. Restoring Hidden again is not that input.
