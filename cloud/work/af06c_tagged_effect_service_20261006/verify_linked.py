"""Replay an existing linked object; no compiler or source edits are permitted."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from inputs import load,BASE
from fixture import Fixture
from native import Machine
from compiled_machine import Machine as Compiled
HERE=Path(__file__).resolve().parent

def linked(path):
    raw=path.read_bytes();header=struct.unpack_from('>16sHHIIIIIHHHHHH',raw)
    assert header[0][:6]==b'\x7fELF\x01\x02' and header[1]==2 and header[2]==8
    sections=[struct.unpack_from('>10I',raw,header[6]+i*header[11]) for i in range(header[12])]
    names=sections[header[13]];strings=raw[names[4]:names[4]+names[5]]
    code={};entry=None;own=[]
    for sec in sections:
        name=strings[sec[0]:].split(b'\0')[0].decode()
        if sec[1] in (4,9):assert sec[5]==0
        if sec[2]&2 and sec[5]:
            data=raw[sec[4]:sec[4]+sec[5]]
            if sec[2]&4:
                code.update({sec[3]+4*i:w[0] for i,w in enumerate(struct.iter_unpack('>I',data))})
            else:
                assert not sec[2]&1,('unexpected candidate writable data',name)
                own.append((sec[3],data))
        if sec[1]==2:
            strsec=sections[sec[6]];strtab=raw[strsec[4]:strsec[4]+strsec[5]]
            for off in range(0,sec[5],16):
                n,value,size,info,other,shndx=struct.unpack_from('>IIIBBH',raw,sec[4]+off)
                symbol=strtab[n:].split(b'\0')[0].decode()
                if symbol=='save_write_data':entry=value
                assert not symbol or shndx!=0,('unresolved linked symbol',symbol)
    assert entry is not None
    return code,entry,own

def compare(original,compiled,entry,image,own,options,coverage,branches):
    expected,actual=Fixture(image,**options),Fixture(image,**options)
    for a,raw in own:actual.memory.regions.append((a,bytearray(raw)))
    Machine(original,expected).run();machine=Compiled(compiled,actual,entry);machine.run()
    assert actual.trace==expected.trace,('linked ordered boundary mismatch',options,actual.trace,expected.trace)
    snapshot=actual.memory.snapshot()
    if own:snapshot=tuple((a,b) for a,b in snapshot if a not in {x[0] for x in own})
    for (a,x),(b,y) in zip(snapshot,expected.memory.snapshot()):
        assert a==b
        if x!=y:
            i=next(i for i in range(len(x)) if x[i]!=y[i]);raise AssertionError(('linked state mismatch',options,hex(a+i),x[i:i+16].hex(),y[i:i+16].hex()))
    coverage.update(machine.coverage);branches.update(machine.branches)

def run(reference,elf):
    original,image,identities=load(reference);compiled,entry,own=linked(elf)
    coverage,branches=set(),set();count=0
    def check(**options):
        nonlocal count
        compare(original,compiled,entry,image,own,options,coverage,branches);count+=1
    for mode in (0,1,-1,2):
        for index in range(6):
            for player_count in (-1,0,1,3,4,6):
                for alloc in ((False,False),(False,True),(True,False),(True,True)):
                    check(mode=mode,index=index,count=player_count,alloc=alloc)
    for index in range(6):
        for player_count in (0,3,4,6):
            for alloc in ((False,),(True,)):
                for old_scene in (-1,0,23,0x12340002):
                    check(shortcut=True,index=index,count=player_count,alloc=alloc,old_scene=old_scene)
    for ring in (0,1,48,49):
        for mode in (0,1):
            for enabled in (0,1,-1):
                for scale in (-2.,0.,.25,.49999997,.5,.50000006,.99999994,1.,1.00000012,5.):
                    check(mode=mode,count=4,ring=ring,enabled=enabled,scale=scale,sound=1)
    return dict(status='PASS: bounded existing-linked-output/native agreement; NOT STRICT MATCH',
        base=BASE,fixtures=count,compiled_instructions_covered=len(coverage),branch_outcomes=len(branches),
        elf_sha256=hashlib.sha256(elf.read_bytes()).hexdigest(),target_compiler_invocations=0,
        packet_sha256={p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in (HERE/'candidate.c',HERE/'compiled_machine.py',HERE/'verify_linked.py')})
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--reference-root',type=Path,required=True)
    p.add_argument('--elf',type=Path,required=True);p.add_argument('--output',type=Path,default=HERE/'linked-semantics.json');a=p.parse_args()
    result=run(a.reference_root,a.elf);a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
