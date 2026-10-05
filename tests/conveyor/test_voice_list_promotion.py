"""Source-only voice/list contract guards, including future promotion lifecycle."""
import copy
import importlib.util
from pathlib import Path
import shutil
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
WORK = ROOT / 'cloud/work/boot_tail_promotion/voice_lists'


def module(name):
    spec = importlib.util.spec_from_file_location('voice_lists_' + name, WORK / (name + '.py'))
    value = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = value
    spec.loader.exec_module(value)
    return value


VERIFY = module('verify')
SEMANTICS = module('semantics')


def toolchain():
    if not (VERIFY.score.IDO / 'cc').is_file():
        pytest.skip('IDO compiler absent')
    if not shutil.which('mips-linux-gnu-as') or not shutil.which('mips-linux-gnu-objdump'):
        pytest.skip('MIPS binutils absent')


@pytest.fixture(scope='module')
def proof(tmp_path_factory):
    toolchain()
    return VERIFY.verify(tmp_path_factory.mktemp('voice-lists'))


def test_whole_combined_tus_exact_extents_and_relocations(proof):
    assert proof['candidate_bytes'] == 1900
    assert len(proof['standalone']) == 8
    for tu, group in VERIFY.SCOPE.items():
        row = proof['translation_units'][tu]
        required = {'func_' + a for a in group['candidates'] + group['locked']}
        for fn in required:
            match = row['combined'][fn]
            assert match['strict_match']
            assert match['function_symbol_bytes'] == match['target_bytes']
            assert match['differing'] == match['extra_words'] == 0
            assert not (match['errors'] or match['unresolved'] or match['unverified'])
        assert row['allocated_data_bytes'] == 0
        assert row['all_function_offsets_unchanged']
        assert row['old_declaration_refusal'].startswith('redeclaration')
        for failed in row['negative_controls'].values():
            assert failed['differing'] > 0
            assert not (failed['errors'] or failed['unresolved'] or failed['unverified'])


@pytest.mark.parametrize('tu', VERIFY.SCOPE)
def test_future_valid_promotions_keep_the_replay_usable(tu, proof):
    path = Path('src/rom') / (tu + '.c')
    source = (ROOT / path).read_text()
    locks = copy.deepcopy(VERIFY.load_lock())
    pending, _, names = VERIFY.current_state(tu, source, locks)
    paths = {fn: ROOT / VERIFY.SOURCES / (fn + '.c') for fn in pending}
    promoted = VERIFY.splice(tu, source, paths, proof['header_context'])
    for fn, body in VERIFY.bodies(promoted).items():
        locks[str(path) + ':' + fn] = {'body_sha256': VERIFY.body_hash(body)}
    still_pending, current, population = VERIFY.current_state(tu, promoted, locks)
    assert not still_pending
    assert set(paths) <= set(current)
    assert population == names
    # The same replay now reads the accepted candidates; it does not splice a
    # second copy and still runs full objects/negative controls.
    assert VERIFY.splice(tu, promoted, {}, proof['header_context']) == promoted


@pytest.mark.parametrize('tu', VERIFY.SCOPE)
def test_missing_or_changed_current_lock_is_rejected(tu):
    path = Path('src/rom') / (tu + '.c')
    source = (ROOT / path).read_text()
    locks = copy.deepcopy(VERIFY.load_lock())
    fn = 'func_' + VERIFY.SCOPE[tu]['locked'][0]
    del locks[str(path) + ':' + fn]
    with pytest.raises(AssertionError, match='missing'):
        VERIFY.current_state(tu, source, locks)
    locks = VERIFY.load_lock()
    body = VERIFY.bodies(source)[fn]
    changed = source.replace(body, body.replace('{', '{ return 0;', 1), 1)
    with pytest.raises(AssertionError, match='original locked body changed'):
        VERIFY.current_state(tu, changed, locks)


@pytest.mark.parametrize('address', SEMANTICS.CASES)
def test_explicit_actual_adapted_source_semantics(address):
    if not shutil.which('cc'):
        pytest.skip('C compiler absent')
    result = SEMANTICS.run_case(address)
    assert result['source'] == str(VERIFY.SOURCES / ('func_' + address + '.c'))
    assert result['result'] == 'PASS'


def test_entire_replay_after_all_eight_valid_promotions(proof, tmp_path):
    overrides = {}
    locks = copy.deepcopy(VERIFY.load_lock())
    for tu in VERIFY.SCOPE:
        path = Path('src/rom') / (tu + '.c')
        source = (ROOT / path).read_text()
        pending, _, _ = VERIFY.current_state(tu, source, locks)
        paths = {fn: ROOT / VERIFY.SOURCES / (fn + '.c') for fn in pending}
        promoted = VERIFY.splice(tu, source, paths, proof['header_context'])
        overrides[tu] = promoted
        for fn, body in VERIFY.bodies(promoted).items():
            locks[str(path) + ':' + fn] = {'body_sha256': VERIFY.body_hash(body)}
    replay = VERIFY.verify(tmp_path, overrides, locks)
    for row in replay['translation_units'].values():
        assert not row['pending_candidates']
