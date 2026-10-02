# C24 real motion-source pair

Claim only func_800E9C70 (444bytes). Existing accepted func_8008B3C8 (44bytes) is real unchanged context, with exact normalized definition copied from current locked source. context_provenance.json records the current original source/lock and body identity. Both canonical function-scoped stack-sensitive scores0 and all linked retail words0; no masks, unresolved/unverified data or extra words. All emitted functions have verified bodies; there are no stand-ins or synthetic roots.

Actual func_8008B3C8 consumes one vector pointer in a0 and returns its scalar norm in f0. The old caller seed inferred a second integer formal from an incidental a1=1 comparison operand, though the actual callee never consumes it; the corrected source uses the actual one-pointer contract. The caller’s physical vector is a genuine local f32[3], initialized in full before either real consumer. One shared real vector pointer replaces redundant decompiler per-branch pointer variables, eliminating an unnecessary eight-byte frame difference. All three vector differences are computed before zeroing the independently named per-player scalar, matching the true source access scheduling. The plain O2 candidate does not match because it treats the real helper as an opaque clobbering call; this real two-function O3 profile reproduces the caller-register ABI.

No data is newly owned or emitted. D_8012E638 is the original external table of three-float player vectors; D_8012E670 the original byte array of flags; D_80152720 the original scalar player array; D_801244D4 remains a named scalar constant. There are no anonymous ROM constants or guessed storage addresses. Argument and return widths of func_800E8D50 are proven by the real pointer/integer register consumers; its unused return is left void and does not affect the exact callsite ABI.

Group source first line and group.json pin `-g0 -O3 -mips2 -G 0 -non_shared`; standard repository compile_group emits through the established real IDO uld/usplit/umerge/uopt/ugen/as1 profile, including assembler r4300_mul errata flag. keep includes both real bodies, claims/members only the new caller. The parent retains the existing helper’s standalone O2 source/provenance and credits only the444 new bytes after image/ROM gates.

Replay with the two current private C target objects:

```sh
python3 PACKET/motion_group/verify.py --repo ~/agents/D/wt --toolkit ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 --target-dir TARGET_DIR --output D_C24_motion.json
```

Private frozen source/object: Rocky ~/agents/C/scratch/game-C24/motion_group/group.c and ~/agents/C/scratch/game-C24/motion_group.o. Source and group metadata hashes are fixed in source_hashes.json. Ordinary C4180 packet is separately immutable.
