"""Exercise the proposed production helper through real MIPS ELF and linker inputs."""
import copy
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import struct
import subprocess

import pytest

ROOT = Path(__file__).resolve().parents[4]
DRAFT = Path(__file__).parent / 'tools/conveyor/pipeline/owned_data.py'
spec = importlib.util.spec_from_file_location('tools.conveyor.pipeline.c85_owned_data', DRAFT)
O = importlib.util.module_from_spec(spec)
spec.loader.exec_module(O)


def save(repo, registry):
    (repo / O.REGISTRY).write_text(json.dumps(registry))


@pytest.fixture
def fixture(tmp_path):
    if not shutil.which('mips-linux-gnu-as'):
        pytest.skip('MIPS GNU binutils required')
    (tmp_path / 'assets/us').mkdir(parents=True)
    (tmp_path / 'tools/asm-processor').mkdir(parents=True)
    shutil.copy2(ROOT / 'tools/asm-processor/asm_processor.py',
                 tmp_path / 'tools/asm-processor/asm_processor.py')
    obj = tmp_path / 'build/us/src/rom/lib_34a0.o'
    obj.parent.mkdir(parents=True)
    source = tmp_path / 'fixture.s'
    source.write_text('''.set noreorder
.text
.globl entry
.ent entry
entry:
jr $ra
nop
.end entry
.section .rodata
.balign 16
.rept 89
.word entry
.endr
''')
    subprocess.run(['mips-linux-gnu-as', '-march=vr4300', '-mabi=32',
                    str(source), '-o', str(obj)], check=True)
    original = bytes(range(1, 9)) + struct.pack('>I', 0x80001000) * 89 + bytes(range(21, 41))
    (tmp_path / 'assets/us/data.bin').write_bytes(original)
    row = {'owner': 'fcvt', 'tu': 'src/rom/lib_34a0.c',
           'source': 'assets/us/data.bin', 'offset': 8, 'size': 356,
           'container_vram': 0x8002D550, 'sha256': hashlib.sha256(original[8:364]).hexdigest(),
           'passthrough_asm': 'asm/us/fcvt.table.s', 'alignment': 4, 'logical_extent': True}
    registry = {'schema': 1, 'rom_slots': [row], 'storage_blocks': []}
    save(tmp_path, registry)
    return tmp_path, obj, row, registry, original


def load(fixture):
    repo, obj, *_ = fixture
    E = O._elf_processor(repo)
    return E, E.ElfFile(obj.read_bytes())


def store(fixture, elf):
    # Fixtures deliberately alter real ELF records, not the helper's decisions.
    elf.symtab.data = b''.join(s.to_bin() for s in elf.symtab.symbol_entries)
    for section in elf.sections:
        if section.is_rel():
            section.data = b''.join(r.to_bin() for r in section.relocations)
    elf.write(fixture[1])


def normalize(fixture):
    return O.normalize_object_sections(fixture[0], fixture[2]['tu'], fixture[1])


def test_real_elf_preserves_all_words_records_symbols_and_idempotence(fixture):
    E, old = load(fixture)
    old_table = old.find_section('.rodata')
    assert len(old_table.data) == 368
    retained = old_table.data[:356]
    records = {s.name: s.data for s in old.sections if s.is_rel()}
    symbols = [s.to_bin() for s in old.symtab.symbol_entries]
    assert normalize(fixture) == [{'owner': 'fcvt', 'section': '.rodata', 'size': 356, 'alignment': 4}]
    _, new = load(fixture)
    assert new.find_section('.rodata').data == retained
    assert new.find_section('.rodata').sh_addralign == 4
    assert {s.name: s.data for s in new.sections if s.is_rel()} == records
    assert [s.to_bin() for s in new.symtab.symbol_entries] == symbols
    before = fixture[1].read_bytes()
    assert normalize(fixture) == []
    assert fixture[1].read_bytes() == before


@pytest.mark.parametrize('changes', [
    {'alignment': 8}, {'alignment': True}, {'alignment': '4'},
    {'logical_extent': 'yes'}, {'logical_extent': False},
    {'alignment': 16}, {'offset': 9}, {'size': 355}, {'size': 0},
    {'container_vram': 0x8002D551}, {'owner_section': '.data'},
])
def test_registry_geometry_rejects_without_object_mutation(fixture, changes):
    repo, obj, row, registry, _ = fixture
    row.update(changes)
    save(repo, registry)
    before = obj.read_bytes()
    with pytest.raises(O.OwnershipError):
        normalize(fixture)
    assert obj.read_bytes() == before


