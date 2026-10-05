"""Exercise the real split transaction with deterministic SPLAT output fixtures."""
import json
from pathlib import Path
from types import SimpleNamespace

import pytest

from tools.conveyor.pipeline import layout as L, lock
from tools.conveyor.pipeline import layout_split as S


@pytest.fixture
def repo(tmp_path, monkeypatch):
    root = tmp_path
    for rel in ('src/rom', 'asm/us/nonmatchings/rom/old', 'assets/us', 'build'):
        (root / rel).mkdir(parents=True)
    yaml = root / 'splat.us.yaml'
    yaml.write_text('''segments:
  - name: main
    type: code
    subsegments:
      - [0x8800, c, rom/old]
  - type: bin
    start: 0x8880
''')
    source = root / 'src/rom/old.c'
    source.write_text('''/* GENERATED ROM-aligned TU — segment 0x8800 (rom/old)
 * fixture generated header */
/* Context notice retained in both outputs. */
#include "rom_tu.h"
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/old/f_a.s")
/* PROMOTED fixture — f_b
 * Evidence: original lock and ROM gate
 */
int f_b(void) { return 1; }
''')
    syms = root / 'symbol_addrs.us.txt'
    syms.write_text('f_a = 0x80007C00; // type:func\nf_b = 0x80007C40; // type:func\n')
    locked = root / 'matched.lock.json'
    locked.write_text(json.dumps({'src/rom/old.c:f_b': {
        'target_id': 'f_b', 'flagset': '-g0 -O2 -mips2 -G 0 -non_shared',
        'body_sha256': lock.body_sha(source, 'f_b')}}))
    for name, address in [('f_a', 0x80007C00), ('f_b', 0x80007C40)]:
        (root / 'asm/us/nonmatchings/rom/old' / (name + '.s')).write_text(
            json.dumps({'address': address, 'words': ['00000000'] * 16}))
    (root / 'rush2049.us.ld').write_text('original linker')
    (root / 'assets/us/data.bin').write_bytes(b'original neighbors')
    for key, value in {'REPO': root, 'SPLAT_YAML': yaml, 'SYMBOL_ADDRS': syms,
                       'LOCKFILE': locked, 'NONMATCHINGS': root / 'asm/us/nonmatchings',
                       'ROM_SRC_DIR': root / 'src/rom',
                       'LAYOUT_JSON': root / 'build/layout.us.json',
                       'OPT_OVERRIDES_MK': root / 'src/rom/opt_overrides.mk'}.items():
        monkeypatch.setattr(L, key, value)
    monkeypatch.setattr(L, '_dirty', lambda paths: '')
    monkeypatch.setattr(L, '_nonzero_gap', lambda start, size: None)

    def collect():
        regions = {}
        for path in (root / 'asm/us').rglob('*.s'):
            item = json.loads(path.read_text())
            regions[item['address']] = SimpleNamespace(words=item['words'])
        return regions
    monkeypatch.setattr(L, 'collect_regions', collect)

    def extract():
        mapping = L.derive()
        for segment in mapping['segments']:
            for fn in segment['functions']:
                dest = root / 'asm/us/nonmatchings' / segment['rom_tu'] / (fn['name'] + '.s')
                dest.parent.mkdir(parents=True, exist_ok=True)
                dest.write_text(json.dumps({'address': int(fn['vaddr'], 16),
                                           'words': ['00000000'] * 16}))
        (root / 'rush2049.us.ld').write_text('regenerated: ' + ','.join(
            x['rom_tu'] for x in mapping['segments'] if x['functions']))
    monkeypatch.setattr(L, '_run_extract', extract)
    return root


def files(root):
    return {str(p.relative_to(root)): (p.read_bytes(), p.stat().st_mode)
            for p in root.rglob('*') if p.is_file()}


def run():
    return S.split('0x8800', 0x8840, 'rom/sdk', 'suffix')


