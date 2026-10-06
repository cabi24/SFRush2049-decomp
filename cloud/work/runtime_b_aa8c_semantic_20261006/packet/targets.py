"""Native target authentication; production context comes from a recorded commit."""
import hashlib, importlib.util, struct, subprocess, sys, zlib
from pathlib import Path
BASE = '6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
EXPECTED = {
'func_8038A95C':(0x8038A95C,184,'f6f3b782e02e565f5bf723f54839b9c7f80483f410028f6a9db7e595b580664e'),
'func_8038AA14':(0x8038AA14,120,'9f23d5a9e52f138ede5552e5f931250ec7df73aa654150fd157966e6ca83808d'),
'func_8038AA8C':(0x8038AA8C,7812,'c8842f0b25b7c17e36345611643e4b67a7c0c17929f7be6ba93785b530b67b39')}
IMAGE_HASH = 'b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd'
def git_read(root,path): return subprocess.check_output(['git','-C',str(root),'show',BASE+':'+path])
def load(root):
    # Support canonical score.py sibling imports from any working directory.
    sys.path.insert(0, str(root / 'tools/cloud'))
    spec=importlib.util.spec_from_file_location('full_aa8c_score',root/'tools/cloud/score.py')
    score=importlib.util.module_from_spec(spec);sys.modules[spec.name]=score;spec.loader.exec_module(score)
    score.ASM_DIR=root/'asm/us/ovl_b';targets=score.targets()
    inflater=zlib.decompressobj(-15)
    image=inflater.decompress(git_read(root,'assets/us/data.bin')[0xB6FEC4-0x283D0:])
    assert inflater.eof and len(image)==43888 and hashlib.sha256(image).hexdigest()==IMAGE_HASH
    code={}
    for name,(address,size,sha) in EXPECTED.items():
        words=targets[name];raw=struct.pack('>'+'I'*len(words),*words)
        assert len(raw)==size and hashlib.sha256(raw).hexdigest()==sha
        assert raw==image[address-0x8038A400:address-0x8038A400+size]
        code.update({address+4*i:w for i,w in enumerate(words)})
    return code,image
