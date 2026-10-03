"""C11 source semantics and read-only preflight contracts; no ROM required."""
import ctypes
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest

WORK = Path(__file__).resolve().parents[1]
ROOT = WORK.parents[2]
sys.path.insert(0, str(ROOT))
spec = importlib.util.spec_from_file_location('preflight', WORK / 'scripts/preflight.py')
preflight = importlib.util.module_from_spec(spec)
spec.loader.exec_module(preflight)


class PreflightTests(unittest.TestCase):
    def setUp(self):
        self.inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())
        self.extents = json.loads((ROOT / 'asm/us/boot_tail/extents.json').read_text())

    def test_all_extents(self):
        self.assertEqual(preflight.check_extents(self.inventory, self.extents),
                         {'functions': 439, 'bytes': 99120, 'equal': True})

    def test_size_drift_rejected(self):
        self.extents['functions'][5]['size'] += 4
        with self.assertRaisesRegex(ValueError, 'drifted'):
            preflight.check_extents(self.inventory, self.extents)

    def test_duplicate_start_rejected(self):
        self.extents['functions'][1]['address'] = self.extents['functions'][0]['address']
        with self.assertRaisesRegex(ValueError, 'unique'):
            preflight.check_extents(self.inventory, self.extents)

    def test_parallel_claims_disjoint(self):
        registry = json.loads((WORK / 'claims.json').read_text())
        claimed = []
        for packet in registry['claims']:
            self.assertEqual(packet['base'], '76780b3a1b3e26c54b86e1f153344e92dac15b20')
            self.assertEqual(packet['bytes'], sum(t['size'] for t in packet['targets']))
            for target in packet['targets']:
                start = int(target['address'], 16)
                end = int(target['end_exclusive'], 16)
                self.assertEqual(end - start, target['size'])
                self.assertTrue(any(int(r['start'], 16) <= start < end <= int(r['end_exclusive'], 16)
                                    for r in packet['partition_intervals']))
                claimed.append((start, end))
        claimed.sort()
        self.assertEqual(len(claimed), len(set(claimed)))
        self.assertTrue(all(a[1] <= b[0] for a, b in zip(claimed, claimed[1:])))
        self.assertEqual(len(claimed), registry['claimed_functions'])
        self.assertEqual(sum(end - start for start, end in claimed), registry['claimed_bytes'])


class C11SemanticsTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        compiler = shutil.which('cc')
        if compiler is None:
            raise RuntimeError('host C compiler is required for the C11 semantic tests')
        cls.temp = tempfile.TemporaryDirectory(prefix='boot-tail-c11-host-')
        path = Path(cls.temp.name)
        files = [ROOT / ('cloud/matches/boot_tail/func_' + a + '.c')
                 for a in ['8001E740', '8001E768', '8001E930', '8001E9A0']]
        files.append(WORK / 'C11-small/func_8001E790_NONMATCH.c')
        harness = '''
unsigned int D_8002CC50;
static unsigned int allocation_calls, release_calls, observed_size, observed_mode;
static void *observed_pointer;
static char token;
static void *allocate(unsigned int size, unsigned int mode) {
    ++allocation_calls; observed_size = size; observed_mode = mode; return &token;
}
static void release(void *allocation) { ++release_calls; observed_pointer = allocation; }
void *(*D_80038018)(unsigned int, unsigned int) = allocate;
void (*D_8003801C)(void *) = release;
unsigned int call_count(unsigned int which) { return which ? release_calls : allocation_calls; }
unsigned int size_seen(void) { return observed_size; }
unsigned int mode_seen(void) { return observed_mode; }
void *pointer_seen(void) { return observed_pointer; }
void *token_pointer(void) { return &token; }
'''
        (path / 'harness.c').write_text(harness)
        subprocess.run([compiler, '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror',
                        '-shared', '-fPIC', *map(str, files), str(path / 'harness.c'),
                        '-o', str(path / 'test.so')], check=True, capture_output=True, text=True)
        cls.lib = ctypes.CDLL(str(path / 'test.so'))
        cls.lib.func_8001E740.argtypes = [ctypes.c_uint]
        cls.lib.func_8001E740.restype = ctypes.c_void_p
        cls.lib.func_8001E768.argtypes = [ctypes.c_void_p]
        cls.lib.func_8001E768.restype = None
        cls.lib.func_8001E790.restype = ctypes.c_ushort
        cls.lib.func_8001E930.argtypes = [ctypes.POINTER(ctypes.c_uint)]
        cls.lib.func_8001E930.restype = None
        cls.lib.func_8001E9A0.argtypes = [ctypes.c_uint]
        cls.lib.func_8001E9A0.restype = ctypes.c_uint
        for name in ['pointer_seen', 'token_pointer']:
            getattr(cls.lib, name).restype = ctypes.c_void_p
        for name in ['size_seen', 'mode_seen', 'call_count']:
            getattr(cls.lib, name).restype = ctypes.c_uint

    @classmethod
    def tearDownClass(cls):
        cls.temp.cleanup()

    def test_callback_arguments_and_result(self):
        self.assertEqual(ctypes.sizeof(ctypes.c_uint), 4)
        before_alloc = self.lib.call_count(0)
        before_free = self.lib.call_count(1)
        for size in [0, 1, 0xFFFFFFFF]:
            pointer = self.lib.func_8001E740(size)
            self.assertEqual(pointer, self.lib.token_pointer())
            self.assertEqual(self.lib.size_seen(), size)
            self.assertEqual(self.lib.mode_seen(), 0)
            self.lib.func_8001E768(pointer)
            self.assertEqual(self.lib.pointer_seen(), pointer)
        self.assertEqual(self.lib.call_count(0) - before_alloc, 3)
        self.assertEqual(self.lib.call_count(1) - before_free, 3)

    def test_scale_wrap_and_conversion(self):
        for value in [0, 1, 255, 256, 257, 0xFFFFFF, 0x1000000, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF]:
            word = ctypes.c_uint(value)
            self.lib.func_8001E930(ctypes.byref(word))
            self.assertEqual(word.value, (value * 256) & 0xFFFFFFFF)
            self.assertEqual(self.lib.func_8001E9A0(value), value // 256)

    def test_nonmatch_prng_semantics_only(self):
        state = ctypes.c_uint.in_dll(self.lib, 'D_8002CC50')
        for seed in [0, 1, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF]:
            state.value = seed
            expected = seed
            for _ in range(100):
                expected = (expected * 2822053219) & 0xFFFFFFFF
                self.assertEqual(self.lib.func_8001E790(), (expected >> 6) & 0xFFFF)
                self.assertEqual(state.value, expected)