def test_real_transaction_preserves_locks_provenance_slots_and_context(repo):
    original_lock = (repo / 'matched.lock.json').read_bytes()
    original_body = lock.body_sha(repo / 'src/rom/old.c', 'f_b')
    original_mapping = L.derive()
    result = run()
    assert result['coverage_added'] == 0 and result['native_bytes'] == 128
    assert (repo / 'matched.lock.json').read_bytes() == original_lock
    assert lock.body_sha(repo / 'src/rom/old.c', 'f_b') == original_body
    assert not lock.check(lock.load_lock(repo / 'matched.lock.json'), repo)
    suffix = (repo / 'src/rom/old.c').read_text()
    prefix = (repo / 'src/rom/sdk.c').read_text()
    assert 'Evidence: original lock and ROM gate' in suffix
    assert 'Context notice retained' in prefix and 'Context notice retained' in suffix
    assert 'f_a' not in suffix and 'f_b' not in prefix
    assert 'asm/us/nonmatchings/rom/sdk/f_a.s' in prefix
    assert not (repo / 'asm/us/nonmatchings/rom/old/f_a.s').exists()
    assert (repo / 'asm/us/nonmatchings/rom/sdk/f_a.s').is_file()
    assert [(n,a,b) for _,n,a,b in S._geometry(L.derive())] == [
        (n,a,b) for _,n,a,b in S._geometry(original_mapping)]
    segments = [s for s in L.derive()['segments'] if s['functions']]
    assert [f['state'] for s in segments for f in s['functions']] == ['passthrough','promoted']
    assert segments[1]['flagset'] == '-g0 -O2 -mips2 -G 0 -non_shared'
    assert 'rom/sdk' in (repo / 'rush2049.us.ld').read_text()


@pytest.mark.parametrize('boundary', [0x8800, 0x8880, 0x8844, 0x8820, '0x8840'])
def test_bad_geometry_is_atomic(repo, boundary):
    before = files(repo)
    with pytest.raises(S.SplitRefusal): S.split('0x8800', boundary, 'rom/sdk')
    assert files(repo) == before


@pytest.mark.parametrize('name', ['rom/old', '../outside', 'rom/nested/sdk', 'sdk', 'rom/a-b'])
def test_bad_or_colliding_tu_is_atomic(repo, name):
    before = files(repo)
    with pytest.raises(S.SplitRefusal): S.split('0x8800', 0x8840, name)
    assert files(repo) == before


@pytest.mark.parametrize('surface', ['src/rom/sdk.c', 'asm/us/nonmatchings/rom/sdk'])
def test_destination_file_or_directory_collision(repo, surface):
    dest = repo / surface
    if surface.endswith('.c'): dest.write_text('user data')
    else: dest.mkdir()
    before = files(repo)
    with pytest.raises(S.SplitRefusal, match='already exists'): run()
    assert files(repo) == before


def test_dirty_surface_refuses(repo, monkeypatch):
    monkeypatch.setattr(L, '_dirty', lambda paths: ' M src/rom/old.c')
    before = files(repo)
    with pytest.raises(S.SplitRefusal, match='dirty'): run()
    assert files(repo) == before


@pytest.mark.parametrize('content', ['int storage = 0;\n', 'typedef int Private;\n',
                                     '#define SHAPE 1\n', 'void helper(void);\n'])
def test_unsupported_top_level_content_refuses(repo, content):
    path = repo / 'src/rom/old.c'
    path.write_text(path.read_text() + content)
    before = files(repo)
    with pytest.raises(S.SplitRefusal, match='unsupported'): run()
    assert files(repo) == before


def test_body_drift_refuses(repo):
    path = repo / 'src/rom/old.c'
    path.write_text(path.read_text().replace('return 1;', 'return 2;'))
    before = files(repo)
    with pytest.raises(S.SplitRefusal, match='invalid'): run()
    assert files(repo) == before


def test_promoted_source_cannot_change_lock_path(repo):
    before = files(repo)
    with pytest.raises(S.SplitRefusal, match='locked source path'):
        S.split('0x8800', 0x8840, 'rom/sdk', 'prefix')
    assert files(repo) == before


