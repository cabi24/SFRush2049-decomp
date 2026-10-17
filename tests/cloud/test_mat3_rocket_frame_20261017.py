"""One bounded research-packet replay with native layout and semantic controls."""
import importlib.util
from pathlib import Path


def load(path, name):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def test_rocket_frame_research_packet(tmp_path):
    root = Path(__file__).resolve().parents[2]
    packet = root/'cloud/work/frontier/mat3_rocket_frame_20261017'
    verifier = load(packet/'verify.py', 'mat3_rocket_frame_verify')
    result = verifier.verify(tmp_path)
    assert result['claims'] == []
    assert result['new_matching_bytes'] == 0
    semantic = load(packet/'test_palette_semantics.py', 'mat3_palette_semantics')
    checks = semantic.PaletteSemantics()
    checks.test_native_table_access_width()
    checks.test_layout_and_palette_branches()
