#!/usr/bin/env python3
"""Rebuild complete research source and bounded native/GNU behavior evidence."""
import argparse
import dataclasses
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import random
import shutil
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
spec = importlib.util.spec_from_file_location('checkpoint_native', HERE / 'native.py')
native = importlib.util.module_from_spec(spec)
spec.loader.exec_module(native)
NAME = 'race_countdown_display'
BASE = 0x800D24C8
EXPECTED = '0498412f32c960ca1b267af9cb41677f34375ad415e14ee596d453dc3cf75ad3'
HELPER = 'func_800B61A8'
CONTEXT_BASE = 'dea99f09ab19b1d3b324ed7097162f7b378e7096'
if not __debug__:
    raise RuntimeError('Verification requires Python assertions; do not use -O')


def sha(data): return hashlib.sha256(data).hexdigest()
def words_bytes(words): return b''.join(w.to_bytes(4, 'big') for w in words)
def run(cmd): return subprocess.run(cmd, check=True, capture_output=True)


def elf(obj):
    data, sections = score._elf(obj)
    assert data[:6] == b'\x7fELF\x01\x02'
    header = struct.unpack_from('>HHIIIIIHHHHHH', data, 16)
    assert header[0] == 1 and header[1] == 8 and header[9] == 0
    assert header[6] & 0xF0000000 == 0x10000000, 'MIPS-II ABI required'
    assert header[6] == 0x10000001, 'unexpected pinned-IDO ELF ABI flags'
    syms = [s for i, sec in enumerate(sections) if sec['type'] == 2
            for s in score._symbol_table(data, sections, i)]
    text = score._text_index(sections)
    functions = {s['name']: s for s in syms if s['type'] == 2 and s['section'] == text}
    assert set(functions) == {NAME, HELPER}
    assert functions[HELPER]['value'] == 0 and functions[HELPER]['size'] == 84
    assert functions[NAME]['value'] == 92 and functions[NAME]['size'] == 1116
    assert sections[text]['size'] == 1216
    raw_text = data[sections[text]['off']:sections[text]['off'] + sections[text]['size']]
    assert raw_text[1208:] == bytes(8), 'nonzero alignment tail'
    stub = list(struct.unpack('>2I', raw_text[84:92]))
    assert (stub[0] >> 26, (stub[0] >> 21) & 31, stub[0] & 0x1FFFFF, stub[1]) == (0,31,8,0), 'deleted helper stub'
    assert not [s for s in syms if s['section'] == 0xFFF2], 'common storage'
    shoff, shsize, shnum = header[5], header[10], header[11]
    for i, sec in enumerate(sections):
        row = struct.unpack_from('>10I', data, shoff + shsize * i)
        if row[2] & 2 and sec['name'] != '.text':
            assert sec['name'] in ('.reginfo','.options'), ('owned allocated data',sec['name'])
    addresses = score.image_symbols()
    undefined = {s['name'] for s in syms if s['section'] == 0 and s['name']}
    bindings = {name: addresses.get(name, score.address_named(name)) for name in undefined}
    assert all(value is not None for value in bindings.values())
    relocations = []
    for sec in sections:
        if sec['type'] != 9: continue
        assert sec['name'] == '.rel.text' and sec['info'] == text
        table = score._symbol_table(data, sections, sec['link'])
        for offset in range(sec['off'], sec['off'] + sec['size'], 8):
            site, info = struct.unpack_from('>II', data, offset)
            symbol = table[info >> 8]
            kind = info & 255
            assert kind in (4,5,6) and site % 4 == 0 and site < 1208
            assert symbol['name'] in bindings, ('unexpected internal relocation',symbol)
            relocations.append(dict(offset=site, type=kind, symbol=symbol['name']))
    assert len(relocations) == 56
    return data, functions, bindings, relocations


