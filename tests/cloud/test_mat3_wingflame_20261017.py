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


def test_wingflame_guards_survive_optimized_python(tmp_path):
    import subprocess
    import sys
    packet = Path(__file__).resolve().parents[2] / 'cloud/work/frontier/mat3_wingflame_20261017'
    program = r'''
import copy, importlib.util, json, subprocess, sys
from pathlib import Path
p, out = Path(sys.argv[1]), Path(sys.argv[2])
def load(name):
    spec = importlib.util.spec_from_file_location(name, p / (name + '.py'))
    module = importlib.util.module_from_spec(spec); spec.loader.exec_module(module)
    return module
m = load('verify'); frozen = json.loads((p / 'verification.json').read_text())
def rejected(call, reason):
    try:
        call()
    except RuntimeError as error:
        if reason not in str(error):
            raise RuntimeError('wrong refusal: ' + str(error))
        return
    raise RuntimeError('accepted invalid proof: ' + reason)
for key, value, reason in [
    ('differing', 1, 'strict comparison differs'),
    ('extra_words', 1, 'strict comparison has'),
    ('unresolved', ['bad'], 'strict comparison has'),
    ('unverified', ['bad'], 'strict comparison has'),
    ('errors', ['bad'], 'strict comparison has'),
    ('total', 1, 'complete member extent changed'),
]:
    bad = copy.deepcopy(frozen); bad['positive'][m.TARGET][key] = value
    rejected(lambda: m.verify_report(bad), reason)
for field, value, reason in [
    ('claims', [], 'claim inventory changed'),
    ('new_bytes', 640, 'false credit'),
    ('cartridge_coverage_claimed', True, 'false credit'),
]:
    bad = copy.deepcopy(frozen); bad[field] = value
    rejected(lambda: m.verify_report(bad), reason)
bad = copy.deepcopy(frozen); bad['symbol_bytes'][m.TARGET] -= 4
rejected(lambda: m.verify_report(bad), 'complete member extent changed')
bad = copy.deepcopy(frozen); bad['negative_control'][m.TARGET]['differing'] = 0
rejected(lambda: m.verify_report(bad), 'semantic negative result changed')
original = Path.read_text
for mode, reason in [
    ('source', 'packet input changed'),
    ('target', 'native target words changed'),
    ('context', 'copied context changed'),
    ('trace_input', 'trace input binding changed'),
    ('trace_pair', 'trace identity receipt changed'),
    ('trace_object', 'complete stock/trace object changed'),
    ('frozen_object', 'fresh sealed receipt changed'),
    ('group_claims', 'claim inventory changed'),
]:
    def changed(path, *args, **kwargs):
        text = original(path, *args, **kwargs)
        if path == p / 'verification.json':
            data = json.loads(text)
            if mode == 'source': data['inputs_sha256']['verify.py'] = '0' * 64
            if mode == 'target': data['target_words_sha256'][m.TARGET] = '0' * 64
            if mode == 'context': data['unchanged_context_sha256'][m.UNCHANGED[0]] = '0' * 64
            if mode == 'frozen_object': data['object_sha256'] = '0' * 64
            return json.dumps(data)
        if path == p / 'trace_fidelity.json':
            data = json.loads(text)
            if mode == 'trace_input': data['inputs_sha256']['verify.py'] = '0' * 64
            if mode == 'trace_pair': data['trace_object_sha256'] = '0' * 64
            if mode == 'trace_object': data['stock_object_sha256'] = data['trace_object_sha256'] = '0' * 64
            return json.dumps(data)
        if path == p / 'group/group.json' and mode == 'group_claims':
            data = json.loads(text); data['claims'] = []; return json.dumps(data)
        return text
    Path.read_text = changed
    try: rejected(lambda: m.verify(out / mode), reason)
    finally: Path.read_text = original
# Exercise the actual trace identity guard under -O without running a compiler.
t = load('trace_fidelity'); trace_out = out / 'mock_trace'; trace_out.mkdir()
(trace_out / 'st').write_bytes(b'symbols')
(trace_out / 'stock.o').write_bytes(b'stock')
(trace_out / 'trace.o').write_bytes(b'changed')
run = t.subprocess.run
t.subprocess.run = lambda *args, **kwargs: subprocess.CompletedProcess(args, 0, b'', b'')
try: rejected(lambda: t.run(trace_out, out / 'mock_uopt'), 'instrumented compiler changed the object')
finally: t.subprocess.run = run
print('20 optimized refusal cases pass')
'''
    result = subprocess.run([sys.executable, '-O', '-c', program, str(packet), str(tmp_path)],
                            capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    assert result.stdout == '20 optimized refusal cases pass\n'


def test_wingflame_relocated_packet(tmp_path):
    import shutil
    root = Path(__file__).resolve().parents[2]
    packet = root / 'cloud/work/frontier/mat3_wingflame_20261017'
    relocated = tmp_path / 'other-checkout' / 'packet'
    shutil.copytree(packet, relocated, ignore=shutil.ignore_patterns('__pycache__'))
    spec = importlib.util.spec_from_file_location('wing_relocated', packet / 'verify.py')
    module = importlib.util.module_from_spec(spec); spec.loader.exec_module(module)
    module.HERE = relocated
    assert module.verify(tmp_path / 'objects')['new_bytes'] == 636
