"""Exact native placement for the image-B texture and object-phase initializers."""
import importlib.util
import json
import os
from pathlib import Path
import shutil
import struct

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKETS = ('runtime_b_texture_ring_20261006', 'runtime_b_object_phases_20261006')


@pytest.fixture(params=PACKETS)
def packet(request):
    path = ROOT / 'cloud/work' / request.param / 'verify.py'
    spec = importlib.util.spec_from_file_location('gnu_placement_' + request.param, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    previous = module.score.ASM_DIR
    try:
        yield module
    finally:
        module.score.ASM_DIR = previous


def require_tools(module):
    names = ('mips-linux-gnu-ld', 'mips-linux-gnu-readelf', 'mips-linux-gnu-nm', 'gcc')
    present = (module.score.IDO / 'cc').is_file() and all(shutil.which(n) for n in names)
    if not present:
        if os.environ.get('REQUIRE_TOOLCHAIN') == '1':
            pytest.fail('required pinned IDO and GNU MIPS toolchain is missing')
        pytest.skip('pinned IDO and GNU MIPS toolchain required')


def test_word_aligned_section_address_is_explicit(packet):
    assert packet.ADDRESS % 4 == 0 and packet.ADDRESS % 16 == 12
    script = packet.native_link_script()
    assert '.text 0x%08X : SUBALIGN(4) { *(.text) }' % packet.ADDRESS in script
    assert '. = ' not in script
    for name, value in packet.GLOBALS.items():
        assert '%s = 0x%08x;' % (name, value) in script


def test_receipt_keeps_source_and_verifier_binding(packet):
    receipt = json.loads((packet.PACKET / 'verification.json').read_text())
    assert receipt['source_sha256'] == packet.sha(packet.SOURCE)
    for name, digest in receipt['packet_sha256'].items():
        assert packet.sha(packet.PACKET / name) == digest
    assert receipt['packet_sha256']['verify.py'] == packet.sha(packet.PACKET / 'verify.py')


def test_complete_linked_words_survive_gnu_layout_and_alignment(packet, tmp_path):
    require_tools(packet)
    obj = tmp_path / 'candidate.o'
    packet.score.compile_single(packet.SOURCE, packet.FLAGS, obj)
    data, sections, fn, raw = packet.inspect(obj)
    assert fn['value'] == 0
    relocated, masks, unresolved, unverified, errors = packet.score.relocate(
        obj, packet.score.text_words(obj), 0, packet.SIZE, packet.GLOBALS)
    assert not any((masks, unresolved, unverified, errors))
    expected = struct.pack('>%dI' % len(relocated), *relocated)
    receipt = json.loads((packet.PACKET / 'verification.json').read_text())
    assert packet.sha_bytes(expected[:packet.SIZE]) == receipt['elf']['body_sha256']
    shoff = struct.unpack_from('>I', data, 0x20)[0]
    entsize = struct.unpack_from('>H', data, 0x2e)[0]
    ti = packet.score._text_index(sections)
    script = tmp_path / 'native.ld'
    script.write_text(packet.native_link_script())
    options = ((), ('--hash-size=1',), ('-z', 'max-page-size=0x1000'),
               ('-z', 'max-page-size=0x10000'))
    packed = []
    for alignment in (4, 16, 32, 256):
        changed = bytearray(data)
        struct.pack_into('>I', changed, shoff + ti * entsize + 32, alignment)
        aligned = tmp_path / 'aligned.o'
        aligned.write_bytes(changed)
        assert packet.inspect(aligned)[3] == raw
        for flags in options:
            output = tmp_path / 'linked.elf'
            packet.run(['mips-linux-gnu-ld', *flags, '-EB', '-T', script, '-o', output, aligned])
            linked, linkedsecs, linkedfn, linkedraw = packet.inspect(output)
            assert linkedfn['value'] == packet.ADDRESS and linkedraw == expected
            sho = struct.unpack_from('>I', linked, 0x20)[0]
            she = struct.unpack_from('>H', linked, 0x2e)[0]
            assert struct.unpack_from('>I', linked, sho + packet.score._text_index(linkedsecs) * she + 12)[0] == packet.ADDRESS
            if alignment == 16:
                packed.append(linked)
    assert packed[2] != packed[3]  # Real header/file packing change, same proof.


@pytest.mark.parametrize('mutation', ('rounded-placement', 'symbol-address',
                                     'section-address', 'native-word', 'text-extent'))
def test_actual_proof_rejects_changed_linked_evidence(packet, monkeypatch, mutation):
    require_tools(packet)
    original_run = packet.run
    touched = []

    def changed_link(args, **kwargs):
        if str(args[0]) != 'mips-linux-gnu-ld':
            return original_run(args, **kwargs)
        if mutation == 'rounded-placement':
            path = Path(args[args.index('-T') + 1])
            text = path.read_text()
            assert '.text 0x%08X :' % packet.ADDRESS in text
            path.write_text(text.replace('.text 0x%08X :' % packet.ADDRESS,
                                         '.text 0x%08X :' % ((packet.ADDRESS + 15) & ~15)))
        result = original_run(args, **kwargs)
        touched.append(True)
        if mutation != 'rounded-placement':
            path = Path(args[args.index('-o') + 1])
            data, sections, _, _ = packet.inspect(path)
            changed = bytearray(data)
            ti = packet.score._text_index(sections)
            sho = struct.unpack_from('>I', data, 0x20)[0]
            she = struct.unpack_from('>H', data, 0x2e)[0]
            if mutation == 'symbol-address':
                symbols = next(sec for sec in sections if sec['type'] == 2)
                at = next(at for at in range(symbols['off'], symbols['off'] + symbols['size'], 16)
                          if data[at + 12] & 15 == 2 and
                          struct.unpack_from('>H', data, at + 14)[0] == ti)
                struct.pack_into('>I', changed, at + 4, packet.ADDRESS + 4)
            elif mutation == 'section-address':
                struct.pack_into('>I', changed, sho + ti * she + 12, packet.ADDRESS + 4)
            elif mutation == 'native-word':
                changed[sections[ti]['off'] + 3] ^= 1
            elif mutation == 'text-extent':
                struct.pack_into('>I', changed, sho + ti * she + 20, sections[ti]['size'] + 16)
            path.write_bytes(changed)
        return result

    monkeypatch.setattr(packet, 'run', changed_link)
    with pytest.raises(AssertionError):
        packet.prove()
    assert touched
