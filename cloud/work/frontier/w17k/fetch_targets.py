#!/usr/bin/env python3
"""w17k (Pi, read-only): copy the reloc-aware inventory target .o of each named
static function (n64_target.target_o_sha, the object `lock add` scores against)
into OUTDIR/<target_id>.o.
    python3 cloud/work/frontier/w17k/fetch_targets.py OUTDIR fn [fn...]"""
import shutil
import sys
from pathlib import Path

sys.path.insert(0, str(Path.cwd()))
from tools.conveyor.client import DEFAULT_DATA  # noqa: E402
from tools.conveyor.coordinator import db as dbmod  # noqa: E402
from tools.conveyor.coordinator.store import BlobStore  # noqa: E402

out = Path(sys.argv[1])
out.mkdir(parents=True, exist_ok=True)
conn = dbmod.connect(Path(DEFAULT_DATA) / "conveyor.db")
store = BlobStore(Path(DEFAULT_DATA) / "blobs")
for fn in sys.argv[2:]:
    row = conn.execute("SELECT population, target_o_sha FROM n64_target WHERE target_id=?",
                       (fn,)).fetchone()
    if row is None or not row["target_o_sha"] or row["population"] != "static":
        sys.exit(f"{fn}: no static inventory target")
    shutil.copyfile(store.get(row["target_o_sha"]), out / f"{fn}.o")
    print(fn, row["target_o_sha"][:12])
