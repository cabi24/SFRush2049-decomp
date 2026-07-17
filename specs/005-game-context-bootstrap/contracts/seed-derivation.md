# Contract: Seed Derivation & Histogram for Extracted Targets

Consumers: `pipeline/disasm.py` (new), `pipeline/autodecomp.py` (widened),
downstream farm/report tooling (unchanged).

## Derived disassembly format (normative)

For an extracted target with repaired extent, `disasm.py` MUST emit a file
m2c parses without error, in exactly the dry-run-validated shape
(research/m2c-dryrun.md §"Disassembly format that worked"):

```asm
glabel <target_id>
.L800FD464:
    addiu  $sp,$sp,-0x68
.L800FD468:
    sw     $ra,0x24($sp)
    ...
```

Normalization rules (from GNU objdump `-m mips:4300 -EB
--adjust-vma=0x80086A50` output):

1. Strip the address and opcode-word columns.
2. Prefix every GPR with `$`.
3. Label every instruction `.L<vaddr>:`; rewrite all branch operands
   (`b*`, `bc1f[l]`, `bc1t[l]`) to those labels.
4. Rewrite `jal`/`j` numeric targets: if the vaddr is an `n64_target`
   address → that `target_id`; else `func_<vaddr:08x>`.
5. Data-reference symbolization (D3): rewrite to `%hi(name)`/`%lo(name)`
   when the effective address is in the committed game symbol table, for
   **both** address-consumption idioms *(amended 2026-07-17 at the T019
   review gate — the original contract named only idiom (a), which left 4
   of 9 table symbols silently unreachable; see research/t019-stall.md
   root cause 1)*:
   (a) `lui $r, hi` followed by a memory access `imm($r)` — rewrite the
       `lui` operand to `%hi(name)` and the access offset to `%lo(name)`;
   (b) `lui $r, hi` followed by pointer formation `addiu $d, $r, imm` —
       rewrite to `%hi(name)` / `addiu $d, $r, %lo(name)`.
   Register state rules: an `addiu $d,$r,imm` with `$d == $r` updates the
   tracked value for `$r`; with `$d != $r` it propagates the formed address
   to `$d` while `$r` keeps its `lui` value. Any other instruction that
   writes a tracked register **invalidates** its entry (this closes the
   stale-`lui` false-pairing mechanism the T019 report identified as
   possible-but-not-yet-observed; a regression test must cover it). One
   `lui` may legally serve multiple accesses/symbols only when every
   emitted `%hi` matches its paired `%lo`'s symbol; when consumers resolve
   to different symbols, leave the `lui` and the conflicting consumers
   numeric rather than emit a mismatched pair.
   All other data refs stay numeric — numeric absolute addresses are
   score-correct against raw-word target objects.

   The game symbol table additionally admits the addresses the T019 report
   traced with evidence (all from research/cluster-data-refs.md):
   `0x80114658`, `0x8017A4E0`, `0x8017A4E4`, `0x80140BF0`, `0x8002EB64`,
   `0x8014A108`, `0x8014A110` — each entry cites its access-pattern
   evidence, and each gets a typed extern in `include/game_types.h` with
   the same evidence comment (observed-layout provenance; arcade file:line
   where a plausible arcade counterpart exists).

Cache: `build/m2c_asm/<target_id>.s`, invalidated by (extent, image sha,
symbol-table sha) change. Derivation is deterministic.

## Seeding path (behavioral)

`autodecomp seed|one|clusters` gain:

- `--population {static,extracted}` (default `static` — existing behavior
  byte-for-byte preserved; SC-005 gate).
- `--targets <id,...|@file>`; `tools/conveyor/clusters/game_loop.txt` is the
  committed cluster list. Unresolvable names abort with the name.
- Asm resolver: static → `_asm_index()` (unchanged); extracted →
  `disasm.py` cache. Targets with `gate_reason` starting `extent_conflict`
  are refused for seeding.
- Context chain: `types.h` + `PR/*.h` + `m2c_types.h` + **`game_types.h`**
  (new, last), preprocessed by the existing `_context()` `cpp -P -nostdinc`
  flow. m2c stderr MUST be captured per target and preserved in diagnostics
  (never reduced to a generic error line).
- Flagset for extracted targets: `-g0 -O2 -mips2 -G 0 -non_shared` primary,
  `-O1` variant recorded as alternate. Submission, scoring, harvest, and
  status transitions reuse the static path unchanged (permuter_search jobs;
  no toolkit change).

## Histogram artifact (normative)

`autodecomp clusters --population extracted --limit 0` MUST produce
`build/m2c_histogram.json` + `build/m2c_histogram.md` with:

- **Coverage**: every `extracted` target with a target object appears in
  exactly one bucket: `compiled` | `blocked` | `decompiler_failure` |
  `no_disasm` | `extent_conflict`. Bucket counts sum to population size.
- **Blocked detail**: per target, the named undefined symbols (existing
  tokenizer); per blocker, count + blocked-function names + arcade hint
  (existing `_arcade_hint`).
- **Failure detail**: `decompiler_failure` entries carry the first line of
  m2c stderr; `no_disasm` entries carry the derivation failure reason
  (e.g. `scan_overrun`).
- **Determinism**: identical inputs (image, extents, context, symbol table)
  ⇒ identical JSON bytes modulo the timestamp field (SC-003).
- **Local-only**: the run performs no network calls and requires no nodes
  (compile probe = `cpp -P -nostdinc` + `mips-linux-gnu-gcc -fsyntax-only
  -std=gnu89`, both already Pi-resident).

## Promotion firewall (normative)

`pipeline/lock.py add` and `pipeline/promote.py run|batch` MUST resolve the
function's `n64_target.population` and reject `extracted` with:
`error: <target_id> is extracted-population — evidence-only (005/FR-010)`.
Covered by a unit test; SC-006 verified by inspecting `promotion_record`
after the feature's runs.
