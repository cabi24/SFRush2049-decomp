# Typed force adjustment: NONMATCH research

## Result

`func_800E1AA0` is an ordinary one-state-pointer leaf at `0x800E1AA0`, with
100 native instructions / 400 bytes. This candidate has **40/100 full words
different**, a **392-byte ELF function** and 8 zero alignment bytes. Those
alignment bytes do not extend its function. No unresolved/unverified relocations,
masked words, relocation errors or nonzero excess words occur. Both canonical
strict comparison and a separate GNU-linked raw comparison confirm the residual.
`verification.json` lists every differing offset. This is **not a match**.

`claims` is empty. No accepted source, lock, native target, scorer, generated
context, build wiring or coverage total changes. No splice, promotion or merge.
The complete image and ROM inputs are unavailable; image identity, compressed
stream identity, full-ROM SHA-1 and `make test` have **not run**.

## Added value and source provenance

The existing `cloud/work/game_C31/func_800E1AA0.c` is a byte-offset seed with
58/100 differing words. This new typed offset view makes every accessed field
explicit and asserts all 14 O32 offsets, including the nested config's scale.
Names such as force, direction, speed and mode are hypotheses; full original
structures and allocation capacities have not been recovered.

The first typed source has 56/100 differences. A natural two-step accumulation,
`factor = state->bias; factor += state->blend * 0.5f`, produces the native initial
FP register lifetime and reduces that to 40/100. This is a useful causal result,
not a cosmetic republication. No invented helper, extra ABI argument, unused
padding local, volatile barrier, pointer laundering or artificial operation.

Subsequent natural controls (reused factor, named actual upper/threshold
constants, height local, early return, integer-zero comparisons, operand order)
do not improve the 40-word residual. Explicit product locals worsen it to 48.
Double comparison controls alter FP precision/structure and are not selected;
O1 has 96 differing target words and 12 nonzero excess words. The workbench
diagnosis is a structural two-instruction deficit, not a pure register rename:
the candidate reuses one zero FP constant while native materializes it at
separate sites. Native loads height in the earlier branch-likely delay slot;
the candidate loads zero there. Later temporary allocation then diverges.

The primary code is ordinary IDO/C89 C. The unknown-byte arrays express actual
observed field offsets, not compiler padding tricks. The host has a larger
pointer than O32, so host tests populate fields by name; separate IDO compile-time
layout assertions prove the native layout. The pointer at+4 targets a structure
whose float at+36 is read only on the final scaled-correction path.

## Behavior and ABI

- Flag bit 0x10 skips all updates.
- Unless both mode words are 8, accumulate bias + blend*0.5, cap only above 1,
  and subtract magnitude times either the global coefficient (speed>100) or
  speed*120, then times the capped factor, from the float at+320.
- Negative height skips the second correction. Unordered (NaN) height does
  not count as negative; preserving `!(height < 0)` matters.
- If direction*velocity is positive, a nonzero signed halfword at+1994 selects
  direction*100. Otherwise blend<0.5 additionally gates direction*100*config.scale.
- Only the float at+320 is written, zero, one or two times.

No calls, frame or callee-saved-register changes in the target; input a0,
void result. Protected direct-call inventory finds `object_update_full`, word 9,
with a0 set from s0 in its jal delay slot. Indirect callers are not excluded.
No direct arcade equivalent is established; the arcade reference checkout is
not present in this environment.

## Reproduction

With pinned IDO5.3 installed via the repository setup and GNU MIPS binutils in
PATH (and its runtime libraries available):

```sh
python3 cloud/work/dot_force_adjustment/replay.py /tmp/force-proof.json
python3 cloud/work/dot_force_adjustment/verify_semantics.py /tmp/force-semantics.json
python3 -m pytest tests/conveyor/test_force_adjustment_research.py -q
python3 tools/cloud/score.py fn cloud/work/dot_force_adjustment/candidate.c func_800E1AA0
```

The last command must fail with NONMATCH 40/100. `replay.py` exits 0 only when the
frozen documented nonmatch and native layout are reproduced; it never promotes.
The differential script checks 22,240 deterministic cases (edge patterns,
all gate combinations and random bits) against execution of every protected
native instruction. It also runs 200,000 host calls under ASan/UBSan with whole
state preservation checks. Leak detection alone is disabled for the ptrace
environment. Tests include signed zeros, infinities, NaNs, subnormals and finite
extremes, native branch-likely delays and fail-closed unknown instructions,
unaligned reads and prohibited writes. Two pytest regressions run under normal CI.

The host oracle rounds each modeled operation to binary32, compares NaNs by
class and does not model N64 FCSR exception flags, NaN payloads or denormal modes.
These are bounded checks with separate state/config objects, not a universal
aliasing, concurrency or real-N64 execution proof. Arbitrary initial global
coefficient values are tested; the original constant's data bytes are unavailable.

Independent review audited all 100 instructions, ABI and field offsets, replayed
fresh IDO+GNU linking and reran 22,240 differential plus 200,000 sanitized cases.
See `independent_review.json`. Approved as **NONMATCH research only**.

Base: fresh master `a12daa63ae67c8dab3404efb666089c884dadfd4`.
Current accepted locks, single/group claims and all PR 1–29 source patches were
checked; no prior PR owns this function. Historical C31 sources remain untouched.
This research directory is outside the matching-submission selector, so its
zero-submission result and green CI do not certify a match or rerun the full
standalone differential harness.

Final local verification: 1,309 repository tests passed, 41 skipped and 9
node-required tests deselected; both new regression tests passed. All 161 static
locks, all 664 blob/group source hashes and all 23 protected manifest entries
verified. Canonical single and three-member group control scorers all MATCH.
C89 strict syntax/warnings pass. Initial local full-suite attempts found absent
submodules and a relative-binutils PATH; initializing pinned submodules and using
an absolute tool PATH resolved those environment issues without code changes.
