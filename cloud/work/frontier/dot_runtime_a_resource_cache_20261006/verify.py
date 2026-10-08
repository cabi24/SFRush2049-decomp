#!/usr/bin/env python3
"""Rebuild the complete research object and check bounded native/C behavior."""
import argparse,ctypes,hashlib,importlib.util,json,os,struct,subprocess,sys
from pathlib import Path
import native
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
BASE="cd22879d40b3de443cfde047b86e75e159b6cec6"
NAME='func_80390BC0'
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
ANCHORS={'D_803BA230':0x803BA230,'D_80142B08':0x80142B08,'audio_frame_sync':0x80097798,'func_800BB02C':0x800BB02C}
DIFFS=[0x120,0x124,0x12c,0x130,0x134]

def sha(b):return hashlib.sha256(b).hexdigest()
def require(ok,message):
    if not ok:raise AssertionError(message)
def sh(args):
    p=subprocess.run([str(x) for x in args],capture_output=True,text=True)
    require(p.returncode==0,(args,p.stdout,p.stderr));return p.stdout

def pinned(repo, path):
    return subprocess.check_output(["git", "-C", str(repo), "show", BASE + ":" + path])

def elf(path):
    b=path.read_bytes();require(b[:6]==b'\x7fELF\x01\x02','ELF class/endianness')
    require(struct.unpack_from('>H',b,18)[0]==8,'MIPS ELF')
    at=struct.unpack_from('>I',b,32)[0];stride,n,ni=struct.unpack_from('>HHH',b,46);require(stride==40,'section size')
    rows=[struct.unpack_from('>10I',b,at+i*stride) for i in range(n)]
    nr=rows[ni];names=b[nr[4]:nr[4]+nr[5]];sections={};symbols={};relocs=[]
    for i,r in enumerate(rows):
        name=names[r[0]:].split(b'\0')[0].decode();raw=b[r[4]:r[4]+r[5]];sections[name]=(i,r,raw)
        if r[1]==2:
            st=rows[r[6]];strings=b[st[4]:st[4]+st[5]]
            for off in range(0,len(raw),16):
                no,v,size,info,other,index=struct.unpack_from('>IIIBBH',raw,off);name=strings[no:].split(b'\0')[0].decode()
                if name:symbols[name]=(v,size,info&15,index)
        if r[1]==9:
            relocs.extend(struct.unpack_from('>II',raw,o) for o in range(0,len(raw),8))
    return sections,symbols,relocs

# Portable ELF fingerprint reused unchanged from reviewed PR #131,
# commit 31d8af99a3b08a1b52763aef0ecb3c7eb9c1eb65.
def portable_elf(data):
    """Bind every section and ABI attribute except nonallocated ECOFF debug data.

    Absolute source paths change .mdebug size and physical file offsets. Neither
    executable/data bytes, relocations, symbols nor MIPS ABI metadata are masked.
    """
    assert data[:6]==b"\x7fELF\x01\x02"
    header=struct.unpack_from('>HHIIIIIHHHHHH',data,16)
    assert header[0]==1 and header[1]==8 and header[9]==0
    shoff,shsize,shnum,names_index=header[5],header[10],header[11],header[12]
    assert shsize==40 and shoff+shnum*shsize<=len(data)
    sections=[struct.unpack_from('>10I',data,shoff+shsize*i) for i in range(shnum)]
    names=sections[names_index];records=[]
    for row in sections:
        index,typ,flags,addr,offset,size,link,info,align,entry_size=row
        assert typ==8 or offset+size<=len(data)
        assert index<names[5]
        start=names[4]+index
        name=data[start:data.index(b'\0',start,names[4]+names[5])].decode()
        record=dict(name=name,type=typ,flags=flags,address=addr,link=link,
                    info=info,alignment=align,entry_size=entry_size)
        if name=='.mdebug':
            assert typ==0x70000005 and flags==0 and addr==0,'unexpected allocated/debug section'
            record['scope']='nonallocated ECOFF debug metadata; fingerprinted separately'
        else:
            record['size']=size
            record['sha256']=sha(data[offset:offset+size]) if typ!=8 else None
        records.append(record)
    return {'ident_sha256':sha(data[:16]),'header':list(header[:5])+list(header[6:]),'sections':records}



