"""Lock-preserving split of one converted ROM TU at a real function boundary.

Geometry/source migration only: no new C promotion, recipe repin or Git write.
"""
import copy
import json
import re
import shutil
import tempfile
from pathlib import Path

from . import layout as L, lock, owned_data
from ..seeds.extract_candidates import extract_functions


class SplitRefusal(ValueError):
    pass


def _source_parts(text, segment):
    """Accept only generated preamble, ordered passthroughs and promoted blocks."""
    header = re.match(r'/\* GENERATED ROM-aligned TU\b.*?\*/', text, re.S)
    if not header:
        raise SplitRefusal('source is not a generated ROM TU')
    if f"segment {segment['yaml_name']} ({segment['rom_tu']})" not in header[0]:
        raise SplitRefusal('generated TU header does not identify this segment')
    functions = {a: (n, b) for n, a, b in extract_functions(text)}
    pos, pending, preamble, parts = header.end(), '', '', []
    context_includes = 0
    while pos < len(text):
        white = re.match(r'\s+', text[pos:])
        if white:
            pending += white[0]; pos += len(white[0]); continue
        if text.startswith('/*', pos):
            end = text.find('*/', pos + 2)
            if end < 0: raise SplitRefusal('unterminated top-level comment')
            pending += text[pos:end + 2]; pos = end + 2; continue
        if text.startswith('//', pos):
            end = text.find('\n', pos)
            if end < 0: end = len(text)
            pending += text[pos:end]; pos = end; continue
        if text[pos] == '#':
            end = text.find('\n', pos)
            if end < 0: end = len(text)
            line = text[pos:end]
            if re.fullmatch(r'#include\s+"[A-Za-z0-9_./-]+"\s*', line) and not parts:
                if not re.fullmatch(r'#include\s+"rom_tu.h"\s*', line) or context_includes:
                    raise SplitRefusal('only one standard rom_tu.h context include is supported')
                context_includes += 1
                preamble += pending + line + '\n'; pending = ''; pos = min(end + 1, len(text)); continue
            match = re.fullmatch(r'#pragma GLOBAL_ASM\("asm/us/nonmatchings/' +
                                 re.escape(segment['rom_tu']) + r'/([A-Za-z_]\w*)\.s"\)', line)
            if not match: raise SplitRefusal('unsupported top-level directive/content')
            parts.append((match[1], 'passthrough', pending + line))
            pending = ''; pos = end; continue
        if pos in functions:
            name, end = functions[pos]
            markers = L._PROMOTED_HDR_RE.findall(pending)
            if markers != [name]: raise SplitRefusal('function lacks its unique promoted provenance')
            parts.append((name, 'promoted', pending + text[pos:end]))
            pending = ''; pos = end; continue
        raise SplitRefusal('unsupported top-level declaration/content')
    if [n for n, _, _ in parts] != [f['name'] for f in segment['functions']]:
        raise SplitRefusal('source slot ordering differs from authoritative layout')
    if context_includes != 1:
        raise SplitRefusal('missing standard ROM context include')
    return preamble, {n: (state, body) for n, state, body in parts}, pending


def _geometry(mapping):
    return [(s['yaml_name'], f['name'], f['vaddr'], f['size'])
            for s in mapping['segments'] for f in s['functions']]


def _states(mapping):
    return [(f['name'], f['state']) for s in mapping['segments'] for f in s['functions']]


def _counts(mapping):
    coverage = L.coverage(mapping)
    return {key: coverage[key] for key in ('promoted_functions', 'promoted_bytes',
            'static_functions', 'static_bytes', 'promoted_slot_bytes', 'preserved_padding_bytes')}


def _number(value):
    try:
        if isinstance(value, bool): raise ValueError()
        number = int(value, 0) if isinstance(value, str) else value
        if type(number) is not int or number < 0: raise ValueError()
        return number
    except (TypeError, ValueError):
        raise SplitRefusal('malformed physical ownership range')


