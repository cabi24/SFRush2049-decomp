# LEAN RESEARCH: race/session result packing

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: extracted-game `func_800F45F8`, 900 bytes / 225 words.

At IDO 5.3 `-g0 -O3 -mips2 -G 0 -non_shared` plus canonical
`-Wab,-r4300_mul`, positional differing words improve **113/225 to 85/225**.
Both sources still have **6 nonzero excess words** and the canonical scorer's
**unpaired R_MIPS_HI16 for input_rec0 at .text+0x36c** diagnostic. It remains
explicitly unfixed and unmasked. This is a research nonmatch, not acceptance,
a verified relocation pass, behavior proof or ROM credit.

## New source reason

The native code retains the resource-data pointer through its bit-packing loop,
then derives the session at byte offset 1780 for the scalar updates. B80 supplied
a source with this organization, but also changed the player-count expression;
it scored 199/225. The new candidate uses that actual data-pointer lifetime while
retaining the original `6 - (active_player_count == 1 ? 0 : 2)` count expression.
This measured combination improves the stronger 113-word baseline to 85.
A separate typed-owner control was unchanged and is not included.

The whole body remains present: optional first-two-value swap, three packed bits
per input row, elapsed score/length, the two real update calls, ranking-based
level adjustment and final real object-data call. No new helper, fabricated
formal, dummy operation, padding local, volatile, forced register or assembly
is added. Session fields retain the existing native layout; their object-relative
byte address is explicit. This is N64 reconstruction, not a claimed arcade donor.

The existing valid-state assumptions remain: six real ranking rows with a first
and second place; valid selected handle/object/resource chain; enough values for
four or six consumed entries (and two for the swap); allocated session storage;
valid bit positions within the 56-byte packed member; representable ordinary
floating-point update; and established callback effects. The first/second indices
are not given invented fallback values for malformed rankings. The source adds
no bounds checks or new behavior. Real callees stay external.

## Minimal replay

Set `IDO_DIR` to the pinned compiler and run:

    python3 replay.py --repo /path/to/SFRush2049-decomp

It materializes named immutable Git inputs, compiles baseline/candidate at the
same flags, and reports canonical score, diagnostic and true ELF extent. No
scorer or comparison boundary is changed to hide the excess body. The source
hashes and observed result are in observed.json.

No independent review, behavior harness, full suite, CI wait or ROM gate was run.
Checker/Claude owns verification, acceptance and ROM integration. No protected
source/tool/target, lock or production file changes are included.
