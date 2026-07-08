# Contract: relocation-blind score

Node-side, in `tools/conveyor/jobs/scoring.py`. Stdlib only. Runs wherever the true
score runs, on the same two object files, using the toolkit-bundled objdump.

## Signature

```python
def reloc_blind_score(target_o, cand_o) -> int
```

Returned for every compiled cell as `score_reloc_blind` (present iff
`compile == "ok"`, alongside the unchanged true `score`).

## Algorithm (normative — implement exactly this)

1. **Words**: run `objdump -d` on each object; for the `.text` section take the
   32-bit instruction words in address order (the hex word column). Call them
   `T[0..n)` and `C[0..m)`.
2. **Reloc sites**: run `objdump -r` on the **candidate** object; parse `.text`
   relocation entries into `(instruction_index, mask)` where
   `instruction_index = offset // 4` and:
   - `R_MIPS_HI16`, `R_MIPS_LO16` → mask `0xFFFF0000` (i.e. keep the high 16 bits:
     opcode/registers; zero the low 16-bit immediate field)
   - `R_MIPS_26` → mask `0xFC000000` (keep the 6-bit opcode; zero the 26-bit target)
   - any other relocation type → mask `0xFFFFFFFF` (keep whole word — do NOT
     silently widen normalization; unknown types are not normalized)
3. **Masking**: for each reloc site `(i, mask)`: `C[i] &= mask`, and if `i < n`
   also `T[i] &= mask`. The target object has no relocations of its own (assembled
   from raw ROM words); it is masked only at the candidate's sites.
4. **Score**: `difflib.SequenceMatcher(a=T_masked, b=C_masked)`; sum over opcodes:
   `equal` → 0; `replace` → `max(i2-i1, j2-j1)`; `delete` → `i2-i1`;
   `insert` → `j2-j1`. Return the sum.

## Invariants (tests MUST cover each)

- **I1** (FR-007): empty candidate reloc table ⇒ score is pure word-sequence diff;
  in particular `T == C` ⇒ 0. True score 0 ⇒ reloc-blind 0.
- **I2** (FR-006 sc.1): streams equal except in masked fields at reloc sites ⇒ 0.
  Fixture: the osCreateMesgQueue case — 12 words, sites at indices 0,1 (HI16) and
  2,3 (LO16), words differ only in low 16 bits there.
- **I3** (FR-006 sc.2): a difference in the *unmasked* bits of a reloc-site word
  (e.g. different destination register in a `lui` under HI16) ⇒ nonzero.
- **I4**: a difference in any non-reloc-site word ⇒ nonzero.
- **I5**: length mismatch (`n != m`) ⇒ nonzero (SequenceMatcher guarantees this).
- **I6**: unknown reloc type present ⇒ that word is compared unmasked (no
  normalization creep).

## Parsing notes (objdump specifics, verified against toolkit output)

- Disassembly lines: `   0:\t3c0e8003 \tlui\tt6,0x8003` — word is column 2;
  filter to the `.text` section (`Disassembly of section .text:` header) and skip
  `...` ellipsis lines and non-instruction lines.
- Reloc listing (`objdump -r`): section header `RELOCATION RECORDS FOR [.text]:`,
  entries `00000000 R_MIPS_HI16       __osThreadQueue`. Offsets are hex.
- Do not shell out twice per pair for the true score's benefit — the true score
  path is untouched; this function makes its own two objdump calls (they are cheap
  relative to compilation, and correctness beats micro-optimization here).

## Result schema delta

See data-model.md — one new key `score_reloc_blind` in each result cell; ingest
stores it in `matrix_entry.score_reloc_blind` (NULL when absent from old blobs).

## Explicit non-goals

- No normalization of branch offsets (the true score's permuter diff already has
  `ign_branch_targets`; reloc-blind operates on raw words where branch offsets are
  position-relative and SHOULD match when streams align).
- No `.data`/`.rodata`/`.bss` comparison (see research D4 scope note).
- No change to the true score, its scorer, or the vendored permuter.
