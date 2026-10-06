# MP_IntervalPos: full-score match candidate with real callers

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_800E451C`, `[0x800E451C, 0x800E4B50)`, 1,588 bytes / 397 words.
Status: match candidate for the independent checker. No integration or accepted
coverage is claimed.

## Reproduce

With pinned IDO 5.3 available through `IDO_DIR`, from the repository root:

    python3 tools/cloud/score.py group cloud/work/frontier/dot_interval_position_20261006 --claims

Actual flags: `-g0 -O3 -mips2 -G 0 -non_shared`, with canonical whole-program
stages and the scorer's `as1 -r4300_mul` setting. Toolchain identity follows the
repository's pinned IDO package, `796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5`.

Observed canonical result:

    func_800E451C: MATCH
    own .rodata verified at 0x80124440..0x80124454

The complete ELF function extent is exactly 397 words. Differing words, extra
nonzero words, unresolved symbols, unverified relocation words, and errors are
all zero. The native 232-byte frame is reproduced. Existing E4300 remains
`MATCH`, 135/135 words; it is context only and earns no duplicate credit.

The strongest existing real-context baseline is
`cloud/work/ipa-groups/dot_nearest_path_context_20261005`: E451C had 365/397
differing positions, 394 emitted words, a 224-byte frame, and ten unverified
own-rodata words. The two complete enclosing bodies remain NONMATCH:
E398C 592/599 and E4B58 521/567. Neither is claimed by this packet.

## Source and required context

The existing current packet supplies the complete genuine caller graph:
E4B58 calls E451C three times and E398C once; E451C calls E4300 and naturally
inlines `mp_interval_pos`. E4B58 is the only kept ABI root. No stand-in caller,
clobber helper, fabricated parameter, or production edit is introduced.

The algorithm and restored declarations come from
[game/maxpath.c](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/maxpath.c)
at `845329d7b36f5a384c5625ed9a0aef584ab46139`, revision 3.48, copyright 1996 Atari
Corporation: `MP_IntervalPos` and its `mp_interval_pos` helper. The historical
`w1g/groups/func_800E4300` draft already carried the body with stand-in outer
callers; this is not claimed as a newly discovered algorithm.

Three changes relative to the newer real-context packet restore the full score:

1. Restore the donor's unused `nx` and `ny` declarations. They explain the missing
   eight bytes of frame, with native source-layout evidence rather than an
   invented padding object.
2. Restore the donor's unused `invdist` reciprocal statement inside the genuine
   interval helper. The compiler removes its runtime code, but its source block
   affects the post-call float allocation. The declaration and statement are
   disclosed legacy source, not a claim of exact original N64 text.
3. Reuse the existing `extern volatile s16 D_80152768` contract for this exact
   symbol from accepted `src/blob/func_800EC914.c:66` at the base commit. That
   accepted source explicitly discloses the qualification as shaping. The
   historical E451C native body repeatedly reloads this signed-halfword count.
   This packet establishes qualification provenance and observed code equality;
   it does not establish interrupt/concurrency behavior or generalize volatile
   to other globals.

The intermediate donor-layout restoration without that existing qualification
still had 364/397 differing positions and was three words short. Reusing the
accepted contract supplies the native reload pattern and closes the complete
body, including every own-literal reference under the unchanged scorer.

## Handoff limits

Only E451C is newly listed in `claims`. E4300's earlier full-score proof is
preserved, not re-counted. `mp_interval_pos` is genuine inlined context; this
packet assigns no new matching credit or definitive address to its deleted
stub. Complete parent semantics, canonical whole-program placement, accepted
neighbors, compressed image, and ROM equality remain for the independent
checker. The E398C/E4B58 source assumptions are inherited from the pinned current
packet. The separate donor-layout draft PRs are not dependencies.

This submission contains only source, actual group flags/context, and these
reproduction/score/assumption notes. No independent review, behavior harness,
full tests, CI wait, or ROM integration was performed before publication.
