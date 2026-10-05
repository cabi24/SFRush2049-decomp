# C118 VI getter refusal

The complete natural getter computes `(gViAccumTime - gViTickCounter) * gScaleSecondsPerTick` using the current VI module g0/O2 recipe. All 13 meaningful native words match, including the normal return. The original 68-byte native extent contains two further unreachable JR/NOP pairs; IDO emits a true 52-byte function. Strict score including stack is 400, with no masks, unresolved relocations or scoring errors.

No artificial return, padding, source pressure or VI recipe change was attempted. Existing accepted VI bodies and pins remain untouched. This packet has zero accepted coverage credit. See proof.json and eligibility.json for frozen hashes and exact current lock checks.
