# A12: real vector transform closure — one MATCH

Frozen 2026-10-01. New claim: **vector_diff_process, 20 words / 80 bytes**. Current shared root lock checked before freeze: vector_diff_process, steering_sensitivity and traction_control are all unlocked. No stand-in, dummy formal, register binding, wrapper, padding or runtime dead read. Source is five real functions, totaling 556 retail words: helper20, steering232, traction248, accepted matrix transform37 and accepted matrix copy19. Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`.

Canonical `score.py group --claims` exits0: strict0, compiled20, target20, zero extras, no unresolved/unverified relocations or errors. The two previously accepted callees remain MATCH and are context only. Neither caller is claimed: steering229/232, compiled251 (19 extra words); traction240/248, compiled247. See scores.json and score_claims.txt. No claim that caller context is byte-identical.

## Real inputs and calls

Retail helper800ACB74 reads origin XYZ through incoming t0, input XYZ through a3, writes the differences to its real three-float stack delta at28/32/36, then invokes real func_800A61B0(delta,out,basis), retaining incoming a1 output and a2 matrix basis. Incoming a0 is overwritten before use. The four logical source inputs are all consumed; their declaration order is a reconstruction, not a recovered historical formal order. With the actual callers present and helper omitted from the ABI keep list, IDO reproduces its complete retail instructions and the hidden-input ABI exactly.

There are three authentic call sites. Steering800AD128 sets t0 to the selected 132-byte record, a2 to record+12, a3 to its third incoming formal (position pointer), and a1 to its real local vec3 at52. Traction800ACC18 first uses the same record origin, basis and position input with local vec3 at68. Its second call800ACFD4 uses the computed origin vec3 at104, arg4 matrix basis, arg2 input position, and arg3 output position. Neither call has an unused fabricated first argument.

Steering's true homes104/108/112/116/120/124 establish unused arg0, record index arg1, input-position pointer arg2, output-position pointer arg3, output-matrix pointer arg4 and float threshold arg5. Its basis copy uses the unchanged accepted math_utility body, nine floats from record+12 to arg4. Float rotation declarations are repaired to real `(f32 sine,f32 cosine,void *matrix)` interfaces proven by their f12/f14/a2 retail inputs. sqrtf/fabsf are declared float before their intrinsic pragmas. D_80152034 is a byte-record pointer; D_801526F0 is a u16-count pointer.

## Caller semantic audit

Traction was freshly decompiled from its own authoritative retail instructions, not a synthetic caller seed. Repaired all actual vec3 storage: offsets68,104,116,128,140,152,164,176,188. The decompiler had emitted scalars with undeclared adjacent lanes, an incorrect extra-argument lerp call, and a wrong final helper call/return type; these are corrected from actual homes and call setup. Its five genuine inputs match homes224/228/232/236/240; no used integer return is present. Record selection wraps next index against unsigned16 count, with stride132. First transform feeds its interpolation fractions; four genuine func_800ACBC4 calls build matrix columns and two record-origin vectors; their fifth call interpolates the origin. Cross product is the audited `(y*z-z*y,z*x-x*z,x*y-y*x)` operation, written between the two other matrix columns. Cubic displacement subtracts the output matrix's middle column from that interpolated origin before the second transform.

Each retail pointer loop processes exactly three floats. Source bounds use the relevant vector's own one-past pointer rather than relying on unrelated local arrays occupying adjacent stack addresses. This repairs a decompiler stack-layout dependency without changing its three-element memory operations or expression order. The two nonlinear interpolation expressions retain their actual multiply/add association. Remaining compiler allocation/frame differences prevent either caller from being advertised as a near match; no pressure-only workaround is retained.

The complete shared accepted func_800A61B0 and math_utility definitions were extracted unchanged and remain strict MATCH. The former computes three ordered dot products into the output vector. External func_800ACBC4 has real typed four-input interface (two vec3 pointers, float fraction, output vec3 pointer). Rotation and lerp definitions were tested as optional authentic context, but did not improve either caller, so those extra definitions are not delivered.

## Directed controls

24 permutations of the four consumed helper formals with consistent genuine call-site permutations were measured while preserving its ABI with keep: none matched (18/20). Twelve keep-list/code-free guard controls then exposed the justified mechanism: omit the helper from keep while retaining the real callers and existing ABI matrix callees. The plain body immediately MATCHes. No guard is retained. Optional real rotation/lerp context was tested in three more compiling controls without improvement. Controls are sequential on private Rocky A. All tables contain counts only; compiled objects, full disassembly and ROM words remain ignored build/private scratch.

Reproduce:

```sh
rsync -a cloud/work/ipa-groups/codex_steering_a12/ Rocky:agents/A/wt/cloud/work/ipa-groups/codex_steering_a12/
ssh Rocky 'cd ~/agents/A/wt && python3 tools/cloud/score.py group cloud/work/ipa-groups/codex_steering_a12 --claims'
```

Root independent scoring, source-image gate and full ROM gate remain the acceptance steps.

Frozen group.c SHA256: `f10247924e1e254397ab010ef81926b47ff35efc29544826ca3b5c64bafabbfd`.
