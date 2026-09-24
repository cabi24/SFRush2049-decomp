# Contract: Image Layout, TU Generation & the Byte-Identity Gate

Consumers: `pipeline/blob_layout.py`, `pipeline/blob_tu.py`,
`pipeline/blob_build.py`.

Image constants: base `0x80086A50`, length 647,072, end `0x801249F0`,
authority `build/game_code.bin` (sha256 recorded in the map).

## Layout map (normative)

1. **Population**: gate-passed extracted rows only — `population='extracted'`,
   `address` and `insn_count` present, `gate_reason` neither
   `extent_conflict*` nor `scan_overrun*`. 912 rows at baseline. A row whose
   extent leaves the image bounds is excluded and reported.
2. **Overlap**: extents MUST NOT overlap after this filter. An overlap is a
   hard error naming both targets — it means the 007 suffix rule missed a
   case, and guessing would corrupt the image.
3. **Opaque runs**: every byte not inside an extent becomes an `opaque` run
   with its vram range and length. Interior gaps and the tail are the same
   kind of thing; no attempt is made to classify them.
4. **Completeness**: `sum(function bytes) + sum(opaque bytes) == 647072`, and
   regions in strictly ascending vram order with no gaps. This is checked, not
   assumed.
5. **Regions (TUs)**: a region is a maximal run of entries split when an
   opaque run of ≥ `SPLIT_GAP` bytes (default 4096) is crossed or the region
   reaches `MAX_FUNCS` (default 60). Each region carries `vaddr_start`,
   `vaddr_end`, its ordered entries, and `flagset` (default
   `-g0 -O2 -mips2 -G 0 -non_shared`).
6. **Determinism**: the emitted JSON is sorted and stable; two derivations
   from identical inputs are byte-identical apart from nothing — there is no
   timestamp in the map.

## Translation units (normative)

7. **Function entries** are emitted as `GLOBAL_ASM` passthroughs over the
   per-function asm already used for scoring, so a TU with no splices is
   byte-identical to the image by construction.
8. **Opaque entries** are emitted as `.incbin "build/game_code.bin", <off>,
   <len>` — offsets relative to the image file, never to the ROM.
9. **Placement**: `blob.ld` places the TUs contiguously from `0x80086A50` in
   map order, with no linker-inserted padding between regions. Any alignment
   directive that could insert padding is forbidden.
10. **Regeneration** with an unchanged map and unchanged splice state produces
    byte-identical files — the generator is safe to re-run.

## The gate (normative)

11. **Procedure**: compile each TU with IDO through asm-processor at the
    region's flagset, link with `blob.ld`, `objcopy -O binary` the loaded
    range `[0x80086A50, 0x801249F0)`, sha256 the result.
12. **Pass** iff that sha256 equals `build/game_code.bin`'s. There is no
    tolerance, no masked comparison, no "differs only in padding" allowance.
13. **Failure** MUST report: the first differing byte offset, its vram, the
    region that owns it, the function that owns it (or `opaque`), and the
    expected vs actual words. The build exits non-zero.
14. **Splice** is: replace exactly one passthrough with a C body, run the
    gate, commit on pass, restore the previous file content on fail. A failed
    splice leaves the tree exactly as it was — verified by hashing the TU
    before and after.
15. **Provenance**: a committed splice records target id, source sha256,
    flagset, toolkit sha, score (which MUST be 0) and date in
    `blob_matched.lock.json`. A body whose source hash no longer matches its
    lock entry fails the check command, mirroring `matched.lock.json`.

## Firewall (normative)

16. Nothing in this feature writes to `src/rom/`, `matched.lock.json`,
    `promotion_record`, or the cartridge build. Image coverage and ROM
    coverage are separate numbers and MUST be reported separately; any output
    stating image coverage MUST also state that the cartridge still embeds the
    original compressed stream (005 FR-010 is unchanged).

## Acceptance oracles

- All-passthrough build: image sha256 equals `build/game_code.bin`'s.
- Map completeness: assigned bytes sum to 647,072 with zero overlaps.
- Determinism: two derivations byte-identical; two generations byte-identical.
- Corruption drill: altering one instruction in a spliced body fails the gate
  and names the correct offset and function.
- Revert: after reverting a splice, the image sha256 returns to its prior
  value.
- Refusals are reported with reasons rather than silently skipped: a matched
  function that cannot be spliced appears in the output with its cause.
