"""Replay two evidence-led repairs with strict full-function relocation and ELF checks."""
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score

NAME = 'func_8001ECE0'
BASE = ROOT / 'cloud/work/boot_tail/BT03-high-chains/nonmatch/func_8001ECE0.c'
DONOR = WORK / 'controls/func_8001ECE0_free_head_assignment.c'
FINAL = ROOT / 'cloud/matches/boot_tail/func_8001ECE0.c'
BASE_HASH = '5b4fc9a766c0518d026e1c4c76b20e2164cae2daa015647852db0c79ff575df8'
DONOR_HASH = '2213685e44f31cf0a271730bce425dda1f3fcdb78968f04b587393f7105e6388'
FINAL_HASH = '00f70ee6c8e555735cc2e2edb348f715e63622f2b92a97bd57536ffe3ef72192'
OLD_ADVANCE = '    D_80050C54 = node->next;\n    if (D_80050C54 != 0) D_80050C54->previous = 0;'
NEW_ADVANCE = '    if ((D_80050C54 = D_80050C54->next) != 0)\n        D_80050C54->previous = 0;'
OLD_LOOP = '    current = D_80050C50;\n    previous = 0;\n    while (current != 0) {'
NEW_LOOP = '    previous = 0;\n    for (current = D_80050C50; current != 0; current = current->next) {'
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def trace_receipt(source, ordinary, temp):
    obj = temp / ('traced-' + source.stem + '.o')
    command = [score.ido('cc'), '-K', '-c', *FLAGS.split(), score.R4300_CC,
               '-Wa,-R', '-o', str(obj), str(source.resolve())]
    proc = subprocess.run(command, cwd=temp, check=True, capture_output=True, text=True)
    assert obj.read_bytes() == ordinary.read_bytes()
    nodes = {}
    for match in re.finditer(r'Node\s+(\d+): inst ([0-9a-f]+), relocation \d+, lineno (\d+)\n\s*before (\d+), aftercycles (\d+), maxhazard (\d+)', proc.stdout):
        instruction = score.disasm_word(int(match[2], 16))
        if instruction in ('move a0,s0', 'move at,s0'):
            assert instruction not in nodes
            nodes[instruction] = dict(node=int(match[1]), line=int(match[3]),
                before=int(match[4]), aftercycles=int(match[5]), maxhazard=int(match[6]))
    assert set(nodes) == {'move a0,s0', 'move at,s0'}
    previous, packed = nodes['move a0,s0'], nodes['move at,s0']
    assert previous['before'] == packed['before'] == 0
    assert previous['aftercycles'] == packed['aftercycles'] == 6
    assert previous['maxhazard'] == packed['maxhazard'] == 0
    needle = 'Initial nodes: %d %d\n' % (previous['node'], packed['node'])
    block = proc.stdout.split(needle)[1].split('\nNode')[0]
    for node in (previous['node'], packed['node']):
        assert re.search(r'node %d \(INST \d+\), time = 0, aftercycles = 6, latency = 1,' % node, block)
    picks = re.findall(r'Picking node (\d+).*? at ([0-9a-f]+),', block)
    first = [int(row[0]) for row in picks[:2]]
    expected = [previous['node'], packed['node']] if source == DONOR else [packed['node'], previous['node']]
    assert first == expected
    assert (previous['line'] < packed['line']) == (source == DONOR)
    return dict(source_path=str(source.relative_to(ROOT)), source_sha256=digest(source),
        stock_trace_object_byte_identical=True, previous_copy=previous,
        packed_load_address_copy=packed, first_two_selected_nodes=first,
        equal_readiness_critical_path_and_latency=True,
        selected_order='previous then packed address' if source == DONOR else 'packed address then previous')


