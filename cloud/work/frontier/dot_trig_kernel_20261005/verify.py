"""Rebuild an isolated authentic trig unit, and prove its full native contract."""
import argparse
from dataclasses import asdict
import hashlib
import importlib.util
import json
import math
from pathlib import Path
import random
import re
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
spec = importlib.util.spec_from_file_location('trig_native', HERE/'native.py')
native = importlib.util.module_from_spec(spec)
spec.loader.exec_module(native)
bits, value, f = native.bits, native.value, native.f
GROUP = ROOT/'cloud/work/ipa-groups/dot_trig_kernel_20261005'
SOURCE = GROUP/'group.c'
NAMES = ['func_8009C3F8', 'camera_update_c', 'select_screen_update']
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
ARCHIVE = ROOT/'cloud/work/frontier/w6a/dev/part_asin.c'
A21 = ROOT/'cloud/work/ipa-groups/codex_trig_roots_a21'
LITERAL = 0x80123abc
TABLE = 0x8011f010
DONOR_URL = 'https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/LIB/asincos.c'


def sha(data): return hashlib.sha256(data).hexdigest()
def run(command): return subprocess.run(command, check=True, capture_output=True, text=True).stdout


def functions(obj):
    data, secs = score._elf(obj)
    ti = score._text_index(secs)
    return {s['name']: s for i,sec in enumerate(secs) if sec['type']==2
            for s in score._symbol_table(data,secs,i) if s['section']==ti and s['type']==2}


def inspect(obj, name):
    fn = functions(obj)[name]
    result = asdict(score.compare(obj,name,show=0))
    result['errors']=[e.split(' (')[0]+' (content mismatch; compared bytes omitted)' if 'differs from retail' in e else e for e in result['errors']]
    result.update(elf_bytes=fn['size'], native_bytes=len(score.targets()[name])*4)
    return result


def exact(result):
    return result['elf_bytes']==result['native_bytes'] and not any(result[k] for k in
        ['differing','extra_words','unresolved','unverified','errors'])


