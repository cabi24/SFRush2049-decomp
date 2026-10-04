"""Replay one bounded rejected source delta; no target or scorer changes."""
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score

NAME = 'func_8001EF8C'
OLD_WORK = ROOT / 'cloud/work/boot_tail/BT03-high-runtime-lists'
BASE = OLD_WORK / 'nonmatch' / (NAME + '.c')
CONTROL = WORK / 'controls/func_8001EF8C_head_assignment.c'
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
OLD_TEST = '    link->next = D_80050548[channel];\n    if (D_80050548[channel] != 255) {'
NEW_TEST = '    if ((link->next = D_80050548[channel]) != 255) {'


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def fields(word):
    return word >> 26, (word >> 21) & 31, (word >> 16) & 31, word & 65535


def topology(words, offset):
    load, constant, branch, store, reload = [fields(w) for w in words[offset:offset+5]]
    assert load == (36, 8, 2, 0)  # byte bucket head into v0
    assert constant == (9, 0, 1, 255)
    assert branch[:3] == (4, 2, 1)
    assert store == (40, 3, 2, 1)  # assignment store in branch delay slot
    assert reload == (36, 8, 15, 0)  # fresh read for the nonempty backlink
    return dict(test_uses_assigned_value=True, next_store_in_branch_delay_slot=True,
                nonempty_backlink_rereads_bucket=True)


