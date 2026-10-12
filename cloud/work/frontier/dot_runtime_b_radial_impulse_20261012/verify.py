#!/usr/bin/env python3
"""Current-input-bound semantic prerequisite; never an installable match claim."""
import argparse
import ctypes
from contextlib import contextmanager
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
import zlib

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score,owndata
from tools.conveyor import compilers
NAME,ADDRESS,SIZE='func_8038D498',0x8038d498,768
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
TEST=ROOT/'tests/cloud/test_runtime_b_radial_impulse.py'

def module(name,path):
    spec=importlib.util.spec_from_file_location(name,path);m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);return m
native=module('radial_native',HERE/'native.py')
contract=module('radial_count_contract',HERE/'count_contract.py')

def require(value,message):
    if not value:raise ValueError(message)

def sha(data):return hashlib.sha256(data).hexdigest()
def packed(words):return struct.pack('>%dI'%len(words),*words)

def image_bytes():
    data=(ROOT/'assets/us/data.bin').read_bytes()
    require(sha(data)=='f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8','asset identity')
    require(struct.unpack_from('>I',data,0x3854)[0]==0xb6fec4,'loader pointer')
    image=zlib.decompress(data[0xb6fec4-0x283d0:],-15)
    require(len(image)==43888 and sha(image)=='b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd','image identity')
    return image

@contextmanager
def targets(image,population='ovl_b'):
    old=score.ASM_DIR;score.ASM_DIR=ROOT/'asm/us'/population
    key=owndata.artifact_dir(score.ASM_DIR);present=key in score._own_data;prior=score._own_data.get(key)
    score._own_data[key]=None if image is None else owndata.ImageData.from_image(image,0x8038a400)
    try:yield
    finally:
        score.ASM_DIR=old
        if present:score._own_data[key]=prior
        else:score._own_data.pop(key,None)

