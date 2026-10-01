# A8: state_utility real sound closure — NONMATCH

Frozen 2026-10-01. Empty claims; no new match or splice candidate. Shared root blob_matched.lock.json confirmed state_utility unlocked before work. Current direct graph is state_utility99 + sound_update_channel122 =221 words. Including the authentic empty callee and all actual A3 contexts gives433 retail words. No synthetic caller, added pressure argument or stand-in callee.

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`. Private Rocky A, sequential controls. Delivered strict result: **77/99 differing words, compiled97 words, zero extras, no unresolved relocations**. Baseline reconstructed literal field-access body was80/99, compiled96. Replacing the byte sum with the real object_bytes23_sum body improves to77; umerge inlines its real getters. Call sequence remains exactly two object_manager_update calls, four sound_update_channel calls and one menu_input_process call; no new runtime helper call.

## Actual source semantics

Retail homes24/28/32 establish `(s16 x, s16 y, void *object)`. D_80118E20 selects horizontal placement adjustment: mode1 subtracts the unsigned object-manager result shifted right1; mode2 subtracts the full result. Both narrow x back to signed16. D_80118E24 selects vertical adjustment: mode1 subtracts the signed16 byte2+byte3 sum divided by2; mode4 subtracts their full sum. Sentinel -32768 prevents the corresponding global signed16 store. The final real menu_input_process receives object and -1.

The existing real object_bytes23_sum getter sequence reads byte2 after sound_update_channel(0), then byte3 after a second sound_update_channel(0). It is safe to use its signed16 result in mode4 because unsigned bytes sum to at most510, so the cast changes no values. The two reads stay sequenced in that order. A plain `getter2()+getter3()` control scored76 but IDO evaluates those calls in reverse order; discarded as a delivery despite its numerically lower score.

All A3 source context bodies were copied unchanged. Seven previously accepted callers still independently MATCH in this module (mode_byte2_set, mode_byte_set, object_type_byte2_get, object_type_byte3_get, object_byte9_set, func_800F68A4, camera_shake_update); they are context only and provide no new coverage. sound_update_channel is125/122 emitted words with3 extras, empty func_80096288 emits2/4, slot_value_get and both sum functions remain NONMATCH context, as recorded in scores.json. The inherited unreachable empty-callee switch inhibits inlining but has no runtime effects; no new empty-body or dead-code trick was added.

## Directed controls and remaining blocker

34 compiling controls in controls1/2/3.json cover literal accesses, real ordered sum/getter inlining, signed/unsigned subtraction carriers, explicit signed16 narrowing, real summed-value carriers, and local/formal x/y webs. Named temporary returns/subtractions are inert. Copying x into a separate local produces99 words but still80/99 differences; copying y worsens to102/105 words. No register-pressure source or padding was retained.

One intermediate script used malformed text replacement for four sum controls; those syntax-error controls were discarded and corrected from the authoritative baseline before producing controls3.json. Its nine retained controls all compile. Original control snippets and caches/build outputs stayed in private scratch.

Concrete unresolved differences: compiler x-narrowing reuses the x carrier directly, while retail emits two additional temp/move webs. The compiler also hoists the bank-global address into retained t4 across each sound call pair; retail emits separate lui/lw loads. Full raw sound register-def audit confirms both retail and delivered context write at,v0,v1,a0-a3,t6-t9,sp,ra, leaving t1-t4 intact. Falsely changing its clobber contract to force a match would be unjustified. Residual shape/allocation is large enough that no source is claimed close or accepted.

Reproduce:

```sh
rsync -a cloud/work/ipa-groups/codex_state_utility_a8/ Rocky:agents/A/wt/cloud/work/ipa-groups/codex_state_utility_a8/
ssh Rocky 'cd ~/agents/A/wt && python3 tools/cloud/score.py group cloud/work/ipa-groups/codex_state_utility_a8 --claims'
ssh Rocky 'cd ~/agents/A/wt && python3 cloud/work/tools/amatch/builder.py group cloud/work/ipa-groups/codex_state_utility_a8 --json'
```

Canonical --claims exits0 only because claims are empty, explicitly reporting work in progress; see score_claims.txt. scores.json carries the actual NONMATCH verdict.

Frozen group.c SHA256: `38664e153822b588a75f5416e4626716270ecf43e07516a98288eeae9af20b75`.
