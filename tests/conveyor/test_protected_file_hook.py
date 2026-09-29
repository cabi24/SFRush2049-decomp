"""Exercise the actual configured Claude hook using its stdin/exit contract."""
import json
import os
from pathlib import Path
import re
import shutil
import subprocess

import pytest

REPO = Path(__file__).resolve().parents[2]


@pytest.fixture
def hook(tmp_path):
    repo = tmp_path / "repo with spaces"
    directory = repo / ".claude/hooks"
    directory.mkdir(parents=True)
    shutil.copyfile(REPO / ".claude/hooks/protect_files.py", directory / "protect_files.py")
    settings = json.loads((REPO / ".claude/settings.json").read_text())
    entry, = settings["hooks"]["PreToolUse"]
    assert re.fullmatch(entry["matcher"], "Edit")
    assert re.fullmatch(entry["matcher"], "Write")
    assert not re.fullmatch(entry["matcher"], "Bash")
    command = entry["hooks"][0]["command"]

    def invoke(path, tool="Edit", cwd=None, raw=None):
        event = {"tool_name": tool, "tool_input": {"file_path": path},
                 "cwd": str(cwd or repo), "hook_event_name": "PreToolUse"}
        return subprocess.run(command, shell=True, cwd=tmp_path,
                              env=dict(os.environ, CLAUDE_PROJECT_DIR=str(repo)),
                              input=json.dumps(event) if raw is None else raw,
                              capture_output=True, text=True)

    return repo, invoke


@pytest.mark.parametrize("tool", ["Edit", "Write"])
@pytest.mark.parametrize("name", [
    "asm/us/blob/blob_80086a50.s", "asm/us/blob/symbols.json", "asm/us/blob/SHA256SUMS",
    "matched.lock.json", "nested/test.lock.json", "us.sha1", "src/blob/blob.ld",
    "tools/cloud/score.py",
])
def test_denies_protected_edit_and_write(hook, tool, name):
    repo, invoke = hook
    for path in (name, str(repo / name)):
        result = invoke(path, tool)
        assert result.returncode == 2
        assert "Edit/Write denied" in result.stderr


@pytest.mark.parametrize("name", ["cloud/matches/f.c", "cloud/work/ipa-groups/g/group.c",
                                  "docs/notes.md", "tools/cloud/setup.sh"])
def test_other_files_follow_normal_permission_flow(hook, name):
    _, invoke = hook
    result = invoke(name)
    assert result.returncode == 0
    assert result.stdout == result.stderr == ""


def test_read_and_bash_are_unaffected(hook):
    _, invoke = hook
    assert invoke("us.sha1", "Read").returncode == 0
    assert invoke("us.sha1", "Bash").returncode == 0


def test_relative_traversal_uses_event_cwd(hook):
    repo, invoke = hook
    assert invoke("../asm/us/blob/file.s", cwd=repo / "src").returncode == 2


def test_symlink_alias_into_protected_directory_is_denied(hook):
    repo, invoke = hook
    (repo / "asm/us/blob").mkdir(parents=True)
    (repo / "alias").symlink_to(repo / "asm/us/blob", target_is_directory=True)
    assert invoke("alias/file.s").returncode == 2


def test_replacing_protected_symlink_to_outside_is_denied(hook, tmp_path):
    repo, invoke = hook
    (repo / "us.sha1").symlink_to(tmp_path / "outside.txt")
    assert invoke("us.sha1", "Write").returncode == 2


@pytest.mark.parametrize("raw", ["not json", "{}", '{"tool_name":"Edit","tool_input":{}}'])
def test_malformed_request_blocks_with_exit_two(hook, raw):
    _, invoke = hook
    result = invoke("ignored", raw=raw)
    assert result.returncode == 2
    assert "cannot validate" in result.stderr
