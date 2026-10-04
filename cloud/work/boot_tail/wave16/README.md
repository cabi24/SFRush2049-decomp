# Sixteenth source cut: two prior nonmatches closed

Two independently reviewed bodies now strictly match: `8001C1D8` (432 B) and
`80020820` (484 B), a matching-source gain of **916 B**. Both have full relocated
word equality, one exact-size ELF function symbol at offset zero, and clean
relocation proof. Their original nonmatching C, controls, receipts and historical
manifests remain unchanged. This cut attempts **zero new target addresses**.

The current unique state becomes **239 new matching bodies / 28,096 B**, plus the
separate historical 12-byte getter. The distinct attempted population remains
394: 154 current complete nonmatches / 44,312 B and one 96-byte blocked lead remain
outside matching credit. The manifest binds each reclassification to its prior
source hash and residual. Totals select the latest classification by address;
396 historical selected-source records are not 396 distinct attempted targets.
Exact-head hosted CI remains pending for this source cut.

## Narrow source-line scheduling finding

The retained loops put the loop header and store body on the same physical line.
Ordinary multiline braced loops separate their source ownership. Stock compiler
reports explain how original and unrolled operations receive header/body line
ownership and how the stock assembler schedules the resulting stores. This is
observed even with the recorded `-g0` flags.

For `1C1D8`, the genuine 16-halfword clear changed from four rotated store sites
to exact native order. For `20820`, the two genuine 134-byte clears changed from
ten schedule differences to exact native order. Independent orthogonal controls,
whole native/source ABI checks, exact ELF extents and semantic tests reproduce
the effects. No fields, arithmetic, data accesses, helper bodies, arguments,
manual unrolling, artificial padding, directives or compiler changes were added.

This is a narrowly demonstrated source/loop ownership effect, not permission for
arbitrary blank-line padding or a generic formatting sweep. A new target still
needs an actual residual diagnosis, exhausted-control review and a concrete
source-supported hypothesis. Stack-home and narrow-result allocation plateaus
are not thereby solved. The separate stream retry found no new lever and stopped;
other negative probes do not add attempted or matching credit to this cut.

## Provenance and checks

Parent: published PR75 head `d3e457b05b9eb7b5a4512e4ae4b1fc32c34e268f`;
its exact-head success is carried in `../wave15/ci.json`. Neither prior published
source nor prior packet is rewritten. The reviewed source manifest binds final
peer commits and SHA-256s for both new canonical C files. All actual parameter,
layout, valid-object and external-data qualifications from the original packets
remain in the new independent reviews; no original external values are guessed.

Use authenticated `IDO_DIR`, then `verify_wave16.py --check`, `generate.py --check`,
the central tests, changed-submission rescoring, protected-path and 161-lock
checks. Reproduce the packet compiler/control/layout/host/native proof through
each packet README. The exact whole-function extent check is independent of the
scorer's nonzero-excess count. D10 and all protected inputs remain unchanged.

No ROM bytes, raw assembly dumps, compiled objects, credentials or raw internal
coordination notes are included. Merging and cartridge promotion remain with the
independent checker. Later work is excluded from this frozen source cut.
