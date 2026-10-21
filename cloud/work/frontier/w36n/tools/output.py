"""w36n outputs: map_functions.json, map_functions.md, renames_proposed.json (lane dir only)."""
import glob
import json
import os
import re
from collections import Counter, defaultdict

from data import REPO

LANE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LV = {1: 'lead', 2: 'strong', 3: 'proven'}
PREFIX = {'highscorescr.c': 'highscore_', 'optionscr.c': 'options_', 'audioscr.c': 'audio_',
          'controlscr.c': 'controls_', 'videoscr.c': 'video_', 'pausescr.c': 'pause_', 'carscr.c': 'car_',
          'trackscr.c': 'track_', 'startscr.c': 'start_', 'playerscr.c': 'player_', 'circuitscr.c': 'circuit_',
          'recordscr.c': 'records_', 'gameoverscr.c': 'gameover_', 'titlescr.c': 'title_',
          'licensescr.c': 'license_', 'modescr.c': 'mode_', 'cheatscr.c': 'cheat_', 'warningscr.c': 'warning_',
          'pakscr.c': 'pak_', 'demo.c': 'demo_', 'hud.c': 'hud_'}
FUNC_RE = re.compile(r'^func_[0-9A-F]{8}$')


def proposed_name(gc, g):
    v = gc[g]
    base = v['name']
    if base.startswith('_') and not base.startswith('__'):
        names = getattr(proposed_name, '_names', None)
        if names is None:
            names = proposed_name._names = set(x['name'] for x in gc.values())
        if base[1:] not in names:      # keep the underscore when GC also has the bare name (_ReadGhostData)
            base = base[1:]
    if '@' in g:
        f = v['file']
        pre = PREFIX.get(f, re.sub(r'(scr)?\.c$', '', f) + '_')
        base = pre + base
    return base


def label_wrong(name, g, newname, applied):
    if FUNC_RE.match(name) or name in applied:
        return False
    return name != newname and name.lstrip('_') != g.split('@')[0].lstrip('_')


def existing_symbols():
    names = set()
    for p in ['asm/us/blob/symbols.json', 'asm/us/ovl_a/symbols.json', 'asm/us/ovl_b/symbols.json']:
        names |= set(json.load(open(os.path.join(REPO, p)))['symbols'])
    return names


LIBC = set('bsearch qsort rand srand abs labs memmove memcpy memset memcmp strcpy strncpy strcmp strncmp strlen '
           'strcat strchr strrchr sprintf printf atoi atof sqrt sqrtf sin cos sinf cosf tan atan atan2 floor ceil '
           'fabs modf exit malloc free'.split())


def header_words():
    """identifiers declared as functions/objects in include/ (comments stripped): name -> header path."""
    words = {}
    for p in glob.glob(os.path.join(REPO, 'include', '**', '*.h'), recursive=True):
        try:
            t = open(p, errors='replace').read()
        except Exception:
            continue
        t = re.sub(r'/\*.*?\*/', ' ', t, flags=re.S)
        t = re.sub(r'//[^\n]*', ' ', t)
        for m in re.finditer(r'\b([A-Za-z_][A-Za-z0-9_]*)\s*\(', t):
            words.setdefault(m.group(1), os.path.relpath(p, REPO))
    return words


