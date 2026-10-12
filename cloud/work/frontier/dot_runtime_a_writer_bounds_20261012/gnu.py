"""Independent GNU assembly/link relocation proof of all complete group members."""
import hashlib
import struct
import subprocess


def require(ok,why):
    if not ok:raise ValueError(why)


def check(obj,work,image,base,sizes,score,owndata,expected_differences=None):
    elf=owndata._Object(obj)
    ti=next(i for i,s in enumerate(elf.sections) if s['name']=='.text')
    ri=next(i for i,s in enumerate(elf.sections) if s['name']=='.rodata')
    syms=[s for i,sec in enumerate(elf.sections) if sec['type']==2 for s in elf.symbols(i)]
    funcs={s['name']:s for s in syms if s['type']==2 and s['section']==ti}
    text=elf.raw(ti);raw=elf.raw(ri)
    require(len(raw)==32 and not any(raw[20:]),'unexpected data beyond five-entry table')
    kinds={2:'R_MIPS_32',4:'R_MIPS_26',5:'R_MIPS_HI16',6:'R_MIPS_LO16'}
    lines=['.set noreorder','.set noat'];externs=set();counts={}
    def owner(address):
        matches=[f for f in funcs.values() if f['value']<=address<f['value']+f['size']]
        require(len(matches)==1,'section address has no unique owner')
        require(matches[0]['name'] in sizes,'unclaimed local text target')
        return matches[0]
    for name,size in sizes.items():
        fn=funcs[name];start=fn['value']
        require(fn['size']==size,'GNU input function extent changed')
        following=min((s['value'] for s in funcs.values() if s['value']>start),default=len(text))
        require(following>=start+size and not any(text[start+size:following]),'GNU extra code after function')
        words=list(struct.unpack('>%dI'%(size//4),text[start:start+size]));rels=[]
        for off,kind,sym in elf.relocations(ti):
            if not start<=off<start+size:continue
            off-=start;symbol=sym['name']
            require(kind in (4,5,6) and off%4==0,'unsupported text relocation')
            if sym['type']==3 and sym['section']==ti:
                require(kind==4,'non-call local-text reference')
                value=(words[off//4]&0x3ffffff)*4+sym['value'];f=owner(value);symbol=f['name']
                words[off//4]=(words[off//4]&0xfc000000)|((value-f['value'])//4)
            elif sym['section']==ri:
                require(sym['type']==3 and kind in (5,6),'unexpected owned-data reference')
                symbol='owned_table'
                if kind==6:require(0<=(words[off//4]&65535)<20,'owned-data reference leaves table')
            require(symbol and not symbol.startswith('.'),'unhandled section reference')
            rels.append((off,kind,symbol));externs.add(symbol)
        lines+=['.section .proof_'+name+',"ax",@progbits','.balign 4','.globl '+name,'.type '+name+',@function',name+':']
        lines+=['.word 0x%08x'%w for w in words]+['.size %s,.-%s'%(name,name)]
        lines+=['.reloc %s+%d,%s,%s'%(name,o,kinds[k],s) for o,k,s in rels]
        counts[name]=len(rels)
    data_rels=elf.relocations(ri)
    require([r[0] for r in data_rels]==[0,4,8,12,16],'table relocation coverage differs')
    lines+=['.section .proof_table,"a",@progbits','.balign 4','.globl owned_table','owned_table:']
    refs=[]
    for off,kind,sym in data_rels:
        require(kind==2 and sym['section']==ti,'table entry does not point to local text')
        value=struct.unpack_from('>I',raw,off)[0]+sym['value'];f=owner(value)
        require(f['name']=='func_803A7918','table jumps outside real root')
        lines+=['.word %d'%(value-f['value'])];refs.append((off,f['name']))
    lines+=['.reloc owned_table+%d,R_MIPS_32,%s'%r for r in refs]
    src,reloc,linked,script=[work/('independent.'+suffix) for suffix in ('s','o','elf','ld')]
    src.write_text('\n'.join(lines)+'\n')
    symbol_map=score.image_symbols();assign=[]
    for name in sorted(externs-set(sizes)-{'owned_table'}):
        addr=symbol_map.get(name,score.address_named(name));require(addr is not None,'unknown external '+name)
        assign.append('%s = 0x%X;'%(name,addr))
    script.write_text('SECTIONS {\n'+''.join('.proof_%s 0x%s : SUBALIGN(4) { *(.proof_%s) }\n'%(n,n[-8:],n) for n in sizes)+
        '.proof_table 0x803B9834 : SUBALIGN(4) { *(.proof_table) }\n'+
        '/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n'+'\n'.join(assign)+'\n')
    for cmd in [('mips-linux-gnu-as','-EB','-mips2','-o',reloc,src),('mips-linux-gnu-ld','-EB','-T',script,'-o',linked,reloc)]:
        r=subprocess.run(list(map(str,cmd)),text=True,capture_output=True);require(r.returncode==0,r.stdout+r.stderr)
    result=owndata._Object(linked);require(not any(s['type'] in (4,9) for s in result.sections),'GNU left unresolved relocations')
    offset=struct.unpack_from('>I',result.data,0x20)[0];stride=struct.unpack_from('>H',result.data,0x2E)[0]
    def address(i):return struct.unpack_from('>I',result.data,offset+stride*i+12)[0]
    resolved_syms=[s for i,sec in enumerate(result.sections) if sec['type']==2 for s in result.symbols(i)]
    out={}
    for name,size in sizes.items():
        i=next(i for i,s in enumerate(result.sections) if s['name']=='.proof_'+name)
        native=int(name[-8:],16);body=result.raw(i)
        require(len(body)==size and address(i)==native,'GNU final extent/address mismatch')
        symbol=next(s for s in resolved_syms if s['name']==name)
        require((symbol['value'],symbol['size'],symbol['type'],symbol['section'])==(native,size,2,i),'GNU STT_FUNC metadata mismatch')
        expected=struct.pack('>%dI'%(size//4),*score.targets()[name])
        differences=[k*4 for k,(a,b) in enumerate(zip(struct.unpack('>%dI'%(size//4),body),score.targets()[name])) if a!=b]
        require(differences==(expected_differences or {}).get(name,[]),'GNU complete member mismatch')
        out[name]={'bytes':size,'relocations':counts[name],'difference_offsets':differences,'sha256':hashlib.sha256(body).hexdigest()}
    i=next(i for i,s in enumerate(result.sections) if s['name']=='.proof_table')
    table=result.raw(i);require(address(i)==0x803B9834 and len(table)==20 and table==image[0x803B9834-base:0x803B9848-base],'GNU complete owned table mismatch')
    out['owned_table']={'address':'0x803b9834','bytes':20,'relocations':5,'sha256':hashlib.sha256(table).hexdigest()}
    return out
