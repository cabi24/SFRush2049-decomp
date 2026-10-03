# B117: directed B49 difficulty_select follow-up

No coverage claim. LOCAL main lock does not contain difficulty_select or its lap_counter_display alias, 0x800D2A74 / 412 bytes. This is an explicitly labeled follow-up of B49, preserving all frozen B49 sources. Root notified before reconstruction.

Fresh independent compilation reproduces native 25 / 103 differing words, no nonzero excess. Diagnose ran before refinements: native frame48 vs original32, while the recursive call and broad instruction structure align. The original has true four consumed word arguments, 80-byte records, 16-byte descriptors and only genuine self-recursion.

Two bounded dataflow controls: reusing the consumed anchor variable as its selected base produces 102 / 103 with one nonzero excess; reusing original formals only within their terminal branches produces 99 / 103 with one excess. Both worsen the native baseline and remain unclaimed. No artificial pressure, unused formal, buffer, masks, symbol/scorer alterations, reflow search or fake helper. All comparisons have zero errors, unresolved and unverified references. Actual flags and SHA-256 are frozen in results; detailed diagnosis and raw objects remain private temporary evidence. Shared accepted sources, locks and integration untouched.
