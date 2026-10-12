"""Own source-produced readonly slots inside existing ROM data containers.

Registry offsets describe real slots in ignored extracted assets. Splitting removes
those bytes from the linked container; the owner's actual section replaces them.
Passthrough companions and promotion are one TU transaction. No ROM score masks.
"""
import argparse
import hashlib
import importlib.util
import json
import os
import re
import subprocess
import tempfile
from pathlib import Path

REGISTRY = 'rom_owned_data.json'
# assets/us/data.bin is splat's `data` bin: ROM 0x283D0 (end of boot-segment
# CPU code, vram 0x800277D0) to 0xC00000.
DATA_ROM_START = 0x283D0
# Reserved source-built compressed game slot within assets/us/data.bin
# (ROM 0xB0CB10 - DATA_ROM_START). Checked against blob_rom/Makefile by tests;
# this build helper stays standalone.
SOURCE_BUILT_GAME_SLOT = 0xAE4740
SOURCE_BUILT_GAME_LENGTH = 326180
REPO = Path(__file__).resolve().parents[3]
IDENT = re.compile(r'[A-Za-z_]\w*\Z')


class OwnershipError(ValueError):
    pass


LOGICAL_KINDS = ('table', 'constants')


def _relative(value):
    if not isinstance(value, (str, Path)) or not re.fullmatch(r'[A-Za-z0-9_./-]+', str(value)):
        raise OwnershipError('unsafe ownership path characters')
    p = Path(value)
    if p.is_absolute() or '..' in p.parts or not p.parts:
        raise OwnershipError('ownership paths must be repository-relative')
    return p.as_posix()


def load_registry(repo=REPO):
    p = Path(repo) / REGISTRY
    if not p.exists():
        return {'schema': 1, 'rom_slots': [], 'storage_blocks': []}
    try:
        data = json.loads(p.read_text())
    except (OSError, ValueError) as exc:
        raise OwnershipError('unreadable ownership registry') from exc
    if not isinstance(data, dict):
        raise OwnershipError('ownership registry must be an object')
    if data.get('schema') != 1:
        raise OwnershipError('unsupported ownership registry schema')
    for k in ['rom_slots', 'storage_blocks']:
        if not isinstance(data.get(k, []), list):
            raise OwnershipError(k + ' must be a list')
        data.setdefault(k, [])
    return data


def rom_slots(repo=REPO, *, check_source=True):
    repo = Path(repo)
    result, owners, sections, containers = [], set(), set(), {}
    registry = load_registry(repo)
    bss = []
    for raw in registry['rom_slots']:
        if isinstance(raw, dict) and raw.get('owner_section') == BSS:
            r = _bss_slot(repo, raw)
            section = (r['owner_object'], r['owner_section'])
            if r['owner'] in owners or section in sections:
                raise OwnershipError('duplicate owner or input section')
            owners.add(r['owner']); sections.add(section)
            bss.append(r); result.append(r)
            continue
        required = {'owner', 'tu', 'source', 'offset', 'size', 'container_vram', 'sha256'}
        if isinstance(raw, dict) and raw.get('storage_owner'):
            storage_owners = [v for v in load_registry(repo)['storage_blocks'] if isinstance(v, dict) and v.get('owner') == raw['storage_owner']]
            if len(storage_owners) != 1 or not storage_owners[0].get('active') or storage_owners[0].get('activation_mode') != 'existing_tu' or storage_owners[0].get('data_slot') != raw:
                raise OwnershipError('mutable slot requires its active reviewed complete storage owner')
        else:
            required.add('passthrough_asm')
        if not isinstance(raw, dict) or not required.issubset(raw):
            raise OwnershipError('missing required ROM owner metadata')
        r = dict(raw)
        if not isinstance(r['owner'], str) or not IDENT.fullmatch(r['owner']):
            raise OwnershipError('invalid ROM owner name')
        # Other functions of the same TU whose bodies also emit into the owned
        # section (__fixunssfdi beside __fixunsdfdi). The slot changes hands
        # only when the owner and every co-owner are promoted together.
        co = r.get('co_owners', [])
        if (not isinstance(co, list) or any(not isinstance(n, str) or not IDENT.fullmatch(n) for n in co)
                or len(set(co)) != len(co) or r['owner'] in co):
            raise OwnershipError('co_owners must be distinct function names other than the owner')
        if co and r.get('storage_owner'):
            raise OwnershipError('mutable storage slots have no co-owners')
        r['co_owners'] = list(co)
        for k in ['tu', 'source'] + ([] if r.get('storage_owner') else ['passthrough_asm']):
            r[k] = _relative(r[k])
            if not (repo / r[k]).resolve().is_relative_to(repo.resolve()):
                raise OwnershipError('ownership path resolves outside repository')
        if not r['tu'].startswith('src/rom/') or not r['tu'].endswith('.c'):
            raise OwnershipError('ROM owner must identify a ROM TU')
        if not r['source'].startswith('assets/') or not r['source'].endswith('.bin'):
            raise OwnershipError('ROM container must identify an extracted binary asset')
        if r.get('composed_source') is not None:
            r['composed_source'] = _relative(r['composed_source'])
            if not (repo / r['composed_source']).resolve().is_relative_to(repo.resolve()):
                raise OwnershipError('composed input path resolves outside repository')
        r['owner_object'] = 'build/us/' + str(Path(r['tu']).with_suffix('.o'))
        r['container_object'] = 'build/us/' + str(Path(r['source']).with_suffix('.o'))
        r.setdefault('container_section', '.data')
        r.setdefault('owner_section', '.rodata')
        for k in ['container_section', 'owner_section']:
            if not isinstance(r[k], str) or not re.fullmatch(r'\.(?:data|rodata)(?:\.[A-Za-z_]\w*)*', r[k]):
                raise OwnershipError('only owned data/readonly sections are supported')
        for k in ['offset', 'size', 'container_vram']:
            if isinstance(r.get(k), str):
                try:
                    r[k] = int(r[k], 0)
                except ValueError as exc:
                    raise OwnershipError('invalid ownership integer') from exc
            if type(r.get(k)) is not int or r[k] < 0:
                raise OwnershipError('nonnegative integer required for ' + k)
        alignment = r.get('alignment', 16)
        logical = r.get('logical_extent', False)
        if type(alignment) is not int or alignment not in (4, 16) or type(logical) is not bool:
            raise OwnershipError('owned alignment must be 4 or 16, logical extent must be boolean')
        if alignment != 16 and not logical:
            raise OwnershipError('4-byte ownership requires an explicit logical table extent')
        if logical and (alignment != 4 or r['owner_section'] != '.rodata' or r.get('storage_owner')):
            raise OwnershipError('logical extents support only complete 4-byte readonly tables')
        # A logical extent is either a word-relocated jump table (default) or,
        # with logical_kind "constants", relocation-free literal words (a
        # float constant the TU's other readonly words follow, w21i).
        if 'logical_kind' in r and (not logical or r['logical_kind'] not in LOGICAL_KINDS):
            raise OwnershipError('logical_kind must be "table" or "constants" on a logical extent')
        if logical:
            r['logical_kind'] = r.get('logical_kind', 'table')
        if not r['size'] or r['offset'] % alignment or r['size'] % alignment:
            raise OwnershipError('owned slots must occupy complete aligned sections')
        if logical and (r['container_vram'] + r['offset']) % alignment:
            raise OwnershipError('logical table address must satisfy its actual alignment')
        if r['container_vram'] + r['offset'] + r['size'] > 0x100000000:
            raise OwnershipError('owned slot address overflows MIPS image')
        if not isinstance(r['sha256'], str) or not re.fullmatch(r'[0-9a-f]{64}', r['sha256']):
            raise OwnershipError('owned slot requires exact original SHA-256')
        section = (r['owner_object'], r['owner_section'])
        if r['owner'] in owners or section in sections:
            raise OwnershipError('duplicate owner or input section')
        owners.add(r['owner']); sections.add(section)
        group = containers.setdefault(r['source'], [])
        if group and (group[0]['container_section'], group[0]['container_vram']) != (r['container_section'], r['container_vram']):
            raise OwnershipError('inconsistent container ownership')
        # This region is already produced from game C sources by blob_rom;
        # ownership cannot replace any part of that compressed source image.
        if r['source'] == 'assets/us/data.bin':
            if r['offset'] < SOURCE_BUILT_GAME_SLOT + SOURCE_BUILT_GAME_LENGTH and SOURCE_BUILT_GAME_SLOT < r['offset'] + r['size']:
                raise OwnershipError('owned slot overlaps source-built compressed game blob')
        group.append(r); result.append(r)
    for source, rows in containers.items():
        rows.sort(key=lambda r: r['offset'])
        for a, b in zip(rows, rows[1:]):
            if a['offset'] + a['size'] > b['offset']:
                raise OwnershipError('owned slots overlap')
        if check_source:
            p = repo / source
            if not p.resolve().is_relative_to(repo.resolve()):
                raise OwnershipError('source resolves outside repository')
            try:
                data = p.read_bytes()
            except OSError as exc:
                raise OwnershipError('owned source container is missing or unreadable') from exc
            for r in rows:
                if r['offset'] + r['size'] > len(data):
                    raise OwnershipError('owned slot exceeds source bounds')
                if hashlib.sha256(data[r['offset']:r['offset'] + r['size']]).hexdigest() != r['sha256']:
                    raise OwnershipError('owned slot original-byte identity changed')
    _bss_overlaps(bss, registry['storage_blocks'])
    return sorted(result, key=lambda r: (r['source'] or '', r['container_vram'] + r['offset']))


