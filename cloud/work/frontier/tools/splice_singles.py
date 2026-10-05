"""splice_singles.py NAME=PATH ... : splice single-function sources with the flags on their first line."""
import re, sys
from pathlib import Path
from tools.conveyor.pipeline import blob_splice
from tools.conveyor.client import DEFAULT_DATA
from tools.conveyor.coordinator import db as dbmod
src, flags = {}, {}
for arg in sys.argv[1:]:
    name, _, path = arg.partition("=")
    path = Path(path or f"cloud/matches/{name}.c")
    text = path.read_text()
    m = re.search(r"-O[123]", text.splitlines()[0])
    if not m:
        sys.exit(f"{name}: no -O level on line 1 of {path}")
    src[name] = text
    flags[name] = f"-g0 {m.group(0)} -mips2 -G 0 -non_shared"
conn = dbmod.connect(Path(DEFAULT_DATA) / "conveyor.db")
r = blob_splice.splice(conn, list(src), lambda n: src[n], flagsets=flags)
print("flags", {n: f.split()[1] for n, f in flags.items()})
print("refused", r["refused"]); print("image_ok", r["image_ok"], "locked", len(r["spliced"]))
