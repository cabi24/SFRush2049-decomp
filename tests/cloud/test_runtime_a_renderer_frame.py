"""One bounded input/stock-IDO replay, with shared missing-tool handling."""
import importlib.util
from pathlib import Path


def test_runtime_a_renderer_frame():
    root = Path(__file__).resolve().parents[2]
    path = root / 'cloud/work/frontier/dot_runtime_a_renderer_frame_20261011/verify.py'
    spec = importlib.util.spec_from_file_location('runtime_a_renderer_frame', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    module.verify()