# A .bss slot (w21d) owns a TU's whole zero-initialised .bss at a fixed RAM
# address. It has no ROM bytes, so no container, original-byte hash or split:
# the proof is the linked address/size ASSERTs (rewrite_linker) plus the full
# ROM SHA-1 of the code that addresses it. Row fields: owner, co_owners, tu,
# owner_section/container_section ".bss", container_vram (the block address),
# offset 0, size, passthrough_asm (the companion: `.space size` while the
# members are passthrough, so the block is occupied in both states).
BSS = '.bss'
BSS_FIELDS = {'owner', 'co_owners', 'tu', 'owner_section', 'container_section', 'container_vram',
              'offset', 'size', 'alignment', 'passthrough_asm', 'comment'}


def _bss_slot(repo, raw):
    if not {'owner', 'tu', 'container_vram', 'offset', 'size', 'passthrough_asm'}.issubset(raw):
        raise OwnershipError('missing required .bss slot metadata')
    extra = set(raw) - BSS_FIELDS
    if extra:
        raise OwnershipError('.bss slot has no ROM container fields: ' + ', '.join(sorted(extra)))
    r = dict(raw)
    if not isinstance(r['owner'], str) or not IDENT.fullmatch(r['owner']):
        raise OwnershipError('invalid ROM owner name')
    co = r.get('co_owners', [])
    if (not isinstance(co, list) or any(not isinstance(n, str) or not IDENT.fullmatch(n) for n in co)
            or len(set(co)) != len(co) or r['owner'] in co):
        raise OwnershipError('co_owners must be distinct function names other than the owner')
    r['co_owners'] = list(co)
    for k in ('tu', 'passthrough_asm'):
        r[k] = _relative(r[k])
        if not (Path(repo) / r[k]).resolve().is_relative_to(Path(repo).resolve()):
            raise OwnershipError('ownership path resolves outside repository')
    if not r['tu'].startswith('src/rom/') or not r['tu'].endswith('.c'):
        raise OwnershipError('ROM owner must identify a ROM TU')
    if r.setdefault('container_section', BSS) != BSS:
        raise OwnershipError('a .bss slot is its own container')
    for k in ['offset', 'size', 'container_vram']:
        if isinstance(r.get(k), str):
            try:
                r[k] = int(r[k], 0)
            except ValueError as exc:
                raise OwnershipError('invalid ownership integer') from exc
        if type(r.get(k)) is not int or r[k] < 0:
            raise OwnershipError('nonnegative integer required for ' + k)
    if r.setdefault('alignment', 16) != 16:
        raise OwnershipError('.bss slots own complete 16-byte aligned sections')
    if r['offset'] != 0:
        raise OwnershipError('a .bss slot starts at its own address (offset 0)')
    if not r['size'] or r['size'] % 16 or r['container_vram'] % 16:
        raise OwnershipError('owned slots must occupy complete aligned sections')
    if not 0x80000000 <= r['container_vram'] or r['container_vram'] + r['size'] > 0x100000000:
        raise OwnershipError('.bss slot must lie in KSEG0/KSEG1 RAM')
    r['source'] = None
    r['owner_object'] = 'build/us/' + str(Path(r['tu']).with_suffix('.o'))
    r['container_object'] = None
    return r