def _physical_overlap(row, start, end, family):
    ranges = []
    if 'rom_start' in row or 'next_rom_start' in row:
        a, b = _number(row.get('rom_start')), _number(row.get('next_rom_start'))
        ranges.append((a, b, start, end))
    if 'vram_start' in row:
        a = _number(row.get('vram_start'))
        length = _number(row.get('extent_bytes') if family == 'text_boundaries' else row.get('size'))
        ranges.append((a, a + length, start + L.VRAM_DELTA, end + L.VRAM_DELTA))
    if family == 'rom_slots' and any(k in row for k in ('container_vram', 'offset', 'size')):
        a = _number(row.get('container_vram')) + _number(row.get('offset'))
        length = _number(row.get('size'))
        ranges.append((a, a + length, start + L.VRAM_DELTA, end + L.VRAM_DELTA))
    if isinstance(row.get('data_slot'), dict):
        _physical_overlap(row['data_slot'], start, end, 'rom_slots')
    for a, b, first, last in ranges:
        if b <= a: raise SplitRefusal('malformed physical ownership range')
        if first < b and a < last:
            raise SplitRefusal('physical owned range overlaps split segment')


def _reject_owned(repo, old_tu, new_tu, start, end):
    try:
        registry = owned_data.load_registry(repo)
    except (OSError, ValueError) as error:
        raise SplitRefusal('invalid ownership registry') from error
    tus = {'src/' + old_tu + '.c', 'src/' + new_tu + '.c'}
    for family in ('storage_blocks', 'rom_slots', 'text_boundaries'):
        for row in registry.get(family, []):
            if not isinstance(row, dict): raise SplitRefusal('malformed ownership registry')
            contexts = row.get('context_files', [])
            if not isinstance(contexts, list) or any(not isinstance(item, dict) for item in contexts):
                raise SplitRefusal('malformed owned context paths')
            context = [item.get('path') for item in contexts]
            if row.get('tu') in tus or any(path in tus for path in context):
                raise SplitRefusal(f'{family} owns or pins an affected TU')
            _physical_overlap(row, start, end, family)


def _snapshot_paths(repo):
    # Normal SPLAT output surfaces, including ignored extracted assets/cache.
    return [repo / rel for rel in ('asm/us', 'src', 'assets/us', 'splat.us.yaml',
            'rush2049.us.ld', 'rush2049.us.ld.d', 'rush2049.us.ld_sections.txt',
            'build/us', 'build/layout.us.json',
            'undefined_syms_auto.us.txt', 'undefined_funcs_auto.us.txt',
            'symbol_addrs.us.txt', 'matched.lock.json', '.splat_cache')]


class _Snapshot:
    def __init__(self, paths, backup):
        backup.mkdir(parents=True, exist_ok=True)
        self.items = []
        for i, path in enumerate(paths):
            if path.is_symlink() or (path.is_dir() and any(p.is_symlink() for p in path.rglob('*'))):
                raise SplitRefusal('symlink in extraction/migration output surface')
            saved = backup / str(i)
            if path.is_dir(): shutil.copytree(path, saved)
            elif path.exists(): shutil.copy2(path, saved)
            self.items.append((path, saved))

    def restore(self):
        for path, saved in self.items:
            if path.is_symlink(): path.unlink()
            elif path.is_dir(): shutil.rmtree(path)
            elif path.exists(): path.unlink()
            if saved.is_dir(): shutil.copytree(saved, path)
            elif saved.exists():
                path.parent.mkdir(parents=True, exist_ok=True); shutil.copy2(saved, path)


