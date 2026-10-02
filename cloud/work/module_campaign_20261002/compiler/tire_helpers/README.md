# Tires module helper audit

No new matching claims. Accepted physics sources and the frozen clutch group were untouched.

## calctireuv — camera_collision_avoid, 412 bytes

Direct ancestor: `rushtherock/game/tires.c:calctireuv`. N64 replaces the short steering/pitch basis construction with float sine/cosine, a transformed road normal, and two cross products. The eight observed ordinary ABI arguments are consumed. The local tire velocity is exactly three floats. The native caller uses 36-byte bases and 12-byte vectors.

Fresh baseline full O3 reproduces 14/103 differing words. Every residual is a frame or stack-home displacement: native frame 120 bytes, candidate 104. The register and arithmetic/call webs match. Workbench diagnosis was run before controls, with raw artifacts retained only in ignored build storage.

| Grounded control | Differing words | Extra words | Retained transform helper |
|---|---:|---:|---|
| Prior complete struct body | 14/103 | 0 | External |
| Genuine float arrays | 48/103 | 0 | External |
| Array form, original integer zero | 48/103 | 0 | External |
| Array form with complete accepted transform | 48/103 | 0 | 0/37 |
| Best struct form with complete accepted transform | 14/103 | 0 | 0/37 |

The complete transform body is genuine accepted source, compiled with both retained entries through the protected O3 recipe. It receives no additional source credit. None of these comparisons has unresolved or unverified relocation sites or errors. No capacity, padding, spare coordinate, dummy pressure, fake ABI, or removed donor short local was added. The frame gap remains unexplained source ownership; freeze until a new real source/context lead appears.

## dotireforce — camera_follow_target, 812 bytes

Direct ancestor: `rushtherock/game/tires.c:dotireforce`. Its actual N64 ABI removes opposite tire velocity and retains original airfact as argument fourteen. Native side/normal/traction axes are 0/1/2. The friction-circle callee has seven ordinary arguments, and the final basis transform has three. Native Tire92 fields and Model2056 offsets were audited into the standalone typed source. The unused airfact formal is retained from genuine donor/caller evidence, rather than invented to affect compilation.

Fresh complete opaque fourteen-argument control reproduces 177/203 differences, no extras. Typed fields yield 178/203; both have an 88-byte frame versus native 96. The reconstruction lane owns subsequent genuine donor temporary/literal/expression controls. This compiler lane owns independent full body/pool/ABI verification if a match emerges.

Numerical comparisons and source/object/protected-target hashes are under `results/`; raw objects, instruction streams, diagnostics and image bytes remain ignored. No target, scorer, masks, accepted source, locks, or main tooling were modified.

## Complete accepted tires context

A fresh full O3 group retained the complete unchanged accepted `src/blob/camera_dolly.c` and `src/blob/func_800BC21C.c` in separate translation units, together with the typed fourteen-argument caller. The caller remains 178/203. The accepted friction-circle body is 0/575 words and the slip-angle body is 0/40. Independent full relocation proves all 2300 and 160 body bytes equal, with no masks, unresolved/unverified sites, or errors; receipt `results/accepted_tire_context_preservation.json`. All three entry points are retained. No helper gets new credit, and its accepted source hash remains unchanged.

## Final bounded dotireforce controls and freeze

The reconstruction lane tried original consumed donor scalar ownership (179/203), actual array maximum comparison (182/203), real literal coefficients (182/203 plus ten unresolved local-pool reference checks), and original integer/cast literal types (179/203 plus ten local-pool checks). None yields a matching lead. The coefficients are numerically native; their local pool ownership is not claimed proven because the body is already a nonmatch.

The compiler lane's final two grounded controls distinguish original vector integer-zero stores from scalar float-zero assignments (186/203), and use a separate native polynomial force-vector compound update followed by donor `normal=forcevec[1]` (176/203). Both preserve the complete fourteen-argument body without extra words. The best force-update form was replayed in the full genuine O3 context group: 176/203, accepted friction-circle 0/575 and slip-angle 0/40. No remaining source-grounded control justifies continued frame/register sweeps. Freeze this complete NONMATCH packet; no source coverage credit.
