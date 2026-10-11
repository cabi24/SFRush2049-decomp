"""The two w21i owned slots as committed, and logical-extent literal constants.

func_80022CD4 (lib_22300, MusyX DoSetPitch) owns lib_22300's .data at
0x8002D460 (its function-local `static u16 kf[13]` + IDO's 6-byte section pad,
0x20). func_8001C860 (lib_1cf90, snd3d CalcEmitter) owns the first 4 bytes of
lib_1cf90's .rodata at 0x8002D900 (the `1.f / 60.f` literal, 0x3C888889; the
TU's D_8002D904/908/90C follow): a logical extent of kind "constants", which
is a relocation-free readonly section trimmed from IDO's 16 to 4 bytes.
"""
import copy
import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess

import pytest

from tools.conveyor.pipeline import owned_data as O

REPO = Path(__file__).resolve().parents[2]


def test_committed_rows_match_their_companions_and_tus():
    rows = {r["owner"]: r for r in O.rom_slots(REPO, check_source=False)}
    kf, lit = rows["func_80022CD4"], rows["func_8001C860"]
    assert (kf["tu"], kf["owner_section"], kf["container_vram"] + kf["offset"], kf["size"]) == (
        "src/rom/lib_22300.c", ".data", 0x8002D460, 0x20)
    assert not kf.get("logical_extent")
    assert (lit["tu"], lit["owner_section"], lit["container_vram"] + lit["offset"], lit["size"]) == (
        "src/rom/lib_1cf90.c", ".rodata", 0x8002D900, 4)
    assert lit["logical_extent"] and lit["logical_kind"] == "constants" and lit["alignment"] == 4
    for r in (kf, lit):
        companion = (REPO / r["passthrough_asm"]).read_text()
        assert ".incbin" in companion and ".word" not in companion
        if (REPO / r["source"]).is_file():
            assert companion == O.companion_source(r, REPO)
        text = (REPO / r["tu"]).read_text()
        if f'/{r["owner"]}.s")' in text:
            assert text.count(O.companion_block(r)) == 1
    if (REPO / lit["source"]).is_file():
        data = (REPO / lit["source"]).read_bytes()
        assert data[lit["offset"]:lit["offset"] + 4] == bytes.fromhex("3c888889")


def save(repo, registry):
    (repo / O.REGISTRY).write_text(json.dumps(registry))


@pytest.fixture
def fixture(tmp_path):
    """A real MIPS object shaped like IDO's: text loading a float literal from
    .rodata (HI16/LO16 to the section symbol), .rodata = 4 literal bytes + 12
    bytes of zero section padding, alignment 16, no .rodata relocations."""
    if not shutil.which('mips-linux-gnu-as'):
        pytest.skip('MIPS GNU binutils required')
    (tmp_path / 'assets/us').mkdir(parents=True)
    (tmp_path / 'tools/asm-processor').mkdir(parents=True)
    shutil.copy2(REPO / 'tools/asm-processor/asm_processor.py',
                 tmp_path / 'tools/asm-processor/asm_processor.py')
    obj = tmp_path / 'build/us/src/rom/lib_1cf90.o'
    obj.parent.mkdir(parents=True)
    source = tmp_path / 'fixture.s'
    source.write_text('''.set noreorder
.text
.globl entry
.ent entry
entry:
lui $at, %hi(lit)
lwc1 $f4, %lo(lit)($at)
jr $ra
nop
.end entry
.section .rodata
.balign 16
lit:
.word 0x3C888889
.space 12
''')
    subprocess.run(['mips-linux-gnu-as', '-march=vr4300', '-mabi=32',
                    str(source), '-o', str(obj)], check=True)
    original = bytes(range(1, 17)) + bytes.fromhex('3c888889') + bytes.fromhex('3e19999a3da3d70a3e99999a')
    (tmp_path / 'assets/us/data.bin').write_bytes(original)
    row = {'owner': 'func_8001C860', 'tu': 'src/rom/lib_1cf90.c',
           'source': 'assets/us/data.bin', 'offset': 16, 'size': 4,
           'container_vram': 0x8002D8F0, 'sha256': hashlib.sha256(original[16:20]).hexdigest(),
           'passthrough_asm': 'asm/us/nonmatchings/rom/lib_1cf90/func_8001C860.rodata.s',
           'alignment': 4, 'logical_extent': True, 'logical_kind': 'constants'}
    registry = {'schema': 1, 'rom_slots': [row], 'storage_blocks': []}
    save(tmp_path, registry)
    return tmp_path, obj, row, registry, original


def normalize(fixture):
    return O.normalize_object_sections(fixture[0], fixture[2]['tu'], fixture[1])


def load(fixture):
    E = O._elf_processor(fixture[0])
    return E, E.ElfFile(fixture[1].read_bytes())


def test_constants_extent_trims_padding_and_keeps_relocations(fixture):
    E, old = load(fixture)
    assert len(old.find_section('.rodata').data) == 16
    records = {s.name: s.data for s in old.sections if s.is_rel()}
    assert normalize(fixture) == [{'owner': 'func_8001C860', 'section': '.rodata', 'size': 4, 'alignment': 4}]
    _, new = load(fixture)
    assert new.find_section('.rodata').data == bytes.fromhex('3c888889')
    assert new.find_section('.rodata').sh_addralign == 4
    assert {s.name: s.data for s in new.sections if s.is_rel()} == records
    before = fixture[1].read_bytes()
    assert normalize(fixture) == []
    assert fixture[1].read_bytes() == before


