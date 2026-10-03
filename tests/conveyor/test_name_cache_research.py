"""Research evidence gates, never a matching/coverage gate."""
import hashlib,json,os,shutil,subprocess,sys
from pathlib import Path
import pytest
ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/dot_name_cache'

def test_provenance_and_nonmatch_extent():
    source=(HERE/'func_800F1D04.c').read_bytes()
    assert source==(ROOT/'cloud/work/tiny_A68/func_800F1D04.c').read_bytes()
    proof=json.loads((HERE/'verification.json').read_text())
    assert proof['source_sha256']==hashlib.sha256(source).hexdigest()
    assert proof['claims']==[] and proof['verdict']=='NONMATCH_CONFIRMED'
    assert proof['target_bytes']==888 and proof['candidate_function_bytes']==884
    assert proof['comparison']['differing']==104
    assert not proof['comparison']['unresolved'] and not proof['comparison']['unverified']
    semantic=json.loads((HERE/'semantics.json').read_text())
    assert semantic['verdict']=='PASS' and semantic['cases']==1162
    assert semantic['all_ages_wrap_counterexample'][0]['result']==0
    assert 'unmapped' in semantic['all_ages_wrap_counterexample'][1]['native_error']

def test_native_linked_host_differential():
    ido=Path(os.environ.get('IDO_DIR',ROOT/'tools/cloud/ido'))
    if not (ido/'cc').exists() or not all(shutil.which(t) for t in ['mips-linux-gnu-ld','mips-linux-gnu-objcopy','cc']):
        pytest.skip('requires pinned IDO and GNU MIPS link tools')
    result=subprocess.run([sys.executable,str(HERE/'verify_semantics.py')],cwd=ROOT,text=True,capture_output=True,check=True)
    proof=json.loads(result.stdout)
    assert proof['verdict']=='PASS' and proof['cases']==1162
