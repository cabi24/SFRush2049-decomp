"""Publication must preserve old promotions and roll back database failures."""
from subprocess import CompletedProcess
from types import SimpleNamespace

import pytest

from tools.conveyor.coordinator import db
from tools.conveyor.pipeline import existing_storage as E


@pytest.fixture
def publication(tmp_path, monkeypatch):
    conn = db.connect(tmp_path / 'conveyor.db')
    conn.execute("INSERT INTO promotion_record (promotion_id,target_id,source_sha,outcome,created_at) VALUES ('old','old','original','promoted','before')")
    conn.commit()
    row = {'new_members': ['new'], 'owner': 'timer_services', 'tu': 'src/rom/timer.c',
           'source': 'candidate.c', 'flags': '-O1', 'source_sha256': 'complete'}
    calls = []
    monkeypatch.setattr(E.lock, 'body_sha', lambda *a: 'body')
    def run(command, **kwargs):
        calls.append(command)
        output = 'SHA-1 EXACT' if command[-1] == 'rom' else ''
        if command[:3] == ['git', 'ls-files', '-z']:
            output = 'src/rom/timer.c\0'
        return CompletedProcess(command, 0, output, '')
    monkeypatch.setattr(E.promote, '_run', run)
    yield conn, tmp_path, row, calls, run
    conn.close()


def test_success_publishes_only_new_member_and_tracked_package(publication):
    conn, repo, row, calls, _ = publication
    E.publish(conn, repo, row, True, [repo / row['tu'], repo / 'baserom.us.z64'])
    records = {r['target_id']: dict(r) for r in conn.execute('SELECT * FROM promotion_record')}
    assert records['old']['source_sha'] == 'original'
    assert records['new']['source_sha'] == 'body'
    assert records['new']['build_ok'] == records['new']['sha1_ok'] == 1
    commit = calls[-1]
    assert commit[:2] == ['git', 'commit']
    assert commit[commit.index('--') + 1:] == ['src/rom/timer.c']


def test_commit_failure_rolls_back_new_promotion(publication, monkeypatch):
    conn, repo, row, calls, run = publication
    def fail(command, **kwargs):
        if command[:2] == ['git', 'commit']:
            return CompletedProcess(command, 1, '', 'commit rejected')
        return run(command, **kwargs)
    monkeypatch.setattr(E.promote, '_run', fail)
    with pytest.raises(E.promote.Refusal, match='publication failed'):
        E.publish(conn, repo, row, True, [repo / row['tu']])
    assert [(r['target_id'], r['source_sha']) for r in conn.execute('SELECT * FROM promotion_record')] == [('old', 'original')]


@pytest.mark.parametrize('failure', ['exit', 'missing_hash'])
def test_failed_rom_gate_never_publishes(publication, monkeypatch, failure):
    conn, repo, row, calls, run = publication
    monkeypatch.setattr(E.promote, '_run', lambda cmd, **kw: CompletedProcess(cmd, int(failure == 'exit'), 'build succeeded', ''))
    with pytest.raises(E.promote.Refusal):
        E.publish(conn, repo, row, True, [repo / row['tu']])
    assert conn.execute('SELECT count(*) FROM promotion_record').fetchone()[0] == 1


def test_named_extent_excludes_section_alignment(tmp_path, monkeypatch):
    conn = db.connect(tmp_path / 'conveyor.db')
    conn.execute("INSERT INTO n64_target (target_id,address,population,insn_count,target_o_sha) VALUES ('timer',4096,'static',8,'original')")
    monkeypatch.setattr(E.targets, 'index_asm_regions', lambda: {'region': SimpleNamespace(name='timer', words=[0] * 6, vaddr=4096)})
    assert E.trusted_targets(conn, ['timer']) == {'timer': ('original', 6)}
    conn.execute("UPDATE n64_target SET population='game'")
    with pytest.raises(E.promote.Refusal, match='authoritative'):
        E.trusted_targets(conn, ['timer'])
    conn.close()
