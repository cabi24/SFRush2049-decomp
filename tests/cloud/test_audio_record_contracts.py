"""Explicit CI for the adapted audio sources and post-promotion lifecycle."""
import copy
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

from tools.cloud import score

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/boot_tail_promotion/audio_record_contracts'
SPEC = importlib.util.spec_from_file_location('audio_record_contract_verifier', PACKET / 'verify.py')
V = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(V)


def fixture_inputs():
    text = (ROOT / V.TU).read_text()
    locks = json.loads((ROOT / 'matched.lock.json').read_text())
    sources = {n:(PACKET/'sources'/(n+'.c')).read_text() for n in V.CANDIDATES}
    # Fast lifecycle fixtures use full source preambles; this is source testing,
    # not the independently context-checked compiler splice used by the proof.
    contexts = {n:{'preamble':[source[:source.index(V.function_body(source,n))]]}
                for n,source in sources.items()}
    return text, locks, sources, contexts


def promote_fixture(text, locks, sources, contexts, names):
    accepted = V.accepted_functions(text, locks)
    selected = {n:sources[n] for n in names}
    if V.DEFERRED in selected:
        text = V.adapted_cleanup(text, (PACKET/'sources'/(V.CLEANUP+'.c')).read_text())
        locks[V.TU+':'+V.CLEANUP]['body_sha256'] = V.sha(V.normalize_body(V.function_body(text,V.CLEANUP)).encode())
    text = V.splice(text, selected, contexts, accepted)
    for name in names:
        for key in list(locks):
            if key.endswith(':'+name):
                del locks[key]
        locks[V.TU+':'+name] = {'body_sha256':V.sha(V.normalize_body(V.function_body(text,name)).encode())}
    return text, locks


@pytest.mark.parametrize('names', [(), ('func_80011C84',),
    tuple(n for n in V.CANDIDATES if n != V.DEFERRED), V.CANDIDATES])
def test_accepts_legitimate_partial_and_complete_promotion(names):
    text, locks, sources, contexts = fixture_inputs()
    text, locks = promote_fixture(text, copy.deepcopy(locks), sources, contexts, names)
    accepted = V.accepted_functions(text, locks)
    assert set(names) <= set(accepted)
    assert V.splice(text, {n:sources[n] for n in names}, contexts, accepted) == text
    # Pending candidates remain coverable after other slots are promoted.
    remaining = {n:sources[n] for n in V.CANDIDATES if n != V.DEFERRED and n not in accepted}
    combined = V.splice(text, remaining, contexts, accepted)
    assert all(V.function_body(combined,n) for n in set(names)|set(remaining))


def test_missing_migrated_lock_fails_closed():
    text, locks, sources, contexts = fixture_inputs()
    name = 'func_80011C84'
    text, locks = promote_fixture(text, locks, sources, contexts, [name])
    del locks[V.TU+':'+name]
    with pytest.raises(V.VerificationError, match='missing pending slot/current lock'):
        V.splice(text, {name:sources[name]}, contexts, V.accepted_functions(text, locks))


def test_changed_locked_body_fails_closed():
    text, locks, sources, contexts = fixture_inputs()
    name = 'func_80011C84'
    text, locks = promote_fixture(text, locks, sources, contexts, [name])
    text = text.replace('audio->active = 1;', 'audio->active = 0;')
    with pytest.raises(V.VerificationError, match='current body lock differs'):
        V.accepted_functions(text, locks)


@pytest.mark.parametrize('actual_size', [36, 44])
def test_exact_extent_rejects_short_or_long_function(actual_size):
    symbols = {'func_800149DC':{'size':actual_size,'value':0}, 'next':{'size':4,'value':100}}
    with pytest.raises(V.VerificationError, match='extent differs'):
        V.exact_extent(symbols, 'func_800149DC', 40)


def test_exact_extent_rejects_neighbor_overlap():
    symbols = {'one':{'size':40,'value':0}, 'next':{'size':4,'value':36}}
    with pytest.raises(V.VerificationError, match='extends into its neighbor'):
        V.exact_extent(symbols, 'one', 40)


def test_actual_tu_audio_record_contracts(tmp_path):
    if not (score.IDO/'cc').is_file():
        pytest.skip('IDO unavailable')
    for tool in ('mips-linux-gnu-as','mips-linux-gnu-ld','mips-linux-gnu-objcopy','mips-linux-gnu-readelf','mips-linux-gnu-objdump'):
        if not shutil.which(tool):
            pytest.skip(tool + ' unavailable')
    result = subprocess.run([sys.executable,str(PACKET/'verify.py'),'--out',str(tmp_path)],
                            cwd=ROOT, capture_output=True, text=True, timeout=180)
    assert result.returncode == 0, result.stdout + result.stderr
    report = json.loads(result.stdout)
    assert report['result'] == 'PASS'
    assert report['baseline_existing_locked_functions'] == 35
    assert report['independent_candidates'] == 11
    assert report['independent_candidate_bytes'] == 1388
    assert report['cleanup_dependent_bytes'] == 112
    assert report['layout']['record_size'] == 104
    assert len(report['standalone']) == 13
    assert all(s['all_tu_slots_verified'] == 75 for s in report['stages'].values())
    assert len({s['linked_text_sha256'] for s in report['stages'].values()}) == 1
    assert report['negative_controls']['wrong_buffer_rejected']
    assert report['negative_controls']['wrong_stride_rejected']


def test_bad_promoted_body_with_fresh_fixture_lock_still_fails(tmp_path):
    if not (score.IDO/'cc').is_file():
        pytest.skip('IDO unavailable')
    for tool in ('mips-linux-gnu-as','mips-linux-gnu-ld','mips-linux-gnu-objcopy'):
        if not shutil.which(tool):
            pytest.skip(tool + ' unavailable')
    text, locks, sources, contexts = fixture_inputs()
    name = 'func_80011C84'
    text, locks = promote_fixture(text, locks, sources, contexts, [name])
    text = text.replace('audio->active = 1;', 'audio->active = 2;')
    locks[V.TU+':'+name]['body_sha256'] = V.sha(V.normalize_body(V.function_body(text,name)).encode())
    accepted = V.accepted_functions(text, locks)
    assert name in accepted
    assert V.splice(text, {name:sources[name]}, contexts, accepted) == text
    old = score.ASM_DIR, score._targets, score._target_fingerprint
    try:
        score.ASM_DIR = ROOT/'asm/us/boot_tail'
        obj = V.compile_tu(text, tmp_path, 'incorrect_promoted_body')
        with pytest.raises(V.VerificationError, match='differ'):
            V.check_object(obj, [name], tmp_path, score.targets(), score.image_symbols())
    finally:
        score.ASM_DIR, score._targets, score._target_fingerprint = old
