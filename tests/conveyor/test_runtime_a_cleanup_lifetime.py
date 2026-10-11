"""Complete cleanup nonmatch evidence, with strict matching gates unchanged."""
import importlib.util
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import pytest
ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/dot_runtime_a_cleanup_lifetime_20261011'
spec=importlib.util.spec_from_file_location('cleanup_lifetime_verify_test',PACKET/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)

def tools_required():
    ido=Path(os.environ.get('IDO_DIR',ROOT/'tools/cloud/ido'))
    if any(not (ido/n).is_file() for n in ('cc','uopt','ugen','as1')) or any(not shutil.which(n) for n in ('mips-linux-gnu-as','mips-linux-gnu-ld')):
        if os.environ.get('REQUIRE_TOOLCHAIN')=='1':pytest.fail('required toolchain missing')
        pytest.skip('toolchain unavailable')

def test_receipt_preserves_nonmatch_and_binds_source():
    r=json.loads((PACKET/'verification.json').read_text())
    assert r['status']=='VERIFIED NONMATCH'
    assert r['source_sha256']==v.base.sha((v.SOURCE/'group.c').read_bytes())
    assert r['score']['differing']==18
    assert r['new_matching_bytes']==r['accepted_image_bytes']==r['cartridge_bytes']==0

def test_fresh_native_GNU_model_lifetimes():
    tools_required();assert v.verify()==json.loads((PACKET/'verification.json').read_text())

def test_guard_and_optimized_mode():
    with pytest.raises(ValueError,match='test'):v.require(False,'test')
    r=subprocess.run([sys.executable,'-O',str(PACKET/'verify.py')],capture_output=True,text=True)
    assert r.returncode!=0 and 'Run without Python -O' in r.stderr

def test_interpreter_fails_closed_and_model_rejects_wrong_call():
    s=v.semantics;c=next(s.cases())
    with pytest.raises(ValueError,match='unknown opcode'):s.run([0xfc000000],c,0)
    with pytest.raises(ValueError,match='escaped code'):s.run([0],c,0)
    with pytest.raises(ValueError,match='unmapped read'):s.read({},0,4)
    v.base.score.ASM_DIR=ROOT/'asm/us/ovl_a';native=v.base.score.targets()[v.NAME][:]
    # Alter the first service argument from its authenticated immediate by one.
    native[3]^=1
    assert s.run(native,c,0)['calls']!=s.oracle(c)[1]

def test_dynamic_count_and_guard_coverage():
    r=json.loads((PACKET/'verification.json').read_text())['semantics']
    assert r['cases_per_native_GNU_and_model']==512
    assert r['instructions_covered']==169
    assert r['dynamic_player_count_mutations']==209
    assert r['standalone_O32_replacement'] is False
