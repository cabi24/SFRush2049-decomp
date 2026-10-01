# Worker B3 — natural structure and genuine compile context (2026-10-01)

Delivered strict matches, independently rescored exact deliverables:
- `cloud/matches/random_seed_init.c`: 61 words / 244 bytes, O2.
- `cloud/matches/func_8008D0C0.c`: 24 words / 96 bytes, O2.
- `cloud/work/ipa-groups/codex_task_complete_signal/`: task claim 36 words /
  144 bytes, O3; genuine adjacent comparator context also strictly matches.

Flags singles: -g0 -O2 -mips2 -G 0 -non_shared, exact first-line flags comments.
Group: -g0 -O3 -mips2 -G 0 -non_shared. Scorer adds r4300_mul. No root integration,
lock/state changes, commits, or image/ROM gates performed here. Private Rocky B
scratch codex_B, compilation serial. Current trusted DB target objects were
exported read-only; all original source origins were near-miss/base.c.

## physics_forces2 — legacy alias, not a splice target

Strict scorer refuses: no .text.physics_forces2 in trusted current target assembly.
Coordinator independently confirms 800953C0 is the interior delay-slot word of
registered func_800953BC; next head 800953C4. Legacy synthetic DB object disagrees
with empty seed and should not authorize a splice. No variants or claims.

## random_seed_init — 11/61 -> MATCH

Diagnose: identical temp/shared lanes, a2/a3 pool web swap: index and final entry
pointer. Code-free **if(arg0){}** immediately before r->active assignment flips
both webs and strictly matches. if(arg0+1){} also exact; chose simpler dead read.
This is a compile-affecting quirk, not likely original source; note in commit.
Other probes: arg0&255 dead read=25; late e/global/r dead reads=35; argument
signedness unchanged 11 or narrowing worse (58-61 plus2 extras); explicit byte
casts at store all11. Thirteen bounded probes plus exact deliverable rescore.

## func_8008D0C0 — 9/24 -> MATCH

Diagnose: identical temp lane, target global address v1 and loop locals t0/t1
versus a3/a1/a2. Eight first probes (extra formal params, register form, dead
argument/counter/pointer reads) unchanged9 or added spills24+2.

Source reconstruction resolved it: named m removed ->6; named t removed ->9;
both removed ->4. Eliminate all pass-through n/m/t and use global counter directly:
```
*arg0 = 0;
if (arg0 + 0x2C == (s16 *)((E58 *)&D_8015B268 + D_801613B4)) {
    do {
        --D_801613B4;
        arg0 -= 0x2C;
        if (*arg0) break;
    } while (D_801613B4 > 0);
}
```
Strict MATCH. Alternative named n predecrement18. Thirteen bounded probes.
No dummy reads, additional parameters, or other quirks in final version.
Tool lane evidence was useful, but natural web population was the lever rather
than changing colors/reservations or involving callers.

## task_complete_signal — genuine group MATCH pending root acceptance

Single baseline strict5/36 plus1 extra nonzero word; dead epilogue alignment.
Existing cloud padded3 artifact strictly MATCHed but root acceptance rejected
undefined invented PG and nonzero input-section start. That pending single
`cloud/matches/task_complete_signal.c` is superseded by the genuine group above;
**do not integrate the invented predecessor version**.

A real predecessor func_8008A6A4 and O3 single still have extra tail padding.
Existing real DMA group and six source permutations failed, 4-5 extra words.
Real adjacent queue-init A704 group failed, 1 or5 extra words depending ordering.
Inspection showed root order reversed and target epilogue needs module phase
start congruent4 bytes mod32. Genuine adjacent **17-word func_8008AD04** body from
its already verified match supplies that phase. Group keeps both real functions
external, claims only task; exact delivered directory score.py group --claims
returns MATCH, informational func_8008AD04 also MATCH. No stand-ins, invented
helpers, or synthetic calls. func_8008AD04 is locked context, not a new claim.
About thirteen context/order probes; full source/spec/status in group directory.

C2E4 best3 not revisited because genuine task context and fresh packet took priority.
