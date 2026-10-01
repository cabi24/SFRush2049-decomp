"""Synthetic objects only; no extracted images or coordinator mutation."""
import io
import json
from pathlib import Path
import shutil
import sqlite3
import subprocess
import tarfile
from types import SimpleNamespace

import pytest

from tools.conveyor.pipeline import diagnose


def document():
    return {'comparison': {'symbol': 'fn', 'word_mismatches': 2,
                           'raw_word_mismatches': 3, 'target_instructions': 8,
                           'candidate_instructions': 9},
            'view': {'symbol': 'fn', 'verdict': 'allocation-mismatch',
                     'target_frame_size': 24, 'candidate_frame_size': 32,
                     'lanes': [{'class': 'temp', 'slot': 1, 'rotation': 4}]},
            'owning_pass': 'ugen-temp-ring', 'ownership_basis': 'heuristic',
            'lever': {'lever_class': 'source-edit', 'edit_family': 'temp-lifetime'}}


def test_summary_preserves_counts_and_ownership():
    result = diagnose.summary(document())
    assert result['words_differing'] == 2
    assert result['strict_words_differing'] == 3
    assert result['frame_delta'] == 8
    assert result['owning_pass'] == 'ugen-temp-ring'
    assert result['ownership_basis'] == 'heuristic'
    assert result['lever'] == 'temp-lifetime'
    assert result['lanes'] == [{'class': 'temp', 'slot': 1, 'rotation': 4}]


def test_summary_missing_frame_and_exact_lever():
    data = document()
    del data['lever']
    data['view']['candidate_frame_size'] = None
    result = diagnose.summary(data)
    assert result['frame_delta'] is None
    assert result['lever'] == 'none'


def test_batches():
    assert list(diagnose.batches(list(range(5)), 2)) == [[0, 1], [2, 3], [4]]
    with pytest.raises(ValueError):
        list(diagnose.batches([1], 0))


def tar_bytes(entries):
    stream = io.BytesIO()
    with tarfile.open(fileobj=stream, mode='w') as archive:
        for name, data in entries.items():
            info = tarfile.TarInfo(name)
            info.size = len(data)
            archive.addfile(info, io.BytesIO(data))
    return stream.getvalue()


def test_compile_one_roundtrip_mixed_flags_and_failure(tmp_path):
    calls = []
    def runner(argv, **kwargs):
        calls.append(argv)
        with tarfile.open(fileobj=io.BytesIO(kwargs['input'])) as archive:
            manifest = json.load(archive.extractfile('manifest.json'))
            assert manifest['jobs'] == [{'function': 'fn', 'flags': ['-O2', '-G', '0']},
                                        {'function': 'bad', 'flags': ['-O1']}]
            assert archive.extractfile('fn.c').read() == b'int fn(void){return 1;}'
        return SimpleNamespace(returncode=0, stdout=tar_bytes({
            'failures.json': b'{"bad":"syntax error"}', 'fn.o': b'synthetic'}), stderr=b'')
    objects, failures = diagnose.compile_batch([
        {'function': 'fn', 'source': 'int fn(void){return 1;}', 'flags': '-O2 -G 0'},
        {'function': 'bad', 'source': 'invalid', 'flags': '-O1'}], tmp_path, runner=runner)
    assert len(calls) == 1
    assert calls[0][0] == 'ssh'
    assert objects['fn'].read_bytes() == b'synthetic'
    assert failures == {'bad': 'syntax error'}
    assert not (tmp_path / 'bad.o').exists()


def test_builder_failure_is_not_empty_success(tmp_path):
    def runner(*args, **kwargs):
        return SimpleNamespace(returncode=255, stdout=b'', stderr=b'unreachable')
    with pytest.raises(RuntimeError, match='unreachable'):
        diagnose.compile_batch([{'function': 'fn', 'source': '', 'flags': '-O2'}], tmp_path, runner=runner)


def test_reject_shell_function_name(tmp_path):
    with pytest.raises(ValueError, match='invalid function'):
        diagnose.compile_batch([{'function': '../fn', 'source': '', 'flags': ''}], tmp_path)


def test_markdown_stable_order_counts_and_escaping():
    rows = [dict(diagnose.summary(document()), function='z'),
            dict(diagnose.summary(document()), function='a', words_differing=0, lever='a|b'),
            {'function': 'broken', 'verdict': 'diagnosis-failed'}]
    markdown = diagnose.render_verdicts(rows, 'abc123', '2026-10-01')
    assert markdown == diagnose.render_verdicts(list(reversed(rows)), 'abc123', '2026-10-01')
    assert markdown.index('| a |') < markdown.index('| z |') < markdown.index('| broken |')
    assert 'a\\|b' in markdown
    assert '- allocation-mismatch: 2' in markdown
    assert '- diagnosis-failed: 1' in markdown


def assemble(tmp_path, name, body):
    if not shutil.which('mips-linux-gnu-as') or not shutil.which('mips-linux-gnu-objdump'):
        pytest.skip('MIPS binutils unavailable')
    source, obj = tmp_path / (name + '.s'), tmp_path / (name + '.o')
    source.write_text('.text\n.set noreorder\n.globl fn\n.ent fn\nfn:\n' + body + '\n.end fn\n')
    subprocess.run(['mips-linux-gnu-as', '-EB', '-mips2', '-o', str(obj), str(source)], check=True, capture_output=True)
    return obj


