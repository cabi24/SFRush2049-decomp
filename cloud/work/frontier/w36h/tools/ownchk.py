"""ownchk.py OBJ NAME[,NAME] [--why]: run on the Pi from the repo root with python3 -I. Scores NAME in a group object
with tools/cloud/score.compare, but verifies own .rodata/.data against build/ovl/b/image.bin (the builder scratch
has no image-B artefact, so score.py reports them as unverified)."""
import sys
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score, owndata
score.ASM_DIR = Path('asm/us/ovl_b').resolve()
img = owndata.ImageData.from_image(Path('build/ovl/b/image.bin').read_bytes(), 0x8038A400)
score.own_data = lambda: img
obj = Path(sys.argv[1])
for n in sys.argv[2].split(','):
    r = score.compare(obj, n)
    print(n, r.summary()[:200])
    for note in r.notes[:40]: print('   ', note)
    if '--why' in sys.argv:
        print('  dir:', [a for a in dir(r) if not a.startswith('_')])
        for a in ('errors','unresolved','extra_words'):
            if hasattr(r,a): print('  ',a, getattr(r,a))
