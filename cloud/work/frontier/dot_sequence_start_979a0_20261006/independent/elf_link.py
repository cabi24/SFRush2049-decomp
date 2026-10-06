"""Independent manifest/ELF parsing and unmasked GNU relocation replay.

Temporary GNU objects contain unchanged complete ELF function bytes. For a
section-symbol call, an absolute alias supplies the native callee address less
the encoded object offset. GNU ld performs every actual relocation; no
canonical scorer relocation or mask is used by this proof.
"""
import hashlib,json,re,struct,subprocess
from pathlib import Path

def sha(data):return hashlib.sha256(data).hexdigest()
def native(root):
    directory=root/'asm/us/blob';entries={}
    for line in (directory/'SHA256SUMS').read_text().splitlines():
        m=re.fullmatch(r'([0-9a-f]{64})  ([A-Za-z0-9_.-]+)',line)
        assert m and m[2] not in entries;entries[m[2]]=m[1]
    assert {p.name for p in directory.glob('*.s')}=={n for n in entries if n.endswith('.s')}
    bodies={}
    for name,digest in entries.items():
        data=(directory/name).read_bytes();assert sha(data)==digest,(name,'manifest')
        if name=='symbols.json':addresses={n:int(a,16) for n,a in json.loads(data)['symbols'].items()}
        if not name.endswith('.s'):continue
        current=None
        for line in data.decode().splitlines():
            m=re.fullmatch(r'\.section \.text\.([^,]+),.*',line)
            if m:current=m[1];assert current not in bodies;bodies[current]=[]
            m=re.fullmatch(r'\s*\.word (0x[0-9a-fA-F]+)',line)
            if m:assert current;bodies[current].append(int(m[1],16))
    return bodies,addresses

class Elf:
    def __init__(self,path):
        self.raw=Path(path).read_bytes();d=self.raw
        assert d[:6]==b'\x7fELF\x01\x02' and struct.unpack_from('>H',d,18)[0]==8
        offset=struct.unpack_from('>I',d,32)[0];size,count,names=struct.unpack_from('>HHH',d,46)
        assert size==40
        self.sections=[dict(zip(('name_offset','type','flags','address','offset','size','link','info','align','entry'),struct.unpack_from('>10I',d,offset+i*size))) for i in range(count)]
        table=self.data(self.sections[names])
        for s in self.sections:s['name']=table[s['name_offset']:].split(b'\0',1)[0].decode()
        self.text=next(s for s in self.sections if s['name']=='.text');self.text_index=self.sections.index(self.text)
        self.symbols=[]
        for s in self.sections:
            if s['type']!=2:continue
            strings=self.data(self.sections[s['link']]);self.sym_section=s
            assert s['entry']==16
            for off in range(s['offset'],s['offset']+s['size'],16):
                no,val,sz,info,other,sec=struct.unpack_from('>IIIBBH',d,off)
                self.symbols.append({'name':strings[no:].split(b'\0',1)[0].decode(),'value':val,'size':sz,'type':info&15,'section':sec})
        self.relocations=[]
        for s in self.sections:
            if s['type']!=9 or s['info']!=self.text_index:continue
            assert self.sections[s['link']]==self.sym_section and s['entry']==8
            for off in range(s['offset'],s['offset']+s['size'],8):
                at,info=struct.unpack_from('>II',d,off);self.relocations.append((at,info&255,self.symbols[info>>8]))
    def data(self,section):return self.raw[section['offset']:section['offset']+section['size']]
    def function(self,name):
        matches=[s for s in self.symbols if s['name']==name and s['type']==2 and s['section']==self.text_index]
        assert len(matches)==1;return matches[0]
    def body(self,name):
        f=self.function(name);begin=f['value']-self.text['address'];return self.data(self.text)[begin:begin+f['size']]

def link_function(obj,name,addresses,expected,work):
    work.mkdir();elf=Elf(obj);fn=elf.function(name);start,size=fn['value'],fn['size'];raw=elf.body(name)
    assert size==len(raw)==len(expected)*4,(name,'complete extent',size,len(expected)*4)
    binary=work/'body.bin';binary.write_bytes(raw)
    asm=['.section .text,"ax",@progbits','.balign 4','.globl proof_body','.type proof_body,@function','proof_body:',f'.incbin "{binary}"',f'.size proof_body,{size}']
    assignments=[];records=[];aliases={}
    for off,kind,sym in elf.relocations:
        if not start<=off<start+size:continue
        site=off-start;addend=0
        assert kind in (4,5,6),(name,'unsupported relocation',kind)
        if sym['type']==3:
            assert kind==4 and sym['section']==elf.text_index,(name,'unsupported section reference',sym)
            word=struct.unpack_from('>I',raw,site)[0];addend=(word&0x3ffffff)*4
            callees=[s for s in elf.symbols if s['type']==2 and s['section']==elf.text_index and s['value']==addend]
            assert len(callees)==1 and callees[0]['name'] in addresses
            resolved=addresses[callees[0]['name']]-addend;target=callees[0]['name']
        else:
            assert sym['name'] in addresses,(name,'unknown symbol',sym['name'])
            assert sym['section'] in (0,elf.text_index)
            resolved=addresses[sym['name']];target=sym['name']
        key=(target,resolved)
        if key not in aliases:aliases[key]='ref%d'%len(aliases)
        alias=aliases[key]
        asm.append('.reloc proof_body+%d,%s,%s'%(site,{4:'R_MIPS_26',5:'R_MIPS_HI16',6:'R_MIPS_LO16'}[kind],alias))
        assignment='%s = 0x%X;'%(alias,resolved)
        if assignment not in assignments:assignments.append(assignment)
        records.append({'offset':site,'type':kind,'symbol':target,'object_section_addend':addend})
    source=work/'body.s';source.write_text('\n'.join(asm)+'\n');out=work/'body.o'
    assembled=subprocess.run(['mips-linux-gnu-as','-EB','-mips2','-o',str(out),str(source)],check=True,capture_output=True)
    assert not assembled.stderr,assembled.stderr.decode()
    script=work/'link.ld';script.write_text('ENTRY(proof_body)\nSECTIONS { .text 0x%X : SUBALIGN(4) { *(.text) } }\n'%addresses[name]+'\n'.join(assignments))
    linked=work/'linked.elf';linked_run=subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(linked),str(out)],check=True,capture_output=True)
    assert not linked_run.stderr,linked_run.stderr.decode()
    linked_elf=Elf(linked);result=linked_elf.body('proof_body');assert linked_elf.function('proof_body')['value']==addresses[name]
    assert result==struct.pack('>%dI'%len(expected),*expected),(name,'GNU linked body differs')
    return list(struct.unpack('>%dI'%len(expected),result)),{'bytes':size,'words':len(expected),'body_sha256':sha(result),'relocations':records,'complete_gnu_equal':True,'owned_data_references':0}
