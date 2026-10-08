"""Read historical context and authenticate complete targets, without a compiler."""
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
import zlib

BASE = '6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
TARGETS = {'save_write_data': (0x800AF06C, 1200),
           'func_80090308': (0x80090308, 1128)}
EXPECTED = {'save_write_data': '3d7f79a62ea3664248e76b8cd9fc06dbd8142ab8d31e6167e8c1ca246323fca1'}
IMAGE_BASE = 0x80086A50

def sha(raw): return hashlib.sha256(raw).hexdigest()
def read(root,path):
    return subprocess.check_output(['git','-C',str(root),'show',BASE+':'+path])

def load(root):
    asset = read(root,'assets/us/data.bin')
    dec = zlib.decompressobj(-15)
    image = dec.decompress(asset[0xB0CB10-0x283D0:])
    assert dec.eof and len(image) == 647072
    assert sha(image) == 'bf7da3fa6283428a97372250cd4076d15e9eae10f9d5709c0387fe0742d43a1d'
    paths = subprocess.check_output(['git','-C',str(root),'ls-tree','-r','--name-only',BASE,'asm/us/blob']).decode().splitlines()
    symbols = json.loads(read(root,'asm/us/blob/symbols.json'))['symbols']
    code, identities = {}, {}
    for path in paths:
        if not path.endswith('.s'): continue
        for section in read(root,path).decode().split('.section .text.')[1:]:
            name = section.split(',')[0]
            if name not in TARGETS: continue
            address,size = TARGETS[name]
            words = [int(w,16) for w in re.findall(r'\.word 0x([0-9a-fA-F]{8})',section)]
            raw = struct.pack('>'+str(len(words))+'I',*words)
            assert address == int(symbols[name],16) and len(raw) == size
            assert raw == image[address-IMAGE_BASE:address-IMAGE_BASE+size]
            if name in EXPECTED: assert sha(raw) == EXPECTED[name]
            code.update({address+4*i:w for i,w in enumerate(words)})
            identities[name] = {'address':hex(address),'size':size,'sha256':sha(raw)}
    assert set(identities) == set(TARGETS)
    return code,image,identities
