#!/usr/bin/env python3
"""Source-bound strict scoring and independent no-mask local-table link proof.
Never opens asset data. Temporary object/link files are not publication artifacts.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score
NAME = 'func_80023BDC'
START, TABLE = 0x80023BDC, 0x8002D950
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
SOURCE = WORK / ('nonmatch/' + NAME + '.c')


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def elf_sections(path):
    data, sections = score._elf(path)
    return data, sections, {s['name']: data[s['off']:s['off']+s['size']] for s in sections}


def read_relocations(path):
    data, sections, contents = elf_sections(path)
    records = []
    for section in sections:
        if section['type'] != 9:
            continue
        syms = score._symbol_table(data, sections, section['link'])
        for i in range(section['size']//8):
            offset, info = struct.unpack_from('>II', data, section['off']+8*i)
            symbol = syms[info >> 8]
            records.append(dict(section=sections[section['info']]['name'],
                offset=offset, type=info & 255, symbol=symbol['name'],
                symbol_type=symbol['type'], symbol_value=symbol['value']))
    return records


def build_link(source, flags, directory):
    obj, linked = directory/'candidate.o', directory/'candidate.elf'
    score.compile_single(source, flags, obj)
    script = directory/'proof.ld'
    # Evaluation placement only: no production linker/storage ownership claim.
    script.write_text('SECTIONS {\n'
        ' .text 0x80023BDC : SUBALIGN(4) { *(.text) }\n'
        ' .rodata 0x8002D950 : SUBALIGN(4) { *(.rodata) }\n'
        ' /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.comment) *(.pdr) *(.mdebug*) }\n'
        '}\nfunc_80023AD4 = 0x80023AD4;\nfunc_80023B50 = 0x80023B50;\n')
    subprocess.run(['mips-linux-gnu-ld', '-T', str(script), '-o', str(linked), str(obj)],
                   check=True, capture_output=True)
    return obj, linked


def proof(obj, linked):
    _, _, original = elf_sections(obj)
    data, sections, linked_contents = elf_sections(linked)
    relocs = read_relocations(obj)
    calls = [r for r in relocs if r['section'] == '.text' and r['type'] == 4]
    halves = [r for r in relocs if r['section'] == '.text' and r['type'] in (5,6)]
    entries = [r for r in relocs if r['section'] == '.rodata']
    assert len(relocs) == 10 and len(calls) == 3 and len(halves) == 2 and len(entries) == 5
    assert [r['type'] for r in halves] == [5,6]
    assert all(r['symbol'] == '.rodata' and r['symbol_type'] == 3 for r in halves)
    assert [r['offset'] for r in entries] == list(range(0,20,4))
    assert all(r['type'] == 2 and r['symbol'] == '.text' and r['symbol_type'] == 3 for r in entries)
    assert [r['symbol'] for r in calls] == ['func_80023AD4','func_80023AD4','func_80023B50']
    words = list(struct.unpack('>'+str(len(original['.text'])//4)+'I', original['.text']))
    raw_table = list(struct.unpack('>5I', original['.rodata'][:20]))
    # Independently resolve every relocation, without masks or target-derived words.
    for r in calls:
        i = r['offset']//4
        destination = int(r['symbol'][5:],16) + ((words[i] & 0x3FFFFFF)<<2)
        assert destination & 3 == 0
        words[i] = (words[i]&0xFC000000) | ((destination>>2)&0x3FFFFFF)
    hi, lo = halves
    h, l = hi['offset']//4, lo['offset']//4
    low = words[l]&65535
    low = low-65536 if low&32768 else low
    address = TABLE + ((words[h]&65535)<<16) + low
    words[h] = (words[h]&0xFFFF0000) | (((address+0x8000)>>16)&65535)
    words[l] = (words[l]&0xFFFF0000) | (address&65535)
    mapped = [START + x for x in raw_table]
    assert all(x%4 == 0 and 0 <= x < len(original['.text']) for x in raw_table)
    independently_linked = struct.pack('>'+str(len(words))+'I',*words)
    assert independently_linked == linked_contents['.text']
    assert struct.pack('>5I',*mapped) == linked_contents['.rodata'][:20]
    assert not any(original['.rodata'][20:]) and len(original['.rodata']) == 32
    assert not read_relocations(linked)
    syms = [s for i,sec in enumerate(sections) if sec['type']==2
            for s in score._symbol_table(data,sections,i)]
    assert not any(s['section']==0 and s['name'] for s in syms)
    function = next(s for s in syms if s['name']==NAME)
    assert function['value']==START
    assert score.symbols(obj)=={NAME:0}
    assert all(x < function['size'] for x in raw_table)
    want = score.targets()[NAME]
    expected = [int(e['target'],16) for e in json.loads((WORK/'mapping_proof.json').read_text())['selector_to_target']]
    differing = sum(i>=len(words) or word != words[i] for i,word in enumerate(want))
    excess = sum(word!=0 for word in words[len(want):])
    return dict(independent_relocation_agrees_with_gnu_linker=True,
        all_relocations_resolved=True, masks_used=0,
        function_symbol_bytes=function['size'], text_section_bytes=len(independently_linked),
        full_word_differing=differing, extra_nonzero_words=excess,
        text_equal=differing==0 and excess==0,
        table_bytes_compared=20, candidate_rodata_alignment_tail_bytes=12,
        alignment_tail_is_not_claimed_native_data=True,
        candidate_case_targets=['0x%08X'%v for v in mapped],
        case_mapping_equal=mapped==expected,
        exact_text_and_table=differing==0 and excess==0 and mapped==expected,
        relocations=[{k:v for k,v in r.items() if k not in ('symbol_type','symbol_value')} for r in relocs],
        linked_text_sha256=hashlib.sha256(independently_linked[:404]).hexdigest(),
        linked_table_sha256=hashlib.sha256(linked_contents['.rodata'][:20]).hexdigest())


def run():
    score.ASM_DIR = ROOT/'asm/us/boot_tail'
    expected = json.loads((WORK/'mapping_proof.json').read_text())
    want = score.targets()[NAME]
    assert len(want)*4 == 404
    assert hashlib.sha256(struct.pack('>101I',*want)).hexdigest() == expected['canonical_function_sha256']
    manifest = score.target_manifest()
    for name in manifest:
        score.verified_bytes(score.ASM_DIR/name,manifest)
    inventory=json.loads((ROOT/'specs/015-boot-tail-runtime/inventory.json').read_text())['functions']
    extents=json.loads((score.ASM_DIR/'extents.json').read_text())['functions']
    assert {x['address']:x['size'] for x in inventory} == {x['address']:x['size'] for x in extents}
    pinned=json.loads((ROOT/'cloud/work/boot_tail/packet2/preflight.json').read_text())
    toolchain={name:digest(score.IDO/name) for name in pinned['compiler_files_sha256']}
    assert toolchain==pinned['compiler_files_sha256']
    results=[]
    with tempfile.TemporaryDirectory(prefix='bt05-mapped-macro-') as temp:
        directory=Path(temp)
        score.compile_single(ROOT/'cloud/matches/boot_tail/func_80010A00.c',FLAGS,directory/'getter.o')
        assert score.compare(directory/'getter.o','func_80010A00',show=0).accepted()
        for source in [SOURCE]+sorted((WORK/'controls').glob('*.c')):
            for flags in (FLAGS, FLAGS.replace('-O2','-O1')):
                obj,linked=build_link(source,flags,directory)
                result=score.compare(obj,NAME,show=0)
                resolved=proof(obj,linked)
                assert not result.accepted() and not resolved['exact_text_and_table']
                results.append(dict(source=str(source.relative_to(ROOT)),source_sha256=digest(source),
                    flags=flags,effective_flags=flags+' -Wab,-r4300_mul',
                    scorer=dict(differing=result.differing,total=result.total,extra_words=result.extra_words,
                        unresolved=result.unresolved,unverified=result.unverified,errors=result.errors,
                        accepted=result.accepted()),link_proof=resolved))
    return dict(result='COMPLETE_NONMATCH',base_commit='cf4b9c619c72e83aa5da2e8b5c110765f9b03693',
        matching_functions=0,verified_body_bytes=0,scorer_unchanged_sha256=digest(ROOT/'tools/cloud/score.py'),
        manifest_sha256=digest(score.ASM_DIR/'SHA256SUMS'),mapping_proof_sha256=digest(WORK/'mapping_proof.json'),
        pinned_compiler_hashes_verified=True,canonical_manifests_and_extents_verified=True,
        strict_getter_replay_pass=True,production_table_ownership='unassigned',results=results)


if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--check',action='store_true');args=parser.parse_args()
    result=run()
    if args.check:
        assert result==json.loads((WORK/'verification.json').read_text())
        print('PASS: source-bound scoring and independent no-mask text/table link replay.')
    else: print(json.dumps(result,indent=2))
