#!/bin/sh
# ownv.sh PACKET_DIR ovl_a|ovl_b : compile the packet on the builder (as score.py group does), fetch the object and
# score every member LOCALLY with score.compare, own data verified against the retail image build/ovl/<x>/image.bin.
set -e
D="$1"; IMG="$2"
LABEL=$(echo "$D" | sed 's#/*$##; s#^.*cloud/work/frontier/w41b/##; s#/#__#g')
R=rush2049/scratch/frontier/w41b
ssh watchman2 "rm -rf ~/$R/cand/$LABEL && mkdir -p ~/$R/cand/$LABEL" && scp -q "$D"/* watchman2:$R/cand/$LABEL/
ssh watchman2 "cd ~/$R && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 objsyms.py cand/$LABEL $IMG /tmp/w41b_ownv.o >/dev/null"
OUT=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/be6c3ef7-ec10-405a-a401-0caaa1bc1bac/scratchpad/ownv_$LABEL.o
scp -q watchman2:/tmp/w41b_ownv.o "$OUT"
python3 - "$OUT" "$IMG" "$D" <<'PY'
import sys, json
sys.path.insert(0, 'tools/cloud')
from pathlib import Path
import score, owndata
obj, img, d = sys.argv[1], sys.argv[2], sys.argv[3]
score.ASM_DIR = Path('asm/us/' + img).resolve()
letter = img[-1]
data = Path(f'build/ovl/{letter}/image.bin').read_bytes()
image = owndata.ImageData.from_image(data, 0x8038A400)
score.own_data = lambda: image
spec = json.loads((Path(d) / 'group.json').read_text())
bad = 0
for m in spec['members']:
    import io, contextlib
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        r = score.compare(obj, m)
    s = r.summary()
    print(f'{m}: {s}  {"; ".join(getattr(r, "notes", ())[:2])}')
    if s != 'MATCH': bad += 1
print('members', len(spec['members']), 'not strict MATCH (own data verified locally):', bad)
PY
