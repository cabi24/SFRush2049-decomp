# Boot-tail promotion, wave 3 (2026-10-05)

Promotion of the 53 bodies that the first boot-tail run refused for
conflicting shared declarations within the ROM TU (37 global + 16 function),
after PRs #82 and #83 repaired their source contracts. The 19
"relocation symbol split" refusals were out of scope and are unchanged.

## Result

| | Static functions | Static bytes |
|---|---:|---:|
| `make progress` before | 327 / 668 | 61,060 / 160,720 (37.99%) |
| `make progress` after | 379 / 668 | 69,680 / 160,720 (43.35%) |
| Promoted in wave 3 | **52** | **8,620** |
| Still refused (of the 53) | 1 | 112 |

Game-code coverage was not touched (780/1216, 180,612 bytes before and after).
`python3 -m tools.conveyor.pipeline.lock check`: all 383 locks intact after
every commit. The ROM gate for every promotion/relock commit is the driver's:
full repo sync (`rsync -a --delete asm/`), touched TUs, `make COMPILER=ido -j16`
ending in `ROM matches!`, `make test`, and every touched object newer than its
source, on watchman2.

## Commits

| Commit | What |
|---|---|
| `78f795bd` | Relock 45 adapted/repaired source paths at true score 0 via the pool (`relock_wave3.sh`, `relock_wave3_results.tsv`); retire each superseded candidate lock; fresh `context_check.py` rows on watchman2, all ok (`context_wave3.jsonl`); driver `--only`. |
| `ca0a8c25` | Driver `--prefix-dedup` (see below). |
| `55e38d89` | **Promote 45** (7,024 B) in lib_11640, lib_16320, lib_1a660, lib_1cf90, lib_1f5b0, lib_207b0, lib_22300, lib_25bb0. One combined gate passed, then the final-set gate. |
| `ddf9ec6b` | Adopt the sequence-context canonical types in `src/rom/lib_17dc0.c` (`include/sequence_context.h`); relock the six accepted bodies whose spelling changed. Full-ROM gate passed (`gate_only.py`). |
| `eb1af4eb` | Sequence candidate context rows (`context_wave3_seq.jsonl`); splices skip an `#include` already made before the slot. |
| `f695fae7` | **Promote 7** sequence candidates (1,596 B) in lib_17dc0. |

## Per packet

- Macro wrappers (PR #82), 8 / 352 B: `macro_wrapper_contracts/sources/` relocked and promoted.
- Queue wrappers (PR #82), 3 / 140 B: unchanged `cloud/matches/boot_tail/` paths relocked (fresh score 0) and promoted.
- Identifier wrapper (PR #82), `func_800201D0` 48 B: relocked and promoted.
- Audio records, 11 / 1,388 B: `audio_record_contracts/sources/` promoted. The twelfth, `func_80013964`, stays refused (below).
- Voice/list, 8 / 1,900 B; macro/stream, 8 / 1,840 B; samples/buffers/emitters (`sources/`), 6 / 1,356 B: promoted.
- Sequence contexts, 7 / 1,596 B: promoted after the accepted-body relock below.

## Accepted-body relocks

**Sequence context (done).** `verify.py:adapt_context` was applied to the
production TU. The header moved to `include/sequence_context.h`, with the packet's declarations
unchanged. Production uses `#include "sequence_context.h"` so the packet's
lifecycle verifier recognises the TU as already canonical. Six accepted bodies changed
spelling only: `func_8001729C`, `func_8001734C`, `func_800173B4`,
`func_80017410`, `func_80017470` and `func_800174D0`. The evidence for each
before its lock was re-pinned:
1. A standalone copy of the adapted body (`wave3_sequence_sources/accepted_relock/`,
   with the same normalized body hash as production) scores **true 0** through the pool.
2. The full-ROM gate passes with the adapted TU and a fresh object.
The locks were then re-pinned as `rom-sha1` with the new body hashes. The seven candidates use
`wave3_sequence_sources/` copies that include the production header (the bodies are identical
to the packet sources) and are locked at true score 0.

**Audio cleanup (not done).** `func_80013964` needs the two-cell table
`unsigned short *D_800382D8[2]`, so accepted `func_8001144C` must free
`D_800382D8[1]`. Its adapted body scores **true 10** through the pool. The target
relocates `lw %lo(D_800382DC)` against `D_800382DC` as a separate symbol, while
the body reaches it as `D_800382D8+4`. The relocated bytes are identical (as the
packet says), but no true score 0 exists, so this is the same relocation-symbol-split
class as the 19 out-of-scope refusals. The production edit and the trial lock were
reverted. `func_80013964` and `func_8001144C` are unchanged.

## Remaining refusals (`final_status.json`)

| Reason | Functions | Bytes |
|---|---:|---:|
| no true-0 proof: target splits an interior data address into its own D_ symbol (pre-existing, out of scope) | 19 | 3,500 |
| dependent accepted-body relock has no true-0 proof: relocation symbol split (`func_80013964`) | 1 | 112 |

Both need target symbol naming (`D_800382DC` as an interior address of
`D_800382D8`), not source changes.

## Driver changes (no gate weakened)

- `--only fn,...` restricts the plan to the named slots. Other slots are skipped without refusal rows.
- `--prefix-dedup rom/lib_X,...`: in those TUs, a preamble declaration is skipped
  only if it already appears *before* the slot. This is the insertion-point visibility that
  the audio (prefix) and sample (full-source) packet proofs used. The first wave-3 attempt
  used whole-file deduplication, which left `D_8003801C` (lib_11640) and `D_8002C630`
  (lib_1cf90) undeclared at their use. That run was aborted and its four spurious refusal
  rows were discarded. It was used for lib_11640, lib_16320 and lib_1cf90. The other TUs keep the
  whole-file rule that their packets proved.
- `#include` preamble lines already made before the slot are not repeated.
- `gate_only.py` runs the same gate without splicing (used for the lib_17dc0 relock).

## Post-promotion checks

- `python3 -m pytest tests/conveyor tests/cloud -q -m "not node_required"` (Pi):
  1506 passed, 679 skipped (toolchain-dependent tests skip on the aarch64 Pi), 0 failed.
- All eight packet verifiers were rerun on watchman2 against the promoted tree:
  macro wrappers, queue, identifier, audio, sequence, sample, voice and macro/stream. All
  exit 0. Audio, macro wrappers and macro/stream used the pinned IDO 5.3 v1.2
  release (sha256-checked) via `IDO_DIR`, because the builder's install lacks
  `c++filt` from the pinned file set.
