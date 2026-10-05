"""Scoped regression tests for the fresh resource-initializer NONMATCH packet."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_resource_initializer'
spec = importlib.util.spec_from_file_location('fresh_resource_initializer', HERE / 'verify.py')
packet = importlib.util.module_from_spec(spec)
spec.loader.exec_module(packet)


def test_receipt_binds_complete_nonmatch_source():
    receipt = json.loads((HERE / 'verification.json').read_text())
    assert receipt['source_sha256'] == hashlib.sha256((HERE / 'candidate.c').read_bytes()).hexdigest()
    assert receipt['host_source_sha256'] == hashlib.sha256((HERE / 'host.c').read_bytes()).hexdigest()
    assert receipt['status'] == 'NONMATCH' and receipt['claims'] == []
    assert receipt['accepted_byte_gain'] == 0
    assert receipt['comparison']['differing'] == 12
    assert receipt['comparison']['total'] == 92
    assert receipt['comparison']['elf_function_bytes'] == receipt['candidate_text_bytes'] == 368
    assert receipt['candidate_alignment_bytes'] == 0
    assert receipt['independent_gnu_link_agrees'] and receipt['full_body_relocations_verified']
    for field in ('unresolved', 'unverified', 'errors'):
        assert receipt['comparison'][field] == []


def test_genuine_callee_control_preserves_arity_caveat():
    receipt = json.loads((HERE / 'verification.json').read_text())
    group = receipt['controls']['accepted_arity_group']
    assert group['caller']['differing'] == 59
    assert group['caller']['elf_function_bytes'] == 360
    assert group['read_only_callee']['differing'] == 0
    assert group['read_only_callee']['elf_function_bytes'] == 364
    assert 'fifth' in receipt['abi_caveat'] and 'four-parameter' in receipt['abi_caveat']


def test_pointer_host_fixture_sanitized(tmp_path):
    if not shutil.which('cc'):
        pytest.skip('host C compiler unavailable')
    exe = tmp_path / 'host'
    subprocess.run(['cc', '-std=c99', '-O1', '-g', '-Wall', '-Wextra', '-Werror',
                    '-fsanitize=undefined', '-fno-sanitize-recover=all', '-DHOST_MAIN',
                    str(HERE / 'host.c'), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)


def test_fresh_full_body_link_and_differential_replay(tmp_path):
    ido = Path(os.environ.get('IDO_DIR', ROOT / 'tools/cloud/ido'))
    if not (ido / 'cc').exists() or not all(shutil.which(x) for x in
                                          ['cc', 'mips-linux-gnu-ld', 'mips-linux-gnu-objcopy']):
        pytest.skip('pinned IDO/GNU MIPS toolchain unavailable')
    receipt = packet.verify(tmp_path)
    assert receipt['differential_cases'] == 13840
    assert receipt['sanitizer_cases'] == 100000
    assert receipt['comparison']['differing'] == 12
    assert receipt['native_frame_bytes'] == 88 and receipt['candidate_frame_bytes'] == 64
