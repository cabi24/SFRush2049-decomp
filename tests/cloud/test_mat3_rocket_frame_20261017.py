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


def test_proof_guards_remain_active_with_python_optimized(tmp_path):
    import subprocess
    import sys
    root = Path(__file__).resolve().parents[2]
    packet = root/'cloud/work/frontier/mat3_rocket_frame_20261017'
    program = r'''
import importlib.util, json, shutil, sys
from pathlib import Path
packet, scratch = map(Path, sys.argv[1:])
spec = importlib.util.spec_from_file_location('proof', packet/'verify.py')
v = importlib.util.module_from_spec(spec); spec.loader.exec_module(v)
shutil.copytree(packet, scratch/'packet')
v.HERE = scratch/'packet'
def reject(call):
    try: call()
    except ValueError: return
    raise RuntimeError('optimized interpreter disabled a proof guard')
reject(lambda: v.clean(dict(extra_words=0, unverified=['unknown'], unresolved=[], errors=[])))
spec_path = v.HERE/'group/group.json'
body = json.loads(spec_path.read_text()); body['claims'] = ['false-credit']
spec_path.write_text(json.dumps(body))
reject(lambda: v.verify(scratch/'claimed'))
shutil.copyfile(packet/'group/group.json', spec_path)
receipt_path = v.HERE/'verification.json'
receipt = json.loads(receipt_path.read_text()); receipt['inputs_sha256']['verify.py'] = 'changed'
receipt_path.write_text(json.dumps(receipt))
reject(lambda: v.verify(scratch/'changed'))
print('PASS optimized proof negatives: unverified reference, false claim, changed source')
'''
    result = subprocess.run([sys.executable, '-O', '-c', program, str(packet), str(tmp_path)],
                            text=True, capture_output=True)
    assert result.returncode == 0, result.stdout + result.stderr