def compile_and_link(work):
    obj = work / 'candidate.o'
    score.compile_group(HERE / 'group', obj)
    data, functions, bindings, relocations = elf(obj)
    comparison = dataclasses.asdict(score.compare(obj, NAME, show=0))
    helper_comparison = dataclasses.asdict(score.compare(obj, HELPER, show=0))
    assert comparison == dict(differing=60,total=280,unresolved=[],unverified=[],errors=[],extra_words=0)
    assert helper_comparison == dict(differing=0,total=21,unresolved=[],unverified=[],errors=[],extra_words=1)
    text = score.text_words(obj)
    syms = score.image_symbols()
    relocated, masks, unresolved, unverified, errors = score.relocate(obj,text[:],0,len(text)*4,syms)
    assert not unresolved and not unverified and not errors
    assert all(mask==0xFFFFFFFF for mask in masks)
    # Place the entire unchanged object so the target's own STT_FUNC begins at BASE.
    # Kept B61A8 is an 84-byte independent companion followed by an eight-byte
    # deleted inline helper stub. SUBALIGN(4) keeps the target exactly at its
    # four-byte-aligned native address without modifying any object bytes. No local
    # relocations join the two functions. Its complete body is separately equal.
    script = work / 'link.ld'
    script.write_text('SECTIONS { .text 0x%08X : SUBALIGN(4) { *(.text) } }\n' % (BASE - functions[NAME]['value']) +
                      ''.join('%s = 0x%08X;\n' % item for item in sorted(bindings.items())))
    linked = work / 'linked.elf'
    run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(linked),str(obj)])
    raw = work / 'linked.bin'
    run(['mips-linux-gnu-objcopy','-O','binary','--only-section=.text',str(linked),str(raw)])
    linked_text = raw.read_bytes()
    assert linked_text == words_bytes(relocated), 'GNU/canonical complete-object disagreement'
    linked_data, linked_secs = score._elf(linked)
    linked_syms = [s for i,sec in enumerate(linked_secs) if sec['type']==2
                   for s in score._symbol_table(linked_data,linked_secs,i)]
    own = [s for s in linked_syms if s['name']==NAME and s['type']==2]
    assert len(own)==1 and own[0]['value']==BASE and own[0]['size']==1116
    native_words = score.targets()[NAME]
    assert len(native_words)==280 and sha(words_bytes(native_words))==EXPECTED
    assert len(score.targets()[HELPER])==21
    assert relocated[:21] == score.targets()[HELPER]
    target = relocated[23:302]
    gnu = list(struct.unpack('>279I',linked_text[92:1208]))
    assert target == gnu
    return native_words, target, gnu, dict(
        target_bytes=1120, candidate_elf_function_bytes=1116, companion_function_bytes=84,
        full_text_bytes=1216, deleted_inline_stub_bytes=8, zero_alignment_bytes=8, owned_data_bytes=0,
        comparison=comparison, canonical_context_comparison=helper_comparison, actual_retained_helper_elf_body_equal=True,
        full_text_sha256=sha(words_bytes(relocated)), candidate_sha256=sha(words_bytes(target)),
        GNU_whole_object_agreement=True, GNU_target_address=hex(BASE),
        unresolved=[], unverified=[], bindings={k:hex(v) for k,v in sorted(bindings.items())},
        relocations=relocations)


def cases():
    # No pointer aliases; slot0..3 and synthetic backing are explicit preconditions.
    rows=[]
    baseline=[0,2,1,2,3,6,0,0,3,0,0,1,2,1,5,1,0]
    for player in range(4):
        for hook in range(5):
            for place in (-128,-1,0,1,2,3,127):
                for sound in (0,1):
                    row=baseline[:];row[0]=player;row[9]=place;row[11]=sound;row[-1]=hook
                    rows.append(tuple(row))
            for remaining in (-128,-1,0,1,2,3,127):
                for total in (-32768,-1,0,1,2,3,4,32767):
                    row=baseline[:];row[0]=player;row[8]=remaining;row[4]=total;row[-1]=hook
                    rows.append(tuple(row))
    for player in range(4):
        row=baseline[:];row[0]=player;row[4]=4;row[11]=0
        rows.append(tuple(row))
        row=baseline[:];row[0]=player;row[10]=-1
        rows.append(tuple(row))
    rng=random.Random(0xD24C8)
    fields=[range(4),(-32768,-1,0,1,2,4,32767),(-128,0,1,127),
            (-128,-1,0,1,2,3,126,127),(-32768,-1,0,1,2,3,4,32767),
            (0,1,6,255),(0,1,2,3),(0,8,0xFFFFFFFF),
            (-128,-1,0,1,2,3,127),(-128,-1,0,1,2,3,127),(-128,-1,0,1,127),
            (0,1,255),(-32768,-1,0,1,2,4,32767),(-32768,-1,0,1,2,4,32767),
            (-32768,-1,0,1,2,5,32767),(-32768,-1,0,1,2,32767),range(5)]
    rows.extend(tuple(rng.choice(f) for f in fields) for _ in range(1800))
    return rows


