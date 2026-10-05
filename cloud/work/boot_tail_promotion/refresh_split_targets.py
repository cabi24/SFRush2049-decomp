"""Rebuild the reloc-aware targets of named static functions from the current
asm through `targets.populate` (the `matrix extract` code path), restricted to
those names. The boot-tail targets are not in the work/**/info.txt inventory, so
a temporary inventory carries each function's registered address and extent.

    python3 cloud/work/boot_tail_promotion/refresh_split_targets.py FN [FN...]

Prints each target's tier and object sha before and after. A target whose
object changes has its matrix evidence superseded, as in `matrix extract`.
"""
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path.cwd()))
from tools.conveyor.client import DEFAULT_DATA  # noqa: E402
from tools.conveyor.coordinator import db as dbmod  # noqa: E402
from tools.conveyor.coordinator.store import BlobStore  # noqa: E402
from tools.conveyor.pipeline import targets  # noqa: E402


def main(names):
    data = Path(DEFAULT_DATA)
    conn = dbmod.connect(data / "conveyor.db")
    rows = {r["target_id"]: dict(r) for r in conn.execute(
        "SELECT target_id, address, population, insn_count, target_o_sha, tier"
        " FROM n64_target WHERE target_id IN (%s)" % ",".join("?" * len(names)), names)}
    missing = sorted(set(names) - set(rows))
    if missing or any(r["population"] != "static" for r in rows.values()):
        sys.exit(f"not registered static targets: {missing or names}")
    with tempfile.TemporaryDirectory() as tmp:
        for name, r in rows.items():
            d = Path(tmp) / name
            d.mkdir()
            (d / "info.txt").write_text(
                f"name: {name}\naddress: 0x{r['address']:08X}\n"
                f"comment: ({r['insn_count'] * 4} bytes)\n")
        targets.populate(conn, BlobStore(data / "blobs"), work_dir=tmp)
    for name in names:
        after = conn.execute("SELECT target_o_sha, tier, gate_reason FROM n64_target"
                             " WHERE target_id=?", (name,)).fetchone()
        before = rows[name]
        print(f"{name}: {before['tier']} {before['target_o_sha'][:12]} -> "
              f"{after['tier']} {after['target_o_sha'][:12]}"
              + (f" ({after['gate_reason']})" if after["gate_reason"] else ""))


if __name__ == "__main__":
    main(sys.argv[1:])
