# callers.py: list game-code functions whose retail words jal/j to the given addresses
import sys, json
sys.path.insert(0, '/home/cburnes/projects/rush2049-decomp/tools/cloud')
import score
from pathlib import Path
syms = {k: int(v, 16) for k, v in json.load(open('/home/cburnes/projects/rush2049-decomp/asm/us/blob/symbols.json'))['symbols'].items()}
targets = {int(a, 16): a for a in sys.argv[1:]}
T = score.targets()
for name, words in T.items():
    va = syms.get(name)
    if va is None: continue
    for i, w in enumerate(words):
        op = w >> 26
        if op in (2, 3):
            t = (w & 0x3FFFFFF) << 2 | ((va + 4 * i) & 0xF0000000)
            if t in targets:
                print(f'{name} +{4*i:#x} {"jal" if op == 3 else "j"} {targets[t]}')
