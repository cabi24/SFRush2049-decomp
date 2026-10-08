import hashlib
import importlib.util
import json
import os
import shutil
import struct
import tempfile
from pathlib import Path
import unittest

HERE = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location('event_ring_verify', HERE/'verify.py')
v = importlib.util.module_from_spec(spec)
spec.loader.exec_module(v)


class PacketTests(unittest.TestCase):
    def test_receipt_binding(self):
        receipt = json.loads((HERE/'verification.json').read_text())
        self.assertEqual(receipt['status'], 'MATCH')
        self.assertEqual(receipt['native_sha256'], v.TARGET)
        self.assertEqual(receipt['source_sha256'], v.sha(v.SOURCE))
        for name, digest in receipt['packet_sha256'].items():
            self.assertEqual(v.sha(HERE/name), digest)
        self.assertEqual(receipt['elf']['function_bytes'], 396)
        self.assertEqual(receipt['elf']['differing_words'], 0)
        self.assertEqual(receipt['accepted_or_coverage_bytes'], 0)

    def test_fixture_domain(self):
        cases = v.fixtures()
        self.assertEqual(len(cases), 2560)
        self.assertEqual({x[3] for x in cases}, set(range(-128, 128)))
        self.assertEqual({x[5] for x in cases}, set(range(-128, 128)))
        self.assertEqual({(x[1], x[2], x[6], x[7]) for x in cases},
                         {(a, b, c, d) for a in range(4) for b in range(4)
                          for c in range(4) for d in range(1, 5)})

    def test_ordered_gates(self):
        case = (0, 0, 0, -1, 0, -1, 3, 1)
        before, after = v.fixture(case), v.expected(case)
        self.assertEqual(after.get(0x80395ED0, 1), 3)
        self.assertEqual(after.get(0x80152818+931, 1), 255)
        self.assertEqual(after.get(0x80149428, 1), (before.get(0x80149428, 1)-1) & 255)
        after = v.expected((0, 0, 0, -1, 1, -1, 3, 1))
        self.assertEqual(after.get(0x80152818+931, 1), 0)
        self.assertEqual(after.get(0x80395ED0, 1), 3)
        after = v.expected((0, 0, 0, 0, 0, -1, 3, 1))
        self.assertEqual(after.get(0x80395ED0, 1), 0)
        self.assertEqual(after.get(0x80395E70+3*24, 1), 1)


class LinkScriptTests(unittest.TestCase):
    def test_explicit_word_aligned_output_address_and_input_subalignment(self):
        self.assertEqual(v.ADDRESS % 4, 0)
        self.assertNotEqual(v.ADDRESS % 16, 0)
        script = v.native_link_script({'func_800F7E30': v.HELPER})
        self.assertIn('.text 0x803914B4 : SUBALIGN(4) { *(.text) }', script)
        self.assertNotIn('. = 0x803914B4;', script)
        self.assertIn('func_800F7E30 = 0x800f7e30;', script)


class GNUPlacementTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        present = (v.score.IDO / 'cc').is_file() and shutil.which('mips-linux-gnu-ld')
        if not present:
            if os.environ.get('REQUIRE_TOOLCHAIN') == '1':
                raise AssertionError('required pinned IDO and MIPS GNU linker are missing')
            raise unittest.SkipTest('pinned IDO and MIPS GNU linker required')
        cls.scratch = tempfile.TemporaryDirectory(prefix='event-ring-placement-')
        cls.addClassCleanup(cls.scratch.cleanup)
        cls.tmp = Path(cls.scratch.name)
        cls.obj = cls.tmp / 'candidate.o'
        v.score.compile_single(v.SOURCE, v.FLAGS, cls.obj)
        cls.data, cls.sections, cls.raw = v.inspect(cls.obj)
        receipt = json.loads((HERE / 'verification.json').read_text())
        cls.bindings = {name: int(address, 16) for name, address in receipt['elf']['bindings'].items()}
        cls.relocated, masks, unresolved, unverified, errors = v.score.relocate(
            cls.obj, v.score.text_words(cls.obj), 0, v.SIZE, cls.bindings)
        assert not any((masks, unresolved, unverified, errors))
        assert len(cls.relocated) == 100 and cls.relocated[-1] == 0
        assert v.sha_bytes(struct.pack('>99I', *cls.relocated[:99])) == v.TARGET

    def link(self, name, obj=None, options=(), address=None):
        script = self.tmp / (name + '.ld')
        text = v.native_link_script(self.bindings)
        if address is not None:
            text = text.replace('.text 0x%08X' % v.ADDRESS, '.text 0x%08X' % address)
        script.write_text(text)
        output = self.tmp / (name + '.elf')
        v.run(['mips-linux-gnu-ld', *options, '-EB', '-T', script, '-o', output, obj or self.obj])
        return output

    def test_exact_placement_and_complete_words_across_gnu_file_layouts(self):
        images = []
        for index, options in enumerate(((), ('--hash-size=1',),
                                         ('-z', 'max-page-size=0x1000'),
                                         ('-z', 'max-page-size=0x10000'))):
            with self.subTest(options=options):
                output = self.link('layout-%d' % index, options=options)
                _, _, raw = v.inspect(output, True)
                self.assertEqual(list(struct.unpack('>100I', raw)), self.relocated)
                self.assertEqual(v.sha_bytes(raw[:v.SIZE]), v.TARGET)
                images.append(output.read_bytes())
        # Header/file-offset packing may differ; complete text and placement may not.
        self.assertNotEqual(images[2], images[3])

    def test_input_alignment_cannot_move_the_function(self):
        shoff = struct.unpack_from('>I', self.data, 0x20)[0]
        entsize = struct.unpack_from('>H', self.data, 0x2e)[0]
        ti = v.score._text_index(self.sections)
        for alignment in (4, 16, 32, 256):
            with self.subTest(alignment=alignment):
                # Only a scratch object's alignment metadata changes. The
                # actual source, native target, full payload and symbols do not.
                changed = bytearray(self.data)
                struct.pack_into('>I', changed, shoff + ti * entsize + 32, alignment)
                obj = self.tmp / ('alignment-%d.o' % alignment)
                obj.write_bytes(changed)
                self.assertEqual(v.inspect(obj)[2], self.raw)
                output = self.link('alignment-%d' % alignment, obj=obj)
                _, _, raw = v.inspect(output, True)
                self.assertEqual(list(struct.unpack('>100I', raw)), self.relocated)

    def test_misplaced_gnu_function_is_rejected(self):
        output = self.link('misplaced', address=(v.ADDRESS + 15) & ~15)
        with self.assertRaises(AssertionError):
            v.inspect(output, True)

    def test_changed_symbol_section_extent_and_padding_are_rejected(self):
        output = self.link('inspect-controls')
        original, sections, _ = v.inspect(output, True)
        shoff = struct.unpack_from('>I', original, 0x20)[0]
        entsize = struct.unpack_from('>H', original, 0x2e)[0]
        ti = v.score._text_index(sections)
        symtab = next(sec for sec in sections if sec['type'] == 2)
        function = next(at for at in range(symtab['off'], symtab['off'] + symtab['size'], 16)
                        if original[at + 12] & 15 == 2 and
                        struct.unpack_from('>H', original, at + 14)[0] == ti)
        mutations = [('symbol address', function + 4, v.ADDRESS + 4),
                     ('symbol extent', function + 8, v.SIZE - 4),
                     ('section address', shoff + ti * entsize + 12, v.ADDRESS + 4),
                     ('section extent', shoff + ti * entsize + 20, 396),
                     ('nonzero trailing word', sections[ti]['off'] + v.SIZE, 1)]
        for name, offset, value in mutations:
            with self.subTest(mutation=name):
                changed = bytearray(original)
                struct.pack_into('>I', changed, offset, value)
                mutant = self.tmp / 'mutant.elf'
                mutant.write_bytes(changed)
                with self.assertRaises(AssertionError):
                    v.inspect(mutant, True)


if __name__ == '__main__':
    unittest.main()