def split(segment_name, boundary, new_tu, keep='suffix'):
    """Split atomically, retaining all promoted bodies at their original lock path.

    `boundary` is a ROM offset; `new_tu` is one flat rom/<name> TU. The retained
    side keeps the old source path. The other side must contain no promoted C.
    Caller owns subsequent complete native object/ROM/test gates and publication.
    """
    repo = L.REPO
    if keep not in ('prefix', 'suffix'): raise SplitRefusal('invalid retained side')
    if not isinstance(new_tu, str) or not re.fullmatch(r'rom/[A-Za-z_]\w*', new_tu):
        raise SplitRefusal('new TU must be a flat rom/<identifier> name')
    if type(boundary) is not int: raise SplitRefusal('boundary must be an integer ROM offset')
    try:
        segment_name = hex(int(segment_name, 0))
    except (TypeError, ValueError):
        raise SplitRefusal('segment must be an integer ROM offset')
    subsegments, code_end = L.parse_subsegments()
    candidate = next((s for s in subsegments if hex(s['off']) == segment_name), None)
    if not candidate or candidate['type'] != 'c' or not candidate['name']:
        raise SplitRefusal('split requires a clean converted static segment')
    old_tu = candidate['name']
    if not re.fullmatch(r'rom/[A-Za-z_]\w*', old_tu):
        raise SplitRefusal('unsupported original TU path')
    idx = subsegments.index(candidate)
    original_end = subsegments[idx + 1]['off'] if idx + 1 < len(subsegments) else code_end
    if original_end is None:
        raise SplitRefusal('original segment has no known endpoint')
    _reject_owned(repo, old_tu, new_tu, candidate['off'], original_end)
    try:
        mapping = L.derive()
    except (OSError, ValueError, KeyError, SystemExit) as error:
        raise SplitRefusal('invalid layout/owned context: ' + str(error)) from error
    segment = L._segment_by_name(mapping, segment_name)
    if not segment or segment['refusal'] or not segment['converted']:
        raise SplitRefusal('split requires a clean converted static segment')
    if new_tu == old_tu or any(s['rom_tu'] == new_tu for s in mapping['segments']):
        raise SplitRefusal('TU name collides with existing layout')
    subsegments, _ = L.parse_subsegments()
    if [s['off'] for s in subsegments] != sorted(set(s['off'] for s in subsegments)):
        raise SplitRefusal('unknown or duplicate subsegment ordering')
    start = int(segment['yaml_name'], 16); end = start + segment['size']
    if not start < boundary < end or boundary % 16:
        raise SplitRefusal('split must be an interior 16-aligned function boundary')
    cut = next((i for i, f in enumerate(segment['functions'])
                if int(f['vaddr'], 16) == boundary + L.VRAM_DELTA), None)
    if not cut: raise SplitRefusal('split is not an original complete function boundary')
    _reject_owned(repo, old_tu, new_tu, start, end)
    destination = repo / 'src' / (new_tu + '.c')
    exports = repo / 'asm/us/nonmatchings' / new_tu
    if destination.exists() or destination.is_symlink() or exports.exists() or exports.is_symlink():
        raise SplitRefusal('destination source/assembly ownership already exists')
    paths = _snapshot_paths(repo)
    if L._dirty(paths): raise SplitRefusal('dirty extraction/migration paths')
    source = repo / 'src' / (old_tu + '.c')
    preamble, components, tail = _source_parts(source.read_text(), segment)
    original_states, original_counts = _states(mapping), _counts(mapping)
    entries = lock.load_lock(L.LOCKFILE)
    problems = lock.check(entries, repo)
    if problems: raise SplitRefusal('existing locks/context invalid: ' + str(problems[:3]))
    retained = segment['functions'][cut:] if keep == 'suffix' else segment['functions'][:cut]
    retained_names = {f['name'] for f in retained}
    for name, (state, _) in components.items():
        if state != 'promoted': continue
        spec = 'src/' + old_tu + '.c:' + name
        if name not in retained_names: raise SplitRefusal('promoted body would change its locked source path')
        if spec not in entries or entries[spec].get('target_id') != name:
            raise SplitRefusal('promoted body lacks original canonical lock')
    yaml = L.SPLAT_YAML.read_text(); line = next(s for s in subsegments if s['off'] == start)
    first, second = (new_tu, old_tu) if keep == 'suffix' else (old_tu, new_tu)
    replacement = line['indent'] + f'- [0x{start:X}, c, {first}]\n' + line['indent'] + f'- [0x{boundary:X}, c, {second}]'
    yaml_lines = yaml.splitlines(keepends=True); ending = '\n' if yaml_lines[line['lineno']].endswith('\n') else ''
    yaml_lines[line['lineno']] = replacement + ending
    # Trial derive checks the production geometry without altering SPLAT input.
    with tempfile.TemporaryDirectory(prefix='rush-layout-split-') as tmp:
        trial = Path(tmp) / 'splat.us.yaml'; trial.write_text(''.join(yaml_lines))
        derived = L.derive(splat_path=trial)
        pieces = [L._segment_by_name(derived, hex(x)) for x in (start, boundary)]
        if any(not s or s['refusal'] for s in pieces): raise SplitRefusal('split geometry does not tile original native slots')
        old_geometry = [(n, a, b) for _, n, a, b in _geometry(mapping)]
        if old_geometry != [(n, a, b) for _, n, a, b in _geometry(derived)]:
            raise SplitRefusal('split changes original slot ordering/denominators')
        for part in pieces:
            promoted_flags = {entries['src/' + old_tu + '.c:' + f['name']]['flagset']
                              for f in part['functions'] if components[f['name']][0] == 'promoted'}
            if len(promoted_flags) > 1 or (promoted_flags and part['flagset'] not in promoted_flags):
                raise SplitRefusal('split changes accepted compiler recipe')
        snapshot = _Snapshot(paths, Path(tmp) / 'backup')
        lock_bytes = L.LOCKFILE.read_bytes() if L.LOCKFILE.exists() else None
        before_regions = L.collect_regions()
        authority = {address: tuple(region.words) for address, region in before_regions.items()}
        symbol_bytes = L.SYMBOL_ADDRS.read_bytes()
        try:
            L.SPLAT_YAML.write_text(trial.read_text())
            mh = L.map_hash(derived)
            for part in pieces:
                part = copy.deepcopy(part)
                for f in part['functions']:
                    state, body = components[f['name']]
                    f['state'] = state
                    if state == 'promoted': f['body'] = body
                content = L.generate_tu(part, mh).replace('#include "rom_tu.h"\n', preamble + '\n', 1)
                content += tail
                target = repo / 'src' / (part['rom_tu'] + '.c')
                target.parent.mkdir(parents=True, exist_ok=True); target.write_text(content)
            L._run_extract()
            # Only migrated SDK/VI exports lose their original directory ownership.
            moved = pieces[0] if keep == 'suffix' else pieces[1]
            old_exports = repo / 'asm/us/nonmatchings' / old_tu
            for f in moved['functions']:
                (old_exports / (f['name'] + '.s')).unlink(missing_ok=True)
            final = L.derive()
            if [(n, a, b) for _, n, a, b in _geometry(final)] != old_geometry:
                raise SplitRefusal('extraction changed native slot geometry')
            if _states(final) != original_states or _counts(final) != original_counts:
                raise SplitRefusal('extraction changed native coverage/provenance states')
            regions = L.collect_regions()
            if L.SYMBOL_ADDRS.read_bytes() != symbol_bytes:
                raise SplitRefusal('extraction changed authoritative symbol addresses')
            for address, words in authority.items():
                if address not in regions or tuple(regions[address].words) != words:
                    raise SplitRefusal('extraction changed original assembly authority: ' + hex(address))
            for part in pieces:
                for f in part['functions']:
                    if components[f['name']][0] == 'passthrough':
                        path = repo / 'asm/us/nonmatchings' / part['rom_tu'] / (f['name'] + '.s')
                        if not path.is_file(): raise SplitRefusal('missing regenerated assembly ownership: ' + f['name'])
            if (L.LOCKFILE.read_bytes() if L.LOCKFILE.exists() else None) != lock_bytes or lock.check(entries, repo):
                raise SplitRefusal('lock/source/context changed during split')
            L.write_opt_overrides(final)
            L.LAYOUT_JSON.parent.mkdir(parents=True, exist_ok=True)
            L.LAYOUT_JSON.write_text(json.dumps(final, indent=2) + '\n')
        except BaseException:
            snapshot.restore()
            raise
    return {'original_segment': segment_name, 'boundary': hex(boundary),
            'prefix_tu': first, 'suffix_tu': second, 'functions': len(segment['functions']),
            'native_bytes': segment['size'], 'locks_unchanged': True,
            'coverage_added': 0, 'gate_required': 'complete source-built ROM and original locks/tests'}