def test_default_16_byte_behavior_still_rejects_4_byte_extent(fixture):
    repo, _, row, registry, _ = fixture
    del row['alignment'], row['logical_extent']
    save(repo, registry)
    with pytest.raises(O.OwnershipError):
        O.rom_slots(repo)


def test_default_16_byte_owner_does_not_rewrite_any_object(fixture):
    repo, obj, row, registry, original = fixture
    del row['alignment'], row['logical_extent']
    row.update(offset=16, size=32, sha256=hashlib.sha256(original[16:48]).hexdigest())
    save(repo, registry)
    before = obj.read_bytes()
    assert normalize(fixture) == []
    assert obj.read_bytes() == before


def test_unrelated_real_table_object_cannot_be_normalized_as_fcvt(fixture):
    repo, obj, row, *_ = fixture
    other = obj.with_name('unrelated.o')
    shutil.copy2(obj, other)
    before, original = other.read_bytes(), obj.read_bytes()
    with pytest.raises(O.OwnershipError, match='registry-derived owner'):
        O.normalize_object_sections(repo, row['tu'], other)
    assert other.read_bytes() == before
    assert obj.read_bytes() == original


def test_bad_relocation_symbol_index_is_a_clean_atomic_refusal(fixture):
    _, obj = load(fixture)
    obj.find_section('.rodata').relocated_by[0].relocations[-1].sym_index = len(obj.symtab.symbol_entries) + 5
    store(fixture, obj)
    before = fixture[1].read_bytes()
    with pytest.raises(O.OwnershipError, match='symbol index'):
        normalize(fixture)
    assert fixture[1].read_bytes() == before


def test_unrelated_tu_does_not_read_ignored_containers(fixture):
    repo, obj, row, *_ = fixture
    (repo / row['source']).unlink()
    before = obj.read_bytes()
    assert O.normalize_object_sections(repo, 'src/rom/lib_unrelated.c', obj) == []
    assert obj.read_bytes() == before


def test_matching_owner_still_requires_original_source_identity(fixture):
    repo, obj, row, _, original = fixture
    altered = bytearray(original)
    altered[row['offset']] ^= 1
    (repo / row['source']).write_bytes(altered)
    before = obj.read_bytes()
    with pytest.raises(O.OwnershipError, match='original-byte identity'):
        normalize(fixture)
    assert obj.read_bytes() == before


@pytest.mark.parametrize('failure', ['nonzero_tail', 'extra_zero_word', 'alignment',
                                     'tail_relocation', 'unaligned_relocation',
                                     'missing_relocation', 'duplicate_relocation',
                                     'non_word_relocation', 'bad_text_pointer',
                                     'tail_symbol', 'crossing_symbol', 'tail_address',
                                     'tail_hi_lo', 'orphan_hi'])
def test_real_elf_content_and_reference_negatives_are_atomic(fixture, failure):
    E, obj = load(fixture)
    table = obj.find_section('.rodata')
    reltab = table.relocated_by[0]
    if failure == 'nonzero_tail':
        table.data = table.data[:356] + b'X' + table.data[357:]
    elif failure == 'extra_zero_word':
        table.data += b'\0' * 16
    elif failure == 'alignment':
        table.sh_addralign = 8
    elif failure == 'tail_relocation':
        reltab.relocations[-1].r_offset = 356
    elif failure == 'unaligned_relocation':
        reltab.relocations[-1].r_offset = 351
    elif failure == 'missing_relocation':
        reltab.relocations.pop()
    elif failure == 'duplicate_relocation':
        reltab.relocations[-1].r_offset = 348
    elif failure == 'non_word_relocation':
        reltab.relocations[-1].rel_type = E.R_MIPS_HI16
    elif failure == 'bad_text_pointer':
        table.data = struct.pack('>I', 32) + table.data[4:]
    elif failure in ['tail_symbol', 'crossing_symbol']:
        name = 'real_tail_object'
        value, size = (356, 4) if failure == 'tail_symbol' else (352, 8)
        sym = E.Symbol.from_parts(obj.fmt, obj.symtab.strtab.add_str(name), value, size,
                                  (E.STB_GLOBAL << 4) | E.STT_OBJECT, 0, table.index,
                                  obj.symtab.strtab, name)
        obj.symtab.symbol_entries.append(sym)
    elif failure in ['tail_address', 'tail_hi_lo', 'orphan_hi']:
        # An actual text relocation to .rodata+356 must not survive trimming.
        section_symbol = next(i for i, s in enumerate(obj.symtab.symbol_entries)
                              if s.type == E.STT_SECTION and s.st_shndx == table.index)
        text = obj.find_section('.text')
        if failure == 'tail_address':
            text.data = struct.pack('>I', 356) + text.data[4:]
            records = obj.fmt.pack('II', 0, (section_symbol << 8) | E.R_MIPS_32)
        else:
            text.data = struct.pack('>II', 0x3c080000, 0x25080164) + text.data[8:]
            records = obj.fmt.pack('II', 0, (section_symbol << 8) | E.R_MIPS_HI16)
            if failure == 'tail_hi_lo':
                records += obj.fmt.pack('II', 4, (section_symbol << 8) | E.R_MIPS_LO16)
        obj.add_section('.rel.text', E.SHT_REL, 0, obj.symtab.index, text.index, 4, 8, records)
    store(fixture, obj)
    before = fixture[1].read_bytes()
    with pytest.raises(O.OwnershipError):
        normalize(fixture)
    assert fixture[1].read_bytes() == before


