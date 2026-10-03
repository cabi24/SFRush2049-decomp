"""Protected-path checks use base-branch evidence, including group sources."""
import json
import sys

import pytest

from tools.cloud import check_submissions as ci, guard_paths as guard


def _write(repo, name, content):
    path = repo / name
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content)
    return path


def _commit(repo):
    ci.git(repo, "add", ".")
    ci.git(repo, "-c", "user.name=Test", "-c", "user.email=test@example.com",
           "commit", "-qm", "fixture")
    return ci.commit(repo, "HEAD")


@pytest.fixture
def repository(tmp_path):
    ci.git(tmp_path, "init", "-q")
    _write(tmp_path, "blob_matched.lock.json", json.dumps({
        "single": {"source": "src/blob/single.c"},
        "member": {"source": "src/blob/groups/example/group.json", "group": "example"},
    }))
    _write(tmp_path, "src/blob/groups/example/group.json", json.dumps({
        "files": ["member.c", "context.c"], "members": ["member"]}))
    for path in ("src/blob/single.c", "src/blob/groups/example/member.c",
                 "src/blob/groups/example/context.c"):
        _write(tmp_path, path, "/* locked */\n")
    _write(tmp_path, "asm/us/blob/example.s", ".word 0x03E00008\n")
    return tmp_path, _commit(tmp_path)


@pytest.mark.parametrize("path", [
    "asm/us/blob/example.s", "asm/us/blob/symbols.json", "asm/us/blob/SHA256SUMS",
    "asm/us/ovl_a/ovl_a_8038a400.s", "asm/us/ovl_b/extents.json",
    "asm/us/boot_tail/SHA256SUMS", "src/blob/blob.ld", "blob_matched.lock.json", "nested/example.lock.json",
    "us.sha1", "src/blob/single.c", "src/blob/groups/example/group.json",
    "src/blob/groups/example/member.c", "src/blob/groups/example/context.c",
])
def test_protects_ground_truth_and_all_locked_group_inputs(repository, path):
    repo, base = repository
    assert guard.violations(repo, [path], base) == [path]


def test_allows_submissions_unlocked_sources_and_documentation(repository):
    repo, base = repository
    assert guard.violations(repo, [
        "cloud/matches/single.c", "cloud/work/ipa-groups/new/group.c",
        "src/blob/unlocked.c", "CloudHandoff.md", "tools/cloud/score.py",
    ], base) == []


def test_pr_cannot_remove_protection_by_rewriting_the_lock_or_group(repository):
    repo, base = repository
    _write(repo, "blob_matched.lock.json", "{}")
    _write(repo, "src/blob/groups/example/group.json", '{"files": []}')
    _write(repo, "src/blob/single.c", "changed")
    _write(repo, "src/blob/groups/example/context.c", "changed")
    head = _commit(repo)
    blocked = guard.violations(repo, ci.changed_paths(repo, base, head), base)
    assert blocked == ["blob_matched.lock.json", "src/blob/groups/example/context.c",
                       "src/blob/groups/example/group.json", "src/blob/single.c"]


def test_deleting_and_moving_locked_files_are_changes(repository):
    repo, base = repository
    (repo / "src/blob/single.c").unlink()
    (repo / "src/blob/groups/example/member.c").rename(repo / "moved.c")
    head = _commit(repo)
    assert guard.violations(repo, ci.changed_paths(repo, base, head), base) == [
        "src/blob/groups/example/member.c", "src/blob/single.c"]


def test_altered_target_word_fails_the_cli(repository, monkeypatch, capsys):
    repo, base = repository
    _write(repo, "asm/us/blob/example.s", ".word 0xDEADBEEF\n")
    _commit(repo)
    monkeypatch.setattr(guard, "REPO", repo)
    monkeypatch.setattr(sys, "argv", ["guard_paths", "--base", base])
    assert guard.main() == 1
    assert "asm/us/blob/example.s" in capsys.readouterr().err


def test_submission_only_passes_the_cli(repository, monkeypatch):
    repo, base = repository
    _write(repo, "cloud/matches/new.c", "/* flags: -O2 */\n")
    _commit(repo)
    monkeypatch.setattr(guard, "REPO", repo)
    monkeypatch.setattr(sys, "argv", ["guard_paths", "--base", base])
    assert guard.main() == 0


def test_protection_can_use_newer_base_branch_than_diff_merge_base(repository):
    repo, merge_base = repository
    _write(repo, "blob_matched.lock.json", json.dumps({
        "new": {"source": "src/blob/new.c"}}))
    base_branch = _commit(repo)
    # A function locked on master since the PR branched is also protected.
    assert guard.violations(repo, ["src/blob/new.c"], merge_base) == []
    assert guard.violations(repo, ["src/blob/new.c"], base_branch) == ["src/blob/new.c"]


def test_missing_base_lock_fails_closed(repository, monkeypatch, capsys):
    repo, _ = repository
    (repo / "blob_matched.lock.json").unlink()
    base = _commit(repo)
    monkeypatch.setattr(guard, "REPO", repo)
    monkeypatch.setattr(sys, "argv", ["guard_paths", "--base", base])
    assert guard.main() == 1
    assert "protected-path check failed" in capsys.readouterr().err
