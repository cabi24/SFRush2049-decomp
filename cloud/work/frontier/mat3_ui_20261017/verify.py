"""Read-only source/group proof. No image build, splice, lock or ROM claim."""
import contextlib
import hashlib
import io
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
from tools.conveyor.pipeline import blob_group

GROUP = HERE / 'groups/frontier_optionscr_buttons'
NAME = 'func_800DA0BC'
MEMBERS = ['func_800DB1E0', 'func_800DA2C0', NAME]

def require(ok, reason):
    if not ok:
        raise RuntimeError(reason)

def sha(data):
    return hashlib.sha256(data).hexdigest()

def packed(words):
    return struct.pack('>%dI' % len(words), *words)

def compare(obj, name):
    with contextlib.redirect_stdout(io.StringIO()):
        return score.compare(obj, name)

def own_proof(obj, name):
    image = score.own_data()
    addresses = score.image_symbols()
    return blob_group.verify_own_data(
        obj, name, score.targets()[name], addresses[name], image,
        addresses=lambda n: addresses.get(n, score.address_named(n)),
        data_runs=[(a, a + len(b)) for a, b in image.runs])

def evidence():
    spec = json.loads((GROUP / 'group.json').read_text())
    require(spec['members'] == MEMBERS and spec['claims'] == MEMBERS and spec['keep'] == ['func_800DB1E0'], 'complete real group and preserved claims')
    require(not spec.get('allow_unverified'), 'no uncertainty waiver')
    require(spec['context'] == [] and 'standin' not in (GROUP/'uninit.c').read_text(), 'no stand-in')
    require((GROUP/'buttons.c').read_bytes() == (ROOT/'src/blob/groups/frontier_optionscr_buttons/buttons.c').read_bytes(), 'preserved caller source')
    words = score.targets()
    addresses = score.image_symbols()
    call = 0x0c000000 | ((addresses[NAME] >> 2) & 0x03ffffff)
    census = {n: [i * 4 for i, w in enumerate(body) if w == call]
              for n, body in words.items() if call in body}
    require(set(census) == {'func_800DB1E0'}, 'real caller census')
    require(len(words[NAME]) == 46, 'complete target extent')
    return {
        'base': '58425072b49e95f0fbe5c4c239988fb4885f7c5c',
        'population': 'game', 'new_source_exact_bytes': 184,
        'new_accepted_image_bytes': 0, 'new_rom_verified_bytes': 0,
        'targets': {n: {'address': hex(addresses[n]), 'bytes': 4*len(words[n]),
                        'sha256': sha(packed(words[n]))} for n in MEMBERS},
        'caller_offsets': census,
        'test_sha256': sha((ROOT/'tests/cloud/test_ui_teardown_genuine_context.py').read_bytes()),
        'sources': {str(p.relative_to(HERE)): sha(p.read_bytes())
                    for p in [GROUP/'buttons.c', GROUP/'uninit.c', GROUP/'group.json', HERE/'verify.py']}}

def gnu_new_body(obj, directory):
    data, sections = score._elf(obj)
    functions = score.symbols(obj)
    text_base = score.image_symbols()[NAME] - functions[NAME]
    addresses = score.image_symbols()
    externs = {}
    for i, section in enumerate(sections):
        if section['type'] != 2:
            continue
        for s in score._symbol_table(data, sections, i):
            if s['section'] == 0 and s['name']:
                a = addresses.get(s['name'], score.address_named(s['name']))
                if a is not None:
                    externs[s['name']] = a
            if s['name'] == NAME:
                require(s['size'] == 184 and s['type'] == 2, 'complete ELF STT_FUNC extent')
    script = directory/'proof.ld'
    script.write_text('SECTIONS { .text 0x%x : SUBALIGN(4) { *(.text) } .rodata 0x80400000 : { *(.rodata) } .data : { *(.data) } .bss : { *(.bss) } }\n' % text_base +
                      ''.join('%s = 0x%x;\n' % pair for pair in sorted(externs.items())))
    elf = directory/'proof.elf'
    result = subprocess.run(['mips-linux-gnu-ld', '-T', str(script), '-o', str(elf), str(obj)], capture_output=True, text=True)
    require(result.returncode == 0, 'GNU link failed: '+result.stderr)
    linked = score.text_words(elf)
    at = functions[NAME]//4
    require(linked[at:at+46] == score.targets()[NAME], 'GNU original-object full 184-byte inequality')

def main():
    inputs = evidence()
    receipt = HERE/'RECEIPT.json'
    if '--write-receipt' in sys.argv:
        receipt.write_text(json.dumps(inputs, indent=2, sort_keys=True)+'\n')
    require(inputs == json.loads(receipt.read_text()), 'input receipt changed')
    with tempfile.TemporaryDirectory(prefix='rush-ui-proof-') as temp:
        d = Path(temp)
        obj = d/'group.o'
        score.compile_group(GROUP, obj)
        summaries = {}
        for n in MEMBERS:
            result = compare(obj, n)
            require(result.differing == 0 and result.extra_words == 0 and not result.unresolved, n+' code/ref failure')
            require(all(blob_group.ONE_BASE_REFUSAL in e for e in result.errors), n+' unexpected canonical error')
            own = own_proof(obj, n)
            require(own.ok and not own.failures and not own.unverified, n+' owned-data failure')
            if n == NAME:
                require(result.accepted(False), 'new body must pass canonical strict score')
            summaries[n] = {'canonical': result.summary(), 'own_data': own.notes}
        gnu_new_body(obj, d)
        # Source negative control: real sentinel becomes zero instead of -1.
        neg = d/'negative'
        shutil.copytree(GROUP, neg)
        source = neg/'uninit.c'
        source.write_text(source.read_text().replace('slot[1] = -1;', 'slot[1] = 0;'))
        score.compile_group(neg, d/'negative.o')
        rejected = compare(d/'negative.o', NAME)
        require(not rejected.accepted(False) and rejected.differing > 0, 'wrong sentinel not rejected')
        # Independent owned-data negative: corrupt actual byte of a read-only window.
        raw, sections = score._elf(obj)
        ro = next(s for s in sections if s['name'] == '.rodata')
        corrupted = bytearray(raw)
        corrupted[ro['off'] + 0x18] ^= 1
        broken = d/'wrong-own.o'
        broken.write_bytes(corrupted)
        require(not own_proof(broken, 'func_800DA2C0').ok, 'wrong split-owned literal not rejected')
        print(json.dumps({'members': summaries, 'new_bytes': 184,
                          'gnu_new_body': '184/184 bytes exact',
                          'wrong_sentinel_differences': rejected.differing,
                          'wrong_own_data_rejected': True}, indent=2))
if __name__ == '__main__':
    main()
