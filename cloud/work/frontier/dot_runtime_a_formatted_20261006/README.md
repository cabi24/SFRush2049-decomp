# Image-A formatted menu callback

## Current receipt schema (2026-10-06)

Current receipts omit whole-file manifest, scorer/own-data tool, lock and redundant
production-context digests. The recorded BASE and `git show BASE:path` source reads
remain, as do native target authentication, own packet/verifier/C-harness bindings,
compiler executable identities, complete compiled extents, relocations, owned data
and behavioral evidence. Historical compatibility normalizers accept only their
explicitly listed legacy fields; unknown proof fields and changed invariants still
fail comparison. Descriptions of the earlier receipt schema below are historical.
This schema correction adds no matching or accepted bytes; fresh replay and the
aggregate test matrix are separate required checks.

Strict standalone MATCH for **A:func_803AF744**, `[0x803AF744, 0x803AF978)`,
**564 bytes / 141 words**. Base: `cd22879d40b3de443cfde047b86e75e159b6cec6`.
This is a source/evidence submission awaiting the independent checker; it earns
**zero newly accepted bytes or cartridge/game-blob coverage**.

## Source and authentic contracts

The protected image-A extent records a data-reference start and prologue.
The callback table itself was not inspected. Image B ends below this address.
The callback has ordinary O32 entry/exit, homes its unused word argument, uses
only a saved return address, formats a local text buffer, renders one or two
menu rows, restores drawing state, and returns one.

Accepted image-A `func_803AE51C` and independently reviewed PR #143
`func_803A6A28` supply the same witnessed draw-state and menu-text callback
structure. They do not establish an original translation-unit boundary.
The authentic arcade checkout at commit
`845329d7b36f5a384c5625ed9a0aef584ab46139` was searched in `game/select.c`,
`game/xselect.c`, and `game/attract.c`; its formatting/selection code did not
identify an equivalent callback. This reconstruction is N64-specific, without
an asserted direct arcade donor or recovered original source spelling.

Native helper contracts are separately bound by complete body hashes and entry
addresses in the receipt:

- `render_helper` takes one float; `func_800B669C` takes two word-sized states.
- `object_create` takes a word selector and returns an ignored pointer;
  `dispatch_handler` takes a word selector.
- `camera_auto_follow` takes six signed halfwords and a text pointer;
  `state_utility` takes two signed halfword coordinates and a text pointer.
- `func_800A3508` is a pure four-instruction rounded shift. The actual argument
  `0x810` deterministically yields **9**. This leaf has no global side effects,
  so C argument-evaluation ordering around that call does not alter the
  formatted pointer values.
- `object_bytes_sum_global` at `0x800B3F50` returns a sign-extended signed-halfword
  sum of two unsigned bytes and one signed byte. Its attainable return range
  is **[-128,637]**. Within this domain all caller arithmetic and halfword
  coordinate conversions stay representable. It is treated as a bounded
  external helper in behavioral tests, not an executed renderer implementation.
- The static `sprintf` wrapper at `0x80004990` homes O32 arguments and supplies
  a contiguous varargs pointer to its formatter. Its declared return is ignored.

The `MenuText` view uses pointers at native offsets 344,352,360,608,612,616,
624,944, checked under a 32-bit C ABI. Format `D_803B92B4` remains opaque.
The pointer-field interpretation and descriptive names are inferred; the
original format string, full object definition, and text contents are not
claimed recovered. The behavior domain requires valid stable text pointers,
a valid format for the supplied arguments, and formatted output of at most
256 bytes including its terminating NUL. This is **not a universal formatting
or buffer-overflow safety proof**.

Helpers may change enabled/selection globals and the text-object pointer
between calls. The source reloads them at the native observation boundaries;
hoisting or caching selection across calls is incorrect under that contract.

## Matching history

The first natural complete source emitted the exact 564-byte instruction
sequence except for five stack-home operands. Workbench diagnosis established
same instruction count, registers and 320-byte frame, with operand-only
residuals. Native stack homes identify a compiler temporary at +48, height at
+56, y at +60, and the output buffer at +64 through +319.

