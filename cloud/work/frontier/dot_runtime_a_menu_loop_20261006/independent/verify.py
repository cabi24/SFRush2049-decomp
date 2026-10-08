"""Independent source/native menu-loop review. Local-only generated material."""
import argparse,ctypes,hashlib,json,os,random,re,struct,subprocess,sys,copy
if not __debug__: raise RuntimeError("Assertions are required for verification")
from pathlib import Path
HERE=Path(__file__).resolve().parent
PACKET=HERE.parent
ROOT=PACKET.parents[3]
OUT=ROOT/"build/runtime_a_menu_loop_independent"
SOURCE=PACKET/"candidate.c"
ENTRY,END=0x803A4340,0x803A44EC
COUNT,MODE,FLAGS,TABLES,NAME,MENU=0x8014A108,0x8014A110,0x803BA028,0x80140BDC,0x803B85E8,0x8017A4E0
TEX,IDX,TEXT,TOK=0x80400000,0x80401000,0x80420000,0x80500000
CALLS={0x800B65B4:(1,1),0x800B42F0:(2,1),0x800B74A0:(3,1),0x800B24EC:(4,5),0x800B3F50:(5,0),0x800B71D4:(6,3)}

def portable_receipt(receipt):
    result = json.loads(json.dumps(receipt))
    for key in ('manifest', 'helper_protected_manifest'):
        result.pop(key, None)
    return result


def sha(b): return hashlib.sha256(b).hexdigest()
def shell(*args): return subprocess.check_output([str(x) for x in args],stderr=subprocess.STDOUT).decode()
def verify_manifest(folder):
    result={}
    for line in (folder/'SHA256SUMS').read_text().splitlines():
        digest,name=line.split('  ')
        assert sha((folder/name).read_bytes()) == digest
        result[name]=digest
    return result
def target(folder,name):
    for p in folder.glob('*.s'):
        m=re.search(r'^\.section \.text\.'+name+r',.*?(?=^\.section|\Z)',p.read_text(),re.M|re.S)
        if m:
            w=[int(v,16) for v in re.findall(r'^\s*\.word\s+(0x[0-9a-fA-F]+)',m[0],re.M)]
            return struct.pack('>%dI'%len(w),*w)
    raise AssertionError(name)
def elf(path):
    b=path.read_bytes(); assert b[:6]==b'\x7fELF\x01\x02'
    assert struct.unpack_from('>H',b,18)[0]==8 and struct.unpack_from('>I',b,36)[0]&0xF0000000==0x10000000
    offset=struct.unpack_from('>I',b,32)[0]
    size,count,namesidx=struct.unpack_from('>HHH',b,46)
    sections=[struct.unpack_from('>10I',b,offset+i*size) for i in range(count)]
    def string(sec,off):
        start=sec[4]+off; return b[start:b.index(0,start)].decode()
    names={string(sections[namesidx],s[0]):s for s in sections}
    symbol_tables={}; symbols={}; relocs=[]
    for idx,s in enumerate(sections):
        if s[1] == 2:
            syms=[]
            for off in range(s[4],s[4]+s[5],16):
                n,value,sz,info,other,shndx=struct.unpack_from('>IIIBBH',b,off)
                syms.append(dict(name=string(sections[s[6]],n),value=value,size=sz,info=info,section=shndx))
            symbol_tables[idx]=syms
            symbols.update((x['name'],x) for x in syms)
    for s in sections:
        if s[1] in (4,9):
            assert s[1]==9
            for off in range(s[4],s[4]+s[5],8):
                loc,info=struct.unpack_from('>II',b,off)
                relocs.append(dict(offset=loc,type=info&255,symbol=symbol_tables[s[6]][info>>8]['name'],target_section=s[7]))
    s=names['.text']
    return dict(text=b[s[4]:s[4]+s[5]],text_address=s[3],symbols=symbols,sections=names,relocs=relocs,entry=struct.unpack_from('>I',b,24)[0])
