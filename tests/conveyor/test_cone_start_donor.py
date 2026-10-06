"""Cone-start buffer-contract correction; explicitly no binary match claim."""
import importlib.util
import json
from pathlib import Path
import shutil
import tempfile
import pytest
ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_cone_start_donor_20261005'
spec=importlib.util.spec_from_file_location('cone_start_proof',HERE/'verify.py')
proof=importlib.util.module_from_spec(spec);spec.loader.exec_module(proof)

def toolchain():
    if not Path(proof.score.ido('cc')).exists() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and GNU MIPS tools required')

def test_complete_replay_matches_receipt():
    toolchain()
    assert json.loads(json.dumps(proof.main()))==json.loads((HERE/'verification.json').read_text())

def test_record_offsets_and_matrix_capacity():
    toolchain()
    with tempfile.TemporaryDirectory() as tmp:
        p=Path(tmp)/'layout.c';o=Path(tmp)/'layout.o'
        checks={'sizeof(MATRIX)':48,'sizeof(Descriptor)':48,'sizeof(Car)':952,'sizeof(Model)':2056,
          '__builtin_offsetof(Target,orientation)':20,'__builtin_offsetof(Target,position)':56,
          '__builtin_offsetof(Target,motion)':108,'__builtin_offsetof(Visual,callback)':20,
          '__builtin_offsetof(Visual,lifetime)':16,'sizeof(Visual)':24}
        # This isolated IDO setup has no system headers; use its C89 offsetof form.
        source='#define offsetof(t,m) ((unsigned)&((t *)0)->m)\n#include "%s"\n'%(HERE/'candidate.c')
        for i,(expr,value) in enumerate(checks.items()):
            expr=expr.replace('__builtin_offsetof','offsetof')
            source+='typedef char check_%d[(%s)==%d?1:-1];\n'%(i,expr,value)
        p.write_text(source);proof.score.compile_single(p,proof.FLAGS,o)

def test_unknown_instruction_fails_closed():
    with pytest.raises(AssertionError,match='unsupported'):
        proof.native.execute([0xFFFFFFFF],proof.START,[],[proof.T,0,0,0])

def test_truncated_body_is_rejected():
    case=next(proof.cases());case[0]=1;case[2]=236
    with pytest.raises(AssertionError):
        proof.native.execute(proof.score.targets()[proof.FN][:-1],proof.START,proof.initial(case),[proof.T,0,0,0],proof.handler(case,[]))

def test_redirected_stack_save_is_rejected():
    case=next(proof.cases());case[0]=1;case[2]=236
    words=proof.score.targets()[proof.FN][:]
    words[1]=(words[1]&0xFFFF0000)|32
    with pytest.raises(AssertionError):
        proof.native.execute(words,proof.START,proof.initial(case),[proof.T,0,0,0],proof.handler(case,[]))

def test_wrong_call_target_is_rejected():
    case=next(proof.cases());case[0]=1;case[2]=236
    words=proof.score.targets()[proof.FN][:]
    for i,w in enumerate(words):
        if w>>26==3:words[i]=(w&0xFC000000)|0x100;break
    with pytest.raises(AssertionError,match='unknown callback'):
        proof.native.execute(words,proof.START,proof.initial(case),[proof.T,0,0,0],proof.handler(case,[]))

def test_research_path_has_no_matching_claims():
    from tools.cloud import check_submissions
    assert list(check_submissions.commands(ROOT,[str((HERE/'candidate.c').relative_to(ROOT))]))==[]
    assert json.loads((HERE/'provenance.json').read_text())['claims']==[]
