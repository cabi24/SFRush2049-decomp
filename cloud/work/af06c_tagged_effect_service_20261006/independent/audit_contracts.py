#!/usr/bin/env python3
"""Compiler-free metadata audit. Read immutable base; never emit native bytes."""
import argparse,hashlib,json,re,struct,subprocess,zlib
from pathlib import Path
BASE='6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
EXPECTED={
 'save_write_data':(0x800AF06C,1200,'3d7f79a62ea3664248e76b8cd9fc06dbd8142ab8d31e6167e8c1ca246323fca1'),
 'func_80090308':(0x80090308,1128,'dd5fb2b4ba651dfc726d27571fc810534f081935e00ed42fdeb70f5456d87169'),
 'func_80090284':(0x80090284,132,'30a84341f5dadb3b164c8e49d76c350693b8369e33e6f62663cf9d2c8fe807eb'),
 'math_utility':(0x8008D6B0,76,'e2b566e9e8acf312ddb9d228ba6cfb6fdbc517c14b00cbd5386e37ec51fc8159'),
 'func_8008E26C':(0x8008E26C,300,'54c0fe9d2ad5160f5dca1268f7819ecec5ab1c447f7cf0f0a14ab0a57c78fc90'),
 'entity_spawn_callback':(0x80090088,416,'0a341263df072121b6344998cef815065803fdd5ba4974c9520a3ecb60c8cdbe'),
 'camera_target_track':(0x800AED64,636,'a057021cac52f5037d124838784fd62d4b2bf90a961c66b6a6b83344cb7da032'),
 'entity_spawn_init':(0x8008EA10,5544,'21476242fe1fd3da148631a748f35db1ee8b898c49a031b674b4f719b0e91f8a'),
 'entity_physics_update':(0x800908A0,712,'1b79380ce3f4ffed899490871ba429397a8051c8786fe77c33b0e7fcfec36fa2'),
 'entity_collision_detect':(0x80090B68,820,'505b41e3e8fb42e0d8f374117d5bd5e9acd19eaeb708a29363edb12512d56e6e'),
 'entity_update_callback':(0x80090FEC,2184,'e66d88a28474d069e371d71305ed50cf4a79f264af208b4b69503fee95253b31'),
}
def load(repo):
 def read(p):return subprocess.check_output(['git','-C',str(repo),'show',BASE+':'+p])
 symbols=json.loads(read('asm/us/blob/symbols.json'))['symbols']
 image=zlib.decompress(read('assets/us/data.bin')[0xB0CB10-0x283D0:],-15)
 metadata={};bodies={};code={}
 for path in ['asm/us/blob/blob_8008d0c0.s','asm/us/blob/blob_800aeb54.s']:
  for section in read(path).decode().split('.section .text.')[1:]:
   name=section.split(',')[0]
   if name not in EXPECTED:continue
   words=[int(w,16) for w in re.findall(r'\.word 0x([0-9A-Fa-f]{8})',section)]
   address,size,digest=EXPECTED[name]
   raw=struct.pack('>'+str(len(words))+'I',*words)
   assert int(symbols[name],16)==address and len(raw)==size
   assert hashlib.sha256(raw).hexdigest()==digest
   assert raw==image[address-0x80086A50:address-0x80086A50+size]
   bodies[name]=words;code.update((address+4*i,w) for i,w in enumerate(words))
   metadata[name]=dict(address=hex(address),bytes=size,sha256=digest)
 assert set(bodies)==set(EXPECTED)
 # Prove exact bounds and no indirect invocations in root/private child.
 for name in ['save_write_data','func_80090308']:
  for word in bodies[name]:
   if word>>26==0 and word&63 in (8,9):
    assert word&63==8 and (word>>21)&31==31
 # Private input is caller outgoing word 0, child's big-endian low half +2.
 root,private=bodies['save_write_data'],bodies['func_80090308']
 assert root[(0x800AF0EC-0x800AF06C)//4]>>26==43
 w=private[(0x80090348-0x80090308)//4]
 assert w>>26==33 and (w>>21)&31==29 and w&65535==274
 # Every stored callback explicitly consumes low signed half of a1.
 for name in ['entity_collision_detect','entity_physics_update','entity_update_callback']:
  assert any(w>>26==0 and (w>>16)&31==5 and (w>>6)&31==16 and w&63==0 for w in bodies[name][:12])
 # Native copy contains exactly 9 float loads and stores, with no arithmetic.
 words=bodies['math_utility']
 assert sum(w>>26==49 for w in words)==sum(w>>26==57 for w in words)==9
 assert all(w>>26 in (0,49,57) for w in words)
 return code,image,metadata

def main():
 p=argparse.ArgumentParser();p.add_argument('--reference-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 _,_,metadata=load(a.reference_root)
 result=dict(status='PASS: immutable native identities and selected structural contracts',base=BASE,native=metadata,target_compilations=0,own_verifier_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest())
 a.output.write_text(json.dumps(result,indent=2)+'\n');print(result['status'])
if __name__=='__main__':main()
