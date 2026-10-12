#!/usr/bin/env python3
"""Input-bound full-extent and ABI research proof; empty claims, no match waiver."""
import argparse
from contextlib import contextmanager,redirect_stdout
from dataclasses import asdict
import hashlib
import importlib.util
import io
import json
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
from tools.conveyor import compilers
NAME='func_8038C910'
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
SIZE=276
ADDRESS=0x8038c910

def require(value,message):
    if not value:raise ValueError(message)

def sha(raw):return hashlib.sha256(raw).hexdigest()
def packed(words):return struct.pack('>%dI'%len(words),*words)
def run(cmd,**kw):
    result=subprocess.run([str(x) for x in cmd],capture_output=True,text=True,**kw)
    require(result.returncode==0,(cmd,result.returncode,result.stdout,result.stderr));return result.stdout

@contextmanager
def population(name):
    old=score.ASM_DIR;score.ASM_DIR=ROOT/'asm/us'/name
    try:yield
    finally:score.ASM_DIR=old

def elf(path):
    """Independent ELF32 big-endian reader; no project relocation application."""
    data = path.read_bytes()
    require(data[:6] == b'\x7fELF\x01\x02' and struct.unpack_from('>H', data, 18)[0] == 8, 'ELF ABI')
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

def inputs():
    names=['candidate.c','baseline.c','verify.py','native.py','host.c','trace.py']
    result=dict(protocol_version=2,population='ovl_b',claims=[],new_matching_bytes=0,
                files={n:sha((HERE/n).read_bytes()) for n in names},
                test_sha256=sha((ROOT/'tests/cloud/test_runtime_b_callback_init_research.py').read_bytes()),targets={},callers={})
    with population('ovl_b'):
        target=score.targets();symbols=score.image_symbols();manifest=score.target_manifest()
        extents=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest))
        rows={r['name']:r for r in extents['functions']}
        require(extents['image']=='B' and extents['rom_offset']=='0xB6FEC4','population')
        require(rows[NAME]['size']==SIZE and int(rows[NAME]['address'],16)==ADDRESS,'native full extent')
        require(min(int(r['address'],16) for r in rows.values() if int(r['address'],16)>ADDRESS)==ADDRESS+SIZE,'next native boundary')
        for n in (NAME,'func_8038AA8C'):
            result['targets']['ovl_b:'+n]=dict(size=len(target[n])*4,words_sha256=sha(packed(target[n])))
        tokens=set(re.findall(r'\b[A-Za-z_]\w*\b',(HERE/'candidate.c').read_text()))
        result['symbols']={n:hex(symbols.get(n,score.address_named(n))) for n in sorted(tokens) if symbols.get(n,score.address_named(n)) is not None}
    for pop in ('blob','ovl_a','ovl_b'):
        with population(pop):
            ts=score.targets()
            for n,words in ts.items():
                offsets=[i*4 for i,w in enumerate(words) if w>>26==3 and 0x80000000|((w&0x3ffffff)<<2)==ADDRESS]
                if offsets:result['callers'][pop+':'+n]=dict(sites=offsets,size=len(words)*4,words_sha256=sha(packed(words)))
            if pop=='blob':
                for n in ('save_slot_valid','func_8008E26C','func_8008D6FC'):
                    result['targets']['blob:'+n]=dict(size=len(ts[n])*4,words_sha256=sha(packed(ts[n])))
    require(result['callers'],'no genuine caller evidence')
    path=ROOT/'src/blob/save_slot_valid.c'
    result['accepted_wrapper_source_sha256']=sha(path.read_bytes())
    return result

def compiler_identity():
    score.ido('cc')
    require(compilers.identify_dir(score.IDO)=='ido5.3','not recognized stock IDO5.3')
    return {n:sha(Path(score.ido(n)).read_bytes()) for n in ('cc','cfe','ugen','uopt','uld','ujoin','usplit','umerge','as1')}

