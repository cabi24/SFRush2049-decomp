import importlib.util
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
spec = importlib.util.spec_from_file_location("vendor_workbench", REPO / "tools" / "vendor_workbench.py")
vendor = importlib.util.module_from_spec(spec)
spec.loader.exec_module(vendor)


def test_status_line_covers_each_state():
    assert vendor.status_line(None, "abc") == "not vendored yet"
    assert "unreachable" in vendor.status_line("a" * 40, None)
    assert "is current" in vendor.status_line("a" * 40, "a" * 40)
    assert "upstream is at bbbbbbbbbb" in vendor.status_line("a" * 40, "b" * 40)


def test_pinned_commit_is_read_from_upstream_md(tmp_path):
    (tmp_path / "UPSTREAM.md").write_text("# x\n\n- commit: " + "c" * 40 + "\n- licence: CC0-1.0\n")
    assert vendor.pinned_commit(tmp_path) == "c" * 40
    assert vendor.pinned_commit(tmp_path / "missing") is None


def test_the_vendored_tool_runs_from_the_repo():
    out = subprocess.run([sys.executable, str(REPO / "tools" / "workbench.py"), "--version"],
                         capture_output=True, text=True)
    assert out.returncode == 0 and "decomp-workbench" in out.stdout
    assert (REPO / "third_party" / "n64-decomp-workbench" / "LICENSE.md").is_file()
    assert vendor.pinned_commit() is not None


def test_a_fixture_comparison_is_instruction_exact():
    fixtures = REPO / "third_party" / "n64-decomp-workbench" / "examples" / "fixtures"
    out = subprocess.run([sys.executable, str(REPO / "tools" / "workbench.py"), "compare-dumps",
                          str(fixtures / "target.objdump"), str(fixtures / "relocated-match.objdump")],
                         capture_output=True, text=True)
    assert "verdict=instruction-exact" in out.stdout