def _bss_overlaps(bss, storage_blocks):
    ranges = [(r['container_vram'], r['container_vram'] + r['size'], r['owner']) for r in bss]
    for row in storage_blocks:
        if isinstance(row, dict) and 'vram_start' in row and 'size' in row:
            start = int(row['vram_start'], 0) if isinstance(row['vram_start'], str) else row['vram_start']
            size = int(row['size'], 0) if isinstance(row['size'], str) else row['size']
            ranges.append((start, start + size, row.get('owner')))
    ranges.sort()
    for a, b in zip(ranges, ranges[1:]):
        if a[1] > b[0]:
            raise OwnershipError('owned .bss slot overlaps another storage block: %s, %s' % (a[2], b[2]))
    tus = [r['tu'] for r in bss]
    if len(set(tus)) != len(tus) or any(isinstance(s, dict) and s.get('tu') in tus for s in storage_blocks):
        raise OwnershipError('a TU has one owned .bss block')


def is_bss(row):
    return row.get('owner_section') == BSS


def _groups(repo):
    groups = {}
    for r in rom_slots(repo):
        if is_bss(r):
            continue
        groups.setdefault(r['source'], []).append(r)
    return groups


def _elf_processor(repo):
    """Use the existing project MIPS ELF reader/writer, including debug fixups."""
    path = Path(repo) / 'tools/asm-processor/asm_processor.py'
    spec = importlib.util.spec_from_file_location('_owned_data_elf', path)
    if spec is None or spec.loader is None:
        raise OwnershipError('missing project ELF processor')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def _relocation_symbol(obj, relocation):
    if not 0 <= relocation.sym_index < len(obj.symtab.symbol_entries):
        raise OwnershipError('relocation symbol index is outside the symbol table')
    return obj.symtab.symbol_entries[relocation.sym_index]


