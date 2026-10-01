# Lane A2 registered heads (2026-10-01)

One new strict ABI match: func_800DC1AC, 39 words / 156 bytes. No splice, lock/layout edit, commit, or ROM gate was performed.

## func_800DC1AC: new strict MATCH, 39 words / 156 bytes

Frozen full TU: `heads_A2/func_800DC1AC_match.c`, exact flags
`-g0 -O3 -mips2 -G 0 -non_shared`. Canonical Rocky score.py fn says plain MATCH,
exit 0. This ABI function is absent from the current lock; the old group claimed
only func_800DC120. It needs no IPA context or synthetic callers.

The old loop body updated the byte, incremented global bit position, then shifted
value. Moving the latter two operations into the for-loop update clause before i++
preserves execution order and changes the scheduler to the target final sequence:
`sh; move a3,t8; bnez; sb (delay)`. All 39 relocated words now agree. Both update
orders `D++, value >>= 1, i++` and `i++, D++, value >>= 1` match; delivered source uses
the former, which retains the original runtime operation order exactly. No volatile
locals, unused loads, unreachable padding, or intentional undefined behavior.

The isolated baseline was 3/39. All 1,023 nonempty subsets of its ten token-preserving
line-join boundaries left that score unchanged. Fifteen bounded expression controls
then tested loop-update clauses and saved-position locals; two ordinary update-clause
forms match, while named-position forms disturb coloring (16–34 words). Results are
in `heads_A2/DC1AC_line_controls.json` and `DC1AC_expression_controls.json`.
The failed source-layout experiment does not support a general matching gain claim.

```bash
python3 tools/cloud/score.py fn cloud/work/heads_A2/func_800DC1AC_match.c func_800DC1AC --flags '-g0 -O3 -mips2 -G 0 -non_shared'
```

## func_8010C2E4: existing 3/89 remains

Full TU: `heads_A2/func_8010C2E4_best.c`, exact flags `-g0 -O2 -mips2 -G 0 -non_shared`.
Reused B's fully repaired seed, not its original 89-word-different decompiler output.
All global register lanes already agree. Remaining words: the store/move pair
at +0x120/+0x124 is reversed; the final OR at +0x130 has commuting operands reversed.
There is no frame or register-pool mismatch to repair with declaration padding.

Ten individual whitespace-only line joins in the final flags block, all 45 pairs,
all 120 triples, and joining all ten boundaries leave **3/89**. These add scheduling
controls absent from B's recorded 32 type/expression controls. They do not justify
more blind operand reversal, which B already exhausted. `heads_A2/C2E4_line_controls.json`
records the 166 combined controls. Canonical score.py confirms the baseline, exit 1.

## Two real handlers sharing slot_state_setup

Frozen full TU + manifest: `ipa-groups/codex_heads_A2/`. Canonical score.py group:
func_8010E8B4 **53/85**, func_8010B7FC **113/115**, exit 1. Claims are empty.
Both heads were selected from the small registered IPA callers: direct dependency
closure is 258 words (85 + 115 + real slot_state_setup 58). Unlike the old generated
seeds, these two handlers provide real second/third call sites; no synthetic caller
is delivered. The real sound/no-op body and real historical callers from the audited
codex_sound_channel packet inhibit inlining without placeholders. Context matches
for four already accepted sound callers are informational only.

Sixteen bounded controls cover s32 versus s8 slot return, real object-byte setter
context, actual caller keep order, and exported versus internal callee. Twenty more
cover volatile returned-slot local arrays of 7 through 16 words, with/without a
register-volatile discarded slot result in the other handler. Every control compiles.
The signed-byte return restores the real slot argument in s2; slot context now differs
16/58, principally s0/s1 base-register swapping. E8B4's returned-slot write is kept by
a volatile eight-word local array, reproducing its 96-byte frame; it still lacks the
retail unused s4 save/restore, and the local store offset/schedule is not exact.
This array is an explicit source-layout experiment, not inferred original source.

B7FC's two retail dead `move s0,v0` return copies remain absent. Register-volatile
locals introduce extra writes and worsen the score, so they are not in the frozen
source. Its generated float bit-pattern-as-integer call was repaired to -1.0f;
object_manager_update's half result uses an unsigned shift as retail does; the
running vertical position uses s32 and narrows only at the actual state_utility
s16 argument, preserving target arithmetic. Context compilation differs from retail
and is not offered as a fully closed module. Existing external real callees retain
their actual symbol names. No unsafe dead reads or missing-body stand-ins were used.

Reproduce on an IDO builder with the current protected target manifest:

```bash
python3 tools/cloud/score.py fn cloud/work/heads_A2/func_8010C2E4_best.c func_8010C2E4 --flags '-g0 -O2 -mips2 -G 0 -non_shared'
python3 tools/cloud/score.py group cloud/work/ipa-groups/codex_heads_A2
python3 cloud/work/tools/ipakit/deps.py closure func_8010E8B4 func_8010B7FC --json
```

Private Rocky A initially had stale target regions: a missing target can return
builder strict_diff=0 alongside err and target_size=0. Those are errors, not matches.
Refreshed that private asm/us/blob snapshot from current local files before the
measurements above, and required no err + target_size=89 for C2E4 controls. All
compiler caches, objects, and scratch outputs remain outside repository work products.
