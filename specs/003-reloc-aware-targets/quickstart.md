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

> **MEASURED**: toolkit sha ________, smoke ________

## 2. Re-extraction with the gate

```bash
python3 -m tools.conveyor.pipeline.matrix extract
```

Expected output (new lines from this feature):
`target tiers: reloc_aware=<n≥150> raw_word_static=<m> raw_word_dynamic=<k>` (SC-001),
`gate fallbacks: … top reasons …`, `superseded: <p> targets, <q> rows purged`.

Determinism check (SC-007): run extract again — `superseded: 0 targets, 0 rows`,
tier counts identical.

> **MEASURED**: tiers ________, fallbacks ________, superseded ________, rerun ________

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

> **MEASURED**: true-0 ________, flagged 19 → ________, attribution ________

## 4. Lock regression gate (SC-004)

```bash
python3 -m tools.conveyor.pipeline.lock verify     # all 12 must re-prove score 0
python3 -m tools.conveyor.pipeline.lock check
```

Both green — the 12 locked functions score 0 against their reloc-aware targets.

> **MEASURED**: ________

## 5. Suite + wrap

```bash
python3 -m pytest tests/conveyor -m "not node_required" -q
```

Green including `test_target_assembly.py` and `test_supersession.py`. Commit per
project conventions; leave wiki/status updates to the reviewer.
