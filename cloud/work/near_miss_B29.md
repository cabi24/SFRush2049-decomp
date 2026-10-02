B29 — real floating ABI and intrinsic audit (frozen 2026-10-01)

No new accepted coverage is claimed. The independent 80-byte func_8008B424 match corroborates C25's earlier source; root selected C25 and credits it once. B29 retains its own exact variant privately as func_8008B424_sum1.c (SHA256 ae89de3665376aa8a03865299a93d9a2809a857f317aa89e0e9aa2126fcf7083).

Every probe uses the unchanged strict protected-target scorer and literal flags `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`. Original baselines were replayed from build/codex-r4300-sweep/sources. Sources and complete verdict summaries are in near_miss_B29/results.json. Raw target/disassembly streams remain ignored build artifacts.

| Target | Original strict residual | Best repaired residual | Finding |
|---|---:|---:|---|
| func_8009D708 (660 bytes) |164/165 +17 extra|155/165 +7 extra|Actual f32 sqrtf declaration and intrinsic remove implicit-int call. Genuine float fourth argument and native matrix indices retained; widespread frame/register allocation differs. Capturing the already-live argument regresses to161/165.|
| audio_channel_alloc (676 bytes)|168/169|161/169|Actual descriptor has u16 count at0 and pointer at4, rather than two halfword values. Real six-byte point stride, widened loop counters, and inline sqrt repaired. Retail contiguous distance scratch/vector lifetime remains structurally unresolved.|
| camera_follow_target (812 bytes)|189/203 +1 extra|188/203|fabsf intrinsic and real camera_dolly seven-argument float/integer/pointer ABI reconstructed. Actual three-float output vector is consumed by transform. Frame/lifetime mismatch remains.|
| func_800A557C (456 bytes)|110/114 +11 extra|70/114|fabsf intrinsic and actual modff(f32,f32*) ABI remove phantom second float and implicit-int calls. Cleaner polynomial/sequence repairs score74/114 and86/114; numeric reconstruction is genuine but no machine match.|

Authoritative ABI evidence is include/game/math.h (float sqrtf), actual modff assembly in asm/us/34A0.s at80002A64 (f12 scalar and a1 output pointer), and adapted src/libm/math.c. A557C's seed read an output before modff initialized it and reused overwritten polynomial intermediates; natural variants fix those defects rather than preserve undefined behavior to force coloring. The descriptor relation D_801407F4=D_801407F0+4 is actual image/symbol evidence. Return types of ignored auxiliary calls are not claimed fully audited.

No fake buffers, volatile pressure, invented formals, dummy helpers, scorer changes, relaxed masks, or stand-in groups were used. Full measured residuals, including regressions, are retained. These four larger targets stop after bounded meaningful controls; they need real source/context reconstruction rather than indefinite allocation sweeps.
