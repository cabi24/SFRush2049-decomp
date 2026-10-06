# avoid_areas source restoration: NONMATCH research

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_800E398C`, `[0x800E398C, 0x800E42E8)`, 2,396 bytes / 599 words.
No claims, promotion, matching coverage, or image/ROM verification.

## Result and reproduction

From the repository root, with the pinned IDO 5.3 toolchain available through
`IDO_DIR`, run:

    python3 cloud/work/frontier/dot_avoid_areas_20261006/repro.py

Actual flags: `-g0 -O3 -mips2 -G 0 -non_shared`, canonical whole-program stages,
including the scorer's `as1 -r4300_mul` setting.

Observed output:

    current baseline: 592/599 words differ; emitted=567 words; frame=200; extra_nonzero=0; unresolved=0; unverified=0; errors=0
    donor candidate NONMATCH: 583/599 words differ; emitted=561 words; frame=288; extra_nonzero=0; unresolved=0; unverified=26; errors=1

The candidate has the native 288-byte frame. The three vector homes at sp+224,
sp+236, and sp+248 now agree with the target. Instruction alignment also improves
from 120 missing opcode rows to 100, and 538 missing relocated word rows to 402
(SequenceMatcher diagnostic only, not a matching verdict).

The 26 own-rodata relocation words remain unverified because this is a broadly
misaligned body. The canonical scorer records one own-data comparison failure;
the nine-word positional improvement is not a complete relocated-byte proof.
The source still emits 38 fewer words than the native function. The reconstructed
caller context remains NONMATCH, so no group member is claimed.

## New source evidence

The actual arcade ancestor is `avoid_areas` in
[game/maxpath.c](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/maxpath.c)
(revision 3.48; copyright 1996 Atari Corporation). The behavior matches the full
steering/traffic-avoidance routine: transform the guide point, handle startup and
slow travel, avoid nearby cars, limit speed, then apply path hints and retain the
previous direction. N64 coordinates, indexed records, extra obstacle helper,
fixed-point speed conversion, and hint modifications differ from the arcade.

This adaptation restores a repeated hint condition absent from the archived
hand-written draft, uses direct array/index operations, and replaces this
function's extern-literal placeholders with the corresponding float literals.
The accepted vector transform, magnitude, and obstacle-scaling helpers are kept
as genuine context. Adding the helpers alone did not improve the baseline.

The donor really declares the currently unused `tmp2`, `temp[3]`,
`dir_list[MAX_LINKS][3]`, `dir_weight[MAX_LINKS]`, and `cur_rate[3]`. The N64 value
`MAX_LINKS=6` is independently used by accepted `src/blob/func_800EC914.c` and
`src/blob/groups/render_large_objects/group.c`. At six entries, the intervening
arrays explain the native 132-byte distance between `dir` and `save_dir`.
Retaining this subset of legacy declarations in the N64 version remains a
source-history hypothesis. No array size was tuned, and no arbitrary padding,
volatile carrier, false prototype, fake caller, or assembly was added.

## Required real context and remaining assumptions

`repro.py` replaces only E398C in the existing
`cloud/work/ipa-groups/dot_nearest_path_context_20261005/group.c`. That newer packet supplies its real
parent E4B58 and real sibling E451C/E4300 source and field declarations. It extracts
only the actual accepted transform/magnitude definitions from their canonical
source files, excluding unrelated historical bodies, and includes the accepted
`camera_blend_between.c` body. The parent and these helpers are kept; E398C stays
an internal function. Source definitions and compiler settings are visible in the
reproduction script; temporary objects stay under ignored `build/`.

The current nearest-path packet's parent arithmetic and struct hypotheses are inherited,
not re-certified. E4B58's external `state_utility` dependency remains open. Current
residuals include input register allocation, float-constant hoisting, statement
scheduling, and missing instruction structure. Native uses s7 for the narrow
input and s6 for the transformed vector; this candidate uses s8 and s7. The next
useful step is source reconstruction of the genuine parent/remaining control
structure, not a blind register sweep. No independent-review, behavior-harness,
full-test, or CI gate was added to this research publication.

## Baseline reconciliation

The initial draft used the older A7 archive. This revision uses the newer
`dot_nearest_path_context_20261005` packet, including its correct donor-backed
E451C parameter order and signed count/real array declarations. Its E398C baseline
is also 592/599, so the nine-word improvement is still new against that packet.
The previously proven E4300 helper remains canonical MATCH in this context; it
earns no new credit here. The historical wave-1 E451C stand-in result is not an
accepted real-context result.
