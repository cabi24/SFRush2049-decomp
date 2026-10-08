#!/usr/bin/env python3
"""Complete NONMATCH and actual caller-footprint proof, never a splice claim."""
import argparse, contextlib, hashlib, importlib.util, io, json, os, struct, subprocess, sys
from pathlib import Path
from elf_support import elf
import native
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
NAME='func_8039D494'
NATIVE_HASHES={'func_8039D494':'613dae3c390ee99c67793f916883023f1ab829aaf20b077272c80cc36a2a93fa','func_8039E3BC':'c56db54a7f5e9b363c23161f3f2049a5593a0343d19b6edaf1926e2e6abebb85','func_8039D6A4':'357a46728db7402e79d58a22ba9cf9d39187b7ceccc4a6c91db1e8abb92b5343'}
FLAGS='-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
ANCHORS={n:int(n[2:],16) for n in ('D_801407D0','D_80156994','D_8014978C','D_803AF980','D_8014A110','D_801164BE','D_803B9FD0','D_803B3020')}
def sha(b):return hashlib.sha256(b).hexdigest()
def packed(words):return struct.pack('>%dI'%len(words),*words)
def shell(*args,**kwargs):
    p=subprocess.run([str(x) for x in args],text=True,capture_output=True,**kwargs)
    assert p.returncode==0,(args,p.returncode,p.stdout,p.stderr)
    assert 'runtime error:' not in p.stderr,p.stderr
    return p.stdout

def inspect(obj,linked=False):
    sec,sym,rel=elf(obj);index,row,raw=sec['.text']
    defined={n:s for n,s in sym.items() if s[2]==2 and s[3] not in (0,0xfff1)}
    assert set(defined)=={NAME}
    assert sym[NAME]==(native.ENTRY if linked else 0,528,2,index)
    assert row[3]==(native.ENTRY if linked else 0) and len(raw)==528
    for n,(_,r,b) in sec.items():
        if r[2]&2 and n not in ('.text','.reginfo','.options'):assert r[5]==0,n
    if linked:
        assert not rel
        for n,a in ANCHORS.items():assert sym[n][0]==a and sym[n][3]==0xfff1
    else:
        assert len(rel)==22
        assert all(o<528 and o%4==0 and t in (5,6) and s in ANCHORS and target==index for o,t,s,target in rel)
        assert {s for _,_,s,_ in rel}==set(ANCHORS)
    return raw,rel

def host(build,source,label,case_list):
    driver=build/(label+'_host.c');driver.write_text((HERE/'host.c').read_text().replace('#include "candidate.c"','#include "'+str(source)+'"'))
    out=build/(label+'_host');shell('gcc','-std=c89','-pedantic-errors','-O2','-fsanitize=undefined,bounds','-fno-sanitize-recover=all',driver,'-o',out)
    result=shell(out,input=''.join(' '.join(map(str,c))+'\n' for c in case_list))
    values=[int(v) for v in result.splitlines()];assert len(values)==len(case_list)
    return values

