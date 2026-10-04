"""Compare complete C89 bodies with canonical MIPS and independent models.
Fixture extents (8 regular sets, 64 effects channels) are synthetic storage,
not a claim about native overall allocation bounds. C getter inputs stay inside
134-byte rows and the regular 16-channel stride. No table bytes are imported.
"""
import ctypes as C
import importlib.util
from pathlib import Path
import random
import subprocess
import sys
import tempfile
import unittest

WORK = Path(__file__).resolve().parent
ROOT = WORK.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
spec = importlib.util.spec_from_file_location('controller_native',WORK/'native_replay.py')
native = importlib.util.module_from_spec(spec)
spec.loader.exec_module(native)


def model_set(controller,channel,set_no,value):
    if controller < 64: first,second = controller&31,(controller&31)+32
    elif controller in (128,129,132,133): first,second = controller&254,(controller&254)+1
    else: return [(controller,channel,set_no,(value>>7)&255)]
    return [(first,channel,set_no,(value>>7)&255),(second,channel,set_no,value&127)]


def model_get(controller,channel,set_no,midi,effects):
    data = effects if set_no == 255 else midi
    offset = channel*134 if set_no == 255 else set_no*2144+channel*134
    if controller < 64:
        base = offset+(controller&31)
        return (data[base]<<7)|data[base+32], [base,base+32]
    if controller in (128,129,132,133):
        base = offset+(controller&254)
        return (data[base]<<7)|data[base+1], [base,base+1]
    if controller < 70: return 0 if data[offset+controller] < 64 else 16383,[offset+controller]
    if 96 <= controller <= 101: return 0,[]
    return data[offset+controller]<<7,[offset+controller]


class ControllerSemantics(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.TemporaryDirectory(prefix='c13-controller-host-')
        p = Path(cls.tmp.name)
        stub = p/'fixture.c'
        stub.write_text('''
typedef unsigned char u8;
u8 D_80050D00[8][16][134];
u8 D_80055000[64][134];
unsigned int calls[2][4];
unsigned int count;
void reset_calls(void) { count = 0; }
void func_80020610(u8 a, u8 b, u8 c, u8 d) {
    if (count < 2) {
        calls[count][0]=a; calls[count][1]=b;
        calls[count][2]=c; calls[count][3]=d;
    }
    ++count;
}
''')
        cls.libs = []
        source_sets = [
            ('retained',WORK/'nonmatch/func_800206AC.c',WORK/'nonmatch/func_80020A04.c'),
            ('controls',WORK/'variants/func_800206AC_initial.c',WORK/'variants/func_80020A04_repeated_arrays.c')]
        for label,setter,getter in source_sets:
            libpath = p/(label+'.so')
            subprocess.run(['cc','-std=c89','-pedantic','-Wall','-Wextra','-Werror','-shared','-fPIC',str(setter),str(getter),str(stub),'-o',str(libpath)],check=True)
            lib = C.CDLL(str(libpath))
            lib.func_800206AC.argtypes = [C.c_ubyte,C.c_ubyte,C.c_ubyte,C.c_ushort]
            lib.func_800206AC.restype = None
            lib.func_80020A04.argtypes = [C.c_ubyte]*3
            lib.func_80020A04.restype = C.c_ushort
            cls.libs.append(lib)
        score.ASM_DIR = ROOT/'asm/us/boot_tail'
        cls.setter = score.targets()['func_800206AC']
        cls.getter = score.targets()['func_80020A04']

    @classmethod
    def tearDownClass(cls): cls.tmp.cleanup()

    def test_setter_all_selectors_values_and_call_order(self):
        cases = 0
        for controller in range(256):
            for channel,set_no in ((0,0),(15,7),(63,255),(255,255)):
                for value in (0,1,127,128,129,8191,8192,16383,16384,32768,65535):
                    expected = model_set(controller,channel,set_no,value)
                    # Dirty high bits test the native entry normalization too.
                    replay = native.execute(self.setter,0x800206AC,
                        [0xBEEF0000|controller,0xABCD0000|channel,
                         0xFACE0000|set_no,0xABCD0000|value])
                    self.assertEqual(replay['calls'],expected)
                    self.assertFalse(replay['reads'])
                    for lib in self.libs:
                        lib.reset_calls()
                        lib.func_800206AC(controller,channel,set_no,value)
                        self.assertEqual(C.c_uint.in_dll(lib,'count').value,len(expected))
                        calls = ((C.c_uint*4)*2).in_dll(lib,'calls')
                        self.assertEqual([tuple(calls[i]) for i in range(len(expected))],expected)
                    cases += 1
        self.assertEqual(cases,11264)

    def test_getter_full_row_domain_arbitrary_synthetic_data(self):
        rng = random.Random(2049)
        patterns = [bytes([v])*(8*16*134) for v in (0,63,64,127,255)]
        patterns.append(bytes(rng.randrange(256) for _ in range(8*16*134)))
        cases = 0
        for midi in patterns:
            effects = midi[:64*134]
            for lib in self.libs:
                C.memmove((C.c_ubyte*len(midi)).in_dll(lib,'D_80050D00'),midi,len(midi))
                C.memmove((C.c_ubyte*len(effects)).in_dll(lib,'D_80055000'),effects,len(effects))
            before = [(bytes((C.c_ubyte*len(midi)).in_dll(lib,'D_80050D00')),
                       bytes((C.c_ubyte*len(effects)).in_dll(lib,'D_80055000'))) for lib in self.libs]
            for set_no in (0,1,7,255):
                channels = (0,1,7,15) if set_no != 255 else (0,1,15,31,63)
                for channel in channels:
                    for controller in range(134):
                        expected,offsets = model_get(controller,channel,set_no,midi,effects)
                        replay = native.execute(self.getter,0x80020A04,
                            [0xBEEF0000|controller,0xABCD0000|channel,0xFACE0000|set_no],midi,effects)
                        self.assertEqual(replay['result'],expected)
                        base = native.EFFECTS if set_no == 255 else native.MIDI
                        self.assertEqual(sorted(replay['reads']),sorted(base+off for off in offsets))
                        self.assertFalse(replay['calls'])
                        for lib in self.libs:
                            self.assertEqual(lib.func_80020A04(controller,channel,set_no),expected)
                        cases += 1
            for lib,prior in zip(self.libs,before):
                self.assertEqual(bytes((C.c_ubyte*len(midi)).in_dll(lib,'D_80050D00')),prior[0])
                self.assertEqual(bytes((C.c_ubyte*len(effects)).in_dll(lib,'D_80055000')),prior[1])
        self.assertEqual(cases,13668)

    def test_interpreter_rejects_unmapped_and_unknown(self):
        with self.assertRaises(ValueError): native.execute([0xFFFFFFFF],0x80000000,[])
        with self.assertRaises(ValueError): native.execute(self.getter,0x80020A04,[7,0,0])


if __name__ == '__main__': unittest.main()
