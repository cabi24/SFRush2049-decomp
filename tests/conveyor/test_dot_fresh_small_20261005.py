"""Fresh proof regressions for two explicitly nonmatching research candidates."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil

import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_fresh_small_20261005'
spec = importlib.util.spec_from_file_location('dot_fresh_small', HERE / 'verify.py')
packet = importlib.util.module_from_spec(spec)
spec.loader.exec_module(packet)
# Protected manifests and the scorer are a frozen provenance snapshot, not
# locks on future work: asm/us/blob/SHA256SUMS changes with every game splice.
FROZEN_PROVENANCE = ('target_manifest_sha256',)


def test_nonmatch_receipt_is_bound_to_complete_sources():
    saved = json.loads((HERE / 'verification.json').read_text())
    assert saved['status'] == 'NONMATCH' and saved['claims'] == []
    assert saved['accepted_byte_gain'] == 0
    for name, residual in [('func_800DC628', 20), ('func_800D4DFC', 12)]:
        proof = saved['targets'][name]
        assert proof['source_sha256'] == hashlib.sha256((HERE / (name + '.c')).read_bytes()).hexdigest()
        assert proof['comparison']['elf_function_bytes'] == 248
        assert proof['comparison']['differing'] == residual
        assert len(proof['differing_word_offsets']) == residual
        assert proof['independent_gnu_link_verified']
        for field in ['unresolved', 'unverified', 'errors']:
            assert proof['comparison'][field] == []
        assert proof['comparison']['extra_words'] == 0
        assert proof['alignment_bytes'] == 8


def test_real_accepted_callee_and_host_limits_are_explicit():
    saved = json.loads((HERE / 'verification.json').read_text())
    context = saved['accepted_context']
    assert context['source_sha256'] == hashlib.sha256((ROOT / 'src/blob/math_utility.c').read_bytes()).hexdigest()
    assert context['math_utility']['differing'] == 0
    assert context['math_utility']['elf_function_bytes'] == 76
    assert context['caller']['differing'] == 12
    host = saved['host_semantics']
    assert host['packing_cases'] == 4608 and host['snapshot_cases'] == 4096
    assert host['asan_ubsan'] == 'passed'
    assert set(host['negative_controls'].values()) == {'rejected_by_semantic_assertion'}
    assert 'not native execution' in host['limit']


def test_fresh_source_extent_relocation_and_semantic_replay(tmp_path):
    ido = Path(os.environ.get('IDO_DIR', ROOT / 'tools/cloud/ido'))
    available = (ido / 'cc').exists() and all(shutil.which(tool) for tool in
                 ['cc', 'mips-linux-gnu-ld', 'mips-linux-gnu-objcopy'])
    if not available:
        if os.environ.get('REQUIRE_TOOLCHAIN') == '1':
            pytest.fail('required IDO/GNU MIPS/host toolchain unavailable')
        pytest.skip('IDO/GNU MIPS/host toolchain unavailable')
    fresh = packet.verify(tmp_path)
    saved = json.loads((HERE / 'verification.json').read_text())
    for key in FROZEN_PROVENANCE:
        fresh.pop(key, None)
        saved.pop(key, None)
    assert fresh == saved
