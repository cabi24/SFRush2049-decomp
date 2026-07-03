"""Guard: coordinator/ and agent/ must import only the Python stdlib (research R9).

The coordinator runs on the Pi and the node agent bootstraps onto bare boxes
with `curl | python3`; neither may grow third-party dependencies.
"""
import ast
import sys
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[3]
GUARDED_DIRS = [
    REPO_ROOT / "tools" / "conveyor" / "coordinator",
    REPO_ROOT / "tools" / "conveyor" / "agent",
]

# sys.stdlib_module_names is a 3.10+ API; the *shipped* code keeps the 3.9
# floor, but this dev-only guard needs the authoritative list, so it runs
# wherever it's available (CI/dev boxes) and skips on 3.9.
if not hasattr(sys, "stdlib_module_names"):
    pytest.skip("stdlib guard needs Python >= 3.10", allow_module_level=True)

STDLIB = set(sys.stdlib_module_names)


def _imports_of(path):
    tree = ast.parse(path.read_text(), filename=str(path))
    for node in ast.walk(tree):
        if isinstance(node, ast.Import):
            for alias in node.names:
                yield node, alias.name.split(".")[0]
        elif isinstance(node, ast.ImportFrom):
            if node.level:  # relative import within the package: allowed
                continue
            if node.module:
                yield node, node.module.split(".")[0]


def test_guarded_packages_are_stdlib_only():
    violations = []
    for directory in GUARDED_DIRS:
        for path in sorted(directory.rglob("*.py")):
            for node, root in _imports_of(path):
                # Imports of the conveyor package itself are allowed.
                if root == "tools":
                    continue
                if root not in STDLIB:
                    violations.append(f"{path}:{node.lineno} imports non-stdlib '{root}'")
    assert not violations, "\n".join(violations)
