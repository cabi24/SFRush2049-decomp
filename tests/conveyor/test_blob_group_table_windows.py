"""Independent original image windows for native IPA switch tables."""
import shutil
import struct
import subprocess

import pytest
from tools.conveyor.pipeline import blob_group

pytestmark = pytest.mark.skipif(shutil.which('mips-linux-gnu-as') is None,
                                reason='needs the mips binutils')


def _case(tmp_path, first='.word f+8, f+12', second='.word g+8, g+12', extra=''):
    assembly = '''.set noreorder
.text
.globl f
.type f,@function
f:
 lui $t0,%hi(first_table)
 lw $t0,%lo(first_table)($t0)
 jr $ra
 nop
.globl g
.type g,@function
g:
 lui $t1,%hi(second_table)
 lw $t1,%lo(second_table)($t1)
 jr $ra
 nop
.section .rodata
first_table:
 FIRST
second_table:
 SECOND
 EXTRA
'''.replace('FIRST', first).replace('SECOND', second).replace('EXTRA', extra)
    src, obj = tmp_path/'tables.s', tmp_path/'tables.o'
    src.write_text(assembly)
    subprocess.run(['mips-linux-gnu-as','-EB','-mips2','-32','-o',str(obj),str(src)],check=True)
    extents = {'f': {'vaddr':0x80100000,'size':16},
               'g': {'vaddr':0x80100020,'size':16}}
    slices, ndx = blob_group.member_slices(obj,['f','g'],extents)
    image = bytearray(0x200)
    def put(off, words):
        image[off:off+len(words)*4] = struct.pack('>'+str(len(words))+'I',*words)
    put(0,[0x3c088010,0x8d080100,0x03e00008,0])
    put(0x20,[0x3c098010,0x8d290140,0x03e00008,0])
    put(0x100,[0x80100008,0x8010000c])
    put(0x140,[0x80100028,0x8010002c])
    return obj,slices,ndx,image


def _relocate(case, members=None, extern=None):
    obj,slices,ndx,image = case
    return blob_group.relocate(obj,slices,ndx,extern or {},members=members,
                              image=(bytes(image),0x80100000))


def test_two_native_tables_get_their_exact_distinct_original_windows(tmp_path):
    case = _case(tmp_path)
    bodies = _relocate(case)
    assert bodies['f'] == bytes(case[3][:16])
    assert bodies['g'] == bytes(case[3][0x20:0x30])


def test_context_table_is_verified_even_when_only_caller_is_output(tmp_path):
    case = _case(tmp_path)
    bodies = _relocate(case,members=['g'])
    assert bodies['g'] == bytes(case[3][0x20:0x30])
    case[3][0x100] ^= 1
    with pytest.raises(blob_group.GroupError,match='differs from image'):
        _relocate(case,members=['g'])


def test_original_table_entry_mismatch_is_refused(tmp_path):
    case = _case(tmp_path)
    case[3][0x147] ^= 4
    with pytest.raises(blob_group.GroupError,match='differs from image'):
        _relocate(case)


def test_overlapping_original_table_windows_are_refused(tmp_path):
    case = _case(tmp_path)
    struct.pack_into('>I',case[3],0x24,0x8d290104)
    with pytest.raises(blob_group.GroupError,match='overlapping image'):
        _relocate(case)


@pytest.mark.parametrize('first,second,extra,reason',[
    ('.word f+9, f+12','.word g+8, g+12','','unaligned table text'),
    ('.word f+64, f+12','.word g+8, g+12','','no member slice'),
    ('.word ext_fn, f+12','.word g+8, g+12','','not covered text'),
    ('.word f+8\n.word 123','.word g+8, g+12','','unmapped data between'),
    ('.word f+8, f+12','.word g+8, g+12','.word 123','nonzero unmapped'),
])
def test_incomplete_or_nontext_native_table_evidence_is_refused(tmp_path,first,second,extra,reason):
    case = _case(tmp_path,first,second,extra)
    with pytest.raises(blob_group.GroupError,match=reason):
        _relocate(case,extern={'ext_fn':0x80000400})


@pytest.mark.parametrize('mutation,reason',[
    ('duplicate','overlapping table relocations'),
    ('unaligned','unaligned or out-of-window'),
    ('outside','unaligned or out-of-window'),
    ('unknown','unsupported table relocation'),
    ('missing','unmapped data between'),
])
def test_bad_native_relocation_metadata_is_refused(tmp_path,monkeypatch,mutation,reason):
    case = _case(tmp_path)
    rels,others = blob_group._text_relocations(case[0])
    data = list(others['.rel.rodata'])
    if mutation=='duplicate': data.append(data[0])
    elif mutation=='unaligned': data[0]=(1,*data[0][1:])
    elif mutation=='outside': data[0]=(4096,*data[0][1:])
    elif mutation=='unknown': data[0]=(data[0][0],'R_MIPS_HI16',data[0][2])
    elif mutation=='missing': data.pop(1)
    monkeypatch.setattr(blob_group,'_text_relocations',lambda obj:(rels,{'.rel.rodata':data}))
    with pytest.raises(blob_group.GroupError,match=reason): _relocate(case)


