"""Run context_check.py with the --replace-slot row applied (restored after),
as promote_batch --replace-slot plans with it. Usage (builder repo root):
python3 cloud/work/frontier/w22i/ctx_replaced.py ROW.json -- <context_check args>"""
import subprocess
import sys
import json
from pathlib import Path

sys.path.insert(0, str(Path.cwd()))
from tools.conveyor.pipeline import owned_data  # noqa: E402

i = sys.argv.index('--')
row, originals = owned_data.replace_slot(Path.cwd(), json.loads(Path(sys.argv[1]).read_text()))
try:
    rc = subprocess.call([sys.executable, 'cloud/work/boot_tail_promotion/context_check.py'] + sys.argv[i + 1:])
finally:
    owned_data.restore(originals)
sys.exit(rc)
