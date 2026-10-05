"""Run promote_batch's full-ROM gate (sync, touch, make, make test, fresh
objects) on the current tree for the given TUs, without splicing.
Usage: python3 cloud/work/boot_tail_promotion/gate_only.py src/rom/lib_X.c ..."""
import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).parent))
import promote_batch as pb  # noqa: E402

ok, detail = pb.gate([pb.REPO / p for p in sys.argv[1:]])
print("GATE", "PASS" if ok else "FAIL", "|", detail)
sys.exit(0 if ok else 1)
