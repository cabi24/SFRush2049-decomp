#!/usr/bin/env python3
"""Actual-source sanitizer, canonical native, and compiled-candidate fixtures."""
import argparse
import hashlib
import itertools
import json
import os
from pathlib import Path
import random
import struct
import subprocess
import tempfile
from native_replay import execute, signed
from verify import ROOT, WORK, NAME, FLAGS, SOURCE, TABLE, build_link, elf_sections, digest, score
VOICE, COMMAND = 0x100000, 0x200000


def expected(case):
    op,w0,w1,left,right,mutate_at,new0,new1=case
    reads=[((w0>>24)&255,w1&255)]
    if mutate_at==1: w0,w1=new0,new1
    if op==4: right=signed(w1>>8,16)
    else:
        reads.append(((w1>>8)&255,(w1>>16)&255))
        if mutate_at==2: w0,w1=new0,new1
    if op in (0,4): value=left+right
    elif op==1: value=left-right
    elif op==2: value=left*right
    elif op==3: value=0 if right==0 else (abs(left)//abs(right))*(-1 if (left<0)!=(right<0) else 1)
    else: raise AssertionError('undefined operation must not reach C oracle')
    value=max(-32768,min(32767,value))
    selectors=[v for pair in reads for v in pair]
    if len(reads)==1: selectors.extend((0,0))
    return [0,len(reads),1]+selectors+[(w0>>8)&255,(w0>>16)&255,value,w0,w1]


def run_native(words,table,case,upper=0,stack_fill=0x5A):
    op,w0,w1,left,right,mutate_at,new0,new1=case
    calls=[]
    def helper(destination,args,memory,index):
        assert args[0]==VOICE
        if destination==0x80023AD4:
            assert len(calls)<2
            event=('read',args[1]&255,args[2]&255)
            value=left if not calls else right
            if mutate_at==len(calls)+1:
                memory(COMMAND,4,new0);memory(COMMAND+4,4,new1)
            calls.append(event)
            return value,event
        assert destination==0x80023B50
        event=('write',args[1]&255,args[2]&255,signed(args[3],16))
        calls.append(event)
        return 0xBAADF00D,event
    result=execute(words,{VOICE:bytes(32), COMMAND:struct.pack('>II',w0,w1),
                         TABLE:struct.pack('>5I',*table)},helper,[VOICE,COMMAND,op|upper],stack_fill)
    reads=[x for x in calls if x[0]=='read'];writes=[x for x in calls if x[0]=='write']
    assert len(writes)==1 and calls[-1][0]=='write'
    selectors=[v for x in reads for v in x[1:]]
    if len(reads)==1: selectors.extend((0,0))
    updated=struct.unpack('>II',result['regions'][COMMAND])
    output=[result['result'],len(reads),len(writes)]+selectors+list(writes[0][1:])+list(updated)
    return output,result


def fixtures():
    edges=(-32768,-32767,-16384,-2,-1,0,1,2,16383,16384,32766,32767)
    for op,left,right in itertools.product(range(5),edges,edges):
        word1=0xDA00001F | ((right&65535)<<8) if op==4 else 0xDAFEC21F
        yield (op,0xC4EDAB55,word1,left,right,0,0,0)
    # Immediate signed conversion is tested for every 16-bit encoding.
    for raw in range(65536):
        yield (4,0xA0B1C233,0x5A0000FF|(raw<<8),(-32768,0,32767)[raw%3],0,0,0,0)
    # Every controller/index byte and both read-side live command reloads.
    for field in range(256):
        for op in range(5):
            w0=field*0x01010101;w1=(255-field)*0x01010101
            for mutate_at in (0,1,2):
                yield (op,w0,w1,-32768,(-1,0,1)[field%3],mutate_at,w0^0xF17D29C3,w1^0xACE09B67)
    rng=random.Random(0x23BDC)
    for _ in range(2048):
        yield (rng.randrange(5),rng.getrandbits(32),rng.getrandbits(32),
               rng.randrange(-32768,32768),rng.randrange(-32768,32768),rng.randrange(3),
               rng.getrandbits(32),rng.getrandbits(32))


def run():
    score.ASM_DIR=ROOT/'asm/us/boot_tail'
    words=score.targets()[NAME]
    proof=json.loads((WORK/'mapping_proof.json').read_text())
    native_table=[int(x['target'],16) for x in proof['selector_to_target']]
    cases=list(fixtures()); wanted=[expected(case) for case in cases]
    with tempfile.TemporaryDirectory(prefix='bt05-macro-fixtures-') as temp:
        directory=Path(temp);exe=directory/'host'
        command=['gcc','-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror',
                 '-Wno-maybe-uninitialized','-O2','-fsanitize=address,undefined','-no-pie',
                 str(WORK/'host_fixture.c'),'-o',str(exe)]
        subprocess.run(command,check=True,capture_output=True)
        env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1')
        output=subprocess.run([str(exe)],input=''.join(' '.join(map(str,x))+'\n' for x in cases),
            capture_output=True,text=True,check=True,env=env)
        host=[list(map(int,line.split())) for line in output.stdout.splitlines()]
        assert host==wanted
        obj,linked=build_link(SOURCE,FLAGS,directory)
        _,_,sections=elf_sections(linked)
        compiled=list(struct.unpack('>'+str(len(sections['.text'])//4)+'I',sections['.text']))
        compiled_table=list(struct.unpack('>5I',sections['.rodata'][:20]))
        native_visited=set();candidate_visited=set()
        for index,(case,want) in enumerate(zip(cases,wanted)):
            upper=(0,0x12340000,0xFFFFFF00)[index%3]
            for body,table,visited in ((words,native_table,native_visited),
                                        (compiled,compiled_table,candidate_visited)):
                got,result=run_native(body,table,case,upper)
                assert got==want,(case,got,want)
                assert not result['uninitialized_stack_reads']
                assert [a for a,n in result['reads'] if TABLE<=a<TABLE+20]==[TABLE+4*case[0]]
                visited.update(result['visited'])
        # Undefined native selectors: prove guard bypass and uninitialized result.
        # No host-C execution or defined-source equivalence is claimed here.
        defaults=0
        for op,fill in itertools.product(range(5,256),(0,0x7F,0x80,0xFF)):
            case=(op,0xABCDEF12,0xFEE0CD13,-32768,-1,0,0,0)
            got,result=run_native(words,native_table,case,0x87650000,fill)
            assert len(result['uninitialized_stack_reads'])==1
            assert result['uninitialized_stack_reads'][0]==(0x700000+248,4)
            assert not any(TABLE<=a<TABLE+20 for a,n in result['reads'])
            assert got[9]==max(-32768,min(32767,signed(fill*0x01010101)))
            defaults+=1;native_visited.update(result['visited'])
        # Arithmetic trap blocks and scheduler-duplicated instructions are not live.
        missed=[hex(0x80023BDC+4*i) for i in range(101) if 0x80023BDC+4*i not in native_visited]
        return dict(result='PASS',source_sha256=digest(SOURCE),test_sha256=digest(__file__),
            native_replay_sha256=digest(WORK/'native_replay.py'),host_fixture_sha256=digest(WORK/'host_fixture.c'),
            host_actual_source_calls=len(cases),canonical_native_valid_calls=len(cases),
            compiled_candidate_valid_calls=len(cases),native_undefined_default_cases=defaults,
            immediate_encodings=65536,all_controller_and_index_bytes=True,
            live_helper_command_mutations=True,caller_saved_clobbers=True,
            native_valid_or_default_instruction_pcs_covered=len(native_visited),
            native_unvisited_pcs=missed,sanitizers=['address','undefined'],
            limitations=['Synthetic two-helper contracts; no transitive helper implementation claim.',
                'Defined C and behavioral equality require operation 0..4, aligned live command storage, and valid helper contracts.',
                'Native defaults depend on uninitialized stack storage; host C is not executed for those selectors.',
                'No malformed-stream, hardware, full engine, production link or ROM coverage claim.'])


if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--check',action='store_true');args=parser.parse_args()
    result=run()
    if args.check:
        assert result==json.loads((WORK/'semantic_verification.json').read_text())
        print('PASS: native/candidate replay, all immediate encodings, sanitizer and default-domain fixtures.')
    else: print(json.dumps(result,indent=2))
