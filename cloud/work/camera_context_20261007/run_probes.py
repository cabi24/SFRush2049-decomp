"""Reproduce bounded, evidence-driven camera context probes; no raw dumps emitted."""
import argparse
from dataclasses import asdict
import datetime
import hashlib
import json
from pathlib import Path
import resource
import sys
import time

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
from audit_context import audit


def run(out):
    baseline = ROOT / 'cloud/work/lean_camera_transform_20261006'
    source = (baseline / 'candidate.c').read_text()
    a = source.index('s32 MP_TargetSteerPos(')
    b = source.index('static s32 stop_entry')
    prefix, helper, caller = source[:a], source[a:b], source[b:]
    prototype = 's32 MP_TargetSteerPos(Entry *,f32,f32);\n'
    outer = ROOT / 'cloud/work/lean_effect_tick_20261006'
    outer_spec = json.loads((outer / 'group.json').read_text())
    outer_files = {n: (outer / n).read_text() for n in outer_spec['files']}
    outer_files['camera.c'] = source
    shared_helper = helper.replace('func_8001FEA4(entry->voice,(u8)(u32)(volume*127.0f));', 'set_volume(entry,volume);').replace('func_8001FFF4(entry->voice,(u8)(u32)((pan+1.0f)*0.5f*127.0f));', 'set_pan(entry,pan);')
    variants = [
        ('exported_baseline', {'candidate.c': source}, ['camera_transform', 'MP_TargetSteerPos']),
        ('internal_same', {'candidate.c': source}, ['camera_transform']),
        ('internal_split', {'helper.c': prefix + helper, 'caller.c': prefix + prototype + caller}, ['camera_transform']),
        ('internal_after', {'candidate.c': prefix + prototype + caller + helper}, ['camera_transform']),
        ('genuine_outer', outer_files, outer_spec['keep']),
        ('shared_setters', {'candidate.c': prefix + 'static s32 set_volume(Entry *,f32);\nstatic s32 set_pan(Entry *,f32);\n' + shared_helper + caller}, ['camera_transform']),
        ('structural_probe', {'candidate.c': Path(__file__).with_name('candidate.c').read_text()}, ['camera_transform', 'MP_TargetSteerPos']),
    ]
    rows = []
    for name, files, keep in variants:
        directory = out / name
        directory.mkdir(parents=True, exist_ok=True)
        for filename, contents in files.items():
            (directory / filename).write_text(contents)
        (directory / 'group.json').write_text(json.dumps({'files': list(files), 'keep': keep, 'members': ['camera_transform'], 'claims': [], 'flags': '-g0 -O3 -mips2 -G 0 -non_shared'}, indent=2) + '\n')
        utc = datetime.datetime.now(datetime.timezone.utc).isoformat()
        start = time.monotonic()
        cpu = resource.getrusage(resource.RUSAGE_CHILDREN)
        obj = directory / 'candidate.o'
        score.compile_group(directory, obj)
        compared = score.compare(obj, 'camera_transform', show=0)
        cpu_end = resource.getrusage(resource.RUSAGE_CHILDREN)
        rows.append({'id': name, 'start_utc': utc, 'wall_seconds': time.monotonic() - start,
                     'child_cpu_seconds': cpu_end.ru_utime + cpu_end.ru_stime - cpu.ru_utime - cpu.ru_stime,
                     'source_sha256': {n: hashlib.sha256(c.encode()).hexdigest() for n, c in files.items()},
                     'comparison': asdict(compared), 'camera_extent_words': audit(obj)['compiled']['camera_transform']['extent_words']})
        print(name + ': ' + compared.summary())
    result = {'timing_scope': 'Per-probe compile+canonical-score machine elapsed time and child CPU; excludes human/agent analysis and publication.', 'rows': rows}
    (out / 'results.json').write_text(json.dumps(result, indent=2) + '\n')
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('output', type=Path)
    args = parser.parse_args()
    run(args.output.resolve())
