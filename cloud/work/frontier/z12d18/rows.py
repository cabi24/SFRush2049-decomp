"""rows.py SRC [SRC...]: obj71 each source, keep its rows as <scratch>/<stem>.rows, print score and differing ranges."""
import os, re, subprocess, sys
from pathlib import Path
S = Path("/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/62f66057-aadb-46cb-af75-cd497ac20bf0/scratchpad/z")
def rows(path):
    return [l.rstrip("\n") for l in open(path)]
def ranges(t, m):
    idx = [i for i in range(max(len(t), len(m))) if (t[i][9:] if i < len(t) else "") != (m[i][9:] if i < len(m) else "")]
    out, s, p = [], None, None
    for i in idx:
        if s is None: s = p = i
        elif i - p > 4: out.append((s, p)); s = p = i
        else: p = i
    if s is not None: out.append((s, p))
    return " ".join(f"{a}-{b}" for a, b in out)
for src in sys.argv[1:]:
    env = {**os.environ, "SCR": "rush2049/scratch/frontier/z12d18"}
    r = subprocess.run(["sh", "cloud/work/frontier/tools/trace71/obj71.sh", src, "func_80012D18"], capture_output=True, text=True, env=env)
    m = re.search(r"differing rows (\d+)", r.stdout + r.stderr)
    stem = Path(src).stem
    subprocess.run(["scp", "-q", "watchman2:rush2049/scratch/frontier/z12d18/t71/run/func_80012D18/m.rows", str(S / f"{stem}.rows")])
    print(f"{stem}: {m.group(1) if m else '?'}  |  {ranges(rows(S/'t.rows'), rows(S/(stem + '.rows')))}")
