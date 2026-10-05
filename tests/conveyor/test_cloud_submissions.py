"""Exercise CI change selection and strict acceptance of submitted C."""
import shutil
import subprocess

import pytest

from tools.cloud import check_submissions as ci, score


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


def test_diff_includes_both_sides_of_rename_and_deleted_paths(tmp_path):
    ci.git(tmp_path, "init", "-q")
    old = _write(tmp_path, "cloud/matches/old.c", "same\n")
    _write(tmp_path, "cloud/matches/deleted.c", "gone\n")
    base = _commit(tmp_path)
    old.rename(old.with_name("new.c"))
    (tmp_path / "cloud/matches/deleted.c").unlink()
    head = _commit(tmp_path)
    assert set(ci.changed_paths(tmp_path, base, head)) == {
        "cloud/matches/old.c", "cloud/matches/new.c", "cloud/matches/deleted.c"}
    assert ci.changed_paths(tmp_path, "0" * 40, head) == ["cloud/matches/new.c"]
    with pytest.raises(ValueError, match="checkout must be"):
        ci.changed_paths(tmp_path, head, base)


def test_selects_singles_and_deduplicates_changed_groups(tmp_path):
    single = "cloud/matches/function_name.c"
    _write(tmp_path, single, "/* flags: -g0 -O2 -mips2 -G 0 -non_shared */\n")
    group = "cloud/work/ipa-groups/a group"
    _write(tmp_path, group + "/group.json", "{}")
    jobs = list(ci.commands(tmp_path, [single, group + "/group.c",
                                      group + "/STATUS.md", "cloud/work/ipa-groups/INDEX.md",
                                      "cloud/matches/deleted.c", "unrelated.c"]))
    assert len(jobs) == 2
    assert jobs[0][2:] == ["fn", str(tmp_path / single), "function_name",
                           "--flags=-g0 -O2 -mips2 -G 0 -non_shared"]
    assert jobs[1][2:] == ["group", str(tmp_path / group), "--claims"]


def test_runtime_image_submissions_score_against_their_own_targets(tmp_path):
    header = "/* flags: -g0 -O2 -mips2 -G 0 -non_shared */\n"
    for image in ("ovl_a", "ovl_b", "boot_tail"):
        _write(tmp_path, f"cloud/matches/{image}/func_8038A400.c", header)
    _write(tmp_path, "cloud/matches/ovl_c/func_8038A400.c", header)
    jobs = list(ci.commands(tmp_path, [
        "cloud/matches/ovl_a/func_8038A400.c", "cloud/matches/ovl_b/func_8038A400.c",
        "cloud/matches/boot_tail/func_8038A400.c", "cloud/matches/ovl_c/func_8038A400.c"]))
    assert [job[-1] for job in jobs] == [
        "--targets=" + str(tmp_path / "asm/us/boot_tail"),
        "--targets=" + str(tmp_path / "asm/us/ovl_a"),
        "--targets=" + str(tmp_path / "asm/us/ovl_b")]
    assert all(job[4] == "func_8038A400" for job in jobs)


@pytest.mark.parametrize("header", ["", "/* flags:  */", "int f(void);",
                                    "\n/* flags: -O2 */", "/* flags: -O2 */ trailing"])
def test_requires_flags_on_first_line(tmp_path, header):
    name = "cloud/matches/f.c"
    _write(tmp_path, name, header)
    with pytest.raises(ValueError, match="line 1"):
        list(ci.commands(tmp_path, [name]))


def test_deleted_group_is_skipped_but_partial_group_fails(tmp_path):
    group = "cloud/work/ipa-groups/f"
    assert list(ci.commands(tmp_path, [group + "/group.json"])) == []
    _write(tmp_path, group + "/group.c", "int f(void);")
    with pytest.raises(ValueError, match="missing group.json"):
        list(ci.commands(tmp_path, [group + "/group.json"]))


def test_failure_is_not_hidden_by_a_later_match(tmp_path, monkeypatch):
    names = ["cloud/matches/a.c", "cloud/matches/b.c"]
    for name in names:
        _write(tmp_path, name, "/* flags: -O2 */")
    calls = []

    def run(job, cwd):
        calls.append(job)
        return subprocess.CompletedProcess(job, 1 if len(calls) == 1 else 0)

    monkeypatch.setattr(ci.subprocess, "run", run)
    assert ci.check(tmp_path, names) == 1
    assert len(calls) == 2


@pytest.mark.skipif(not (score.IDO / "cc").exists(), reason="needs x86 IDO")
def test_real_submission_match_passes_and_wrong_global_fails(tmp_path, monkeypatch):
    # Use the real scorer and committed ground truth, with submissions isolated
    # from the repository. No private layout, game image, or ROM is involved.
    (tmp_path / "tools/cloud").mkdir(parents=True)
    shutil.copy(ci.REPO / "tools/cloud/score.py", tmp_path / "tools/cloud/score.py")
    shutil.copy(ci.REPO / "tools/cloud/owndata.py", tmp_path / "tools/cloud/owndata.py")
    (tmp_path / "asm/us").mkdir(parents=True)
    (tmp_path / "asm/us/blob").symlink_to(ci.REPO / "asm/us/blob", target_is_directory=True)
    (tmp_path / "asm/us/blob_data").symlink_to(ci.REPO / "asm/us/blob_data",
                                               target_is_directory=True)
    monkeypatch.setenv("IDO_DIR", str(score.IDO))
    source = (ci.REPO / "src/blob/sound_handles_clear.c").read_text()
    name = "cloud/matches/sound_handles_clear.c"
    path = _write(tmp_path, name, "/* flags: " + score.DEFAULT_FLAGS + " */\n" + source)
    assert ci.check(tmp_path, [name]) == 0
    mutated = path.read_text().replace("D_80116DE4", "D_WRONG")
    assert mutated != path.read_text()
    path.write_text(mutated)
    assert ci.check(tmp_path, [name]) == 1
