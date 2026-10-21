# usage (builder, in scratch): IDO_DIR=... python3 gdump.py cand/GROUP  -- compile the group and list .text symbols in order
import sys, os, struct, subprocess
sys.path.insert(0, 'tools/cloud')
import score
from pathlib import Path
out = Path('gdump.o').resolve()
score.compile_group(Path(sys.argv[1]), out)
elf = score.Elf(str(out)) if hasattr(score, 'Elf') else None
# use the toolkit's readelf-like helper in score.py if present; else parse with objdump
od = None
for cand in [os.path.expanduser('~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/bin/objdump'), 'mips-linux-gnu-objdump']:
    if os.path.exists(cand) or cand.startswith('mips'):
        od = cand; break
r = subprocess.run([od, '-t', str(out)], capture_output=True, text=True, env=dict(os.environ, LD_LIBRARY_PATH=os.path.expanduser('~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/lib')))
rows = []
for line in r.stdout.splitlines():
    p = line.split()
    if len(p) >= 5 and '.text' in line:
        try: rows.append((int(p[0], 16), p[-1], line))
        except ValueError: pass
for a, n, l in sorted(rows): print(hex(a), n)