def _logical_table(obj, section, size, elf, kind='table'):
    """Validate a complete word-relocated readonly table before trimming its tail.

    kind "table" is the proven 4-byte jump-table case: every retained word is a
    relocated pointer into source-built text. kind "constants" (w21i) is a
    readonly section of literal words with no relocations at all (IDO pads a
    lone float literal to 16 bytes; the TU's other readonly words follow it in
    retail). It changes no instruction, table word, relocation, meaningful
    symbol, section index or symbol index.
    """
    alignment = section.sh_addralign
    if (section.name != '.rodata' or section.sh_type != elf.SHT_PROGBITS or
            section.sh_flags != elf.SHF_ALLOC or section.sh_addr != 0 or
            alignment not in (4, 16) or size <= 0 or size % 4 or
            len(section.data) != section.sh_size or
            section.sh_size != (size + alignment - 1) // alignment * alignment):
        raise OwnershipError('malformed logical table geometry or section kind')
    if any(section.data[size:]):
        raise OwnershipError('excluded section tail contains nonzero bytes')
    table_relocations = []
    if kind == 'constants' and any(reltab.relocations for reltab in section.relocated_by):
        raise OwnershipError('logical constants extent must carry no relocations')
    for reltab in section.relocated_by:
        for rel in reltab.relocations:
            if rel.r_offset % 4 or rel.r_offset < 0 or rel.r_offset + 4 > size:
                raise OwnershipError('relocation falls in excluded table extent')
            if reltab.sh_type != elf.SHT_REL or rel.rel_type != elf.R_MIPS_32:
                raise OwnershipError('logical table requires actual 32-bit word relocations')
            symbol = _relocation_symbol(obj, rel)
            if not 0 < symbol.st_shndx < len(obj.sections):
                raise OwnershipError('logical table entry has no source-built text target')
            target = obj.sections[symbol.st_shndx]
            addend = obj.fmt.unpack('I', section.data[rel.r_offset:rel.r_offset + 4])[0]
            address = symbol.st_value + addend
            if target.name != '.text' or address % 4 or not 0 <= address < len(target.data):
                raise OwnershipError('logical table entry points outside source-built text')
            table_relocations.append(rel.r_offset)
    if kind == 'table' and sorted(table_relocations) != list(range(0, size, 4)):
        raise OwnershipError('logical table must retain every complete relocated word once')
    descriptive = []
    for symbol in obj.symtab.symbol_entries:
        if symbol.st_shndx != section.index:
            continue
        if symbol.type == elf.STT_SECTION:
            if (symbol.bind != elf.STB_LOCAL or symbol.st_value != 0 or
                    symbol.st_size not in (0, section.sh_size, size)):
                raise OwnershipError('malformed descriptive section symbol')
            descriptive.append(symbol)
        elif symbol.st_value >= size or symbol.st_value + symbol.st_size > size:
            raise OwnershipError('meaningful symbol falls in excluded table extent')

    # An address relocation elsewhere can refer to the section plus an addend.
    # Retain all such records, but reject a pointer to an excluded table byte.
    for reltab in obj.sections:
        if not reltab.is_rel():
            continue
        pending_hi = {}
        for rel in reltab.relocations:
            symbol = _relocation_symbol(obj, rel)
            if symbol.st_shndx != section.index or rel.rel_type == 0:
                continue
            def check(addend):
                if not 0 <= symbol.st_value + addend < size:
                    raise OwnershipError('address relocation refers to excluded table extent')
            if reltab.sh_type == elf.SHT_RELA:
                check(rel.r_addend)
                continue
            payload = reltab.rel_target.data
            if rel.r_offset % 4 or rel.r_offset + 4 > len(payload):
                raise OwnershipError('malformed table address relocation')
            word = obj.fmt.unpack('I', payload[rel.r_offset:rel.r_offset + 4])[0]
            if rel.rel_type == elf.R_MIPS_32:
                check(word)
            elif rel.rel_type == elf.R_MIPS_HI16:
                pending_hi.setdefault(rel.sym_index, []).append(word & 0xffff)
            elif rel.rel_type == elf.R_MIPS_LO16:
                low = (word & 0xffff) - (0x10000 if word & 0x8000 else 0)
                for high in pending_hi.pop(rel.sym_index, [0]):
                    check((high << 16) + low)
            else:
                raise OwnershipError('unsupported address relocation to logical table')
        if pending_hi:
            raise OwnershipError('unpaired high relocation to logical table')
    return descriptive


# HDRR file-offset fields of an IDO .mdebug symbolic header (bytes 0..0x60,
# 'HHIIIIIIIIIIIIIIIIIIIIIII'): cbLineOffset .. cbExtOffset, as asm-processor
# relocates them.
_MDEBUG_OFFSET_FIELDS = (4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24)


def _mdebug_relocator(section, fmt):
    """asm-processor shifts every HDRR offset when .mdebug moves, including the
    zero offsets of absent tables (ioptMax/crfd = 0). Trimming a logical extent
    moves a later .mdebug BACKWARDS (lib_1cf90: .rodata 16 -> 4, .options
    re-aligns, .mdebug -8), so 0 - 8 overflows the unsigned pack. Shift only the
    present (nonzero) offsets; an absent table stays 0."""
    def relocate(original_offset):
        shift = section.sh_offset - original_offset
        header = list(fmt.unpack('HHIIIIIIIIIIIIIIIIIIIIIII', section.data[0:0x60]))
        if header[0] != 0x7009:
            raise OwnershipError('invalid .mdebug symbolic header')
        for i in _MDEBUG_OFFSET_FIELDS:
            if header[i]:
                header[i] += shift
                if not 0 < header[i] < 1 << 32:
                    raise OwnershipError('.mdebug offset relocation out of range')
        section.data = fmt.pack('HHIIIIIIIIIIIIIIIIIIIIIII', *header) + section.data[0x60:]
    return relocate


def normalize_object_sections(repo, tu, object_path):
    """Apply explicit logical table extents after normal asm post-processing.

    Ordinary existing 16-byte ownership rows are byte-for-byte no-ops. Extent
    validation precedes any write; serialization uses the existing processor
    and an atomic replace. This is a build operation, never a scoring mask.
    """
    repo = Path(repo)
    tu = _relative(tu)
    rows = [r for r in rom_slots(repo, check_source=False)
            if r['tu'] == tu and r.get('logical_extent')]
    if not rows:
        return []
    path = Path(object_path)
    if not path.is_absolute():
        path = repo / path
    if not path.resolve().is_relative_to(repo.resolve()):
        raise OwnershipError('owned object path resolves outside repository')
    if path.is_symlink() or any(path.resolve() != (repo / r['owner_object']).resolve() for r in rows):
        raise OwnershipError('object does not correspond to the registry-derived owner object')
    # Ordinary ROM TUs do not read large ignored containers. A matching logical
    # owner still receives the full original-byte/overlap/source-built-blob guards.
    rows = [r for r in rom_slots(repo) if r['tu'] == tu and r.get('logical_extent')]
    elf = _elf_processor(repo)
    try:
        obj = elf.ElfFile(path.read_bytes())
    except (AssertionError, OSError, IndexError, ValueError) as exc:
        raise OwnershipError('invalid owned MIPS relocatable object') from exc
    changes = []
    # Finish every validation before changing even the in-memory ELF model.
    for row in rows:
        sections = [s for s in obj.sections if s.name == row['owner_section']]
        if len(sections) != 1:
            raise OwnershipError('expected exactly one owned logical input section')
        section = sections[0]
        descriptive = _logical_table(obj, section, row['size'], elf, row['logical_kind'])
        changes.append((row, section, descriptive))
    if all(s.sh_size == r['size'] and s.sh_addralign == r['alignment']
           for r, s, _ in changes):
        return []
    relocations = {s.index: s.data for s in obj.sections if s.is_rel()}
    for row, section, descriptive in changes:
        for symbol in descriptive:
            if symbol.st_size == section.sh_size:
                symbol.st_size = row['size']
        section.data = section.data[:row['size']]
        section.sh_size = row['size']
        section.sh_addralign = row['alignment']
    obj.symtab.data = b''.join(symbol.to_bin() for symbol in obj.symtab.symbol_entries)
    for section in obj.sections:
        if section.sh_type == elf.SHT_MIPS_DEBUG:
            section.relocate_mdebug = _mdebug_relocator(section, obj.fmt)
    fd, temporary = tempfile.mkstemp(prefix=path.name + '.extent-', dir=path.parent)
    os.close(fd)
    try:
        obj.write(temporary)
        replay = elf.ElfFile(Path(temporary).read_bytes())
        if any(replay.sections[i].data != data for i, data in relocations.items()):
            raise OwnershipError('ELF serialization changed a retained relocation')
        if [s.to_bin() for s in replay.symtab.symbol_entries] != [s.to_bin() for s in obj.symtab.symbol_entries]:
            raise OwnershipError('ELF serialization changed a retained symbol')
        os.chmod(temporary, path.stat().st_mode & 0o777)
        os.replace(temporary, path)
    finally:
        if Path(temporary).exists():
            Path(temporary).unlink()
    return [{'owner': r['owner'], 'section': s.name, 'size': r['size'],
             'alignment': r['alignment']} for r, s, _ in changes]


def _slices(rows, length):
    cursor = 0
    for i, r in enumerate(rows):
        yield i, cursor, r['offset']
        cursor = r['offset'] + r['size']
    yield len(rows), cursor, length


def section_name(rows, i):
    return rows[0]['container_section'] + '.owned' + str(i)


def split_container(repo, source, composed, output, *, objcopy='mips-linux-gnu-objcopy'):
    """Remove owned bytes, retaining only actual prefix/inter-slot/suffix inputs."""
    source = _relative(source); rows = _groups(repo).get(source, [])
    data = Path(composed).read_bytes(); original = (Path(repo) / source).read_bytes()
    if len(data) != len(original):
        raise OwnershipError('composed container size changed')
    if not rows:
        subprocess.run([objcopy, '-I', 'binary', '-O', 'elf32-big', str(composed), str(output)], check=True)
        return []
    # Also check the composed range, so an unrelated overlay cannot hide an owner.
    for r in rows:
        if hashlib.sha256(data[r['offset']:r['offset'] + r['size']]).hexdigest() != r['sha256']:
            raise OwnershipError('composed input altered an owned slot')
    output = Path(output); output.parent.mkdir(parents=True, exist_ok=True)
    chunks = []
    for i, start, end in _slices(rows, len(data)):
        chunk = output.with_name(output.name + '.owned' + str(i) + '.bin')
        chunk.write_bytes(data[start:end]); chunks.append((section_name(rows, i), chunk))
    nonempty = [(name, chunk) for name, chunk in chunks if chunk.stat().st_size]
    if nonempty:
        first_name, first = nonempty[0]
        subprocess.run([objcopy, '-I', 'binary', '-O', 'elf32-big', '--rename-section', '.data=' + first_name, str(first), str(output)], check=True)
    else:
        # A completely owned container still needs a valid empty ELF input.
        # The bootstrap byte is removed before linking; no fake payload remains.
        empty = output.with_name(output.name + '.empty.bin'); empty.write_bytes(b'\0')
        subprocess.run([objcopy, '-I', 'binary', '-O', 'elf32-big', str(empty), str(output)], check=True)
        subprocess.run([objcopy, '-I', 'elf32-big', '-O', 'elf32-big', '--remove-section', '.data', str(output)], check=True)
    for name, chunk in nonempty[1:]:
        subprocess.run([objcopy, '-I', 'elf32-big', '-O', 'elf32-big', '--add-section', name + '=' + str(chunk), '--set-section-flags', name + '=alloc,load,data,contents', str(output)], check=True)
    return [str(p) for _, p in chunks]


def rewrite_linker(text, repo=REPO):
    """Apply ownership to SPLAT output, idempotently, with fixed-address/size gates."""
    # Restore ALL generated blocks, even if rows were removed from registry.
    text = re.sub(r'/\* ROM_OWNED_SECTION_MOVED ([^\n]+) \*/', r'\1;', text)
    pattern = r'/\* ROM_OWNED_DATA_BEGIN ([A-Za-z0-9_./-]+\(\.[A-Za-z0-9_.]+\)) \*/.*?/\* ROM_OWNED_DATA_END \1 \*/'
    text = re.sub(pattern, lambda m: m.group(1) + ';', text, flags=re.S)
    text = re.sub(r'/\* ROM_OWNED_SUBALIGN (SUBALIGN\(16\)) \*/', r'\1', text)
    text = re.sub(r'\n[ \t]*/\* ROM_OWNED_BSS_BEGIN (\w+) \*/.*?/\* ROM_OWNED_BSS_END \1 \*/', '', text, flags=re.S)
    text = _rewrite_bss(text, [r for r in rom_slots(repo, check_source=False) if is_bss(r)])
    for source, rows in _groups(repo).items():
        obj = rows[0]['container_object']; section = rows[0]['container_section']
        original = obj + '(' + section + ');'
        for r in rows:
            owned = r['owner_object'] + '(' + r['owner_section'] + ')'
            if text.count(owned + ';') != 1:
                raise OwnershipError('expected exactly one original owner-section linker input')
            text = text.replace(owned + ';', '/* ROM_OWNED_SECTION_MOVED ' + owned + ' */')
        if text.count(original) != 1:
            raise OwnershipError('expected exactly one original container linker input')
        if any(r.get('logical_extent') for r in rows):
            # SPLAT's output-wide override supersedes even a validated input
            # section's actual four-byte alignment. Honor input alignments for
            # this container; fixed slot and size assertions still apply to
            # every owner. Preserve the override in a reversible marker.
            blocks = re.compile(
                r'(?m)^(?P<header>[ \t]*\.[A-Za-z0-9_.]+[^\n{}]*)'
                r'(?P<opening>\s*\{)(?P<body>[^{}]*)(?P<closing>\})')
            found = [m for m in blocks.finditer(text) if original in m['body']]
            if len(found) != 1:
                raise OwnershipError('expected exactly one logical container output section')
            block = found[0]
            header = block['header']
            if 'SUBALIGN' in header:
                if not re.search(r'SUBALIGN\(16\)', header):
                    raise OwnershipError('unexpected logical container subalignment')
                header = header.replace('SUBALIGN(16)',
                                        '/* ROM_OWNED_SUBALIGN SUBALIGN(16) */')
                text = text[:block.start('header')] + header + text[block.end('header'):]
        indent = re.search(r'(?m)^([ \t]*)' + re.escape(original), text).group(1)
        marker = obj + '(' + section + ')'
        lines = ['/* ROM_OWNED_DATA_BEGIN ' + marker + ' */']
        binary_names = {re.sub(r'[^A-Za-z0-9]', '_', name) for name in [source, rows[0].get('composed_source', source)]}
        lines += ['_binary_' + name + '_start = ABSOLUTE(.);' for name in sorted(binary_names)]
        for i, r in enumerate(rows):
            start = 'rom_owned_' + r['owner'] + '_START'; end = 'rom_owned_' + r['owner'] + '_END'
            lines += [obj + '(' + section_name(rows, i) + ');', start + ' = ABSOLUTE(.);', r['owner_object'] + '(' + r['owner_section'] + ');', end + ' = ABSOLUTE(.);', 'ASSERT(' + start + ' == ' + hex(r['container_vram'] + r['offset']) + ', "owned ROM slot address changed");', 'ASSERT(' + end + ' - ' + start + ' == ' + hex(r['size']) + ', "owned ROM slot size changed");']
            if r.get('logical_extent'):
                lines += ['ASSERT(' + start + ' % ' + str(r['alignment']) + ' == 0, "owned table alignment changed");']
        lines += [obj + '(' + section_name(rows, len(rows)) + ');']
        for name in sorted(binary_names):
            lines += ['_binary_' + name + '_end = ABSOLUTE(.);', '_binary_' + name + '_size = ABSOLUTE(_binary_' + name + '_end - _binary_' + name + '_start);']
        lines += ['/* ROM_OWNED_DATA_END ' + marker + ' */']
        text = text.replace(original, ('\n' + indent).join(lines))
    from . import owned_text
    return owned_text.rewrite_linker(text, repo)


BSS_ANCHOR = 'data_VRAM_END = .;'


def _rewrite_bss(text, rows):
    """Move each .bss slot TU's .bss input out of SPLAT's bss output section
    into its own fixed-address NOLOAD section, with address/size ASSERTs."""
    if not rows:
        return text
    if text.count(BSS_ANCHOR) != 1:
        raise OwnershipError('expected exactly one ' + BSS_ANCHOR + ' anchor for owned .bss slots')
    indent = re.search(r'(?m)^([ \t]*)' + re.escape(BSS_ANCHOR), text).group(1)
    blocks = []
    for r in sorted(rows, key=lambda r: r['container_vram']):
        owned = r['owner_object'] + '(.bss)'
        if text.count(owned + ';') != 1:
            raise OwnershipError('expected exactly one original owner .bss linker input')
        text = text.replace(owned + ';', '/* ROM_OWNED_SECTION_MOVED ' + owned + ' */')
        start = 'rom_owned_' + r['owner'] + '_START'; end = 'rom_owned_' + r['owner'] + '_END'
        blocks += ['/* ROM_OWNED_BSS_BEGIN ' + r['owner'] + ' */',
                   '.rom_owned_bss_' + r['owner'] + ' ' + hex(r['container_vram']) + ' (NOLOAD) : SUBALIGN(16)',
                   '{', '    ' + start + ' = ABSOLUTE(.);', '    ' + owned + ';',
                   '    ' + end + ' = ABSOLUTE(.);',
                   '    ASSERT(' + start + ' == ' + hex(r['container_vram']) + ', "owned BSS slot address changed");',
                   '    ASSERT(' + end + ' - ' + start + ' == ' + hex(r['size']) + ', "owned BSS slot size changed");',
                   '}', '/* ROM_OWNED_BSS_END ' + r['owner'] + ' */']
    return text.replace(BSS_ANCHOR, BSS_ANCHOR + ''.join('\n' + indent + line for line in blocks), 1)


def companion_block(row):
    return ('/* ROM_OWNED_RODATA_BEGIN ' + row['owner'] + ' */\n'
            '#pragma GLOBAL_ASM("' + row['passthrough_asm'] + '")\n'
            '/* ROM_OWNED_RODATA_END ' + row['owner'] + ' */\n')


def companions_for_tu(repo, tu, states):
    return ''.join(companion_block(r) for r in rom_slots(repo)
                   if not r.get('storage_owner') and r['tu'] == _relative(tu) and states.get(r['owner']) == 'passthrough')


def strip_companion(text, owner, repo=REPO, tu=None):
    rows = [r for r in rom_slots(repo) if r['owner'] == owner]
    if not rows:
        return text
    r = rows[0]
    if tu is not None and r['tu'] != _relative(tu):
        raise OwnershipError('owner belongs to another TU')
    if r.get('storage_owner'):
        raise OwnershipError('mutable storage slot uses whole existing-TU activation, no companion splice')
    block = companion_block(r)
    if text.count(block) != 1:
        raise OwnershipError('expected exactly one owned passthrough companion')
    return text.replace(block, '', 1)


def section_fits(row, size):
    """True iff a compiled object's owned section of `size` bytes fills `row`'s
    slot. IDO pads a section to its 16-byte alignment; a logical-extent row
    (4-byte table) is trimmed back to its size at build time
    (normalize_object_sections, which also refuses a nonzero tail), so its
    padded size is admitted too."""
    if size <= row['size']:
        return True
    return bool(row.get('logical_extent')) and size <= (row['size'] + 15) // 16 * 16


def slot_members(row):
    """Functions whose promotion together hands the slot to compiled C."""
    return [row['owner']] + list(row.get('co_owners', []))


def passthrough_line(row, member):
    return '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/' + Path(row['tu']).stem + '/' + member + '.s")'


def pending_members(row, text):
    """The slot's members whose passthrough line is still in the TU `text`:
    every member of a prepared slot, only the new members of a replaced slot
    (`replace_slot`) whose earlier members are already C."""
    return [m for m in slot_members(row) if passthrough_line(row, m) in text]


REPLACE_SAME = ('tu', 'source', 'owner_section', 'container_section', 'container_vram', 'offset',
                'passthrough_asm')
# A .bss block may also grow at its front (w23i): its address moves down, so
# only these are kept; the old range must lie inside the new one.
REPLACE_SAME_BSS = ('tu', 'owner_section', 'container_section', 'offset', 'passthrough_asm')


def _grows(old, row):
    """True iff `row` is a strictly larger replacement of `old` (see replace_slot)."""
    if row['size'] <= old['size']:
        return False
    if is_bss(row):
        return (all(old[k] == row[k] for k in REPLACE_SAME_BSS)
                and row['container_vram'] <= old['container_vram']
                and old['container_vram'] + old['size'] <= row['container_vram'] + row['size'])
    return all(old[k] == row[k] for k in REPLACE_SAME) and not row.get('logical_extent')


def replace_slot(repo, raw, linker='rush2049.us.ld'):
    """Supersede a promoted owner's slot by the larger co-owned slot
    its TU's complete section fills once the new members are C (w22i).

    func_80023BDC owns the first 0x14 bytes of lib_22300's .rodata (a 4-byte
    logical table); func_80023E9C's table follows. As C, both tables are one
    0x1E0 section. There is no hash-neutral passthrough state for the grown
    slot: asm-processor emits a GLOBAL_ASM block's .rodata as a file-scope
    `const char[]`, which IDO places before every function's jump table, so a
    companion carrying only the new member's words lands AHEAD of the owner's
    compiled table (measured: ROM differs at the slot and at the owner's
    `lw`). So the replacement is part of the new members' promotion
    transaction (promote_batch --replace-slot): this writes the row (in place
    of the owner's), its companion file (the plain original bytes; the TU gets
    no companion block) and the re-rendered linker script, and returns
    {path: original text} for the caller to restore unless the full-ROM gate
    of the promoted set passes and the paths are committed with it.

    A `.bss` block (w23i) grows the same way, and may also grow at its
    FRONT: lib_16320's block of func_80016E68 and two co-owners (0x80042670,
    0x30) becomes the TU's whole .bss once the function-local statics of
    func_80016C20/80016CF0 (below it) and func_80017040 (above it) are C
    (0x80042630, 0x80). IDO 7.1 lays a TU's .bss out in definition order,
    so there is no hash-neutral passthrough state for the grown block either
    (the C members' statics would sit at its start). The old range must lie
    inside the new one; the new range must not overlap another owned .bss
    block or storage block (rom_slots); the linker ASSERTs pin the block's
    address and size and the full-ROM SHA-1 of the promoted set proves every
    member's statics (old ones included) at their retail addresses.

    Refused unless: the owner has exactly one registered row; the new row
    keeps its TU, container, section, offset and companion path and grows it
    with no logical extent (a .bss row: keeps its TU, section and companion
    path and covers the old range); every member of the old slot is in the
    new one and already C (no passthrough line, no companion block); every
    new member is still passthrough; and the new row validates (original-byte
    SHA-256, overlap, alignment)."""
    repo = Path(repo)
    if not isinstance(raw, dict) or raw.get('storage_owner'):
        raise OwnershipError('replace_slot grows passthrough-companion slots only')
    registry = load_registry(repo)
    index = [i for i, r in enumerate(registry['rom_slots'])
             if isinstance(r, dict) and r.get('owner') == raw.get('owner')]
    if len(index) != 1:
        raise OwnershipError('replace_slot needs exactly one registered row of the owner')
    old = next(r for r in rom_slots(repo, check_source=False) if r['owner'] == raw['owner'])
    if is_bss(old) != (raw.get('owner_section') == BSS):
        raise OwnershipError('the replacement must keep the slot\'s section')
    registry['rom_slots'][index[0]] = raw
    paths = [repo / REGISTRY, repo / linker]
    originals = {p: p.read_text() for p in paths}
    paths[0].write_text(json.dumps(registry, indent=2) + '\n')
    try:
        row = next(r for r in rom_slots(repo) if r['owner'] == raw['owner'])
        if not _grows(old, row):
            raise OwnershipError('the replacement must grow the same slot (TU, container, section, offset, '
                                 'companion path; a .bss block over its old range) to a complete section')
        text = (repo / row['tu']).read_text()
        if any(m not in slot_members(row) for m in slot_members(old)):
            raise OwnershipError('the replacement drops a member of the superseded slot')
        if pending_members(old, text) or companion_block(old) in text:
            raise OwnershipError('the superseded slot is not fully promoted')
        new = [m for m in slot_members(row) if m not in slot_members(old)]
        if pending_members(row, text) != new or any(text.count(passthrough_line(row, m)) != 1 for m in new):
            raise OwnershipError('every new member of the replacement must still be passthrough')
        companion = repo / row['passthrough_asm']
        originals[companion] = companion.read_text() if companion.exists() else None
        source = companion_source(row, repo)
        rendered = rewrite_linker(paths[1].read_text(), repo)
    except BaseException:
        paths[0].write_text(originals[paths[0]])
        raise
    companion.write_text(source)
    paths[1].write_text(rendered)
    return row, originals


def restore(originals):
    """Undo `replace_slot` (None = the file did not exist)."""
    for path, text in originals.items():
        if text is None:
            if Path(path).exists():
                Path(path).unlink()
        else:
            Path(path).write_text(text)


def slots_for_function(repo, func, tu=None):
    """Passthrough-companion slots `func` owns or co-owns (in `tu`, if given)."""
    return [r for r in rom_slots(repo, check_source=False)
            if not r.get('storage_owner') and func in slot_members(r)
            and (tu is None or r['tu'] == _relative(tu))]


def companion_source(row, repo=REPO):
    """The passthrough companion: the slot's original bytes in the owner's
    section. asm-processor accepts `.balign 4`; the section keeps IDO's
    16-byte alignment, which the slot's 16-aligned offset satisfies.

    A logical-extent (4-byte) table is validated on every build as a complete
    word-relocated table (`_logical_table`), passthrough included, so its
    companion spells each original word as `.word OWNER + offset` into the
    owner's passthrough body (as fcvt.table.s does); every word must land in
    that body. A logical extent of kind "constants" has no relocations, so its
    companion is the plain `.incbin` of the original words. The linked bytes
    are the original ones either way."""
    if is_bss(row):
        # No ROM bytes: reserve the block so it is occupied in both states.
        return '.section .bss\n.balign 4\n.space ' + hex(row['size']) + '\n'
    head = '.section ' + row['owner_section'] + '\n.balign 4\n'
    if not row.get('logical_extent') or row.get('logical_kind', 'table') == 'constants':
        return head + '.incbin "' + row['source'] + '", ' + hex(row['offset']) + ', ' + hex(row['size']) + '\n'
    start, size = _passthrough_extent(repo, row)
    data = (Path(repo) / row['source']).read_bytes()[row['offset']:row['offset'] + row['size']]
    if len(data) != row['size']:
        raise OwnershipError('owned slot exceeds source bounds')
    lines = []
    for i in range(0, row['size'], 4):
        word = int.from_bytes(data[i:i + 4], 'big')
        if not start <= word < start + size or (word - start) % 4:
            raise OwnershipError('logical table word %#010x is not an instruction of %s' % (word, row['owner']))
        lines.append('.word ' + row['owner'] + ' + ' + hex(word - start))
    return head + '\n'.join(lines) + '\n'


def _passthrough_extent(repo, row):
    """(vram, size) of the owner's passthrough body, from its GLOBAL_ASM file
    (`nonmatching OWNER, SIZE` and the first instruction's address comment)."""
    path = Path(repo) / 'asm/us/nonmatchings/rom' / Path(row['tu']).stem / (row['owner'] + '.s')
    try:
        text = path.read_text()
    except OSError as exc:
        raise OwnershipError('owner has no passthrough body for its logical table') from exc
    size = re.search(r'(?m)^\s*nonmatching\s+' + re.escape(row['owner']) + r',\s*(0x[0-9A-Fa-f]+|\d+)', text)
    first = re.search(r'(?m)^\s*glabel\s+' + re.escape(row['owner']) + r'\s*\n\s*/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s', text)
    if not size or not first:
        raise OwnershipError('cannot read the owner passthrough extent')
    return int(first.group(1), 16), int(size.group(1), 0)


def prepare(repo, raw):
    """Register a new passthrough slot (hash-neutral; the ROM gate proves it).

    Appends `raw` to the registry, writes its companion assembly and inserts
    the companion block right after the owner's passthrough slot in the TU.
    Every member's slot must still be passthrough. Nothing is written unless
    the resulting registry validates (original bytes, overlap, alignment)."""
    repo = Path(repo)
    if not isinstance(raw, dict) or raw.get('storage_owner') or 'passthrough_asm' not in raw:
        raise OwnershipError('prepare registers passthrough-companion slots only')
    registry = load_registry(repo)
    if any(isinstance(r, dict) and r.get('owner') == raw.get('owner') for r in registry['rom_slots']):
        raise OwnershipError('owner already registered')
    registry['rom_slots'].append(raw)
    path = repo / REGISTRY
    original = path.read_text() if path.exists() else None
    path.write_text(json.dumps(registry, indent=2) + '\n')
    try:
        row = next(r for r in rom_slots(repo) if r['owner'] == raw['owner'])
        tu_path = repo / row['tu']
        text = tu_path.read_text()
        seg = Path(row['tu']).stem
        for member in slot_members(row):
            pragma = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/' + seg + '/' + member + '.s")'
            if text.count(pragma) != 1:
                raise OwnershipError(member + ' has no passthrough slot in ' + row['tu'])
        owner_pragma = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/' + seg + '/' + row['owner'] + '.s")\n'
        if text.count(owner_pragma) != 1:
            raise OwnershipError('owner passthrough line is not a complete TU line')
        block = companion_block(row)
        if block in text:
            raise OwnershipError('companion block already present')
        companion = repo / row['passthrough_asm']
        source = companion_source(row, repo)
        if companion.exists() and companion.read_text() != source:
            raise OwnershipError('existing companion differs from the registry row')
    except BaseException:
        if original is None:
            path.unlink()
        else:
            path.write_text(original)
        raise
    companion.parent.mkdir(parents=True, exist_ok=True)
    companion.write_text(source)
    tu_path.write_text(text.replace(owner_pragma, owner_pragma + block, 1))
    return row


def promotion_paths(repo, tu):
    """Immutable lifecycle paths checked/synced alongside the single mutable TU."""
    tu = _relative(tu); rows = [r for r in rom_slots(repo) if r['tu'] == tu]
    paths = set()
    if rows:
        paths.update([REGISTRY, 'Makefile', 'rush2049.us.ld', 'tools/conveyor/pipeline/owned_data.py', 'tools/asm-processor/asm_processor.py'])
        paths.update(r['passthrough_asm'] for r in rows if not r.get('storage_owner'))
    if any(r.get('tu') == tu for r in load_registry(repo)['storage_blocks']):
        from . import owned_storage
        paths.update(str(Path(p).relative_to(repo)) if Path(p).is_absolute() else _relative(p)
                     for p in owned_storage.package_paths(repo, tu))
    from . import owned_text
    paths.update(str(p.relative_to(repo)) for p in owned_text.package_paths(repo, tu))
    return [Path(repo) / p for p in sorted(paths)]


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--repo', type=Path, default=REPO)
    sub = p.add_subparsers(dest='command', required=True)
    split = sub.add_parser('split'); split.add_argument('source'); split.add_argument('composed'); split.add_argument('output'); split.add_argument('--objcopy', default='mips-linux-gnu-objcopy')
    ld = sub.add_parser('linker'); ld.add_argument('path')
    obj = sub.add_parser('object'); obj.add_argument('tu'); obj.add_argument('path')
    sub.add_parser('companions')
    prep = sub.add_parser('prepare', help='register a passthrough slot from a JSON row file')
    prep.add_argument('row')
    prep.add_argument('--linker', default='rush2049.us.ld',
                      help='generated linker script to re-render with the new slot (as the build does)')
    a = p.parse_args(argv)
    if a.command == 'split':
        split_container(a.repo, a.source, a.composed, a.output, objcopy=a.objcopy)
    elif a.command == 'object':
        normalize_object_sections(a.repo, a.tu, a.path)
    elif a.command == 'prepare':
        row = prepare(a.repo, json.loads(Path(a.row).read_text()))
        ld = Path(a.repo) / a.linker
        ld.write_text(rewrite_linker(ld.read_text(), a.repo))
        print('prepared ' + row['owner'] + ' ' + row['owner_section'] + ' slot ' + hex(row['container_vram'] + row['offset'])
              + ' size ' + hex(row['size']) + ' in ' + row['tu'] + ' (companion ' + row['passthrough_asm'] + ')')
    elif a.command == 'companions':
        print(' '.join(sorted({r['passthrough_asm'] for r in rom_slots(a.repo, check_source=False)
                               if not r.get('storage_owner')})))
    else:
        path = Path(a.path); path.write_text(rewrite_linker(path.read_text(), a.repo))


if __name__ == '__main__':
    main()