def write(M):
    import anchors
    applied = set(anchors.APPLIED)
    gc = M.gc
    rows = []
    for n, g in M.m.items():
        p = M.pop.get(n)
        if p is None:
            continue
        v = gc[g]
        newname = proposed_name(gc, g)
        evs = M.ev[(n, g)]
        evl = []
        for kind, lv, src, text in sorted(evs, key=lambda e: (-e[1], e[0])):
            evl.append('%s[%s]: %s' % (kind, LV[lv], text))
        rows.append(dict(population=p['pop'], n64_name=p['name'], address='0x%08X' % p['address'] if p['address'] else None,
                         size=p['size'], matched=p['matched'], mat3_name=v['name'], mat3_key=g, mat3_file=v['file'],
                         gc_address='0x%08X' % v['address'] if v['address'] else None, gc_size=v['size'],
                         gc_stripped=v['stripped'], confidence=LV[M.conf[n]], evidence=evl,
                         proposed_name=newname, current_label_wrong=label_wrong(p['name'], g, newname, applied)))
    popord = {'game': 0, 'ovl_a': 1, 'ovl_b': 2, 'static': 3}
    rows.sort(key=lambda r: (popord[r['population']], r['address'] or '', r['n64_name']))
    conflicts = [dict(n64=c[0], gc=c[1], n64_candidates=c[2], gc_candidates=c[3]) for c in M.conflicts]
    json.dump(dict(generated_by='cloud/work/frontier/w36n/tools/mapfn.py (w36n, wave 36)',
                   note='MAT3/GC -> N64 function map; confidence proven/strong/lead per tools/mapfn.py docstring. '
                        'Overlay names are per image (ovl_a / ovl_b). Not applied anywhere.',
                   rows=rows, conflicts=conflicts), open(os.path.join(LANE, 'map_functions.json'), 'w'), indent=1)
    # ---------------------------------------------------------------------------------- stats
    stats = {}
    for popn in ('game', 'ovl_a', 'ovl_b', 'static'):
        allf = [v for v in M.pop.values() if v['pop'] == popn and (popn != 'static' or v['size'])]
        un = [v for v in allf if not v['matched']]
        mapped = {r['n64_name']: r for r in rows if r['population'] == popn}
        def cnt(fs, levels):
            sel = [v for v in fs if v['name'] in mapped and mapped[v['name']]['confidence'] in levels]
            return len(sel), sum(v['size'] for v in sel)
        stats[popn] = dict(functions=len(allf), bytes=sum(v['size'] for v in allf), unmatched=len(un),
                           unmatched_bytes=sum(v['size'] for v in un),
                           un_ps=cnt(un, ('proven', 'strong')), un_p=cnt(un, ('proven',)), un_l=cnt(un, ('lead',)),
                           all_ps=cnt(allf, ('proven', 'strong')), all_p=cnt(allf, ('proven',)),
                           all_l=cnt(allf, ('lead',)),
                           wrong=sum(1 for r in mapped.values() if r['current_label_wrong'] and r['confidence'] != 'lead'))
    st = [r for r in rows if r['population'] == 'static']
    # ------------------------------------------------------------------------------- renames
    existing = existing_symbols()
    hw = header_words()
    taken = defaultdict(list)
    for r in rows:
        if r['confidence'] in ('proven', 'strong'):
            taken[(r['population'] if r['population'] != 'static' else 'game', r['proposed_name'])].append(r)
    game_ren, ovl_ren, held = [], [], []
    for r in rows:
        if r['confidence'] not in ('proven', 'strong'):
            continue
        old, new = r['n64_name'], r['proposed_name']
        if not (FUNC_RE.match(old) or r['current_label_wrong']):
            continue
        why = None
        popk = r['population'] if r['population'] != 'static' else 'game'
        if len(taken[(popk, new)]) > 1:
            why = 'name proposed for %d functions' % len(taken[(popk, new)])
        elif r['population'] in ('game', 'static') and new in existing:
            why = 'new name already a symbol'
        elif new in LIBC:
            why = 'new name is a C library name'
        src = 'GC %s %s%s' % (r['mat3_file'], r['mat3_name'], ' (file static; prefixed with the screen file)'
                              if '@' in r['mat3_key'] else '')
        ev = '; '.join(e.split(': ', 1)[0] + ': ' + e.split(': ', 1)[1][:160] for e in r['evidence'][:3])
        row = dict(vram=r['address'], old=old, new=new, kind='function', source=src,
                   evidence='w36n %s: %s' % (r['confidence'], ev), status='proposed', batch='w36n')
        if new in hw and not why:
            row['note'] = '%s already declares %s(...) (an unbound arcade-derived prototype): check its signature' % (
                hw[new], new)
        if why:
            row['held'] = why
            held.append(dict(row, population=r['population']))
        elif r['population'] == 'game':
            game_ren.append(row)
        elif r['population'] == 'static':
            row['kind'] = 'static_function'
            held.append(dict(row, population='static', held='static symbol: needs the static rename proof '
                                                                 '(static_rename), not the game batch'))
        else:
            ovl_ren.append(dict(row, image=r['population']))
    json.dump(dict(note='w36n proposed rename batch (NOT applied). renames: game functions in the exact '
                        'cloud/work/frontier/w29n/renames.json row format (status "proposed": the coordinator flips '
                        'accepted rows to status "accepted" and appends them to renames.json, then runs '
                        'RENAME_BATCH=w36n sh cloud/work/frontier/w29n/replay.sh). overlay_renames: runtime images '
                        'A/B (per-image names; no rename tool exists for overlay names, see RESULTS.md). held: '
                        'rows that need a decision (name collision, static symbol).',
                   renames=game_ren, overlay_renames=ovl_ren, held=held),
              open(os.path.join(LANE, 'renames_proposed.json'), 'w'), indent=1)
    # ------------------------------------------------------------------------------------- md
    L = ['# w36n MAT3 (GameCube) -> N64 function map', '',
         'Generated by `python3 cloud/work/frontier/w36n/tools/mapfn.py` (read-only analysis; nothing applied). '
         'Machine-readable: [map_functions.json](map_functions.json). Method and confidence rules: '
         '[RESULTS.md](RESULTS.md). Overlay names are per image: `func_8038xxxx` in image A and in image B are '
         'different functions.', '',
         '## Coverage', '',
         '| Population | functions (bytes) | unmatched (bytes) | unmatched with proven/strong identity (bytes) | of which proven | lead only | all functions with proven/strong identity | wrong labels (proven/strong) |',
         '|---|---|---|---|---|---|---|---|']
    for popn, s in stats.items():
        L.append('| %s | %d (%s) | %d (%s) | **%d (%s)** | %d (%s) | %d | %d (%s) | %d |' % (
            popn, s['functions'], format(s['bytes'], ','), s['unmatched'], format(s['unmatched_bytes'], ','),
            s['un_ps'][0], format(s['un_ps'][1], ','), s['un_p'][0], format(s['un_p'][1], ','), s['un_l'][0],
            s['all_ps'][0], format(s['all_ps'][1], ','), s['wrong']))
    L += ['', 'Confidence: **proven** = two independent kinds of evidence (or a matched GC port, or call-graph '
          'alignment between proven anchors); **strong** = one solid kind; **lead** = weak (an unverified correlation '
          'pair, a positional guess). Static and game/overlay coverage are separate denominators.', '']
    # ------------------------------------------------------------------ lane targets (wave 36)
    lt_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'lane_targets.json')
    if os.path.exists(lt_path):
        byn = defaultdict(list)
        for (nn, gg) in M.ev:
            if M.ev[(nn, gg)]:
                byn[nn].append((M.score_of(nn, gg), gg))
        rowmap = dict(((r['population'], r['n64_name']), r) for r in rows)
        L += ['## Wave-36 lane targets: identity check', '',
              'Lane table lead (coordinator) vs this map. "agrees" = same GC function; "differs" = the map found another '
              'identity (read the evidence); "new" = no lead was given. Candidates for unmapped functions are leads only.', '',
              '| Lane | N64 function | Size | Lane lead | w36n identity | Conf | Verdict | Key evidence |',
              '|---|---|---|---|---|---|---|---|']
        for lane, popn, name, size, lead in json.load(open(lt_path)):
            r = rowmap.get((popn, name))
            nid = '%s:%s' % (popn, name)
            if r:
                ident, conf = r['mat3_key'], r['confidence']
                ld = lead.rstrip('?').strip()
                if not ld:
                    verdict = 'new'
                elif ld.split('@')[0].lstrip('_') == ident.split('@')[0].lstrip('_') or ld == r['mat3_file']:
                    verdict = 'agrees'
                elif ident.split('@')[0].lstrip('_') in ld or (('@' in ld) and ld.split('@')[1] in r['mat3_file'] and
                                                               ld.split('@')[0] == ident.split('@')[0]):
                    verdict = 'agrees'
                else:
                    verdict = 'differs'
                ev = r['evidence'][0].replace('|', '/')[:120] if r['evidence'] else ''
            else:
                c = sorted(byn.get(nid, []), reverse=True)[:2]
                ident = ('candidates: ' + ', '.join(g for sc, g in c)) if c else '—'
                conf, verdict, ev = '', 'unmapped', ''
            L.append('| %s | %s:%s | %s | %s | %s | %s | %s | %s |' % (lane, popn, name, size or '', lead or '', ident,
                                                                       conf, verdict, ev))
        L.append('')
    for popn, title in (('game', 'Game code image'), ('ovl_a', 'Runtime image A'), ('ovl_b', 'Runtime image B'),
                        ('static', 'Static code (called from the game image)')):
        sel = [r for r in rows if r['population'] == popn]
        L += ['## %s (%d mapped: %s)' % (title, len(sel), ', '.join('%s %d' % (k, v) for k, v in
                                                                     sorted(Counter(r['confidence'] for r in sel).items()))),
              '', '| Address | N64 name | Size | M | MAT3 function | GC file | GC addr/size | Conf | Label wrong | Evidence |',
              '|---|---|---|---|---|---|---|---|---|---|']
        for r in sel:
            ev = '<br>'.join(e.replace('|', '/')[:150] for e in r['evidence'][:3])
            if len(r['evidence']) > 3:
                ev += '<br>+%d more' % (len(r['evidence']) - 3)
            L.append('| %s | %s | %s | %s | %s | %s | %s/%d%s | %s | %s | %s |' % (
                r['address'] or '', r['n64_name'], r['size'] or '', 'M' if r['matched'] else '',
                r['mat3_key'], r['mat3_file'], r['gc_address'] or 'stripped', r['gc_size'],
                ' (stripped)' if r['gc_stripped'] else '', r['confidence'],
                ('**yes** -> ' + r['proposed_name']) if r['current_label_wrong'] else '', ev))
        L.append('')
    if conflicts:
        L += ['## Unresolved conflicts (equal evidence for two pairings)', '']
        for c in conflicts:
            L.append('- %s / %s: N64 side %s; GC side %s' % (c['n64'], c['gc'], c['n64_candidates'], c['gc_candidates']))
    open(os.path.join(LANE, 'map_functions.md'), 'w').write('\n'.join(L) + '\n')
    print('stats', json.dumps(stats))
    print('renames: game %d, overlay %d, held %d' % (len(game_ren), len(ovl_ren), len(held)))
    return stats
