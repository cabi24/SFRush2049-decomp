#!/usr/bin/env python3
"""Input-bound complete stock IDO, independent GNU and selective-body proof."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import zlib

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
GROUP_NAME = 'dot_runtime_a_availability_refresh_20261007'
GROUP = HERE / 'groups' / GROUP_NAME
NEW = 'func_80394B00'
OLD = ['func_80394564', 'func_80394834']
MEMBERS = OLD + [NEW]
CONTEXT = ['func_80397FFC']
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
ASSET_SHA = 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
sys.path.insert(0, str(ROOT))
from tools.cloud import score
from tools.conveyor import compilers
from tools.conveyor.pipeline import blob_group, ovl_splice, ovl_layout
spec = importlib.util.spec_from_file_location('reset_elf', HERE / 'elf.py')
elf = importlib.util.module_from_spec(spec)
spec.loader.exec_module(elf)
require = elf.require


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def packed(words):
    return struct.pack('>%dI' % len(words), *words)


@contextmanager
def targets():
    previous = score.ASM_DIR
    score.ASM_DIR = ROOT / 'asm/us/ovl_a'
    try:
        yield
    finally:
        score.ASM_DIR = previous


def shell(*args):
    result = subprocess.run(list(map(str, args)), capture_output=True, text=True)
    require(result.returncode == 0, (args, result.stdout, result.stderr))
    return result.stdout


def authenticate():
    with targets():
        meta = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', score.target_manifest()))
    asset = (ROOT / 'assets/us/data.bin').read_bytes()
    require(len(asset) == 12418096 and digest(asset) == ASSET_SHA, 'asset identity')
    off = int(meta['rom_offset'], 16)
    require(int.from_bytes(asset[0x3850:0x3854], 'big') == off == 0xB5C534, 'loader pointer')
    decoder = zlib.decompressobj(-15)
    data = decoder.decompress(asset[off-0x283D0:]) + decoder.flush()
    require(decoder.eof and len(data) == meta['size'] and digest(data) == meta['image_sha256'], 'image identity')
    return data, meta


def census(words):
    call = 0x0C000000 | ((int(NEW[-8:], 16) >> 2) & 0x03FFFFFF)
    return {name: [i*4 for i,w in enumerate(body) if w == call]
            for name,body in words.items() if call in body}


def inputs():
    with targets():
        words = score.targets()
        image, meta = authenticate()
        rows = {row['name']: row for row in meta['functions']}
        group = json.loads((GROUP / 'group.json').read_text())
        require(group['members'] == group['claims'] == MEMBERS, 'all strict members claimed')
        require(group['context'] == CONTEXT and group['keep'] == CONTEXT, 'genuine root only')
        require(group['flags'] == FLAGS, 'stock group recipe')
        paths = [GROUP/'group.c', GROUP/'group.json', HERE/'baseline.c', HERE/'verify.py',
                 HERE/'elf.py', HERE/'behavior.c', HERE/'trace.py', HERE/'trace.json',
                 ROOT/'tests/cloud/test_runtime_a_availability_reset.py']
        calls = census(words)
        require(len(calls) == 5 and sum(map(len, calls.values())) == 24, 'native caller census')
        result = dict(sources_sha256={str(p.relative_to(ROOT)): digest(p.read_bytes()) for p in paths},
                      targets={}, claims=MEMBERS, context=CONTEXT, flags=FLAGS,
                      calls=calls, asset_sha256=ASSET_SHA, image_sha256=meta['image_sha256'])
        for name in sorted(set(MEMBERS + CONTEXT + list(calls))):
            row = rows[name]
            require(row['size'] == len(words[name])*4, 'target extent')
            start = int(row['address'],16)
            require(image[start-int(meta['base'],16):start-int(meta['base'],16)+row['size']] == packed(words[name]), 'target/image identity')
            end = min(int(r['address'],16) for r in rows.values() if int(r['address'],16)>start)
            require(end == start + row['size'], 'next native boundary')
            result['targets'][name] = dict(address=row['address'],size=row['size'],next_boundary=hex(end),sha256=digest(packed(words[name])))
        table = score.image_symbols()
        tokens = set(re.findall(r'\b[A-Za-z_]\w*\b', (GROUP/'group.c').read_text()))
        result['symbols'] = {n:hex(table.get(n,score.address_named(n))) for n in sorted(tokens)
                             if table.get(n,score.address_named(n)) is not None}
        # Bind actual helper instruction words, not assembly text/provenance lines.
        text = (ROOT/'asm/us/nonmatchings/rom/lib_3390/memset.s').read_text().split('endlabel memset',1)[0]
        helper = [int(w,16) for w in re.findall(r'/\*\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s*\*/', text)]
        require(len(helper) == 65 and table['memset'] == 0x80002790, 'memset native evidence')
        result['helper'] = dict(name='memset',address=hex(table['memset']),bytes=len(helper)*4,sha256=digest(packed(helper)))
        return result


def controls():
    final = (GROUP/'group.c').read_text()
    first = '''while (node) {
            if (node == D_8014A118[player].owner) break;
            node = node->body->next;
        }'''
    second = '''for (other = D_8012E6E0; other; other = other->body->next) {
                    if (other == node) break;'''
    index = '((s32)(D_8014A118[player].id * sizeof(s32))) / (s32)sizeof(s32)'
    equality = 'node->body->selection->item->player == other->body->selection->item->player'
    require(all(x in final for x in (first, second, index, equality)), 'source control anchors')
    return {
        'baseline':(HERE/'baseline.c').read_text(),
        'short_circuit_loops':final.replace(first,'while (node && node != D_8014A118[player].owner) node = node->body->next;').replace(second,'for (other = D_8012E6E0; other && other != node; other = other->body->next) {'),
        'direct_index':final.replace(index,'D_8014A118[player].id'),
        'old_equality_order':final.replace(equality,'other->body->selection->item->player == node->body->selection->item->player')}


def extent_evidence(obj, binding, exact=True):
    sections, symbols, rels = elf.read(obj)
    ti, row, raw = sections['.text']
    functions = sorted((v[0],n,v[1]) for n,v in symbols.items() if v[2]==2 and v[3]==ti)
    require(set(n for _,n,_ in functions) == set(MEMBERS+CONTEXT), 'complete function set')
    result = {}
    for k,(start,name,size) in enumerate(functions):
        end = functions[k+1][0] if k+1<len(functions) else len(raw)
        require(size>0 and size%4==0 and start+size<=end and not any(raw[start+size:end]), 'extent/padding')
        if name in MEMBERS:
            if exact: require(size==binding['targets'][name]['size'], 'complete member size')
            require(all(sym!='.rodata' for off,typ,sym,dest in rels if dest==ti and start<=off<start+size), 'member owned unexpected data')
        result[name] = dict(offset=start,size=size,next_boundary=end,zero_padding=end-start-size)
    for name,(idx,row,data) in sections.items():
        require(not(row[2]&2) or not row[5] or name in ('.text','.rodata','.reginfo','.options'), 'unexpected allocated input section')
    require(len(sections['.rodata'][2])==16 and sections['.rodata'][2][8:]==bytes(8), 'context data/padding')
    require(not any(dest==sections['.rodata'][0] for off,typ,sym,dest in rels), 'unexpected data relocations')
    return dict(text_bytes=len(raw),functions=result,member_owned_data_bytes=0,
                context_literal_bytes=8,context_data_padding_bytes=8)


def gnu_link(obj, temp, binding, exact=True):
    sections,symbols,rels = elf.read(obj)
    ti,_,raw = sections['.text']
    lines,external,counts = ['.set noreorder','.set noat'],set(),{}
    covered = set()
    for name in MEMBERS+CONTEXT:
        start,size,kind,index = symbols[name]
        require(kind==2 and index==ti, 'function kind')
        words=list(struct.unpack('>%dI'%(size//4),raw[start:start+size]));records=[]
        for off,typ,sym,dest in rels:
            if dest!=ti or not start<=off<start+size:continue
            require(typ in (4,5,6) and off%4==0,'unsupported relocation')
            local=off-start
            require((off,typ,sym,dest) not in covered,'duplicate relocation')
            covered.add((off,typ,sym,dest))
            if sym=='.text':
                require(typ==4,'unsupported text reference')
                addend=(words[local//4]&0x3ffffff)*4
                owners=[n for n,v in symbols.items() if v[2]==2 and v[3]==ti and v[0]==addend]
                require(len(owners)==1,'non-entry/unknown call')
                sym=owners[0];words[local//4]&=0xfc000000
            if sym=='.rodata':
                require(name in CONTEXT,'member references unverified data')
                sym='context_literals'
            require(not sym.startswith('.'),'unowned section reference')
            records.append((local,typ,sym));external.add(sym)
        lines += ['.section .review_'+name+',"ax",@progbits','.balign 4','.globl '+name,
                  '.type '+name+',@function',name+':']
        lines += ['.word 0x%08x'%w for w in words]
        lines += ['.size %s,.-%s'%(name,name)]
        lines += ['.reloc %s+%d,%s,%s'%(name,o,{4:'R_MIPS_26',5:'R_MIPS_HI16',6:'R_MIPS_LO16'}[t],s) for o,t,s in records]
        counts[name]=len(records)
    require(covered == set(rels),'unaccounted original relocation')
    lines += ['.section .context_data,"a",@progbits','.balign 4','.globl context_literals','context_literals:']
    lines += ['.word 0x%08x'%w for w in struct.unpack('>4I',sections['.rodata'][2])]
    source,assembled,linked,script=[temp/n for n in ('gnu.s','gnu.o','gnu.elf','gnu.ld')]
    source.write_text('\n'.join(lines)+'\n');table=score.image_symbols();anchors={}
    for name in sorted(external-set(MEMBERS+CONTEXT)-{'context_literals'}):
        address=table.get(name,score.address_named(name));require(address is not None,'unknown external '+name);anchors[name]=address
    script.write_text('SECTIONS {\n'+''.join('.review_%s 0x%s : SUBALIGN(4) { *(.review_%s) }\n'%(n,n[-8:],n) for n in MEMBERS+CONTEXT)+
        '.context_data 0x80400000 : SUBALIGN(4) { *(.context_data) }\n'+
        '/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n'+
        ''.join('%s = 0x%X;\n'%(n,a) for n,a in sorted(anchors.items())))
    shell('mips-linux-gnu-as','-EB','-mips2','-o',assembled,source)
    shell('mips-linux-gnu-ld','-EB','-T',script,'-o',linked,assembled)
    ss,sy,rr=elf.read(linked);require(not rr,'retained GNU relocation')
    require(all(v[3]!=0 for n,v in sy.items() if n),'undefined GNU symbol')
    require(all(sy[n][0]==a for n,a in anchors.items()),'GNU external address drift')
    expected={'.review_'+n for n in MEMBERS+CONTEXT}|{'.context_data'}
    require({n for n,(i,r,d) in ss.items() if r[2]&2 and r[5]}==expected,'unexpected allocated linked section')
    require(ss['.context_data'][1][3]==0x80400000 and ss['.context_data'][2]==sections['.rodata'][2],'context literal reconstruction')
    result={}
    for name in MEMBERS+CONTEXT:
        idx,row,data=ss['.review_'+name];size=symbols[name][1];address=int(name[-8:],16)
        require(row[3]==address and len(data)==size and sy[name]==(address,size,2,idx),'GNU whole extent')
        result[name]=list(struct.unpack('>%dI'%(size//4),data))
    if exact:require(all(result[n]==score.targets()[n] for n in MEMBERS),'GNU full member bytes')
    return result,dict(relocations=counts,external_bindings={n:hex(a) for n,a in anchors.items()},
                       member_hashes={n:digest(packed(result[n])) for n in MEMBERS},
                       context_candidate_data_sha256=digest(sections['.rodata'][2]),
                       context_data_is_diagnostic_only=True)


def selective(obj,temp,image):
    spec=ovl_splice.packet_spec(GROUP)
    root=temp/'installer';folder=root/GROUP_NAME;folder.mkdir(parents=True)
    (folder/'group.json').write_text(json.dumps(spec));(folder/'group.c').write_bytes((GROUP/'group.c').read_bytes())
    objects=temp/'objects';objects.mkdir();shutil.copyfile(obj,blob_group.object_path(GROUP_NAME,objects))
    image_path=temp/'image.bin';image_path.write_bytes(image)
    doc=ovl_layout.derive('a',image_path=image_path,write=False)
    bodies=blob_group.group_bodies(GROUP_NAME,doc,extern=score.image_symbols(),obj_dir=objects,root=root)
    require(set(bodies)==set(MEMBERS),'selective output set')
    require(all(ovl_splice.mismatch(doc,n,b) is None for n,b in bodies.items()),'selective output mismatch')
    return {n:len(b) for n,b in sorted(bodies.items())}


def replay(deep=False):
    binding=inputs();score.ido('cc');require(compilers.identify_dir(score.IDO)=='ido5.3','stock IDO5.3')
    compiler={n:digest((score.IDO/n).read_bytes()) for n in ('cc','cfe','uld','usplit','umerge','uopt','ugen','as1')}
    with targets(),tempfile.TemporaryDirectory(prefix='reset-proof-') as tmp:
        temp=Path(tmp);obj=temp/'candidate.o';score.compile_group(GROUP,obj)
        results={n:asdict(score.compare(obj,n,show=0)) for n in MEMBERS}
        require(all(score.compare(obj,n,show=0).accepted() for n in MEMBERS),'strict group failure')
        extents=extent_evidence(obj,binding);linked,gnu=gnu_link(obj,temp,binding)
        image,_=authenticate();installation=selective(obj,temp,image)
        raw=obj.read_bytes();sections,syms,rels=elf.read(obj)
        mutant=bytearray(raw);mutant[sections['.text'][1][4]+syms[NEW][0]+3]^=1
        bad=temp/'bad.o';bad.write_bytes(mutant)
        require(not score.compare(bad,NEW,show=0).accepted(),'mutated instruction accepted')
        # Claimed storage cannot be silently introduced by a malformed ELF view.
        over=bytearray(raw);table=sections['.symtab'];changed=0
        for at in range(0,len(table[2]),16):
            no,val,size,info,other,idx=struct.unpack_from('>IIIBBH',table[2],at)
            if (val,size,info&15,idx)==syms[NEW]:struct.pack_into('>I',over,table[1][4]+at+8,size+4);changed+=1
        require(changed==1,'unique extent mutation');bad.write_bytes(over)
        try:extent_evidence(bad,binding)
        except ValueError:pass
        else:raise ValueError('overlong extent accepted')
        controls_out={}
        for label,source in controls().items():
            folder=temp/label;folder.mkdir();(folder/'group.c').write_text(source);(folder/'group.json').write_bytes((GROUP/'group.json').read_bytes())
            co=folder/'candidate.o';score.compile_group(folder,co)
            require(all(score.compare(co,n,show=0).accepted() for n in OLD),'control changed preserved members')
            sc=score.compare(co,NEW,show=0);require(not sc.accepted(),'ineffective causal control')
            ex=extent_evidence(co,binding,exact=False);gw,gp=gnu_link(co,folder,binding,exact=False)
            native=score.targets()[NEW];body=gw[NEW]
            controls_out[label]=dict(score=asdict(sc),complete_bytes=len(body)*4,gnu_differing=sum(a!=b for a,b in zip(native,body))+abs(len(native)-len(body)))
        native=score.targets()[NEW]
        require(native[0]==0x27bdffe8 and native[1]>>21&31==17,'native mode=s1 entry')
        require(native[5]>>16&31==16 and native[177]==0x02002825,'native player=s0; selector=a1')
        result=dict(inputs=binding,compiler_sha256=compiler,results=results,extents=extents,gnu=gnu,
            contexts={n:asdict(score.compare(obj,n,show=0)) for n in CONTEXT},controls=controls_out,
            selective_installer_bytes=installation,native_private_abi=dict(player='s0',mode='s1',frame_bytes=24,entry_registers_bound_by_genuine_group=True,native_call_sites=24,native_caller_bodies=5),
            negative_controls=['instruction corruption rejected','overlong full extent rejected'],new_claim_bytes=728,preserved_bytes=1120,accepted_image_bytes=0)
        if deep:behavior(temp)
        return result


def behavior(temp):
    source=(GROUP/'group.c').read_text()
    # The exact reset body and types run unchanged. Only its external selector
    # implementation is replaced with a call-boundary test double.
    source=source.replace('void func_80394564(s32 player)\n{','void unused_selector_implementation(s32 player)\n{',1)
    out=temp/'behavior.c';out.write_text('void func_80394564(int);\n'+source+'\n'+(HERE/'behavior.c').read_text())
    exe=temp/'behavior'
    shell('cc','-std=c89','-O2','-fstrict-aliasing','-ffunction-sections','-fdata-sections',
          '-Wl,--gc-sections','-fsanitize=undefined,bounds','-fno-sanitize-recover=all',out,'-o',exe)
    print(shell(exe).strip())


def verify(deep=False):
    receipt=json.loads((HERE/'verification.json').read_text());require(inputs()==receipt['inputs'],'receipt input drift')
    result=replay(deep);require(result==receipt,'receipt proof drift');return result


if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--record',action='store_true');parser.add_argument('--deep',action='store_true');parser.add_argument('--score',action='store_true');args=parser.parse_args()
    if args.record:(HERE/'verification.json').write_text(json.dumps(replay(args.deep),indent=2)+'\n')
    elif args.score:
        with targets():
            sys.argv=['score.py','group',str(GROUP),'--targets',str(ROOT/'asm/us/ovl_a')]
            raise SystemExit(score.main())
    else:verify(args.deep)
    print('PASS: 728 new bytes; 1120 preserved; complete GNU/extent/selective proof')