def inputs():
    image=image_bytes();r=dict(protocol_version=2,population='ovl_b',target=NAME,claims=[],installable=False,caller_inventory={},
        files={n:sha((HERE/n).read_bytes()) for n in ('candidate.c','group.json','verify.py','native.py','count_contract.py','host.c','trace.py')},
        test_sha256=sha(TEST.read_bytes()),flags=FLAGS,targets={},symbols={},caller_sites=[])
    with targets(image):
        manifest=score.target_manifest();rows=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest))['functions']
        bodies=score.targets();symbols=score.image_symbols()
        for n in (NAME,'func_8038E114','func_8038FCE0','func_8038D3A4'):
            row=next(x for x in rows if x['name']==n);a=int(row['address'],16);raw=packed(bodies[n])
            require(len(raw)==row['size'] and raw==image[a-0x8038a400:a-0x8038a400+len(raw)],'complete target identity')
            require(min(int(x['address'],16) for x in rows if int(x['address'],16)>a)==a+len(raw),'native next boundary')
            r['targets'][n]=dict(address=hex(a),size=len(raw),sha256=sha(raw))
        require(r['targets'][NAME]['size']==SIZE,'target extent')
        literals=image[0x80394dc8-0x8038a400:0x80394dd4-0x8038a400]
        require(literals==struct.pack('>3f',.6,66000.,330000.),'literal identity')
        r['native_literals']=dict(address='0x80394dc8',size=12,sha256=sha(literals))
        for n in ('D_80152818','D_8014A250','D_8012E700','D_801543CA','func_8038D3A4','func_800A61B0'):
            a=symbols.get(n,score.address_named(n));require(a is not None,'symbol binding');r['symbols'][n]=hex(a)
    for population in ('blob','ovl_a','ovl_b'):
        with targets(image if population=='ovl_b' else None,population):
            inventory=score.targets()
            r['caller_inventory'][population]=sha(json.dumps({n:sha(packed(w)) for n,w in sorted(inventory.items())},sort_keys=True).encode())
            for n,words in inventory.items():
                for i,w in enumerate(words):
                    if w>>26==3 and (w&0x3ffffff)*4==ADDRESS&0x0fffffff:r['caller_sites'].append([population,n,i*4])
    require(r['caller_sites']==[['ovl_b','func_8038E114',0x111c]],'full direct caller census')
    with targets(image):
        r['shared_count_contract']=contract.evidence(ROOT,score)
        caller=score.targets()['func_8038E114'];move=caller[0x1110//4]
        require(move>>26==0 and move&63==37 and move>>21&31==21 and move>>16&31==0 and move>>11&31==18,'genuine s5 to s2 caller argument')
        r['private_abi']=dict(input_register='s2',caller_source_register='s5',argument_move_offset=0x1110,caller_spills=['s5','s6','f22'],ordinary_source_is_adapter=True)
    return r

def elf(path):
    """Independent ELF32 big-endian reader; no project relocation application."""
    data = path.read_bytes()
    require(data[:6] == b'\x7fELF\x01\x02' and struct.unpack_from('>H', data, 18)[0] == 8, 'ELF ABI')
    require(struct.unpack_from('>H',data,16)[0] in (1,2) and struct.unpack_from('>I',data,36)[0]&0xf0000000==0x10000000,'ELF MIPS2 object type')
    base = struct.unpack_from('>I', data, 32)[0]
    stride, count, ni = struct.unpack_from('>HHH', data, 46)
    require(stride == 40, 'ELF section stride')
    rows = [struct.unpack_from('>10I', data, base + i * stride) for i in range(count)]
    names = data[rows[ni][4]:rows[ni][4] + rows[ni][5]]
    sections, symbols, tabs, relocs = {}, {}, {}, []
    for i, row in enumerate(rows):
        name = names[row[0]:].split(b'\0')[0].decode()
        raw = data[row[4]:row[4] + row[5]]
        sections[name] = (i, row, raw)
        if row[1] == 2:
            ss = rows[row[6]]
            strings = data[ss[4]:ss[4] + ss[5]]
            tab = []
            for j in range(0, len(raw), 16):
                no, value, size, info, other, section = struct.unpack_from('>IIIBBH', raw, j)
                name = strings[no:].split(b'\0')[0].decode()
                tab.append(name)
                if name:
                    symbols[name] = (value, size, info & 15, section)
            tabs[i] = tab
    for row in rows:
        if row[1] in (4, 9):
            require(row[1] == 9, 'unexpected RELA')
            for at in range(row[4], row[4] + row[5], 8):
                site, info = struct.unpack_from('>II', data, at)
                relocs.append((site, info & 255, tabs[row[6]][info >> 8], row[7]))
    return sections, symbols, relocs


def check_owned(data):
    require(len(data)==16 and not any(data[12:]) and sorted(struct.unpack('>3I',data[:12]))==sorted(struct.unpack('>3I',struct.pack('>3f',.6,66000,330000))),'complete owned data')


def build(work,source=None):
    require(compilers.identify_dir(score.IDO)=='ido5.3','recognized stock IDO 5.3 required')
    obj=work/'candidate.o';score.compile_single(source or HERE/'candidate.c',FLAGS,obj)
    sections,symbols,relocs=elf(obj);ti,row,text=sections['.text'];ri,_,rodata=sections['.rodata']
    funcs={n:v for n,v in symbols.items() if v[2]==2 and v[3] not in (0,0xfff1)}
    require(set(funcs)=={NAME},'complete function census')
    start,size,kind,index=funcs[NAME]
    require(start==0 and index==ti and size==888 and len(text)==896 and size%4==0 and size<=len(text) and len(text)-size<16 and not any(text[size:]),'complete function/suffix')
    require({n for n,v in sections.items() if v[1][2]&2 and v[1][5]}=={'.text','.rodata','.reginfo'},'allocated section census')
    check_owned(rodata)
    require(len(relocs)==18 and all(dest==ti and typ in (4,5,6) and site%4==0 and site<size for site,typ,sym,dest in relocs),'complete text relocation census')
    externs={n:v for n,v in symbols.items() if v[3]==0 and n!='sqrtf'}
    allowed={'D_80152818','D_8014A250','D_8012E700','D_801543CA','func_8038D3A4','func_800A61B0'}
    require(set(externs)==allowed and all(sym in allowed|{'.rodata'} for _,_,sym,_ in relocs),'symbol contract')
    addresses=score.image_symbols();bindings={n:addresses.get(n,score.address_named(n)) for n in externs}
    script=work/'candidate.ld';linked=work/'candidate.elf'
    script.write_text('SECTIONS { .text 0x8038D498 : SUBALIGN(4) { *(.text) } .rodata 0x80400000 : { *(.rodata) } /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) } }\n'+'\n'.join('%s = 0x%x;'%(n,a) for n,a in bindings.items()))
    subprocess.run(['mips-linux-gnu-ld','-EB','--hash-style=sysv','-T',str(script),'-o',str(linked),str(obj)],check=True,capture_output=True)
    ss,sy,rr=elf(linked);require(not rr and all(v[3] for v in sy.values()),'unresolved GNU output')
    require({n for n,v in ss.items() if v[1][2]&2 and v[1][5]}=={'.text','.rodata'},'GNU allocated sections')
    require(sy[NAME]==(ADDRESS,size,2,ss['.text'][0]),'GNU full function extent')
    require(all(sy[n][0]==a for n,a in bindings.items()),'GNU external bindings')
    require(ss['.rodata'][1][3]==0x80400000 and ss['.rodata'][2]==rodata,'GNU literal preservation')
    linked_text=ss['.text'][2];require(len(linked_text)==len(text) and linked_text[size:]==text[size:],'GNU complete suffix')
    code={ADDRESS+i:w for i,w in enumerate(struct.unpack('>%dI'%(size//4),linked_text[:size])) for i in [i*4]}
    result=score.compare(obj,NAME,show=0)
    native_words=score.targets()[NAME];native_code={ADDRESS+4*i:w for i,w in enumerate(native_words)}
    differences=[i for i in range(max(SIZE,size)//4) if i>=len(native_words) or i>=size//4 or native_words[i]!=code[ADDRESS+4*i]]
    metadata=dict(ordinary_abi=True,native_private_abi=False,complete_function_bytes=size,text_bytes=len(text),zero_text_suffix=len(text)-size,
        owned_data_bytes=12,zero_data_suffix=4,original_relocations=len(relocs),gnu_full_word_differences=len(differences),
        extra_nonzero_words=sum(bool(w) for w in struct.unpack('>%dI'%(size//4),linked_text[:size])[SIZE//4:]),
        canonical=dict(summary=result.summary(),unresolved=result.unresolved,unverified=result.unverified,errors=result.errors,extra_words=result.extra_words),
        compiler={name:sha(Path(score.ido(name)).read_bytes()) for name in ('cc','cfe','uopt','ugen','as1','uld','usplit','umerge')})
    return native_code,code,[(0x80400000,rodata)],metadata

def host_expected(case):
    state,events=native.oracle(case);m=native.Memory(dict(state));out=[m.get(native.EFFECT+4,1),m.get(native.EFFECT+5,1),m.get(native.COUNT,2)]
    for i in range(4):
        p,v=native.PLAYERS+952*i,native.VEHICLES+2056*i
        out += [m.get(p+859,1),m.get(p+44)]
        out += [m.get(v+base+j*4) for base in (292,316) for j in range(3)]
    calls=[]
    for event in events:
        if event[0]=='damage':calls += [1,(event[1]-native.PLAYERS)//952,(event[2]-native.PLAYERS)//952,event[3]]
        else:calls += [2,(event[2]-44-native.PLAYERS)//952,*event[1]]
    return out+[len(calls)]+calls


def host_proof(work,deep=False,sanitize=False):
    compiler=shutil.which('cc');require(compiler is not None,'C compiler missing')
    cases=native.cases(deep)
    flags=['-O1','-std=c99','-ffp-contract=off','-Wno-unknown-pragmas']
    if sanitize:
        output=work/'host-sanitized';cmd=[compiler,*flags,'-DHOST_MAIN','-fsanitize=address,undefined,bounds','-fno-sanitize-recover=all',str(HERE/'host.c'),'-lm','-o',str(output)]
        subprocess.run(cmd,check=True,capture_output=True)
        proc=subprocess.run([str(output)],text=True,capture_output=True,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0'))
        require(proc.returncode==0,'sanitized actual source failed: '+proc.stderr)
        rows=[[int(x,16) for x in line.split()] for line in proc.stdout.splitlines()]
        require(rows==[host_expected(c) for c in native.cases(True)],'sanitized source vs field oracle')
        return dict(cases=len(rows),asan_ubsan_bounds=True,leak_sanitizer_disabled=True)
    output=work/'host.so';subprocess.run([compiler,*flags,'-shared','-fPIC',str(HERE/'host.c'),'-lm','-o',str(output)],check=True,capture_output=True)
    library=ctypes.CDLL(str(output));fn=library.host_run
    fn.argtypes=[ctypes.c_int,ctypes.c_float,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.POINTER(ctypes.c_uint32)];fn.restype=ctypes.c_int
    for case in cases:
        out=(ctypes.c_uint32*160)();size=fn(*case,out);require(0<=size<=160,'host output bound')
        require(list(out)[:size]==host_expected(case),'actual source vs field oracle '+str(case))
    return dict(cases=len(cases),actual_source_field_oracle=True)


def layouts(work):
    code='#define offsetof(t,m) ((unsigned long)&(((t *)0)->m))\n#include "candidate.c"\n'
    checks={'pointer':('sizeof(void*)',4),'player':('sizeof(Player)',952),'vehicle':('sizeof(Vehicle)',2056),
        'object':('sizeof(BObject)',60),'effect':('sizeof(Effect)',104),'scene':('sizeof(Scene)',68),
        'owner':('offsetof(Effect,owner)',4),'mask':('offsetof(Effect,hit_mask)',5),'primary':('offsetof(Effect,primary)',96),
        'position':('offsetof(Player,position)',8),'uv':('offsetof(Player,uv)',44),'active':('offsetof(Player,active)',776),
        'index':('offsetof(Player,index)',859),'velocity':('offsetof(Vehicle,velocity)',292),'planar':('offsetof(Vehicle,planar_velocity)',316),
        'attached':('offsetof(Vehicle,attached)',1732),'scene_radius':('offsetof(Scene,radius)',12)}
    code+=''.join('typedef char check_%s[(%s)==%d?1:-1];\n'%(n,e,v) for n,(e,v) in checks.items())
    path=work/'layout.c';path.write_text(code);subprocess.run([score.ido('cc'),'-c','-g0','-O2','-mips2','-G','0','-non_shared','-I'+str(HERE),str(path),'-o',str(work/'layout.o')],check=True,capture_output=True)
    return json.loads(json.dumps(checks))


def negatives(work,native_code):
    errors=[]
    for label,data in [('missing_data',b''),('corrupt_data',struct.pack('>3f',.7,66000,330000)+b'\0'*4)]:
        try:check_owned(data)
        except ValueError:errors.append(label)
        else:raise ValueError('bad owned data admitted')
    wrong=work/'wrong.c';wrong.write_text((HERE/'candidate.c').read_text().replace('magnitude = 800;','magnitude = 801;'))
    _,code,data,_=build(work,wrong)
    try:native.verify(native_code,code,data,False)
    except ValueError as exc:
        require('semantic mismatch' in str(exc),'wrong source failed outside semantic gate');errors.append('compiled_wrong_magnitude')
    else:raise ValueError('wrong semantic source admitted')
    machine=native.Machine({ADDRESS:0xffffffff},ADDRESS,[],native.cases(False)[0],True)
    try:machine.run()
    except ValueError as exc:
        require('unsupported opcode' in str(exc),'unsupported operation failed elsewhere');errors.append('unsupported_instruction')
    else:raise ValueError('unsupported instruction admitted')
    return errors


def installer_gate():
    from tools.conveyor.pipeline import ovl_splice
    spec=json.loads((HERE/'group.json').read_text())
    require(spec['members']==spec['claims']==[] and spec['context']==spec['keep']==[NAME],'diagnostic recipe widened')
    try:ovl_splice.packet_spec(HERE)
    except ovl_splice.SpliceError as exc:
        require('no claims (research only), nothing to splice' in str(exc),'unexpected refusal')
    else:raise ValueError('private-ABI semantic research became installable')
    return dict(empty_claims_refused=True,installable_code_bytes=0,production_state_written=False)


def verify(deep=False,sanitize=False):
    binding=inputs();score.ido('cc');image=image_bytes()
    with targets(image),tempfile.TemporaryDirectory(prefix='b-radial-') as tmp:
        native_code,code,data,proof=build(Path(tmp))
        behavior=native.verify(native_code,code,data,deep)
        host=host_proof(Path(tmp),deep,sanitize)
        layout=layouts(Path(tmp))
        controls=negatives(Path(tmp),native_code)
        installation=installer_gate()
    return dict(inputs=binding,proof=proof,behavior=behavior,host=host,layout=layout,negative_controls=controls,installer=installation)

def main():
    p=argparse.ArgumentParser();p.add_argument('--deep',action='store_true');p.add_argument('--record',action='store_true');p.add_argument('--sanitize',action='store_true');args=p.parse_args()
    result=verify(args.deep,args.sanitize)
    if args.record:(HERE/'verification.json').write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    else:
        expected=json.loads((HERE/'verification.json').read_text());require(result['inputs']==expected['inputs'],'input receipt mismatch');require(result['proof']==expected['proof'],'output proof mismatch');require(result['layout']==expected['layout'],'layout drift');require(result['negative_controls']==expected['negative_controls'],'negative gate drift');require(result['installer']==expected['installer'],'installer gate drift')
    print(json.dumps(result,indent=2,sort_keys=True))
if __name__=='__main__':main()
