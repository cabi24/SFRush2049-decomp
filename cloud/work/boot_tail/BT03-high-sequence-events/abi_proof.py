"""Native-only whole-object and field-offset assertions for the dispatcher."""
import argparse,hashlib,json,struct,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[4];WORK=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT))
from tools.cloud import score
SOURCE=WORK/'nonmatch/func_80017D38.c'
SIZES={'SequenceTime':8,'SequenceNote':24,'SequenceRequest':40,'SequenceTrack':40,'SequenceContext':4088}
OFFSETS={'SequenceNote':{'identifier':8,'due':12,'time':16},'SequenceTrack':{'time':0,'event_time':8,'events':12,'pitch':16,'modulation':20,'pitch_value':24,'modulation_value':26,'pitch_time':28,'modulation_time':32,'channel':36,'velocity_offset':37,'transpose':38,'number':39},'SequenceContext':{'enabled':272,'lookahead':288,'groups':1320,'tracks':1384,'programs':3968,'request':4040,'result':4080,'pending':4084}}
NATIVE_INPUTS={'80017D38': (872, '4fbd944f2a61b52f46402c29fbcf4dd910a4619268cb7c7ac2c6b2cebaa86c38'), '800173B4': (92, '016d07723d7ff4deb8867ea3a07e783a76e590ca71e9701feebd6ee88b4d8005'), '80017410': (96, '060d55cba3dd2795d3504f5ccb92e240bad55854323128a22afda2afddf67ab8'), '80017720': (204, '60a14c88647c100d6d9f52adb4e0217171e0f35130e1287ac8c8cc1464b59395'), '800177EC': (56, '234248199f3d9099e27f51ac51f7e9457b4b168673c40e78da5ff5c5dacda939'), '80019490': (960, '95eecb9ffd681b61e1b770365a28ae5c52a85092c92f1602c29d82099e2b6daf'), '8001A270': (872, '75c4ce68d65d2b4f5e79773ce033699acbd9685ab10628eac8c442d3268f5bfa'), '80020610': (156, '9037f618c9672f865a80019c52075512f43091b7514049629c350a29646d1df3'), '800198C8': (300, '95806b1f449202b49b273592c459170b33b182521d8555343b9af2263c0299ae'), '800178B0': (1160, 'e2d3b7c8631d3ff01d9ec07b44abeab7d197f2eb0e93c9a68d6a9d1abcaf4b3a')}
def run():
 assert hashlib.sha256(SOURCE.read_bytes()).hexdigest()=='ad5137579da173b15c903c8aa35c69cd26416d51def3b65267045062543bff65'
 score.ASM_DIR=ROOT/'asm/us/boot_tail';targets=score.targets();native={}
 for address,(size,digest) in NATIVE_INPUTS.items():
  words=targets['func_'+address];data=struct.pack('>'+str(len(words))+'I',*words)
  assert len(data)==size and hashlib.sha256(data).hexdigest()==digest,address
  native['func_'+address]=dict(bytes=size,sha256=digest)
 terms=['sizeof('+k+')=='+str(v) for k,v in SIZES.items()]+['((unsigned int)&(('+t+'*)0)->'+f+')=='+str(n) for t,fs in OFFSETS.items() for f,n in fs.items()]+['sizeof(void*)==4','sizeof(unsigned int)==4','sizeof(unsigned short)==2']
 with tempfile.TemporaryDirectory() as tmp:
  p=Path(tmp)/'probe.c';p.write_text('#include "'+str(SOURCE)+'"\ntypedef char native_layout[('+' && '.join(terms)+')?1:-1];\n');score.compile_single(p,score.DEFAULT_FLAGS,Path(tmp)/'probe.o')
 return dict(result='PASS',source_path=str(SOURCE.relative_to(ROOT)),source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),native_sizes=SIZES,native_offsets=OFFSETS,native_inputs_identical_to_reviewed_targets=native,qualifications=['Host LP64 pointer-bearing layouts differ; these assertions are compiled with pinned O32 IDO.','Aligned event storage consists of actual unsigned-halfword objects accessed as bytes for key/velocity; native byte order is big-endian.','D_8004BE7B is the big-endian low byte at D_8004BE78+3. Host fixture globals are initialized and changed consistently, without claiming native global alias placement.','The ten-input constructor and three-input embedded request helper are declarations only; no dependency source is reconstructed here.'])
if __name__=='__main__':
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--check',action='store_true');args=parser.parse_args();result=run()
 if args.check:
  assert result==json.loads((WORK/'abi_proof.json').read_text()),'ABI receipt drift'
  print('PASS: recovered source native sizes and offsets reproduced.')
 else:print(json.dumps(result,indent=2))
