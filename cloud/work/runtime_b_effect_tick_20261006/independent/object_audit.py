#!/usr/bin/env python3
"""Replay the single existing O3 artifact; never invoke a target or host compiler."""
import argparse,hashlib,json,random,sys
from pathlib import Path
from audit import independent_cases

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser();p.add_argument('--packet',type=Path,required=True);p.add_argument('--reference-root',type=Path,required=True);p.add_argument('--score-root',type=Path,required=True);p.add_argument('--work-dir',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    sys.path.insert(0,str(a.packet.resolve()));import baseline as b,elf,native as n
    score=b.load_score(a.score_root.resolve());obj=a.work_dir/'effect.o';linked=a.work_dir/'effect.elf'
    before=[sha(obj),sha(linked)];layout=json.loads((a.work_dir/'layout.json').read_text());assert layout['source_sha256']==sha(a.packet/'effect_tick.c')
    nc,nd=b.native(a.reference_root);cc,cd,symbols=b.elf_image(score,linked);start=symbols['func_80390F60']['value']
    procs,accounting=elf.procedures(score,obj);relocs=elf.validate_link(score,obj,linked)
    cases=independent_cases(n);runs=0;covered=set()
    for ix,(label,case,expect) in enumerate(cases):
        machines=[n.Machine(nc,n.ENTRY,nd,case),n.Machine(cc,start,cd,case)]
        for machine in machines:
            rng=random.Random(ix+0xF60)
            for reg in list(range(1,29))+[30]:machine.r[reg]=rng.getrandbits(32)
            machine.f=[n.bits(rng.uniform(-100,100)) for _ in range(32)]
            machine.original_r=machine.r[:];machine.original_f=machine.f[:]
            machine.condition=bool(ix&1);machine.lo=ix;machine.hi=~ix
        for frame in range(case.get('repeat',1)):
            for machine in machines:machine.r[31]=n.RETURN
            x,y=[machine.run() for machine in machines]
            assert x==y,('existing object mismatch',ix,label,frame,case)
            if expect is not None:assert y[0][3][0]==expect
            candidate=machines[1]
            assert candidate.r[16:24]==candidate.original_r[16:24] and candidate.r[30]==candidate.original_r[30]
            assert candidate.f[20:]==candidate.original_f[20:]
            covered|=candidate.coverage;runs+=1
    assert before==[sha(obj),sha(linked)]
    result={'status':'PASS: existing object bounded semantics; NOT a strict match','source_sha256':sha(a.packet/'effect_tick.c'),'object_sha256':before[0],'linked_sha256':before[1],'paired_fixtures':len(cases),'paired_root_executions':runs,'independent_player_oracles':sum(x[2] is not None for x in cases),'all_nonvolatile_state_preserved':True,'compiled_data_only_for_candidate':True,'compiled_instructions_covered':len(covered),'procedures':procs,'text_accounting':accounting,'relocation_validation':relocs,'compiler_invocations':0,'limits':'Reuses the reviewed packet MIPS interpreter, ELF helpers, and boundary/address model. Exact body extents fail: compiler inlines complete source B10 into F60, emits an 8-byte deleted-static stub, and changes D38/F60 sizes. No original translation-unit or whole-image acceptance claim.'}
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k!='relocation_validation'},indent=2))
if __name__=='__main__':main()