Passing the text conditional directly to `sprintf` instead of retaining a
named text local, then declaring the two genuinely consumed scalars in their
witnessed home order, gives strict equality. These are two bounded source
structure corrections, not a variant sweep. No artificial local, pressure
variable, fake parameter/caller, inline stub, volatile qualifier, compiler
change, byte patch, or protected-input edit is present. A matching declaration
order does not prove recovery of the original C declaration order.

## Producer verification

`verify.py` uses current-master scorer/own-data tooling with unchanged protected
inputs. IDO 5.3 recipe: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.

- Exact ELF function extent: **564 bytes**, followed by **12 separately checked
  zero alignment bytes**. No extra function, owned data or storage.
- All **44 relocations** and **13 unique call/data address anchors** checked.
  Independent ELF parsing and GNU whole-object linking equal every native
  body byte. Image identity, entry, end, native hash, helper entries/bodies,
  field offsets, and all allocated sections are checked explicitly.
- Strict score: **0/141 words**, no unresolved/unverified relocations, errors,
  masks or excess nonzero words.
- **119,808 unchanged-source C89 UBSan/bounds traces** with all 256 signed
  selection bytes, four unsigned-enabled states, nine attainable-height
  boundaries, baseline plus 12 mutation positions, and output lengths 0/1/127/255 plus NUL.
  Traces and buffer handoff agree with a separate Python oracle. External
  renderer and formatter implementations are bounded contract hooks.
- Three compiled wrong-source controls (inverted optional condition, wrong
  vertical spacing, wrong text field) fail both strict scoring and hosted
  behavior. Complete native execution and additional mutants are checked by
  the independent reviewer before publication.
- **48 selected scorer/guard/submission regressions pass**, with 668 unrelated
  locked-source cases deliberately deselected. Initial sparse fixture runs
  stopped on absent fixture source/tool inputs; reusing unchanged protected
  targets and the current checked-in tools resolved these environment gaps.
  This is not a broad repository or locked-source rebuild claim.

## Reproduce

From a full checkout with IDO and MIPS GNU binutils configured:

```sh
PYTHONDONTWRITEBYTECODE=1 python3 cloud/work/frontier/dot_runtime_a_formatted_20261006/verify.py --check
python3 tools/cloud/score.py fn cloud/matches/ovl_a/func_803AF744.c func_803AF744 --targets asm/us/ovl_a
```

A source-only checkout can pass `--repo /path/to/protected-input-checkout`.
`--tools-repo` independently selects the checkout containing current checked-in
`tools/cloud/score.py` and `owndata.py`; it defaults to this checkout. Generated
objects and all raw native material stay under ignored `build/` paths.
The committed receipt contains only hashes, counts, interfaces and results.
The receipt is JSON-canonicalized before replay comparison, preserving all
fields while treating in-memory scorer tuples as their serialized JSON arrays.
The first replay exposed this representation-only mismatch; every stored
receipt value and all source/native proof results remained unchanged.
Full-object hash and local host/linker versions are recorded separately as
local provenance; path-sensitive debug bytes are not portable equality gates.

No original-source recovery, actual renderer execution, concurrency, gameplay,
image/recompression/full-ROM or hardware claim is made. Merging, production
integration and final acceptance remain with the independent checker. No CI
watcher or merge automation is requested.

## Integration-portable replay (2026-10-06)

`portable_receipt()` compares both saved and fresh evidence after excluding only
explicit historical whole-tree/tool/source-context digests. Packet source and
verifier bindings, selected native bodies and addresses, ELF extents, relocations,
owned data, behavior, and compiler executable identities remain authoritative.
Accepted production context is read from the recorded base commit rather than
the live integrated tree. Tests are deliberately not hashed into receipts.

The unchanged candidate body was freshly strict-matched through the canonical
scorer with the bare source recipe `-g0 -O3 -mips2 -G 0 -non_shared`; the scorer
still injects its mandatory `-Wab,-r4300_mul` backend flag. Full packet proof was
replayed under O3. No same-unit callers, inline helpers, or deleted-static stubs
are required. Historical O2 results remain available in Git history.
