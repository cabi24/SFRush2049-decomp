# Quickstart: Game-Code Context Bootstrap (005)

End-to-end bring-up and acceptance walkthrough. Everything except §4 runs on
the Pi alone; §4 needs the coordinator + watchman builder (same bring-up as
`specs/001-matching-pipeline/quickstart.md` §1–4).

## 1. Extent repair (SC prerequisite — run first)

```bash
python3 -m tools.conveyor.pipeline.matrix extract
```

Expect the new repair report: `extents: N agree, N repaired, N conflict`,
plus the 10 cluster before→after lines. Verify against the oracle table in
`contracts/extent-repair.md`:

```bash
python3 - <<'EOF'
import sqlite3, os
db = sqlite3.connect(os.path.expanduser("~/.conveyor/conveyor.db")); db.row_factory = sqlite3.Row
for r in db.execute("SELECT target_id, address, insn_count, gate_reason FROM n64_target"
                    " WHERE target_id IN ('game_loop','countdown','countdown_handler')"):
    print(r["target_id"], hex(r["address"]), r["insn_count"], r["gate_reason"])
EOF
```

`countdown` must now read ~668 instructions (was 2). Re-run extract:
second pass must report `repaired 0` (idempotency / SC-003 determinism).

## 2. Histogram — the measurement instrument (Pi-only)

```bash
python3 -m tools.conveyor.pipeline.autodecomp clusters --population extracted --limit 0
```

Outputs `build/m2c_histogram.md` (ranked blockers) and
`build/m2c_histogram.json`. Check SC-003:

```bash
python3 - <<'EOF'
import json
d = json.load(open("build/m2c_histogram.json"))
assert sum(d["buckets"].values()) == d["run"]["targets"], d["buckets"]
print(d["buckets"])
EOF
```

Run twice; bucket counts must be identical. SC-004: read the top-10
blockers in the md — they should cover a majority of `blocked`.

## 3. Cluster seeds compile (SC-001, Pi-only)

```bash
python3 -m tools.conveyor.pipeline.autodecomp clusters --population extracted \
    --targets @tools/conveyor/clusters/game_loop.txt
```

Expect ≥8/10 in `compiled`, including ≥4 of the 60+-instruction members
(game_loop, playgame_state_change, RaceStateMachine_Update,
attract_or_transition, countdown, …). If blockers name game types, grow
`include/game_types.h` (provenance comment per definition — FR-003) and
re-run; the delta between histogram runs is the progress record.

Static no-regression (SC-005): before merging context changes,

```bash
python3 -m tools.conveyor.pipeline.autodecomp one viGetTimeToDeadline --dry-run
```

(or the tasks' chosen known-good static sample) — seed text must be
byte-identical before/after `game_types.h` lands.

## 4. Cluster seeds score (SC-002 — needs coordinator + builder)

```bash
# coordinator on Pi, agent on watchman (001 quickstart §1–4), then:
python3 -m tools.conveyor.pipeline.autodecomp seed --population extracted \
    --targets @tools/conveyor/clusters/game_loop.txt --budget 3600
python3 -m tools.conveyor.cli report            # after jobs drain
```

Every compiling cluster seed gets a recorded numeric score attributable to
its target (population-tagged in the report). Score 0 = matched (evidence
only — see §5).

## 5. Firewall check (SC-006)

```bash
python3 -m tools.conveyor.pipeline.lock add work/whatever/game_loop.c:game_loop  # MUST refuse
python3 -m tools.conveyor.pipeline.promote batch --locked --via-builder          # touches no extracted fn
```

The lock/promote guards must reject extracted-population functions with the
FR-010 error; `promotion_record` must show no extracted entries.

## 6. Tests

```bash
pytest tests/conveyor -m "not node_required"
```

New: `test_extent_scan.py`, `test_disasm.py`,
`test_autodecomp_population.py` — plus the full existing suite green.
