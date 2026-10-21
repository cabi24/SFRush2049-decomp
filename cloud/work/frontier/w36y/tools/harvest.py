"""w36y: harvest data-symbol identities already stated in lane RESULTS / matched sources ("D_80xxxxxx = gName").

    python3 cloud/work/frontier/w36y/tools/harvest.py     # writes cloud/work/frontier/w36y/build/harvest.json

Each claim keeps its file(s) as provenance. Overlay addresses are per image: the image is taken from the file path
(ovl_a / ovl_b / image A / image B words) when it can be told, else left open ('ovl').
"""
import glob
import json
import os
import re
from collections import defaultdict

REPO = '/home/cburnes/projects/rush2049-decomp'
HERE = os.path.dirname(os.path.abspath(__file__))
NAME = r'((?:g|s)[A-Z][A-Za-z0-9_]*|[a-z]+[A-Z][A-Za-z0-9_]*|[a-z]{3,}[a-z0-9]*(?=\[))'
PAT = re.compile(r'D_(80[0-9A-Fa-f]{6})`? *(?:=|->|is|:) *`?' + NAME)
PAT2 = re.compile(r'(?<![\w>.])' + NAME.replace('(?=\\[)', '') + r'`? = `?(?:image-?[AB] )?D_(80[0-9A-Fa-f]{6})\b(?! *\+)')
PAT3 = re.compile(r'D_(80[0-9A-Fa-f]{6})`? ' + NAME.replace('(?=\\[)', '') + r'(?=[\[ ,;.)]|$)')
LANE_IMAGE = {'w36a': 'ovl_a', 'w36b': 'ovl_a', 'w36c': 'ovl_a', 'w36d': 'ovl_a', 'w36e': 'ovl_a', 'w36f': 'ovl_a',
              'w36g': 'ovl_a', 'w36h': 'ovl_b', 'w36j': 'ovl_b'}
STOP = {'and', 'the', 'base', 'data', 'stride', 'array', 'table', 'records', 'record', 'entry', 'field', 'pointer',
        'int', 'char', 'short', 'float', 'void', 'struct', 'size', 'from', 'with', 'into', 'byte', 'word', 'half'}
SKIP_NAMES = {'gDPLoadTLUT_pal256', 'fixedWidth', 'spaceWidth'}


def files():
    pats = ['cloud/work/frontier/*/RESULTS.md', 'cloud/work/frontier/*/*.md', 'cloud/work/frontier/*/*/*.md',
            'src/blob/*.c', 'src/blob/groups/*/*.c', 'src/ovl_a/*.c', 'src/ovl_b/*.c', 'src/ovl_a/groups/*/*.c',
            'src/ovl_b/groups/*/*.c', 'cloud/matches/*.c', 'cloud/matches/ovl_a/*.c', 'cloud/matches/ovl_b/*.c']
    out = []
    for p in pats:
        out += glob.glob(os.path.join(REPO, p))
    return sorted(set(out))


def image_of(path, text_window):
    t = (path + ' ' + text_window).lower()
    a = 'ovl_a' in t or 'image a' in t or 'runtime a' in t
    b = 'ovl_b' in t or 'image b' in t or 'runtime b' in t
    if a and not b:
        return 'ovl_a'
    if b and not a:
        return 'ovl_b'
    return 'ovl'


def gc_names():
    out = set()
    for line in open(os.path.join(REPO, 'reference', 'repos', 'mat3', 'decomp', 'config', '2049LE', 'symbols.txt')):
        m = re.match(r'(\S+) = \.(\w+):', line)
        if m and 'type:function' not in line:
            out.add(m.group(1))
    return out


def main():
    gcn = gc_names()
    claims = defaultdict(lambda: defaultdict(set))
    for path in files():
        if '/w36y/' in path:
            continue
        try:
            text = open(path, errors='replace').read()
        except OSError:
            continue
        if path.endswith('.c'):
            # claims live in comments; drop code (e.g. `D_801543A4 = gCountDown;` is an assignment)
            import re as _re
            text = '\n'.join(_re.findall(r'/\*.*?\*/', text, _re.S) + _re.findall(r'//[^\n]*', text))
        found = [(m, int(m.group(1), 16), m.group(2)) for m in PAT.finditer(text)
                 if m.group(2) in gcn or not m.group(0).rstrip().endswith('[')]
        found += [(m, int(m.group(2), 16), m.group(1)) for m in PAT2.finditer(text) if m.group(1) in gcn]
        found += [(m, int(m.group(1), 16), m.group(2)) for m in PAT3.finditer(text) if m.group(2) in gcn]
        rel = os.path.relpath(path, REPO)
        lane = rel.split('/')[3] if rel.startswith('cloud/work/frontier/') else None
        for m, addr, name in found:
            if name in SKIP_NAMES or name.lower() in STOP or name.startswith('func_'):
                continue
            scope = 'game'
            if 0x8038A400 <= addr < 0x80400000:
                win = text[max(0, m.start() - 40):m.end() + 10].lower()
                if 'image-a' in win or 'image a' in win:
                    scope = 'ovl_a'
                elif 'image-b' in win or 'image b' in win:
                    scope = 'ovl_b'
                elif lane in LANE_IMAGE:
                    scope = LANE_IMAGE[lane]
                else:
                    scope = image_of(rel, text[max(0, m.start() - 300):m.end() + 100])
            claims[(scope, addr)][name].add(rel)
    rows = []
    for (scope, addr), d in sorted(claims.items(), key=lambda kv: (kv[0][1], kv[0][0])):
        for name, fs in sorted(d.items()):
            rows.append(dict(scope=scope, address='0x%08X' % addr, name=name, sources=sorted(fs)[:6],
                             n_sources=len(fs), contested=len(d) > 1))
    os.makedirs(os.path.join(HERE, '..', 'build'), exist_ok=True)
    json.dump(dict(rows=rows), open(os.path.join(HERE, '..', 'build', 'harvest.json'), 'w'), indent=1)
    print('%d claims, %d addresses' % (len(rows), len(claims)))


if __name__ == '__main__':
    main()
