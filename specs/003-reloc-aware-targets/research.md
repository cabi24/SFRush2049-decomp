# Research: Relocation-Aware Target Assembly

Grounded against the live repo/pipeline on 2026-07-08. Facts below were checked,
not assumed.

## D1. Source of truth: splat asm regions, matched by address

- **Decision**: index every `asm/us/*.s` file once, parsing `glabel <name>` …
  `endlabel <name>` regions; each region's identity is the vaddr of its first
  instruction (the second hex field of splat's `/* off vaddr word */` comment).
  Targets are matched to regions by `n64_target.address` — **never by name**
  (asm glabels are `func_80006A00` style; target_ids are renamed symbols like
  `osCreateMesgQueue`).
- **Verified**: `asm/us/7600.s` region for 0x80006A00 is osCreateMesgQueue,
  carries `%hi(D_8002C3D0)`/`%lo(D_8002C3D0)`; 56 of 88 files contain `%hi`.
- **Alternatives**: re-running splat to regenerate asm with current symbol names —
  rejected: bigger blast radius, and address matching makes names irrelevant.

## D2. Assembling a region

- **Decision**: emit a standalone `.s`: preamble
  `.set noreorder` / `.set noat` / `.section .text` / `.globl <target_id>` /
  `<target_id>:`, then the region's instruction lines verbatim minus the leading
  `/* … */` comments, keeping any interior local labels (`.L…:`) splat emitted.
  Assemble with the same `mips-linux-gnu-as -march=vr4300 -mabi=32` invocation
  `assemble_words` already uses. Undefined symbols (`D_*`, `func_*`, renamed OS
  symbols, tail-jump targets) are exactly what we want: the assembler emits
  R_MIPS_HI16/LO16/26 relocations for them.
- **Rationale**: the mnemonic text already exists and already encodes the symbol
  structure; we add no disassembler of our own. The gate (D3) catches any case
  where splat's text doesn't round-trip.
- **Alternatives**: synthesizing `%hi/%lo` pairs from raw words (needed later for
  the 752 dynamic targets) — out of scope by spec; patching relocations into the
  raw-word object by hand — reimplements the assembler badly.

## D3. Round-trip gate (the safety property)

- **Decision**: after assembling, disassemble the new object (`objdump -d`, word
  column) and read its reloc sites (`objdump -r`) — **reusing
  `jobs/scoring._parse_text_words`, `_parse_relocs`, `_masked_diff` verbatim**
  (importable on the Pi; `test_reloc_blind.py` already imports them). Gate:
  `_masked_diff(rom_words, new_words, sites) == 0` **and**
  `len(rom_words) == len(new_words)`. On pass: the reloc-aware object becomes
  `target_o_sha`, tier `reloc_aware`. On any failure (assembler error, word
  mismatch, length mismatch, missing region): keep/rebuild the raw-word object,
  tier `raw_word`, `gate_reason` recorded. Gate failures never abort extraction.
- **Rationale**: this is 002's mask logic pointed at a new problem — "equal
  everywhere the relocations don't own" is precisely the no-distortion property.
  Length equality is required because masking is positional.
- **Note**: `%lo(sym+addend)` forms assemble with the addend in the masked field;
  differences there are invisible to the gate *and* to the scorer's reloc
  handling, which is correct — the linker owns those bits.

## D4. Evidence attribution: manifest→result echo

- **Decision**: `matrix submit`/`corpus submit`/`lock verify` already put the
  target file into each cell's `targets` list; they now also set
  `"target_o_sha"` on each target entry. Node-side `compile_score.py` echoes it
  into every result cell verbatim (one line; absent key echoes as absent).
  `matrix ingest` stores it in the new `matrix_entry.target_o_sha` column (NULL
  for legacy blobs).
- **Rationale**: exact attribution with zero races — the value travels with the
  work itself, not looked up at ingest time. Manifest change ⇒ new manifest_sha ⇒
  correct cache behavior falls out for free (a changed target .o already changed
  `input_shas`, so dedupe was never going to reuse the old *result*; what it
  wrongly reused was the *matrix_entry row* — fixed by D5).
- **Alternatives**: stamping current `n64_target.target_o_sha` at ingest time —
  rejected (race between submit and ingest across a re-extraction).

## D5. Supersession: purge derived rows on target change

- **Decision**: when `targets.populate` replaces a target's `target_o_sha`, it
  deletes that target's `matrix_entry` rows in the same transaction (all
  toolkits) and prints the purge count. Result blobs and work_unit rows are
  untouched — the content-addressed audit trail survives (FR-006). Submit-side
  `_existing_cells` then naturally reschedules; rankings/reports/flag-recompute
  see only current-object evidence because stale rows no longer exist.
- **Rationale**: `matrix_entry` is derived state (the 002 flag recompute already
  treats it that way); purging is the only approach that keeps the existing
  PRIMARY KEY `(target_id, candidate_id, flagset, toolkit_sha)` and
  `INSERT OR IGNORE` semantics honest without a schema rewrite. The
  `target_o_sha` column (D4) makes attribution verifiable (SC-006) and guards
  against any future path that forgets to purge.
- **Alternatives**: widening the PK with target_o_sha — SQLite can't alter PKs
  without table rebuild of a 281k-row table, and rankings would then need
  "current object only" filters everywhere (easy to miss one); keeping stale rows
  with a `superseded` flag — same filter-everywhere hazard.

## D6. Scorer behavior with relocations on both sides

- **Decision**: no scorer changes. The vendored permuter Scorer already objdumps
  with `-drz` (relocations interleaved) and was designed for the standard decomp
  workflow where the target .o is assembled from asm *with* relocations; the
  reloc-blind score reads sites from the candidate only and is unaffected.
- **Verification hook (in tasks)**: before trusting it at scale, score the
  osCreateMesgQueue pairing against a reloc-aware target on watchman and require
  true score 0 — if the permuter needs different symbol-name handling (target
  glabel name vs candidate function name already match by construction: we name
  the assembled symbol `<target_id>`), this surfaces immediately on one function.

## D7. Blob/GC behavior

- **Decision**: superseded raw-word target blobs simply stop being referenced by
  `n64_target.target_o_sha`; the existing `cli gc` reclaims unreferenced blobs
  after its age window. No new GC logic.

## D8. Smoke fixture and lock interplay

- **Decision**: `cli smoke` keeps assembling its own raw-word strlen target
  (self-contained proof of the fabric, independent of inventory state).
  `lock verify` reads `n64_target.target_o_sha` at run time, so it exercises the
  new reloc-aware objects automatically — that is SC-004's mechanism, not extra
  code.
