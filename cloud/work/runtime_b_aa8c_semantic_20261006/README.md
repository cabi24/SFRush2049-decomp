# Full AA8C semantic research and source-boundary proofs

This additive packet preserves the complete physical-address semantic C model,
its independent review, bounded actual-native service checks, and the narrowed
owner/source-object contract. It is **verification-only research**. It is not a
MIPS matching submission, original translation unit, native C-object identity
proof, whole-program input invariant, or gameplay claim.

Base context: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`.

## Contents and established results

- `packet/`: the full AA8C callback plus its two real private cleanup functions
  expressed as executable semantic C over an explicitly defined physical bus.
  The authenticated native closure contains 2,029 words / 8,116 bytes. Its 1,759
  paired cases cover all 2,027 reachable words, 112 branch outcomes, and 64
  direct call sites. Two duplicate native loads are structurally unreachable.
  Ten wrong C contracts and two fail-closed controls are rejected.
- `review/`: independent challenge cases run the actual semantic C with both
  host C99 O0 and host C99 O3 plus UBSan, with floating contraction disabled:
  2,766 cases per configuration, or 5,532 additional pairs. Eight wrong C
  variants and a separate double-product rounding mutation are rejected.
- `effects/`: 61 bounded actual-native service calls cover 480 instructions.
  They support retained transform pointers, sequential overlapping copies,
  position-before-matrix writes, handle/resource widths, removal/link updates,
  and the callback's visibility modes. These helpers are tested separately;
  the full callback replay continues to use explicit service effect models.
- `objects/`: 105 full-native initializer cases, 630 actual flag projections,
  and 567 UI scan/clamp cases establish a conditional owner-domain result.
  Positive count 1..4 followed by mode-6 setup and physics flag projection
  leaves owners 4/5 inactive. Native count-5/6 counterexamples remain recorded.

The producer plus independent review totals 7,291 native/host-C pairs. The
independent cases alone cover 2,020 words; the complete reachable-closure claim
comes from the separately replayed producer. Host O3 is semantic verification,
not IDO compilation or matching evidence. The complete C body and both real
private helpers remain intact; adapters are explicitly host-only functions.

## Open gates

Mode 8 is genuine. Its model-indexed physical offset reads span
`[80394884,80394920)`, overlapping independently used color and vertex data.
The bus preserves those loads without inventing a ninth float row, enclosing
union, or merged native object. Original declarations, extents, effective types,
and alias ownership remain unresolved. Legal speculative loading from a genuine
source-level guard has not been demonstrated under the canonical IDO route.

Registration can name owners 0..5, while the known attachment regions have
four-record layout extents. The bounded setup/UI result does not establish that
every registration is dominated by that path or that no intervening producer
reactivates owners 4/5. Counts 5 and 6 explicitly preserve those owners in actual
native setup. Model-byte bounds, general service pointer validity, N64 trig,
native RNG state, FCSR/exceptions, and complete-game behavior remain separate
obligations. A fixture restriction cannot replace these missing proofs.

## Portable replay

After integration, from any working directory:

    SFRUSH_REFERENCE_ROOT=/path/to/repo TMPDIR=/writable/tmp \
      python3 -m pytest /path/to/repo/tests/cloud/test_runtime_b_aa8c_semantic_20261006.py -q

The tests also find their repository automatically when integrated. They invoke
the packet runners in subprocesses from a foreign working directory, avoiding
generic-module collisions with other research packets. Set the repository's
normal Python dependencies as usual. Host-C checks require `cc`; independent
O3 review also requires its UBSan support. A missing host compiler causes those
three checks to skip before execution. Native service and owner-domain checks
need no compiler, IDO, or MIPS linker. Unsupported UBSan is an explicit compiler
failure, not a silently successful review.

The original standalone tests are retained beside each applicable packet.
The complete semantic C, host adapters, fixtures and independent challenge
runners are preserved byte-for-byte. Three packet-owned native loaders add the
selected checkout's tools/cloud import directory for the newer canonical
scorer's sibling owndata dependency. Rebound receipts retain identical proof
results, apart from those owned-file digests. Replays
bind owned C, adapters, fixtures and verifiers, native target words read through
the canonical `score.targets()` loader, and production context read with
`git show` at the recorded base. The initialized-image digest describes that
base-commit native image, never the mutable live integration tree. Receipts do
not bind test files, changing manifests/scorers, production source, live context
digests or current lock state. No raw native/image/disassembly bytes are shipped.

This packet adds its own paths only. Earlier entry and selector packets are not
copied or replaced. It changes no production sources, native assets, contexts,
locks, scoring/build tools, or matching claims. Focused replays do not replace
the current-master full with/without-IDO prepublication matrix or independent
review required before any later publication.
