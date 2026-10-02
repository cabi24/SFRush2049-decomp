"""Run unchanged existing ownership tests against the private production draft.

Only this Python process's imported module references are redirected; no live
source, registry, layout, header, tool or Git state is edited.
"""
import contextlib
import importlib.util
import io
import json
from pathlib import Path
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
import pytest
from tools.conveyor.pipeline import layout, promote

spec = importlib.util.spec_from_file_location('tools.conveyor.pipeline.c85_owned_data',
                                            HERE / 'tools/conveyor/pipeline/owned_data.py')
draft = importlib.util.module_from_spec(spec)
spec.loader.exec_module(draft)
layout.owned_data = draft
promote.owned_data = draft


class UseDraft:
    def pytest_collection_modifyitems(self, items):
        for item in items:
            item.module.O = draft


output = io.StringIO()
with contextlib.redirect_stdout(output), contextlib.redirect_stderr(output):
    code = pytest.main(['-q', str(ROOT / 'tests/conveyor/test_owned_data.py')], plugins=[UseDraft()])
proof = {'command': 'python3 cloud/work/static_C85/proposal/run_existing_compatibility.py',
         'unchanged_test_source': 'tests/conveyor/test_owned_data.py', 'exit_code': int(code),
         'stdout_and_stderr': output.getvalue()}
(HERE / 'existing_compatibility.json').write_text(json.dumps(proof, indent=2) + '\n')
print(output.getvalue())
raise SystemExit(code)
