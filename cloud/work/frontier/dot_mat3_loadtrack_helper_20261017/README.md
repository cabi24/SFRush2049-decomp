# LoadTrack: genuine N64 view/checkpoint helper boundary

Research only. Empty claims and zero new accepted bytes. Base: 58425072b49e95f0fbe5c4c239988fb4885f7c5c.

LoadTrack at 0x800BB9B0 remains 39/526 differing words (2104 bytes, frame 168). This packet replaces the historical hypothetical `w30_Clear(s8 *)` followed by `audio_dsp_process()` with the genuine locked `mode_flags_clear()` at 0x800BAF64. Its unchanged source clears the first two view flags at D_80110680 and calls the checkpoint initializer. The original file is copied byte-for-byte from `src/blob/mode_flags_clear.c` (SHA256 78a8448ef0cddcfc13d6d98bd13e4c6c2477242500dd84ac1f4e0538d49dadcb).

The complete relocated LoadTrack body is identical to the historical candidate, not merely equal in score. All ten slot_sound functions and the separately kept 44-byte mode_flags_clear are strict matches. The helper is eligible for inlining into LoadTrack while retaining its exact standalone body. No unused locals, artificial reads, synthetic caller, forced registers, compiler modification, or assembly replacement was introduced.

Six fresh-build controls:

| Build | LoadTrack words differing | Frame | Helper words differing |
|---|---:|---:|---:|
| Genuine native helper | 39/526 | 168 | 0/11 |
| Exact repeat | 39/526 | 168 | 0/11 |
| Historical hypothetical helper | 39/526 | 168 | not part of historical group |
| Flatten genuine helper at caller | 62/526 | 160 | 0/11 |
| Clear second view flag twice | 69/526 | 168 | 10/11 |
| Call path initializer instead of checkpoint initializer | 40/526 | 168 | 1/11 |

Flattening is semantically equivalent for this boundary but removes the 8-byte inline area. It is a structural negative, not a claimed behavioral error. The other two negatives deliberately change behavior. The ten slot_sound functions stay strict exact in all six controls. Every comparison requires no unresolved symbols, unverified relocations, relocation errors, or extra words. Source, verifier, and all twelve target rows are SHA256-bound by protocol v2. All verifier guards raise explicit exceptions and remain active under `python3 -O`; optimized-interpreter tests require rejection of false claims, changed source/verifier/targets, and an unverified relocation.

Source evidence also supports the donor `int *worldptr` local, which is body-inert here. GC LoadTrack contains used endian-conversion locals swapTG and swapT; neither is imported as unused padding. GC supplies the genuine NewWorld -> FindWorld lookup boundary, but has no getter at its final race tests and no helper between InitVisuals and InitCheckpointData. The replacement above is specifically supported by the locked N64 helper, rather than that GC gap.

Limits: this does not close the two known LoadTrack residuals: one temporary-register allocation before trackInited, and the WorldBase store/reload address lifetime. Historical volatile-u32 WorldBase and differential_output declarations remain in this candidate and are not claimed to settle the original native type contract. Earlier pointer/volatile declaration experiments remain negative. The independently reconstructed donor helper bodies remain research inputs. Existing locked source coverage is preservation, not new credit.

Observation-only uopt tracing of the genuine helper candidate produced a complete object identical to the stock compiler object (SHA256 9175d3ac0f0b5224f7a6abe90d01d30da781aaf4e7ca89789e920e86523bf3b9 before the comment/type-only packet cleanup). The replay uses only stock tools; no trace binary, raw disassembly, object, ROM, or external donor dump is included.

Run `python3 cloud/work/frontier/dot_mat3_loadtrack_helper_20261017/verify.py` with the existing IDO toolchain configured. The optional argument selects an independent checkout supplying the unmodified scorer and protected target artifacts. No canonical build, full integration, merge, or CI watcher is involved.