def test_synthetic_objects_workbench_end_to_end(tmp_path):
    target = assemble(tmp_path, 'target', 'jr $ra\naddiu $v0,$zero,1')
    candidate = assemble(tmp_path, 'candidate', 'jr $ra\naddiu $v0,$zero,2')
    result = diagnose.summary(diagnose.run_workbench(target, candidate, 'fn', 'mips-linux-gnu-objdump'))
    assert result['words_differing'] == 1
    assert result['strict_words_differing'] == 1
    assert result['verdict'] == 'constant'
    exact = diagnose.summary(diagnose.run_workbench(target, target, 'fn', 'mips-linux-gnu-objdump'))
    assert exact['words_differing'] == 0


def test_diagnose_jobs_batches_errors_and_json(tmp_path, monkeypatch):
    target = assemble(tmp_path, 'target', 'jr $ra\nnop')
    groups = []
    def compile_batch(jobs, output, builder):
        groups.append([job['function'] for job in jobs])
        return {job['function']: target for job in jobs if job['function'] != 'bad'}, {'bad': 'syntax error'}
    monkeypatch.setattr(diagnose, 'compile_batch', compile_batch)
    # Use fn for the actual symbol, isolate failure on a separate name.
    jobs = [{'function': name, 'source': '', 'origin': 'fixture', 'flags': '-O2',
             'target': str(target), 'source_sha256': 'fixture', 'target_sha256': 'fixture'}
            for name in ['fn', 'bad']]
    results = diagnose.diagnose_jobs(jobs, tmp_path / 'reports', 'builder', 'mips-linux-gnu-objdump', 1)
    assert groups == [['fn'], ['bad']]
    assert results[0]['words_differing'] == 0
    assert results[1]['verdict'] == 'diagnosis-failed'
    assert json.loads((tmp_path / 'reports/bad.json').read_text())['error'] == 'syntax error'


def test_readstore_detects_corruption_and_never_creates_directory(tmp_path):
    store = diagnose.ReadStore(tmp_path / 'absent')
    assert store.get('a' * 64) is None
    assert not store.root.exists()
    store.root.mkdir()
    (store.root / ('a' * 64)).write_bytes(b'not the claimed hash')
    with pytest.raises(ValueError, match='SHA-256 mismatch'):
        store.get('a' * 64)
    assert store.get('../not-a-hash') is None


def test_remote_program_compiles_and_returns_objects(tmp_path):
    import os
    import shlex
    import sys
    target = assemble(tmp_path, 'template', 'jr $ra\nnop')
    compiler = tmp_path / 'rush2049/cache/toolkits' / diagnose.TOOLKIT / 'ido/cc'
    compiler.parent.mkdir(parents=True)
    compiler.write_text('#!/bin/sh\nwhile [ "$1" != "-o" ]; do shift; done\nshift\ncp ' + shlex.quote(str(target)) + ' "$1"\n')
    compiler.chmod(0o755)
    def runner(argv, **kwargs):
        assert argv[0] == 'ssh'
        return subprocess.run([sys.executable, '-c', diagnose.REMOTE],
                              env=dict(os.environ, HOME=str(tmp_path)), **kwargs)
    objects, failures = diagnose.compile_batch([
        {'function': 'fn', 'source': 'int fn(void){return 0;}', 'flags': '-O2'}],
        tmp_path / 'output', runner=runner)
    assert not failures
    assert objects['fn'].read_bytes() == target.read_bytes()


def test_source_selection_flags_and_readonly_database(tmp_path):
    import hashlib
    blob = b'synthetic-target'
    sha = hashlib.sha256(blob).hexdigest()
    (tmp_path / sha).write_bytes(blob)
    db = tmp_path / 'test.db'
    with sqlite3.connect(db) as conn:
        conn.executescript('CREATE TABLE n64_target(target_id TEXT, target_o_sha TEXT);'
                           'CREATE TABLE function_status(target_id TEXT, flagset TEXT);')
        conn.execute('INSERT INTO n64_target VALUES (?,?)', ('fn', sha))
        conn.execute('INSERT INTO function_status VALUES (?,?)', ('fn', '-O1'))
    source = tmp_path / 'source.c'
    source.write_text('/* flags: -O2 -G 0 */\nint fn(void){return 0;}\n')
    before = db.read_bytes()
    with sqlite3.connect(db.as_uri() + '?mode=ro', uri=True) as conn:
        conn.row_factory = sqlite3.Row
        jobs = diagnose.select_jobs(conn, diagnose.ReadStore(tmp_path), 'fn', source)
        assert jobs[0]['flags'] == '-O2 -G 0'
        jobs = diagnose.select_jobs(conn, diagnose.ReadStore(tmp_path), 'fn', source, '-O3')
        assert jobs[0]['flags'] == '-O3'
    assert db.read_bytes() == before
