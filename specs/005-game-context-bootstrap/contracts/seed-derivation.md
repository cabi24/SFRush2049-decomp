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
   possible-but-not-yet-observed; a regression test must cover it). A `%hi`/`%lo` pair must always name the same symbol — m2c pairs by the
   `%hi` symbol and **silently drops** a differently-named `%lo`'s
   semantics (verified: `lui %hi(symA)` + `lw %lo(symB)` decompiles to
   `symA * 2`, losing symB). When one `lui` serves consumers that resolve
   to different symbols (or a mix of symbols and numerics), the derivation
   **re-emits a synthetic `lui`** immediately before each consumer whose
   required binding differs from the register's current one: symbolized
   consumers get `lui $r,%hi(their-symbol)`, numeric consumers get the
   original numeric `lui` restored. This is safe because derived asm is
   m2c seed input only — never assembled, never scored — and each
   consumer's effective address is unchanged; synthetic instructions carry
   no `.L` label. (Verified: the duplicated-lui form decompiles to
   `symA + symB`, correct dataflow.) *(Amended 2026-07-17 second gate:
   the first amendment's mismatched-pair rule was insufficient — a numeric
   co-consumer of a renamed `%hi` computes `symbol + delta`, which m2c
   renders as an invalid `sym.unk-NNNN` member expression, observed as
   `game_loop_tick.unk-1718` in game_loop; and leaving whole groups
   numeric would strand typed adjacent globals like the
   `countdown_state`/`countdown_object` pair that share one `lui`.)*
   All other data refs stay numeric — numeric absolute addresses are
   score-correct against raw-word target objects.

   The game symbol table additionally admits the addresses the T019 report
   traced with evidence (all from research/cluster-data-refs.md):
   `0x80114658`, `0x8017A4E0`, `0x8017A4E4`, `0x80140BF0`, `0x8002EB64`,
   `0x8014A108`, `0x8014A110` — each entry cites its access-pattern
   evidence, and each gets a typed extern in `include/game_types.h` with
   the same evidence comment (observed-layout provenance; arcade file:line
   where a plausible arcade counterpart exists).

   *(Amended 2026-07-17 second gate — table coverage policy.)* The table
   covers **every global the cluster survey enumerates**
   (research/cluster-data-refs.md per-function tables), not a curated
   subset: m2c types an unnamed formed pointer as `void *`, so any
   surveyed-but-untabled global a cluster function touches produces an
   uncompilable `*(void *)0xADDR` expression. Naming policy: entries with
   established semantic evidence keep semantic names; all others use the
   decomp-standard placeholder `D_<ADDR>` (e.g. `D_80035471`) — zero
   naming judgment, renamed when matched. Types come from the survey's
   access width (byte→`u8`, half→`u16`, word→`s32`/`u32`, float→`f32`);
   surveyed multi-offset bases become padded structs with fields only at
   surveyed offsets. This remains FR-004-bounded: an address enters the
   table only with cluster evidence — a row in the survey tables, or
   (third amendment, 2026-07-17) a direct citation of the derived cluster
   assembly that forms it (`<target>.s` + instruction address), for
   accesses the original survey pass missed.

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
