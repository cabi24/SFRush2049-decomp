# Image-A scrolling settings renderer: complete NONMATCH research

Base: `3bec9ce30` (current master at selection). Target `func_803A5358`,
`[0x803A5358,0x803A5658)`, 768 bytes / 192 words. No earlier source body was
found in the current tree; adjacent A5658 belongs to the already published
#335 and is untouched.

## Result

The complete new C reconstructs the heading, 13 wrapped rows from a 21-setting
list, labels and selectable values, the signed-byte numeric special case,
selected-row font/color/shadow choices, row stepping, and callback return.
It has **58/192 differing words**, zero nonzero excess words, unresolved
symbols, unverified references or owned-data errors. A standalone single is
not submitted to `cloud/matches`: this remains NONMATCH research with empty
claims, **zero new exact bytes and zero accepted image/ROM bytes**.

The first full natural draft differed at 128/192 and emitted 190 instructions.
The value-table label offset must be treated as unsigned before the pointer
addition to reproduce the 192-instruction geometry. This source-level type
hypothesis is explicit; it is not a recovered original declaration. The
verifier recompiles the signed-offset control, which retains the 128-word gap.
O2 and O3 standalone and a one-member kept O3 group give the same best body.

## Traced stopping point

The stock ugen tree/listing and instrumented uopt trace were inspected in
private scratch. Instrumented versus stock optimized Ucode and symbol table
were byte-identical. Every colored pool register agrees with native. Workbench
first diagnosed a structural two-instruction gap; the unsigned offset closes
that gap. The remaining facts are:

- Native frame is 136 bytes; candidate is 128. Text starts at native sp+112,
  candidate sp+104. Save-register homes already agree.
- At the numeric formatter, native passes the existing carried text pointer;
  uopt copy propagation instead supplies the local-array address again. The
  subsequent ugen temporary ring is one position behind, accounting for the
  broad but regular register-only residual.
- Source-order, pointer-expression, genuine callback-parameter reuse and
  harmless conditional-form probes did not recover the native pointer web.
  No speculative pressure locals, altered helper implementations, false
  return types, fake callers, compiler/object patches or accepted-code changes
  are retained. No blind mass sweep was run.

A future attempt should trace this specific local-address copy-propagation /
web coalescing decision before trying another source form. An allocation-only
forced-color oracle cannot fix an address already rematerialized in optimized
Ucode. The 8-byte frame residual should be addressed only after that source
structure is settled. Raw compiler listings, target words and binaries are
not part of this packet.

## Buffer and semantic limits

The 24-byte buffer capacity was selected for the fresh reconstruction from
native stack spacing and remained unchanged during the retained diagnosis.
Its only write is the image-authenticated `%d` format of the native signed-byte
setting. All 256 signed-byte values require at most five bytes including NUL.
This bound uses the existing integer-formatter contract; the formatter is not
recompiled or behaviorally replayed here. No localized string is copied into
this buffer. Original array size remains unknown.

The partial language view preserves all observed field offsets; selected
setting must be 0..20, value selectors and language tables must be backed,
and strings must be terminated. Original names, complete object layouts,
arcade ancestry and original source grouping are not established. The sole
ordinary callback argument is genuinely unused and retained; its original
pointed-to type is unknown. External helpers are not inlined or changed.

## Reproduce

Configure stock IDO 5.3 and GNU MIPS binutils, then run:

```
python3 cloud/work/frontier/dot_runtime_a_settings_scroll_20261012/verify.py --deep
python3 -m pytest tests/cloud/test_runtime_a_settings_scroll.py -q
```

The input-bound receipt checks the selected target against the authenticated
complete image, full STT_FUNC extent, all 29 relocations, no emitted data or
nonzero suffix, stock compiler identities and the signed-index control. GNU
links the original candidate object independently at its real address and
reproduces the same 58-word full-extent residual. Tests compile only the
candidate and its one causal control. No protected inputs, scorer, installed
source, accepted locks or ROM artifacts change.