def test_conflicting_references_to_one_object_table_are_refused(tmp_path):
    case = _case(tmp_path)
    # Both native pairs refer to first_table, but original pairs name distinct
    # windows. This cannot be repaired by selecting an arbitrary section base.
    rels,others=blob_group._text_relocations(case[0])
    text=blob_group._text(case[0])
    struct.pack_into('>I',text,20,struct.unpack_from('>I',text,20)[0]&0xffff0000)
    from unittest.mock import patch
    with patch.object(blob_group,'_text',return_value=text):
        with pytest.raises(blob_group.GroupError,match='conflicting references'):
            _relocate(case)


def test_table_addend_outside_verified_windows_is_refused():
    windows=blob_group._TableWindows([(0,8,0x80100100),(8,16,0x80100140)])
    assert windows.address(12)==0x80100144
    with pytest.raises(blob_group.GroupError,match='outside verified'):
        windows.address(16)
    with pytest.raises(blob_group.GroupError,match='unaligned'):
        windows.address(9)


@pytest.mark.parametrize('mutation,reason',[
    ('duplicate','duplicate text relocation site'),
    ('unaligned','unaligned or invalid text relocation site'),
    ('missing_hi','LO16 .*no.* HI16|LO16 at .*no HI16'),
    ('missing_lo','unpaired HI16'),
    ('unknown','unsupported'),
])
def test_invalid_table_reference_pairs_are_refused(tmp_path,monkeypatch,mutation,reason):
    case=_case(tmp_path)
    rels,others=blob_group._text_relocations(case[0])
    rels=list(rels)
    if mutation=='duplicate': rels.insert(1,rels[0])
    elif mutation=='unaligned': rels[0]=(1,*rels[0][1:])
    elif mutation=='missing_hi': rels.pop(0)
    elif mutation=='missing_lo': rels.pop(3)
    elif mutation=='unknown': rels[1]=(rels[1][0],'R_MIPS_16',rels[1][2])
    monkeypatch.setattr(blob_group,'_text_relocations',lambda obj:(rels,others))
    with pytest.raises(blob_group.GroupError,match=reason): _relocate(case)


def test_unaligned_original_table_address_is_refused(tmp_path):
    case=_case(tmp_path)
    struct.pack_into('>I',case[3],0x24,0x8d290141)
    with pytest.raises(blob_group.GroupError,match='unaligned jump table reference'):
        _relocate(case)


def test_reference_beyond_native_table_data_is_refused(tmp_path,monkeypatch):
    case=_case(tmp_path)
    text=blob_group._text(case[0])
    struct.pack_into('>I',text,20,(struct.unpack_from('>I',text,20)[0]&0xffff0000)|4096)
    monkeypatch.setattr(blob_group,'_text',lambda obj:text)
    with pytest.raises(blob_group.GroupError,match='outside section'):
        _relocate(case)


def test_same_base_tables_still_use_existing_contiguous_section_placement(tmp_path):
    case=_case(tmp_path)
    struct.pack_into('>I',case[3],0x24,0x8d290108)
    case[3][0x108:0x110]=case[3][0x140:0x148]
    bodies=_relocate(case)
    assert bodies['f']==bytes(case[3][:16])
    assert bodies['g']==bytes(case[3][0x20:0x30])



def test_single_table_member_still_accepts_context_extent_cutting_its_pair(tmp_path):
    # g is genuinely compiled with its whole pair, but only its first word is
    # covered by the context extent. The old single-table path deliberately
    # ignores context-only relocations and must retain this behavior.
    case=_case(tmp_path,second='')
    obj,slices,ndx,image=case
    lo,va,_=slices['g']
    slices['g']=(lo,va,4)
    bodies=_relocate(case,members=['f'])
    assert bodies['f']==bytes(image[:16])


def test_single_table_member_still_ignores_unsupported_context_only_site(tmp_path,monkeypatch):
    case=_case(tmp_path,second='')
    rels,others=blob_group._text_relocations(case[0])
    changed=[(o,'R_MIPS_16' if o==16 else kind,name) for o,kind,name in rels]
    monkeypatch.setattr(blob_group,'_text_relocations',lambda obj:(changed,others))
    bodies=_relocate(case,members=['f'])
    assert bodies['f']==bytes(case[3][:16])


def test_multi_window_fallback_refuses_reference_outside_covered_text(tmp_path):
    case=_case(tmp_path,second='.word f+8, f+12')
    case[1]['g']=(case[1]['g'][0],case[1]['g'][1],4)
    with pytest.raises(blob_group.GroupError,match='outside covered text'):
        _relocate(case,members=['f'])
