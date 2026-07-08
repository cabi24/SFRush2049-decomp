# Contract: reloc-aware target assembly + round-trip gate

Pi-local, in `tools/conveyor/pipeline/targets.py`. Stdlib + the system
`mips-linux-gnu-as` / `mips-linux-gnu-objdump` already in use.

## Region index

```python
def index_asm_regions(asm_dir=REPO / "asm/us") -> dict[int, Region]
```

- Parse each `*.s` once. A region starts at `glabel <name>` and ends at
  `endlabel <name>` (same name). Region key = vaddr of its first instruction
  line (second hex field of the `/* off vaddr word */` comment).
- Region payload: the raw text lines between the markers (instruction lines
  AND any interior label lines, e.g. `.L80006A40:`), plus the parsed ROM words
  (third hex field) for the gate.
- Lines that are not instruction or label lines (blank, directives) are kept
  verbatim inside the region — the assembler tolerates them; the gate judges
  the outcome.
- Files without `%hi(`/`%lo(` may still index (regions with zero symbol refs
  assemble to relocation-free objects — harmless, tier still `reloc_aware` if
  the gate passes; word-equality is then exact equality).

## Assembly

```python
def assemble_region(region, target_id, out_o) -> None   # raises on as failure
```

Emitted source, exactly:

```asm
.set noreorder
.set noat
.section .text
.globl <target_id>
<target_id>:
<region lines, stripped of the leading /* … */ comment on instruction lines>
```

Assembler invocation identical to `assemble_words`:
`mips-linux-gnu-as -march=vr4300 -mabi=32 -o <out_o> <src>`.

The assembled symbol is named `<target_id>` (matches the candidate function
name for name-paired corpus cells — same convention as raw-word targets today).

## Round-trip gate

```python
def gate_target(rom_words, new_o) -> tuple[bool, str | None]
```

1. `new_words = _parse_text_words(objdump -d new_o)`;
   `sites = _parse_relocs(objdump -r new_o)` — **import these from
   `tools/conveyor/jobs/scoring.py`; do not reimplement.**
2. Fail `("length_mismatch <n> != <m>")` if `len(new_words) != len(rom_words)`.
3. Fail `("word_mismatch@<i>")` at the first index where
   `_masked_diff(rom_words, new_words, sites) != 0` (report the first differing
   masked index; computing it via a simple zip after masking is fine — reuse the
   mask application, the diff for the reason string may be positional since
   lengths are equal).
4. Pass otherwise.

## populate() integration

Per static-population target, in order:

1. Look up the region by `address`. Missing → tier `raw_word`,
   `gate_reason='no_asm_region'`, raw-word object as today.
2. Assemble; on assembler error → tier `raw_word`,
   `gate_reason='assemble_error: <first stderr line>'`, raw-word object.
3. Gate; on fail → tier `raw_word`, `gate_reason=<reason>`, raw-word object.
4. On pass → the reloc-aware object is the target object; tier `reloc_aware`,
   `gate_reason=NULL`.
5. If the resulting `target_o_sha` differs from the stored one → supersession
   purge (see evidence-supersession.md) in the same transaction as the row
   update.

Dynamic-population targets skip 1–4 entirely (tier `raw_word`,
`gate_reason=NULL`).

## Determinism requirements

- Sorting: regions processed in deterministic order; assembler input text is a
  pure function of the region text + target_id.
- Two consecutive `populate` runs with unchanged `asm/`, symbol table, and
  images MUST produce byte-identical objects (assert in tests by extracting
  twice into temp stores).

## Test fixtures (unit, no assembler where avoidable)

- Region parsing: a synthetic `.s` with two glabel regions, interior `.L` label,
  and the osCreateMesgQueue excerpt from `asm/us/7600.s` (vaddr keying, word
  extraction).
- Gate logic: fixture word lists + reloc sites exercising pass, word_mismatch
  (unmasked-bit difference at a reloc site), length_mismatch.
- One `node-not-required` integration test that runs the real assembler on the
  osCreateMesgQueue region (the Pi has mips binutils; mark with a skipif on
  `shutil.which("mips-linux-gnu-as")` so CI-less environments skip cleanly) and
  asserts: 12 words, HI16 relocs at word indices 0,1, LO16 at 2,3, gate passes
  against the raw ROM words.