def run():
    pins = json.loads((WORK / 'input_pins.json').read_text())
    for path, expected in pins['files_sha256'].items():
        assert digest(ROOT / path) == expected, path
    compiler = {p.name: digest(p) for p in sorted(score.IDO.iterdir()) if p.is_file()}
    assert compiler == pins['compiler_files_sha256']
    assert digest(BASE) == BASE_HASH and digest(DONOR) == DONOR_HASH and digest(FINAL) == FINAL_HASH
    assert BASE.read_text().count(OLD_ADVANCE) == 1
    assert DONOR.read_text() == BASE.read_text().replace(OLD_ADVANCE, NEW_ADVANCE)
    assert DONOR.read_text().count(OLD_LOOP) == 1
    assert DONOR.read_text().count('        current = current->next;\n') == 1
    assert FINAL.read_text() == DONOR.read_text().replace(OLD_LOOP, NEW_LOOP).replace('        current = current->next;\n', '')
    assert (WORK / 'controls/func_8001ECE0_for_traversal.c').read_bytes() == FINAL.read_bytes()
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    targets = score.targets()
    assert len(targets) == 439 and sum(map(len, targets.values())) * 4 == 99120
    assert len(targets[NAME]) * 4 == 276
    inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())
    extents = json.loads((score.ASM_DIR / 'extents.json').read_text())
    assert {r['address']:r['size'] for r in inventory['functions']} == {r['address']:r['size'] for r in extents['functions']}
    manifest = subprocess.run(['sha256sum', '-c', 'SHA256SUMS'], cwd=score.ASM_DIR,
                              check=True, capture_output=True, text=True)
    rows, traces = [], []
    with tempfile.TemporaryDirectory(prefix='voice-allocation-verify-') as temp:
        temp = Path(temp)
        for source, o2_diff, o1_extra, o1_size in ((BASE,8,21,368),(DONOR,3,22,372),(FINAL,0,21,368)):
            for level in (2,1):
                flags = FLAGS.replace('-O2', '-O%d' % level)
                obj = temp / (source.stem + '-O%d.o' % level)
                score.compile_single(source.resolve(), flags, obj)
                result = score.compare(obj, NAME, show=0)
                data, sections = score._elf(obj)
                funcs = [s for i, section in enumerate(sections) if section['type'] == 2
                         for s in score._symbol_table(data, sections, i)
                         if s['type'] == 2 and s['name'] == NAME]
                assert len(funcs) == 1 and funcs[0]['value'] == 0
                extent = funcs[0]['size']
                assert extent == (276 if level == 2 else o1_size)
                words = score.text_words(obj)
                relocated, masks, unresolved, unverified, errors = score.relocate(
                    obj, words, 0, len(words)*4, score.image_symbols())
                assert not masks and not unresolved and not unverified and not errors
                assert not any(words[extent//4:])
                assert not result.unresolved and not result.unverified and not result.errors
                assert result.differing == (o2_diff if level == 2 else 69)
                assert result.extra_words == (0 if level == 2 else o1_extra)
                matching = source == FINAL and level == 2
                assert result.accepted() == matching
                assert (relocated[:69] == targets[NAME]) == matching
                rows.append(dict(source_path=str(source.relative_to(ROOT)), source_sha256=digest(source),
                    flags=flags, effective_flags=flags+' '+score.R4300_CC,
                    differing_words=result.differing, total_words=result.total, extra_words=result.extra_words,
                    strict_match=result.accepted(), elf_function_bytes=extent,
                    masks=0, unresolved=[], unverified=[], errors=[],
                    full_text_relocations_checked=True, padding_is_zero=True,
                    entire_native_function_equal=matching))
                if level == 2 and source in (DONOR, FINAL):
                    traces.append(trace_receipt(source, obj, temp))
        getter = ROOT / 'cloud/matches/boot_tail/func_80010A00.c'
        score.compile_single(getter, FLAGS, temp/'getter.o')
        assert score.compare(temp/'getter.o', getter.stem, show=0).accepted()
        layout = temp / 'native_layout.c'
        layout.write_text('#define offsetof(t,m) ((unsigned long)&((t *)0)->m)\n#include "'+str(FINAL)+'"\n'
            'typedef char pointer_width[sizeof(void *) == 4 ? 1 : -1];\n'
            'typedef char node_size[sizeof(SequenceNode) == 16 ? 1 : -1];\n'
            'typedef char node_next[offsetof(SequenceNode,next) == 0 ? 1 : -1];\n'
            'typedef char node_previous[offsetof(SequenceNode,previous) == 4 ? 1 : -1];\n'
            'typedef char node_key[offsetof(SequenceNode,key) == 8 ? 1 : -1];\n'
            'typedef char node_value[offsetof(SequenceNode,value) == 12 ? 1 : -1];\n'
            'typedef char voice_node[offsetof(VoicePrefix,entry18) == 24 ? 1 : -1];\n'
            'typedef char voice_id[offsetof(VoicePrefix,identifier60) == 96 ? 1 : -1];\n'
            'typedef char voice_prefix_size[sizeof(VoicePrefix) == 100 ? 1 : -1];\n')
        score.compile_single(layout, FLAGS, temp/'native_layout.o')
    return dict(result='PASS', base_commit=pins['base_commit'], source_forms_added=2,
        new_verified_functions=1, new_verified_bytes=276, new_unique_attempts=0,
        all_input_pins_verified=True, compiler_files_sha256=compiler,
        target_manifest_verified=manifest.stdout.splitlines(), target_population_functions=439,
        target_population_bytes=99120, existing_getter_strict_match=True,
        native_layout_assertions=9, results=rows, stock_trace_receipts=traces)


if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