def portable(receipt):
    """Exclude historical integration and proven path-only debug provenance."""
    r=json.loads(json.dumps(receipt))
    require("portable_elf" in r and "portability_controls" in r,"missing portable proof")
    for field in ("object_sha256","mdebug_sha256"):r.pop(field)
    for label,control in r["controls"].items():
        require("portable_elf" in control,("missing control fingerprint",label))
        for field in ("object_sha256","mdebug_sha256"):control.pop(field)
    for field in ("game_manifest", "protected_manifest", "scorer_sha256"):
        r.pop(field, None)
    for section in ("matched_siblings", "helper_contracts"):
        require(isinstance(r[section], dict), ("invalid context proof", section))
        for record in r[section].values():
            require(isinstance(record, dict), ("invalid context record", section))
            record.pop("source_sha256", None)
    return r

def inspect(path,linked=False):
    sections,symbols,relocs=elf(path);i,r,raw=sections['.text']
    funcs={n:s for n,s in symbols.items() if s[2]==2 and s[3] not in (0,0xfff1)}
    require(set(funcs)=={NAME},'unexpected function set');v,size,typ,index=funcs[NAME]
    require(v==(native.ENTRY if linked else 0) and size==364 and index==i,'function extent/address')
    require(r[3]==(native.ENTRY if linked else 0) and len(raw)==368 and raw[364:]==bytes(4),'complete text/alignment')
    for name,(_,row,_) in sections.items():
        if row[2]&2 and name not in ('.text','.options','.reginfo'):require(row[5]==0,('owned data',name))
    if linked:
        require(not relocs,'remaining linked relocations')
        for name,address in ANCHORS.items():require(symbols[name][0]==address and symbols[name][3]==0xfff1,'linked anchor')
    return raw,relocs

def packed(m):return bytes(m[a] for lo,hi in ((native.RECORDS,native.RECORDS+192),(native.TABLE,native.TABLE+26),(native.OUT,native.OUT+4)) for a in range(lo,hi))
def fixtures():
    serial=0;kinds=(-32768,-89,-88,-1,0,1,167,32767);handles=(-2147483648,-32769,-1,0,1,63,32767,32768,2147483647)
    for ident in range(13):
        for pos in range(16):
            for scenario in range(7):
                rows=[(0x13570000+i,0,20+i) for i in range(16)]
                if scenario==1:rows[pos]=(0x98370000+pos,1,ident)
                elif scenario==2:
                    rows[pos]=(0x43210000+pos,0,ident)
                    if pos:rows[0]=(0x54321098,0,-7)
                elif scenario==3:rows[pos]=(0x65781234,0,-1)
                elif scenario==4:
                    rows[0]=(0x43210987,0,ident);rows[15]=(0x12348765,255,ident)
                elif scenario==5:rows[pos]=(0x98234567,128,ident)
                elif scenario==6:rows[pos]=(0x98765432,0,-128)
                for mode in (0,1):
                    yield native.state(rows,serial),ident,kinds[serial%8],handles[serial%9],mode,serial
                    serial+=1
    for loaded in range(256):
        for pos in range(16):
            ident=pos%13;rows=[(i*17,0,40+i) for i in range(16)];rows[pos]=(0x82340000+loaded,loaded,ident)
            yield native.state(rows,serial),ident,kinds[loaded%8],handles[loaded%9],loaded%2,serial;serial+=1
    for sentinel in range(-128,0):
        for pos in range(16):
            ident=pos%13;rows=[(i*19,0,60+i) for i in range(16)];rows[pos]=(0x56781234,0,sentinel)
            yield native.state(rows,serial),ident,kinds[pos%8],handles[pos%9],pos%2,serial;serial+=1

def host_library(source,build,label):
    out=build/(label+'.so')
    sh(['gcc','-std=c89','-O2','-fstrict-aliasing','-shared','-fPIC','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-DCANDIDATE="'+str(source)+'"',HERE/'host.c','-o',out])
    lib=ctypes.CDLL(str(out));lib.host_run.argtypes=[ctypes.c_void_p,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_void_p,ctypes.c_void_p,ctypes.c_void_p]
    return lib

