"""Read-only authenticated inputs and linked-ELF adapter; never invokes a compiler."""
import hashlib
import importlib.util
from pathlib import Path
import struct
import subprocess
import sys
import zlib
from closure import MEMBERS, ENTRY

BASE='6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
def sha(raw):return hashlib.sha256(raw).hexdigest()

def load_score(reference):
    reference=Path(reference).resolve();sys.path.insert(0,str(reference/'tools/cloud'))
    spec=importlib.util.spec_from_file_location('genuine_closure_score',reference/'tools/cloud/score.py')
    score=importlib.util.module_from_spec(spec);sys.modules[spec.name]=score;spec.loader.exec_module(score)
    return score

def native(reference,score=None):
    reference=Path(reference).resolve();score=score or load_score(reference)
    raw=subprocess.check_output(['git','-C',str(reference),'show',BASE+':assets/us/data.bin'])
    assert sha(raw)=='f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
    dec=zlib.decompressobj(-15);image=dec.decompress(raw[0xB6FEC4-0x283D0:])
    assert dec.eof and len(image)==43888
    assert sha(image)=='b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd'
    score.ASM_DIR=reference/'asm/us/ovl_b';targets=score.targets();code={};identities={}
    for suffix,size in MEMBERS.items():
        name='func_8038'+suffix;address=0x80380000+int(suffix,16)
        words=targets[name];actual=struct.pack('>'+str(len(words))+'I',*words)
        assert len(actual)==size and actual==image[address-0x8038a400:address-0x8038a400+size]
        code.update({address+4*i:w for i,w in enumerate(words)})
        identities[name]={'address':hex(address),'size':size,'sha256':sha(actual)}
    # Full authenticated image is readonly input memory, not executable code.
    # Main-image vehicle bounds are explicit fixtures, not claimed native data.
    bounds=b''.join(struct.pack('>4f',1.+i*0.125,0.75+i*0.25,2.,1.5) for i in range(13))
    data=[(0x8038a400,image),(0x8011f844,bounds)]
    return code,ENTRY,data,identities

def linked(score,path):
    raw,sections=score._elf(Path(path));header=struct.unpack_from('>16sHHIIIIIHHHHHH',raw)
    assert header[1]==2 and header[2]==8, 'expected fully linked big-endian MIPS ELF'
    symbols={s['name']:s for i,sec in enumerate(sections) if sec['type']==2
             for s in score._symbol_table(raw,sections,i) if s['name']}
    assert not [n for n,s in symbols.items() if s['section']==0], 'unresolved ELF symbols'
    functions={n:s for n,s in symbols.items() if s['type']==2 and s['section'] not in (0,0xfff1)}
    assert 'func_8038FCE0' in functions
    code={};data=[];meta={}
    for i,sec in enumerate(sections):
        h=struct.unpack_from('>IIIIIIIIII',raw,header[6]+i*header[11])
        if sec['type'] in (4,9):assert sec['size']==0, 'unresolved relocations'
        if not(h[2]&2 and h[5]):continue
        content=raw[h[4]:h[4]+h[5]]
        if h[2]&4:
            assert len(content)%4==0
            code.update({h[3]+4*j:w for j,(w,) in enumerate(struct.iter_unpack('>I',content))})
            for name,s in functions.items():
                if s['section']!=i:continue
                addr,size=s['value'],s['size'];assert size%4==0
                off=addr-h[3];assert 0<=off and off+size<=len(content)
                code.update({addr+j:w for j,(w,) in zip(range(0,size,4),struct.iter_unpack('>I',content[off:off+size]))})
                meta[name]={'address':hex(addr),'size':size}
        else:
            assert not(h[2]&1), ('writable candidate-owned data unsupported',sec['name'])
            data.append((h[3],content))
    return code,symbols['func_8038FCE0']['value'],data,meta
