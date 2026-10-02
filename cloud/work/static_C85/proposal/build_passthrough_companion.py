"""Privately build the genuine eight-C/one-asm transition with its real table.

Compiler input/objects remain ignored; only symbolic companion/evidence is saved.
"""
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path[:0] = [str(ROOT), str(ROOT / 'tools/asm-processor')]
import asm_processor

work = ROOT / 'build/C85/proposal_module'
work.mkdir(exist_ok=True)
retail = (ROOT / 'asm/us/34A0.s').read_text()
block = re.search(r'^nonmatching fcvt,.*?(?=^nonmatching __ecvt_internal,)', retail, re.M | re.S).group()
(work / 'fcvt.s').write_text(block)
entries = struct.unpack('>89I', (ROOT / 'baserom.us.z64').read_bytes()[0x2e158:0x2e158+356])
table = '.section .rodata\n.balign 4\nglabel jtbl_8002D558_main\n' + ''.join(
    '.word fcvt + 0x%x\n' % (address - 0x80002cd0) for address in entries)
assert table == (HERE / 'fcvt.table.proposed.s').read_text()
(work / 'fcvt.table.s').write_text(table)
frozen = ROOT / 'cloud/work/static_C81/lib_34a0.bsd.proposed.c'
assert hashlib.sha256(frozen.read_bytes()).hexdigest() == '3278ec22c194616962c2e5ba6414da422336cced7ee5cd2f2f265dbb7916bd11'
source = frozen.read_text().replace('build/C81/lib_34a0/fcvt.s', 'build/C85/proposal_module/fcvt.s')
source = source.replace('#pragma GLOBAL_ASM("build/C85/proposal_module/fcvt.s")',
                        '#pragma GLOBAL_ASM("build/C85/proposal_module/fcvt.s")\n'
                        '#pragma GLOBAL_ASM("build/C85/proposal_module/fcvt.table.s")')
src = work / 'lib_34a0.c'
src.write_text(source)
preprocessed = work / 'preprocessed.c'
with preprocessed.open('wb') as output:
    functions, _ = asm_processor.run(['-g', str(src)], outfile=output)
remote = 'agents/C/scratch/static-C67/module/'
subprocess.run(['scp', str(preprocessed), 'Rocky:' + remote + 'lib_34a0_C85_passthrough.c'], check=True, capture_output=True)
flags = '-g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm -Iinclude -Iinclude/PR -Irom -D_LANGUAGE_C'
command = ('cd ~/' + remote + ' && '
           '~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido/cc -c '
           + flags + ' lib_34a0_C85_passthrough.c -o lib_34a0_C85_passthrough.o')
cp = subprocess.run(['ssh', 'Rocky', command], capture_output=True, text=True)
if cp.returncode:
    raise RuntimeError(cp.stderr)
obj = work / 'lib_34a0.o'
subprocess.run(['scp', 'Rocky:' + remote + 'lib_34a0_C85_passthrough.o', str(obj)], check=True, capture_output=True)
asm_processor.run(['-g', str(src), '--post-process', str(obj), '--assembler',
                   'mips-linux-gnu-as -march=vr4300 -mabi=32 -Iinclude', '--asm-prelude',
                   str(ROOT / 'tools/asm-processor/prelude.inc')], functions=functions)
proof = {'private_transition_only': True, 'native_functions': 8, 'fcvt_body': 'original asm passthrough',
         'compile_exit': cp.returncode, 'stderr': cp.stderr, 'flags': flags,
         'source_sha256': hashlib.sha256(src.read_bytes()).hexdigest(),
         'object_sha256': hashlib.sha256(obj.read_bytes()).hexdigest(),
         'companion_sha256': hashlib.sha256(table.encode()).hexdigest()}
(HERE / 'passthrough_companion_compile.json').write_text(json.dumps(proof, indent=2) + '\n')
print(json.dumps(proof))
