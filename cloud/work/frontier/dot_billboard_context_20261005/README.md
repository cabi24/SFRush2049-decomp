# Genuine save-input context: F1930 matches

Date: 2026-10-05. Base `f88dc3cb`, following the separately reviewed F207C
packet. **New candidate: F1930, 245/245 words, complete 980-byte ELF extent,
strict MATCH.** All relocations verify. The unchanged production group reader
independently produces the same entire body. **No splice, complete shadow-unit
check, image gate, ROM verification, or accepted-byte gain is claimed.**

F207C (1,684 bytes) and the existing accepted F2718 (368 bytes) also remain exact
in this group. They receive no duplicate new-match credit.

## Scope and genuine source provenance

Claimed matching range: F1930 `[0x800F1930,0x800F1D04)`. Its only native caller,
`billboard_render`, is present in full and is the sole kept root. The group has
no invented caller, substitute helper body, padding, dead read or register keeper.
Compiler flags and protected targets/context/scorers/overrides are unchanged.

Additional reconstructed, **nonmatching** context:

- F1210 `[0x800F1210,0x800F1928)`: old-state cleanup plus the complete six-way
  new-state entry switch, including menu rectangles, queue operations, text
  dimensions, timers, slot refreshes and the final sound/control registration.
  Both switch mappings were checked against protected native jump tables.
- F0F44 `[0x800F0F44,0x800F1114)`: complete slot-status/list traversal from the
  existing genuine `ipa-groups/func_800F0F44/f.c` body, with its synthetic callers
  removed. Native loads prove two missing pointer indirections: the reference
  record is `*(Ref **)D_8014A160`, and each node's descriptor is `*o->q`. The
  linked-list next field is a handle address (`Obj **`). An explicit local
  carries the three actual status-byte reads.
- Billboard: complete genuine earlier caller source, with native-backed pointer
  and return interfaces normalized during independent review; it still does not
  match.
- F2718: the existing accepted body is copied unchanged, while its old incomplete
  F1210 definition and four synthetic caller functions are omitted. Its exact
  result now uses the real reconstructed F1210, not that old substitute.
- F207C: byte-for-byte source copy from the earlier matching packet.

Unavailable helpers remain external declarations. In particular,
track_texture_load is not reconstructed here; it is another actual F1210 caller
and the route to billboard's saved floating-point registers through speed_set.
This family is **not a complete original translation unit or closed call graph**.
The definitions above are retail-grounded reconstructions, not original-source
proof, and their exact local declarations remain unsettled where they do not
match.

## Reviewed declaration repair and production blocker

Independent review found incompatible declarations introduced while assembling
this research group. The scoped repair preserves every accepted/copied function
body and the separate F207C proof:

- D_8014A160 and D_801461A8 consistently use void pointers in the new interfaces.
  F0F44 explicitly follows the native double load with `*(Ref **)D_8014A160`.
- Handle globals and sound_handles_array_clear use the existing s8-pointer
  contract. CC040 has its consumed signed integer return and pointer arguments;
  CC50C returns the handle pointer stored by billboard.
- F0F44's next field is a pointer to a node handle. No guessed tail capacity is
  added: Ref.pos is known to begin at offset +20, but the inherited `[4]` bound
  is **unproven**. The native string comparator is unbounded. This remains an
  unresolved context model, not a guarantee of a four-byte string.

**Production integration is blocked on inherited declarations and open context.**
The untouched accepted F2718 copy still declares/calls viDeadlinePassed with an
extra argument, whereas the other files use a no-argument prototype. Its input
record model and resource_type_select signedness also differ from the other
inherited views. They were not silently rewritten to expand this repair's scope.
An independent fuller normalization control retained exact native bodies, but
that does not authorize changing the accepted source or establish the original
whole-program interfaces.

Consequently, this packet proves exact bytes in reconstructed compiler context;
it does **not** establish a fully C-compatible production unit. These interface
issues must be resolved through reviewed integration before promotion. The
source-bound receipt and regression tests preserve that distinction.

## F1930 semantics and the final word

F1930 handles save-device/menu input. It skips unavailable rows, wraps the device
and row selections, handles overwrite/error dialogs and save/cancel actions,
transitions to state 5 when finished, and refreshes the selected slot at the end.
The current selection is read again after the save helper, as the native code
requires; the host fixture includes a helper changing it during a call.

The first complete reconstructed F1930 was **1/245 words off**, with the exact
980-byte extent: its final call passed the real slot index in s7, while native
F0F44 receives it in s8. Workbench diagnosis confirmed the single register
allocation residual.

The only closing change is the existing F0F44 mode comparison's literal spelling:
`D_8014A110 == 2U`. This pools the native constant-2 web and assigns the parameter
to s8. The global remains signed s32; no shared declaration was changed. Equality
with 2 has the same result for every s32 value with either literal. **The original
literal spelling is unknown**; this is a disclosed code-generation dependency,
not evidence that retail used that exact spelling. The signed-literal control
is rebuilt and recorded and restores the one-word F1930 mismatch.

Other bounded probes: early-return versus scoped mode guard, nested versus
continue-based list filtering, load/operand ordering, explicit status-byte value,
and a state pointer in F1210. No padding or otherwise unexplained declarations
were introduced. F0F44's remaining native frame deficit is left unresolved.

## Complete-body results, including failures

- F1930: 0/245 differ; ELF 980 bytes; native frame 24 reproduced.
- F207C: 0/421 differ; ELF 1,684 bytes, unchanged earlier match.
- F2718: 0/92 differ; ELF 368 bytes, unchanged accepted body.
- F0F44: 68/116 differ; ELF 468 bytes versus native 464; frame 56 versus 72.
- F1210: 399/454 differ; ELF 1,788 bytes versus native 1,816; frame 48 versus 72.
  Its emitted jump table does not reproduce native destination offsets and is
  explicitly unverified/failing. No matching claim covers it.
- Billboard: 236/244 differ; ELF 960 bytes versus native 976; frame 88 versus 144.
  Its emitted jump-table references also remain unverified.

The receipt records the full details and every uncertainty. Context mismatches
are never counted as accepted bodies; only F1930 is in the new claims list.
The source-level switches follow the verified native case mappings; their failing
compiled offsets are not fixed by altering protected tables or the scorer.

A further control adds the genuine existing slot_state_setup reconstruction from
the menu-options research packet. F1930/F207C/F2718 stay exact; slot_state_setup
itself remains nonmatching (18/58 words), also recorded. No shaping blocker or
keep override is added for it.

## Verification and limitations

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_billboard_context_20261005/group
python3 cloud/work/frontier/dot_billboard_context_20261005/verify.py
python3 -m pytest -q tests/conveyor/test_billboard_save_input_packet.py
```

The host fixture passes **103 scenarios under UBSan**: row normalization and
navigation over all four rows and seven status values, device wraparound,
dialog priorities/toggling, overwrite confirmation, save failure/success,
cancel/finish, and the save helper changing the current selection. This tests
the new F1930 body only; it is not a comprehensive behavioral test of its
nonmatching surrounding helpers. Host callback stubs never enter a matching
build. The earlier F207C empty-name-finish caveat still applies to that unchanged
source and is documented in its separate packet.

The verifier authenticates protected targets through the stock scorer, checks
whole ELF symbol extents and all relocated bytes, and repeats relocation through
`blob_group` with its independent binutils-backed reader. Only source and receipt
metadata are committed, never native bytes, raw assembly, objects or credentials.

Before any promotion: independent source review, the complete unchanged shadow
unit, group/image verification, and the normal full-ROM gate are still required.
