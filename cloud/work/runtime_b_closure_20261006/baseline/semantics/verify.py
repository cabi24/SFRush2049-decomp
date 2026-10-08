#!/usr/bin/env python3
"""Run bounded genuine-root native/linked-candidate closure comparisons."""
import argparse
import hashlib
import json
from pathlib import Path
import sys
sys.dont_write_bytecode=True
from adapter import native, linked, load_score, BASE
from closure import Machine, MEMBERS, ENTRY, INTERNAL, COUNT, PLAYERS, bits


def fixtures():
    yield {'label':'empty','count':0}
    yield {'label':'root_status_only','pressed':0,'status':3}
    for kind in range(9):
        for status in (0,3,11,19):
            yield dict(label=f'root_kind{kind}_status{status}',kind=kind,status=status,frames=2)
    for kind in range(9):
        for collision in (0,1):
            yield dict(label=f'root_four_kind{kind}_collision{collision}',kind=kind,count=4,collision=collision,frames=2)
    for count in (-32768,-1,0,1,4):
        yield dict(label=f'cleanup_count{count}',count=count,pressed=0,debris=4,releases=4)
    for kind in range(9):
        for flags in (0,1,2,0x10,0x20,0x40,0x70,0x80,0xff):
            for collision in (0,1):
                yield dict(label=f'preexisting_kind{kind}_flags{flags}_collision{collision}',pressed=0,
                    records=[dict(kind=kind,flags=flags)],collision=collision,frames=2)
    for life in (0.,0.125,0.1875,0.25):
        for kind in range(9):
            yield dict(label=f'expire_kind{kind}_life{life}',pressed=0,records=[dict(kind=kind,life=life)],frames=2)
    for status in range(32):
        for timer in (0.,0.125,0.25):
            yield dict(label=f'status{status}_timer{timer}',pressed=0,status=status,transition_timer=timer,scale_timer=timer)
    for mask in (0,2,0x7f,0x80,0xfe,0xff):
        for scale in (2.,8.,8.25,20.,20.25):
            yield dict(label=f'kind7_mask{mask}_scale{scale}',pressed=0,count=4,
                records=[dict(kind=7,hit_mask=mask)],scene_scale=scale,frames=2)
    for kind in (0,1,2,3,4,6,7,8):
        for collision in (0,1):
            yield dict(label=f'callback_mutation_kind{kind}_collision{collision}',kind=kind,count=4,
                collision=collision,mutation=1,frames=2)
    for kind in (0,1,3):
        yield dict(label=f'allocation_fail_kind{kind}',kind=kind,allocation_fail=True)
    for failure in ('debris_fail','quad_fail'):
        yield dict(label=failure,kind=1,collision=1,frames=2,**{failure:True})
    for count in (0,1,3,4):yield dict(label=f'callback_count{count}',count=1,callback_count=count,frames=2)


    # Focused branches missing after the initial 410-case genuine-root batch.
    for kind in range(9):
        for key,values in [('ammo',(0,-1)),('cooldown',(0.0625,0.25)),
            ('pressed',(0,2)),('active',(0,)),('blocked',(-1,)),
            ('vehicle_blocked',(-1,)),('held',(1,))]:
            for value in values:
                yield dict(label=f'root_gate_kind{kind}_{key}{value}',kind=kind,**{key:value})
    for model in (0,6,12):
        yield dict(label=f'kind5_bounds_model{model}',kind=5,ammo=0,model=model)
    for status in (3,11,19):
        for key,values in [('vehicle_blocked',(-1,)),('scale',(0.05,0.06,2.)),
            ('alpha',(0,47,48,255)),('transition',(2,))]:
            for value in values:
                yield dict(label=f'status{status}_{key}{value}',status=status,pressed=0,status_scene=5,**{key:value})
    for count in (0,1,4):
        yield dict(label=f'preexisting_count{count}',count=count,pressed=0,records=[dict(kind=0)])
    for key,values in [('vehicle_state',(0,)),('active',(0,)),('vehicle_blocked',(-1,)),
        ('position',((0.,100.,1.),(100.,0.,1.),(0.,0.,-1.)))]:
        for value in values:
            yield dict(label=f'collision_skip_{key}{value}',count=2,pressed=0,
                players=[{},dict(**{key:value})],records=[dict(kind=0)])
    for angle in (-2.,-1.35,0.,1.35,2.):
        yield dict(label=f'shield_angle{angle}',count=2,pressed=0,angle=angle,
            players=[{},dict(kind=5,position=(0.,0.,5.))],records=[dict(kind=0)])
    for key,values in [('active',(0,)),('vehicle_state',(0,))]:
        for value in values:
            yield dict(label=f'kind7_skip_{key}{value}',count=2,pressed=0,
                players=[{},dict(**{key:value})],records=[dict(kind=7)])
    for latched in (1,2):
        for key in ('blocked','vehicle_blocked'):
            yield dict(label=f'latched{latched}_{key}',kind=3,latched=latched,pressed=0,
                records=[dict(kind=3,timer=0.)],**{key:1})
    yield dict(label='zero_delta_collision',pressed=0,dt=0.,records=[dict(kind=0)])
    yield dict(label='kind7_zero_count',count=0,pressed=0,records=[dict(kind=7)])


def run_frames(machine,case):
    outputs=[]
    for frame in range(case.get('frames',1)):
        machine.frame=frame;machine.reset_registers()
        outputs.append(machine.run()[0])
    return outputs,machine.trace


def coverage_report(code,covered,identities):
    result={}
    for name,m in identities.items():
        start=int(m['address'],16);size=m['size'];addresses=set(range(start,start+size,4))
        result[name]={'executed_instructions':len(addresses&covered),'total_instructions':size//4,
            'unexecuted_offsets':[hex(a-start) for a in sorted(addresses-covered)]}
    return result


