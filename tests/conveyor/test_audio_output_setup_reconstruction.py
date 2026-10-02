"""Keep the audio_output_setup NONMATCH research honest and reproducible."""
import importlib.util
import os
from pathlib import Path
import shutil
import pytest
ROOT = Path(__file__).resolve().parents[2]
PATH = ROOT / 'cloud/work/natural_audio_output_setup/verify.py'
spec = importlib.util.spec_from_file_location('audio_reconstruction_proof', PATH)
proof = importlib.util.module_from_spec(spec)
spec.loader.exec_module(proof)

def test_audio_output_setup_native_reconstruction(tmp_path):
    ido = Path(os.environ.get('IDO_DIR', ROOT/'tools/cloud/ido'))
    if not (ido/'cc').is_file() or any(not shutil.which(x) for x in ['mips-linux-gnu-ld','mips-linux-gnu-objcopy','mips-linux-gnu-objdump','mips-linux-gnu-readelf']):
        pytest.skip('requires pinned IDO and MIPS binutils')
    result = proof.verify(tmp_path)
    assert result['status'] == 'NONMATCH'
    assert result['full_word_differences'] == 24
    assert result['semantic_cases'] == 4010
    assert result['candidate_body_bytes'] == 144
    assert result['target_bytes'] == 148

def test_target_interpreter_rejects_unknown_instruction():
    target,symbols=proof.targets()
    target[0] = 0xffffffff
    with pytest.raises(AssertionError):
        proof.emulate(target,symbols,[],[],False)

def test_target_interpreter_rejects_invalid_call():
    target,symbols=proof.targets()
    target[6] = 0x0c000001
    with pytest.raises(AssertionError):
        proof.emulate(target,symbols,[],[],False)

def test_typed_audio_source_host_semantics(tmp_path):
    import subprocess
    if not shutil.which('gcc'):
        pytest.skip('requires a host C compiler')
    source = PATH.parent/'host_test.c'
    executable = tmp_path/'host_test'
    subprocess.run(['gcc','-std=c99','-O2','-fsanitize=undefined','-fno-sanitize-recover=all',str(source),'-o',str(executable)],check=True)
    result = subprocess.run([str(executable)],check=True,capture_output=True,text=True)
    assert '20000' in result.stdout
