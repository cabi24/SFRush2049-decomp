from pathlib import Path
import os,sys,json,hashlib,struct
from dataclasses import asdict
repo=Path(__file__).resolve().parents[3]
sys.path[:0]=[str(repo),str(repo/'tools/conveyor/jobs')]
from tools.conveyor.pipeline import targets
p=repo/'build/C98';target=p/'target.o';candidate=p/'candidate.o'
# Byte proof uses the unchanged target .text and actual original ROM interval.
import subprocess
out=p/'candidate.text';subprocess.run(['mips-linux-gnu-objcopy','-O','binary','--only-section=.text',str(candidate),str(out)],check=True)
text=out.read_bytes();rom=(repo/'baserom.us.z64').read_bytes();original=rom[0x10970:0x10970+48]
words=[f'{w:08X}' for w in struct.unpack('>12I',original)]
proof={'source_sha256':hashlib.sha256((repo/'cloud/work/static_C98/__osPiDeviceBusy.c').read_bytes()).hexdigest(),'object_sha256':hashlib.sha256(candidate.read_bytes()).hexdigest(),'target_sha256':hashlib.sha256(target.read_bytes()).hexdigest(),'original_rom_sha1':hashlib.sha1(rom).hexdigest(),'target_roundtrip_gate':targets.gate_target(words,target),'native_text_size':len(text),'native_text_sha256':hashlib.sha256(text).hexdigest(),'original_text_sha256':hashlib.sha256(original).hexdigest(),'all48_bytes_exact':text==original,'genuine_instruction_bytes':36,'zero_alignment_tail_bytes':12,'actual_owner':'existing data.bin container (static layout ends ROM0x10000)','registered_slot':False,'accepted_credit':0}
assert len(text)==48 and text==original
(repo/'cloud/work/static_C98/proof.json').write_text(json.dumps(proof,indent=2)+'\n');print(json.dumps(proof,indent=2))
