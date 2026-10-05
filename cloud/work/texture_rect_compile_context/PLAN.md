# Authentic compilation-context control for 80087110

Recorded before compiling this experiment, 2026-10-05, checkout 493e7835.

## Hypothesis and provenance

The existing 445-word C leaf has six ordinary O32 inputs and no callees.
Consequently it does not require an IPA callee closure to explain its four-word
schedule residual. Whole-program compilation can nevertheless alter symbol or
alias information available to a kept external leaf. Test the specific claim
that adding the unchanged leaf to the accepted graphics family changes its
output. A change would warrant examining the actual linked-Ucode metadata;
identical relocated bytes falsify this proposed lever for these inputs.

The authentic control is the current accepted `src/blob/groups/gfx_modes`
group. Its six input files now include complete `object_render.c` and
`init.c`, accepted on October 5. The initializer really writes the rectangle's
clip bounds, mode, and flag globals; the other members consume the same
command-list pointer and mode. `object_render.c` supplies the genuine s64
cache definition that the existing initializer build requires. These are
complete production bodies, not reconstructed stand-ins. None is modified.

The original C file boundaries and original linker root list are unknown.
Accepted reconstruction groups prove an effective compiler recipe, not the
original source layout. The group has no real caller of 80087110. Add that
leaf as a retained external root, reflecting the existing six-word O32
interface and permitting callers outside this bounded control; do not mark it
internal or invent a caller. There is no assertion that this is the original
renderer TU.

## Bounded experiment

1. Stock-compile the unchanged frozen standalone rectangle baseline.
2. Rebuild the unchanged accepted graphics group as a regression baseline.
3. Add only the exact frozen rectangle file and its retained external root to
   an ignored copy of that group. Use the stock, unchanged `compile_group`
   implementation and its existing flags/optimization limits.
4. Compare the entire rectangle extent and all seven existing group bodies.
   Preserve unresolved/unverified information and distinguish strict zero from
   relocation-blind zero. Check source hashes before and after the experiment.

No keep-root variants, compiler-option search, scope rewrites, dummy functions,
assembly changes, global-definition inventions, or dead-code pressure probes.
An unchanged rectangle ends this experiment. A neighbor regression invalidates
it as a production proposal. No acceptance, lock, splice, image, or ROM claim.
