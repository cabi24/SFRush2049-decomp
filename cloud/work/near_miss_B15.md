# B15: repaired compile-failed game seeds

Outcome: zero strict matches, zero coverage added. No shared source, assembly, layout, lock, farm state, or match-delivery files changed. This packet freezes three previously compile-failed sweep seeds after bounded structural repairs. Current root locks were checked when selecting the packet. Baselines came from Rocky `~/agents/D/scratch/r4300-sweep/sources/`; original copies, DB target-object hashes, and repaired sources are in `cloud/work/near_miss_B15/`.

All frozen strict checks used literal flags `-g0 -O2 -mips2 -G 0 -non_shared`, the cloud scorer's normal R4300 multiply handling, and trusted target objects copied to private B scratch. Exact source SHA256, scorer stdout, stderr, and captured exit codes are in `verification.json`. Every frozen check exits 1. These are repair leads, not acceptance candidates.

| Target | Address / extent | Frozen meaningful seed | Strict verdict |
|---|---|---|---|
| physics_response | 0x800B93A8 / 920 bytes | physics_response_recordbase.c | 230/230 differing words, 55 extra |
| steering_sensitivity | 0x800AD128 / 928 bytes | steering_sensitivity_realclosure.c | 229/232 differing words, 45 extra, unresolved local helper symbol |
| func_800D0424 | 0x800D0424 / 996 bytes | func_800D0424_firstalias.c | 171/249 differing words, no extra |

## physics_response

The baseline declared `D_8012E5EC` as a scalar despite dereferencing it as a record pointer. The repair uses a byte pointer with actual eight-byte records: three signed short coordinates and a speed byte. The authoritative target initializes the metadata base at `D_80151CE8` and retains that base. Primary/secondary indices are offsets 6/4 (`D_80151CEE` / `D_80151CEC`), not changing record bases. The repaired source retains the fixed base, reloads the secondary index for the post-segment stop, reloads the primary index on the second pass, and uses the secondary index in final averaging. This corrects meaningful errors that a pointer-only repair left in place.

The target saves only `ra` in a 64-byte frame while writing unsaved `s0` through `s6` and `f20`, `f22`, `f24`. That is genuine whole-program allocation evidence. Ordinary standalone compilation saves those registers and cannot reproduce this body without authentic context. The earlier pointer-only repair scored 228/230 plus 47 extra words, but it retained wrong metadata-base semantics and is not selected merely for its smaller residual. Three real calls come from `audio_effect_apply`; PCs are recorded in `caller_audit.json`. No physical-register bindings or invented padding contexts were tried.

## steering_sensitivity

The actual six incoming homes are arg0 at sp68, arg1 at sp6C, arg2 at sp70 (input-position pointer), arg3 at sp74 (output-position pointer), arg4 at sp78 (output-matrix pointer), and arg5 at sp7C (float threshold), relative to the target's 104-byte frame. The baseline incorrectly treated arg2 as an unused integer and omitted genuine helper output locals. `D_80152034` is a byte-record pointer with stride 132, and `D_801526F0` points to a u16 count. The repaired source declares those widths and actual three-float output storage, and gives fabsf/sqrtf their float intrinsic declarations.

The real 20-word `vector_diff_process` uses hidden incoming `t0` for the origin, `a3` for position, `a1` for output, and `a2` for the nine-float basis; incoming `a0` is overwritten before use. It computes three position-minus-origin deltas in genuine stack storage, then calls real `func_800A61B0(delta, out, basis)`. The caller sets `t0` to its current record immediately before this call. `math_utility` copies the nine basis floats to arg4; its return is unused.

The frozen source reconstructs a logical four-parameter private helper without asserting the original source argument ordering, inventing a dummy a0 parameter, or binding t0 explicitly. Its O2 verdict remains nonmatching with an unresolved local section symbol; O3 also failed at 232/232 plus 60 extra words. Compile-only variants with uninitialized output locals are retained as intermediate diagnostics and are not selected as valid seeds. Authentic `steering_sensitivity` / `vector_diff_process` / `func_800A61B0` closure is handed to worker A; B stopped probing this closure to avoid duplication. All four linked main callers are recorded in `caller_audit.json`.

## func_800D0424

This ordinary function's baseline omitted two words of a real three-float vector output. It also declared `func_8008B424` as returning integer bits, although the actual 20-word callee returns a reciprocal vector length in f0. The repair uses a float return and genuine three-float input/output arrays. A seed call to `func_8009E820` had an extra phantom NULL argument; the actual call is input vector, output vector, matrix. The actor field at offset zero is a pointer, not a float value.

After these corrections, a duplicate first-loop pointer alias was collapsed, and its advancement moved after the stores as the assembly requires. That reduces the strict residual to 171/249 with no extra words. The earlier partially repaired scalar source was unsuitable because a vector-output callee wrote three floats through one scalar object. Full arrays alone scored 246/249; last-loop alias collapse alone scored 248/249, and collapsing both scored 179/249. The selected first-loop repair retains a 192-byte frame versus the target's 176-byte frame; actor register allocation and initial floating copy ordering still differ. These bounded pointer/control-flow probes did not introduce fake frame buffers or dead padding locals. Actual linked callers are `func_800D11BC` and `object_update_full`.

## Reproduction artifacts

`provenance.json` freezes authoritative target IDs, addresses, extents, object hashes and extent gate reasons. `verification.json` freezes exact source hashes and full strict scorer results. `caller_audit.json` lists real linked call PCs found in retail game code and mapped through current root layout. Local ignored assembly aids are `build/codex-B15/targets_dis.txt` and `build/codex-B15/callees_dis.txt`. Private compiler scratch is Rocky `~/agents/B/scratch/codex_B/near_miss_B15`; no private binaries are deliverables.
