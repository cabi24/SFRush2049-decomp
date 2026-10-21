"""w36y: proposed data-symbol renames (proven rows only) in the w29n renames.json row format. Not applied.

    python3 cloud/work/frontier/w36y/tools/renames.py     # writes renames_proposed.json

Rows: game/static-data addresses (GAME_SYMBOLS / symbol_addrs homes) in `renames`; runtime-image addresses in
`overlay_renames` (informational: no overlay rename tooling exists, see w36n RESULTS). Naming rule (names.md): GC
spelling; a GC static whose name occurs in several GC source files gets the N64 file prefix (`hud_`, `hudextra_`,
`carscr_` ...: the GC file stem). `collision_files` = src/ files that already use the new name as an identifier
(locals/parameters/fields named like the global): the rename tool must not touch those uses -- check them first.
"""
import glob
import json
import os
import re
import sys
from collections import Counter, defaultdict

HERE = os.path.dirname(os.path.abspath(__file__))
LANE = os.path.abspath(os.path.join(HERE, '..'))
REPO = '/home/cburnes/projects/rush2049-decomp'
sys.path.insert(0, REPO)


def symbol_addrs():
    out = {}
    for line in open(os.path.join(REPO, 'symbol_addrs.us.txt')):
        m = re.match(r'\s*([A-Za-z_]\w*)\s*=\s*0x([0-9A-Fa-f]+)\s*;(.*)', line)
        if m:
            out.setdefault(int(m.group(2), 16), (m.group(1), m.group(3).strip()))
    return out


def src_words():
    """{identifier: set(files)} over src/ C sources (to flag collisions)."""
    idx = defaultdict(set)
    for p in glob.glob(os.path.join(REPO, 'src', '**', '*.c'), recursive=True) + \
            glob.glob(os.path.join(REPO, 'src', '**', '*.h'), recursive=True) + \
            glob.glob(os.path.join(REPO, 'include', '*.h')):
        try:
            text = open(p, errors='replace').read()
        except OSError:
            continue
        for w in set(re.findall(r'\b[A-Za-z_]\w*\b', text)):
            idx[w].add(os.path.relpath(p, REPO))
    return idx


def main():
    rows = json.load(open(os.path.join(LANE, 'map_data.json')))['rows']
    from tools.conveyor.pipeline import disasm as D
    gsyms = D.GAME_SYMBOLS
    sa = symbol_addrs()
    words = src_words()
    names_count = Counter()
    gj = json.load(open(os.path.join(REPO, 'docs', 'external', 'gc', 'globals.json')))['globals']
    files_of = defaultdict(set)
    for g in gj:
        if g['scope'] != 'global' or True:
            files_of[g['name']].add(g['file'])
    statics_multi = set()
    for g in gj:
        if g['scope'] in ('local', 'static', 'file') or g.get('function'):
            pass
    # GC names defined as distinct objects in several files (statics): name -> number of distinct addresses
    addrs = defaultdict(set)
    for g in gj:
        addrs[g['name']].add(g['address'])
    out, ovl, held = [], [], []
    taken = set()
    for r in rows:
        if r['confidence'] != 'proven':
            continue
        a = int(r['n64_address'], 16)
        gname = r['mat3_name']
        new = gname
        if len(addrs[gname]) > 1:
            stem = os.path.splitext((r['mat3_file'] or '').split(',')[0])[0]
            new = '%s_%s' % (stem, gname)
        cur = r['n64_symbol'] or 'D_%08X' % a
        sa_name = sa.get(a, (None, ''))[0]
        ev = 'w36y proven: ' + '; '.join(r['evidence'][:3])
        if r.get('content'):
            ev += '; ' + r['content']
        row = dict(vram='0x%08X' % a, old=cur, new=new, kind='data',
                   source='GC %s %s (%s)' % (r['mat3_file'], gname, r['mat3_type']),
                   evidence=ev[:600], status='proposed', batch='w36y',
                   n64_type=r['n64_type'], volatile=r['volatile'], support_functions=r['support_functions'])
        if sa_name and sa_name != cur:
            row['symbol_addrs_label'] = sa_name
        if r['population'] in ('ovl_a', 'ovl_b'):
            row['image'] = r['population']
            ovl.append(row)
            continue
        if cur == new:
            continue
        if (a in gsyms and not gsyms[a].startswith('D_')):
            row['current_label_wrong'] = True
        col = sorted(words.get(new, set()))
        if col:
            row['collision_files'] = col[:8]
            row['collision_count'] = len(col)
        if len(new) <= 3 or len(col) >= 20:
            row['note'] = ('new name is too generic for a mechanical rename (%d src files use the identifier); '
                           'pick a distinct N64 spelling (e.g. g%s%s) or rename by hand' % (len(col), new[:1].upper(), new[1:]))
            held.append(row)
            continue
        if new in taken:
            row['note'] = 'duplicate new name in this batch'
            held.append(row)
            continue
        taken.add(new)
        out.append(row)
    out.sort(key=lambda r: r['vram'])
    ovl.sort(key=lambda r: (r['image'], r['vram']))
    json.dump(dict(note='w36y proposed data-symbol renames (proven rows of map_data.json only), w29n renames.json '
                        'row format, status proposed, batch w36y. NOT applied. `renames` = game image / static-data '
                        'addresses (homes: GAME_SYMBOLS in tools/conveyor/pipeline/disasm.py, symbol_addrs.us.txt for '
                        'labels there); `overlay_renames` = runtime images A/B (no rename tooling yet). '
                        '`current_label_wrong` = an existing hand label that names something else; '
                        '`symbol_addrs_label` = a speculative arcade label at the same address; `collision_files` = '
                        'src files already using the new name as an identifier (check before replay).',
                   renames=out, overlay_renames=ovl, held=held),
              open(os.path.join(LANE, 'renames_proposed.json'), 'w'), indent=1)
    print('renames %d (wrong labels %d, collisions %d), overlay %d, held %d' % (
        len(out), sum(1 for r in out if r.get('current_label_wrong')), sum(1 for r in out if r.get('collision_files')),
        len(ovl), len(held)))


if __name__ == '__main__':
    main()
