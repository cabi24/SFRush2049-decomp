"""Collect the image-B event-ring packet's exact GNU placement regressions."""
import importlib.util
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
PATH = ROOT / 'cloud/work/runtime_b_event_ring_20261006/test_packet.py'
SPEC = importlib.util.spec_from_file_location('event_ring_packet_tests', PATH)
MODULE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(MODULE)

PacketTests = MODULE.PacketTests
LinkScriptTests = MODULE.LinkScriptTests
GNUPlacementTests = MODULE.GNUPlacementTests