def host(lib,m,ident,kind,handle,mode):
    inp=ctypes.create_string_buffer(packed(m));out=ctypes.create_string_buffer(222);events=ctypes.create_string_buffer(444);args=(ctypes.c_int*13)()
    result=lib.host_run(inp,ident,kind,native.signed(handle),mode,out,events,args);calls=[]
    for i in range(args[12]):
        address=native.LOAD if args[i*6]==1 else native.SETUP
        values=tuple(args[i*6+1:i*6+6]) if address==native.LOAD else tuple(args[i*6+1:i*6+4])
        calls.append((address,values,events.raw[i*222:(i+1)*222]))
    return out.raw,result,calls

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--repo',type=Path,default=ROOT);ap.add_argument('--output',type=Path,default=HERE/'verification.json');a=ap.parse_args()
    repo=a.repo.resolve();build=ROOT/'build/runtime_a_cache';build.mkdir(parents=True,exist_ok=True);tmp=build/'tmp';tmp.mkdir(exist_ok=True)
    os.environ['TMPDIR']=str(tmp);import tempfile;tempfile.tempdir=str(tmp)
    sys.path.insert(0,str(repo/'tools/cloud'));spec=importlib.util.spec_from_file_location('cache_score',repo/'tools/cloud/score.py');score=importlib.util.module_from_spec(spec);sys.modules[spec.name]=score;spec.loader.exec_module(score)
    score.ASM_DIR=repo/'asm/us/ovl_a';manifest=score.target_manifest();words=score.targets()[NAME];target=struct.pack('>91I',*words)
    metadata=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest));row=next(r for r in metadata['functions'] if r['name']==NAME)
    require(metadata['image']=='A' and metadata['rom_offset']=='0xB5C534' and row['address']=='0x80390BC0' and row['size']==364,'target identity')
    require(metadata['image_sha256']=='0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667','image identity')
    addresses=score.image_symbols()
    for n,v in ANCHORS.items():require(addresses.get(n,score.address_named(n))==v,('anchor drift',n))
    require(sha(target)=='02e3869e40edbc543340d6887bda6ec3743edb8d793990921b7432c6d935231e','native body drift')
    source=HERE/'candidate.c';obj=build/'candidate.o';score.compile_single(source,FLAGS,obj);raw,relocs=inspect(obj);comparison=score.compare(obj,NAME,show=0)
    full=score.text_words(obj);resolved,masks,unresolved,unverified,errors=score.relocate(obj,full,0,len(full)*4,addresses)
    require(not any((masks,unresolved,unverified,errors)),'unverified relocation')
    require([i*4 for i,(x,y) in enumerate(zip(words,resolved)) if x!=y]==DIFFS and len(resolved)==92 and resolved[-1]==0,'unexpected residual')
    script=build/'whole.ld';script.write_text('SECTIONS { .text 0x80390BC0 : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.options) *(.reginfo) *(.mdebug) *(.pdr) *(.gnu.attributes) } }\n'+''.join('%s = 0x%X;\n'%(n,v) for n,v in ANCHORS.items()))
    linked=build/'candidate.elf';sh(['mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj]);linkedraw,_=inspect(linked,True)
    require(linkedraw==struct.pack('>92I',*resolved),'GNU/production disagreement')
    layout=build/'layout.c';layout.write_text('#include "'+str(source)+'"\n'+'typedef char a[sizeof(ResourceCache)==12?1:-1];\ntypedef char b[__builtin_offsetof(ResourceCache,loaded)==4?1:-1];\ntypedef char c[__builtin_offsetof(ResourceCache,id)==5?1:-1];\ntypedef char d[sizeof(void*)==4?1:-1];\n')
    sh(['gcc','-m32','-std=c89','-fsyntax-only',layout])
    lib=host_library(source,build,'host');coverage=[set(),set(),set()];branches=[set(),set(),set()];counts={};cases=0;digest=hashlib.sha256();fixture_list=list(fixtures())
    for m,ident,kind,handle,mode,seed in fixture_list:
        expected,ret,calls,route=native.oracle(m,ident,kind,handle,mode);counts[route]=counts.get(route,0)+1
        for j,body in enumerate((words,resolved[:91],list(struct.unpack('>91I',linkedraw[:364])))):
            result=native.run(body,m,ident,kind,handle,mode,seed)
            require(result['memory']==expected and result['return']==ret and result['calls']==calls,('native discrepancy',j,seed,route))
            coverage[j]|=result['coverage'];branches[j]|=result['branches']
        hc=host(lib,m,ident,kind,handle,mode);wanted=(packed(expected),ret,[(addr,args,packed(snapshot)) for addr,args,snapshot in calls])
        require(hc==wanted,('host discrepancy',seed,route));digest.update(repr((hc[0].hex(),hc[1],[(x[0],x[1],sha(x[2])) for x in hc[2]])).encode());cases+=1
    mutants={
      'loaded_equals_one':source.read_text().replace('record->loaded != 0','record->loaded == 1'),
      'only_minus_one_free':source.read_text().replace('D_803BA230[i].id < 0','D_803BA230[i].id == -1'),
      'ignores_unloaded_match':source.read_text().replace('if (D_803BA230[i].id == id) {','if (D_803BA230[i].id == id && D_803BA230[i].loaded) {'),
      'rejects_negative_handle':source.read_text().replace('    *result = record->handle;\n    record->loaded = 1;','    if (record->handle < 0) return 0;\n    *result = record->handle;\n    record->loaded = 1;')}
    negatives={}
    for label,body in mutants.items():
        require(body!=source.read_text(),'ineffective mutant');p=build/(label+'.c');p.write_text(body);mo=build/(label+'.o');score.compile_single(p,FLAGS,mo);comp=score.compare(mo,NAME,show=0);require(not comp.accepted(),'mutant matched');ml=host_library(p,build,label);detected=None
        for item in fixture_list:
            m,ident,kind,handle,mode,seed=item
            if mode:continue
            expected,ret,calls,route=native.oracle(m,ident,kind,handle,mode)
            if host(ml,m,ident,kind,handle,mode)!=(packed(expected),ret,[(addr,args,packed(snapshot)) for addr,args,snapshot in calls]):detected=seed;break
        require(detected is not None,('mutant undetected',label));require(portable_elf(mo.read_bytes())!=portable_elf(obj.read_bytes()),('mutant ELF not distinguished',label));negatives[label]={'comparison':comp.__dict__,'detected_fixture':detected,'portable_elf_differs':True}
    # Unknown instructions and wrong full-function extents fail closed.
    for bad in ([0xffffffff]+words[1:],words[:-1],words+[0]):
        try:native.run(bad,*fixture_list[0])
        except AssertionError:pass
        else:raise AssertionError('invalid instruction/extent accepted')
    contexts={}
    for n in ('func_8039156C','func_80390D2C'):
        p=build/(n+'.c');p.write_bytes(pinned(repo,'cloud/matches/ovl_a/'+n+'.c'));o=build/(n+'.o');score.compile_single(p,score.DEFAULT_FLAGS,o);c=score.compare(o,n,show=0);require(c.accepted(),('sibling regression',n));contexts[n]={'native_sha256':sha(struct.pack('>%dI'%len(score.targets()[n]),*score.targets()[n])),'comparison':c.__dict__}
    # Bind both actual helper bodies and their independently documented source
    # contracts without presenting those helper reconstructions as new matches.
    score.ASM_DIR=repo/'asm/us/blob';game_manifest=score.target_manifest();game_words=score.targets();game_symbols=score.image_symbols()
    helper_sources={'audio_frame_sync':'src/blob/groups/audio_frame_sync/group.c','func_800BB02C':'cloud/work/tiny_A93/func_800BB02C.c'}
    helpers={}
    for n,rel in helper_sources.items():
        require(game_symbols[n]==ANCHORS[n],('helper entry drift',n));body=game_words[n]
        pinned(repo, rel)  # Retain the frozen source witness without serializing its digest.
        helpers[n]={'address':hex(ANCHORS[n]),'bytes':len(body)*4,'native_sha256':sha(struct.pack('>%dI'%len(body),*body)),'source':rel,'scope':'read-only source/native contract witness; not compiled or executed as helper internals'}
    callers=[]
    for n,body in game_words.items():
        for i,w in enumerate(body):
            if w>>26==3 and (0x80000000|((w&0x3ffffff)<<2))==native.ENTRY:callers.append({'image':'game','function':n,'call_address':hex(game_symbols[n]+i*4)})
    score.ASM_DIR=repo/'asm/us/ovl_a'
    for n,body in score.targets().items():
        for i,w in enumerate(body):
            if w>>26==3 and (0x80000000|((w&0x3ffffff)<<2))==native.ENTRY:callers.append({'image':'A','function':n,'call_address':hex(addresses[n]+i*4)})
    controls={};base=source.read_text()
    recipes={
      'literal_bounds_O2':(base.replace('    s32 count = 16;\n','').replace('count','16').replace('i != 16','i < 16'),FLAGS.replace('-O3','-O2')),
      'equality_bounds_O2':(base.replace('    s32 count = 16;\n','').replace('count','16'),FLAGS.replace('-O3','-O2')),
      'shared_count_O2':(base,FLAGS.replace('-O3','-O2')),
      'shared_count_O1':(base,FLAGS.replace('-O3','-O1')),
      'chained_result':(base.replace('    record->handle = audio_frame_sync(kind + 88, 1, 1, 0, 0);\n    *result = record->handle;','    *result = record->handle = audio_frame_sync(kind + 88, 1, 1, 0, 0);'),FLAGS),
      'publish_loaded_last':(base.replace('    record->loaded = 1;\n    D_80142B08[id] = record->handle;','    D_80142B08[id] = record->handle;\n    record->loaded = 1;'),FLAGS),
      'result_first':(base.replace('    record->handle = audio_frame_sync(kind + 88, 1, 1, 0, 0);\n    *result = record->handle;','    *result = audio_frame_sync(kind + 88, 1, 1, 0, 0);\n    record->handle = *result;'),FLAGS)}
    for label,(body,flags) in recipes.items():
        p=build/(label+'.c');p.write_text(body);o=build/(label+'.o');score.compile_single(p,flags,o);sc=score.compare(o,NAME,show=0);sec,syms,rels=elf(o)
        controls[label]={'flags':flags,'source_sha256':sha(body.encode()),'object_sha256':sha(o.read_bytes()),'mdebug_sha256':sha(sec['.mdebug'][2]),'portable_elf':portable_elf(o.read_bytes()),'function_bytes':syms[NAME][1],'comparison':sc.__dict__}
    path_controls={}
    all_objects={'candidate':(source,FLAGS,obj)}
    all_objects.update({label:(build/(label+'.c'),flags,build/(label+'.o')) for label,(_,flags) in recipes.items()})
    # Two fixed scratch pairs are reused across every recipe to bound inode use.
    for label,(sp,flags,op) in all_objects.items():
        reference=portable_elf(op.read_bytes());runs=[]
        for directory in (ROOT/'build/tmp',build/'tmp'):
            directory.mkdir(parents=True,exist_ok=True)
            moved=directory/'path.c';alt=directory/'path.o';moved.write_bytes(sp.read_bytes());score.compile_single(moved,flags,alt)
            data=alt.read_bytes();require(portable_elf(data)==reference,('source-path changed semantic ELF',label))
            debug=elf(alt)[0]['.mdebug'][2];require(debug.count(str(moved).encode()+b'\0')==1,'source-path debug binding')
            runs.append((sha(data),sha(debug)))
        require(runs[0][0]!=runs[1][0] and runs[0][1]!=runs[1][1],'path variation ineffective')
        path_controls[label]={'identical_source_two_absolute_paths':True,'only_nonallocated_mdebug_and_physical_offsets_vary':True,'source_path_bound_in_debug':True}
    raw_object=obj.read_bytes();reference=portable_elf(raw_object);sections,_,_=elf(obj);semantic_mutations={}
    for name in ('.text','.rel.text','.symtab','.strtab','.reginfo','.options'):
        section=sections[name][1];require(section[5]>0,('empty mutation section',name))
        altered=bytearray(raw_object);altered[section[4]]^=1
        require(portable_elf(altered)!=reference,('semantic mutation accepted',name));semantic_mutations[name]='rejected'
    altered=bytearray(raw_object);altered[39]^=1;require(portable_elf(altered)!=reference,'ABI mutation accepted');semantic_mutations['ELF_ABI_flags']='rejected'
    receipt={'status':'NONMATCH','image':'A','base':'cd22879d40b3de443cfde047b86e75e159b6cec6','address':hex(native.ENTRY),'end':hex(native.ENTRY+364),'native_bytes':364,'function_bytes':364,'text_bytes':368,'zero_alignment_bytes':4,'owned_data_bytes':0,'flags':FLAGS,'differing_offsets':[hex(x) for x in DIFFS],'comparison':comparison.__dict__,'source_sha256':sha(source.read_bytes()),'native_sha256':sha(target),'object_sha256':sha(obj.read_bytes()),'mdebug_sha256':sha(sections['.mdebug'][2]),'portable_elf':reference,'portability_controls':{'source_paths':path_controls,'semantic_mutations':semantic_mutations},'gnu_text_sha256':sha(linkedraw),'relocations':len(relocs),'anchors':{n:hex(v) for n,v in ANCHORS.items()},'cases':cases,'native_executions':cases*3,'routes':counts,'instruction_coverage':[len(x) for x in coverage],'branch_outcomes':[sorted(x) for x in branches],'host_trace_sha256':digest.hexdigest(),'mutants':negatives,'matched_siblings':contexts,'helper_contracts':helpers,'direct_callers':callers,'controls':controls,'files':{p.name:sha(p.read_bytes()) for p in (source,HERE/'host.c',HERE/'native.py',Path(__file__))},'tools':{n:sha((score.IDO/n).read_bytes()) for n in ('cc','cfe','uopt','ugen','as1')},'limits':['ID 0..12 and valid disjoint output/table/cache storage','Signed16 kind and arbitrary32 returned handle are O32 boundary stress; actual resource validity and helper success are not proved','Helpers are side-effecting hooks; helper internals and whole-game callers do not execute','No concurrency, gameplay, image, compression, ROM, CI, accepted-byte or original-source claim']}
    a.output.write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps({k:receipt[k] for k in ('status','comparison','cases','native_executions','routes','instruction_coverage','relocations')},indent=2))
if __name__=='__main__':
    require(__debug__,'Python optimization is unsupported: invariant checks must be enabled')
    main()
