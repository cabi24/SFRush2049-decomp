# Data Model: Relocation-Aware Target Assembly

Deltas against the 002 schema. All migrations additive + idempotent
(`PRAGMA table_info`-guarded), same pattern as 002.

## Extended table: `n64_target`

| Column | Type | Notes |
|---|---|---|
| `tier` | TEXT NOT NULL DEFAULT 'raw_word' | `'reloc_aware'` or `'raw_word'` |
| `gate_reason` | TEXT NULL | NULL when tier is `reloc_aware` or the target never attempted the gate (dynamic population); otherwise why the fallback happened: `no_asm_region`, `assemble_error: …`, `word_mismatch@<index>`, `length_mismatch <n> != <m>` |

Rules:

- `population` already distinguishes static vs game-code targets; only static
  targets attempt reloc-aware assembly in this feature.
- `target_o_sha` keeps its meaning (current comparison object); replacing it is
  the supersession trigger (below).
- Re-running extraction with unchanged inputs MUST leave `target_o_sha`, `tier`,
  and `gate_reason` unchanged for every target (FR-010 / SC-007).

## Extended table: `matrix_entry`

| Column | Type | Notes |
|---|---|---|
| `target_o_sha` | TEXT NULL | the exact target object this cell was scored against; NULL only for cells ingested from pre-003 result blobs |

Attribution flow: submit writes `target_o_sha` into each manifest cell's target
entry → node echoes it into the result cell → ingest stores it. Ingest MUST NOT
reject result blobs lacking the key (stores NULL).

Invariant (SC-006): for every row, `target_o_sha` is either NULL (legacy) or
equal to the `n64_target.target_o_sha` current at ingest-time; after any purge
(below), all surviving rows for a target satisfy
`matrix_entry.target_o_sha IN (current, NULL-from-pre-purge-era)` — and the purge
removes the NULL-era rows too, so post-003 the invariant is simply
"all rows match the current object".

## Supersession semantics (derived-state purge)

When `targets.populate` replaces a target's `target_o_sha` (any change of bytes,
including raw→reloc and future re-extractions):

1. In the same transaction: `DELETE FROM matrix_entry WHERE target_id = ?`
   (all toolkits, all origins — stale evidence is stale regardless).
2. `function_status` is NOT touched here; the next `corpus ingest-results` /
   `update_rankings` recomputes flags and pairings from surviving evidence
   (002's recompute rules already handle "evidence disappeared").
3. Result blobs and `work_unit` rows are never deleted (audit trail; FR-006);
   superseded target blobs become GC-eligible via the existing unreferenced-blob
   rule.
4. The purge count per run is printed and included in the coverage report.

Dedupe consequence: `_existing_cells` (matrix and corpus submit) sees no rows for
the changed target ⇒ reschedules exactly its pairings; unchanged targets keep
their rows ⇒ full cache behavior (SC-007). The result *blob* cache is untouched
and correct by construction: a changed target .o changes `input_shas` ⇒ new
`manifest_sha` ⇒ fresh compute; unchanged inputs ⇒ `cached_result`.

## Manifest / result cell schema delta (node-visible)

Manifest cell target entry gains one key (submit-side, all three producers:
matrix, corpus, lock):

```json
{"target_id": "osCreateMesgQueue", "file": "osCreateMesgQueue.o",
 "target_o_sha": "<sha256 of the .o bytes>"}
```

Result cell echoes it:

```json
{"candidate_id": "…", "flagset": "…", "target_id": "…",
 "score": 0, "score_reloc_blind": 0, "compile": "ok",
 "target_o_sha": "<echoed verbatim>"}
```

Echo rule: present iff present in the manifest entry (older manifests replayed
from cache produce cells without it → ingested as NULL). This is a node-side
change and ships in the 003 toolkit rebuild.

## Gate record (no new table)

The gate result lives on `n64_target` (`tier`, `gate_reason`) — one current
answer per target, recomputed each extraction. Historical gate outcomes are not
retained (they are a pure function of asm + symbol table + assembler, all in
git; re-derivable at any commit).

## Coverage report shape (FR-009)

```text
target tiers: reloc_aware=<n> raw_word_static=<m> raw_word_dynamic=<k>
gate fallbacks (static): <m> — top reasons: assemble_error=<a> word_mismatch=<b> …
superseded this run: <p> targets, <q> matrix_entry rows purged
flagged reloc_only_diff: <before> -> <after>   (populated after first re-score)
```