def test_descriptive_section_size_is_updated_without_removing_symbol(fixture):
    E, obj = load(fixture)
    table = obj.find_section('.rodata')
    section_symbol = next(s for s in obj.symtab.symbol_entries
                          if s.type == E.STT_SECTION and s.st_shndx == table.index)
    section_symbol.st_size = 368
    store(fixture, obj)
    old_symbols = [(s.name, s.st_value, s.st_shndx, s.type, s.bind)
                   for s in obj.symtab.symbol_entries]
    normalize(fixture)
    _, new = load(fixture)
    assert [(s.name, s.st_value, s.st_shndx, s.type, s.bind)
            for s in new.symtab.symbol_entries] == old_symbols
    assert next(s for s in new.symtab.symbol_entries
                if s.type == E.STT_SECTION and s.st_shndx == table.index).st_size == 356


def test_overlapping_owned_slots_still_reject(fixture):
    repo, _, row, registry, _ = fixture
    other = copy.deepcopy(row)
    other.update(owner='other', tu='src/rom/lib_other.c', offset=12)
    registry['rom_slots'].append(other)
    save(repo, registry)
    with pytest.raises(O.OwnershipError, match='overlap'):
        O.rom_slots(repo)


def link_container(fixture):
    repo, obj, row, _, original = fixture
    container = repo / 'build/us/assets/us/data.o'
    O.split_container(repo, row['source'], repo / row['source'], container)
    script = '''SECTIONS {
 .text 0x80001000 : { build/us/src/rom/lib_34a0.o(.text) }
 .rodata : {
  build/us/src/rom/lib_34a0.o(.rodata);
 }
 .data 0x8002D550 : {
  build/us/assets/us/data.o(.data);
 }
 /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }
}
'''
    rewritten = O.rewrite_linker(script, repo)
    assert '== 0x8002d558' in rewritten
    assert '== 0x164' in rewritten
    assert O.rewrite_linker(rewritten, repo) == rewritten
    (repo / 'model.ld').write_text(rewritten)
    cp = subprocess.run(['mips-linux-gnu-ld', '--accept-unknown-input-arch', '-T', 'model.ld', '-o', 'linked.elf',
                         str(obj.relative_to(repo)), str(container.relative_to(repo))],
                        cwd=repo, capture_output=True, text=True)
    return cp, repo / 'linked.elf'


def test_real_split_link_preserves_entire_container_and_nonzero_neighbors(fixture):
    normalize(fixture)
    cp, linked = link_container(fixture)
    assert cp.returncode == 0, cp.stderr
    out = fixture[0] / 'linked_data.bin'
    subprocess.run(['mips-linux-gnu-objcopy', '-O', 'binary', '-j', '.data', str(linked), str(out)], check=True)
    assert out.read_bytes() == fixture[4]
    assert out.read_bytes()[364:] == bytes(range(21, 41))


def test_untrimmed_full_section_cannot_pass_exact_linker_assertions(fixture):
    cp, _ = link_container(fixture)
    assert cp.returncode != 0
    assert 'owned ROM slot size changed' in cp.stderr
