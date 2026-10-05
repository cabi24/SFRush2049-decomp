#!/usr/bin/env python3
"""Fixed donor-boundary controls. Successful execution certifies NONMATCH evidence."""
from contextlib import redirect_stdout
from dataclasses import asdict
import hashlib
import io
import json
from pathlib import Path
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT / 'tools/cloud'))
import score

NAME = 'func_800D2C10'
BASE = 'e24b47d89a0c8ffade1e4c75ad76b9d390a1c232'
DONOR = '845329d7b36f5a384c5625ed9a0aef584ab46139'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'

def sha(data):
    return hashlib.sha256(data).hexdigest()

def symbol(obj, name):
    data, sections = score._elf(obj)
    matches = [s for i, sec in enumerate(sections) if sec['type'] == 2
               for s in score._symbol_table(data, sections, i)
               if s['name'] == name and s['type'] == 2]
    if len(matches) != 1 or not matches[0]['size']:
        raise AssertionError('exact nonzero function extent required')
    return matches[0]

def audit(obj, directory):
    target = score.targets()[NAME]
    sym = symbol(obj, NAME)
    start, end = sym['value'], sym['value'] + sym['size']
    words, masks, unresolved, unverified, errors = score.relocate(obj, score.text_words(obj), start, end, score.image_symbols())
    assert not (masks or unresolved or unverified or errors)
    own = words[start//4:end//4]
    with redirect_stdout(io.StringIO()):
        result = score.compare(obj, NAME, show=0)
    assert not result.accepted()
    assert not (result.unresolved or result.unverified or result.errors)
    data, secs = score._elf(obj)
    relocations = sum(1 for sec in secs if sec['type'] == 9 and secs[sec['info']]['name'] == '.text'
                      for at in range(sec['off'], sec['off'] + sec['size'], 8)
                      if start <= struct.unpack_from('>I', data, at)[0] < end)
    # GNU ld resolves the whole object independently, at the native body address.
    native = score.image_symbols()[NAME]
    script = directory / 'link.ld'
    script.write_text('SECTIONS { .text 0x%x : SUBALIGN(4) { *(.text) } }\n' % (native-start)
                      + '\n'.join('%s = 0x%X;' % (n, score.image_symbols()[n])
                                  for n in ('D_8012E5E8','D_8012417C')) + '\n')
    linked = directory / 'linked.elf'
    subprocess.run(['mips-linux-gnu-ld','-m','elf32btsmip','-T',str(script),str(obj),'-o',str(linked)],
                   check=True, capture_output=True)
    lsym = symbol(linked, NAME)
    assert lsym['value'] == native and lsym['size'] == sym['size']
    gnu = score.text_words(linked)[start//4:end//4]
    assert gnu == own
    full_differing = sum(i >= len(own) or own[i] != w for i,w in enumerate(target))
    extra = max(len(own)-len(target),0)
    frame = next((-((w & 0xffff)-0x10000) for w in own if w >> 16 == 0x27bd and w & 0x8000),0)
    return dict(status='NONMATCH', strict_match=False, elf_bytes=sym['size'], frame_bytes=frame,
                target_words=len(target), full_target_differing=full_differing,
                excess_elf_words=extra, missing_elf_words=max(len(target)-len(own),0),
                canonical=asdict(result), relocations=relocations,
                full_body_gnu_equal=True, body_sha256=sha(struct.pack('>%dI'%len(own),*own)))

def verify():
    targets = score.targets(); syms = score.image_symbols()
    assert NAME not in json.loads((ROOT/'blob_matched.lock.json').read_text())
    old = ROOT/'cloud/work/near_miss_B6/func_800D2C10_best.c'
    signed_old = ROOT/'cloud/work/near-miss/func_800D2C10/base.c'
    macro = (HERE/'vector_macro.c').read_text()
    helper = (HERE/'vector_dotprod.c').read_text()
    cases = [
        ('historical_unsigned_O2', old.read_text(), '-g0 -O2 -mips2 -G 0 -non_shared', False),
        ('historical_unsigned_O3', old.read_text(), FLAGS, False),
        ('vector_macro_unsigned_O3', macro, FLAGS, False),
        ('vector_dotprod_unsigned_group', helper, FLAGS, True),
        ('vector_macro_signed_O3', macro.replace('u32 i;','s32 i;'), FLAGS, False),
        ('vector_dotprod_signed_group', helper.replace('u32 i;','s32 i;'), FLAGS, True),
        ('vector_macro_signed_O1', macro.replace('u32 i;','s32 i;'), '-g0 -O1 -mips2 -G 0 -non_shared', False),
        ('historical_signed_O1', signed_old.read_text(), '-g0 -O1 -mips2 -G 0 -non_shared', False),
    ]
    rows = {}
    with tempfile.TemporaryDirectory(prefix='nearest-donor-') as tmp:
        for label, text, flags, group in cases:
            directory=Path(tmp)/label;directory.mkdir()
            src=directory/'source.c';src.write_text(text);obj=directory/'output.o'
            if group:
                spec=dict(members=[NAME],files=['source.c'],flags=flags,keep=[NAME],claims=[])
                (directory/'group.json').write_text(json.dumps(spec))
                score.compile_group(directory,obj)
            else:
                score.compile_single(src,flags,obj)
            rows[label]=dict(source_sha256=sha(src.read_bytes()),flags=flags,group=group,**audit(obj,directory))
    caller_sites=[]
    for caller,words in targets.items():
        for i,w in enumerate(words):
            if w>>26 == 3 and (0x80000000 | ((w & 0x3ffffff)<<2)) == syms[NAME]:
                caller_sites.append(dict(caller=caller,address='0x%08X'%(syms[caller]+4*i)))
    words=targets[NAME]
    return dict(schema=1,status='REJECTED_DONOR_BOUNDARY_HYPOTHESIS',claims=[],base=BASE,
        function=NAME,start='0x%08X'%syms[NAME],end='0x%08X'%(syms[NAME]+len(words)*4),
        target_bytes=len(words)*4,target_sha256=sha(struct.pack('>%dI'%len(words),*words)),
        caller_sites=caller_sites,donor_commit=DONOR,
        verifier_sha256=sha(Path(__file__).read_bytes()),
        source_files={p.name:sha(p.read_bytes()) for p in sorted(HERE.glob('*.c'))},
        input_files={str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in
          [old,signed_old,ROOT/'asm/us/blob/SHA256SUMS',ROOT/'asm/us/blob/symbols.json',ROOT/'tools/cloud/score.py']},
        compiler_sha256={n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','uopt','ugen','as1','uld','umerge']},
        experiments=rows,behavioral_proof=False,accepted_byte_gain=0)

if __name__=='__main__':
    print(json.dumps(verify(),indent=2,sort_keys=True))
