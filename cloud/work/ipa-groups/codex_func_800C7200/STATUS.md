# Codex real-closure seed, 2026-10-01

No claims; does not compile yet. Five real m2c bodies, no stand-ins, generated under build first and copied here as a reproducible starting point. `func_800C7200` was removed from keep to reproduce its IPA-internal temp reservation; real callers remain kept. No splice or lock mutation.

Initial direct closure: func_800C7200, func_800C813C, func_800CC50C, func_800C7EC8, draw_ui_element: 696 target words. This is substantially larger than the pilot's synthetic two callers. `draw_ui_element` has an additional real external caller func_800CBF2C. It also preserves a2/a3/t0-t2 across calls to real format_string_parse, requiring that 45-word function in the unit; direct closure missed it. Fully transitive closure expands into the UI graph (menu_back, draw_speedometer, props_render etc.), so blindly selecting full mode is unsuitable.

Current generated defects confirmed by the compiler: func_800CC50C second argument is a byte-output pointer but sigs chooses s32; func_800C7EC8 reads s0 on entry but sigs leaves its prototype parameterless and seed introduces an uninitialized saved_reg_s0 scalar; draw_ui_element loses t0/t1/t2/a2/a3 after the external format_string_parse call and emits M2C_ERROR placeholders. This seed must not be claimed or spliced. Replacing placeholders with zero would be a fake result.

m2c tooling: default groupgen private-copy patch application failed because the existing tools/mips_to_c tree is already patched (0001 patch did not apply). Explicit GROUPGEN_M2C=tools/mips_to_c reads the already-patched source and generated 5/5 bodies successfully without changing that submodule. Rocky A lacks m2c dependencies, so generation ran on coordinator and compilation on Rocky.

Best previously verified pilot result remains 4/64 words for func_800C7200 with synthetic callers; that result is not spliceable. No new verified result is claimed by this real closure experiment.