def verify(repo=ROOT,tools_repo=ROOT):
    build=ROOT/'build/availability_boundary';build.mkdir(parents=True,exist_ok=True)
    source=HERE/'candidate.c';sys.path.insert(0,str(tools_repo/'tools/cloud'))
    spec=importlib.util.spec_from_file_location('availability_score',tools_repo/'tools/cloud/score.py');score=importlib.util.module_from_spec(spec);sys.modules[spec.name]=score;spec.loader.exec_module(score)
    prior_state=(score.ASM_DIR,score._targets,score._target_fingerprint,dict(score._own_data))
    try:
        score.ASM_DIR=repo/'asm/us/ovl_a';manifest=score.target_manifest();targets=score.targets();addresses=score.image_symbols()
        meta=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest));assert meta['image']=='A' and meta['rom_offset']=='0xB5C534' and meta['base']=='0x8038A400'
        bindings={}
        for name,size in ((NAME,528),('func_8039E3BC',136),('func_8039D6A4',2908)):
            entry=int(name[-8:],16);row=next(f for f in meta['functions'] if f['name']==name)
            assert row['address']==hex(entry).upper().replace('0X','0x') and row['size']==size
            assert addresses.get(name,score.address_named(name))==entry and len(targets[name])*4==size
            assert sha(packed(targets[name]))==NATIVE_HASHES[name]
            bindings[name]={'address':hex(entry),'bytes':size,'sha256':sha(packed(targets[name])),'entry_evidence':row['evidence']}
        for n,a in ANCHORS.items():assert addresses.get(n,score.address_named(n))==a,n
        layout=build/'types.c';layout.write_text('#include \"'+str(source)+'\"\n'+'typedef char c8[sizeof(s8)==1?1:-1];\n'+'typedef char c16[sizeof(s16)==2?1:-1];\n'+'typedef char c32[sizeof(s32)==4?1:-1];\n'+'typedef char ptr32[sizeof(void*)==4?1:-1];\n');shell('gcc','-m32','-std=c89','-fsyntax-only',layout)
        obj=build/'candidate.o';score.compile_single(source,FLAGS,obj)
        with contextlib.redirect_stdout(io.StringIO()):comp=score.compare(obj,NAME,show=0)
        assert (comp.differing,comp.total,comp.extra_words)==(18,132,0)
        assert not any((comp.unresolved,comp.unverified,comp.errors)) and not comp.accepted()
        raw,rel=inspect(obj);resolved,masks,unresolved,unverified,errors=score.relocate(obj,score.text_words(obj),0,len(raw),addresses)
        assert not any((masks,unresolved,unverified,errors)) and len(resolved)==132
        bad=[4*i for i,(a,b) in enumerate(zip(targets[NAME],resolved)) if a!=b];assert len(bad)==18
        # Only source/destination register fields differ, with identical operation/immediate fields.
        for a,b in zip(targets[NAME],resolved):
            if a!=b:
                assert a>>26==b>>26
                assert (a & (0xFC0007FF if a>>26==0 else 0xFC00FFFF))==(b & (0xFC0007FF if b>>26==0 else 0xFC00FFFF))
        o3=build/'candidate_o3.o';score.compile_single(source,FLAGS.replace('-O2','-O3'),o3)
        r3,m3,u3,v3,e3=score.relocate(o3,score.text_words(o3),0,len(raw),addresses)
        assert not any((m3,u3,v3,e3)) and r3==resolved;inspect(o3)
        ld=build/'whole.ld';ld.write_text('SECTIONS { .text 0x8039D494 : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.options) *(.reginfo) *(.mdebug) *(.pdr) *(.gnu.attributes) } }\n'+''.join('%s = 0x%X;\n'%(n,v) for n,v in ANCHORS.items()))
        linked=build/'candidate.elf';shell('mips-linux-gnu-ld','-EB','-T',ld,'-o',linked,obj)
        linkedraw,_=inspect(linked,True);assert linkedraw==packed(resolved)
        gnu=list(struct.unpack('>132I',linkedraw));case_list=list(native.cases());expected=[native.oracle(c) for c in case_list]
        assert host(build,source,'candidate',case_list)==expected
        coverage=set();edges=set();candidate_clobbers=set();native_preserved=True;reads_equal=True
        for i,c in enumerate(case_list):
            memory=native.state(c);r=native.run({native.ENTRY:targets[NAME]},memory,c[0],c[1],salt=i)
            g=native.run({native.ENTRY:gnu},memory,c[0],c[1],salt=i)
            assert r['return']==g['return']==expected[i] and r['memory']==g['memory']==memory and not r['writes'] and not g['writes']
            assert r['reads']==g['reads'];assert all(r['registers'][k]==r['initial_registers'][k] for k in range(10,14))
            candidate_clobbers.update(k for k in range(10,14) if g['registers'][k]!=g['initial_registers'][k])
            coverage.update(r['coverage']);edges.update(r['branches'])
        assert candidate_clobbers==set(range(10,14))
        signed_cases=0
        for selector in range(-128,0):
            for item in (7,8,9):
                c=(item,selector%4,1,1,7,1,2,1,selector,(selector*31)&255);m=native.state(c)
                for body in (targets[NAME],gnu):assert native.run({native.ENTRY:body},m,item,c[1])['return']==native.oracle(c)
                signed_cases+=1
        caller_cases=0;caller_rejected=0;first_failure=None;caller_coverage=set()
        for mask in range(32):
            for player in range(4):
                c=(0,player,mask&1,(mask>>1)&1,3 if mask&4 else -1,(mask>>3)&1,2 if mask&16 else 6,mask&4,player,mask&7)
                m=native.state(c);rr=native.run({native.ENTRY:targets[NAME],native.CALLER:targets['func_8039E3BC']},m,player=player,caller=True)
                wanted=[item for item in range(19) if native.oracle((item,)+c[1:])]
                assert native.read(rr['memory'],native.COUNTS+player*4,4)==len(wanted)
                assert [native.read(rr['memory'],native.OUTPUT+player*76+4*j,4) for j in range(len(wanted))]==wanted
                allowed=set(range(native.COUNTS+player*4,native.COUNTS+player*4+4))|set(range(native.OUTPUT+player*76,native.OUTPUT+player*76+len(wanted)*4))|set(range(native.SP-24,native.SP))
                assert all(rr['memory'][a]==b for a,b in m.items() if a not in allowed)
                caller_coverage.update(rr['coverage']);caller_cases+=1
                try:
                    wrong=native.run({native.ENTRY:gnu,native.CALLER:targets['func_8039E3BC']},m,player=player,caller=True)
                    assert wrong['memory']==rr['memory'],'changed caller output'
                except AssertionError as exc:
                    caller_rejected+=1
                    if first_failure is None:first_failure={'case':list(c),'failure':str(exc)}
        assert caller_rejected==caller_cases
        mutants={}
        for label,old,new in (('wrong_player','D_803B9FD0[player]','D_803B9FD0[player ^ 1]'),('wrong_attribute_mask','& 4','& 2'),('lost_negative_track_guard','D_8014978C >= 0 && ',''),('wrong_mode','D_8014A110 != 2','D_8014A110 != 6')):
            text=source.read_text();assert text.count(old)==(3 if label=='wrong_player' else 1)
            path=build/(label+'.c');path.write_text(text.replace(old,new));values=host(build,path,label,case_list)
            failures=[i for i,(a,b) in enumerate(zip(values,expected)) if a!=b];assert failures
            mutants[label]={'failed_cases':len(failures),'first_case':list(case_list[failures[0]]),'source_sha256':sha(path.read_bytes())}
        decoder_checks=[]
        for label,code in (('unknown_opcode',[0xFC000000]),('escaped_code',[0])):
            try:native.run({native.ENTRY:code},native.state(case_list[0]))
            except AssertionError:decoder_checks.append(label)
        assert len(decoder_checks)==2
        receipt={'status':'NONMATCH','reason':'18 register-only words; native caller retains t2-t5, isolated C clobbers them','image':'A','base_commit':'cd22879d40b3de443cfde047b86e75e159b6cec6','address':hex(native.ENTRY),'end':hex(native.ENTRY+528),'bytes':528,'words':132,'flags':FLAGS,'source_sha256':sha(source.read_bytes()),'native_bindings':bindings,'comparison':comp.__dict__,'full_difference_offsets':bad,'resolved_body_sha256':sha(packed(resolved)),'gnu_linked_body_sha256':sha(linkedraw),'elf_function_bytes':528,'alignment_bytes':0,'relocations':len(rel),'owned_data_bytes':0,'address_bindings':{n:hex(a) for n,a in ANCHORS.items()},'o3_same_complete_executable':True,'behavior':{'four_route_cases':len(case_list),'routes':['native','project relocation identical to GNU','GNU','unchanged C89 UBSan/bounds'],'native_and_gnu_leaf_executions':2*(len(case_list)+signed_cases),'native_only_signed_selector_cases':signed_cases,'instruction_offsets_covered':sorted(a-native.ENTRY for a in coverage),'branch_outcomes_covered':len(edges),'read_traces_equal':True,'no_memory_writes':True,'native_preserves':['t2','t3','t4','t5'],'candidate_clobbers':['t2','t3','t4','t5'],'native_caller_cases':caller_cases,'candidate_substitution_rejected':caller_rejected,'first_substitution_failure':first_failure,'caller_instruction_offsets_covered':sorted(a-native.CALLER for a in caller_coverage if native.CALLER<=a<native.CALLER+136)},'mutants':mutants,'decoder_negative_checks':decoder_checks,'support_sha256':{p.name:sha(p.read_bytes()) for p in (HERE/'native.py',HERE/'elf_support.py',HERE/'host.c',HERE/'verify.py')},'limits':['No full-image, source-shadow, compression, ROM, hardware or accepted coverage proof','Native caller E3BC has private t4 input and s0 clobber; it is only executed from protected native code, not reconstructed or claimed','Host C domain: player 0..3 and selected attribute index 0..127; actual asset table contents and full reachability unknown','Negative signed selector tests use explicitly mapped native memory and are not portable-C validity claims','D6A4 caller is extent/hash bound and callsites inspected, not fully executed','No whole-function arcade donor or original source recovery claim']}
        receipt=json.loads(json.dumps(receipt))
        (build/'local_provenance.json').write_text(json.dumps({'object_sha256':sha(obj.read_bytes()),'gnu_linked_object_sha256':sha(linked.read_bytes()),'o3_object_sha256':sha(o3.read_bytes()),'ido':{n:sha((score.IDO/n).read_bytes()) for n in ('cc','cfe','uopt','ugen','as1')},'gcc':shell('gcc','--version').splitlines()[0],'ld':shell('mips-linux-gnu-ld','--version').splitlines()[0]},indent=2)+'\n')
        return receipt
    finally:
        score.ASM_DIR,score._targets,score._target_fingerprint,score._own_data=prior_state

def comparable(receipt):
    """Exclude enumerated historical integration provenance, never proof inputs."""
    result = json.loads(json.dumps(receipt))
    for key in ('protected_targets', 'scorer_sha256'):
        result.pop(key, None)
    return result


def main():
    ap=argparse.ArgumentParser();ap.add_argument('--repo',type=Path,default=ROOT);ap.add_argument('--tools-repo',type=Path,default=ROOT);ap.add_argument('--out',type=Path,default=HERE/'verification.json');ap.add_argument('--check',action='store_true');a=ap.parse_args();receipt=verify(a.repo.resolve(),a.tools_repo.resolve())
    if a.check:assert comparable(json.loads(a.out.read_text()))==comparable(receipt),'portable proof receipt differs'
    else:a.out.write_text(json.dumps(receipt,indent=2)+'\n')
    print(json.dumps({k:receipt[k] for k in ('status','bytes','comparison','behavior')},indent=2))
if __name__=='__main__':
    assert __debug__,'Python optimization unsupported'
    main()
