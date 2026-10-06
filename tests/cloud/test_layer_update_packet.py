"""The registered layer helper requires complete source, native and behavior proof."""
import copy,importlib.util,json,os,shutil,struct,sys
from pathlib import Path
import pytest
ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/ipa-groups/dot_layer_update_20261005'
spec=importlib.util.spec_from_file_location('layer_update_verify',HERE/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)

def receipt():return json.loads((HERE/'verification.json').read_text())
def require_toolchain():
    if not (v.score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        if os.environ.get('REQUIRE_TOOLCHAIN')=='1':pytest.fail('pinned toolchain missing')
        pytest.skip('pinned IDO and GNU MIPS tools required')

def test_source_bound_and_honest_scope():
    r=receipt();assert r['source_bindings']==v.bindings()
    assert r['status']=='MATCHING_CANDIDATE' and r['bytes']==152 and r['accepted_byte_gain']==0
    assert set(r['strict_bodies'])=={v.FN,*v.CONTEXT}
    assert all(r['gnu_bodies'][n]['complete_gnu_equal'] for n in r['strict_bodies'])
    assert r['strict_bodies'][v.FN]['elf_bytes']==152
    assert r['data_sections_owned']==r['target_following_alignment_bytes']==0
    assert r['unclaimed_callers']['func_800DFBA0']['elf_bytes']==1184
    assert r['unclaimed_callers']['func_800E05F0']['elf_bytes']==1312
    assert r['controls']['archived_A169']['target']['comparison']['differing']==0
    assert r['controls']['archived_A169']['allocator']['comparison']['differing']==11

def test_native_contract_and_all_target_instructions():
    b=receipt()['behavior'];assert b['cases']==b['host_c89_ubsan_cases']==3116
    assert b['native_executions']==6232
    assert b['instruction_coverage'][v.FN]==list(range(0,152,4))
    assert b['target_branch_outcomes']=={'0x14':[False,True],'0x40':[False,True]}
    assert len(b['executed_bodies'])==5 and b['complete_native_link_read_write_traces_equal']
    assert b['queue_callback_mutations'] and b['stack_canaries_and_private_abi_checked']
    assert all(r['rejected_cases']>0 for r in receipt()['semantic_mutants'].values())

def test_registered_submission_uses_strict_existing_path():
    from tools.cloud.check_submissions import commands
    jobs=list(commands(ROOT,[str((HERE/'candidate.c').relative_to(ROOT))]))
    assert len(jobs)==1 and jobs[0][2:]==['group',str(HERE),'--claims']
    g=json.loads((HERE/'group.json').read_text());assert g['claims']==g['members']==[v.FN]
    assert not g.get('allow_unverified')

def test_native_unknown_instruction_fails_closed():
    b,a=v.proof.native(ROOT);machine=v.semantics.Machine(a,b)
    machine.code[a[v.FN]]=0xffffffff
    with pytest.raises(AssertionError,match='unknown opcode'):machine.run(next(v.semantics.corpus()))

def test_behavior_detects_delayed_handle_snapshot():
    cases=list(v.semantics.corpus())
    selected=[c for c in cases if c['layer'][0]==256 and (3,0,257) in c['mutations']]
    assert selected
    case=copy.deepcopy(selected[0]);case['values'][0][3]=v.semantics.bits(0)
    expected=v.semantics.oracle(case)
    assert expected[0]==257
    # At receive event 3 the layer handle changes, but the second message is
    # still addressed to the handle evaluated before the lock acquisition.
    assert expected[13]==2 and expected[14+9+8]==0

def test_frozen_replay(tmp_path):
    require_toolchain()
    # Protected manifests and the scorer change with every splice: frozen provenance, not a lock.
    strip=lambda d:{k:x for k,x in d.items() if k!='target_manifest_sha256'}
    assert strip(v.verify(tmp_path))==strip(receipt())

@pytest.fixture(scope='module')
def compiled(tmp_path_factory):
    require_toolchain();directory=tmp_path_factory.mktemp('layer_negative_elf')
    obj=v.compile_source(directory/'source',(HERE/'candidate.c').read_text())
    return directory,obj

def mutate_symbol_size(raw,name,new_size):
    offset=struct.unpack_from('>I',raw,32)[0];ss,count,si=struct.unpack_from('>HHH',raw,46)
    secs=[struct.unpack_from('>10I',raw,offset+i*ss) for i in range(count)]
    for s in secs:
        if s[1]!=2:continue
        strings=secs[s[6]];table=raw[strings[4]:strings[4]+strings[5]]
        for at in range(s[4],s[4]+s[5],16):
            no=struct.unpack_from('>I',raw,at)[0]
            if table[no:].split(b'\0',1)[0].decode()==name:struct.pack_into('>I',raw,at+8,new_size);return
    raise AssertionError('symbol absent')

@pytest.mark.parametrize('size',[148,156])
def test_real_elf_extent_corruption_refused(compiled,tmp_path,size):
    directory,obj=compiled;raw=bytearray(obj.read_bytes());mutate_symbol_size(raw,v.FN,size)
    mutant=tmp_path/'wrong.o';mutant.write_bytes(raw);b,a=v.proof.native(ROOT)
    with pytest.raises(AssertionError,match='complete extent'):v.proof.link_function(mutant,v.FN,a,b[v.FN],tmp_path/'gnu')

def test_real_instruction_corruption_refused(compiled,tmp_path):
    directory,obj=compiled;elf=v.proof.Elf(obj);fn=elf.function(v.FN);raw=bytearray(obj.read_bytes())
    raw[elf.text['offset']+fn['value']+3]^=8
    mutant=tmp_path/'wrong.o';mutant.write_bytes(raw);b,a=v.proof.native(ROOT)
    with pytest.raises(AssertionError,match='GNU linked body differs'):v.proof.link_function(mutant,v.FN,a,b[v.FN],tmp_path/'gnu')

def test_real_unknown_relocation_refused(compiled,tmp_path):
    directory,obj=compiled;elf=v.proof.Elf(obj);fn=elf.function(v.FN);raw=bytearray(obj.read_bytes());changed=False
    for section in elf.sections:
        if section['type']!=9 or section['info']!=elf.text_index:continue
        for at in range(section['offset'],section['offset']+section['size'],8):
            site,info=struct.unpack_from('>II',raw,at)
            if fn['value']<=site<fn['value']+fn['size']:
                struct.pack_into('>I',raw,at+4,(info&0xffffff00)|255);changed=True;break
    assert changed
    mutant=tmp_path/'wrong.o';mutant.write_bytes(raw);b,a=v.proof.native(ROOT)
    with pytest.raises(AssertionError,match='unsupported relocation'):v.proof.link_function(mutant,v.FN,a,b[v.FN],tmp_path/'gnu')
