"""Compile and strictly verify this genuine closure; no raw bytes are printed."""
import contextlib
import dataclasses
import hashlib
import io
import json
from pathlib import Path
import struct
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
from tools.cloud.owndata import ImageData


def sha(data):
    return hashlib.sha256(data).hexdigest()


def main():
    build = ROOT / 'build/palette_generation'
    build.mkdir(parents=True, exist_ok=True)
    obj = build / 'verified.o'
    spec = score.compile_group(HERE, obj)
    offsets = score.symbols(obj)
    words = score.text_words(obj)
    data, sections = score._elf(obj)
    function_symbols = {
        sym['name']: sym for i, sec in enumerate(sections) if sec['type'] == 2
        for sym in score._symbol_table(data, sections, i) if sym['type'] == 2
    }
    rows = []
    for name in spec['members'] + spec['context']:
        start = offsets[name]
        end = min((v for v in offsets.values() if v > start), default=len(words)*4)
        native = score.targets()[name]
        with contextlib.redirect_stdout(io.StringIO()):
            result = score.compare(obj, name, show=0)
        resolved, masks, unresolved, unverified, errors = score.relocate(
            obj, words, start, end, score.image_symbols())
        native_bytes = struct.pack('>%dI' % len(native), *native)
        body = resolved[start//4:end//4]
        compiled_bytes = struct.pack('>%dI' % len(body), *body)
        row = {
            'function': name,
            'claimed': name in spec['claims'],
            'target_address': '0x%08X' % score.image_symbols()[name],
            'target_bytes': len(native_bytes),
            'target_sha256': sha(native_bytes),
            'elf_st_size': function_symbols[name]['size'],
            'extent_to_next_symbol_or_text_end': end-start,
            'comparison': dataclasses.asdict(result),
            'full_extent_unresolved': unresolved,
            'full_extent_unverified': unverified,
            'full_extent_errors': errors,
            'fully_resolved_extent_sha256': sha(compiled_bytes) if not
                (masks or unresolved or unverified or errors) else None,
            'exact_full_extent_bytes': compiled_bytes == native_bytes and not
                (masks or unresolved or unverified or errors),
        }
        if row['claimed']:
            assert row['exact_full_extent_bytes']
            assert row['elf_st_size'] == len(native_bytes)
            assert result.differing == 0 and result.extra_words == 0
        rows.append(row)
    layout_obj = build / 'layout.o'
    score.compile_single(HERE/'layout_probe.c', score.DEFAULT_FLAGS, layout_obj)
    layout_data, layout_sections = score._elf(layout_obj)
    layout_symbol = next(sym for i, sec in enumerate(layout_sections) if sec['type'] == 2
                         for sym in score._symbol_table(layout_data, layout_sections, i)
                         if sym['name'] == 'palette_layout')
    layout_offset = layout_sections[layout_symbol['section']]['off'] + layout_symbol['value']
    native_layout = list(struct.unpack_from('>7I', layout_data, layout_offset))
    assert native_layout == [24,64,68,20,56,2,18]
    context_sources = {
        'heap_release.c': 'src/blob/groups/codex_heap_release_a25/group.c',
        'alloc_at.c': 'src/blob/groups/codex_heap_release_a25/alloc_at.c',
    }
    for local, original in context_sources.items():
        assert (HERE/local).read_bytes() == (ROOT/original).read_bytes()
    image = ImageData.from_artifact(ROOT/'asm/us/blob_data')
    raw = image.read(0x80123498,32)
    assert raw is not None
    format_string = raw.split(b'\0',1)[0].decode('ascii')
    max_name_bytes = max(len(format_string % n)+1 for n in (-32767,32768))
    assert max_name_bytes <= 32
    controls = []
    for source in sorted((HERE/'controls').glob('*.c')):
        directory = build / ('control_' + source.stem)
        directory.mkdir(exist_ok=True)
        for name in spec['files'] + ['group.json']:
            (directory/name).write_bytes((HERE/name).read_bytes())
        (directory/'palette.c').write_bytes(source.read_bytes())
        control_obj = directory/'control.o'
        score.compile_group(directory, control_obj)
        results = {}
        for fn in ('sound_bank_unload','func_800B0EA0'):
            with contextlib.redirect_stdout(io.StringIO()):
                comparison = score.compare(control_obj, fn, show=0)
            results[fn] = dataclasses.asdict(comparison)
        controls.append({'source': 'controls/' + source.name,
                         'source_sha256': sha(source.read_bytes()), 'results': results})
    output = {
        'base_commit': 'cf10b3392d7f00ae42d75c008b79fdc2541aab6b',
        'status': '192-byte strict interpolation match; complete nonmatching palette caller',
        'claims': spec['claims'],
        'native_layout_sizes_and_offsets': native_layout,
        'native_format_fits_32_byte_name_buffer': True,
        'maximum_formatted_name_bytes_including_null': max_name_bytes,
        'compiler_recipe': 'tools.cloud.score.compile_group; group.json; as1 -r4300_mul',
        'compiler_sha256': {name: sha(Path(score.ido(name)).read_bytes()) for name in
                            ('cc','uld','usplit','umerge','uopt','ugen','as1')},
        'source_sha256': {name: sha((HERE/name).read_bytes()) for name in
                          spec['files'] + ['group.json']},
        'unchanged_accepted_context_sources': context_sources,
        'target_manifest_sha256': sha((score.ASM_DIR/'SHA256SUMS').read_bytes()),
        'scorer_sha256': sha(Path(score.__file__).read_bytes()),
        'results': rows,
        'rejected_controls': controls,
    }
    print(json.dumps(output, indent=2, sort_keys=True))


if __name__ == '__main__':
    main()
