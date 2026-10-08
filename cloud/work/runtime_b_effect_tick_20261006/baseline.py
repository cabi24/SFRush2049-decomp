#!/usr/bin/env python3
"""One canonical O3 closure baseline after compiler-checked layout.

Historical production context is read at BASE. Receipt comparisons bind only
packet sources and semantic/object evidence, not mutable integration files.
"""
import argparse,contextlib,ctypes,hashlib,importlib.util,io,json,shutil,struct,subprocess,sys
from dataclasses import asdict
from pathlib import Path
from semantic import BASE,HERE,TARGETS,native
from native import Machine,ENTRY,RETURN,fixtures,verify_host
from elf import procedures,sections,validate_link
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'

def sha(data):return hashlib.sha256(data).hexdigest()
def load_score(root):
    sys.path.insert(0,str(root/'tools/cloud'))
    spec=importlib.util.spec_from_file_location('effect_tick_score',root/'tools/cloud/score.py')
    score=importlib.util.module_from_spec(spec);sys.modules[spec.name]=score;spec.loader.exec_module(score)
    score.ASM_DIR=root/'asm/us/ovl_b'
    return score

def layout(score,work):
    facts=[('sizeof(void*)',4),('sizeof(s8)',1),('sizeof(s16)',2),('sizeof(s32)',4),('sizeof(f32)',4),
           ('sizeof(EffectObject)',84),('sizeof(EffectEntry)',8),('sizeof(EffectControl)',20),('sizeof(EffectPlayer)',952)]
    fields={'EffectObject':dict(flags=4,scene=12,animation=80),
            'EffectEntry':dict(object=0,state=4,eligibility=6),
            'EffectControl':dict(count=0,selected_fast=1,selected_second=2,selected_first=3,timer_fast=4,timer_first=8,timer_second=12,entries=16),
            'EffectPlayer':dict(status=908,countdown=928,alpha=929,phase=930,timer=936)}
    facts += [('OFF('+typ+','+field+')',value) for typ,fs in fields.items() for field,value in fs.items()]
    source=(HERE/'effect_tick.c').read_text().split('extern EffectControl')[0]
    src=work/'layout.c';src.write_text(source+'\n#define OFF(t,m) ((u32)&((t *)0)->m)\nu32 layout[] = {'+','.join(e for e,v in facts)+'};\n')
    obj=work/'layout.o';score.compile_single(src,FLAGS,obj)
    raw,secs=score._elf(obj)
    syms=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(raw,secs,i)]
    assert not [s for s in syms if s['type']==2]
    sym=next(s for s in syms if s['name']=='layout');sec=secs[sym['section']]
    values=struct.unpack_from('>'+str(len(facts))+'I',raw,sec['off']+sym['value'])
    assert values==tuple(v for e,v in facts)
    return dict(status='PASS',facts=[dict(expression=e,value=v) for e,v in facts])

def elf_image(score,path):
    raw,secs=sections(score,path);code={};data=[]
    for sec in secs:
        if sec['name']=='.text':code.update({sec['address']+i:w[0] for i,w in zip(range(0,sec['size'],4),struct.iter_unpack('>I',raw[sec['off']:sec['off']+sec['size']]))})
        elif sec['name'] in ('.rodata','.data') and sec['size']:data.append((sec['address'],raw[sec['off']:sec['off']+sec['size']]))
        elif sec['flags']&2 and sec['size']:assert sec['name'] in ('.reginfo','.options')
    syms={s['name']:s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(raw,secs,i) if s['name']}
    return code,data,syms

def compare_behavior(ncode,ndata,ccode,cdata,start):
    cov,branches=set(),set();total=0;cases=list(fixtures())
    for num,case in enumerate(cases):
        nm=Machine(ncode,ENTRY,ndata,case);cm=Machine(ccode,start,cdata,case)
        for repeat in range(case.get('repeat',1)):
            nm.r[31]=cm.r[31]=RETURN
            a=nm.run();b=cm.run();total+=1
            assert a==b,('candidate semantic mismatch',num,repeat,case,a,b)
            assert cm.r[16:24]==cm.original_r[16:24] and cm.r[30]==cm.original_r[30] and cm.f[20:]==cm.original_f[20:]
            cov|=cm.coverage;branches|=cm.branches
    return dict(fixtures=len(cases),root_runs=total,candidate_instructions=len(cov),candidate_branches=len(branches),candidate_unexecuted=[hex(a-start) for a in sorted(set(ccode)-cov)])

