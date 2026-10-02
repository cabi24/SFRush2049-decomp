# C41 — genuine variadic constructor, bounded nonmatch

`car_stats_display` at `0x800D197C` is the complete canonical 308-byte/77-word extent. Current blob locks, accepted address intervals, all report text and artifact filenames were checked before reservation; no earlier source packet or accepted overlap was found. No acceptance files changed.

Actual ABI: signed kind, f32 value passed through a1, two u32 values, signed count at caller stack+16, then u32 variadic arguments. The function allocates a real object, fills up to four words, publishes it through the actual list helper, and returns its original handle across osJamMesg. SDK IDO stdarg definitions are copied literally from `reference/repos/ultralib/include/compiler/ido/stdarg.h`. Object field offsets come directly from the original loads/stores. List tail is genuinely the field at +8 in D_80149860; treating D_80149868 as independent storage generated an unnecessary address load.

Exact flags: `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.

Baseline: strict stack-sensitive 1479, unmasked linked 50/77 words different. Correct list field and store order: strict733, 28/77. Genuine cached start float: strict713, 24/77, exact extent, no extras/unresolved/unverified/errors. Remaining differences are varargs loop scheduling, integer allocation and the saved object slot; no zero and no credit.

Ten compile controls including baseline: typed list/store order, while form, explicit consuming argument cursor, real declaration ordering, genuine output pointer, cached start float, signed return carrier. One accidentally identical cursor control is explicitly retained in controls.json; controls2.json contains the actual distinct cursor source. No synthetic arguments, pressure operations, padding, broad line sweep, driver changes or scorer relaxation.

Best source `game_C41/car_stats_display.floats.c`, SHA256 `9379de779f4a0e1403256a94cf9a9159695d816a46a11a8dfa727f4ce3c27522`.
Authoritative target object SHA256 `358e2dc41e201769a21f735e40e17dfcfabee5caeb3273fdf8d5c1b13527c33a`, tier reloc_aware, extent_repaired. Object and compiled candidates stay private at Rocky `~/agents/C/scratch/game-C41/`. Checked-in baseline/controls/targets carry complete score/provenance records. No pending jobs.