def gnu_proof(obj, directory, expanded=False):
    directory.mkdir()
    fn = functions(obj)
    assert set(NAMES) <= set(fn)
    assert expanded or set(fn) == set(NAMES)
    data, secs = score._elf(obj)
    ti = score._text_index(secs)
    ro, = [s for s in secs if s['name']=='.rodata']
    assert (ro['size']>=48 if expanded else ro['size']==48) and not any(s['size'] for s in secs if s['name'] in ('.data','.bss'))
    assert data[ro['off']:ro['off']+48] == score.own_data().read(LITERAL,48)
    text = secs[ti]
    end = max(s['value']+s['size'] for s in fn.values())
    assert expanded or (end==524 and text['size']==528)
    assert not any(data[text['off']+end:text['off']+text['size']])
    # GNU readelf independently supplies each ELF STT_FUNC size and offset.
    observed = {}
    for line in run(['mips-linux-gnu-readelf','-sW',str(obj)]).splitlines():
        match = re.match(r'\s*\d+:\s+([0-9a-f]+)\s+(\d+)\s+FUNC\s+\S+\s+\S+\s+\d+\s+(\S+)$',line)
        if match: observed[match[3]] = [int(match[1],16),int(match[2])]
    assert observed == {n:[s['value'],s['size']] for n,s in fn.items()}
    relocs=[]
    for sec in secs:
        if sec['type']!=9: continue
        assert sec['info']==ti, 'unexpected non-text relocation section'
        syms=score._symbol_table(data,secs,sec['link'])
        for at in range(sec['off'],sec['off']+sec['size'],8):
            offset, info=struct.unpack_from('>II',data,at)
            relocs.append(dict(offset=offset,type=info&255,symbol=syms[info>>8]['name']))
    all_relocation_symbols={r['symbol'] for r in relocs}
    relocs=[r for r in relocs if any(fn[n]['value']<=r['offset']<fn[n]['value']+fn[n]['size'] for n in NAMES)]
    assert len(relocs)==32
    assert sum(r['type']==4 and r['symbol']=='.text' for r in relocs)==2
    assert sum(r['symbol']=='.rodata' for r in relocs)==24
    assert sum(r['symbol']=='D_8011F010' for r in relocs)==6
    # One genuine object is linked without rewriting instructions or relocations.
    # The two wrapper symbols need not occupy their final noncontiguous addresses:
    # they contain no PC-relative operation except their within-body branches, and
    # both absolute JALs target the correctly placed kernel. Words are then checked
    # in their entirety and replayed at each protected native entry address.
    script=directory/'link.ld'
    undefined={s['name'] for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i) if s['section']==0 and s['name'] in all_relocation_symbols}
    symbols=score.image_symbols()
    definitions=[]
    for name in sorted(undefined):
        address=symbols.get(name,score.address_named(name))
        assert address is not None,name
        definitions.append('%s = 0x%08x;\n'%(name,address))
    script.write_text('SECTIONS { .text 0x%08x : SUBALIGN(4) { *(.text) }\n' % (0x8009c3f8-fn[NAMES[0]]['value']) +
                      '.rodata 0x80123ABC : SUBALIGN(4) { *(.rodata) } }\n'+''.join(definitions))
    elf=directory/'linked.elf'
    run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)])
    raw_path=directory/'text.bin';ro_path=directory/'rodata.bin'
    run(['mips-linux-gnu-objcopy','--dump-section','.text='+str(raw_path),
         '--dump-section','.rodata='+str(ro_path),str(elf)])
    raw=raw_path.read_bytes()
    assert ro_path.read_bytes()[:48]==score.own_data().read(LITERAL,48)
    linked={}
    for n in NAMES:
        s=fn[n];body=raw[s['value']:s['value']+s['size']]
        words=list(struct.unpack('>'+str(s['size']//4)+'I',body))
        assert words==score.targets()[n], n+' GNU full-body mismatch'
        linked[n]=words
    return linked, dict(all_complete_bodies_equal=True,owned_literal_bytes=48,
        owned_literal_address=hex(LITERAL),relocation_count=len(relocs),relocations=relocs,
        elf_functions={n:observed[n] for n in NAMES},text_alignment_outside_functions=text['size']-end,
        wrapper_final_placement_claimed=False,
        object_sha256=sha(obj.read_bytes()),linked_text_sha256=sha(raw))


def gnu_control(obj, directory):
    """Independent full-extent GNU rejection evidence, with no masked words."""
    directory.mkdir();fn=functions(obj);data,secs=score._elf(obj)
    symbols=score.image_symbols();relocations=[]
    for sec in secs:
        if sec['type']!=9:continue
        syms=score._symbol_table(data,secs,sec['link'])
        for at in range(sec['off'],sec['off']+sec['size'],8):
            offset,info=struct.unpack_from('>II',data,at)
            relocations.append((offset,info&255,syms[info>>8]))
    names={r[2]['name'] for r in relocations if r[2]['section']==0}
    definitions=[]
    for name in sorted(names):
        address=symbols.get(name,score.address_named(name));assert address is not None,name
        definitions.append('%s = 0x%08x;\n'%(name,address))
    script=directory/'link.ld'
    script.write_text('SECTIONS { .text 0x%08x : SUBALIGN(4) { *(.text) }\n' % (0x8009c3f8-fn[NAMES[0]]['value']) +
                      '.rodata 0x80123ABC : SUBALIGN(4) { *(.rodata) } }\n'+''.join(definitions))
    elf=directory/'control.elf';raw=directory/'text.bin'
    run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)])
    run(['mips-linux-gnu-objcopy','--dump-section','.text='+str(raw),str(elf)])
    linked=raw.read_bytes();result={}
    for n in NAMES:
        body=linked[fn[n]['value']:fn[n]['value']+fn[n]['size']]
        words=list(struct.unpack('>'+str(len(body)//4)+'I',body));want=score.targets()[n]
        different=sum(words[i]!=want[i] for i in range(min(len(words),len(want))))
        different+=abs(len(words)-len(want))
        result[n]=dict(full_extent_differing_positions=different,elf_bytes=len(body),native_bytes=4*len(want),
                       excess_inside_function_words=max(0,len(words)-len(want)),body_sha256=sha(body))
    return dict(functions=result,all_relocations_linked=len(relocations),
                owned_rodata_bytes=sum(sec['size'] for sec in secs if sec['name']=='.rodata'))


def oracle(x, flag):
    # Independent scalar Cody-Waite model; every arithmetic stage rounds to f32.
    y=f(abs(x));i=flag
    if y<f(2.3e-10): r=y
    elif y>=1.0: r=f(math.pi/2)
    else:
        if y>0.5:
            i=1-i;g=f(f(f(0.5-y)+0.5)/2);y=f(math.sqrt(g));y=f(-f(y+y))
        else: g=f(y*y)
        numerator=f(-0.69674575)
        for c in (10.152522,-39.688862,57.20823,-27.368494): numerator=f(f(numerator*g)+f(c))
        numerator=f(numerator*g)
        denominator=f(g+f(-23.823858))
        for c in (150.95271,-381.86304,417.14432,-164.21097): denominator=f(f(denominator*g)+f(c))
        r=f(f(f(numerator/denominator)*y)+y)
    offsets=[f(0),f(math.pi/4),f(math.pi/2),f(math.pi/4)]
    if flag:
        c=offsets[i+2 if x<0 else i]
        r=f(c+f(c+r)) if x<0 else f(c+f(c-r))
    else:
        c=offsets[i];r=f(c+f(c+r))
        if x<0:r=-r
    return bits(r)


def cases():
    inputs=set()
    for x in (0.0,2.3e-10,0.5,1.0,2.0):
        w=bits(x)
        for delta in range(-4,5):
            if 0<=w+delta<0x7f800000:
                inputs.update((w+delta,(w+delta)|0x80000000))
    inputs.update([0x00000001,0x007fffff,0x00800000,0x7f7fffff,
                   0x80000001,0x807fffff,0x80800000,0xff7fffff])
    for i in range(2049):inputs.add(bits(i/1024.0-1.0))
    rng=random.Random(0x9c3f8)
    for _ in range(2048):
        w=rng.getrandbits(32)
        if w&0x7f800000!=0x7f800000:inputs.add(w)
        inputs.add(bits(rng.uniform(-1,1)))
    return [(x,mode) for x in sorted(inputs) for mode in (0,1)]


def host_run(directory, source, input_cases, label):
    exe=directory/(label+'-host')
    run(['cc','-std=c89','-O0','-fno-fast-math','-ffp-contract=off','-fexcess-precision=standard',
         '-fsanitize=undefined','-fno-sanitize-recover=undefined','-Wno-unknown-pragmas',
         str(source),str(HERE/'host.c'),'-lm','-o',str(exe)])
    result=subprocess.run([str(exe)],input=''.join('%08x %d\n'%c for c in input_cases),
                          text=True,capture_output=True,check=True)
    assert not result.stderr, result.stderr
    out=[tuple(int(x,16) for x in line.split()) for line in result.stdout.splitlines()]
    assert len(out)==len(input_cases) and all(len(r)==2 for r in out)
    return out


def behavior(directory, linked):
    addresses=score.image_symbols()
    original=score.targets()
    code=lambda bodies:{addresses[n]+4*i:w for n in NAMES for i,w in enumerate(bodies[n])}
    streams=[code(original),code(linked)]
    data={LITERAL+i:struct.unpack('>I',score.own_data().read(LITERAL+i,4))[0] for i in range(0,48,4)}
    table=[0.0,math.pi/4,math.pi/2,math.pi/4]
    assert b''.join(struct.pack('>f',v) for v in table)==score.own_data().read(TABLE,16)
    data.update({TABLE+4*i:bits(v) for i,v in enumerate(table)})
    values=cases();host=host_run(directory,SOURCE,values,'candidate')
    visited=set();branches=set()
    for (x,mode),(h,k) in zip(values,host):
        expected=oracle(value(x),mode)
        assert h==k==expected, ('host/oracle',x,mode,h,k,expected)
        wrapper=NAMES[mode+1]
        for kernel,entry in [(True,addresses[NAMES[0]]),(False,addresses[wrapper])]:
            a=native.execute(streams[0],entry,data,x,mode,kernel)
            b=native.execute(streams[1],entry,data,x,mode,kernel)
            assert a==b and a[0]==expected, ('native/linked/oracle',x,mode)
            visited.update(a[1]);branches.update(a[2])
    mutants={
        'half_range_boundary':('y > 0.5f','y > 0.6f'),
        'polynomial_coefficient':('-0.69674575f','-0.59674575f'),
        'negative_asin_sign':('r = -r;','r = r;'),
        'mode_reduction_index':('i = 1 - flag;','i = flag;'),
        'acos_negative_table':('D_8011F010[i + 2]','D_8011F010[i]'),
    }
    rejected={};source=SOURCE.read_text()
    for label,(old,new) in mutants.items():
        assert old in source
        path=directory/(label+'.c');path.write_text(source.replace(old,new))
        out=host_run(directory,path,values,label)
        witness=next((i for i,(a,b) in enumerate(zip(host,out)) if a!=b),None)
        assert witness is not None,label+' survived'
        rejected[label]=dict(rejected=True,input=value(values[witness][0]),mode=values[witness][1])
    assert visited==static_reachable(streams[0],[addresses[n] for n in NAMES]), 'dynamic/CFG mismatch'
    coverage={n:dict(executed_offsets=sorted(pc-addresses[n] for pc in visited
                        if addresses[n]<=pc<addresses[n]+4*len(original[n])),
                     unexecuted_offsets=[4*i for i in range(len(original[n])) if addresses[n]+4*i not in visited]) for n in NAMES}
    for wrapper in NAMES[1:]: assert not coverage[wrapper]['unexecuted_offsets']
    return dict(cases=len(values),native_executions=4*len(values),unchanged_host_c89_ubsan=True,
                all_native_and_gnu_results_and_traces_equal=True,read_only_data_preserved=True,
                full_stack_canaries_and_saved_registers=True,dynamic_coverage_equals_static_cfg=True,private_kernel_inputs=['f16','a0'],
                ordinary_wrapper_input='f12',coverage=coverage,branch_outcomes=len(branches),
                rejected_source_mutants=rejected)


def static_reachable(code, entries):
    """Conservative CFG, accounting for unconditional and likely delay slots."""
    pending=list(entries);seen=set()
    while pending:
        pc=pending.pop()
        if pc in seen:continue
        assert pc in code
        seen.add(pc);w=code[pc];op=w>>26
        if op==0 and w&63==8:
            seen.add(pc+4)
        elif op==3:
            seen.add(pc+4)
            pending.extend([pc+8,((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)])
        elif op in (4,5,20,21) or (op==17 and (w>>21)&31==8):
            off=w&65535;off=off-65536 if off&32768 else off
            seen.add(pc+4)
            pending.append(pc+4+off*4)
            if not (op==4 and (w>>21)&31==(w>>16)&31):pending.append(pc+8)
        else:pending.append(pc+4)
    return seen


def caller_census():
    addresses=score.image_symbols();targets=score.targets();out={n:[] for n in NAMES}
    for caller,words in targets.items():
        for i,w in enumerate(words):
            if w>>26!=3:continue
            target=0x80000000|((w&0x3ffffff)<<2)
            for n in NAMES:
                if target==addresses[n]:out[n].append(dict(caller=caller,call_address=hex(addresses[caller]+4*i)))
    return out


def genuine_context(directory):
    """Reuse the complete archived real caller packet, with only its true ABI adapted."""
    original=ROOT/'cloud/work/frontier/dot_camera_path_contracts'
    source=(original/'group.c').read_text()
    assert source.count('f32 func_8009C3F8(s32,f32);')==1
    assert source.count('func_8009C3F8(0,delta[0])')==1
    source=source.replace('f32 func_8009C3F8(s32,f32);','f32 func_8009C3F8(f32,s32);')
    source=source.replace('func_8009C3F8(0,delta[0])','func_8009C3F8(delta[0],0)')
    assert 'standin' not in source and 'zz_caller' not in source
    group=directory/'real_context';group.mkdir()
    (group/'caller.c').write_text(source);(group/'kernel.c').write_bytes(SOURCE.read_bytes())
    config=json.loads((original/'group.json').read_text())
    context=config['members']+config['context']
    config.update(files=['caller.c','kernel.c'],members=NAMES,context=context,claims=[])
    config['keep']+=NAMES[1:]
    (group/'group.json').write_text(json.dumps(config))
    obj=group/'context.o';score.compile_group(group,obj)
    results={n:inspect(obj,n) for n in NAMES}
    assert all(exact(r) for r in results.values())
    _,gnu=gnu_proof(obj,group/'gnu',expanded=True)
    return dict(trig_bodies=results,gnu_link=gnu,
                unclaimed_context={n:inspect(obj,n) for n in context},
                source_bindings={str((original/n).relative_to(ROOT)):sha((original/n).read_bytes()) for n in ['group.c','group.json']},
                only_caller_changes=['Correct kernel declaration to float-first/int-second.',
                                     'Adapt its single real camera call to float-first/int-second.'])


def report(directory):
    obj=directory/'candidate.o';score.compile_group(GROUP,obj)
    results={n:inspect(obj,n) for n in NAMES}
    assert all(exact(r) for r in results.values())
    assert ARCHIVE.read_text() in SOURCE.read_text(), 'archive body changed'
    linked,gnu=gnu_proof(obj,directory/'gnu')
    controls={};gnu_controls={}
    old=directory/'old.o';score.compile_group(A21,old)
    controls['archived_a21']={n:inspect(old,n) for n in NAMES}
    gnu_controls['archived_a21']=gnu_control(old,directory/'old_gnu')
    source=SOURCE.read_text()
    variants={
        'flag_first':source.replace('func_8009C3F8(f32 x, s32 flag)',
             'func_8009C3F8(s32 flag, f32 x)').replace('func_8009C3F8(x, 0)','func_8009C3F8(0, x)').replace('func_8009C3F8(x, 1)','func_8009C3F8(1, x)'),
        'folded_half_literal':source.replace(' / 2.0f',' * 0.5f'),
    }
    for label,text in variants.items():
        group=directory/label;group.mkdir();(group/'group.c').write_text(text)
        (group/'group.json').write_bytes((GROUP/'group.json').read_bytes())
        out=group/'candidate.o';score.compile_group(group,out)
        controls[label]={n:inspect(out,n) for n in NAMES}
        gnu_controls[label]=gnu_control(out,group/'gnu')
        assert not all(exact(r) for r in controls[label].values()),label+' unexpectedly exact'
    bindings=[SOURCE,GROUP/'group.json',ARCHIVE,A21/'group.c',A21/'group.json',
              HERE/'verify.py',HERE/'native.py',HERE/'host.c']
    r=dict(base_revision='e24b47d89a0c8ffade1e4c75ad76b9d390a1c232',
           status='MATCHING_RESEARCH',candidate_bytes=524,newly_discovered_instruction_match_bytes=0,
           accepted_byte_gain=0,flags=FLAGS,donor_url=DONOR_URL,
           source_bindings={str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in bindings},
           compiler_sha256={n:sha(Path(score.ido(n)).read_bytes()) for n in
                            ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']},
           ranges={n:[hex(score.image_symbols()[n]),hex(score.image_symbols()[n]+r['native_bytes'])] for n,r in results.items()},
           object=results,gnu_link=gnu,controls=controls,controls_gnu=gnu_controls,caller_census=caller_census(),
           behavior=behavior(directory,linked),genuine_context=genuine_context(directory),
           limitations=['Only modes 0 and 1 and finite binary32 input are proven.',
             'FCSR exception flags, signaling NaNs and hardware timing are unmodeled.',
             'Real D348C/stunt/camera context is compiled but remains unclaimed and unexecuted; particle_system and BFD8C are audit-only.',
             'No full-game shadow unit, image, compression, ROM, acceptance or coverage claim.',
             'GNU checks the unchanged complete object; noncontiguous final wrapper placement is not a linker claim.'])
    return r


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--write',action='store_true')
    args=parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='trig-kernel-proof-') as tmp:result=report(Path(tmp))
    if args.write:(HERE/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result,indent=2))
