# A75 documented hardware sqrt follow-up — frozen non-match

Coordinator authorized this separately labeled follow-up after A83 object audit
proved the frozen A75 source lacked the true IDO intrinsic declaration. Frozen
A75 sources, original hashes, and old proofs remain unchanged; original STATUS
has only a factual erratum linking this new packet.

Added #pragma intrinsic(sqrtf), documented by compiler playbook and existing SDK
float declarations. This removes a real unintended call and restores sqrt.s.
Fresh full original-size target is 145/201 O2 and 136/201 O3, no extras, unresolved,
unverified, or errors. Final O3 file has its own literal O3 line and fresh hash.
The corrected actual nullable Mat3 pointer contract from A76 is preserved.

Bounded consumed-source controls: reverse declaration order of the two actual
Vec3 arrays to investigate their observed stack homes (unchanged145); reuse the
actual length local as the consumed interpolation ratio (unchanged145, native
frame160->152); type the consumed next path index as the true signed-half return
(unchanged145 O2 /136 O3). No pressure, fake work, unused locals, padded buffers,
reflow sweeps, or synthetic group context. The original target frame144, native
argument retention, stack homes, and FP temporary schedule remain unresolved.

All sources, hashes/literal flags, and sanitized fresh comparisons frozen in
verification.json and packet.json. Empty claims, no match/coverage credit.
