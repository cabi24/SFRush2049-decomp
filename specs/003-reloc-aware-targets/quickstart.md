# Quickstart: Relocation-Aware Target Assembly end-to-end

Run from the repo root on the Pi. Doubles as the acceptance run (SC-001..SC-007).
Order is load-bearing: Pi code → toolkit → extraction (purge) → re-score → gates.
Record measured numbers into this file at the marked points.

## 0. Preconditions

```bash
python3 -m pytest tests/conveyor -m "not node_required" -q     # green before starting
python3 -m tools.conveyor.cli nodes                            # watchman attached
python3 -m tools.conveyor.pipeline.lock check                  # all 12 intact
which mips-linux-gnu-as mips-linux-gnu-objdump                 # both present (Pi)
python3 -c "import sqlite3,pathlib;c=sqlite3.connect(str(pathlib.Path.home()/'.conveyor/conveyor.db'));print(c.execute(\"SELECT COUNT(*) FROM function_status WHERE human_flag='reloc_only_diff'\").fetchone()[0], 'flagged before')"
```

Expected: 19 flagged before.

## 1. Toolkit rebuild (ships the target_o_sha echo)

Per 002 quickstart §1 on watchman (rsync → build_toolkit → publish-toolkit), then:

```bash
python3 -m tools.conveyor.cli smoke     # SMOKE PASS, score 0 (SC-005)
```

