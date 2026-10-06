# LEAN RESEARCH: DA0BC with its genuine DB1E0 caller

Frozen tool/target base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_800DA0BC`, 184 bytes / 46 words.

At IDO 5.3 O3 the old standalone near-miss seed scores **42/46 + 6 nonzero
excess words**, with a 208-byte ELF function and an unpaired-HI16 diagnostic.
The clean complete source in a two-function group with its actual caller scores
**16/46**, **no excess or unresolved/unverified/error**, and an exact **184-byte
ELF function extent**. This is a genuine-context research improvement, still
not a match, accepted source or ROM coverage.

Flags: `-g0 -O3 -mips2 -G 0 -non_shared`; canonical compile_single adds
`-Wab,-r4300_mul`, and compile_group passes the same erratum setting to as1.
The replay records the different standalone/group modes explicitly; this is not
presented as an isolated source-spelling comparison.

## Real interfaces, context and provenance

The native cleanup handles 32 real 64-byte rows at D_8011650C, with a signed
full-word identifier at +4. Each non-sentinel ID is narrowed to the actual
entity_spawn_callback(s16,int,int) argument, then the original word is reset.
State-dependent sound cleanup, the separate sound handle, row-count reset and
final byte-flag reset retain native order. No invented fallback or extra call
is supplied. No dummy, padding local, forced register or volatile is introduced.

Native DA0BC uses s0-s2 without saving them; its sole direct caller is DB1E0.
`caller.c` is the **unchanged earlier complete DB1E0 reconstruction** from the
DA2C0 options-menu packet, originally researched at base
`9651ca53bfd16b015adb63e1b3d76487a995a84f`. It is not newly reconstructed here.
Its SHA-256 is
`37da94b5178384b1e2d85bb08685f242d76e99b072608e541d8cbf0858aba8a0`.
DA2C0 and the remaining real callees stay external; no stand-in caller or helper
body is added. The cleanup uses the same D_801174B4 symbol as the caller.

The caller remains nonmatching in this bounded group: **344/350 differing,
1300 versus 1400 bytes**, with **two unverified owned-table relocation sites**.
It receives no new matching claim. This is not a complete original compilation
unit, and adding further genuine context can change either result.

Assumptions: valid row storage and established external callback/queue effects.
Identifier -1 is the only skip sentinel; zero is a valid identifier. The source
uses the N64 O32 layout. No malformed-state or application-wide guarantee is
made, and prior behavior results are not claimed as tests of this new group.

## Minimal replay

With the pinned IDO selected in `IDO_DIR`:

    python3 replay.py --repo /path/to/SFRush2049-decomp

The script reads named immutable Git inputs and compiles the published C/group.
It reports canonical strict scores, all unresolved/unverified/error fields,
source hashes and ELF extents. No comparison boundary, target or scorer changes.
No independent review, behavior harness, full suite, CI wait or ROM gate was
run. Checker/Claude owns verification, acceptance and ROM integration.
