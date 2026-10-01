# Worker B9 — next three small compile-failed seeds

All three initially failed current-context m2c compilation. Exact sweep metadata/errors/hashes frozen in `cloud/work/near_miss_B9/provenance.json`. Protected target sections and lock checked; read-only DB targets copied privately. Private Rocky B repo/scratch, one compile process, strict scorer with actual multiply errata assembler flag. No integration, source/asm/locks/state/tooling changes or commits. None are MATCH deliveries.

| Target | Retail extent | Repaired baseline | Retained source verdict |
|---|---:|---|---|
| car_lights_render |284 B|47/71, O2|13/71, O2|
| func_800E1C30 |848 B|159/212, O2|156/212, O2|
| func_800E92C8 |788 B|194/197 +20 nonzero extra, O2|196/197 +20 nonzero extra, O3, unpaired HI16|

Full compiling TUs retained under `cloud/work/near_miss_B9/` using exact remote variant names. E92C8's structurally repaired vector/prototype source is retained even though its positional score is two words worse: neither source is a near-match, and explicit valid memory layout is more useful for a future real group. No source should be spliced.

## car_lights_render

Missing sp30/sp34 are components1/2 of the real transform output atsp44/48/52. Input is a real vector atsp32/36/40. Target callee func_8009E820 uses a0=input,a1=output,a2=original arg2 matrix; seed passed incidental depth in f12 as a false first float argument and omitted the matrix argument. Fixed three-pointer prototype/call, retained actual fourth formal f32 depth (target mtc1 a3,f12) and fifth formal output pointer from the stack.

Reused input.x directly instead of extra temp_f8: 47→24 and frame64→target56. Placed independent depth component after y computation:24→21. Denominator operand demand corrected using actual field products in reversed C spelling:21→19. Natural vector structs (three real float fields) give target output load/store order after the call:19→13. Typed72-byte viewport rows preserve13 and exact address stride. No fake declarations needed.

Remaining13 words are local scheduling/load demand around the initial x/y computation and depth store, not missing output components or wrong data symbols. Volatile row reads, input.z placement, restoring named product and splitting product/division into memory updates worsen or retain residuals. About13 meaningful probes, bounded because shape changes ceased improving. O3 does not improve the repaired baseline. This is a useful new13-word lead, not a match claim.

## func_800E1C30

Actual target reads a0 as a structure pointer immediately, not f32. Replaced float/float signature with one void* formal. Deleted spurious var_f12=arg0/var_f14=arg1 assignments. func_800A61B0 receives three real pointers (object+724, object+520, object+748), not incidental float-prefix arguments plus a phantom fourth pointer. func_8008B3C8 receives one vector pointer and returns f32; repaired integer-return conversion.

The local output atsp40/44/48 is a three-float vector, not separate unbounded scalar locals. Reused four arithmetic temporaries in that vector and reused existing factor/return carrier: frame80→64→exact56, residual159→156. First64 bytes are exact except one changed load offset at+44; later initial position accumulation schedules and colors differently, so this is substantial work remaining. O3 gives same baseline. Seven structural probes; no padded frame or semantic stand-in.

## func_800E92C8

Registered head is a genuine IPA function, not an accidental interior extraction. It reads hidden incoming s0(object), s4(output), f22(force) and f24(damping); it also uses unsaved s3/s5 and FP registers. Caller func_800EA108 loads object into s0, creates output in s4, sets f22/f24, and calls at800EA198; other genuine callers E95DC and EA2DC share the protocol. Original formal order cannot be proven from these incoming registers alone. Retained logical four-input signature is an explicit source reconstruction for future real group work, not a claimed standalone ABI.

Repaired pointer-inferred float stack words into three actual vectors: low_speed(sp124), result(sp136), input(sp148). First fallback vector is (0,damping,-force), not pointers/nulls. Removed incidental FP-prefix arguments from both matrix-transform helpers; they receive three real pointers. func_800CFDEC receives two vectors, count3, fourth float1.0 bits in a3, outgoing floats100/speed and final vector pointer (seven actual arguments). Seed incorrectly prepended live f12/f14. Intrinsic sqrtf/fabsf now compile to FP instructions instead of unresolved calls. Repaired branch's computed global pointer into actual D_80152720 table element, avoiding an m2c high-half-only absolute address.

Logical seed now compiles, but standalone ABI necessarily saves registers/reloads formals differently and does not match. Strict scoring also reports an unpaired R_MIPS_HI16 for D_80152720 at .text+310; do not accept under relocation masking or bypass integrity. No artificial physical-register binding or fake caller was introduced. Only three source-repair baseline probes, because real caller closure is required.
