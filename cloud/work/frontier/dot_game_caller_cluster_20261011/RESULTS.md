# Game name-entry caller cluster: reconstructed D816C, two words open

Research draft, empty claims. Population: game (`asm/us/blob`), not an overlay.
Base: `74149b5cd`. No new matched bytes, accepted-image bytes, or ROM credit.
The complete new reconstruction is `group/target.c`; the genuine D91A0 caller
and required inline context accompany it. No unmerged commit is needed to replay.

## Result

| Body | Stock canonical result | Compiled extent | Status |
|---|---:|---:|---|
| func_800D816C | 2/699 differing words | 699 words / 2796 bytes | Research |
| func_800D91A0 | 239/965 differing words, one extra nonzero word | 966 words | Research |
| Eight preservation members | strict MATCH | full native extents | Existing work; no new credit |
| drone_throttle_calc | code identical; 20 unverified references | full 1024 bytes | Split `.rodata` refusal retained |

D816C's only differences are at byte offsets `0x88` and `0x8C`: an ordinary
comparison branch plus empty delay slot in native code, versus an equivalent
branch-likely that copies the shared loop increment into its delay slot.
Every other instruction, all register choices, all stack homes, the 232-byte
frame, and the five natural float literals at `0x801241E8` agree.
Equivalence is not a match. The ordinary strict group command fails, as it must.

D91A0's historical eight *mnemonic-aligned* rows and 212 aligned word rows are
not canonical scores. Its current canonical result remains 239/965.
The initial compact D816C reconstruction was 654/699, emitted672 words,
frame296. The older coordinator full-unit627/699 result used another context.

## Source and compiler causes

- Typed 64-byte menu entries recover alias precision: type/handle at0/4,
  angle8, matrix12, position48, alpha60; 17 entries give the actual1088-byte row.
- Native arithmetic needs repeated expressions rather than named float
  carriers. Restoring the span and current-height expressions changes uopt's
  interference graph without adding instructions. The final frame census is
 140 bytes of source/inline area plus60 compiler-temp bytes; UGEN reports
  T20/P4/M200, giving frame232. All six used compiler-temp homes are exact.
- x/y/z store order, a named signed16-bit handle, separate float/int literal
  spelling, and the measured operand order restore the remaining schedule.
- Four compiled-out reads (`j == 10`, two `i == 10` reads, and `spacing`) are
  disclosed source-shaping levers. Tracing measured the index/constant
  priorities before changing them; stock output then reproduced the oracle's
  saved-register choices. These are not claimed original source spellings.
- Four unused scalar declarations remain: `span`, `distance`, `current_y`,
  `frame_residual3`. They reserve16 bytes. The first three are historical carrier
  homes left after restoring expressions. Removing all four gives frame216 and
 22/699 differences, with the same two opcode differences. The replay checks it.
- D8154 keeps its genuine leading player argument and six-word body. One
  unreachable assignment is a disclosed umerge source-size lever: it permits
  three genuine D816C inlines and suppresses all six D91A0 inlines. Stock
  umerge's caller/callee size threshold explains the split; one through four
  statements permit it, five through nine suppress both callers. No invented
  helper identity or artificial caller is used.

The local uopt instrumentation and forced-color oracle were diagnostic only.
Forces-disabled optimized streams and symbol tables were byte-identical to
stock. The stock assembler's `-R` trace and a narrow diagnostic of
`do_branch_opt` were also whole-object identical. That pass finds the shared
increment eligible and copies it for both first-loop branches. Only the second
copy disagrees with native. Equivalent continue, ternary, and loop-condition
forms did not establish a clean source-side blocker. No compiler/scorer patch
is part of the proof or submitted packet.

## Context and ABI boundaries

D91A0, the real caller, is complete. Independent review of its native call at
`+0x5A0` and both parent save/restore sets supports D816C's private `t2` argument
and callee-save-free body. This is group/private ABI evidence, not permission to
install the standalone function or to omit its caller.

The supplied D8154 and DA0B4 bodies, two row routines, predicate, and three math
helpers are explicit preservation members. Four large, unnecessary direct-callee
copies were removed after a score-invariant control, keeping the packet small.
The remaining Item/Node declarations are compatible across source files.
D80113ED8 consistently uses a32-bit address token; the caller explicitly casts
sound_control's pointer result. B24EC consistently returns an opaque NameEntry
pointer, explicitly cast where the native interface takes a texture token.
These N64-width casts are intentional, not a portability claim. The semantic
review assumes valid player/handle domains and finite animation values whose
alpha conversion is representable; it is not an arbitrary-memory or unrestricted
floating-input safety proof.

D8002EB94 and D80140BDC volatile views follow accepted sibling interfaces and
native repeated reads; neither proves asynchronous behavior. D91A0 retains the
w17e `func_800A1910` unsigned-size declaration against the locked signed-size
prototype. Its supplied calls use10, so the native argument bits agree, but this
scoped type-view limitation must be settled before integration. DTC retains its
existing four-slot/source-read reconstruction levers and split-owned-data
failure; code equality does not promote it to a strict match.

## Replay

From the submitted checkout, using canonical stock IDO5.3:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_game_caller_cluster_20261011/group
# Expected nonzero: two research members remain nonmatching.
python3 cloud/work/frontier/dot_game_caller_cluster_20261011/verify.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest -q tests/cloud/test_game_caller_cluster.py
```

One collected test binds submitted sources, group manifest, own verifier/test,
parsed native word hashes/extents, the relevant referenced symbol/address rows, and D816C's owned
literal hash. It performs one stock group replay and one four-slot control.
The receipt preserves strict failures rather than using an empty-claims success
as validation. Compiler hashes are recorded metadata; mutable tools, locks,
assembly text, and whole manifests are not receipt equality pins. This is not a
ROM translation-unit build; no production compiler pin or integration gate is
changed. Independent source/relocation review is recorded below when complete.

## Independent review, 2026-10-11

The independent verifier approved source commit `eb44877623a9c59f91cdfa9b8ca8718b23c0a533`
for **empty-claims draft publication only**. Ordinary and optimized-Python
replays passed. All10 member extents/next boundaries and331 GNU relocations
were checked independently: D816C retained exactly two differences; D91A0
retained239 differences plus4 bytes; eight preservation bodies remained exact.
All48 referenced owned-literal bytes agreed. The genuine parent ABI/save set,
source-type normalization, four-home control, missing-tool behavior, and
isolated scorer/import state were checked. Four round tests passed together
against current master `e46f06453` in0.83s (this packet0.34s); the local single-test
command including startup took0.76s. Protected paths were unchanged.

Reviewed target SHA-256:
`934930dec693d09f909fe09f27c7e690d2b760f21814d6f3dab9a3c6edf995be`.
Recipe: `c4d9ba95f11267765f25206fa47fc8e3009bb078ddec2c42b2ab154dce41455b`.
Verifier: `eff6b8fbff4bfc526e0dc795af7f579087139bb43287365e8298bb326814812a`.
Receipt: `2fa00dfde1a088366cb180950e9c199a7d5675e29c2cfdd5ee4de338fac7a09b`.

A private counterfactual diagnostic also suppressed exactly the second as1
cross-block copy and obtained full D816C equality. Replaying stock immediately
still gave2/699. This experimentally isolates the pass decision; the modified
assembler is excluded from the packet and supplies no source-match credit.
The unresolved task is a defensible source form that makes stock as1 retain
that native empty delay slot. No source-side blocker has yet been established.
