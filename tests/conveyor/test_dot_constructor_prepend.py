"""Bounded regression for the D197C constructor's accepted list contract."""
import copy
import importlib.util
import json
from pathlib import Path
import shutil
import struct
import pytest

ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/dot_constructor_prepend_20261005'
spec=importlib.util.spec_from_file_location('constructor_packet',PACKET/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)


def test_receipt_has_no_matching_claim():
    r=json.loads((PACKET/'verification.json').read_text())
    c=json.loads((PACKET/'claim.json').read_text())
    assert r['status']=='NONMATCH' and r['accepted_byte_gain']==0 and c['claims']==[]
    assert r['start']=='0x800d197c' and r['end']=='0x800d1ab0'
    assert all(x['functions'][v.FN]['differing_words']==24 for x in r['compile'].values())


def test_source_hash_and_real_list_members():
    r=json.loads((PACKET/'verification.json').read_text())
    assert v.sha(v.SOURCE.read_bytes())==r['source_sha256']
    source=v.SOURCE.read_text()
    assert 'u32 count, head, tail;' in source
    assert 'func_80091FBC(&D_80149860, obj, D_80149860.head);' in source
    assert 'volatile' not in source and '__standin' not in source


def test_complete_context_and_no_owned_data():
    r=json.loads((PACKET/'verification.json').read_text())
    result=r['compile']['context']
    assert result['owned_data_bytes']==0
    for n,size in [('func_800D18D8',164),('func_80091FBC',352),('func_8009211C',348)]:
        f=result['functions'][n]
        assert f['symbol_bytes']==size==f['native_bytes'] and f['differing_words']==0
        assert f['gnu_equals_project_relocation']


def test_native_bit_patterns_and_boundaries():
    code=v.score.targets()[v.FN]
    for seed in [0,1,0xffffffff,0x80000000,0x7fffffff,0xdeadbeef]:
        for count in [-0x80000000,-3,0,1,3,4]:
            v.native.Machine(code,v.native.fixture(seed,count)).run()


def test_native_unmapped_read_rejected():
    m=v.native.Machine(v.score.targets()[v.FN],v.native.fixture(1,2))
    del m.mem[v.native.STACK+20]
    with pytest.raises(AssertionError):m.run()


def test_fresh_frozen_replay():
    required=['mips-linux-gnu-ld','cc']
    if not all(shutil.which(x) for x in required):pytest.skip('Needs native/host compiler tools')
    if not Path(v.score.ido('cc')).exists():pytest.skip('Needs pinned IDO')
    assert v.portable(v.verify())==v.portable(json.loads((PACKET/'verification.json').read_text()))


def synthetic_elf(payloads=None,debug_path=b'/first/source.c\0',debug_flags=0):
    """Format-only fixture; no retail or compiled instruction bytes."""
    payloads=payloads or {}
    sections=[('',0,0,b''),('.shstrtab',3,0,b''),('.text',1,6,b'code'),
              ('.rel.text',9,0,b'reloc'),('.symtab',2,0,b'symbols'),
              ('.strtab',3,0,b'names'),('.reginfo',0x70000006,2,b'registers'),
              ('.options',0x7000000d,0,b'options'),('.data',1,3,b'data'),
              ('.rodata',1,2,b'constants'),('.bss',8,3,b'zero'),
              ('.mdebug',0x70000005,debug_flags,debug_path)]
    names=b'\0'.join(name.encode() for name,_,_,_ in sections)+b'\0'
    sections[1]=('.shstrtab',3,0,names)
    data=bytearray(52);rows=[]
    for name,kind,flags,original in sections:
        payload=payloads.get(name,original);offset=len(data)
        if kind!=8:data.extend(payload)
        rows.append((names.index(name.encode()+b'\0'),kind,flags,0,
                     offset,len(payload),0,0,1,0))
    shoff=len(data)
    for row in rows:data.extend(struct.pack('>10I',*row))
    data[:16]=b'\x7fELF\x01\x02\x01'+b'\0'*9
    struct.pack_into('>HHIIIIIHHHHHH',data,16,1,8,1,0,0,shoff,
                     0x10000000,52,0,0,40,len(rows),1)
    return data


def test_source_path_only_changes_are_portable():
    normal=synthetic_elf();moved=synthetic_elf(debug_path=b'/different/longer/source.c\0')
    assert normal!=moved and v.portable_elf(normal)==v.portable_elf(moved)
    report=json.loads((PACKET/'verification.json').read_text())['portability_controls']
    assert set(report['source_path_controls'])=={'archived','corrected','corrected_header_o2'}
    assert all(all(control.values()) for control in report['source_path_controls'].values())
    assert report['semantic_mutations']==dict.fromkeys(
        ['.text','.rel.text','.symtab','.reginfo','.options','ELF_ABI_flags'],'rejected')


@pytest.mark.parametrize('section',['.text','.rel.text','.symtab','.strtab','.reginfo',
                                   '.options','.data','.rodata','.bss'])
def test_portable_elf_binds_every_non_debug_section(section):
    assert v.portable_elf(synthetic_elf())!=v.portable_elf(synthetic_elf({section:b'changed payload'}))


@pytest.mark.parametrize('field,value',[(1,1),(2,2),(2,4),(3,0x80000000)])
def test_portable_elf_rejects_unexpected_debug_attributes(field,value):
    data=synthetic_elf();shoff=struct.unpack_from('>I',data,32)[0]
    struct.pack_into('>I',data,shoff+11*40+4*field,value)
    with pytest.raises(AssertionError,match='unexpected allocated/debug section'):v.portable_elf(data)


@pytest.mark.parametrize('offset',[7,24,36,39])
def test_portable_elf_binds_abi_and_entry_metadata(offset):
    data=synthetic_elf();changed=bytearray(data);changed[offset]^=1
    assert v.portable_elf(data)!=v.portable_elf(changed)


@pytest.mark.parametrize('field',[1,2,3,6,7,8,9])
def test_portable_elf_binds_section_attributes(field):
    data=synthetic_elf();changed=bytearray(data);shoff=struct.unpack_from('>I',data,32)[0]
    at=shoff+2*40+4*field
    struct.pack_into('>I',changed,at,struct.unpack_from('>I',data,at)[0]^1)
    assert v.portable_elf(data)!=v.portable_elf(changed)


def test_portable_receipt_excludes_only_proven_path_and_host_provenance():
    receipt=json.loads((PACKET/'verification.json').read_text());changed=copy.deepcopy(receipt)
    for field in ['gnu_linker_version','host_compiler_version']:changed[field]='other host build'
    for label in ['archived','corrected','corrected_header_o2']:
        for field in ['object_sha256','mdebug_sha256']:changed['compile'][label][field]='other source path'
    assert v.portable(receipt)==v.portable(changed)
    assert receipt==json.loads((PACKET/'verification.json').read_text())
    assert changed['compile']['corrected']['object_sha256']=='other source path'
    # Every top-level proof remains bound except the two documented host identities.
    for field in receipt.keys()-{'gnu_linker_version','host_compiler_version'}:
        altered=copy.deepcopy(receipt)
        if field=='compile':
            altered[field]['corrected']['functions'][v.FN]['body_sha256']='changed'
        else:altered[field]='changed'
        assert v.portable(receipt)!=v.portable(altered),field
    for label,compiled in receipt['compile'].items():
        excluded={'object_sha256','mdebug_sha256'} if label!='context' else set()
        for field in compiled.keys()-excluded:
            altered=copy.deepcopy(receipt);altered['compile'][label][field]='changed'
            assert v.portable(receipt)!=v.portable(altered),(label,field)
