#!/usr/bin/env python3
"""Independent source-token semantics review; intentionally no compiler calls."""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import sys
from source_semantics import Source

HERE = Path(__file__).resolve().parent
BASE = '6b2e9e506fe3d2267a710e41c85af5364ccd00c7'


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


def sha(path): return hashlib.sha256(path.read_bytes()).hexdigest()


def review(packet, reference):
    producer = load('review_input_verifier', packet / 'verify.py')
    native = load('review_input_native', packet / 'native.py')
    code, targets = producer.load_targets(reference)
    sources = [(packet / name).read_text() for name in ('layouts.h', 'cleanup_helpers.c', 'aa8c_entry.c.fragment')]
    source = Source(*sources)
    expected_layouts = {
        'BattleTransform': (0x30, {'matrix': 0, 'position': 0x24}),
        'BattleSlots': (0x10c, {'handles': 0, 'transforms': 0x14, 'active': 0x104, 'timer': 0x108}),
        'BattleGroup': (0x148, {'handles': 0, 'transforms': 0x14, 'extra_handle': 0x104,
            'extra_transform': 0x108, 'state138': 0x138, 'state139': 0x139,
            'state13A': 0x13a, 'value13C': 0x13c, 'value140': 0x140, 'value144': 0x144}),
        'BattleDescriptor': (0x18, {'handle': 6, 'owner': 8}),
        'BattlePlayer': (0x3b8, {'attachment': 0x2f0, 'color': 0x34c, 'mode': 0x384, 'selection': 0x385}),
        'BattlePhysics': (0x808, {'model': 8, 'inhibit': 0x640}),
    }
    for name, (size, fields) in expected_layouts.items():
        actual = source.types[name]
        assert actual.size == size, name
        for field, offset in fields.items(): assert actual.fields[field][1] == offset, (name, field)
    assert source.types['BattleDescriptor'].fields['owner'][0].signed
    assert source.types['BattleDescriptor'].fields['handle'][0].signed
    assert source.types['BattlePhysics'].fields['model'][0].signed is False
    assert source.types['BattlePlayer'].fields['mode'][0].signed

    count, coverage, branches, modes = 0, set(), set(), {'return': 0, 'continue': 0}
    standard = [-1, 0, 0x0000ffff, 0x80000001, 0xffff8000]
    def check(start=native.ROOT, raw_player=None, source_override=source, **args):
        nonlocal count
        machine_fixture, source_fixture = native.Fixture(**args), native.Fixture(**args)
        machine = native.Machine(code, machine_fixture, start, raw_player)
        actual = machine.run()
        name = f'func_{start:08X}'
        predicted = source_override.run(source_fixture, name, raw_player)
        assert actual == predicted, (name, args, actual, predicted)
        assert machine_fixture.memory.snapshot() == source_fixture.memory.snapshot(), (name, args, 'memory')
        assert machine_fixture.trace == source_fixture.trace, (name, args, 'services')
        # At the source level the shared word is evaluated before the guard.
        # This does not claim a compiler retains or orders nonvolatile loads.
        if start == native.ROOT:
            reads = source_fixture.memory.reads
            assert (native.COLOR, 4) in reads
            if args['update'] & 65535:
                pos = (native.PHYSICS + args['player']*0x808 + 0x640, 1)
                assert reads.index((native.COLOR, 4)) < reads.index(pos)
            else:
                assert not any(native.PHYSICS <= a < native.PHYSICS + 4*0x808 for a,w in reads)
        count += 1
        modes[actual] += 1
        coverage.update(machine.coverage)
        branches.update(machine.branches)

    for player in range(4):
        for cached in range(256):
            for update, inhibit in ((0,0), (1,0), (1,1), (1,128)):
                check(player=player,cached=cached,update=update,inhibit=inhibit,handles=standard,extra=0x1234ffff)
        for inhibit in range(256):
            for cached in (0,1):
                check(player=player,cached=cached,update=1,inhibit=inhibit,handles=standard,extra=-1)
        for update in (0,1,2,-1,0x8000,0xffff0000,0x10000,0xffff0001):
            for cached in (0,1,8,9,255):
                for mutation in (0,1,2):
                    check(player=player,cached=cached,update=update,inhibit=0,handles=standard,extra=0x7fffffff,mutation=mutation)
        for start in (native.GROUP_CLEAN,native.SLOT_CLEAN):
            for mask in range(32):
                values = [standard[i] if mask & (1<<i) else -1 for i in range(5)]
                if mask & 1: values[0] = 0x7654ffff
                for extra in (-1,0x0000ffff):
                    for mutation in (0,1,2):
                        check(start,raw_player=0xface0000|player,player=player,cached=0,update=0,inhibit=0,
                              handles=values,extra=extra,mutation=mutation)
    assert count == 8160 and coverage == set(code)
    conditional = {a for a,w in code.items() if w>>26 in (4,5,20) and ((w>>21)&31) != ((w>>16)&31)}
    assert all((a,t) in branches for a in conditional for t in (False,True))
    normal_count = count
    # Extra source-focused descriptor, sign, trailing-state boundary cases.
    for player in range(4):
        for handle in (0,1,32767,32768,65535):
            for state139 in (0,127,128,255):
                for extra in (-1,0,32768,0x0000ffff):
                    check(player=player,cached=0,update=0,inhibit=0,handles=standard,extra=extra,
                          descriptor_handle=handle,state139=state139)
    source_extra_count = count - normal_count
    baseline_modes = modes.copy()
    baseline_count = count
    changes = [
        ('narrow sentinel',1,'handles[i] != -1','(s16) D_80399550[player].handles[i] != -1'),
        ('wrong trailing byte',1,'.state139 = -1','.state138 = -1'),
        ('early store',1,'sound_call_minimal((s16) D_80399550[player].handles[i]);',
         'D_80399550[player].handles[i] = -1; sound_call_minimal((s16) D_80399550[player].handles[i]);'),
        ('wrong cache selector',2,'D_80399118[player]','state->mode'),
        ('unconditional inhibit read',2,'update == 0 || D_8014A250[descriptor->owner].inhibit != 0',
         'D_8014A250[descriptor->owner].inhibit != 0 || update == 0'),
        ('reload state after helper',2,'state->mode = 8;',
         'state = &D_80152818[descriptor->owner]; state->mode = 8;'),
    ]
    rejected = []
    for label,index,old,new in changes:
        changed = sources.copy()
        if label == 'narrow sentinel':
            old = 'D_80399550[player].handles[i] != -1'
        assert old in changed[index]
        changed[index] = changed[index].replace(old,new,1)
        mutated = Source(*changed)
        try:
            check(source_override=mutated,player=2,cached=0,update=0,inhibit=0,
                  handles=[0x0000ffff,1,-1,0x8000,-1],extra=0x1234ffff,mutation=2)
        except AssertionError: rejected.append(label)
        else: raise AssertionError(('source mutation survived',label))
    assert len(rejected) == len(changes)
    return {'status':'INDEPENDENT_BOUNDED_SOURCE_SEMANTICS_REVIEW_PASS', 'base_commit':BASE,
        'native_targets':targets,
        'input_packet_sha256':{n:sha(packet/n) for n in ('layouts.h','cleanup_helpers.c','aa8c_entry.c.fragment','native.py','verify.py')},
        'review_sha256':{n:sha(HERE/n) for n in ('source_semantics.py','verify_review.py')},
        'source_native_pairs':baseline_count,'producer_domain_replayed':normal_count,
        'additional_source_boundary_pairs':source_extra_count,'terminals':baseline_modes,
        'native_instructions':len(coverage),'conditional_branch_outcomes':len(conditional)*2,
        'source_mutations_rejected':rejected,'layout_assertions':{n:{'size':s,'fields':f} for n,(s,f) in expected_layouts.items()},
        'limitations':['owners 0..3 only','source AST abstract interpretation, not compiled C execution',
            'nonvolatile C loads do not guarantee retained machine reads','shared service fixtures; scene internals not executed',
            'no ordinary-ABI private helper build','root prefix stays incomplete','no full-function MATCH or gameplay claim'],
        'compiler_invocations':0,'publication_performed':False}


if __name__ == '__main__':
    ap=argparse.ArgumentParser()
    ap.add_argument('--packet',type=Path,required=True)
    ap.add_argument('--reference-root',type=Path,required=True)
    ap.add_argument('--output',type=Path)
    args=ap.parse_args()
    result=review(args.packet.resolve(),args.reference_root.resolve())
    result=json.dumps(result,indent=2)+'\n'
    if args.output: args.output.write_text(result)
    else: print(result,end='')
