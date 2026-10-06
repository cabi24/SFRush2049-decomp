"""Source, ABI, complete native body, bounded semantics, and rejection checks."""
import copy
import importlib.util
import json
from pathlib import Path
import shutil
import struct
import subprocess

import pytest

ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_material_selector_20261006'
spec=importlib.util.spec_from_file_location('material_verify',HERE/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)

@pytest.fixture(scope='module')
def fresh(tmp_path_factory):
    for program in ('cc','mips-linux-gnu-readelf','mips-linux-gnu-ld'):
        assert shutil.which(program), program+' required for this proof'
    return v.verify(tmp_path_factory.mktemp('material-selector'))


def test_fresh_source_bound_receipt(fresh):
    saved=json.loads((HERE/'verification.json').read_text())
    # Whole manifests are historical provenance; current integrity is mandatory.
    # Selected native bodies, consumed symbols, source, and tools remain bound.
    a,b=copy.deepcopy(fresh),copy.deepcopy(saved)
    a.pop('target_manifest_sha256');b.pop('target_manifest_sha256')
    assert a==b


def test_whole_bodies_and_context(fresh):
    for key in ('O2','O3','shared_data_context'):
        assert fresh[key]['elf_bytes']==216
        assert fresh[key]['complete_differing_offsets']==[]
        assert fresh[key]['canonical_verdict']=='MATCH'
    assert fresh['accepted_consumer']['elf_bytes']==208
    assert fresh['accepted_consumer']['canonical_verdict']=='MATCH'
    assert fresh['zero_alignment_bytes_outside_function']==8
    assert fresh['accepted_byte_gain']==0


def test_native_coverage_and_mutations(fresh):
    b=fresh['behavior']
    assert b['cases']==4480 and b['native_and_gnu_executions']==17920
    assert len(b['reachable_instruction_offsets'])==53
    assert b['unreachable_duplicate_load_offset']==120
    assert len(b['compiled_semantic_mutants_rejected'])==5
    assert len(b['malformed_controls_rejected'])==3


def test_negative_selector_marks_only_header():
    n=v.native;words=v.score.targets()[v.FN]
    mem,args,base=n.fixture(0,-1,0xBEEF,4,713)
    initial=mem.copy()
    result=n.execute(words,mem,args)
    assert result[0]==1
    for i in range(4):
        assert n.get(mem,base+24+16*i,2)==0xBEEF
        assert n.get(mem,base+26+16*i,2)==n.get(initial,base+26+16*i,2)
    assert n.get(mem,base+10,2)==n.get(initial,base+10,2)|0x8000
    assert [site for site,size,val in result[3][3:]]==[base+x for i in range(4) for x in (24+16*i,10)]


def test_nonnegative_does_not_fall_through():
    n=v.native;words=v.score.targets()[v.FN]
    mem,args,base=n.fixture(0,1,0xBEEF,4,719)
    initial=mem.copy();n.execute(words,mem,args)
    assert n.get(mem,base+40,2)==0xBEEF
    for i in (0,2,3):
        assert bytes(mem[base+24+16*i+j] for j in range(16))==bytes(initial[base+24+16*i+j] for j in range(16))


def test_larger_positive_selector_returns_before_access():
    n=v.native;words=v.score.targets()[v.FN]
    mem,args,base=n.fixture(0,32767,1,4,727)
    initial=mem.copy();result=n.execute(words,mem,args)
    assert result[0]==0 and len(result[3])==3
    assert bytes(mem[base+i] for i in range(88))==bytes(initial[base+i] for i in range(88))


def test_wrong_elf_extent_is_rejected(tmp_path):
    obj=tmp_path/'candidate.o';v.score.compile_single(v.SOURCE,v.FLAGS,obj)
    data,sections,symbols=v.elf(obj)
    altered=bytearray(data)
    for sec in sections:
        if sec['type']!=2: continue
        syms=v.score._symbol_table(data,sections,sections.index(sec))
        for i,sym in enumerate(syms):
            if sym['name']==v.FN:
                struct.pack_into('>I',altered,sec['off']+i*16+8,224)
    obj.write_bytes(altered)
    with pytest.raises(AssertionError):v.inspect(obj,v.FN,tmp_path)
