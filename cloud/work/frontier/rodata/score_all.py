#!/usr/bin/env python3
"""Score every locked body with a given scorer and write one line per result.

    python3 cloud/work/frontier/rodata/score_all.py tools/cloud/score.py out.txt [jobs]

Singles use the lock's flagset; groups use their directory. Each output line is
`<kind> <name> exit=<code> <scorer output joined with ' | '>`, sorted, so two
runs (before/after a scorer change) can be compared with diff(1).
"""
import json
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

REPO = Path(__file__).resolve().parents[4]


def run(job):
    kind, name, command = job
    proc = subprocess.run(command, capture_output=True, text=True, cwd=REPO)
    text = " | ".join(line.strip() for line in (proc.stdout + proc.stderr).splitlines()
                      if line.strip())
    return f"{kind} {name} exit={proc.returncode} {text}"


def main():
    scorer, out = sys.argv[1], sys.argv[2]
    jobs_n = int(sys.argv[3]) if len(sys.argv) > 3 else 2
    lock = json.loads((REPO / "blob_matched.lock.json").read_text())
    jobs = []
    for name, entry in sorted(lock.items()):
        if "group" not in entry:
            jobs.append(("single", name, [sys.executable, scorer, "fn", entry["source"],
                                          name, "--flags", entry["flagset"]]))
    for group in sorted(p.name for p in (REPO / "src/blob/groups").iterdir() if p.is_dir()):
        jobs.append(("group", group, [sys.executable, scorer, "group",
                                      f"src/blob/groups/{group}"]))
    with ThreadPoolExecutor(jobs_n) as pool:
        lines = sorted(pool.map(run, jobs))
    Path(out).write_text("\n".join(lines) + "\n")
    ok = sum(f" exit=0 " in line + " " for line in lines)
    singles = sum(line.startswith("single") for line in lines)
    print(f"{len(lines)} jobs ({singles} singles, {len(lines) - singles} groups): "
          f"{ok} exit 0, {len(lines) - ok} nonzero")


if __name__ == "__main__":
    main()
