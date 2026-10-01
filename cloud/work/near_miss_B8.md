# Worker B8 — concrete errata-sweep source repairs

Three initially noncompiling fresh m2c seeds are now valid compiling TUs. Exact initial sweep provenance/errors/hashes retained in `cloud/work/near_miss_B8/provenance.json`. All are NONMATCH. Sources and experiments used private Rocky B checkout/scratch only, current read-only DB targets, protected strict scorer and assembler multiply errata flag. No integration, source/assembly/lock/state/tooling changes or commits.

| Target | Initial seed | Retained strict verdict | Flags |
|---|---|---|---|
| stat_lap_split | undefined sp58 | 6/152 words differ | -g0 -O3 -mips2 -G 0 -non_shared |
| random_float | undefined sp50, sp4C | 296/356 words differ | -g0 -O3 -mips2 -G 0 -non_shared |
| func_800E4300 | undefined arg3, arg4; unset t2/t0/t5 | 135/135 plus23 nonzero extra words | -g0 -O3 -mips2 -G 0 -non_shared -Wo,-loopunroll,0 |

Retained files under `cloud/work/near_miss_B8/` preserve their exact remote variant names. None are match deliveries and none should be spliced.

## stat_lap_split

Actual output pointer is sp80, and func_800A61B0 writes three floats at80,84,88. Missing sp58 is output[2], not an unrelated uninitialized scalar. Input vector resides68,72,76. Added real output[3]/delta[3] arrays and audited calls against actual assembly:

- `void func_800A61B0(void *input, void *output, void *matrix)` — a0=input, a1=output, a2=player+44. Seed's fourth live byte argument was spurious; callee reads only those three integer arguments and writes three output floats.
- `s32 high_scores_display(s32 arg0, s32 arg1, s32 arg2, u8 arg3, f32 scale, f32 x, f32 y)` — a0=original arg0, a1=original arg1, a2=1, a3=original byte; three floats at outgoing sp16/20/24. Seed incorrectly prepended incidental live f12/f14 as two formal float arguments. Removed both. Return integer result matches v0 propagation.
- D_8011F020/D_8011F040 are signed16 arrays; target uses index*2 and lh. D_8010FFC0 is signed8, D_80153E8F unsigned8, stride8. Kept their current-context widths and repaired actual indexed table types.

Deleted redundant temp_t6/arg3 copying (formal u8 already generates target masking). Reused squared-product carrier rather than declaring a second temporary, expressed each sign as `(value >= 0 ? 1 : -1)`, and placed the existing GameCar pointer before the two arrays. This obtains exact target96-byte frame, array homes68/80, integer allocation and 146/152 exact words, without padding declarations.

Residuals: +88,+8C,+90,+9C,+A0 are initial vector load/store scheduling; +168 swaps commutative FP multiply operands (target f2*f10, candidate f10*f2, destination f2). Typed prototypes, volatility of input/car/delta, sign-product commutation, alternate carriers and separate signed-square local were tested; volatile delta or added local worsen shape/frame. About17 meaningful probes after repair; no reflow or stand-ins. Real closure with the actual transform callee could be investigated if authenticated allocation/reservation analysis warrants it.

## random_float (legacy name, actual physics routine)

The authoritative target at0x800FD9F8 is a 1424-byte physics routine, not the unrelated guessed random-number body in src/game/game.c. Do not use that guessed implementation. Missing sp50/sp4C are velocity[2]/velocity[1], written by func_800A61B0; local output atsp60 is also a three-float vector. Reconstructed input delta and force as actual three-float arrays, rather than assuming separate scalar locals were writable vector buffers.

Call audit:

- func_800A61B0 receives exactly three pointers; deleted seed's incidental live fourth argument.
- func_8009E820 receives force pointer, delta pointer, matrix pointer. Caller target sets a0=sp108,a1=sp96,a2=sp28; callee uses only those pointer registers. Seed's apparent 4000.0f and0.0f were incidental live FP registers, not arguments. Replaced the false float-prefix call and prototype with real three-pointer call.
- func_8008B3C8 returns f32 and takes one vector pointer; seed treated return as integer cast to float and passed incidental a1=2. Actual callee uses a0 only and returns f0. Fixed.
- fabsf is IDO intrinsic, so repaired seed has no unresolved fabsf call symbols.

Reusing seven float calculation temporaries directly in already-present vector components removes redundant local homes and improves347/356 to296/356. First84 bytes are exact; remaining frame/home/register/scheduling divergence is substantial. This source is a compiling, semantically reconstructed lead, not a near-match claim. One intermediate text-replacement probe had a syntax error from replacing a prefix of another variable name; corrected with token boundaries and discarded it.

## func_800E4300

Assembly proves an IPA calling convention: point pointer in a2, previous-path short in t2 with original formal home4, point-index short in t5/home8, row short in a1/home12, current-path short in t0/home16. Caller func_800E451C loads t5 from s6+36, t2 from s6+40, row from s5+2018, and calls it; caller itself uses hidden s5/s6 inputs. No stand-in formal-register assignments were invented.

Reconstructed a valid logical source signature `(void *point, s16 previousPath, s16 pointIndex, s16 row, s16 currentPath)` in the original formal order inferred from homes, replacing every unset-register M2C_ERROR with the appropriate genuine parameter. Corrected D_8012E668 from word to signed16; reconstructed 80-byte path rows; repaired an m2c pointer addition that multiplied the byte offset by sizeof(s32) in the wraparound lookup. Source compiles. Default loop optimization expands the loop dramatically; supported no-unroll flag reduces extras159→23 but cannot reproduce hidden-register inputs. Standalone strict score is not an ABI acceptance check and stays135/135. Deliver this only as a genuine real-call-context lead.
