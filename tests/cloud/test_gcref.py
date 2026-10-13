"""Fast checks of tools/gcref (GameCube rush_d.elf reference tooling).

Skip cleanly when rush_d.elf (repo root), capstone, or the N64 build image is absent.
"""
import os
import sys

import pytest

REPO = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
sys.path.insert(0, REPO)
ELF = os.path.join(REPO, 'rush_d.elf')

pytestmark = pytest.mark.skipif(not os.path.exists(ELF), reason='rush_d.elf not present')


@pytest.fixture(scope='module')
def dwarf():
    from tools.gcref.elf import Elf
    from tools.gcref.dwarf1 import Dwarf1
    return Dwarf1(Elf(ELF))


@pytest.fixture(scope='module')
def gcprog():
    pytest.importorskip('capstone')
    from tools.gcref.disasm import GcProgram
    return GcProgram(ELF)


@pytest.fixture(scope='module')
def gcfeats(gcprog):
    from tools.gcref.disasm import analyse, build_outputs, cs_ppc
    feats = analyse(gcprog, cs_ppc())
    build_outputs(gcprog, feats)
    return feats


def test_dwarf_entry_counts_match_writeup(dwarf):
    counts = {}
    for e in dwarf.walk():
        if not e.stripped:
            counts[e.tagname] = counts.get(e.tagname, 0) + 1
    assert counts['structure_type'] == 2431
    assert counts['member'] == 26640
    assert counts['union_type'] == 178
    assert counts['enumeration_type'] == 130
    assert counts['formal_parameter'] == 2757
    assert counts['local_variable'] == 5591


def test_dwarf_struct_layout(dwarf):
    from tools.gcref.dwarf1 import render_aggregate
    kmv = [e for e in dwarf.entries.values() if e.tag == 0x13 and e.name == 'KMVERTICES'][0]
    text = render_aggregate(dwarf, kmv)
    assert 'struct KMVERTICES { /* size 0x14 */' in text
    assert 'float fV; /* 0x010 size 0x4 */' in text
    md = [e for e in dwarf.entries.values() if e.tag == 0x13 and e.name == 'tModelData'][0]
    text = render_aggregate(dwarf, md)
    assert 'struct _tCar *car; /* 0x000 size 0x4 */' in text
    assert 'float tireuvs[4][3][3]; /* 0x178 size 0x90 */' in text


def test_dwarf_function_signature(dwarf):
    from tools.gcref.dwarf1 import build
    _, functions, globals_, _ = build(dwarf)
    f = [x for x in functions if x['name'] == 'AdjustSpeed'][0]
    assert f['signature'] == 'static void AdjustSpeed(struct tModelData *m, float tspd)'
    assert f['file'] == 'maxpath.c' and f['address'] == '0x800270E4'
    cp = [x for x in functions if x['name'] == 'CheckCPs'][0]
    dist = [l for l in cp['locals'] if l['name'] == 'distVec'][0]
    assert dist['type'] == 'float [3]' and dist['location'] == {'kind': 'stack', 'base': 'r1', 'offset': 8}


def test_disasm_symbolisation(gcprog, tmp_path):
    from tools.gcref.disasm import analyse, cs_ppc
    feats = analyse(gcprog, cs_ppc(), str(tmp_path), only='_PrintScores')
    text = (tmp_path / 'highscorescr.c' / '_PrintScores.s').read_text()
    assert 'bl       sprintf' in text
    assert '@sda21(r13)' in text and 'gTrackNum@sda21(r13)' in text
    assert '# "%01d:%02d.%03d"' in text
    assert 'lfs      f0, @581@sda21(r2)  # 6969.0' in text
    assert 'psq_st' in text                     # paired-single mode on
    f = feats['_PrintScores']
    assert 'sprintf' in f['callees'] and '%01d:%02d.%03d' in f['strings']


def test_disasm_table_strings(gcfeats):
    censor = gcfeats['censor']
    assert 'WETBACK' in censor['strings_via_tables'] or 'WETBACK' in censor['strings']
    assert 'fixword' in censor['callees']


def test_correlate_seed_and_string_pair(gcfeats):
    from tools.gcref import correlate
    try:
        root = correlate.find_n64_root()
    except SystemExit:
        pytest.skip('N64 build/game_code.bin not available')
    images, by_addr = correlate.load_n64(REPO if os.path.exists(os.path.join(REPO, 'asm', 'us', 'blob')) else root,
                                         root)
    ext = {a: n for a, n in by_addr.items() if not n.startswith('D_')}
    n64 = correlate.n64_features(images[0], ext)
    order = sorted(n64, key=lambda k: n64[k]['address'])
    corr = correlate.Correlator(n64, gcfeats, order)
    seeds = {n: n for n in correlate.SEED_NAMES if n in gcfeats and n not in correlate.GAME_SEEDS}
    assert {'sprintf', 'strlen', 'memcpy', 'sinf'} <= set(seeds)
    # the N64 censor routine shares its bad-word pointer table with GC highscorescr.c censor
    cand = corr.score_one('func_800F084C', seeds, {})
    best = max(cand.items(), key=lambda kv: kv[1][0])
    assert best[0] == 'censor'
    # a seed (libc) pair turns into call anchors for N64 callers of sprintf
    callers = [n for n, f in n64.items() if 'sprintf' in f['callees']]
    assert callers
    ev = [e for n in callers[:20] for g, (s, evs) in corr.score_one(n, seeds, {}).items() for w, e in evs]
    assert any(e == 'call sprintf' for e in ev)
