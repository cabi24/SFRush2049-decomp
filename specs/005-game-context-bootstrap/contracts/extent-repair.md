# Contract: Extent Repair for Extracted Targets

Consumer: `pipeline/targets.py` extract flow (and anything that trusts
`n64_target.insn_count` for the `extracted` population).

## Scan algorithm (normative)

Input: `address` (RAM vaddr in `[0x80086A50, 0x80086A50+len(game_code.bin))`),
the `game_code.bin` image. Output: `insn_count` (instructions) or a conflict
classification.

1. Decode words sequentially from `address` (big-endian, 4-byte).
2. Maintain `furthest` = max RAM address of any *forward* direct branch
   target seen so far (all `b*` conditional/unconditional branches including
   `bc1*`; backward targets don't extend the extent; `j`/`jal` never extend
   it — `j` beyond the scan is a tail call).
3. On `jr $ra`: the function ends after its delay slot **iff**
   `pc >= furthest` at that point; otherwise continue (early return inside
   the body). The equality case is load-bearing: a shared-return leaf
   function branches directly *to* its `jr $ra` (`beqz …, .Lret; …;
   .Lret: jr $ra`), so `furthest == pc` at the true end — a strict `>`
   overruns into the next function (verified: `sound_stop` @0x800B358C,
   whose `beqz` targets its own `jr` at 0x800B3624; 29/885 targets are
   affected, several cascading into false conflicts).
   (Amended 2026-07-16 at the A1 review gate: the contract originally said
   `pc > furthest`, contradicting its own "at/after" prose; the strict form
   was implemented faithfully and caught empirically.)
4. `jr $reg` with `reg != ra` (jump table dispatch) never terminates.
5. Hard bounds: scan aborts at `min(address + 16 KiB, image end)` →
   classification `scan_overrun` (treated as `no_disasm` downstream).
6. The extent always includes the terminating `jr $ra` delay slot
   (dry-run failure mode #2: never end on a branch/jump without its slot).

Determinism: pure function of (image bytes, address). No DB state, no
randomness, no environment dependence.

## Repair pass (normative)

Runs inside `python3 -m tools.conveyor.pipeline.matrix extract` (same
transaction discipline as 003):

- For each `extracted` target, compute the scanned extent.
- **agree** (scanned == stored): no write, no supersession.
- **repair**: update `insn_count`, re-carve `function_words`, re-run
  `assemble_words`, store new object, set `target_o_sha`,
  `gate_reason='extent_repaired'`; purge that target's `matrix_entry` rows
  in the same transaction (003 supersession contract, reused not
  reimplemented).
- **conflict**: after all extents are scanned, any target whose `address`
  lies strictly inside another target's `[address, end)` gets
  `gate_reason='extent_conflict:<container_target_id>'` and is excluded
  from seeding/scoring queries. Its row and object are retained (evidence
  of the inventory error), and the histogram reports it in the
  `extent_conflict` bucket.
- Extract prints a repair report: counts of agree/repair/conflict and the
  10 cluster targets' before→after extents.

## Acceptance oracle

Codex ground truth (research/cluster-data-refs.md, independent objdump
scan-to-epilogue): the repaired extents MUST produce these end addresses
(exclusive) for the cluster:

| target | start | end |
|---|---|---|
| game_loop | 0x800FD464 | 0x800FD724 |
| game_mode_handler | 0x800C9AE0 | 0x800C9BE0 |
| attract_or_transition | 0x800EDDC0 | 0x800EE5DC |
| process_inputs | 0x800C997C | 0x800C9AE0 |
| sound_control | 0x800B37E8 | 0x800B39BC |
| playgame_state_change | 0x800CA3B4 | 0x800CADA4 |
| RaceStateMachine_Update | 0x800DB81C | 0x800DC080 |
| countdown | 0x800FBF88 | 0x800FC9F8 |
| countdown_handler | 0x800FBC30 | 0x800FBE20 |
| Input_ProcessGameplayPad | 0x800A04C4 | 0x800A0F64 |

(The codex survey noted the recorded 2-instruction entries at 0x800FBC30 and
0x800FBF88 are pre-prologue instructions belonging to these functions.)

Unit tests additionally cover: early-return function (jr ra before
furthest), jump-table function (jr $t does not terminate), delay-slot
inclusion, backward-branch loop (does not extend), and idempotency
(second run = all agree).