@pytest.mark.parametrize('family', ['storage_blocks','rom_slots','text_boundaries'])
def test_owned_tu_refuses_before_extraction(repo, family):
    registry = {'schema':1,'rom_slots':[],'storage_blocks':[],family:[{'tu':'src/rom/old.c'}]}
    (repo / 'rom_owned_data.json').write_text(json.dumps(registry))
    before = files(repo)
    with pytest.raises(S.SplitRefusal, match='owns or pins'): run()
    assert files(repo) == before


def test_other_owned_text_range_overlap_refuses(repo):
    (repo / 'rom_owned_data.json').write_text(json.dumps({'schema':1,'text_boundaries':[
        {'tu':'src/rom/elsewhere.c','rom_start':'0x8820','next_rom_start':'0x8860'}]}))
    before = files(repo)
    with pytest.raises(S.SplitRefusal, match='overlaps'): run()
    assert files(repo) == before


def test_source_order_refuses(repo):
    path = repo / 'src/rom/old.c'
    text = path.read_text().replace('f_a.s','unknown.s')
    path.write_text(text)
    before = files(repo)
    with pytest.raises(S.SplitRefusal, match='ordering'): run()
    assert files(repo) == before


@pytest.mark.parametrize('failure', ['extract','geometry','authority','locks','override','interrupt'])
def test_failures_restore_every_extraction_surface(repo, monkeypatch, failure):
    before = files(repo)
    normal = L._run_extract
    def fail():
        normal()
        (repo / 'assets/us/data.bin').write_bytes(b'changed neighbors')
        (repo / 'src/rom/neighbor.c').write_text('new source')
        (repo / '.splat_cache').write_text('new cache')
        (repo / 'undefined_funcs_auto.us.txt').write_text('new symbols')
        if failure == 'extract': raise SystemExit('extract failed')
        if failure == 'interrupt': raise KeyboardInterrupt()
        if failure == 'geometry':
            (repo / 'symbol_addrs.us.txt').write_text('bad symbols')
        if failure == 'authority':
            path = repo / 'asm/us/nonmatchings/rom/sdk/f_a.s'
            data = json.loads(path.read_text()); data['words'][0] = '12345678'
            path.write_text(json.dumps(data))
        if failure == 'locks': (repo / 'matched.lock.json').write_text('{}')
    monkeypatch.setattr(L, '_run_extract', fail)
    if failure == 'override':
        monkeypatch.setattr(L,'write_opt_overrides',lambda mapping: (_ for _ in ()).throw(RuntimeError('override')))
    with pytest.raises((S.SplitRefusal,SystemExit,KeyboardInterrupt,RuntimeError)): run()
    assert files(repo) == before


def test_valid_prefix_retention_preserves_original_lock_path(repo):
    path = repo / 'src/rom/old.c'
    text = path.read_text()
    text = text.replace('#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/old/f_a.s")',
                        '/* PROMOTED fixture — f_a */\nint f_a(void) { return 3; }')
    start = text.index('/* PROMOTED fixture — f_b')
    text = text[:start] + '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/old/f_b.s")\n'
    path.write_text(text)
    lock_path = repo / 'matched.lock.json'
    lock_path.write_text(json.dumps({'src/rom/old.c:f_a':{'target_id':'f_a',
        'flagset':'-g0 -O2 -mips2 -G 0 -non_shared','body_sha256':lock.body_sha(path,'f_a')}}))
    original_lock = lock_path.read_bytes()
    result = S.split('0x8800', 0x8840, 'rom/vi', 'prefix')
    assert result['prefix_tu'] == 'rom/old' and result['suffix_tu'] == 'rom/vi'
    assert lock_path.read_bytes() == original_lock
    assert 'f_a' in path.read_text() and 'f_b' not in path.read_text()
    assert not lock.check(lock.load_lock(lock_path),repo)


def test_duplicate_subsegment_offsets_refuse(repo):
    path = repo / 'splat.us.yaml'
    path.write_text(path.read_text().replace('  - type: bin',
        '      - [0x8800, asm]\n  - type: bin'))
    before = files(repo)
    with pytest.raises(S.SplitRefusal): run()
    assert files(repo) == before