def gnu(obj,work):
    sections,symbols,relocs=elf(obj)
    require(set(n for n,v in symbols.items() if v[2]==2 and v[3])=={NAME},'unexpected source function')
    start,size,kind,index=symbols[NAME];ti,row,raw=sections['.text']
    require(index==ti and start==0 and size%4==0,'full STT_FUNC starts at zero')
    require(size<=len(raw) and len(raw)-size<16 and not any(raw[size:]),'nonzero text suffix')
    require({n for n,(_,r,_) in sections.items() if r[2]&2 and r[5]}<={'.text','.reginfo'},'unexpected allocated source data')
    require(all(dest==ti and typ in (4,5,6) and site%4==0 and site<size for site,typ,_,dest in relocs),'unexpected original REL')
    addresses=score.image_symbols();bindings=[];expected_bindings={}
    for n,v in symbols.items():
        if v[3]==0:
            a=addresses.get(n,score.address_named(n));require(a is not None,'unresolved '+n);bindings.append('%s = 0x%x;'%(n,a));expected_bindings[n]=a
    script=work/(obj.stem+'.ld');linked=work/(obj.stem+'.elf')
    script.write_text('SECTIONS { .text 0x8038c910 : { *(.text) } /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) } }\n'+'\n'.join(bindings))
    run(['mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj])
    ss,sy,rr=elf(linked);i,r,body=ss['.text']
    require(not rr and all(v[3] for v in sy.values()),'unresolved GNU binding')
    require(all(sy[n][0]==a and sy[n][3]==0xfff1 for n,a in expected_bindings.items()),'GNU external binding drift')
    require({n for n,(_,r,_) in ss.items() if r[2]&2 and r[5]}=={'.text'},'unexpected GNU allocation')
    require(sy[NAME]==(ADDRESS,size,2,i) and r[3]==ADDRESS and len(body)==len(raw),'GNU extent/location')
    require(not any(body[size:]),'GNU nonzero suffix')
    native=packed(score.targets()[NAME]);bad=[i for i in range(0,SIZE,4) if native[i:i+4]!=body[i:i+4]]
    extra=sum(any(body[i:i+4]) for i in range(SIZE,size,4))
    return dict(function_bytes=size,complete_text_bytes=len(body),zero_suffix_bytes=len(body)-size,
                relocations=len(relocs),differing_offsets=bad,extra_words=extra,owned_data_bytes=0),list(struct.unpack('>%dI'%(size//4),body[:size]))

def abi_proof(work):
    source=(ROOT/'src/blob/save_slot_valid.c').read_text()
    require(source.count('void save_slot_valid(')==1 and source.count('    func_8008E26C(')==1,'accepted wrapper contract changed')
    corrected=source.replace('void save_slot_valid(','s32 save_slot_valid(').replace('    func_8008E26C(','    return func_8008E26C(')
    path=work/'return_wrapper.c';path.write_text(corrected);obj=work/'return_wrapper.o'
    score.compile_single(path,FLAGS,obj)
    with population('blob'):
        r=score.compare(obj,'save_slot_valid',show=0)
        require(r.accepted(),'return-corrected accepted wrapper no longer exact')
        w=score.targets()['save_slot_valid'];producer=score.targets()['func_8008E26C']
        calls=[i for i,v in enumerate(w) if v>>26==3 and 0x80000000|((v&0x3ffffff)<<2)==0x8008e26c]
        require(calls==[38],'wrapper final native producer call')
        # Remaining words are only delay-slot a3 setup, ra restore, stack restore,
        # jr ra and nop. None can redefine v0; decode rather than dump bytes.
        tail=w[39:];require(len(tail)==5,'wrapper tail length')
        require(tail[0]>>26==0 and tail[0]&63==0x25 and tail[0]>>11&31==7,'call delay argument setup')
        require((tail[1]>>26,tail[1]>>16&31)==(0x23,31) and (tail[2]>>26,tail[2]>>16&31)==(9,29),'wrapper restores')
        require(tail[3]>>26==0 and tail[3]&63==8 and tail[3]>>21&31==31 and tail[4]==0,'wrapper return')
        # E26C restores v0 from its sp+24 saved sign-extended index.
        require((producer[-4]>>26,producer[-4]>>21&31,producer[-4]>>16&31,producer[-4]&65535)==(0x23,29,2,24),'native producer result restore')
    return dict(return_corrected_wrapper='176-byte strict MATCH',native_return='signed16 scene index sign-extended in v0',integration_blocker='accepted void return and narrow C prototype require shared declaration reconciliation; ABI-facing caller is research only')

def native_layout(work):
    path=work/'layout.c';path.write_text('#include "'+str(HERE/'candidate.c')+'"\n#define OFF(t,f) __builtin_offsetof(t,f)\n')
    # IDO does not provide __builtin_offsetof, use compile-time address expression.
    text=path.read_text().replace('__builtin_offsetof(t,f)','((unsigned int)&(((t *)0)->f))')
    checks=[('address', 'sizeof(address_word)',4),('pointer','sizeof(void *)',4),('callback','sizeof(Callback)',24),('player','sizeof(Player)',952),('control','sizeof(Control)',64),('vehicle','sizeof(Vehicle)',2056),('vector','sizeof(Vec)',12),('callback_base','OFF(Player,callback)',752),('state','OFF(Callback,state)',4),('handle','OFF(Callback,handle)',6),('index','OFF(Callback,player)',8),('time','OFF(Callback,time)',16),('function','OFF(Callback,callback)',20),('parent','OFF(Control,parent)',0),('result','OFF(Control,handle)',24),('model','OFF(Vehicle,model)',8)]
    path.write_text(text+''.join('typedef char check_%s[(%s)==%d?1:-1];\n'%x for x in checks))
    score.compile_single(path,FLAGS,work/'layout.o');return len(checks)

def verify(deep=False,write=False,score_output=False):
    observed=inputs()
    receipt=HERE/'verification.json'
    if not write:require(json.loads(receipt.read_text())['inputs']==observed,'input receipt drift')
    identities=compiler_identity()
    if not write:require(json.loads(receipt.read_text())['compiler']==identities,'stock compiler identity drift')
    output=dict(inputs=observed,compiler=identities,results={})
    with tempfile.TemporaryDirectory(prefix='callback-init-proof-') as temp,population('ovl_b'):
        work=Path(temp);bodies={NAME:score.targets()[NAME]}
        for n,expected in [('candidate',13),('baseline',58)]:
            obj=work/(n+'.o');score.compile_single(HERE/(n+'.c'),FLAGS,obj)
            buffer=io.StringIO()
            with redirect_stdout(buffer):r=score.compare(obj,NAME,show=0)
            require(r.differing==expected and not r.extra_words and not r.unresolved and not r.unverified and not r.errors,'NONMATCH score changed')
            output['results'][n]=asdict(r)
            if score_output:print(n+': %d/%d words differ'%(r.differing,r.total))
            if deep:
                proof,words=gnu(obj,work);require(len(proof['differing_offsets'])==expected and not proof['extra_words'],'GNU comparison differs');output['results'][n]['gnu']=proof;bodies[n]=words
        if deep:
            spec=importlib.util.spec_from_file_location('callback_native',HERE/'native.py');native=importlib.util.module_from_spec(spec);spec.loader.exec_module(native)
            output['native_machine_fixtures']=native.prove(bodies)
            output['abi_contract']=abi_proof(work);output['native_layout_checks']=native_layout(work)
            text=(HERE/'candidate.c').read_text();output['causal_controls']={}
            for label,source in [('typed_callback',text.replace('(Callback *)(address_word)&D_80152818[player].callback','&D_80152818[player].callback')),('direct_time',text.replace('*(f32 *)(address_word)D_8002EB90','D_8002EB90[0]'))]:
                path=work/(label+'.c');path.write_text(source);obj=work/(label+'.o');score.compile_single(path,FLAGS,obj)
                result=score.compare(obj,NAME,show=0);proof,words=gnu(obj,work)
                require(result.differing>13 and len(proof['differing_offsets'])==result.differing,'causal rollback ceased to be causal')
                output['causal_controls'][label]=proof
            mutant=work/'bad_flag.c';mutant.write_text(text.replace('control->parent,0x2000,player','control->parent,0x1000,player'))
            obj=work/'bad_flag.o';score.compile_single(mutant,FLAGS,obj);proof,words=gnu(obj,work)
            try:native.prove({'bad_flag':words})
            except ValueError as exc:require('creation ABI' in str(exc),'unexpected mutation failure')
            else:raise ValueError('wrong helper flag accepted')
            output['negative_control']='compiled wrong creation flag rejected by native ABI fixtures'
            for label,extra in [('host',[]),('sanitized',['-fsanitize=address,undefined,bounds','-fno-sanitize-recover=all','-fno-omit-frame-pointer'])]:
                exe=work/label;run(['gcc','-std=c89','-O2','-Wall','-Wextra','-Werror',*extra,HERE/'host.c','-o',exe])
                import os
                env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0');output[label]=run([exe],env=env).strip()
    if not write:
        expected=json.loads(receipt.read_text())
        if deep:require(output==expected,'deep receipt changed')
        else:
            for n,result in output['results'].items():require(result=={k:v for k,v in expected['results'][n].items() if k!='gnu'},'cheap result receipt changed')
    if write:receipt.write_text(json.dumps(output,indent=2,sort_keys=True)+'\n')
    return output

def main():
    p=argparse.ArgumentParser();p.add_argument('--deep',action='store_true');p.add_argument('--write',action='store_true');p.add_argument('--score',action='store_true');a=p.parse_args()
    print(json.dumps(verify(a.deep,a.write,a.score),indent=2,sort_keys=True))
if __name__=='__main__':main()