def main():
    p=argparse.ArgumentParser();p.add_argument('--reference-root',type=Path,required=True);p.add_argument('--score-root',type=Path,required=True)
    p.add_argument('--work-dir',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--inspect-existing',action='store_true');args=p.parse_args()
    work=args.work_dir.resolve();work.mkdir(parents=True,exist_ok=True)
    score=load_score(args.score_root.resolve())
    if not (score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        print('SKIP: pinned IDO and MIPS GNU linker required');return
    ncode,ndata=native(args.reference_root)
    for fn,(a,n,h) in TARGETS.items():assert sha(struct.pack('>'+str(n//4)+'I',*score.targets()[fn]))==h
    obj,linked=work/'effect.o',work/'effect.elf'
    if not args.inspect_existing:
        assert not (work/'baseline_started').exists(),'one-shot baseline already consumed; inspect the existing object'
        result={'base':BASE,'source_sha256':sha((HERE/'effect_tick.c').read_bytes()),'recipe':json.loads((HERE/'group.json').read_text()),'layout':layout(score,work)}
        (work/'baseline_started').write_text('One canonical whole-closure O3 baseline. No candidate tuning or build-route changes.\n')
        score.compile_group(HERE,obj)
        (work/'layout.json').write_text(json.dumps(result,indent=2)+'\n')
    else:result=json.loads((work/'layout.json').read_text());assert result['source_sha256']==sha((HERE/'effect_tick.c').read_bytes())
    raw,secs=score._elf(obj)
    syms={s['name']:s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(raw,secs,i) if s['name']}
    defs={n:s for n,s in syms.items() if s['type']==2 and s['section'] not in (0,0xFFF1)}
    assert set(defs)=={'func_80390F60'}
    bindings={n:score.address_named(n) for n,s in syms.items() if s['section']==0}
    assert all(a is not None for a in bindings.values()) and not (set(bindings)&set(TARGETS))
    script=work/'effect.ld'
    script.write_text('\n'.join(f'{n} = 0x{a:08X};' for n,a in sorted(bindings.items()))+'\nSECTIONS { .text 0x81000000 : { *(.text) } .rodata 0x81010000 : { *(.rodata) *(.rdata) *(.lit4) *(.lit8) } .data 0x81020000 : { *(.data) *(.sdata) } /DISCARD/ : { *(.reginfo) *(.options) *(.mdebug) *(.comment) } }\n')
    subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(linked),str(obj)],check=True)
    procs,accounting=procedures(score,obj);assert set(procs)==set(TARGETS)
    ccode,cdata,lsyms=elf_image(score,linked)
    assert not [n for n,s in lsyms.items() if s['section']==0]
    for n,a in bindings.items():assert lsyms[n]['value']==a and lsyms[n]['section']==0xFFF1
    with contextlib.redirect_stdout(io.StringIO()):comparison=score.compare(obj,'func_80390F60',show=0)
    behavior=compare_behavior(ncode,ndata,ccode,cdata,lsyms['func_80390F60']['value'])
    for n,meta in procs.items():
        meta['native_size']=TARGETS[n][1];meta['native_sha256']=TARGETS[n][2]
        want=score.targets()[n];got=[ccode[0x81000000+meta['offset']+4*j] for j in range(meta['size']//4)]
        bad=[j for j,w in enumerate(want) if j>=len(got) or got[j]!=w]
        meta['unmasked_research_placement_residual']=dict(differing_words=len(bad),native_words=len(want),missing_words=max(0,len(want)-len(got)),extra_words=max(0,len(got)-len(want)),first_differing_offsets=[hex(4*j) for j in bad[:12]],limits='Whole linked member at research placement; internal calls and owned-data addresses are not normalized. This is not a native-placement acceptance score.')
    allocated=[]
    eraw,esections=sections(score,linked)
    for sec in esections:
        if sec['flags']&2 and sec['size']:
            payload=eraw[sec['off']:sec['off']+sec['size']]
            allocated.append(dict(name=sec['name'],size=sec['size'],address=hex(sec['address']),sha256=sha(payload)))
    data_section=next(sec for sec in esections if sec['name']=='.data')
    payload=eraw[data_section['off']:data_section['off']+data_section['size']]
    assert len(payload)==16 and payload[:8]==ndata[0][1] and not any(payload[8:])
    owned_data=dict(status='initializer content verified; native code-reference placement remains unproved',section='.data',section_bytes=16,payload_bytes=8,zero_alignment_bytes=8,native_address=hex(ndata[0][0]),payload_sha256=sha(payload[:8]))
    result.update(status='COMPLETE SEMANTIC C / ONE CANONICAL O3 BASELINE; NOT A STRICT MATCH',
      actual_backend='unchanged score.compile_group; mandatory as1 -r4300_mul',group_compiler_invocations=1,
      compiler_sha256={n:sha((score.IDO/n).read_bytes()) for n in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']},
      object_sha256=sha(obj.read_bytes()),linked_sha256=sha(linked.read_bytes()),procedures=procs,text_accounting=accounting,allocated_sections=allocated,owned_data=owned_data,
      canonical_root_comparison=asdict(comparison),canonical_root_summary=comparison.summary(),canonical_notes=list(comparison.notes),
      external_bindings={n:hex(a) for n,a in sorted(bindings.items())},relocation_validation=validate_link(score,obj,linked),behavior=behavior)
    args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:result[k] for k in ['status','procedures','canonical_root_summary','behavior']},indent=2))
if __name__=='__main__':main()
