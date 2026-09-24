# Extracted targets cannot score 0 (2026-09-24)

Found by hand-examining `object_process_thunk`, the project's closest
never-matched result (score 5 after a full 4-hour search).

## The mechanism

Extracted (game-code) target objects are assembled **raw-word**: the words
are copied verbatim out of `build/game_code.bin`, so a call carries its
absolute encoded address and a global reference carries its real `%hi/%lo`
immediates. A compiled candidate cannot reproduce those words — the
assembler emits a relocation with a zeroed field, to be patched at link
time. The scorer compares instruction words, so every call and every global
reference is counted as a permanent mismatch.

`object_process_thunk` is an eight-instruction thunk:

```
addiu $sp,$sp,-24 / sw $ra,20($sp) / jal func_800A5B3C / nop
lw $ra,20($sp) / addiu $sp,$sp,24 / jr $ra / nop
```

Our seed compiles to those exact eight instructions under IDO. The only
differing word is the call: target `0C0296CF` (absolute), ours `0C000000`
plus `R_MIPS_26`. True score 5, relocation-blind score 0.

**The tell**: the only two extracted functions that had ever scored 0 —
`func_80095EC0` and `func_800C8738` — are exactly the two with zero
relocatable references. Every other function in the population was
structurally incapable of reaching 0, no matter how correct its C.

## Measured impact

Every extracted target with a stored search result (104) was recompiled with
IDO on watchman2 and rescored against a target assembled from its own
derived asm (which already symbolizes `jal`/`%hi`/`%lo`, so the assembler
emits real relocations):

| outcome | count |
|---|---:|
| **true matches (score 0)** | **17** (was 2) |
| improved | 72 of 104 |
| unchanged or worse | 32 |

Matches recovered include functions that looked far from converging:
`func_800A5B3C` 105 → 0, `func_800B912C` 85 → 0, `func_800A510C` 50 → 0.
The next tier sits at 5–15 and is worth a closer.

Artifacts: `work/extracted_matches/<target>/{matched.c,target.reloc.s,STATUS}`.
All 17 are recorded `matched` + `extracted_evidence_only` — still firewalled
from promotion (005 FR-010), since Track B has no blob rebuild path.

## What this invalidates

- Every score ever recorded for an extracted target with references.
- The conclusion, stated repeatedly in this project, that game-code
  functions "do not converge". They converge; the goal was unreachable.
- The triage threshold `TRIAGE_PROMOTE_MAX_SCORE = 200` is measured in the
  wrong units and should be re-derived once targets carry relocations.

## The fix

Apply feature 003's reloc-aware target assembly to the extracted population:
assemble each target from its derived asm instead of raw words, behind the
same round-trip gate (masked-word equality against the raw words), with the
existing evidence supersession purging stale scores. Unlike 003's static
case, symbol names match by construction here — both the target asm and the
candidate's declarations come from our own generated tables — which is why
this experiment worked first try.
