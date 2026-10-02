"""Compile and record an honest NONMATCH; does not alter any protected artifact."""
from pathlib import Path
import hashlib, json, os, shlex, struct, subprocess, sys, tempfile
ROOT = Path(__file__).resolve().parents[3]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / 'tools/cloud'))
import score
NAME = 'func_80091B00'
SOURCE = HERE / (NAME + '.c')
FLAGS = SOURCE.read_text().split('/* flags: ')[1].split(' */')[0]
def sha(p): return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def digest(words): return hashlib.sha256(struct.pack('>%dI' % len(words), *words)).hexdigest()
with tempfile.TemporaryDirectory() as td:
    td = Path(td)
    (td / 'candidate.c').write_bytes(SOURCE.read_bytes())
    subprocess.run([str(score.IDO.resolve() / 'cc'), '-c', *shlex.split(FLAGS), '-o', 'candidate.o', 'candidate.c'], cwd=td, check=True)
    obj = td / 'candidate.o'
    raw = score.text_words(obj)
    resolved = list(raw)
    data, sections = score._elf(obj)
    text_index = score._text_index(sections)
    symbols = score.image_symbols()
    pending = {}
    relocations = []
    for section in sections:
        if section['type'] != 9 or section['info'] != text_index: continue
        table = score._symbol_table(data, sections, section['link'])
        for j in range(section['size'] // 8):
            offset, info = struct.unpack_from('>II', data, section['off'] + j * 8)
            name = table[info >> 8]['name']; kind = info & 255; index = offset // 4
            assert name in symbols
            rec = dict(offset=offset, type=kind, symbol=name, address=hex(symbols[name]))
            relocations.append(rec)
            if kind == 5: pending.setdefault(name, []).append(index)
            elif kind == 6:
                low = raw[index] & 65535
                signed_low = low if low < 32768 else low - 65536
                for high_index in pending.pop(name):
                    addend = ((raw[high_index] & 65535) << 16) + signed_low
                    value = (symbols[name] + addend) & 0xffffffff
                    resolved[high_index] = (raw[high_index] & 0xffff0000) | (((value + 32768) >> 16) & 65535)
                    resolved[index] = (raw[index] & 0xffff0000) | (value & 65535)
                    rec['addend'] = addend
            else: raise AssertionError(kind)
    assert not pending
    target = score.targets()[NAME]
    differences = [dict(offset=4*i, target='%08x' % a, candidate='%08x' % b) for i,(a,b) in enumerate(zip(target,resolved)) if a != b]
    canonical = score.compare(obj, NAME, show=0)
    body = next(s for i, sec in enumerate(sections) if sec['type'] == 2 for s in score._symbol_table(data,sections,i) if s['name'] == NAME)
    assert body['size'] == 168 and len(target) == 42 and len(differences) == 18
    assert not canonical.accepted(False) and canonical.differing == 18
    assert not canonical.unresolved and not canonical.unverified and not canonical.errors
    assert all(w == 0 for w in resolved[42:])
    calls=[]
    for n,words in score.targets().items():
        for i,w in enumerate(words):
            if w >> 26 in (2,3) and (((symbols[n]+4*i+4)&0xf0000000) | ((w&0x3ffffff)<<2)) == symbols[NAME]:
                calls.append(dict(caller=n,word=i,opcode=w>>26))
    result = dict(verdict='NONMATCH',source_sha256=sha(SOURCE),flags=FLAGS,base_commit=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip(),target_start=hex(symbols[NAME]),target_end_exclusive=hex(symbols[NAME]+168),target_words=42,function_bytes=body['size'],differing_full_words=18,matching_full_words=24,extra_nonzero_words=0,zero_alignment_words=len(resolved)-42,relocations=relocations,unresolved=[],unverified=[],masks={},raw_target_words=['%08x'%w for w in target],raw_candidate_words=['%08x'%w for w in raw],resolved_candidate_body_words=['%08x'%w for w in resolved[:42]],differences=differences,target_sha256=digest(target),resolved_body_sha256=digest(resolved[:42]),object_sha256=sha(obj),compiler_cc_sha256=sha(score.IDO/'cc'),scorer_sha256=sha(ROOT/'tools/cloud/score.py'),direct_transfers=calls,scope='Research-only source; no match, promotion, accepted coverage or ROM identity claim')
    (HERE/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print('Verified NONMATCH: 18/42 full words differ; complete 168-byte function; no unresolved relocations')
