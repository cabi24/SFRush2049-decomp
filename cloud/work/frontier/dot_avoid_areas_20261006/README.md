# avoid_areas source restoration: NONMATCH research

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_800E398C`, `[0x800E398C, 0x800E42E8)`, 2,396 bytes / 599 words.
No claims, promotion, matching coverage, or image/ROM verification.

## Result and reproduction

From the repository root with pinned IDO 5.3 available through `IDO_DIR`:

    python3 cloud/work/frontier/dot_avoid_areas_20261006/repro.py

Actual flags: `-g0 -O3 -mips2 -G 0 -non_shared`, canonical whole-program stages,
including `as1 -r4300_mul`.

Observed output:

    current baseline: 592/599 words differ; emitted=567 words; frame=200; extra_nonzero=0; unresolved=0; unverified=0; errors=0
    donor candidate NONMATCH: 574/599 words differ; emitted=588 words; frame=288; extra_nonzero=0; unresolved=0; unverified=26; errors=0

The candidate has the native 288-byte frame and vector homes at sp+224, +236,
and +248. It remains eleven words short with 26 own-rodata relocation words
unverified because the body is still broadly misaligned. Zero comparison errors
does not make those sites verified; no relocated-byte equality is claimed.

The previous revision scored 583/599 with 561 emitted words. Restoring actual
source control flow and distinct live model roles adds 27 instructions, reduces
the positional residual by nine more words, and removes its own-data comparison
error. The best available committed real-context baseline remains 592/599.

## New source evidence

The actual arcade ancestor is `avoid_areas` in
[game/maxpath.c](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/maxpath.c)
(revision 3.48; copyright 1996 Atari Corporation). Its guide-point transformation,
startup/slow paths, traffic avoidance, speed limiting, path hints, and saved
directions match this N64 operation. Coordinates, records, extra obstacle helper,
fixed-point speed conversion, and some hint decisions differ in the N64 version.

The candidate restores a repeated hint condition omitted by the old hand draft,
uses genuine array/index operations and natural literals, and preserves the
donor's early loop continues. Those continues reproduce additional native
branch/count-load structure lost by the old nested-positive tests. Real reset
loops and separate vector-copy operations recover more of the native length.
The own-car pointer is kept distinct from the currently scanned other car,
matching the native persistence of the own-car record across the traffic scan.

The unused `tmp2`, `temp[3]`, `dir_list[MAX_LINKS][3]`, `dir_weight[MAX_LINKS]`,
and `cur_rate[3]` declarations really exist in the donor. `MAX_LINKS=6` has separate
N64 evidence in accepted `src/blob/func_800EC914.c` and
`src/blob/groups/render_large_objects/group.c`. Their sizes explain the native
132-byte gap between direction and saved-direction arrays. Retention of this
subset in the N64 source remains a hypothesis. No array size was tuned, and no
artificial pressure, padding, volatile carrier, false prototype, or assembly
was added.

## Required real context and assumptions

The reproduction replaces only E398C in the stronger current
`cloud/work/ipa-groups/dot_nearest_path_context_20261005/group.c`. It retains real
E4B58/E451C/E4300 source, donor-backed E451C argument order, correct signed counts,
and genuine arrays. Accepted vector-transform/magnitude and obstacle-scaling
bodies are supplied as real kept context, excluding unrelated archived bodies.
E4300 remains an existing canonical match with no duplicate credit.

The initial draft used older A7 context; the current packet explicitly supersedes
that comparison. Its E398C baseline is also 592/599. The historical code-identical
E451C stand-in result is not used as a real-context acceptance claim.

Inherited parent arithmetic, external `state_utility`, and N64 struct/history
hypotheses are not re-certified. Significant register, scheduling, and control
structure residuals remain. Complete parent semantics and all image/ROM gates
remain with the checker. Temporary compiler objects stay under ignored `build/`.
No independent review, behavior harness, full tests, or CI gate was added.
