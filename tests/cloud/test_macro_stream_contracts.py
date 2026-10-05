"""Explicit CI selection for adapted macro/stream source-contract repairs."""
import copy
import importlib.util
import json
from pathlib import Path
import re
import shutil
import tempfile
from unittest import mock

import pytest

ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/boot_tail_promotion/macro_stream_contracts'
SPEC=importlib.util.spec_from_file_location('macro_stream_verify',PACKET/'verify.py')
v=importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(v)


def require_toolchain():
    if not Path(v.score.ido('cc')).is_file():
        pytest.skip('IDO unavailable')
    for tool in ('mips-linux-gnu-as','mips-linux-gnu-objdump','cc'):
        if not shutil.which(tool):
            pytest.skip('C compiler unavailable' if tool=='cc' else tool+' unavailable')


def fixtures(count):
    context=v.context_module()
    result={}
    with tempfile.TemporaryDirectory() as directory:
        for group, names in v.GROUPS.items():
            text=(ROOT/'src/rom'/(group+'.c')).read_text()
            locks=json.loads((ROOT/'matched.lock.json').read_text())
            # Fixtures also remain useful if these candidates have since promoted.
            pending,_=v.state(group,text,locks)
            picked=pending[:count]
            paths={fn:PACKET/'sources'/(fn+'.c') for fn in picked}
            rows={fn:context.check(fn,Path(directory),str(path)) for fn,path in paths.items()}
            assert all(row['status']=='ok' for row in rows.values())
            text=v.splice(group,text,paths,rows)
            for fn in picked:
                old=[key for key in locks if key.endswith(':'+fn)]
                assert len(old)==1
                entry=copy.deepcopy(locks.pop(old[0]))
                entry['body_sha256']=v.body_hash(v.bodies(text)[fn])
                locks['src/rom/'+group+'.c:'+fn]=entry
            result[group]=(text,locks)
    return result


def test_actual_asm_processed_translation_units():
    require_toolchain()
    report=v.run()
    assert report['result']=='PASS'
    assert report['candidate_count']==8 and report['candidate_bytes']==1840
    assert report['existing_lock_count']==20
    assert report['host_semantics']=={'macro_cases':96,'stream_cases':50,'passed':True}
    assert [row['tu_functions'] for row in report['groups']]==[66,21]
    assert all(row['negative_differing_words']>0 for row in report['groups'])
    assert all(row['all_function_offsets_unchanged'] for row in report['groups'])


@pytest.mark.parametrize('count',[1,4])
def test_partial_and_full_promotions_keep_proof_active(count):
    require_toolchain()
    report=v.run(fixtures(count))
    assert report['result']=='PASS' and report['temporary_lifecycle_fixture']
    assert all(len(row['already_promoted_candidates'])>=count for row in report['groups'])
    assert len(report['candidates'])==8


def test_changed_promoted_source_fails_even_with_fresh_lock_hash():
    require_toolchain()
    current=fixtures(4)
    text,locks=current['lib_22300']
    text=text.replace('state->field00 = state->field08;', 'state->field00 = state->field0C;')
    locks['src/rom/lib_22300.c:func_80021BF0']['body_sha256']=v.body_hash(v.bodies(text)['func_80021BF0'])
    current['lib_22300']=(text,locks)
    with pytest.raises(ValueError,match='func_80021BF0:'):
        v.run(current)


@pytest.mark.parametrize('group',list(v.GROUPS))
def test_missing_accepted_lock_is_rejected(group):
    text=(ROOT/'src/rom'/(group+'.c')).read_text()
    locks=json.loads((ROOT/'matched.lock.json').read_text())
    del locks['src/rom/'+group+'.c:'+v.EXISTING[group][0]]
    with pytest.raises(ValueError,match='baseline accepted lock missing'):
        v.state(group,text,locks)


@pytest.mark.parametrize('group',list(v.GROUPS))
def test_splice_rejects_missing_or_duplicate_slots(group):
    fn=v.GROUPS[group][0]
    paths={fn:PACKET/'sources'/(fn+'.c')}
    for text in ('',v.pragma(group,fn)+'\n'+v.pragma(group,fn)):
        with pytest.raises(ValueError,match='expected one passthrough'):
            v.splice(group,text,paths,{fn:{'preamble':[]}})


def test_macro_shared_helpers_keep_current_abi_and_packed_views():
    tu=(ROOT/'src/rom/lib_22300.c').read_text()
    shared=re.search(r'typedef struct MacroState \{[^\n]+',tu)[0]
    for fn in v.GROUPS['lib_22300']:
        text=(PACKET/'sources'/(fn+'.c')).read_text()
        assert shared in text
        assert '#pragma pack(1)' in text
        assert '((MacroState *)state' in text
        assert text.splitlines()[0]=='/* flags: '+v.FLAGS+' */'
    marker=v.pragma('lib_22300','func_80023520')
    if marker in tu:
        assert tu.index('extern u8 func_800233B0(')<tu.index(marker)


def test_stream_record_is_identical_across_adapted_paths_and_production():
    text=(ROOT/'src/rom/lib_25bb0.c').read_text()
    expected=re.search(r'typedef struct StreamState_80025264 \{[^\n]+',text)[0]
    for fn in v.GROUPS['lib_25bb0']:
        source=(PACKET/'sources'/(fn+'.c')).read_text()
        assert expected in source
        assert 'typedef struct OSMesgQueue_s {' in source
        assert 'volatile unsigned char busy;' in source
        assert re.search(r'\bsigned char state;',source) and re.search(r'\bsigned char scale;',source)
        assert 'void (*callback)(void *, void *, unsigned int, OSMesgQueue *);' in source
        assert 'extern StreamState_80025264 D_80056230[2];' in source


def test_accepted_function_bodies_and_passthroughs_unchanged():
    for group in v.GROUPS:
        frozen=v.bodies(v.frozen('src/rom/'+group+'.c'))
        current=v.bodies((ROOT/'src/rom'/(group+'.c')).read_text())
        for fn in v.EXISTING[group]:
            assert v.body_hash(frozen[fn])==v.body_hash(current[fn])


@pytest.mark.parametrize('words,symbols,reason',[
    ([1,2,3,4],{'f':0,'neighbor':4},'short function extent'),
    ([1,2,0],{'f':0},'full-TU extent mismatch'),
])
def test_exact_extent_rejects_borrowed_neighbors_and_extra_padding(words,symbols,reason):
    with mock.patch.object(v.score,'targets',return_value={'f':[1,2]}), \
         mock.patch.object(v.score,'text_words',return_value=words), \
         mock.patch.object(v.score,'symbols',return_value=symbols):
        with pytest.raises(ValueError,match=reason):
            v.exact_bytes('unused.o','f')


def test_elf_symbol_size_is_required_even_with_matching_span():
    with mock.patch.object(v.score,'targets',return_value={'f':[1,2]}), \
         mock.patch.object(v.score,'text_words',return_value=[1,2]), \
         mock.patch.object(v.score,'symbols',return_value={'f':0}), \
         mock.patch.object(v.score,'_elf',return_value=(b'',[{'type':2}])), \
         mock.patch.object(v.score,'_text_index',return_value=1), \
         mock.patch.object(v.score,'_symbol_table',return_value=[{'section':1,'type':2,'name':'f','size':4}]):
        with pytest.raises(ValueError,match='ELF function size mismatch'):
            v.exact_bytes('unused.o','f')
