# Results-table callbacks: genuine-context source research

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Status: **NONMATCH; no accepted-coverage or runtime-safety claim**.

This packet reconstructs the full bodies of two results-table callbacks that
had no body in the frozen source-lead indexes inspected for this run:

| Target | Native extent | Standalone control | Published group |
| --- | --- | --- | --- |
| `func_80101904` | 0x80101904–0x80101D84, 1,152 bytes / 288 words | 288/288 differing, 11 excess nonzero words | **32/288 differing, 3 excess nonzero words** |
| `func_80101D84` | 0x80101D84–0x8010221C, 1,176 bytes / 294 words | 293/294 differing, 12 excess nonzero words | **36/294 differing, 3 excess nonzero words** |

The published member comparisons have no unresolved symbols, unverified
relocations or relocation errors. These are strict full-word counts, not a
relocation-blind score. They remain substantial nonmatches.

## Genuine source and context

- Both callbacks draw the complete three-column results table, including its
  header cells, colored player cells, clipped names and formatted value cells.
- The first uses a player-to-color mapping, a selected vehicle's signed score,
  and a sum of absolute values from the signed four-column result matrix.
- The second uses player-index colors and two signed fields from the native
  120-byte statistics records. It preserves the header-font change and the
  font reset after each occupied cell.
- `root.c` reconstructs the sole native direct caller, `func_8010221C`: menu
  heading, seven conditional options, selection color and mode dispatch. The
  switch cases were checked against the original target's data references.
- `slot.c` is the real `slot_state_setup` implementation already reconstructed
  for menu work. `rect.c` emits the real five graphics commands from
  `func_80100D5C`, using one block-local command pointer per command as normal
  graphics macros do. All five command blocks do real work.
- Small lock/font/unlock helpers model the native inline-wrapper sequence,
  including its previous-font return. No synthetic caller, dummy argument,
  pressure object, artificial volatile, assembly or optimization blocker is
  introduced.

With the real parent but opaque callees, the first score is 265/288 and the
second 262/294. Making both real callees visible is the important improvement.
Block-scoped real graphics commands restore the observed private rectangle
argument placement. Expressing the first callback's sum as addition of an
absolute value restores its native shared addition, rather than duplicating
addition/subtraction across two branch arms.

## Fixed, unproved local-capacity hypotheses

Only one capacity was selected for each meaningful local buffer:

- First callback: label scratch **212 bytes**, from native sp+64 up to the
  observed RGBA object at sp+276; formatted scratch **40 bytes**, from sp+304
  up to the pointer slot at sp+344.
- Second callback: label scratch **204 bytes**, from native sp+104 up to RGBA
  at sp+308; formatted scratch **40 bytes**, from sp+336 to the pointer at sp+376.

These boundaries do not independently prove the original declarations or the
maximum encoded-string length. Capacities were neither swept nor tuned, and
are **not established safe runtime bounds**. The 60 passed to the clipping
routine is its native argument; it is not asserted to be a byte bound. Source
comments retain this limitation. Resolve it before accepting a final layout.

The external source model uses observed 76-byte player rows, 952-byte vehicle
records, 120-byte statistics rows and four-byte signed matrix rows. Unknown
external record fields preserve those observed layouts; they are not local
stack padding. The two-pointer name lookup and the +20 character address are
retained. Valid native record indices, resource pointers and string encoding
are assumed. No arcade-source identity or runtime test is claimed.

## Residuals and verification limits

Both compiled ELF member bodies have the native byte lengths. Their frames
are still **392 versus native 360 bytes**, and **416 versus native 392 bytes**.
The remaining body differences are mostly local layout and store/scheduling
choices. No stack storage was shrunk to chase these offsets.

The canonical scorer additionally counts three nonzero return instructions
between each member's ELF body and the following named function. They are
empty residual helper bodies produced by this translation-unit organization.
They are included in the reported excess-word counts and are not waived.

All context functions are explicitly unclaimed. In particular, the parent is
kept as the external group entry, while the original itself has a private ABI;
it is 134/137 differing words with 16 excess words and two unverified own-data
references. The slot context is 18/58 with two excess words, and the rectangle
context is 37/63. Do not replace production owners with this context wholesale.

Workbench diagnosis was run on the first member before bounded source
refinement; it reported a structural/frame residual, including the 32-byte
frame discrepancy. Its missing-symbol warnings arise from the local raw
word target object and are not the strict relocation result above.

## Reproduce

With the repository's IDO 5.3 setup available:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_results_callbacks_101904_research_20261007
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`; canonical group pipeline with
`-Olimit 5000` and assembler `-r4300_mul`. `group.json` has empty claims.
Only focused compilation and target comparisons were run. No production,
scorer, protected target, lock or ROM files change. Independent checking,
acceptance, integration and merging remain separate.
