"""One bounded whole-group regression, including a semantic negative control."""
import importlib.util
from pathlib import Path


def test_natural_wingflame_group_and_negative_control(tmp_path):
    root = Path(__file__).resolve().parents[2]
    path = root / "cloud/work/frontier/mat3_wingflame_20261017/verify.py"
    spec = importlib.util.spec_from_file_location("mat3_wingflame_verify", path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    result = module.verify(tmp_path)
    assert result["new_bytes"] == 636
    assert result["positive"][module.TARGET]["total"] == 159
