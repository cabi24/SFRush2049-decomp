# Research: Corpus Candidates

All decisions below were validated against the live repository and pipeline state on
2026-07-08 (branch `002-corpus-candidates`); measurements cited are real, not
estimates.

## D1. First corpus root: decompals/ultralib (local clone)

- **Decision**: `reference/repos/ultralib` at commit
  `e24c836796df4bf520ff8b11a5c9d2cea3a66cbd` (shallow clone, already present) is the
  V1 corpus root, origin name `ultralib`.
- **Rationale**: The existing brace-matcher extractor pulls 686 functions from it
  unmodified; 85 exactly name-match the 1,131-target inventory. Sources are
  one-function-per-file (e.g. `src/os/createmesgqueue.c`), so reduced-TU compilation
  is nearly a no-op. Its `include/` tree resolves all its own `#include "..."` forms
  (`PR/...`, `PRinternal/...`) with a single include dir.
- **Alternatives considered**: decomp.me API — rejected for V1: Cloudflare
  bot-challenge blocks scripted access (verified 403 on 2026-07-08), no published
  dataset exists, and ultralib dominates it for library code. Game-decomp clones
  (sm64, mk64, perfect_dark) — deferred: same code at other SDK versions; the
  registration mechanism supports adding them with zero schema change.

## D2. Candidate identity and provenance

- **Decision**: corpus candidates use `candidate_id = "<origin>:<rel>:<name>"`
  (e.g. `ultralib:src/os/createmesgqueue.c:osCreateMesgQueue`). Arcade rows keep
  their existing 2-part ids untouched. Rows gain `origin TEXT NOT NULL DEFAULT
  'arcade'` and `provenance TEXT NOT NULL DEFAULT '{}'` (JSON: `repo_url`, `rel`,
  `commit`). Parsing rule everywhere: `name = candidate_id.rpartition(":")[2]`; the
  path part is resolved through the row's `origin` (arcade → the arcade tree, else
  `corpus_root.path`), never by re-parsing the id ad hoc.
- **Rationale**: keeps existing arcade ids (and all 281,933 scored cells) stable;
  the origin column, not string format, is authoritative; `rpartition` is already the
  safe parse for both forms.
- **Alternatives considered**: renaming the table / rewriting arcade ids — rejected,
  needless churn of live data; separate corpus_candidate table — rejected, results
  ingest and rankings would need dual-table joins for no benefit.

## D3. Corpus root registry and dirty-clone refusal

- **Decision**: new table `corpus_root(origin PK, path, repo_url, commit,
  include_dirs JSON, ingested_at)`. `corpus register` records it; `corpus ingest`
  re-reads `git -C <path> rev-parse HEAD` and `git -C <path> status --porcelain`
  and refuses (exit 1, explicit message) if the clone is missing, dirty, or at a
  different commit than registered — override with `--allow-dirty` which then
  records the working-tree state as commit `"<sha>-dirty"`.
- **Rationale**: FR-003 — provenance must describe the actual bytes. `git` used only
  as a local subprocess; no network (FR-001).

## D4. Reloc-blind score: exact algorithm (the load-bearing decision)

- **Decision**: computed node-side in `jobs/scoring.py` from the two object files,
  using the toolkit objdump, stdlib only:
  1. Disassemble both `.text` sections (`objdump -d`) and take the 32-bit
     instruction words in order (the hex word column, not the mnemonic text).
  2. Read the candidate's relocation table (`objdump -r`): entries of type
     `R_MIPS_HI16` and `R_MIPS_LO16` mask the low 16 bits of the word at that
     offset; `R_MIPS_26` masks the low 26 bits. The target object (assembled raw
     ROM words) has no relocations; its words are masked at the **same instruction
     indices** as the candidate's reloc sites.
  3. `score_reloc_blind` = number of differing words under
     `difflib.SequenceMatcher` opcodes over the two masked word sequences
     (replace counts max(len) of the span, insert/delete count their span length).
     Equal masked sequences ⇒ 0.
- **Rationale**: masking by the candidate's actual relocation table normalizes
  exactly the fields the linker would patch — registers, opcodes, and non-reloc
  immediates stay significant (FR-006 acceptance scenario 2). Positional masking of
  the target is correct precisely when the instruction streams align, which is the
  only case where reloc-blind 0 is claimable. Word-level (not disasm-text-level)
  comparison avoids objdump formatting differences entirely.
- **Verified precedent**: osCreateMesgQueue compiled vs target on watchman
  (2026-07-08): 12 instructions, identical except words 0–3 (`lui/lui/addiu/addiu`
  pair-patched by HI16/LO16 relocs for `__osThreadQueue`). Under this algorithm:
  masked equal ⇒ score 0. strlen/guMtxIdentF have empty reloc tables ⇒ masking is a
  no-op ⇒ reloc-blind equals word-equality ⇒ 0 (FR-007 invariant holds).