def test_missing_generated_export_restores_transaction(repo, monkeypatch):
    normal = L._run_extract
    def missing():
        normal()
        (repo / 'asm/us/nonmatchings/rom/sdk/f_a.s').unlink()
    monkeypatch.setattr(L, '_run_extract', missing)
    before = files(repo)
    with pytest.raises(S.SplitRefusal): run()
    assert files(repo) == before


def test_symlink_output_surface_refuses_before_mutation(repo):
    outside = repo.parent / 'outside-source'
    outside.write_text('outside source')
    (repo / 'src/rom/unowned.c').symlink_to(outside)
    before = files(repo)
    with pytest.raises(S.SplitRefusal, match='symlink'): run()
    assert files(repo) == before
    assert outside.read_text() == 'outside source'


def test_storage_context_pin_refuses_affected_tu(repo):
    (repo / 'rom_owned_data.json').write_text(json.dumps({'schema':1,'storage_blocks':[
        {'tu':'src/rom/elsewhere.c','context_files':[{'path':'src/rom/old.c'}]}]}))
    before = files(repo)
    with pytest.raises(S.SplitRefusal, match='owns or pins'): run()
    assert files(repo) == before


@pytest.mark.parametrize('extra', ['#include "storage.h"\n', '#include "rom_tu.h"\n'])
def test_extra_context_include_is_atomic_refusal(repo, extra):
    path = repo / 'src/rom/old.c'
    path.write_text(path.read_text().replace('#include "rom_tu.h"\n', '#include "rom_tu.h"\n'+extra))
    before = files(repo)
    with pytest.raises(S.SplitRefusal, match='one standard'): run()
    assert files(repo) == before


def test_provenance_state_drift_restores_even_with_unchanged_body(repo, monkeypatch):
    normal = L._run_extract
    def drift():
        normal()
        path = repo / 'src/rom/old.c'
        path.write_text(path.read_text().replace('PROMOTED fixture — f_b', 'Historical body f_b'))
    monkeypatch.setattr(L, '_run_extract', drift)
    before = files(repo)
    with pytest.raises(S.SplitRefusal, match='coverage/provenance'): run()
    assert files(repo) == before


@pytest.mark.parametrize('family,row', [
    ('rom_slots', {'tu':'src/rom/foreign.c','container_vram':'0x80007c00','offset':32,'size':32}),
    ('storage_blocks', {'tu':'src/rom/foreign.c','vram_start':'0x80007c20','size':32}),
    ('storage_blocks', {'tu':'src/rom/foreign.c','data_slot':{'container_vram':'0x80007c00','offset':32,'size':32}}),
])
def test_foreign_tu_physical_ownership_overlap_refuses(repo, family, row):
    registry={'schema':1,'rom_slots':[],'storage_blocks':[],family:[row]}
    (repo / 'rom_owned_data.json').write_text(json.dumps(registry))
    before = files(repo)
    with pytest.raises(S.SplitRefusal, match='physical owned range'): run()
    assert files(repo) == before


def test_linker_sidecar_and_build_outputs_restore_on_failure(repo, monkeypatch):
    sidecar=repo/'rush2049.us.ld.d';sidecar.write_text('original dependencies')
    build=repo/'build/us';build.mkdir();(build/'owner.d').write_text('original object deps')
    before=files(repo)
    def fail():
        sidecar.write_text('changed dependencies')
        (repo/'rush2049.us.ld_sections.txt').write_text('new sections')
        (build/'owner.d').write_text('changed object deps')
        (build/'new.d').write_text('new object deps')
        raise RuntimeError('extract sidecars')
    monkeypatch.setattr(L,'_run_extract',fail)
    with pytest.raises(RuntimeError):run()
    assert files(repo)==before


def test_unrelated_text_owner_uses_real_extent_schema(repo):
    registry={'schema':1,'text_boundaries':[{'tu':'src/rom/foreign.c',
        'rom_start':'0x9000','next_rom_start':'0x9040',
        'vram_start':'0x80008400','extent_bytes':64}]}
    (repo/'rom_owned_data.json').write_text(json.dumps(registry))
    before=files(repo)
    S._reject_owned(repo,'rom/old','rom/sdk',0x8800,0x8880)
    assert files(repo)==before
