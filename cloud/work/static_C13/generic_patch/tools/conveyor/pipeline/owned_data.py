"""Own source-produced readonly slots inside existing ROM data containers.

Registry offsets describe real slots in ignored extracted assets. Splitting removes
those bytes from the linked container; the owner's actual section replaces them.
Passthrough companions and promotion are one TU transaction. No ROM score masks.
"""
import argparse
import hashlib
import json
import re
import subprocess
from pathlib import Path

REGISTRY = 'rom_owned_data.json'
REPO = Path(__file__).resolve().parents[3]
IDENT = re.compile(r'[A-Za-z_]\w*\Z')


class OwnershipError(ValueError):
    pass


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
    for raw in load_registry(repo)['rom_slots']:
        required = {'owner', 'tu', 'source', 'offset', 'size', 'container_vram', 'sha256', 'passthrough_asm'}
        if not isinstance(raw, dict) or not required.issubset(raw):
            raise OwnershipError('missing required ROM owner metadata')
        r = dict(raw)
        if not isinstance(r['owner'], str) or not IDENT.fullmatch(r['owner']):
            raise OwnershipError('invalid ROM owner name')
        for k in ['tu', 'source', 'passthrough_asm']:
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
        if not r['size'] or r['offset'] % 16 or r['size'] % 16:
            raise OwnershipError('owned slots must occupy complete 16-byte aligned sections')
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
            from .blob_rom import ROM_OFFSET, LENGTH
            built_start = ROM_OFFSET - 0x10000
            if r['offset'] < built_start + LENGTH and built_start < r['offset'] + r['size']:
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
    return sorted(result, key=lambda r: (r['source'], r['offset']))


def _groups(repo):
    groups = {}
    for r in rom_slots(repo):
        groups.setdefault(r['source'], []).append(r)
    return groups


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
        indent = re.search(r'(?m)^([ \t]*)' + re.escape(original), text).group(1)
        marker = obj + '(' + section + ')'
        lines = ['/* ROM_OWNED_DATA_BEGIN ' + marker + ' */']
        binary_names = {re.sub(r'[^A-Za-z0-9]', '_', name) for name in [source, rows[0].get('composed_source', source)]}
        lines += ['_binary_' + name + '_start = ABSOLUTE(.);' for name in sorted(binary_names)]
        for i, r in enumerate(rows):
            start = 'rom_owned_' + r['owner'] + '_START'; end = 'rom_owned_' + r['owner'] + '_END'
            lines += [obj + '(' + section_name(rows, i) + ');', start + ' = ABSOLUTE(.);', r['owner_object'] + '(' + r['owner_section'] + ');', end + ' = ABSOLUTE(.);', 'ASSERT(' + start + ' == ' + hex(r['container_vram'] + r['offset']) + ', "owned ROM slot address changed");', 'ASSERT(' + end + ' - ' + start + ' == ' + hex(r['size']) + ', "owned ROM slot size changed");']
        lines += [obj + '(' + section_name(rows, len(rows)) + ');']
        for name in sorted(binary_names):
            lines += ['_binary_' + name + '_end = ABSOLUTE(.);', '_binary_' + name + '_size = ABSOLUTE(_binary_' + name + '_end - _binary_' + name + '_start);']
        lines += ['/* ROM_OWNED_DATA_END ' + marker + ' */']
        text = text.replace(original, ('\n' + indent).join(lines))
    return text


def companion_block(row):
    return ('/* ROM_OWNED_RODATA_BEGIN ' + row['owner'] + ' */\n'
            '#pragma GLOBAL_ASM("' + row['passthrough_asm'] + '")\n'
            '/* ROM_OWNED_RODATA_END ' + row['owner'] + ' */\n')


def companions_for_tu(repo, tu, states):
    return ''.join(companion_block(r) for r in rom_slots(repo)
                   if r['tu'] == _relative(tu) and states.get(r['owner']) == 'passthrough')


def strip_companion(text, owner, repo=REPO, tu=None):
    rows = [r for r in rom_slots(repo) if r['owner'] == owner]
    if not rows:
        return text
    r = rows[0]
    if tu is not None and r['tu'] != _relative(tu):
        raise OwnershipError('owner belongs to another TU')
    block = companion_block(r)
    if text.count(block) != 1:
        raise OwnershipError('expected exactly one owned passthrough companion')
    return text.replace(block, '', 1)


def promotion_paths(repo, tu):
    """Immutable lifecycle paths checked/synced alongside the single mutable TU."""
    tu = _relative(tu); rows = [r for r in rom_slots(repo) if r['tu'] == tu]
    paths = set()
    if rows:
        paths.update([REGISTRY, 'Makefile', 'rush2049.us.ld', 'tools/conveyor/pipeline/owned_data.py', 'tools/asm-processor/asm_processor.py'])
        paths.update(r['passthrough_asm'] for r in rows)
    if any(r.get('tu') == tu for r in load_registry(repo)['storage_blocks']):
        from . import owned_storage
        paths.update(str(Path(p).relative_to(repo)) if Path(p).is_absolute() else _relative(p)
                     for p in owned_storage.package_paths(repo, tu))
    return [Path(repo) / p for p in sorted(paths)]


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--repo', type=Path, default=REPO)
    sub = p.add_subparsers(dest='command', required=True)
    split = sub.add_parser('split'); split.add_argument('source'); split.add_argument('composed'); split.add_argument('output'); split.add_argument('--objcopy', default='mips-linux-gnu-objcopy')
    ld = sub.add_parser('linker'); ld.add_argument('path')
    sub.add_parser('companions')
    a = p.parse_args(argv)
    if a.command == 'split':
        split_container(a.repo, a.source, a.composed, a.output, objcopy=a.objcopy)
    elif a.command == 'companions':
        print(' '.join(sorted({r['passthrough_asm'] for r in rom_slots(a.repo)})))
    else:
        path = Path(a.path); path.write_text(rewrite_linker(path.read_text(), a.repo))


if __name__ == '__main__':
    main()
