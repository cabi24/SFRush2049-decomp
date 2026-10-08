"""Bounded host-C comparison with clean baseline; not a retail execution test."""
import datetime
import json
from pathlib import Path
import subprocess
import tempfile
import time

ROOT = Path(__file__).resolve().parents[3]
HERE = Path(__file__).resolve().parent


def run():
    start = time.monotonic()
    utc = datetime.datetime.now(datetime.timezone.utc).isoformat()
    with tempfile.TemporaryDirectory(prefix='camera-context-host-') as temporary:
        work = Path(temporary)
        for name, source in [('baseline', ROOT / 'cloud/work/lean_camera_transform_20261006/candidate.c'), ('candidate', HERE / 'candidate.c')]:
            subprocess.run(['gcc', '-std=c89', '-pedantic-errors', '-O0', '-fno-strict-aliasing', '-Dcamera_transform=' + name + '_camera_transform', '-DMP_TargetSteerPos=' + name + '_MP_TargetSteerPos', '-c', str(source), '-o', str(work / (name + '.o'))], check=True)
        subprocess.run(['gcc', '-std=c99', '-O0', str(HERE / 'semantic_harness.c'), str(work / 'baseline.o'), str(work / 'candidate.o'), '-o', str(work / 'check')], check=True)
        result = json.loads(subprocess.check_output([str(work / 'check')], text=True, timeout=15))
    return dict(result, start_utc=utc, wall_seconds=time.monotonic() - start,
                scope='2048 host scenarios against reconstructed baseline, not native execution or complete O32/FCSR equivalence.')


if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
