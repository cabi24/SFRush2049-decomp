# Incoming matching PR cartridge integration

Reviewed PR #24 and #40 at the exact heads in gates.json. Fresh pinned IDO O2 compilation of their complete natural C bodies yields exact native words for func_800B7360 (132 bytes) and func_800A7830 (44 bytes). The latter represents the genuine unused incoming third word demonstrated by native homing; its semantic type and indirect callers remain unknown. Table bounds are not inferred. The former preserves four unsigned-byte argument homes and cache/invalidation stores.

All 664 prior game lock entries are identical. All 666 accepted native bodies were reconstructed from actual compiled objects, with no missing-object assembly fallback, and compared in full against the original image. The complete 647,072-byte image, original 326,180-byte compressed stream and full original ROM SHA-1 pass, MAKE=0 TEST=0. Existing source/group locks pass. No new tests were added or local repository suites rerun for this integration; the submitted revisions already had green CI.

Accepted gain: 176 game bytes. Game coverage: 666/1,216 functions, 100,656/647,072 bytes (15.56%). Static: 158/230 functions, 44,808/61,440 bytes (72.93%). Combined byte tracker: 145,464/708,512 (20.53%). Research incorporation earns no matching credit.

[gates.json](gates.json) contains exact source/body hashes, recipes and gate outcomes.
