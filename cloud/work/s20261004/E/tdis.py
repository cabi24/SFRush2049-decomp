"""tdis.py FUNC [BIN] -> raw disassembly of target (or of BIN placed at FUNC's vaddr)"""
import sys, subprocess, tempfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[4]; sys.path.insert(0, str(ROOT))
from tools.conveyor.pipeline import blob_layout
l = blob_layout.load()
slots = {e['target_id']: e for r in l['regions'] for e in r['entries'] if e['kind'] == 'function'}
e = slots[sys.argv[1]]; base = 0x80086A50
if len(sys.argv) > 2: img = Path(sys.argv[2]).read_bytes()
else: img = (ROOT / 'build/game_code.bin').read_bytes()[e['vaddr']-base:e['vaddr']-base+e['size']]
f = tempfile.NamedTemporaryFile(suffix='.bin', delete=False); f.write(img); f.close()
out = subprocess.check_output(['mips-linux-gnu-objdump', '-D', '-b', 'binary', '-m', 'mips:4000', '-EB', '--no-show-raw-insn', '--adjust-vma=%#x' % e['vaddr'], f.name], text=True)
for line in out.splitlines():
    if ':\t' in line and line.strip()[0] in '0123456789abcdef':
        print(line.split('\t', 1)[1].replace('\t', ' '))
