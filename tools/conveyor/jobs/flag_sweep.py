"""flag_sweep executor: one translation unit's functions across a flag lattice.

Manifest:
    {
      "job_type": "flag_sweep",
      "toolkit_sha": "...",
      "tu": "src/libc/string.c",
      "flagsets": ["-g0 -O1 ...", "-g0 -O2 ..."],
      "functions": [{"name": "strlen", "source": "strlen.c",
                     "target": "strlen.o"}]
    }

Each function is a standalone extracted source (compiled per flagset, scored
against its known target). Aggregate score per flagset = sum over functions;
lower is better; a perfect flagset scores 0.
"""
import sys
import tempfile
from pathlib import Path

_HERE = Path(__file__).resolve().parent
if str(_HERE) not in sys.path:
    sys.path.insert(0, str(_HERE))

import compile_score  # noqa: E402
import scoring  # noqa: E402


def run(job_dir, manifest, progress):
    job_dir = Path(job_dir)
    inputs = job_dir / "inputs"
    rankings = []
    total = len(manifest["flagsets"])
    for i, flagset in enumerate(manifest["flagsets"]):
        aggregate, compiled, failed = 0, 0, 0
        with tempfile.TemporaryDirectory() as tmp:
            for fn in manifest["functions"]:
                out_o = Path(tmp) / (fn["name"] + ".o")
                ok, _ = compile_score.compile_one(
                    inputs / fn["source"], flagset, out_o,
                    include_dirs=[inputs],
                )
                if not ok:
                    failed += 1
                    aggregate += scoring.PENALTY_INF // 1000  # heavy, not inf
                    continue
                compiled += 1
                aggregate += scoring.score(inputs / fn["target"], out_o)
        rankings.append({"flagset": flagset, "aggregate_score": aggregate,
                         "compiled": compiled, "failed": failed})
        progress.update(stage="flag_sweep", flagsets_done=i + 1,
                        flagsets_total=total)
    rankings.sort(key=lambda r: (r["aggregate_score"], r["flagset"]))
    return {"tu": manifest["tu"], "rankings": rankings}, {}
