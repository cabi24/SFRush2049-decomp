#!/usr/bin/env python3
"""mk.py [SEGDIR] [OUTDIR]: assemble the superseding dot_cleanup_colortrace group: original group.c with the
Color4..CarHandles segment replaced by SEGDIR/ff04.c + SEGDIR/d6a4.c (+ SEGDIR/pre.c), FF04 and D6A4 promoted
to members."""
import json, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[5]
seg = Path(sys.argv[1] if len(sys.argv) > 1 else ROOT / 'cloud/work/frontier/w35a/seg')
out = Path(sys.argv[2] if len(sys.argv) > 2 else ROOT / 'cloud/work/frontier/w35a/groups/dot_cleanup_colortrace_20261011')
src = ROOT / 'src/ovl_a/groups/dot_cleanup_colortrace_20261011'
text = (src / 'group.c').read_text().split('\n')
i0 = next(i for i, l in enumerate(text) if l.startswith('typedef struct Color4'))
i1 = next(i for i, l in enumerate(text) if l.startswith('typedef struct CarHandles'))
HEAD_PATCHES = [
    ('extern u8 D_80146180[4],D_80140BDC;', 'extern u8 D_80146180[4];\nextern volatile u8 D_80140BDC;'),
]
head = '\n'.join(text[:i0])
for a, b in HEAD_PATCHES:
    assert a in head, a
    head = head.replace(a, b)
text = head.split('\n') + text[i0:]
i0 = len(head.split('\n'))
i1 = next(i for i, l in enumerate(text) if l.startswith('typedef struct CarHandles'))
new = text[:i0] + (seg / 'seg.c').read_text().rstrip('\n').split('\n') + [''] + text[i1:]
out.mkdir(parents=True, exist_ok=True)
banner = ['/* w35a superseding revision: func_8039FF04 (GC initButtons@carscr.c) and func_8039D6A4 (GC updateButtons@carscr.c) are now',
          ' * members, ported from reference/repos/mat3 carscr.c; see their headers below. Old members and their text are unchanged. */']
(out / 'group.c').write_text('\n'.join(new[:1] + banner + new[1:]))
(out.parent.parent / 'parts' / 'head.c').write_text('\n'.join(text[:i0]) + '\n')
(out.parent.parent / 'parts' / 'tail.c').write_text('\n'.join(text[i1:]))
g = json.loads((src / 'group.json').read_text())
for n in ('func_8039FF04', 'func_8039D6A4'):
    g['context'].remove(n)
    g['members'].append(n)
g['name'] = 'dot_cleanup_colortrace_20261011'
g['allow_unverified'] = True
g['claims'] = ['func_8039FF04', 'func_8039D6A4']
g['provenance'] = ('Supersedes the accepted same-name group (all six old members byte-identical: D300, D05C, D494, ED20, E200, E3BC). '
                   'w35a adds func_8039FF04 (GC initButtons@carscr.c) and func_8039D6A4 (GC updateButtons@carscr.c) as members from the MAT3 '
                   'carscr.c bodies; the only head change is D_80140BDC declared volatile u8 (used by context func_803A0FD8 only). '
                   'Own data/bss unverified for image A (no retail data tree).')
(out / 'group.json').write_text(json.dumps(g, indent=1) + '\n')
print('wrote', out)