def test_constants_companion_is_the_original_bytes():
    row = {'owner': 'f', 'owner_section': '.rodata', 'source': 'assets/us/data.bin',
           'offset': 0x6130, 'size': 4, 'logical_extent': True, 'logical_kind': 'constants'}
    assert O.companion_source(row) == ('.section .rodata\n.balign 4\n'
                                       '.incbin "assets/us/data.bin", 0x6130, 0x4\n')


def test_table_kind_still_requires_every_word_relocated(fixture):
    repo, obj, row, registry, _ = fixture
    del row['logical_kind']
    save(repo, registry)
    before = obj.read_bytes()
    with pytest.raises(O.OwnershipError, match='every complete relocated word'):
        normalize(fixture)
    assert obj.read_bytes() == before


@pytest.mark.parametrize('failure', ['nonzero_tail', 'tail_address', 'rodata_relocation'])
def test_constants_negatives_are_atomic(fixture, failure):
    E, obj = load(fixture)
    table = obj.find_section('.rodata')
    if failure == 'nonzero_tail':
        table.data = table.data[:4] + b'X' + table.data[5:]
    elif failure == 'tail_address':
        # the text's lo16 points at .rodata+8, inside the excluded tail
        text = obj.find_section('.text')
        word = struct.unpack('>I', text.data[4:8])[0]
        text.data = text.data[:4] + struct.pack('>I', (word & 0xffff0000) | 8) + text.data[8:]
    else:
        section_symbol = next(i for i, s in enumerate(obj.symtab.symbol_entries)
                              if s.type == E.STT_SECTION and s.st_shndx == obj.find_section('.text').index)
        records = obj.fmt.pack('II', 0, (section_symbol << 8) | E.R_MIPS_32)
        obj.add_section('.rel.rodata', E.SHT_REL, 0, obj.symtab.index, table.index, 4, 8, records)
    obj.symtab.data = b''.join(s.to_bin() for s in obj.symtab.symbol_entries)
    obj.write(fixture[1])
    before = fixture[1].read_bytes()
    with pytest.raises(O.OwnershipError):
        normalize(fixture)
    assert fixture[1].read_bytes() == before


@pytest.mark.parametrize('changes', [
    {'logical_kind': 'literal'}, {'logical_kind': None},
    {'logical_extent': False, 'alignment': 16, 'size': 16},
])
def test_logical_kind_is_validated(fixture, changes):
    repo, _, row, registry, original = fixture
    row.update(changes)
    if changes.get('size') == 16:
        row['sha256'] = hashlib.sha256(original[16:32]).hexdigest()
    save(repo, registry)
    with pytest.raises(O.OwnershipError, match='logical_kind'):
        O.rom_slots(repo)


def test_constants_split_link_reproduces_the_container(fixture):
    if not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('MIPS GNU binutils required')
    normalize(fixture)
    repo, obj, row, _, original = fixture
    container = repo / 'build/us/assets/us/data.o'
    O.split_container(repo, row['source'], repo / row['source'], container)
    script = '''SECTIONS {
 .text 0x80001000 : { build/us/src/rom/lib_1cf90.o(.text) }
 .data 0x8002D8F0 : SUBALIGN(16) {
  build/us/src/rom/lib_1cf90.o(.rodata);
  build/us/assets/us/data.o(.data);
 }
 /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }
}
'''
    rewritten = O.rewrite_linker(script, repo)
    assert '== 0x8002d900' in rewritten and '== 0x4' in rewritten
    (repo / 'model.ld').write_text(rewritten)
    cp = subprocess.run(['mips-linux-gnu-ld', '--accept-unknown-input-arch', '-T', 'model.ld', '-o', 'linked.elf',
                         str(obj.relative_to(repo)), str(container.relative_to(repo))],
                        cwd=repo, capture_output=True, text=True)
    assert cp.returncode == 0, cp.stderr
    out = repo / 'linked_data.bin'
    subprocess.run(['mips-linux-gnu-objcopy', '-O', 'binary', '-j', '.data', str(repo / 'linked.elf'), str(out)],
                   check=True)
    assert out.read_bytes() == original


class _Fmt:
    def unpack(self, fmt, data):
        return struct.unpack('>' + fmt, data)

    def pack(self, fmt, *args):
        return struct.pack('>' + fmt, *args)


class _Section:
    def __init__(self, sh_offset, data):
        self.sh_offset, self.data = sh_offset, data


def _hdrr(offsets):
    fields = [0x7009, 0x70a] + [0] * 23
    for i, v in zip(O._MDEBUG_OFFSET_FIELDS, offsets):
        fields[i] = v
    return struct.pack('>HHIIIIIIIIIIIIIIIIIIIIIII', *fields) + b'tail'


def test_mdebug_moved_backwards_keeps_absent_tables_at_zero():
    """lib_1cf90: trimming .rodata 16 -> 4 moves .mdebug back 8 bytes; its
    absent optimisation/relative-file tables have offset 0 (asm-processor's own
    relocation would make them negative and fail)."""
    offsets = [0x2ce8, 0x465c, 0x2fcc, 0x3618, 0, 0x3954, 0x3b00, 0x3d68, 0x4114, 0, 0x41ec]
    section = _Section(0x2c80, _hdrr(offsets))
    O._mdebug_relocator(section, _Fmt())(0x2c88)
    assert section.data == _hdrr([v - 8 if v else 0 for v in offsets])
    section = _Section(0x2c90, _hdrr(offsets))
    O._mdebug_relocator(section, _Fmt())(0x2c88)
    assert section.data == _hdrr([v + 8 if v else 0 for v in offsets])


def test_mdebug_relocator_refuses_a_bad_header():
    section = _Section(0, b'\0' * 0x64)
    with pytest.raises(O.OwnershipError, match='mdebug'):
        O._mdebug_relocator(section, _Fmt())(8)
