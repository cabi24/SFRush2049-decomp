"""Replay the private production draft on the frozen real C85 IDO whole module.

This verifies source-built static text/table and unchanged original data container.
It is not a cartridge/coverage claim or a current game-blob/full-ROM proof.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = Path.cwd()
sys.path[:0] = [str(ROOT), str(ROOT / 'tools/cloud')]
import score

draft = ROOT / 'cloud/work/static_C85/proposal/tools/conveyor/pipeline/owned_data.py'
spec = importlib.util.spec_from_file_location('tools.conveyor.pipeline.c85_owned_data', draft)
O = importlib.util.module_from_spec(spec)
spec.loader.exec_module(O)
sha = lambda data: hashlib.sha256(data).hexdigest()
packet = ROOT / 'cloud/work/static_C85'
manifest = json.loads((packet / 'manifest.json').read_text())
compile_proof = json.loads((packet / 'full_module_compile.json').read_text())
source = ROOT / manifest['full_native_module']
assert sha(source.read_bytes()) == manifest['full_native_module_sha256']
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--passthrough', action='store_true', help='verify eight-native/body-passthrough baseline companion')
args = parser.parse_args()
if args.passthrough:
    source = ROOT / 'build/C85/proposal_module/lib_34a0.c'
    input_object = ROOT / 'build/C85/proposal_module/lib_34a0.o'
else:
    input_object = Path('/tmp/rush-static-wrapper-preview/lib_34a0_C85/lib_34a0.o')
    wrapper = json.loads((HERE / 'C85_production_wrapper.json').read_text())
    assert sha(input_object.read_bytes()) == wrapper['object_sha256']
    assert wrapper['frozen_module_sha256'] == manifest['full_native_module_sha256']
row = json.loads((ROOT / 'cloud/work/static_C85/proposal/fcvt.slot.proposed.json').read_text())
original = (ROOT / row['source']).read_bytes()
row_offset = int(row['offset'], 0)
assert sha(original[row_offset:row_offset + int(row['size'], 0)]) == row['sha256']
original_words = {int(a, 16): int(w, 16) for a, w in re.findall(
    r'/\*\s+[0-9A-Fa-f]+\s+([0-9A-Fa-f]+)\s+([0-9A-Fa-f]{8})\s+\*/',
    (ROOT / 'asm/us/34A0.s').read_text())}
old_proof = json.loads((packet / 'full_module_proofs.json').read_text())
function_addresses = {r['function']: int(r['actual_vaddr'], 16) for r in old_proof['members']}
text_start = min(function_addresses.values())
externs = {n: int(a, 16) for n, a in re.findall(
    r'^([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+)',
    (ROOT / 'symbol_addrs.us.txt').read_text() + '\n' +
    (ROOT / 'undefined_syms_auto.us.txt').read_text(), re.M)}

with tempfile.TemporaryDirectory(prefix='static-C85-owned-table-') as scratch:
    repo = Path(scratch)
    (repo / 'assets/us').mkdir(parents=True)
    (repo / 'tools/asm-processor').mkdir(parents=True)
    shutil.copy2(ROOT / 'tools/asm-processor/asm_processor.py',
                 repo / 'tools/asm-processor/asm_processor.py')
    (repo / row['source']).write_bytes(original)
    (repo / O.REGISTRY).write_text(json.dumps({'schema': 1, 'rom_slots': [row], 'storage_blocks': []}))
    obj = repo / 'build/us/src/rom/lib_34a0.o'
    obj.parent.mkdir(parents=True)
    shutil.copy2(input_object, obj)
    E = O._elf_processor(repo)
    old_elf = E.ElfFile(obj.read_bytes())
    old_relocations = {s.name: s.data for s in old_elf.sections if s.is_rel()}
    old_symbols = [s.to_bin() for s in old_elf.symtab.symbol_entries]
    # First prove the excluded compiler tail is the only thing removed.
    changes = O.normalize_object_sections(repo, row['tu'], obj)
    assert changes == [{'owner': 'fcvt', 'section': '.rodata', 'size': 356, 'alignment': 4}]
    new_elf = E.ElfFile(obj.read_bytes())
    table = new_elf.find_section('.rodata')
    assert table.data == old_elf.find_section('.rodata').data[:356]
    assert {s.name: s.data for s in new_elf.sections if s.is_rel()} == old_relocations
    changed_symbols = []
    for before, after in zip(old_elf.symtab.symbol_entries, new_elf.symtab.symbol_entries):
        if before.to_bin() != after.to_bin():
            assert before.type == after.type == E.STT_SECTION
            assert before.st_shndx == after.st_shndx == table.index
            assert (before.st_size, after.st_size) == (368, 356)
            changed_symbols.append({'symbol': before.name, 'old_descriptive_size': 368, 'new_descriptive_size': 356})
    assert len(old_symbols) == len(new_elf.symtab.symbol_entries)
    before_replay = obj.read_bytes()
    assert O.normalize_object_sections(repo, row['tu'], obj) == []
    assert obj.read_bytes() == before_replay
    container = repo / 'build/us/assets/us/data.o'
    O.split_container(repo, row['source'], repo / row['source'], container)
    script = f'''SECTIONS {{
 .text 0x{text_start:x} : {{ build/us/src/rom/lib_34a0.o(.text) }}
 .rodata : {{
  build/us/src/rom/lib_34a0.o(.rodata);
 }}
 .data 0x8000F400 : {{
  build/us/assets/us/data.o(.data);
 }}
 /DISCARD/ : {{ *(.bss) *(.reginfo) *(.options) *(.mdebug) *(.comment) *(.pdr) }}
}}
'''
    script += '\n'.join(f'{n} = 0x{a:x};' for n, a in externs.items() if n not in function_addresses) + '\n'
    rewritten = O.rewrite_linker(script, repo)
    assert O.rewrite_linker(rewritten, repo) == rewritten
    assert 'rom_owned_fcvt_START == 0x8002d558' in rewritten
    assert 'rom_owned_fcvt_END - rom_owned_fcvt_START == 0x164' in rewritten
    (HERE / 'generated_linker.proposed.ld').write_text(rewritten)
    (repo / 'model.ld').write_text(rewritten)
    linked = repo / 'linked.elf'
    command = ['mips-linux-gnu-ld', '--accept-unknown-input-arch', '-T', 'model.ld',
               '-o', 'linked.elf', str(obj.relative_to(repo)), str(container.relative_to(repo))]
    subprocess.run(command, cwd=repo, check=True, capture_output=True, text=True)
    words = score.text_words(linked)
    symbols = score.symbols(linked)
    members = []
    for r in old_proof['members']:
        address = function_addresses[r['function']]
        extent = r['original_slot_words']
        actual = words[(address - text_start)//4:(address - text_start)//4 + extent]
        expected = [original_words[address + 4*i] for i in range(extent)]
        assert actual == expected
        assert symbols[r['function']] == address
        members.append({'function': r['function'], 'native': not (args.passthrough and r['function'] == 'fcvt'),
                        'original_words': extent, 'differing_full_words': 0, 'slot_delta_bytes': 0})
    linked_data = repo / 'linked_data.bin'
    subprocess.run(['mips-linux-gnu-objcopy', '-O', 'binary', '-j', '.data', str(linked), str(linked_data)], check=True)
    payload = linked_data.read_bytes()
    assert payload == original
    logical_size = 356
    neighbor = original[row_offset + logical_size:row_offset + logical_size + 12]
    assert sum(bool(b) for b in neighbor) == 8
    assert payload[row_offset + logical_size:row_offset + logical_size + 12] == neighbor
    # Keep the exact linker size gate: the unchanged original padded section fails.
    normalized_object = obj.read_bytes()
    shutil.copy2(input_object, obj)
    cp = subprocess.run(command, cwd=repo, capture_output=True, text=True)
    assert cp.returncode != 0 and 'owned ROM slot size changed' in cp.stderr
    obj.write_bytes(normalized_object)
    # A changed neighbor remains present in the split container and is detected
    # by the final identity comparison; the helper must not hide unrelated data.
    changed = bytearray(original)
    changed[row_offset + logical_size] ^= 1
    altered = repo / 'altered.bin'
    altered.write_bytes(changed)
    O.split_container(repo, row['source'], altered, container)
    subprocess.run(command, cwd=repo, check=True, capture_output=True, text=True)
    subprocess.run(['mips-linux-gnu-objcopy', '-O', 'binary', '-j', '.data', str(linked), str(linked_data)], check=True)
    changed_payload = linked_data.read_bytes()
    assert changed_payload == bytes(changed)
    assert sha(changed_payload) != sha(original)
    proof = {
        'status': 'Independent root replay: production-wrapper object plus private ownership draft; no publication claim',
        'native_function_count': 8 if args.passthrough else 9,
        'fcvt_body_passthrough': args.passthrough,
        'draft_sha256': sha(draft.read_bytes()), 'source_sha256': sha(source.read_bytes()),
        'input_object_sha256': sha(input_object.read_bytes()),
        'normalized_object_sha256': sha(normalized_object),
        'all_module_original_text_bytes': len(words)*4,
        'native_original_text_bytes': len(words)*4 - (3948 if args.passthrough else 0),
        'members': members,
        'retained_table_bytes': 356, 'retained_table_sha256': row['sha256'],
        'retained_table_relocations': len(table.relocated_by[0].relocations),
        'all_relocation_records_unchanged': True, 'symbol_count_unchanged': True,
        'descriptive_section_symbol_changes': changed_symbols,
        'original_container_bytes': len(original), 'original_container_sha256': sha(original),
        'full_linked_container_exact': True, 'following_12_bytes_sha256': sha(neighbor),
        'following_12_bytes_nonzero_count': 8, 'untrimmed_input_link_rejected': True,
        'changed_neighbor_identity_rejected': sha(changed_payload) != sha(original),
        'generated_linker': str(HERE / 'generated_linker.proposed.ld'),
        'remaining_gates': ['review/integrate draft', 'supported target refresh + independent strict replay',
                            'current source-built game + full-ROM + lock + pytest gates'],
    }
    result_name = 'root_passthrough_companion_proof.json' if args.passthrough else 'C85_root_wrapper_owned_table_proof.json'
    (HERE / result_name).write_text(json.dumps(proof, indent=2) + '\n')
    print(json.dumps(proof, indent=2))
