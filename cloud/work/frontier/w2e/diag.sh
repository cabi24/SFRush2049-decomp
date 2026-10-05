#!/bin/sh
# usage: diag.sh cand.c NAME [keep1,keep2] -- workbench diagnose: candidate .o vs a copy whose NAME words are retail
S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad
d=$(cd $(dirname "$0") && pwd); R=$(cd $d/../../../.. && pwd)
src="$1"; name="$2"; keep="$3"
scp -q "$src" watchman2:rush2049/scratch/frontier/w2e/cand/_d.c
ssh watchman2 "cd ~/rush2049/scratch/frontier/w2e && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 -c \"
import sys,json,tempfile; from pathlib import Path; sys.path.insert(0,'tools/cloud'); import score
keep='$keep'; flags='-g0 -O3 -mips2 -G 0 -non_shared'
if keep:
    g=Path('cand/_dg'); g.mkdir(exist_ok=True); (g/'group.c').write_text(Path('cand/_d.c').read_text())
    (g/'group.json').write_text(json.dumps({'files':['group.c'],'keep':keep.split(','),'flags':flags})); score.compile_group(g,Path('cand/_d.o'))
else: score.compile_single('cand/_d.c',flags,Path('cand/_d.o'))
\"" || exit 1
scp -q watchman2:rush2049/scratch/frontier/w2e/cand/_d.o $S/diag_cand.o
cd $R && python3 - "$S/diag_cand.o" "$S/diag_tgt.o" "$name" <<'PY'
import sys, struct
sys.path.insert(0, 'tools/cloud'); import score
from tools.conveyor.pipeline import blob_unit, blob_layout, frontier
cand, tgt, name = sys.argv[1:4]
data = bytearray(open(cand, 'rb').read())
secs = score._elf(cand) if False else None
fns = score.symbols(cand); words = score.text_words(cand)
start = fns[name]; end = min((o for o in fns.values() if o > start), default=len(words) * 4)
doc = blob_layout.load(); img, base = blob_unit.load_image(doc)
addr = score.image_symbols().get(name) or score.address_named(name)
import subprocess
# find .text file offset
import re
out = subprocess.run(['mips-linux-gnu-objdump', '-h', cand], capture_output=True, text=True).stdout
m = re.search(r'\.text\s+([0-9a-f]+)\s+[0-9a-f]+\s+[0-9a-f]+\s+([0-9a-f]+)', out)
off = int(m.group(2), 16)
n = (end - start) // 4
tw = [struct.unpack_from('>I', img, addr - base + 4 * i)[0] for i in range(n)]
# retail size: stop at the next function in the image if smaller
for i, w in enumerate(tw):
    struct.pack_into('>I', data, off + start + 4 * i, w)
open(tgt, 'wb').write(data)
print('patched %d words at %s' % (n, hex(addr)))
PY
python3 $R/tools/workbench.py diagnose $S/diag_tgt.o $S/diag_cand.o --symbol $name --objdump mips-linux-gnu-objdump --color never --pager never 2>&1 | head -${LINES_OUT:-80}
