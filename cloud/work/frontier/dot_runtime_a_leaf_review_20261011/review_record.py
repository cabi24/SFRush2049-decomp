#!/usr/bin/env python3
"""Peer review of E200: separate ELF/GNU route and adversarial list topology.

The author owns E200's deliverable. This script independently recompiles and
links it, reviews the existing executable tests, and tests an independent
physical-node-order oracle. No original bytes or assembly are persisted.
"""
import argparse
import dataclasses
import hashlib
import importlib.util
import itertools
import json
from pathlib import Path
import random
import subprocess
import sys
import tempfile
import time

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
DEFAULT=ROOT/'cloud/work/frontier/dot_runtime_a_record_context_20261011'
sys.path.insert(0,str(HERE))
import review
from elf_reader import elf

require=review.require
EXPECTED='c7ea7c6f1d1b8595882f3b2776579b6226be8d77570c6195c81c67c42128f651'


def load(name,path):
    spec=importlib.util.spec_from_file_location(name,path)
    module=importlib.util.module_from_spec(spec);sys.modules[name]=module;spec.loader.exec_module(module)
    return module


def peer(group):
    start=time.monotonic();source=(group/'group.c').read_text()
    require(hashlib.sha256(source.encode()).hexdigest()==EXPECTED,'unreviewed source revision')
    score=load('record_peer_score',ROOT/'tools/cloud/score.py');score.ASM_DIR=ROOT/'asm/us/ovl_a'
    targets=score.targets();sem=load('record_peer_semantics',group/'semantics.py')
    config=json.loads((group/'group.json').read_text())
    require(config['keep']==['func_803A1EAC'] and config['claims']==['func_8039E200'],'claim/root drift')
    names={'func_8039E200':444,'func_8039E3BC':136,'func_8039D494':528,'func_8039ED20':480}
    saved=review.SIZES;review.SIZES=names
    with tempfile.TemporaryDirectory(prefix='record-peer-') as td:
        temp=Path(td);obj=temp/'candidate.o';score.compile_group(group,obj)
        try:linked,relocations=review.gnu_link(score,obj,temp)
        finally:review.SIZES=saved
        scores={}
        for name,size in names.items():
            require(linked[name]==targets[name] and len(linked[name])*4==size,'independent full body mismatch')
            result=score.compare(obj,name,show=0);require(result.accepted(),'strict match failed')
            scores[name]=dataclasses.asdict(result)
        callsites=sem.audit_calls(obj,score,elf,targets)
        # Independently constructed ordering model: physical handles need not
        # be in the order they appear in the singly linked list.
        rng=random.Random(0xE200933)
        original_state=sem.state
        cases=[]
        for order in itertools.permutations(range(6)):
            for direction in (-1,0,1):
                available=list(order)+[6,7,8]
                start_node=rng.choice([-1]+available)
                records=[(3,2,rng.randrange(2),rng.choice((0,1,0xFFFFFFFF)),rng.randrange(2),rng.randrange(4)) for _ in range(9)]
                selected=tuple(rng.choice([-1]+available) for _ in range(3))
                cases.append((list(order),(6,start_node,direction,3,2,selected,records)))
        for order in ([],[5],[2,5],[5,0,3],[4,2,0,5]):
            for direction in (-128,0,127):
                for start_node in [-1]+order+[6,7,8]:
                    records=[(3,2,1,int(i==rng.choice(order+[6,7,8])),1,i%4) for i in range(9)]
                    cases.append((order,(len(order),start_node,direction,3,2,(-1,-1,-1),records)))
        coverage=set();calls=0;rewrites=0
        for index,(order,case) in enumerate(cases):
            memory=original_state(case)
            sem.write(memory,sem.HEAD,4,sem.handle(order[0]) if order else 0)
            sem.write(memory,sem.TAIL,4,sem.handle(order[-1]) if order else 0)
            for node in range(9):
                next_node=order[order.index(node)+1] if node in order and order.index(node)+1<len(order) else -1
                for extra in (0,64):sem.write(memory,sem.OBJECTS+128*node+extra,4,sem.handle(next_node))
            sem.state=lambda unused,memory=memory:dict(memory)
            a=sem.run(targets['func_8039E200'],case,index)
            b=sem.run(linked['func_8039E200'],case,index)
            require(a==b,('topology trace differs',index))
            current=case[1]
            if current<0:current=(order[0] if order else -1) if case[2]>0 else 8
            ordered=order+[6,7,8]
            candidates=[] if current<0 else ordered[ordered.index(current):] if case[2]>0 else ordered[:ordered.index(current)+1][::-1]
            if case[2]==0 and current==8:candidates=[8]
            expected=-1;expected_calls=[];expected_rewrites=[]
            for node in candidates:
                track,mode,file,active,callback,mutation=case[6][node]
                if track!=case[3] or mode!=case[4] or node in case[5]:continue
                if file:
                    expected_calls.append(node)
                    if mutation:expected_rewrites.append(node)
                    if not callback:continue
                    if mutation:active=int(mutation==2)
                if active:expected=node;break
            require(a['return']==sem.handle(expected) and a['calls']==expected_calls,('independent ordering oracle failed',index,order))
            allowed=set(range(sem.SP-24,sem.SP))|set(range(sem.SP+4,sem.SP+8))
            allowed|={sem.handle(node)+j for node in expected_rewrites for j in range(4)}
            require(all(a['memory'][address]==value for address,value in memory.items() if address not in allowed),'peer guard corruption')
            require(all(all(address+j in allowed for j in range(size)) for address,size,_ in a['writes']),'peer invalid write')
            coverage.update(a['coverage']);calls+=len(expected_calls);rewrites+=len(expected_rewrites)
        sem.state=original_state
    # Re-run the separately authored verifier rather than trusting its receipt.
    process=subprocess.run([sys.executable,str(group/'verify.py')],capture_output=True,text=True)
    require(process.returncode==0,('author replay failed',process.stdout,process.stderr))
    author=json.loads(process.stdout)
    require(author['source_sha256']==EXPECTED and author['semantics']['host_C89_UBSan_bounds_cases']==6546,'author evidence drift')
    return dict(status='PASS',source_sha256=EXPECTED,claim='func_8039E200',new_candidate_bytes=444,accepted_image_bytes=0,
                independent_scores=scores,independent_gnu_relocations=relocations,
                full_444_byte_extent=True,all_claimed_relocation_fields_resolved=True,
                support_sha256={str(p.relative_to(group)) if p.is_relative_to(group) else str(p.relative_to(ROOT)):
                                hashlib.sha256(p.read_bytes()).hexdigest()
                                for p in (group/'semantics.py',group/'verify.py',HERE/'review_record.py',HERE/'review.py',HERE/'elf_reader.py')},
                separately_reexecuted_author_cases=6546,
                independently_constructed_topology_cases=len(cases),linked_six_node_permutations=720,
                topology_instructions_covered=len(coverage),topology_external_calls=calls,topology_handle_rewrites=rewrites,
                callsites=callsites,
                reviewed_layout_evidence=['src/blob/audio_volume_pan.c uses the same 16-byte list header at D_80152020 with head/tail offsets8/12.',
                                         'Author MIPS-compiled layout probe establishes the actual RecordList,Record,Player,Slot and PanelPart sizes/offsets.',
                                         'Changing scalar head/tail declarations to a common owner changes genuine alias/ownership context; it does not fabricate storage locals.'],
                abi=['Inputs s1=node,s2=low signed byte of direction; result v0.',
                     'Callee writes its direction home at incoming sp+4 and clobbers private s0-s6.',
                     'Three native and three generated A1EAC callsites preserve s5/s6 around the call.'],
                findings=['All source deltas are bound to the prior source by the author verifier; D05C remains explicitly unclaimed.',
                          'The post-callback handle reload is exercised by replacement-object cases.',
                          'Direction zero differs from negative at the third synthetic handle; both paths are exercised.',
                          'Unsigned record mode is compared to a signed global mode; negative global modes are not silently zero-extended.'],
                limits=['The topology interpreter is the reviewed author decoder; the topology construction/oracle and ELF/GNU route are separate.',
                        'Additional topology cases execute native/GNU routes; host C89 coverage is the separately rerun 6546-case author set.',
                        'External predicate behavior is fixture-bounded; its implementation is not claimed.',
                        'No full A1EAC execution, full-image layout, compression, cartridge or gameplay claim.'],
                elapsed_seconds=round(time.monotonic()-start,3))


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--group',type=Path,default=DEFAULT);parser.add_argument('--output',type=Path)
    args=parser.parse_args();result=peer(args.group)
    if args.output:args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result,indent=2))
