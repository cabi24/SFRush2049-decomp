#!/usr/bin/env python3
"""comb.py OUTDIR cand1.c [cand2.c ...]: write OUTDIR/<name>.c = slot_sound group.c + candidate (single-file group for bscore --keep)."""
import sys, re
from pathlib import Path
ROOT = Path('/home/cburnes/projects/rush2049-decomp')
G = (ROOT / 'src/blob/groups/slot_sound/group.c').read_text().replace('extern u8 D_80149B70;', 'extern s8 D_80149B70;')
def comb(text):
    t = re.sub(r'^typedef (signed|unsigned) \w+ [su](8|16|32);\n', '', text, flags=re.M)
    t = t.replace('void sound_update_channel(s32 force);\n', '').replace('extern s8 D_80149B70;\n', '')
    t = t.replace('extern s16 D_80149878[256];\n', '')
    t = t.replace('extern FontHdr *D_801497F0;\n', '#define D_801497F0 (*(FontHdr **)&D_801497F0)\n').replace('extern Glyph *D_80149800;\n', '#define D_80149800 (*(Glyph **)&D_80149800)\n')
    return G + '\n' + t
if __name__ == '__main__':
    out = Path(sys.argv[1]); out.mkdir(exist_ok=True)
    for f in sys.argv[2:]:
        (out / Path(f).name).write_text(comb(Path(f).read_text()))