def run():
    pins = json.loads((WORK / 'input_pins.json').read_text())
    for path, expected in pins['files_sha256'].items():
        assert digest(ROOT / path) == expected, path
    compiler = {p.name: digest(p) for p in sorted(score.IDO.iterdir()) if p.is_file()}
    assert compiler == pins['compiler_files_sha256']
    assert BASE.read_text().count(OLD_TEST) == 1
    assert CONTROL.read_text() == BASE.read_text().replace(OLD_TEST, NEW_TEST)
    assert CONTROL.read_text().count('D_800504C8[D_80050548[channel]].previous = index;') == 1
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    targets = score.targets()
    assert len(targets) == 439 and sum(map(len, targets.values())) * 4 == 99120
    assert len(targets[NAME]) * 4 == 432
    native_topology = topology(targets[NAME], 32)
    assert fields(targets[NAME][0]) == (9, 29, 29, 65536 - 48)
    inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())
    extents = json.loads((score.ASM_DIR / 'extents.json').read_text())
    assert {r['address']: r['size'] for r in inventory['functions']} == {
        r['address']: r['size'] for r in extents['functions']}
    manifest = subprocess.run(['sha256sum', '-c', 'SHA256SUMS'], cwd=score.ASM_DIR,
                              check=True, capture_output=True, text=True)
    callers = []
    for name, words in targets.items():
        for i, word in enumerate(words):
            if word >> 26 == 3 and ((word & 0x3FFFFFF) << 2 | 0x80000000) == 0x8001EF8C:
                callers.append(dict(name=name, call_site='0x%08X' % (int(name[5:], 16) + i*4)))
    assert callers == [dict(name=name, call_site=site) for name, site in (
        ('func_8001F898', '0x8001F934'), ('func_80022514', '0x80022564'),
        ('func_80022580', '0x80022590'), ('func_80024988', '0x80024BA0'))]
    forms = [
        (BASE, ((88, 0, 432, []), (108, 50, 636, []))),
        (OLD_WORK/'controls/func_8001EF8C_initial.c', ((101, 2, 444, []), (108, 51, 640,
            ['unpaired R_MIPS_HI16 for D_80050648 at .text+0x1a4']))),
        (OLD_WORK/'controls/func_8001EF8C_explicit_group_stop.c', ((102, 0, 420, []), (108, 48, 628,
            ['unpaired R_MIPS_HI16 for D_80050A48 at .text+0x1ac']))),
        (CONTROL, ((92, 0, 432, []), (106, 51, 640, []))),
    ]
    results = []
    with tempfile.TemporaryDirectory(prefix='voice-priority-verify-') as temporary:
        temp = Path(temporary)
        for path, expectations in forms:
            for level, (diff, extra, size, compare_errors) in zip((2, 1), expectations):
                flags = FLAGS.replace('-O2', '-O%d' % level)
                obj = temp / (path.stem + '-O%d.o' % level)
                score.compile_single(path, flags, obj)
                result = score.compare(obj, NAME, show=0)
                data, sections = score._elf(obj)
                funcs = [s for i, section in enumerate(sections) if section['type'] == 2
                         for s in score._symbol_table(data, sections, i)
                         if s['type'] == 2 and s['name'] == NAME]
                assert len(funcs) == 1 and funcs[0]['value'] == 0 and funcs[0]['size'] == size
                words = score.text_words(obj)
                relocated, masks, unresolved, unverified, errors = score.relocate(
                    obj, words, 0, len(words)*4, score.image_symbols())
                assert not masks and not unresolved and not unverified and not errors
                assert not any(words[size//4:])
                assert result.differing == diff and result.extra_words == extra
                assert not result.unresolved and not result.unverified
                assert result.errors == compare_errors and not result.accepted()
                assert relocated[:108] != targets[NAME]
                record = dict(source_path=str(path.relative_to(ROOT)), source_sha256=digest(path),
                    flags=flags, effective_flags=flags+' '+score.R4300_CC,
                    differing_words=result.differing, total_words=result.total,
                    extra_words=result.extra_words, strict_match=False,
                    elf_function_bytes=size, stack_frame_bytes=65536-fields(words[0])[3],
                    comparison_errors=result.errors, full_text_relocations_checked=True,
                    full_text_masks=0, full_text_unresolved=[], full_text_unverified=[],
                    full_text_errors=[], trailing_section_padding_is_zero=True)
                if path == CONTROL and level == 2:
                    record['bucket_topology'] = topology(relocated, 30)
                    assert record['stack_frame_bytes'] == 56
                if path == BASE and level == 2:
                    assert record['stack_frame_bytes'] == 48
                results.append(record)
        getter = ROOT / 'cloud/matches/boot_tail/func_80010A00.c'
        score.compile_single(getter, FLAGS, temp/'getter.o')
        assert score.compare(temp/'getter.o', getter.stem, show=0).accepted()
        layout = temp / 'native_layout.c'
        layout.write_text('#define offsetof(t,m) ((unsigned long)&((t *)0)->m)\n#include "'+str(CONTROL)+'"\n'
            'typedef char pointer_width[sizeof(void *) == 4 ? 1 : -1];\n'
            'typedef char byte_width[sizeof(u8) == 1 ? 1 : -1];\n'
            'typedef char half_width[sizeof(u16) == 2 ? 1 : -1];\n'
            'typedef char word_width[sizeof(u32) == 4 ? 1 : -1];\n'
            'typedef char channel_size[sizeof(ChannelLink) == 4 ? 1 : -1];\n'
            'typedef char channel_previous[offsetof(ChannelLink,previous) == 0 ? 1 : -1];\n'
            'typedef char channel_next[offsetof(ChannelLink,next) == 1 ? 1 : -1];\n'
            'typedef char channel_active[offsetof(ChannelLink,active) == 2 ? 1 : -1];\n'
            'typedef char group_size[sizeof(GroupLink) == 4 ? 1 : -1];\n'
            'typedef char group_next[offsetof(GroupLink,next) == 0 ? 1 : -1];\n'
            'typedef char group_previous[offsetof(GroupLink,previous) == 2 ? 1 : -1];\n'
            'typedef char voice_priority[offsetof(VoicePrefix,channel2E) == 46 ? 1 : -1];\n'
            'typedef char voice_identifier[offsetof(VoicePrefix,identifier60) == 96 ? 1 : -1];\n'
            'typedef char voice_size[sizeof(VoicePrefix) == 100 ? 1 : -1];\n')
        score.compile_single(layout, FLAGS, temp/'native_layout.o')
    return dict(result='PASS', outcome='REJECTED_CONTROL_RETAIN_BASELINE',
        base_commit=pins['base_commit'], new_source_forms=1, new_unique_attempts=0,
        new_verified_functions=0, new_verified_bytes=0, all_input_pins_verified=True,
        compiler_files_sha256=compiler, target_manifest_verified=manifest.stdout.splitlines(),
        target_population_functions=439, target_population_bytes=99120,
        existing_getter_strict_match=True, native_layout_assertions=14,
        native_bucket_topology=native_topology, native_direct_boot_tail_callers=callers, results=results)


if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