def sx(n,bits=32):
    n&=(1<<bits)-1; return n if n < 1<<(bits-1) else n-(1<<bits)


def execute(body,players,mode,flag,tables,height,font,first,hook,variant,seed=0):
    words=struct.unpack('>107I',body);rng=random.Random(seed)
    r=[0]+[rng.getrandbits(32) for _ in range(31)];r[29]=0x807F0000;r[31]=0x81818180;r[4]=0xF1E2D3C4;initial=list(r)
    f=[rng.getrandbits(32) for _ in range(32)];initial_f=list(f)
    mem={};log=[];access=[];covered=set();decisions=set();lo=hi=0
    def put(a,v,size=4):
        for k,b in enumerate((v&((1<<(size*8))-1)).to_bytes(size,'big')):mem[a+k]=b
    def get(a,size=4):
        assert a%size==0,(hex(a),size)
        if size==4:
            for bank in range(2):
                offset=a-(TEXT+bank*0x100000)
                if 0<=offset<65539*4:return TOK+bank*0x100000+offset
        if FLAGS<=a<FLAGS+32768:return mem.get(a,flag&255)
        return int.from_bytes(bytes(mem[a+k] for k in range(size)),'big')
    put(COUNT,players,2);put(MODE,mode);put(TABLES,tables,1);put(TEX+18,height,2)
    put(MENU+12,IDX);put(MENU+16,TEXT);put(IDX+26,first,2);put(IDX+0x100+26,first+32769,2)
    pc=ENTRY;pending=None
    for step in range(2_000_000):
        if pc==initial[31]:
            assert pending is None and r[2]==1
            assert all(r[x]==initial[x] for x in list(range(16,24))+[28,29,30,31])
            assert f[20:]==initial_f[20:] and get(initial[29])==initial[4]
            return log,covered,decisions,access
        if pc in CALLS:
            kind,n=CALLS[pc]
            args=[f[12]] if kind==1 else [r[4+i] if i<4 else get(r[29]+16+4*(i-4)) for i in range(n)]
            if kind==4:
                assert args[0]==NAME and args[1]==initial[29]-10 and args[2]==0 and args[4]==1
                assert r[29]==initial[29]-112
                put(args[1],0xA55A,2)
            result=font if kind==5 else None
            log.append([kind]+[x&0xffffffff for x in args]+[0]*(6-n))
            if len(log)==hook and kind!=5:
                if variant==0:put(COUNT,0,2)
                elif variant==1:put(COUNT,2,2)
                elif variant==2:put(COUNT,1,2)
                elif variant==3:put(MODE,2)
                elif variant==4:put(MODE,0)
                elif variant==5:put(FLAGS,1,1)
                elif variant==6:put(FLAGS,0,1)
                elif variant==7:put(TABLES,get(TABLES,1)+129,1)
                elif variant==8:put(TEX+18,get(TEX+18,2)+32769,2)
                elif variant==9:font=-128 if font==637 else 637
                elif variant==10:put(MENU+12,IDX+0x100)
                elif variant==11:put(MENU+16,TEXT+0x100000)
                elif variant==12:put(MENU+12,IDX+0x100);put(MENU+16,TEXT+0x100000)
                elif variant==13:put(COUNT,-32768,2)
                else:assert False
            ret=r[31]
            for x in [1,2,3,*range(4,16),24,25]:r[x]=rng.getrandbits(32)
            for x in range(20):f[x]=rng.getrandbits(32)
            if kind==4:r[2]=TEX
            if kind==5:r[2]=result&0xffffffff
            r[0]=0;pc=ret;assert pending is None;continue
        assert ENTRY<=pc<END and pc%4==0,hex(pc)
        off=pc-ENTRY;covered.add(off);w=words[off//4];op,s,t,d=w>>26,w>>21&31,w>>16&31,w>>11&31
        imm=sx(w,16);dest=None;skip=False
        if op==15:r[t]=(w&65535)<<16
        elif op==13:r[t]=r[s]|(w&65535)
        elif op==9:r[t]=(r[s]+imm)&0xffffffff
        elif op==17:assert s==4;f[d]=r[t]
        elif op in (32,33,35,36,37,43):
            a=(r[s]+imm)&0xffffffff
            if op==43:
                assert initial[29]-112<=a<=initial[29];put(a,r[t]);access.append(('write',a,4))
            else:
                size=1 if op in (32,36) else 2 if op in (33,37) else 4
                v=get(a,size);r[t]=(sx(v,size*8) if op in (32,33) else v)&0xffffffff;access.append(('read',a,size))
        elif op in (1,4,5,6,20,21,22):
            if op==1:assert t==1;take=sx(r[s])>=0
            elif op in (4,20):take=r[s]==r[t]
            elif op in (5,21):take=r[s]!=r[t]
            else:take=sx(r[s])<=0
            decisions.add((off,take));dest=pc+4+imm*4 if take else pc+8
            if op in (20,21,22) and not take:skip=True;dest=None
        elif op==3:r[31]=pc+8;dest=((pc+4)&0xF0000000)|((w&0x3FFFFFF)<<2)
        elif op==0:
            fn=w&63
            if fn==0:r[d]=(r[t]<<(w>>6&31))&0xffffffff
            elif fn==3:r[d]=(sx(r[t])>>(w>>6&31))&0xffffffff
            elif fn==18:r[d]=lo
            elif fn==25:prod=r[s]*r[t];lo=prod&0xffffffff;hi=prod>>32
            elif fn==33:r[d]=(r[s]+r[t])&0xffffffff
            elif fn==35:r[d]=(r[s]-r[t])&0xffffffff
            elif fn==37:r[d]=r[s]|r[t]
            elif fn==42:r[d]=int(sx(r[s])<sx(r[t]))
            elif fn==8:dest=r[s]
            else:raise AssertionError((hex(pc),fn))
        else:raise AssertionError((hex(pc),op))
        r[0]=0
        if pending is not None:assert dest is None and not skip;pc,pending=pending,None
        else:pc,pending=pc+(8 if skip else 4),dest
    raise AssertionError('nontermination')

def fast_oracle(args):
    players,mode,skip,tables,height,font,first,hook,variant=args
    assert hook==0
    out=[[1,0,0,0,0,0,0],[2,12,0,0,0,0,0],[3,1,0,0,0,0,0]]
    if players==1 and sx(skip,8)!=1 and mode!=2:
        for row in range(4):
            out.append([4,NAME,0x807EFFF6,0,sx(tables-1,8)&0xffffffff,1,0])
            out.append([5,0,0,0,0,0,0])
            y=sx(48+2*row*(height//8)-font,16)&0xffffffff
            out.append([6,10,y,TOK+(first+row)*4,0,0,0])
    out.append([1,0xBF800000,0,0,0,0,0])
    return out

def inputs():
    cases=set()
    for flag in range(256):
        for tables in (0,1,128,129,255):
            cases.add((1,0,flag,tables,65535,637,65535,0,0))
    for tables in range(256):cases.add((1,0,0,tables,32768,-128,32767,0,0))
    for font in range(-128,638):cases.add((1,-2147483648,0,255,65535,font,65535,0,0))
    for first in (0,1,32767,32768,65532,65535):
        for height in (0,1,7,8,9,255,32767,32768,43455,43456,65528,65535):
            for font in (-128,-1,0,1,255,510,637):cases.add((1,0,0,1,height,font,first,0,0))
    for players in (-32768,-1,0,1,2,3,32767):
        for mode in (-2147483648,-1,0,1,2,2147483647):
            for flag in (0,1,255):cases.add((players,mode,flag,255,65535,-128,65535,0,0))
    for hook in range(1,17):
        for variant in range(14):
            for start in ((1,0,0),(0,0,0),(2,0,0),(1,2,0),(1,0,1)):
                cases.add((*start,129,65535,637,65535,hook,variant))
    return sorted(cases)

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--repo',type=Path,default=Path(os.environ.get('RUSH_PROTECTED_REPO',ROOT)));ap.add_argument('--out',type=Path,default=HERE/'review.json');ap.add_argument('--check',action='store_true');options=ap.parse_args()
    repo=options.repo.resolve();OUT.mkdir(parents=True,exist_ok=True)
    tmp=OUT/'tmp';tmp.mkdir(exist_ok=True);os.environ['TMPDIR']=str(tmp)
    import tempfile;tempfile.tempdir=str(tmp)
    manifest=verify_manifest(repo/'asm/us/ovl_a');verify_manifest(repo/'asm/us/ovl_b');game_manifest=verify_manifest(repo/'asm/us/blob')
    native=target(repo/'asm/us/ovl_a','func_803A4340')
    assert len(native)==428 and sha(native)=="1e306c7d79a18c4d7a008d658cf6abe17a5188817857ea3d4348ed8fe078ace7"
    extent=json.loads((repo/'asm/us/ovl_a/extents.json').read_text())
    assert extent['image']=='A' and extent['image_sha256']=='0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'
    assert next(x for x in extent['functions'] if x['name']=='func_803A4340')==dict(name='func_803A4340',address='0x803A4340',size=428,evidence=['data_ref','prologue'])
    other=json.loads((repo/'asm/us/ovl_b/extents.json').read_text());assert int(other['base'],16)+other['size']<ENTRY
    source_sha=sha(SOURCE.read_bytes());assert source_sha=='194c86d784441d782e0f57b59738b8c27fc127912383d820c31389ce6bcee7a0'
    compiler=Path(os.environ['IDO_DIR'])/'cc'
    compiler_args=['-g0','-O2','-mips2','-G','0','-non_shared','-Wab,-r4300_mul','-c']
    shell(compiler,*compiler_args,'-o',OUT/'candidate.o',SOURCE)
    obj=elf(OUT/'candidate.o');fn=obj['symbols']['func_803A4340']
    assert fn['value']==0 and fn['size']==428 and len(obj['text'])==432 and obj['text'][428:]==b'\0'*4
    assert {n for n,s in obj['symbols'].items() if s['info']&15==2 and s['section'] not in (0,0xfff1)}=={'func_803A4340'}
    assert not any(s[5] for n,s in obj['sections'].items() if s[2]&2 and n not in ('.text','.reginfo'))
    assert len(obj['relocs'])==21
    assert all(x['offset']<428 and x['offset']%4==0 and x['type'] in (4,5,6) and x['target_section']==fn['section'] for x in obj['relocs'])
    symbols=json.loads((repo/'asm/us/ovl_a/symbols.json').read_text())['symbols'];bindings={}
    for x in obj['relocs']:
        n=x['symbol'];addr=symbols.get(n)
        if addr is None:assert re.fullmatch(r'D_[0-9A-F]{8}',n);addr='0x'+n[2:]
        bindings[n]=int(addr,16)
    expected_bindings={'render_helper':0x800b65b4,'object_create':0x800b42f0,'dispatch_handler':0x800b74a0,'func_800B24EC':0x800b24ec,'object_bytes_sum_global':0x800b3f50,'state_utility':0x800b71d4,'D_8014A108':COUNT,'D_8014A110':MODE,'D_803BA028':FLAGS,'D_80140BDC':TABLES,'D_803B85E8':NAME,'D_8017A4E0':MENU}
    assert bindings==expected_bindings
    assert {n for n,v in obj['symbols'].items() if n and v['section']==0}==set(expected_bindings)
    (OUT/'whole.ld').write_text('OUTPUT_ARCH(mips)\nENTRY(func_803A4340)\nSECTIONS { .text 0x803A4340 : SUBALIGN(4) { *(.text) } }\n'+'\n'.join(f'{n} = 0x{v:X};' for n,v in bindings.items())+'\n')
    shell('mips-linux-gnu-ld','-EB','-T',OUT/'whole.ld','-o',OUT/'whole.elf',OUT/'candidate.o')
    linked=elf(OUT/'whole.elf')
    diffs=[]
    for off in range(0,428,4):
        a,b=struct.unpack_from('>I',native,off)[0],struct.unpack_from('>I',linked['text'],off)[0]
        if a!=b:diffs.append({'offset':off,'native_stack_operand':a&65535,'candidate_stack_operand':b&65535});assert (a>>16)==(b>>16) and (a&65535)==76 and (b&65535)==80
    assert len(diffs)==4
    def validate_linked(linked):
        assert linked['entry']==ENTRY and linked['text_address']==ENTRY and linked['symbols']['func_803A4340']['value']==ENTRY and linked['symbols']['func_803A4340']['size']==428
        assert len(linked['text'])==432 and linked['text'][428:]==b'\0'*4 and not linked['relocs']
        assert linked['text']==expected_linked_text
        assert all(linked['symbols'][n]['value']==v and linked['symbols'][n]['section']==0xfff1 for n,v in bindings.items())
        assert not any(s[5] for n,s in linked['sections'].items() if s[2]&2 and n not in ('.text','.reginfo'))
    expected_linked_text=linked['text'];validate_linked(linked)
    result=dict(status='NONMATCH',source_sha256=source_sha,native_sha256=sha(native),gnu_linked_body_sha256=sha(linked['text'][:428]),full_extent_bytes=428,whole_text_bytes=432,zero_padding_bytes=4,all_relocations=obj['relocs'],bindings={n:hex(v) for n,v in bindings.items()},distinct_image=True,owned_data_bytes=0,whole_linked_elf_match=False,residual=diffs)
    helpers={}
    for name in ('render_helper','object_create','dispatch_handler','func_800B24EC','object_bytes_sum_global','state_utility','sound_update_channel','func_80096288'):
        body=target(repo/'asm/us/blob',name)
        helper_symbols=json.loads((repo/'asm/us/blob/symbols.json').read_text())['symbols']
        helpers[name]={'address':helper_symbols[name],'bytes':len(body),'sha256':sha(body)}
    result['helper_native_body_bindings']=helpers
    result['layout']='32-bit pointers; 36-byte Texture; offsets Texture.height=18, MenuIndices.first=26, MenuData.indices=12, MenuData.text=16'
    result['elf32_big_endian_mips2']=True
    layout='#include "'+str(SOURCE)+'"\n'+'typedef char ptr32[sizeof(void*)==4?1:-1];\n'
    for typ,field,offset in [('Texture','height',18),('MenuIndices','first',26),('MenuData','indices',12),('MenuData','text',16)]:layout+=f'typedef char layout_{typ}_{field}[__builtin_offsetof({typ},{field})=={offset}?1:-1];\n'
    layout+='typedef char texture_size[sizeof(Texture)==36?1:-1];\n'
    (OUT/'layout.c').write_text(layout);shell('gcc','-m32','-std=c89','-fsyntax-only',OUT/'layout.c')
    hostflags=['gcc','-shared','-fPIC','-O2','-std=c89','-fno-builtin','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-Wall','-Wextra','-Wno-unused-parameter']
    shell(*hostflags,HERE/'host.c','-o',OUT/'host.so')
    def load(path):
        lib=ctypes.CDLL(str(path));lib.run.argtypes=[ctypes.c_int]*9;lib.run.restype=ctypes.c_int
        return lib,(ctypes.c_uint32*(20*7)).in_dll(lib,'trace'),ctypes.c_int.in_dll(lib,'count')
    def run(lib,args):
        lib,trace,count=lib;assert lib.run(*args)==1
        return [list(trace[i*7:i*7+7]) for i in range(count.value)]
    lib=load(OUT/'host.so');covered=set();decisions=set();n=0;traces=hashlib.sha256();cases=inputs()
    for args in cases:
        actual=run(lib,args);expected,cov,dec,access=execute(native,*args,seed=n)
        assert actual==expected,(args,actual,expected)
        candidate_trace=execute(linked['text'][:428],*args,seed=n)[0]
        assert candidate_trace==expected,('compiled IDO candidate mismatch',args,candidate_trace,expected)
        covered|=cov;decisions|=dec;n+=1;traces.update(repr(expected).encode())
    assert len(covered)==105,(len(covered),sorted(set(range(0,428,4))-covered))
    assert set(range(0,428,4))-covered=={0xEC,0xF0}
    exhaustive=0
    for height in range(65536):
        args=(1,0,0,height&255,height,[-128,0,637][height%3],65535,0,0)
        assert run(lib,args)==fast_oracle(args),args
        exhaustive+=1
    result['behavior']=dict(native_vs_host_cases=n,native_vs_gnu_linked_ido_candidate_cases=n,exhaustive_unsigned_height_host_oracle_cases=exhaustive,all_tablecount_values=256,all_skip_byte_values=256,all_attainable_font_values=766,scheduled_hook_positions=16,nonmutating_font_event_positions=[5,8,11,14],effective_external_mutation_positions=12,mutation_variants=14,covered_words=len(covered),unreachable_word_offsets=[0xEC,0xF0],unreachable_reason='Unsigned halfword height cannot take signed divide correction arm.',branch_outcomes=len(decisions),trace_sha256=traces.hexdigest(),randomized_volatile_gprs_and_fprs=True,frame_and_saved_registers=True,maximum_count_tested=32767)
    negative={}
    for name in ('shifted_entry','shifted_text_section','shifted_function_symbol','wrong_function_extent','nonzero_alignment','trailing_nonzero_text','wrong_external_binding','extra_allocated_storage','unresolved_relocation','body_corruption'):
        corrupt=copy.deepcopy(linked)
        if name=='shifted_entry':corrupt['entry']+=4
        elif name=='shifted_text_section':corrupt['text_address']+=4
        elif name=='shifted_function_symbol':corrupt['symbols']['func_803A4340']['value']+=4
        elif name=='wrong_function_extent':corrupt['symbols']['func_803A4340']['size']-=4
        elif name=='nonzero_alignment':corrupt['text']=corrupt['text'][:-1]+b'\1'
        elif name=='trailing_nonzero_text':corrupt['text']+=b'\1\2\3\4'
        elif name=='wrong_external_binding':corrupt['symbols']['D_803B85E8']['value']+=4
        elif name=='extra_allocated_storage':corrupt['sections']['.unexpected']=(0,1,3,0,0,4,0,0,4,0)
        elif name=='unresolved_relocation':corrupt['relocs']=[{'type':6}]
        elif name=='body_corruption':corrupt['text']=bytes([corrupt['text'][0]^1])+corrupt['text'][1:]
        try:validate_linked(corrupt)
        except AssertionError:negative[name]='rejected'
        else:raise AssertionError(('accepted corrupt ELF view',name))
    result['negative_full_elf_views']=negative
    source_text=SOURCE.read_text();mutants={}
    mutants['three_rows']=source_text.replace('j < 4','j < 3')
    mutants['wrong_skip_value']=source_text.replace('D_803BA028[i] == 1','D_803BA028[i] == 0')
    mutants['wrong_mode_value']=source_text.replace('D_8014A110 != 2','D_8014A110 != 1')
    mutants['signed_texture_height']=source_text.replace('u16 height;','s16 height;')
    mutants['wrong_height_divisor']=source_text.replace('texture->height / 8','texture->height / 4')
    mutants['wrong_first_index']=source_text.replace('indices->first + j','indices->first + 1 + j')
    a,b=source_text.split('    render_helper(0.0f);')
    a=a.replace('    s32 i;','    s32 i;\n    s32 cached_count;')
    b=b.replace('    dispatch_handler(1);','    dispatch_handler(1);\n    cached_count = D_8014A108;')
    before,after=b.split('    cached_count = D_8014A108;')
    mutants['cached_count']=a+'    render_helper(0.0f);'+before+'    cached_count = D_8014A108;'+after.replace('D_8014A108','cached_count')
    controls={}
    mutant_cases=[(1,0,0,129,65535,-128,32768,0,0),(1,0,1,1,9,637,0,0,0),(1,0,0,1,128,0,1,6,0),(1,2,0,1,128,0,1,0,0)]
    for name,code in mutants.items():
        assert code!=source_text
        mutant=OUT/'mutant.c';mutant.write_text(code)
        shell(compiler,*compiler_args,'-o',OUT/'mutant.o',mutant)
        shell('mips-linux-gnu-ld','-EB','-T',OUT/'whole.ld','-o',OUT/'mutant.elf',OUT/'mutant.o')
        mutant_link=elf(OUT/'mutant.elf');assert mutant_link['text'][:428]!=native
        path=OUT/(name+'.so');shell(*hostflags,'-DREVIEW_SOURCE="'+str(mutant)+'"',HERE/'host.c','-o',path)
        mlib=load(path);witness=None
        for args in mutant_cases:
            expected=execute(native,*args)[0];actual=run(mlib,args)
            if actual!=expected:witness=args;break
        assert witness is not None,name
        controls[name]={'strict_native_equality':False,'behavior_rejected':True,'witness':witness,'source_sha256':sha(code.encode())}
    result['wrong_source_controls']=controls
    result['domain_and_limits']=['Research NONMATCH: complete 428-byte body differs at four private compiler-temporary stack slot operands; no match/promotion/coverage credit.','O32 ABI and partial field views verified, not original source or complete game structs.','Lookup succeeds and returns valid 36-byte nonaliasing texture; flags capacity covers all visited outer indices; valid text pointer table extends through first+3, including first=65535.','All 65536 unsigned texture heights tested against separate oracle. Signed-halfword coordinate conversion uses verified IDO/GCC low-16/sign-extension implementation, not universal ISO C conversion behavior.','Font helper returns within [-128,637]; it performs font synchronization/cache writes through native sound_update_channel. Its transitive graph must not alias menu/count/texture objects; font hook does not arbitrarily mutate those globals.','Other external helper hooks allow bounded menu/count/texture mutations; actual rendering, texture search, font synchronization and gameplay are not executed.','Complete native caller execution uses an independent bounded instruction interpreter with randomized caller-save registers and FPRs; no linked native callee integration or hardware claim.','No protected-input modification, publication, CI watch, image/compression/full-ROM gate or acceptance claim.']
    assert sha(SOURCE.read_bytes())==source_sha
    result['reviewer_script_sha256']=sha((HERE/'verify.py').read_bytes());result['reviewer_host_sha256']=sha((HERE/'host.c').read_bytes())
    result=json.loads(json.dumps(result))
    if options.check: assert portable_receipt(result)==portable_receipt(json.loads(options.out.read_text())),'Independent frozen receipt differs'
    else: options.out.write_text(json.dumps(result,indent=2)+'\n')
    (OUT/'local_provenance.json').write_text(json.dumps({'object_sha256':sha((OUT/'candidate.o').read_bytes()),'elf_sha256':sha((OUT/'whole.elf').read_bytes()),'gcc':shell('gcc','--version').splitlines()[0],'gnu_ld':shell('mips-linux-gnu-ld','--version').splitlines()[0]},indent=2)+'\n')
    print(json.dumps({k:result[k] for k in ('status','source_sha256','native_sha256','residual','behavior','wrong_source_controls')},indent=2))

if __name__=='__main__':main()