> **MEASURED (2026-07-08)**: toolkit sha
> `1e21f523122ef9217aa757221c63a3cd1845965b841fcfe26ed53c51ecc79b01`
> (first rebuild `10cc7e73…` shipped ARM IDO binaries — an rsync from the Pi had
> clobbered watchman's x86_64 `ido-static-recomp/build/out`; restored from the
> cached `b613fc5d` toolkit's `ido/` and rebuilt). smoke: **SMOKE PASS**,
> `src/libc/string.c:strlen @ -O2 compile=ok score=0` (SC-005 ✓).

## 2. Re-extraction with the gate

```bash
python3 -m tools.conveyor.pipeline.matrix extract
```

Expected output (new lines from this feature):
`target tiers: reloc_aware=<n≥150> raw_word_static=<m> raw_word_dynamic=<k>` (SC-001),
`gate fallbacks: … top reasons …`, `superseded: <p> targets, <q> rows purged`.

Determinism check (SC-007): run extract again — `superseded: 0 targets, 0 rows`,
tier counts identical.

> **MEASURED (2026-07-08)**: tiers `reloc_aware=178 raw_word_static=68
> raw_word_dynamic=885` (SC-001 ✓, 178 ≥ 150). fallbacks (static, 68): `assemble_error=39
> no_asm_region=18 length_mismatch=10 word_mismatch=1` — the 39 assemble_errors are
> FP-register / 64-bit-FP libm+gu functions needing splat's `macro.inc` env the
> minimal preamble omits; the 10 length_mismatches are libgcc 64-bit di3 helpers
> (`-mabi=32` expands `sd`/`ld` to two ops); all coherent, safe raw-word fallbacks,
> none acceptance targets. First real extraction superseded `202 targets, 63260
> evidence rows purged` (22% — under the 40% guard). rerun: `superseded: 0 targets,
> 0 rows` (SC-007 ✓) — after fixing a latent inventory bug (12 game-code functions
> share a target_id at different addresses; `load_work_inventory` deduped only by
> address, so both entries ping-ponged the same row every run; now also deduped by
> target_id, lowest address wins). osCreateMesgQueue object: `reloc_aware`, 2×HI16
> + 2×LO16 vs `D_8002C3D0` ✓.

Spot-check one target's object carries relocations:

```bash
python3 - <<'PY'
import sqlite3, pathlib, subprocess
c = sqlite3.connect(str(pathlib.Path.home()/'.conveyor/conveyor.db')); c.row_factory = sqlite3.Row
r = c.execute("SELECT target_o_sha, tier FROM n64_target WHERE target_id='osCreateMesgQueue'").fetchone()
print(r["tier"])
blob = pathlib.Path.home()/f".conveyor/blobs/{r['target_o_sha']}"
print(subprocess.run(["mips-linux-gnu-objdump","-r",str(blob)],capture_output=True,text=True).stdout)
PY
```

Expected: `reloc_aware`; R_MIPS_HI16 ×2 + R_MIPS_LO16 ×2 against one symbol.

## 3. Re-score the corpus pairings

```bash
python3 -m tools.conveyor.pipeline.corpus submit          # purged targets = fresh work
python3 -m tools.conveyor.pipeline.corpus ingest-results
python3 -m tools.conveyor.pipeline.corpus report
```

Expected (SC-002, SC-003):

- `osCreateMesgQueue` per-target line: **true=0** reloc_blind=0 → normal
  promotion path, no reloc_only_diff flag.
- Flag summary: reloc_only_diff dropped from 19 to only those with genuine
  differences beyond relocations; true-0 count rose accordingly. Record both.
- Attribution line: `attribution: <n> cells checked, 0 mismatched` (SC-006).
- Immediate `corpus submit` again → 100% cache (SC-007 second half).

> **MEASURED (2026-07-08) — SC-002/003 NOT MET; STOPPED per research D6 / HANDOFF
> rule 5.** osCreateMesgQueue is still **true=20 reloc_blind=0** against the
> reloc-aware target (was true=20 in 002 — unchanged). Debugged the single pairing:
> the residual 20 = 4 reloc sites × PENALTY_REGALLOC(5). Reproduced deterministically
> (vendored Scorer, no IDO): current target `%hi(D_8002C3D0)` vs candidate
> `%hi(__osThreadTail)` → 20; *name-matched* target vs same candidate → **0**. So the
> sole cause is a reloc **symbol-name** mismatch: splat's asm names 0x8002C3D0
> `D_8002C3D0`, `symbol_addrs.us.txt` names it `__osEmptyMesgQueue`, and ultralib's
> source references `__osThreadTail` — three different names for one address. The
> permuter only forgives a differing reloc field when the candidate side is
> section-relative (`.` in field); named-vs-named differences count. The reloc-aware
> target machinery is correct (relocations present, gate passes, reloc_blind=0); the
> gap is symbol-name reconciliation between target asm and corpus candidates, which
> the spec Assumptions place OUT OF SCOPE. Flag summary after re-score: `8 true-0
> (all leaf: strlen memcpy strchr guMtxIdentF guMtxL2F osPhysicalToVirtual
> __osAiDeviceBusy __osIdCheckSum), 18 reloc_only_diff (19→18; none upgraded), 46
> neither`. attribution: `218967 cells checked, 0 mismatched (expect 0), legacy=218821`
> (SC-006 ✓). Immediate resubmit → 100% cache. Did NOT patch the scorer, lower the
> bar, or proceed to scale.

## 4. Lock regression gate (SC-004)

```bash
python3 -m tools.conveyor.pipeline.lock verify     # all 12 must re-prove score 0
python3 -m tools.conveyor.pipeline.lock check
```

Both green — the 12 locked functions score 0 against their reloc-aware targets.

> **MEASURED (2026-07-08) — SC-004 REGRESSION (4 of 12 fail).** `lock verify`:
> pass@0 → guMtxIdentF, guMtxL2F (raw_word), memcpy, strchr, strlen,
> __osAiDeviceBusy, osPhysicalToVirtual, __osIdCheckSum (leaf, no relocs);
> **FAIL → osDpGetCounters=40, __osSpDeviceBusy=10, __osSpSetPc=20,
> __osSpSetStatus=10**. Root cause: these hardware-register functions became
> reloc_aware because splat symbolized their MMIO addresses (osDpGetCounters=8
> relocs, __osSpSetPc=4, the two DeviceBusy/SetStatus=2), but IDO emits *literal*
> immediates for those `#define`'d KSEG1 addresses (no relocation). The raw-word
> target baked the same literal → matched at 0; the reloc-aware target renders
> `%hi(SYM)` vs the candidate's `lui 0xa4..` literal → 4/2/1 reloc-site penalties.
> The round-trip gate cannot catch this — it verifies ROM-faithfulness (masked
> words equal), which is true, but ROM-faithful-reloc ≠ matches-a-literal-candidate.
> Per HANDOFF ("any lock failure is a feature bug; do not re-pin") the locks were
> NOT re-pinned. `lock check` (source-hash, no pool) stays green.

## 5. Suite + wrap

```bash
python3 -m pytest tests/conveyor -m "not node_required" -q
```

Green including `test_target_assembly.py` and `test_supersession.py`. Commit per
project conventions; leave wiki/status updates to the reviewer.

## 6. Review outcome (2026-07-08, Fable)

Blocker 2 (SC-004) fixed in review: KSEG1 de-symbolization at region-index time
(see the contract amendments). After the fix: re-extraction superseded 21
targets / 18 rows, deterministic on rerun (0/0); **`lock verify`: all 12 at
score 0** (SC-004 ✓); corpus re-score: **12 true-0** (was 8 — the 4 MMIO
functions joined), 19 reloc_only_diff, attribution `0 mismatched` after the
new ingest guard dropped the 4 stale late-arriving cells (SC-006 ✓).

Blocker 1 (SC-002/003) stands as Opus analyzed it: reloc **symbol-name**
mismatch across codebases (target asm `D_8002C3D0` / symbol_addrs
`__osEmptyMesgQueue` / ultralib `__osThreadTail`). Ruling: SC-002 as written
was mis-specified — cross-codebase candidates legitimately carry different
symbol names, so `reloc_blind=0` is the correct ceiling for *corpus* evidence
on globals-referencing functions; **true 0 is proven at adoption time** (adopt
the candidate into src/ with our symbol names, then `lock add`), exactly the
per-function flow used for the 12 locks. Follow-up items on the improvements
wiki: symbol-name canonicalization (rename target reloc symbols from
symbol_addrs; requires symbol-name governance between asm, symbol table, and
src/) and, further out, name-blind bijective reloc scoring for cross-codebase
true-0.