- **Alternatives considered**: patching the vendored decomp-permuter scorer —
  rejected (invasive fork of upstream, harder to reason about); assembling targets
  with real relocations — that is the separate future feature this one must not
  block on; normalizing by mnemonic class (all `lui` immediates etc.) — rejected,
  it erases genuine constant differences (loses FR-006 scenario 2).
- **Scope note**: reloc-blind compares `.text` only. A candidate with differing
  `.data`/`.rodata` could mask a real difference — acceptable because the true score
  still covers it and `reloc_only_diff` is a review state, never a promotion state
  (FR-010/SC-007).

## D5. Name-paired job construction (reuse the lock's mechanism)

- **Decision**: `corpus submit` builds one `compile_score` cell per (paired target ×
  corpus candidate × flagset): source = reduced TU (all other function definitions
  stripped) + the corpus repo's headers resolved transitively, bundled under
  `inputs/` with `include_dirs` from the root's registration (ultralib:
  `["include"]`); target .o bytes from the existing inventory blob. Priority 50
  (above bulk matrix's 100). The reduced-TU and header-resolution helpers move from
  `pipeline/lock.py` to `seeds/context.py` (lock imports them back; helper accepts
  the repo root and include dirs as parameters instead of assuming the game repo).
- **Rationale**: this exact mechanism was proven end-to-end by `pipeline.lock
  verify` on 2026-07-08 (strlen and guMtxIdentF verified at score 0 through the
  pool). Reusing `compile_score` means dedupe, caching, ingest, and the
  failure-clustering report all apply to corpus cells for free (FR-012, edge cases).
- **Alternatives considered**: a new job type — rejected, nothing about the work
  differs; compiling whole ultralib TUs without reduction — unnecessary (they're
  single-function) but reduction stays for uniformity and for future multi-function
  roots.

## D6. Flag sets

- **Decision**: start with the two confirmed sets (`-g0 -O1 -mips2 -G 0 -non_shared`,
  `-g0 -O2 -mips2 -G 0 -non_shared`), no per-root extra defines for V1 (ultralib
  code paths guarded by `_DEBUG`/`BBPLAYER` default off). `corpus submit
  --flagsets ...` overrides, same as matrix.
- **Rationale**: matches docs/COMPILER_SETTINGS.md evidence (os → -O1, gu/libc →
  -O2). The per-TU flag sweep remains the discovery tool if neither set matches.

## D7. Schema migration approach

- **Decision**: idempotent, in `coordinator/db.py` alongside the existing
  `CREATE TABLE IF NOT EXISTS` block: `PRAGMA table_info` checks guarding
  `ALTER TABLE arcade_candidate ADD COLUMN origin/provenance` and
  `ALTER TABLE matrix_entry ADD COLUMN score_reloc_blind INTEGER` (NULL for legacy
  rows), plus `CREATE TABLE IF NOT EXISTS corpus_root`. No data rewrite; legacy rows
  read as origin `'arcade'` via the column default.
- **Rationale**: single-writer SQLite, no migration framework in this project;
  nullable new column keeps old result blobs re-ingestable.

## D8. Artifacts, flag, and upgrade path

- **Decision**: after `corpus` results ingest, for each target whose best cell has
  `score_reloc_blind == 0 and score > 0`: write `corpus_match.c` (candidate source +
  doc header: origin, rel path, commit, flagset, both scores, job id) into the
  target's existing `work/` directory (located by the same rglob rule
  `verify_promote` uses; fallback `work/corpus/<target_id>/`), and set
  `function_status.human_flag = 'reloc_only_diff'` unless the target is already
  past `unmatched`/has an override. True score-0 cells take the existing
  `update_rankings` path unchanged (candidate_identified → … → lock eligibility).
  No new state-machine states.
- **Rationale**: `human_flag` is the existing free-text advisory column
  (`no_ancestry` precedent); FR-011's "no bespoke bookkeeping" holds because the
  flag is recomputed from `matrix_entry` on every ingest — when the future
  reloc-assembly feature ships new target objects, new cells either hit true 0
  (flag superseded by promotion path) or don't (flag clears/stays by the same rule).

## D9. Toolkit rebuild batching

- **Decision**: one rebuild ships three node-side changes together: reloc-blind
  scoring (this feature), the stderr head-capture fix (already committed,
  `8b51c69`), and any shim edits ready at that time. Pin the new sha; run the corpus
  cycle under it. The arcade matrix re-fills at its next submit (already planned for
  shim-coverage iteration) — not this feature's responsibility, but quickstart
  documents the order.
- **Rationale**: toolkit sha changes invalidate the comparison set by design
  (001 FR-005); batching avoids paying that cost twice in one week.