def behavior(native_words, project, gnu):
    coverage=[set(),set(),set()];branches=[set(),set(),set()];events=set()
    fixtures=cases()
    for index,case in enumerate(fixtures):
        mem=native.fixture(case)
        results=[native.execute(words,mem,case,index) for words in (native_words,project,gnu)]
        for j,result in enumerate(results):
            coverage[j].update(result[2]);branches[j].update(result[3])
        assert results[0][:2]==results[1][:2]==results[2][:2], ('behavior mismatch',index,case)
        events.update(t[0] for t in results[0][1])
    assert events==set(native.CALLS.values())
    assert coverage[0] == set(range(0,1120,4)), 'incomplete native coverage'
    assert coverage[1] == set(range(0,1116,4)), 'incomplete candidate coverage'
    expected_branches={(4*i,outcome) for i,w in enumerate(native_words)
                       if w>>26 in (4,5,6,7,20,21,22,23) for outcome in (False,True)}
    # Opcode4 with both operands zero is an unconditional branch pseudoinstruction.
    expected_branches={pair for pair in expected_branches if pair[1] or
                       (native_words[pair[0]//4] & 0xFFFF0000) != 0x10000000}
    assert branches[0] == expected_branches, 'incomplete native branch coverage'
    return dict(fixtures=len(fixtures), executions=len(fixtures)*3,
                native_instruction_offsets=sorted(coverage[0]),
                native_words_covered=len(coverage[0]), native_words_total=280,
                candidate_words_covered=len(coverage[1]), candidate_words_total=279,
                native_branch_outcomes=[list(x) for x in sorted(branches[0])],
                helper_boundaries=sorted(events), full_backing_and_call_entry_snapshots_equal=True,
                caller_saved_poisoning=True, saved_registers_and_stack_canaries=True,
                hook_modes=['no mutation','split changes finish checkpoint','lap changes completed lap count',
                            'finish changes place lock/player kind','sound changes model/car flags'])


def replay_controls(work):
    expected=json.loads((HERE/'controls/ledger.json').read_text())
    actual=[]
    for row in expected:
        directory=work/('control_%02d' % row['control']);directory.mkdir()
        (directory/'candidate.c').write_bytes((HERE/row['candidate']).read_bytes())
        (directory/'sound_wrapper.c').write_bytes((HERE/row['wrapper']).read_bytes())
        (directory/'group.json').write_bytes((HERE/'group/group.json').read_bytes())
        obj=directory/'control.o';score.compile_group(directory,obj)
        data,sections=score._elf(obj)
        syms=[sym for i,sec in enumerate(sections) if sec['type']==2
              for sym in score._symbol_table(data,sections,i)]
        function=next(sym for sym in syms if sym['name']==NAME and sym['type']==2)
        frame=-native.signed(score.text_words(obj)[function['value']//4],16)
        value=dict(row,function_bytes=function['size'],frame_bytes=frame,
                   comparison=dataclasses.asdict(score.compare(obj,NAME,show=0)))
        assert value==row, ('control replay changed',row['control'])
        actual.append(value)
    return actual


def context_at_base():
    source_path = 'cloud/matches/func_800B61A8.c'
    expected = run(['git','-C',str(ROOT),'show',CONTEXT_BASE+':'+source_path]).stdout.decode()
    expected = expected[expected.index('typedef unsigned char'):].replace('s32 func_800B61A8(', '__inline s32 func_800B61A8(')
    assert (HERE/'group/sound_wrapper.c').read_text().endswith(expected)
    locks = json.loads(run(['git','-C',str(ROOT),'show',CONTEXT_BASE+':blob_matched.lock.json']).stdout)
    names = ['func_800D2458','func_800D2054','car_setup_confirm','entity_flags_apply','car_stats_display']
    assert all(name in locks for name in names)
    return dict(base_commit=CONTEXT_BASE, helper_source=source_path,
                accepted_callees_at_base=names, live_locks_not_assumed=True)


def toolchain_available():
    return (score.IDO/'cc').is_file() and bool(shutil.which('mips-linux-gnu-ld'))


def verify():
    assert toolchain_available(), 'pinned IDO and MIPS GNU linker required'
    context = context_at_base()
    with tempfile.TemporaryDirectory(prefix='checkpoint-proof-') as tmp:
        words,project,gnu,compiled=compile_and_link(Path(tmp))
        controls=replay_controls(Path(tmp))
        result=behavior(words,project,gnu)
    files=['group/candidate.c','group/sound_wrapper.c','group/group.json','native.py','verify.py','controls/ledger.json'] + sorted(str(p.relative_to(HERE)) for p in (HERE/'controls').glob('*.c'))
    return dict(schema=1, status='NONMATCH', base='dea99f09ab19b1d3b324ed7097162f7b378e7096',
                target=NAME, extent=['0x800D24C8','0x800D2928'],native_sha256=EXPECTED,
                source_hashes={f:sha((HERE/f).read_bytes()) for f in files},
                compiler=compiled,behavior=result, context=context, controls=controls,
                limits=['Bounded disjoint synthetic backing; slots 0..3 only.',
                        'Real called-helper internals are explicit side-effecting hooks.',
                        'No unrestricted aliasing, invalid-pointer, concurrency, gameplay or ROM proof.',
                        'Host-C evidence is provided independently; this producer executes native and source-built MIPS.'])


if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--record',action='store_true');parser.add_argument('--check',action='store_true')
    args=parser.parse_args()
    if not toolchain_available():
        print('SKIP: pinned IDO and MIPS GNU linker required')
        sys.exit(0)
    result=verify();path=HERE/'verification.json'
    if args.record:path.write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    if args.check:assert result==json.loads(path.read_text()),'receipt mismatch'
    print(json.dumps(result,indent=2,sort_keys=True))
