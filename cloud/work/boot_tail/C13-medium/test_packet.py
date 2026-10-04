"""C89 semantics, packed layout and honest receipt-state checks."""
import ctypes as C
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest

WORK = Path(__file__).resolve().parent
ROOT = WORK.parents[3]


class PacketTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cc = shutil.which('cc')
        if not cc:
            raise RuntimeError('host C compiler required')
        cls.tmp = tempfile.TemporaryDirectory(prefix='c13-medium-host-')
        p = Path(cls.tmp.name)
        source = (ROOT / 'cloud/matches/boot_tail/func_8002106C.c').read_text()
        source += '\n'.join(x.read_text() for x in sorted((WORK/'nonmatch').glob('*.c')))
        source += '''
#include <stddef.h>
unsigned char D_80050D00[8][16][134];
unsigned char D_80055000[32][134];
unsigned char D_80050C60[8][16];
unsigned char D_80050CE0[32];
unsigned char D_800560C0[8][16];
unsigned char D_80056140[32];
typedef char source_size_check[sizeof(ControlSource) == 4 ? 1 : -1];
typedef char input_size_check[sizeof(ControlInput) == 18 ? 1 : -1];
typedef char count_offset_check[offsetof(ControlInput, count) == 16 ? 1 : -1];
typedef char voice_offset_check[offsetof(VoiceControls, input) == 196 ? 1 : -1];
'''
        (p/'harness.c').write_text(source)
        subprocess.run([cc,'-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror',
                        '-shared','-fPIC',str(p/'harness.c'),'-o',str(p/'test.so')],
                       capture_output=True,text=True,check=True)
        cls.lib = C.CDLL(str(p/'test.so'))
        cls.lib.func_80020610.argtypes = [C.c_ubyte]*4
        cls.lib.func_80020610.restype = None
        cls.lib.func_8002106C.argtypes = [C.c_void_p]
        cls.lib.func_8002106C.restype = None
        for n in ['80020F4C','80020FDC']:
            getattr(cls.lib,'func_'+n).argtypes = [C.c_ubyte]*3
            getattr(cls.lib,'func_'+n).restype = None
        for n in ['80020F98','80021028']:
            getattr(cls.lib,'func_'+n).argtypes = [C.c_ubyte]*2
            getattr(cls.lib,'func_'+n).restype = C.c_ubyte

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()

    def array(self, name, n):
        return (C.c_ubyte*n).in_dll(self.lib,name)

    def test_packed_initializer_and_untouched_bytes(self):
        controls = [7,10,131,128,132,1,64,65,91]
        for marker in [0,0xA5,255]:
            for start in [0,1,3]:
                raw = (C.c_ubyte*400)(*([marker]*400))
                expected = bytearray(raw)
                for i,value in enumerate(controls):
                    offset = start+196+18*i
                    expected[offset] = value
                    expected[offset+1] = 0
                    expected[offset+2:offset+4] = (256).to_bytes(2,sys.byteorder)
                    expected[offset+16] = 1
                self.lib.func_8002106C(C.addressof(raw)+start)
                self.assertEqual(bytes(raw),bytes(expected))

    def test_byte_table_pairs(self):
        for setter,getter,table,fx in [('80020F4C','80020F98','D_80050C60','D_80050CE0'),
                                       ('80020FDC','80021028','D_800560C0','D_80056140')]:
            primary = self.array(table,128)
            effects = self.array(fx,32)
            for set_no,channels in [(0,[0,1,15]),(3,[0,1,15]),(7,[0,1,15]),(255,[0,15,31])]:
                for channel in channels:
                    for value in [0,1,127,128,255]:
                        C.memset(primary,0xA5,128); C.memset(effects,0x5A,32)
                        p,f = bytearray(primary),bytearray(effects)
                        (f if set_no==255 else p)[channel if set_no==255 else set_no*16+channel] = value
                        getattr(self.lib,'func_'+setter)(channel,set_no,value)
                        self.assertEqual(bytes(primary),bytes(p)); self.assertEqual(bytes(effects),bytes(f))
                        self.assertEqual(getattr(self.lib,'func_'+getter)(channel,set_no),value)

    def test_controller_table_mask_and_routing(self):
        primary = self.array('D_80050D00',8*16*134)
        effects = self.array('D_80055000',32*134)
        for set_no,channels in [(0,[0,7,15]),(3,[0,7,15]),(7,[0,7,15]),(255,[0,15,31])]:
            for channel in channels:
                for control in [0,7,64,127,133]:
                    for value in [0,127,128,255]:
                        C.memset(primary,0xA5,len(primary)); C.memset(effects,0x5A,len(effects))
                        p,f = bytearray(primary),bytearray(effects)
                        index = channel*134+control if set_no==255 else (set_no*16+channel)*134+control
                        (f if set_no==255 else p)[index] = value & 127
                        self.lib.func_80020610(control,channel,set_no,value)
                        self.assertEqual(bytes(primary),bytes(p)); self.assertEqual(bytes(effects),bytes(f))

    def test_receipt_hashes_and_honest_states(self):
        receipt = json.loads((WORK/'scores.json').read_text())
        self.assertEqual(len(receipt['results']),20)
        matches = [r for r in receipt['results'] if r['strict_match']]
        self.assertEqual(len(matches),1)
        self.assertEqual(matches[0]['name'],'func_8002106C')
        self.assertEqual(matches[0]['target_bytes'],228)
        self.assertEqual(matches[0]['object_text_bytes'],240)
        self.assertEqual(matches[0]['extra_words'],0)
        self.assertTrue(matches[0]['full_relocated_equality'])
        self.assertEqual(matches[0]['unresolved']+matches[0]['unverified']+matches[0]['errors'],[])
        self.assertEqual(matches[0]['masked_relocations'],0)
        for r in receipt['results']:
            self.assertEqual(hashlib.sha256((ROOT/r['source_path']).read_bytes()).hexdigest(),r['source_sha256'])
        for path,digest in receipt['source_hashes'].items():
            self.assertEqual(hashlib.sha256((ROOT/path).read_bytes()).hexdigest(),digest)


if __name__ == '__main__':
    unittest.main()
