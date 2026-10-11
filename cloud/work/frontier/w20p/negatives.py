"""w20p negative controls for owned_slots: build lib_22300 with IDO 7.1 and show
the acceptance is bound to the registered slot (run from the repo root)."""
import copy, json, struct, subprocess, sys, tempfile
from pathlib import Path
ROOT = Path.cwd()
sys.path.insert(0, str(ROOT)); sys.path.insert(0, str(ROOT / 'cloud/work/boot_tail_promotion'))
from tools.cloud import score
from tools.conveyor import compilers
import owned_slots

score.ASM_DIR = ROOT / 'asm/us/boot_tail'
spec_ctx = __import__('importlib.util').util
spec = spec_ctx.spec_from_file_location('ctx', ROOT / 'cloud/work/boot_tail_promotion/context_check.py')
ctx = spec_ctx.module_from_spec(spec); spec.loader.exec_module(ctx)
tmp = Path(tempfile.mkdtemp())
obj = tmp / 'lib_22300.o'
cmd = [sys.executable, 'tools/asm-processor/build.py', str(compilers.tu_cc('lib_22300', score.ido('cc'))), '--',
       'mips-linux-gnu-as', '-march=vr4300', '-mabi=32', '-Iinclude', '--', '-c', *ctx.CFLAGS, '-o', str(obj), 'src/rom/lib_22300.c']
subprocess.run(cmd, check=True)
fn = 'func_80023BDC'
res = score.compare(obj, fn, show=0)
out = {'scorer': res.summary(), 'scorer_strict': res.accepted(False),
       'owned_accepted': owned_slots.accepted(res, obj, 'lib_22300', fn),
       'sections': owned_slots.allocated_sections(obj, 'lib_22300')}
# 1. wrong TU: lib_22300's .rodata is not a slot of lib_21210.
for label, call in [('wrong_tu_sections', lambda: owned_slots.allocated_sections(obj, 'lib_21210')),
                    ('wrong_tu_accept', lambda: owned_slots.accepted(res, obj, 'lib_21210', fn))]:
    try:
        out[label] = call()
    except ValueError as e:
        out[label] = 'refused: ' + str(e)
# 2. altered slot registration (size shrunk, address moved) via monkeypatch.
real = owned_slots.slots
def patched(change):
    def f(tu):
        rows = copy.deepcopy(real(tu)); [r.update(change) for r in rows.values()]; return rows
    return f
for label, change in [('slot_moved', {'offset': 0x6184}), ('slot_too_small', {'size': 0x4}),
                      ('wrong_hash', {'sha256': '0' * 64})]:
    owned_slots.slots = patched(change)
    try:
        out[label + '_accept'] = owned_slots.accepted(res, obj, 'lib_22300', fn)
    except ValueError as e:
        out[label + '_accept'] = 'refused: ' + str(e)
    try:
        owned_slots.allocated_sections(obj, 'lib_22300'); out[label + '_sections'] = 'ACCEPTED'
    except ValueError as e:
        out[label + '_sections'] = 'refused: ' + str(e)
    owned_slots.slots = real
print(json.dumps(out, indent=1))