def verify(native_code,native_data,candidate_code,start,candidate_data,cases=None,native_meta=None,candidate_meta=None):
    report={'status':'PASS','paired_cases':0,'root_invocations_per_image':0,'failures':[],
        'internal_calls_hooked':False,'external_helpers':'bounded deterministic effect models, not game-service equivalence',
        'domain':'FCE0 root; signed count <=4; F938 owner0..3; valid indices; initialized aligned disjoint storage; real singly-linked active/free pool nodes; successful required object/release allocations; finite normal-or-zero binary32/default rounding; D498 distance strictly nonzero on impulse paths; no exception/FCSR/subnormal model'}
    nc,cc,nb,cb,ne,ce=set(),set(),set(),set(),set(),set();digest=hashlib.sha256();external=set()
    for case in fixtures() if cases is None else cases:
        a=b=None
        try:
            a=Machine(native_code,ENTRY,native_data,case,True)
            b=Machine(candidate_code,start,candidate_data,case,False)
            aa=run_frames(a,case);bb=run_frames(b,case)
            if aa!=bb:
                changed=[(hex(x),a.memory[x],b.memory[x]) for x in a.state_order if a.memory[x]!=b.memory[x]]
                difference=next(((i,x,y) for i,(x,y) in enumerate(zip(a.trace,b.trace)) if x!=y),None)
                raise AssertionError(('semantic mismatch',difference,'trace_lengths',len(a.trace),len(b.trace),'changed_bytes',changed[:20]))
            digest.update(repr(aa).encode());report['paired_cases']+=1
            report['root_invocations_per_image']+=case.get('frames',1)
        except (AssertionError,ZeroDivisionError,ValueError,OverflowError) as exc:
            report['status']='FAIL-CLOSED';report['failures'].append({'case':case,'reason':str(exc)})
        for machine,cov,branch,edge in ((a,nc,nb,ne),(b,cc,cb,ce)):
            if machine:
                cov.update(machine.coverage);branch.update(machine.branches);edge.update(machine.internal_edges)
                external.update(x[0] for x in machine.trace)
    report.update(behavior_sha256=digest.hexdigest(),native_executed_instructions=len(nc),candidate_executed_instructions=len(cc),
        native_branch_outcomes=len(nb),candidate_branch_outcomes=len(cb),external_services=sorted(external),
        native_internal_edges=[[hex(a),hex(b)] for a,b in sorted(ne)],candidate_internal_edges=[[hex(a),hex(b)] for a,b in sorted(ce)],
        native_coverage=coverage_report(native_code,nc,native_meta or {}),candidate_coverage=coverage_report(candidate_code,cc,candidate_meta or {}))
    return report


def main():
    p=argparse.ArgumentParser();p.add_argument('--reference-root',type=Path,required=True)
    p.add_argument('--linked-elf',type=Path);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--quick',action='store_true')
    p.add_argument('--boundaries',type=Path,default=Path(__file__).resolve().parent.parent/'boundaries.json')
    p.add_argument('--source',type=Path,default=Path(__file__).resolve().parent.parent/'closure.c')
    args=p.parse_args()
    score=load_score(args.reference_root);nc,ns,nd,nm=native(args.reference_root,score)
    binding={}
    if args.linked_elf:
        cc,cs,cd,cm=linked(score,args.linked_elf)
        boundaries=json.loads(args.boundaries.read_text())
        elf_sha=hashlib.sha256(args.linked_elf.read_bytes()).hexdigest()
        source_sha=hashlib.sha256(args.source.read_bytes()).hexdigest()
        assert elf_sha==boundaries['linked_sha256'], 'linked candidate fingerprint mismatch'
        assert source_sha==boundaries['source_sha256'], 'candidate source fingerprint mismatch'
        assert set(boundaries['functions'])==set(nm), 'incomplete procedure boundary inventory'
        cm=boundaries['functions']
        assert int(cm['func_8038FCE0']['address'],16)==cs
        body=set()
        for member in cm.values():
            begin=int(member['address'],16);extent=set(range(begin,begin+member['size'],4))
            assert extent<=set(cc) and not(body&extent), 'invalid procedure boundaries'
            body.update(extent)
        assert all(cc[a]==0 for a in set(cc)-body), 'nonzero text outside complete procedures'
        binding={'source_sha256':source_sha,'linked_elf_sha256':elf_sha,
            'boundaries_sha256':hashlib.sha256(args.boundaries.read_bytes()).hexdigest(),
            'candidate_function_bytes':sum(m['size'] for m in cm.values()),
            'candidate_text_padding_bytes':4*len(set(cc)-body)}
    else:cc,cs,cd,cm=nc,ns,[],nm
    cases=list(fixtures());cases=cases[:3] if args.quick else cases
    result=verify(nc,nd,cc,cs,nd+cd,cases,nm,cm)
    result.update(binding=binding,
        verifier_source_sha256={name:hashlib.sha256(Path(__file__).with_name(name).read_bytes()).hexdigest() for name in ('closure.py','adapter.py','verify.py','controls.py')},
        reproduction_arguments=sys.argv[1:],base=BASE,native_authentication=nm,candidate='linked ELF' if args.linked_elf else 'NATIVE SELF-REPLAY ONLY; NOT CANDIDATE VERIFICATION')
    args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:result[k] for k in ('status','paired_cases','root_invocations_per_image','failures')},indent=2))
    return 0 if result['status']=='PASS' else 1

if __name__=='__main__':raise SystemExit(main())
